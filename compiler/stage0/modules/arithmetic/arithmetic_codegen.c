#include "arithmetic_codegen.h"
#include "../../../../runtime/tto/runtime_tto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Register counter for temporary values
static int reg_counter = 0;
static int overflow_label_counter = 0;

// Generate assembly for loading a value into register
static void generate_load(FILE* output, const char* value, int reg_num, int is_float, void* context) {
    // If context provided, get actual stack offset
    if (context) {
        // context is FunctionDeclaration*
        typedef struct FunctionDeclaration FunctionDeclaration;
        extern int function_get_var_offset(FunctionDeclaration* func, const char* var_name);
        
        FunctionDeclaration* func = (FunctionDeclaration*)context;
        int offset = function_get_var_offset(func, value);
        
        if (is_float) {
            fprintf(output, "    movsd xmm%d, %d(%%rbp)\n", reg_num, offset);
        } else {
            fprintf(output, "    movq %d(%%rbp), %%r%d  # Load %s\n", offset, reg_num + 8, value);
        }
    } else {
        // Fallback: symbolic reference (old behavior)
        if (is_float) {
            fprintf(output, "    movsd xmm%d, [%s]\n", reg_num, value);
        } else {
            fprintf(output, "    mov r%d, [%s]\n", reg_num + 8, value);
        }
    }
}

// Generate assembly for a literal
static void generate_literal(FILE* output, const char* value, int reg_num, int is_float) {
    if (is_float) {
        // For floats, we need to define data and load it
        fprintf(output, "    section .data\n");
        fprintf(output, "    _float_%d: dq %s\n", reg_counter++, value);
        fprintf(output, "    section .text\n");
        fprintf(output, "    movsd xmm%d, [_float_%d]\n", reg_num, reg_counter - 1);
    } else {
        fprintf(output, "    mov r%d, %s\n", reg_num + 8, value);  // ✅ Intel syntax: mov reg, imm
    }
}

