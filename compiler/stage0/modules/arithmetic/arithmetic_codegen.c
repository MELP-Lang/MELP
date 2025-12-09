#include "arithmetic_codegen.h"
#include "../../../../runtime/tto/runtime_tto.h"
#include "../functions/functions.h"  // For FunctionDeclaration type
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Register counter for temporary values
static int reg_counter = 0;
static int overflow_label_counter = 0;

// Generate assembly for loading a value into register
static void generate_load(FILE* output, const char* value, int reg_num, int is_float, FunctionDeclaration* func) {
    // If func provided, get actual stack offset
    if (func) {
        int offset = function_get_var_offset(func, value);
        
        if (is_float) {
            fprintf(output, "    movsd %d(%%rbp), %%xmm%d\n", offset, reg_num);
        } else {
            fprintf(output, "    movq %d(%%rbp), %%r%d  # Load %s\n", offset, reg_num + 8, value);
        }
    } else {
        // Fallback: symbolic reference (old behavior)
        if (is_float) {
            fprintf(output, "    movsd %s(%%rip), %%xmm%d\n", value, reg_num);
        } else {
            fprintf(output, "    movq %s(%%rip), %%r%d\n", value, reg_num + 8);
        }
    }
}

// Generate assembly for a literal
static void generate_literal(FILE* output, const char* value, int reg_num, int is_float) {
    if (is_float) {
        // For floats, we need to define data and load it
        fprintf(output, "    .section .data\n");
        fprintf(output, "    _float_%d: .quad %s\n", reg_counter++, value);
        fprintf(output, "    .section .text\n");
        fprintf(output, "    movsd _float_%d(%%rip), %%xmm%d\n", reg_counter - 1, reg_num);
    } else {
        fprintf(output, "    movq $%s, %%r%d  # Literal\n", value, reg_num + 8);
    }
}

