#include "arithmetic_codegen.h"
#include "../../../../runtime/tto/runtime_tto.h"
#include "../functions/functions.h"  // For FunctionDeclaration type
#include "../array/array_codegen.h"  // YZ_14: For array access codegen
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
    
    // YZ_48: Simple println() codegen for for-loop support
    if (expr->is_function_call && expr->func_call && 
        strcmp(expr->func_call->function_name, "println") == 0 && 
        expr->func_call->arg_count == 1) {
        fprintf(output, "    # println() call\n");
        // Evaluate argument expression → r8
        generate_expr_code(output, expr->func_call->arguments[0], 0, func);
        // Call TTO print function + manual newline
        fprintf(output, "    movq %%r8, %%rdi\n");
        fprintf(output, "    call tto_print_int64\n");
        fprintf(output, "    movq $10, %%rdi  # newline char\n");
        fprintf(output, "    call putchar\n");
        return;
    }
    
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
        } else if (strcmp(call->function_name, "length") == 0) {
            // YZ_22: length(text) -> mlp_string_length(char*)
            actual_function = "mlp_string_length";
        } else if (strcmp(call->function_name, "substring") == 0) {
            // YZ_22: substring(text, start, len) -> mlp_string_substring(char*, size_t, size_t)
            actual_function = "mlp_string_substring";
        } else if (strcmp(call->function_name, "indexOf") == 0) {
            // YZ_22: indexOf(text, substr) -> mlp_string_indexOf(char*, char*)
            actual_function = "mlp_string_indexOf";
        } else if (strcmp(call->function_name, "toUpperCase") == 0) {
            // YZ_29: toUpperCase(text) -> mlp_string_toUpperCase(char*)
            actual_function = "mlp_string_toUpperCase";
        } else if (strcmp(call->function_name, "toLowerCase") == 0) {
            // YZ_29: toLowerCase(text) -> mlp_string_toLowerCase(char*)
            actual_function = "mlp_string_toLowerCase";
        } else if (strcmp(call->function_name, "trim") == 0) {
            // YZ_29: trim(text) -> mlp_string_trim(char*)
            actual_function = "mlp_string_trim";
        } else if (strcmp(call->function_name, "trimStart") == 0) {
            // YZ_29: trimStart(text) -> mlp_string_trimStart(char*)
            actual_function = "mlp_string_trimStart";
        } else if (strcmp(call->function_name, "trimEnd") == 0) {
            // YZ_29: trimEnd(text) -> mlp_string_trimEnd(char*)
            actual_function = "mlp_string_trimEnd";
        } else if (strcmp(call->function_name, "input") == 0) {
            // YZ_31: input() or input(prompt) -> mlp_input() or mlp_input_prompt()
            if (call->arg_count == 0) {
                actual_function = "mlp_input";
            } else {
                actual_function = "mlp_input_prompt";
            }
        } else if (strcmp(call->function_name, "input_numeric") == 0) {
            // YZ_31: input_numeric() or input_numeric(prompt) -> mlp_input_numeric() or mlp_input_numeric_prompt()
            if (call->arg_count == 0) {
                actual_function = "mlp_input_numeric";
            } else {
                actual_function = "mlp_input_numeric_prompt";
            }
        } else if (strcmp(call->function_name, "replace") == 0) {
            // YZ_31: replace(str, old, new) -> mlp_string_replace(char*, char*, char*)
            actual_function = "mlp_string_replace";
        } else if (strcmp(call->function_name, "replaceAll") == 0) {
            // YZ_31: replaceAll(str, old, new) -> mlp_string_replaceAll(char*, char*, char*)
            actual_function = "mlp_string_replaceAll";
        } else if (strcmp(call->function_name, "split") == 0) {
            // YZ_31: split(str, delimiter) -> mlp_string_split(char*, char*, int*)
            // Note: split returns array, need special handling for list
            actual_function = "mlp_string_split";
        } else if (strcmp(call->function_name, "abs") == 0) {
            // YZ_40: abs(x) -> abs(x) from math.h - Note: stdlib abs works for int64
            // For float use fabs from <math.h>
            actual_function = "abs";
        } else if (strcmp(call->function_name, "pow") == 0) {
            // YZ_40: pow(x, y) -> pow(x, y) from <math.h>
            actual_function = "pow";
        } else if (strcmp(call->function_name, "sqrt") == 0) {
            // YZ_40: sqrt(x) -> sqrt(x) from <math.h>
            actual_function = "sqrt";
        } else if (strcmp(call->function_name, "sin") == 0) {
            // YZ_40: sin(x) -> sin(x) from <math.h>
            actual_function = "sin";
        } else if (strcmp(call->function_name, "cos") == 0) {
            // YZ_40: cos(x) -> cos(x) from <math.h>
            actual_function = "cos";
        } else if (strcmp(call->function_name, "tan") == 0) {
            // YZ_40: tan(x) -> tan(x) from <math.h>
            actual_function = "tan";
        } else if (strcmp(call->function_name, "floor") == 0) {
            // YZ_40: floor(x) -> floor(x) from <math.h>
            actual_function = "floor";
        } else if (strcmp(call->function_name, "ceil") == 0) {
            // YZ_40: ceil(x) -> ceil(x) from <math.h>
            actual_function = "ceil";
        } else if (strcmp(call->function_name, "round") == 0) {
            // YZ_40: round(x) -> round(x) from <math.h>
            actual_function = "round";
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
    
    // YZ_14: Array/Tuple/List index access
    if (expr->is_array_access && expr->array_access) {
        IndexAccess* access = expr->array_access;
        
        // YZ_21: Check if this is a tuple access (must check BEFORE array handling)
        if (func && function_is_tuple(func, access->collection_name)) {
            fprintf(output, "    # Tuple index access: %s[...]\n", access->collection_name);
            
            // Get stack offset for tuple variable
            int offset = function_get_var_offset(func, access->collection_name);
            fprintf(output, "    movq %d(%%rbp), %%rdi  # Load tuple pointer (arg1)\n", offset);
            
            // Load index into rsi (arg2)
            if (access->index_type == 0) {
                // Constant index
                fprintf(output, "    movq $%d, %%rsi  # Constant index (arg2)\n", access->index.const_index);
            } else if (access->index_type == 1) {
                // Variable index
                int idx_offset = function_get_var_offset(func, access->index.var_index);
                fprintf(output, "    movq %d(%%rbp), %%rsi  # Variable index (arg2)\n", idx_offset);
            } else if (access->index_type == 2) {
                // Expression index
                ArithmeticExpr* idx_expr = (ArithmeticExpr*)access->index.expr_index;
                fprintf(output, "    # Evaluate expression index\n");
                generate_expr_code(output, idx_expr, 0, func);  // result in r8
                fprintf(output, "    movq %%r8, %%rsi  # Expression index (arg2)\n");
            }
            
            // Call tto_tuple_get(tuple_ptr, index)
            fprintf(output, "    call tto_tuple_get  # Returns element pointer in rax\n");
            
            // Dereference to get actual value (assuming numeric for now)
            fprintf(output, "    movq (%%rax), %%rax  # Dereference to get value\n");
            
            // Result is in %rax, move to target register
            fprintf(output, "    movq %%rax, %%r%d  # Tuple element value\n", target_reg + 8);
            return;
        }
        
        // YZ_22: Check if this is a list access
        if (func && function_is_list(func, access->collection_name)) {
            fprintf(output, "    # List index access: %s[...]\n", access->collection_name);
            
            // Get stack offset for list variable
            int offset = function_get_var_offset(func, access->collection_name);
            fprintf(output, "    movq %d(%%rbp), %%rdi  # Load list pointer (arg1)\n", offset);
            
            // Load index into rsi (arg2)
            if (access->index_type == 0) {
                // Constant index
                fprintf(output, "    movq $%d, %%rsi  # Constant index (arg2)\n", access->index.const_index);
            } else if (access->index_type == 1) {
                // Variable index
                int idx_offset = function_get_var_offset(func, access->index.var_index);
                fprintf(output, "    movq %d(%%rbp), %%rsi  # Variable index (arg2)\n", idx_offset);
            } else if (access->index_type == 2) {
                // Expression index
                ArithmeticExpr* idx_expr = (ArithmeticExpr*)access->index.expr_index;
                fprintf(output, "    # Evaluate expression index\n");
                generate_expr_code(output, idx_expr, 0, func);  // result in r8
                fprintf(output, "    movq %%r8, %%rsi  # Expression index (arg2)\n");
            }
            
            // Call tto_list_get(list_ptr, index)
            fprintf(output, "    call tto_list_get  # Returns element pointer in rax\n");
            
            // Dereference to get actual value (assuming numeric for now)
            fprintf(output, "    movq (%%rax), %%rax  # Dereference to get value\n");
            
            // Result is in %rax, move to target register
            fprintf(output, "    movq %%rax, %%r%d  # List element value\n", target_reg + 8);
            return;
        }
        
        // Regular array access
        fprintf(output, "    # Array index access: %s\n", access->collection_name);
        
        // Get stack offset for array variable
        if (func) {
            int offset = function_get_var_offset(func, access->collection_name);
            
            if (access->index_type == 0) {
                // Constant index
                int arr_length = function_get_array_length(func, access->collection_name);
                fprintf(output, "    movq %d(%%rbp), %%rbx  # Load array pointer from stack\n", offset);
                
                // Bounds check
                if (arr_length > 0) {
                    fprintf(output, "    # Bounds check: index=%d, length=%d\n", 
                            access->index.const_index, arr_length);
                    if (access->index.const_index >= arr_length) {
                        fprintf(output, "    # COMPILE ERROR: Index %d >= length %d!\n", 
                                access->index.const_index, arr_length);
                    }
                    fprintf(output, "    movq $%d, %%rcx      # Load index to register\n", access->index.const_index);
                    fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                    fprintf(output, "    jl .bounds_ok_read_%p  # Jump if within bounds\n", (void*)access);
                    fprintf(output, "    movq $%d, %%rdi      # index\n", access->index.const_index);
                    fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                    fprintf(output, "    xorq %%rdx, %%rdx    # NULL for array name\n");
                    fprintf(output, "    call mlp_panic_array_bounds\n");
                    fprintf(output, ".bounds_ok_read_%p:\n", (void*)access);
                }
                
                int elem_offset = access->index.const_index * 8;
                fprintf(output, "    movq %d(%%rbx), %%rax  # Get element at index %d\n", elem_offset, access->index.const_index);
            } else if (access->index_type == 1) {
                // Variable index
                int idx_offset = function_get_var_offset(func, access->index.var_index);
                int arr_length = function_get_array_length(func, access->collection_name);
                
                fprintf(output, "    movq %d(%%rbp), %%rbx  # Load array pointer\n", offset);
                fprintf(output, "    movq %d(%%rbp), %%rcx  # Load index value\n", idx_offset);
                
                // Bounds check
                if (arr_length > 0) {
                    fprintf(output, "    # Bounds check: variable index vs length=%d\n", arr_length);
                    fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                    fprintf(output, "    jl .bounds_ok_read_%p  # Jump if within bounds\n", (void*)access);
                    fprintf(output, "    movq %%rcx, %%rdi    # index\n");
                    fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                    fprintf(output, "    xorq %%rdx, %%rdx    # NULL for array name\n");
                    fprintf(output, "    call mlp_panic_array_bounds\n");
                    fprintf(output, ".bounds_ok_read_%p:\n", (void*)access);
                }
                
                fprintf(output, "    shlq $3, %%rcx  # index * 8\n");
                fprintf(output, "    movq (%%rbx,%%rcx), %%rax  # Get element\n");
            } else if (access->index_type == 2) {
                // Expression index: arr[x+1]
                ArithmeticExpr* idx_expr = (ArithmeticExpr*)access->index.expr_index;
                int arr_length = function_get_array_length(func, access->collection_name);
                
                fprintf(output, "    # Expression index: evaluate to get offset\n");
                fprintf(output, "    movq %d(%%rbp), %%rbx  # Load array pointer\n", offset);
                // Evaluate index expression to r8
                generate_expr_code(output, idx_expr, 0, func);  // target_reg=0 → r8
                fprintf(output, "    movq %%r8, %%rcx     # Move index to rcx\n");
                
                // Bounds check
                if (arr_length > 0) {
                    fprintf(output, "    # Bounds check: expression index vs length=%d\n", arr_length);
                    fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                    fprintf(output, "    jl .bounds_ok_read_%p  # Jump if within bounds\n", (void*)access);
                    fprintf(output, "    movq %%rcx, %%rdi    # index\n");
                    fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                    fprintf(output, "    xorq %%rdx, %%rdx    # NULL for array name\n");
                    fprintf(output, "    call mlp_panic_array_bounds\n");
                    fprintf(output, ".bounds_ok_read_%p:\n", (void*)access);
                }
                
                fprintf(output, "    shlq $3, %%rcx       # index * 8\n");
                fprintf(output, "    movq (%%rbx,%%rcx), %%rax  # Get element\n");
            }
        } else {
            fprintf(output, "    # Error: No function context for array access\n");
        }
        
        // Result is in %rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Array element value\n", target_reg + 8);
        return;
    }
    
    // YZ_17: Collection literals (Array, List, Tuple)
    if (expr->is_collection && expr->collection) {
        fprintf(output, "    # Collection literal\n");
        codegen_collection(output, expr->collection);
        // Result is in rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Collection pointer\n", target_reg + 8);
        return;
    }
    
    // Leaf node (literal or variable)
    if (expr->is_literal || (!expr->left && !expr->right)) {
        if (expr->is_literal) {
            if (expr->is_boolean) {
                // Boolean literal - convert true/false to 1/0
                int bool_value = (strcmp(expr->value, "true") == 0) ? 1 : 0;
                fprintf(output, "    movq $%d, %%r%d  # Boolean literal: %s\n", 
                        bool_value, target_reg + 8, expr->value);
            } else if (expr->is_string) {
                // String literal - define in .rodata and load pointer
                static int string_counter = 0;
                fprintf(output, "    .section .rodata\n");
                fprintf(output, "    _str_%d: .asciz \"%s\"\n", string_counter, expr->value);
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
    
    // Handle string concatenation (YZ_07)
    if (is_string && expr->op == ARITH_ADD) {
        fprintf(output, "    # String concatenation (text + text)\n");
        fprintf(output, "    movq %%r%d, %%rdi  # arg1: first string pointer\n", left_reg + 8);
        fprintf(output, "    movq %%r%d, %%rsi  # arg2: second string pointer\n", right_reg + 8);
        fprintf(output, "    call mlp_string_concat  # Returns new string in %%rax\n");
        fprintf(output, "    movq %%rax, %%r%d  # Store result string pointer\n", target_reg + 8);
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
                // Phase 6: Division by zero check
                {
                    static int div_check_label = 0;
                    int current_label = div_check_label++;
                    fprintf(output, "    # Division with zero check\n");
                    fprintf(output, "    testq %%r%d, %%r%d  # Check divisor\n", right_reg + 8, right_reg + 8);
                    fprintf(output, "    jnz .div_ok_%d\n", current_label);
                    fprintf(output, "    # Division by zero! Call runtime error\n");
                    fprintf(output, "    pushq %%r8\n");   // Save registers
                    fprintf(output, "    pushq %%r9\n");
                    fprintf(output, "    subq $8, %%rsp\n");  // Align stack to 16 bytes
                    fprintf(output, "    leaq .div_zero_msg(%%rip), %%rdi\n");
                    fprintf(output, "    call mlp_runtime_error\n");
                    fprintf(output, "    addq $8, %%rsp\n");  // Restore stack (won't reach here)
                    fprintf(output, "    popq %%r9\n");
                    fprintf(output, "    popq %%r8\n");
                    fprintf(output, ".div_ok_%d:\n", current_label);
                }
                // Division requires rax/rdx setup
                fprintf(output, "    movq %%r%d, %%rax\n", left_reg + 8);
                fprintf(output, "    cqo\n");  // Sign extend rax to rdx:rax
                fprintf(output, "    idivq %%r%d\n", right_reg + 8);
                fprintf(output, "    movq %%rax, %%r%d\n", left_reg + 8);
                break;
            case ARITH_MOD:
                // Phase 6: Division by zero check for modulo
                {
                    static int mod_check_label = 0;
                    int current_label = mod_check_label++;
                    fprintf(output, "    # Modulo with zero check\n");
                    fprintf(output, "    testq %%r%d, %%r%d  # Check divisor\n", right_reg + 8, right_reg + 8);
                    fprintf(output, "    jnz .mod_ok_%d\n", current_label);
                    fprintf(output, "    # Modulo by zero! Call runtime error\n");
                    fprintf(output, "    pushq %%r8\n");
                    fprintf(output, "    pushq %%r9\n");
                    fprintf(output, "    subq $8, %%rsp\n");  // Align stack
                    fprintf(output, "    leaq .div_zero_msg(%%rip), %%rdi\n");
                    fprintf(output, "    call mlp_runtime_error\n");
                    fprintf(output, "    addq $8, %%rsp\n");
                    fprintf(output, "    popq %%r9\n");
                    fprintf(output, "    popq %%r8\n");
                    fprintf(output, ".mod_ok_%d:\n", current_label);
                }
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