// Generate code recursively (postorder traversal)
static void generate_expr_code(FILE* output, ArithmeticExpr* expr, int target_reg, void* context) {
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
            generate_expr_code(output, call->arguments[i], 2, context);  // Use r10 (reg 2)
            
            // Move result from r10 to parameter register
            fprintf(output, "    movq %%r10, %s  # Argument %d\n", param_regs[i], i + 1);
        }
        
        // TODO: Stack arguments for more than 6 parameters
        if (call->arg_count > max_reg_params) {
            fprintf(stderr, "Warning: Functions with >6 parameters not yet supported\n");
        }
        
        // Call the function
        fprintf(output, "    call %s\n", call->function_name);
        
        // Return value is in rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Function return value\n", target_reg + 8);
        
        return;
    }
    
    // Leaf node (literal or variable)
    if (expr->is_literal || (!expr->left && !expr->right)) {
        if (expr->is_literal) {
            generate_literal(output, expr->value, target_reg, expr->is_float);
        } else {
            generate_load(output, expr->value, target_reg, expr->is_float, context);
        }
        return;
    }
    
    // Binary operation
    int left_reg = target_reg;
    int right_reg = target_reg + 1;
    
    // Generate code for left subtree
    generate_expr_code(output, expr->left, left_reg, context);
    
    // Generate code for right subtree
    generate_expr_code(output, expr->right, right_reg, context);
    
    // Determine if we're dealing with floats
    int is_float = (expr->left && expr->left->is_float) || 
                   (expr->right && expr->right->is_float);
    
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
                fprintf(output, "    sub r%d, r%d\n", left_reg + 8, right_reg + 8);
                // Check overflow flag
                fprintf(output, "    jo .overflow_detected_%d\n", overflow_label);
                fprintf(output, "    jmp .no_overflow_%d\n", overflow_label);
                fprintf(output, ".overflow_detected_%d:\n", overflow_label);
                fprintf(output, "    # Overflow detected - promote to BigDecimal\n");
                fprintf(output, "    push rdi\n");
                fprintf(output, "    push rsi\n");
                fprintf(output, "    mov rdi, r%d\n", left_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    mov rdi, rax\n");
                fprintf(output, "    mov rsi, r%d\n", right_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    mov rsi, rax\n");
                fprintf(output, "    call tto_bigdec_sub\n");
                fprintf(output, "    pop rsi\n");
                fprintf(output, "    pop rdi\n");
                fprintf(output, "    mov r%d, rax\n", left_reg + 8);
                fprintf(output, ".no_overflow_%d:\n", overflow_label);
                break;
            case ARITH_MUL:
                fprintf(output, "    imul r%d, r%d\n", left_reg + 8, right_reg + 8);
                // Check overflow flag
                fprintf(output, "    jo .overflow_detected_%d\n", overflow_label);
                fprintf(output, "    jmp .no_overflow_%d\n", overflow_label);
                fprintf(output, ".overflow_detected_%d:\n", overflow_label);
                fprintf(output, "    # Overflow detected - promote to BigDecimal\n");
                fprintf(output, "    push rdi\n");
                fprintf(output, "    push rsi\n");
                fprintf(output, "    mov rdi, r%d\n", left_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    mov rdi, rax\n");
                fprintf(output, "    mov rsi, r%d\n", right_reg + 8);
                fprintf(output, "    call tto_bigdec_from_int64\n");
                fprintf(output, "    mov rsi, rax\n");
                fprintf(output, "    call tto_bigdec_mul\n");
                fprintf(output, "    pop rsi\n");
                fprintf(output, "    pop rdi\n");
                fprintf(output, "    mov r%d, rax\n", left_reg + 8);
                fprintf(output, ".no_overflow_%d:\n", overflow_label);
                break;
            case ARITH_DIV:
                // Division requires rax/rdx setup
                fprintf(output, "    mov rax, r%d\n", left_reg + 8);
                fprintf(output, "    cqo\n");  // Sign extend rax to rdx:rax
                fprintf(output, "    idiv r%d\n", right_reg + 8);
                fprintf(output, "    mov r%d, rax\n", left_reg + 8);
                break;
            case ARITH_MOD:
                // Modulo is remainder from division
                fprintf(output, "    mov rax, r%d\n", left_reg + 8);
                fprintf(output, "    cqo\n");
                fprintf(output, "    idiv r%d\n", right_reg + 8);
                fprintf(output, "    mov r%d, rdx\n", left_reg + 8);  // Remainder in rdx
                break;
            case ARITH_POW:
                // Power - repeated multiplication (base^exponent)
                // Save registers and use rcx as counter
                fprintf(output, "    push rcx\n");
                fprintf(output, "    mov rcx, r%d\n", right_reg + 8);  // exponent in rcx
                fprintf(output, "    mov rax, 1\n");  // result starts at 1
                fprintf(output, "    test rcx, rcx\n");  // check if exponent is 0
                fprintf(output, "    jz .pow_done_%d\n", left_reg);
                fprintf(output, ".pow_loop_%d:\n", left_reg);
                fprintf(output, "    imul rax, r%d\n", left_reg + 8);  // result *= base
                fprintf(output, "    dec rcx\n");
                fprintf(output, "    jnz .pow_loop_%d\n", left_reg);
                fprintf(output, ".pow_done_%d:\n", left_reg);
                fprintf(output, "    mov r%d, rax\n", left_reg + 8);
                fprintf(output, "    pop rcx\n");
                break;
            case ARITH_AND:
                fprintf(output, "    and r%d, r%d\n", left_reg + 8, right_reg + 8);
                break;
            case ARITH_OR:
                fprintf(output, "    or r%d, r%d\n", left_reg + 8, right_reg + 8);
                break;
            case ARITH_XOR:
                fprintf(output, "    xor r%d, r%d\n", left_reg + 8, right_reg + 8);
                break;
        }
    }
}

// Generate assembly for arithmetic expression
void arithmetic_generate_code(FILE* output, ArithmeticExpr* expr, void* context) {
    if (!output || !expr) return;
    
    fprintf(output, "\n    # Arithmetic expression\n");
    generate_expr_code(output, expr, 0, context);  // Start with register 0
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
        fprintf(output, "    movsd [%s], xmm0\n", var_name);
    } else {
        fprintf(output, "    mov [%s], r8\n", var_name);
    }
}