// Generate code recursively (postorder traversal)
static void generate_expr_code(FILE* output, ArithmeticExpr* expr, int target_reg, FunctionDeclaration* func) {
    if (!expr) return;
    
    // Phase 3.5: Function call
    if (expr->is_function_call && expr->func_call) {
        FunctionCallExpr* call = expr->func_call;
        
        fprintf(output, "    # Function call: %s()\n", call->function_name);
        
        // x86-64 calling convention registers: rdi, rsi, rdx, rcx, r8, r9
        const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        const int max_reg_params = 6;
        
        // Evaluate and move arguments to appropriate registers
        for (int i = 0; i < call->arg_count && i < max_reg_params; i++) {
            // Evaluate argument expression to r10 (temporary)
            generate_expr_code(output, call->arguments[i], 2, func);  // Use r10 (reg 2)
            
            // Move result from r10 to parameter register
            fprintf(output, "    movq %%r10, %s  # Argument %d\n", param_regs[i], i + 1);
        }
        
        // TODO: Stack arguments for more than 6 parameters
        if (call->arg_count > max_reg_params) {
            fprintf(stderr, "Warning: Functions with >6 parameters not yet supported\n");
        }
        
        // Check if builtin stdlib function
        const char* actual_function = call->function_name;
        if (strcmp(call->function_name, "println") == 0) {
            // TTO: Check if argument is numeric by looking up in function context
            int is_numeric_arg = 1;  // Default: numeric
            if (call->arg_count > 0 && call->arguments[0] && func) {
                ArithmeticExpr* arg_expr = call->arguments[0];
                if (arg_expr && !arg_expr->left && !arg_expr->right && arg_expr->value && !arg_expr->is_literal) {
                    // Variable reference - lookup type from function context
                    is_numeric_arg = function_get_var_is_numeric(func, arg_expr->value);
                }
            }
            
            if (is_numeric_arg) {
                // Numeric argument - TTO-aware: Pass pointer to value + type
                // Value is in %rdi, need to save to stack and pass pointer
                fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
                fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
                fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
                fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
                actual_function = "mlp_println_numeric";
            } else {
                // Text argument - direct call
                actual_function = "mlp_println_string";
            }
        } else if (strcmp(call->function_name, "print") == 0) {
            fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
            fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
            fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
            fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
            actual_function = "mlp_print_numeric";
        } else if (strcmp(call->function_name, "toString") == 0) {
            fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
            fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
            fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
            fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
            actual_function = "mlp_toString_numeric";
        }
        
        // Call the function
        fprintf(output, "    call %s\n", actual_function);
        
        // Clean up temp space for TTO calls
        if (strcmp(call->function_name, "println") == 0 ||
            strcmp(call->function_name, "print") == 0 ||
            strcmp(call->function_name, "toString") == 0) {
            fprintf(output, "    addq $16, %%rsp      # Clean up temp space\n");
        }
        
        // Return value is in rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Function return value\n", target_reg + 8);
        
        return;
    }
    
    // Leaf node (literal or variable)
    if (expr->is_literal || (!expr->left && !expr->right)) {
        if (expr->is_literal) {
            if (expr->is_string) {
                // String literal - define in .rodata and load pointer
                static int string_counter = 0;
                fprintf(output, "    .section .rodata\n");
                fprintf(output, "    _str_%d: .asciz %s\n", string_counter, expr->value);
                fprintf(output, "    .section .text\n");
                fprintf(output, "    leaq _str_%d(%%rip), %%r%d  # Load string pointer\n", 
                        string_counter, target_reg + 8);
                string_counter++;
            } else {
                generate_literal(output, expr->value, target_reg, expr->is_float);
            }
        } else {
            generate_load(output, expr->value, target_reg, expr->is_float, func);
        }
        return;
    }
    
    // Binary operation
    int left_reg = target_reg;
    int right_reg = target_reg + 1;
    
    // Generate code for left subtree
    generate_expr_code(output, expr->left, left_reg, func);
    
    // Generate code for right subtree
    generate_expr_code(output, expr->right, right_reg, func);
    
    // Determine if we're dealing with floats or strings
    int is_float = (expr->left && expr->left->is_float) || 
                   (expr->right && expr->right->is_float);
    int is_string = (expr->left && expr->left->is_string) || 
                    (expr->right && expr->right->is_string);
    
    // Handle string concatenation
    if (is_string && expr->op == ARITH_ADD) {
        fprintf(output, "    # String concatenation\n");
        fprintf(output, "    movq %%r%d, %%rdi  # First string\n", left_reg + 8);
        fprintf(output, "    movq %%r%d, %%rsi  # Second string\n", right_reg + 8);
        fprintf(output, "    call tto_sso_concat\n");
        fprintf(output, "    movq %%rax, %%r%d  # Result string pointer\n", left_reg + 8);
        return;
    }
    
    // Generate operation
    if (is_float) {
        // Floating point operations
        switch (expr->op) {
            case ARITH_ADD:
                fprintf(output, "    addsd xmm%d, xmm%d\n", left_reg, right_reg);
                break;
            case ARITH_SUB:
                fprintf(output, "    subsd xmm%d, xmm%d\n", left_reg, right_reg);
                break;
            case ARITH_MUL:
                fprintf(output, "    mulsd xmm%d, xmm%d\n", left_reg, right_reg);
                break;
            case ARITH_DIV:
                fprintf(output, "    divsd xmm%d, xmm%d\n", left_reg, right_reg);
                break;
            case ARITH_MOD:
                fprintf(stderr, "Error: Modulo not supported for floating point\n");
                break;
            case ARITH_POW:
                // Power for floats - need to call math library
                fprintf(stderr, "Error: Power not yet implemented for floating point\n");
                break;
            case ARITH_AND:
            case ARITH_OR:
            case ARITH_XOR:
                fprintf(stderr, "Error: Bitwise operations not supported for floating point\n");
                break;
        }
    } else {
        // Integer operations with overflow detection
        int overflow_label = overflow_label_counter++;
        
        switch (expr->op) {
            case ARITH_ADD:
                fprintf(output, "    addq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                // Check overflow flag
                fprintf(output, "    jo .overflow_detected_%d\n", overflow_label);
                fprintf(output, "    jmp .no_overflow_%d\n", overflow_label);
                fprintf(output, ".overflow_detected_%d:\n", overflow_label);
                fprintf(output, "    # Overflow detected - promote to BigDecimal\n");
                fprintf(output, "    pushq %%rdi\n");
                fprintf(output, "    pushq %%rsi\n");
                fprintf(output, "    movq %%r%d, %%rdi\n", left_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call tto_bigdec_add\n");
                fprintf(output, "    popq %%rsi\n");
                fprintf(output, "    popq %%rdi\n");
                fprintf(output, "    movq %%rax, %%r%d # BigDecimal pointer in result register\n", left_reg + 8);
                fprintf(output, ".no_overflow_%d:\n", overflow_label);
                break;
            case ARITH_SUB:
                fprintf(output, "    subq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                // Check overflow flag
                fprintf(output, "    jo .overflow_detected_%d\n", overflow_label);
                fprintf(output, "    jmp .no_overflow_%d\n", overflow_label);
                fprintf(output, ".overflow_detected_%d:\n", overflow_label);
                fprintf(output, "    # Overflow detected - promote to BigDecimal\n");
                fprintf(output, "    pushq %%rdi\n");
                fprintf(output, "    pushq %%rsi\n");
                fprintf(output, "    movq %%r%d, %%rdi\n", left_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call tto_bigdec_sub\n");
                fprintf(output, "    popq %%rsi\n");
                fprintf(output, "    popq %%rdi\n");
                fprintf(output, "    movq %%rax, %%r%d # BigDecimal pointer in result register\n", left_reg + 8);
                fprintf(output, ".no_overflow_%d:\n", overflow_label);
                break;
            case ARITH_MUL:
                fprintf(output, "    imulq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                // Check overflow flag
                fprintf(output, "    jo .overflow_detected_%d\n", overflow_label);
                fprintf(output, "    jmp .no_overflow_%d\n", overflow_label);
                fprintf(output, ".overflow_detected_%d:\n", overflow_label);
                fprintf(output, "    # Overflow detected - promote to BigDecimal\n");
                fprintf(output, "    pushq %%rdi\n");
                fprintf(output, "    pushq %%rsi\n");
                fprintf(output, "    movq %%r%d, %%rdi\n", left_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call tto_bigdec_mul\n");
                fprintf(output, "    popq %%rsi\n");
                fprintf(output, "    popq %%rdi\n");
                fprintf(output, "    movq %%rax, %%r%d # BigDecimal pointer in result register\n", left_reg + 8);
                fprintf(output, ".no_overflow_%d:\n", overflow_label);
                break;
            case ARITH_DIV:
                // Division requires rax/rdx setup
                fprintf(output, "    movq %%r%d, %%rax\n", left_reg + 8);
                fprintf(output, "    cqo\n");  // Sign extend rax to rdx:rax
                fprintf(output, "    idivq %%r%d\n", right_reg + 8);
                fprintf(output, "    movq %%rax, %%r%d\n", left_reg + 8);
                break;
            case ARITH_MOD:
                // Modulo is remainder from division
                fprintf(output, "    movq %%r%d, %%rax\n", left_reg + 8);
                fprintf(output, "    cqo\n");
                fprintf(output, "    idivq %%r%d\n", right_reg + 8);
                fprintf(output, "    movq %%rdx, %%r%d  # Remainder\n", left_reg + 8);
                break;
            case ARITH_POW:
                // Power - repeated multiplication (base^exponent)
                // Save registers and use rcx as counter
                fprintf(output, "    pushq %%rcx\n");
                fprintf(output, "    movq %%r%d, %%rcx\n", right_reg + 8);  // exponent in rcx
                fprintf(output, "    movq $1, %%rax\n");  // result starts at 1
                fprintf(output, "    testq %%rcx, %%rcx\n");  // check if exponent is 0
                fprintf(output, "    jz .pow_done_%d\n", left_reg);
                fprintf(output, ".pow_loop_%d:\n", left_reg);
                fprintf(output, "    imulq %%r%d, %%rax\n", left_reg + 8);  // result *= base
                fprintf(output, "    decq %%rcx\n");
                fprintf(output, "    jnz .pow_loop_%d\n", left_reg);
                fprintf(output, ".pow_done_%d:\n", left_reg);
                fprintf(output, "    movq %%rax, %%r%d\n", left_reg + 8);
                fprintf(output, "    popq %%rcx\n");
                break;
            case ARITH_AND:
                fprintf(output, "    andq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                break;
            case ARITH_OR:
                fprintf(output, "    orq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                break;
            case ARITH_XOR:
                fprintf(output, "    xorq %%r%d, %%r%d\n", right_reg + 8, left_reg + 8);  // AT&T: src, dest
                break;
        }
    }
}

// Generate assembly for arithmetic expression
void arithmetic_generate_code(FILE* output, ArithmeticExpr* expr, FunctionDeclaration* func) {
    if (!output || !expr) return;
    
    fprintf(output, "\n    # Arithmetic expression\n");
    generate_expr_code(output, expr, 0, func);  // Start with register 0
    fprintf(output, "    # Result in r8 (integer) or xmm0 (float)\n");
}

// Generate assignment statement: variable = expression
void arithmetic_generate_assignment(FILE* output, const char* var_name, ArithmeticExpr* expr) {
    if (!output || !var_name || !expr) return;
    
    fprintf(output, "\n    # Assignment: %s = <expression>\n", var_name);
    
    // Generate expression code (result in r8 or xmm0)
    generate_expr_code(output, expr, 0, NULL);  // No context for legacy function
    
    // Store result to variable
    int is_float = (expr->left && expr->left->is_float) || 
                   (expr->right && expr->right->is_float) ||
                   (expr->is_literal && expr->is_float);
    
    if (is_float) {
        fprintf(output, "    movsd %%xmm0, %s(%%rip)\n", var_name);
    } else {
        fprintf(output, "    movq %%r8, %s(%%rip)\n", var_name);
    }
}
