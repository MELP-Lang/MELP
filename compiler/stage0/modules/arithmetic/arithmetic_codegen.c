#include "arithmetic_codegen.h"
#include "../../../../runtime/sto/runtime_sto.h"
#include "../functions/functions.h"  // For FunctionDeclaration type
#include "../array/array_codegen.h"  // YZ_14: For array access codegen
#include "../struct/struct.h"  // YZ_82: For member access
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Register counter for temporary values
static int reg_counter = 0;
static int overflow_label_counter = 0;

// YZ_91: Helper to check if a variable is a string type
static int is_var_string_type(const char* var_name, FunctionDeclaration* func) {
    if (!func || !var_name) return 0;
    
    // Check local variables
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, var_name) == 0) {
            return !var->is_numeric;  // is_numeric=0 means string
        }
        var = var->next;
    }
    
    // Check parameters
    FunctionParam* param = func->params;
    while (param) {
        if (strcmp(param->name, var_name) == 0) {
            return (param->type == FUNC_PARAM_TEXT);
        }
        param = param->next;
    }
    
    return 0;  // Default to numeric
}

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
        // Fallback: assume first local variable at -8(%rbp)
        // TODO: Implement proper global variable tracking
        fprintf(output, "    # Warning: No function context for variable %s\n", value);
        if (is_float) {
            fprintf(output, "    movsd -8(%%rbp), %%xmm%d  # Fallback load\n", reg_num);
        } else {
            fprintf(output, "    movq -8(%%rbp), %%r%d  # Fallback load %s\n", reg_num + 8, value);
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
    // (YZ_47: TOKEN_PRINTLN parser, YZ_48: codegen)
    // YZ_91: Added string type support
    if (expr->is_function_call && expr->func_call && 
        strcmp(expr->func_call->function_name, "println") == 0 && 
        expr->func_call->arg_count == 1) {
        fprintf(output, "    # println() call\n");
        
        ArithmeticExpr* arg = expr->func_call->arguments[0];
        
        // Evaluate argument expression → r8
        generate_expr_code(output, arg, 0, func);
        
        // YZ_91: Determine if argument is string type
        int is_string_arg = arg->is_string;
        
        // Check variable type from symbol table if it's a variable
        if (!is_string_arg && !arg->is_literal && arg->value && !arg->left && !arg->right) {
            is_string_arg = is_var_string_type(arg->value, func);
        }
        
        // Check if binary operation with string operand
        if (!is_string_arg && (arg->left || arg->right)) {
            if ((arg->left && arg->left->is_string) || (arg->right && arg->right->is_string)) {
                is_string_arg = 1;
            }
        }
        
        fprintf(output, "    movq %%r8, %%rdi\n");
        
        if (is_string_arg) {
            // String: use puts (prints string + newline)
            fprintf(output, "    call puts  # Print string + newline\n");
        } else {
            // Numeric: use sto_print_int64 + manual newline
            fprintf(output, "    call sto_print_int64\n");
            fprintf(output, "    movq $10, %%rdi  # newline char\n");
            fprintf(output, "    call putchar\n");
        }
        return;
    }
    
    // Phase 3.5: Function call
    if (expr->is_function_call && expr->func_call) {
        FunctionCallExpr* call = expr->func_call;
        
        fprintf(output, "    # Function call: %s()\n", call->function_name);
        
        // x86-64 calling convention registers: rdi, rsi, rdx, rcx, r8, r9
        const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        const int max_reg_params = 6;
        
        // YZ_94: For nested function calls, we need to evaluate all arguments first
        // and store them on the stack to prevent register clobbering
        // Then pop them into parameter registers in reverse order
        
        int actual_arg_count = (call->arg_count < max_reg_params) ? call->arg_count : max_reg_params;
        
        // Phase 1: Evaluate all arguments and push to stack (in order)
        for (int i = 0; i < actual_arg_count; i++) {
            // YZ_84: Check if argument is a struct instance (pass by pointer)
            ArithmeticExpr* arg_expr = call->arguments[i];
            if (arg_expr && !arg_expr->is_literal && arg_expr->value && 
                !arg_expr->left && !arg_expr->right && !arg_expr->is_function_call) {
                StructInstanceInfo* inst = struct_lookup_instance(arg_expr->value);
                if (inst && !inst->is_pointer) {
                    // Pass address of struct value - push to stack
                    fprintf(output, "    leaq -%d(%%rbp), %%r10  # Struct %s address\n", 
                            inst->stack_offset, arg_expr->value);
                    fprintf(output, "    pushq %%r10  # Save arg %d (struct ptr)\n", i + 1);
                    continue;
                }
            }
            
            // Regular argument: evaluate expression to r10 (temporary)
            generate_expr_code(output, arg_expr, 2, func);  // Use r10 (reg 2)
            
            // Push result to stack for later
            fprintf(output, "    pushq %%r10  # Save arg %d\n", i + 1);
        }
        
        // Phase 2: Pop arguments into parameter registers (reverse order)
        for (int i = actual_arg_count - 1; i >= 0; i--) {
            fprintf(output, "    popq %s  # Restore arg %d\n", param_regs[i], i + 1);
        }
        
        // TODO: Stack arguments for more than 6 parameters
        if (call->arg_count > max_reg_params) {
            fprintf(stderr, "Warning: Functions with >6 parameters not yet supported\n");
        }
        
        // Check if builtin stdlib function
        const char* actual_function = call->function_name;
        if (strcmp(call->function_name, "println") == 0) {
            // STO: Check if argument is numeric by looking up in function context
            int is_numeric_arg = 1;  // Default: numeric
            if (call->arg_count > 0 && call->arguments[0] && func) {
                ArithmeticExpr* arg_expr = call->arguments[0];
                if (arg_expr && !arg_expr->left && !arg_expr->right && arg_expr->value && !arg_expr->is_literal) {
                    // Variable reference - lookup type from function context
                    is_numeric_arg = function_get_var_is_numeric(func, arg_expr->value);
                }
            }
            
            if (is_numeric_arg) {
                // Numeric argument - STO-aware: Pass pointer to value + type
                // Value is in %rdi, need to save to stack and pass pointer
                fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
                fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
                fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
                fprintf(output, "    movq $0, %%rsi       # arg2: STO_TYPE_INT64\n");
                actual_function = "mlp_println_numeric";
            } else {
                // Text argument - direct call
                actual_function = "mlp_println_string";
            }
        } else if (strcmp(call->function_name, "print") == 0) {
            fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
            fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
            fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
            fprintf(output, "    movq $0, %%rsi       # arg2: STO_TYPE_INT64\n");
            actual_function = "mlp_print_numeric";
        } else if (strcmp(call->function_name, "toString") == 0) {
            fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
            fprintf(output, "    movq %%rdi, (%%rsp)  # Store value\n");
            fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
            fprintf(output, "    movq $0, %%rsi       # arg2: STO_TYPE_INT64\n");
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
        
        // Clean up temp space for STO calls
        if (strcmp(call->function_name, "println") == 0 ||
            strcmp(call->function_name, "print") == 0 ||
            strcmp(call->function_name, "toString") == 0) {
            fprintf(output, "    addq $16, %%rsp      # Clean up temp space\n");
        }
        
        // Return value is in rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Function return value\n", target_reg + 8);
        
        return;
    }
    
    // YZ_82/YZ_83: Struct member access (p.x or p.addr.zip)
    if (expr->is_member_access && expr->member_access) {
        MemberAccess* access = (MemberAccess*)expr->member_access;
        
        fprintf(output, "    # Member access: %s", access->struct_instance);
        for (int i = 0; i < access->chain_length; i++) {
            fprintf(output, ".%s", access->member_chain[i]);
        }
        fprintf(output, "\n");
        
        // Look up struct instance
        StructInstanceInfo* inst_info = struct_lookup_instance(access->struct_instance);
        if (!inst_info) {
            fprintf(output, "    # ERROR: Instance '%s' not found\n", 
                    access->struct_instance);
            fprintf(output, "    xor %%r%d, %%r%d  # Error fallback\n", 
                    target_reg + 8, target_reg + 8);
            return;
        }
        
        // YZ_84: Handle struct pointers (function parameters)
        int is_pointer = inst_info->is_pointer;
        
        // YZ_83: Traverse member chain for nested access
        StructDef* current_def = inst_info->definition;
        int cumulative_offset = 0;
        
        for (int chain_idx = 0; chain_idx < access->chain_length; chain_idx++) {
            const char* member_name = access->member_chain[chain_idx];
            
            // Find member in current definition
            StructMember* member = current_def->members;
            while (member) {
                if (strcmp(member->name, member_name) == 0) {
                    break;
                }
                member = member->next;
            }
            
            if (!member) {
                fprintf(output, "    # ERROR: Member '%s' not found in chain\n", member_name);
                fprintf(output, "    xor %%r%d, %%r%d  # Error fallback\n", 
                        target_reg + 8, target_reg + 8);
                return;
            }
            
            // Add member's offset to cumulative offset
            cumulative_offset += member->offset;
            
            // If this is a nested struct and not the last in chain, continue
            if (chain_idx < access->chain_length - 1) {
                if (!member->nested_def) {
                    fprintf(output, "    # ERROR: Member '%s' is not a struct\n", member_name);
                    fprintf(output, "    xor %%r%d, %%r%d  # Error fallback\n", 
                            target_reg + 8, target_reg + 8);
                    return;
                }
                current_def = member->nested_def;  // Traverse into nested struct
            }
        }
        
        // YZ_84: Calculate address based on pointer or value type
        if (is_pointer) {
            // Pointer: load pointer first, then access member
            fprintf(output, "    # Load pointer at %d(%%rbp)\n", inst_info->stack_offset);
            fprintf(output, "    movq %d(%%rbp), %%r10  # Load struct pointer\n", 
                    inst_info->stack_offset);
            fprintf(output, "    # Access member at offset %d from pointer\n", cumulative_offset);
            fprintf(output, "    movq %d(%%r10), %%r%d  # Load member value\n",
                    cumulative_offset, target_reg + 8);
        } else {
            // Value: direct offset from rbp
            int final_offset = inst_info->stack_offset - cumulative_offset;
            
            fprintf(output, "    # Load from [rbp - %d + %d] = [rbp - %d]\n",
                    inst_info->stack_offset, cumulative_offset, final_offset);
            
            // Load value (assuming numeric for now)
            fprintf(output, "    movq %d(%%rbp), %%r%d  # Load nested member value\n",
                    -final_offset, target_reg + 8);
        }
        
        return;
    }
    
    // YZ_86: Method call (p.method(args))
    if (expr->is_method_call && expr->method_call) {
        MethodCall* call = (MethodCall*)expr->method_call;
        
        fprintf(output, "    # Method call: %s.%s()\n", 
                call->instance_name, call->method_name);
        
        // Look up struct instance to get its type
        StructInstanceInfo* inst_info = struct_lookup_instance(call->instance_name);
        if (!inst_info) {
            fprintf(output, "    # ERROR: Instance '%s' not found\n", 
                    call->instance_name);
            fprintf(output, "    xorq %%r%d, %%r%d  # Error fallback\n", 
                    target_reg + 8, target_reg + 8);
            return;
        }
        
        // Find method in struct definition
        StructMethod* method = struct_find_method(inst_info->definition, call->method_name);
        if (!method) {
            fprintf(output, "    # ERROR: Method '%s' not found in struct '%s'\n",
                    call->method_name, inst_info->definition->name);
            fprintf(output, "    xorq %%r%d, %%r%d  # Error fallback\n",
                    target_reg + 8, target_reg + 8);
            return;
        }
        
        // Generate function name: StructName_methodname
        char func_name[256];
        snprintf(func_name, sizeof(func_name), "%s_%s",
                 inst_info->definition->name, call->method_name);
        
        fprintf(output, "    # Calling %s(self, ...)\n", func_name);
        
        // Save caller-saved registers if needed
        if (target_reg > 0) {
            fprintf(output, "    pushq %%r8\n");
            fprintf(output, "    pushq %%r9\n");
        }
        
        // Load 'self' pointer as first argument (in %rdi)
        if (inst_info->is_pointer) {
            // Instance is already a pointer (function parameter)
            fprintf(output, "    movq -%d(%%rbp), %%rdi  # Load self pointer\n",
                    inst_info->stack_offset);
        } else {
            // Instance is a value, take its address
            fprintf(output, "    leaq -%d(%%rbp), %%rdi  # Load self address\n",
                    inst_info->stack_offset);
        }
        
        // Load remaining arguments into %rsi, %rdx, %rcx, %r8, %r9
        // YZ_86: For now, we support simple numeric arguments only
        const char* arg_regs[] = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        int max_args = 5;  // After self, we have 5 more registers
        
        for (int i = 0; i < call->arg_count && i < max_args; i++) {
            // Arguments are stored as ArithmeticExpr* (cast from Expression*)
            ArithmeticExpr* arg = (ArithmeticExpr*)call->arguments[i];
            
            // Evaluate argument into a temporary register
            int temp_reg = 0;  // Use r8 as temp
            generate_expr_code(output, arg, temp_reg, func);
            
            // Move result to appropriate argument register
            fprintf(output, "    movq %%r%d, %s  # arg%d\n",
                    temp_reg + 8, arg_regs[i], i + 1);
        }
        
        // Call the method function
        fprintf(output, "    call %s\n", func_name);
        
        // Result is in %rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Method return value\n",
                target_reg + 8);
        
        // Restore caller-saved registers
        if (target_reg > 0) {
            fprintf(output, "    popq %%r9\n");
            fprintf(output, "    popq %%r8\n");
        }
        
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
            
            // Call sto_tuple_get(tuple_ptr, index)
            fprintf(output, "    call sto_tuple_get  # Returns element pointer in rax\n");
            
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
            
            // Call sto_list_get(list_ptr, index)
            fprintf(output, "    call sto_list_get  # Returns element pointer in rax\n");
            
            // Dereference to get actual value (assuming numeric for now)
            fprintf(output, "    movq (%%rax), %%rax  # Dereference to get value\n");
            
            // Result is in %rax, move to target register
            fprintf(output, "    movq %%rax, %%r%d  # List element value\n", target_reg + 8);
            return;
        }
        
        // Regular array access
        fprintf(output, "    # Array index access: %s\n", access->collection_name);
        
        // Get stack offset for array variable
        int offset = 0;
        if (func) {
            offset = function_get_var_offset(func, access->collection_name);
        } else {
            // Fallback: assume first local variable at -8(%rbp)
            // TODO: Implement proper global variable tracking
            offset = -8;
            fprintf(output, "    # Warning: No function context, assuming offset=%d for %s\n", 
                    offset, access->collection_name);
        }
        
        if (access->index_type == 0) {
            // Constant index
            int arr_length = func ? function_get_array_length(func, access->collection_name) : 0;
            fprintf(output, "    movq %d(%%rbp), %%rbx  # Load array pointer from stack\n", offset);
            
            // Bounds check (skip if no func context)
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
            int idx_offset = func ? function_get_var_offset(func, access->index.var_index) : -16;
            int arr_length = func ? function_get_array_length(func, access->collection_name) : 0;
            
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
            int arr_length = func ? function_get_array_length(func, access->collection_name) : 0;
            
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
        
        // Result is in %rax, move to target register
        fprintf(output, "    movq %%rax, %%r%d  # Array element value\n", target_reg + 8);
        return;
    }
    
    // YZ_17: Collection literals (Array, List, Tuple)
    if (expr->is_collection && expr->collection) {
        fprintf(output, "    # Collection literal\n");
        codegen_collection(output, expr->collection, func);
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
    
    // Determine if we're dealing with floats or strings (before eval)
    int is_float = (expr->left && expr->left->is_float) || 
                   (expr->right && expr->right->is_float);
    int is_string = (expr->left && expr->left->is_string) || 
                    (expr->right && expr->right->is_string) ||
                    (expr->op == ARITH_ADD && expr->is_string);  // YZ_92: Check parent's is_string flag
    
    // YZ_92: Handle string concatenation with special register management
    // String concat needs special handling because sub-expressions (like mlp_number_to_string)
    // can clobber caller-saved registers (%r8-%r11) before the concat happens
    if (is_string && expr->op == ARITH_ADD) {
        fprintf(output, "    # String concatenation (text + text)\n");
        
        // YZ_92: Save callee-saved registers BEFORE evaluating sub-expressions
        fprintf(output, "    pushq %%r12  # Save callee-saved register\n");
        fprintf(output, "    pushq %%r13  # Save callee-saved register\n");
        
        // Evaluate left operand and immediately save to r12
        generate_expr_code(output, expr->left, left_reg, func);
        fprintf(output, "    movq %%r%d, %%r12  # Left operand to callee-saved\n", left_reg + 8);
        
        // Evaluate right operand and immediately save to r13
        generate_expr_code(output, expr->right, right_reg, func);
        fprintf(output, "    movq %%r%d, %%r13  # Right operand to callee-saved\n", right_reg + 8);
        
        // YZ_91: Check if operands need numeric-to-string conversion
        int right_is_numeric = 0;
        if (expr->right && !expr->right->is_string && !expr->right->is_literal && expr->right->value) {
            right_is_numeric = !is_var_string_type(expr->right->value, func);
        } else if (expr->right && expr->right->is_literal && !expr->right->is_string) {
            right_is_numeric = 1;
        }
        // YZ_92: Check if right is a binary expression (likely arithmetic)
        if (expr->right && expr->right->left && expr->right->right && !expr->right->is_string) {
            right_is_numeric = 1;
        }
        // YZ_93: Check if right is a function call returning numeric
        if (expr->right && expr->right->is_function_call && expr->right->func_call) {
            const char* fn = expr->right->func_call->function_name;
            // String-returning functions
            if (strcmp(fn, "mlp_number_to_string") == 0 ||
                strcmp(fn, "mlp_string_concat") == 0 ||
                strcmp(fn, "toString") == 0 ||
                strcmp(fn, "substring") == 0 ||
                strcmp(fn, "toUpperCase") == 0 ||
                strcmp(fn, "toLowerCase") == 0 ||
                strcmp(fn, "trim") == 0 ||
                strcmp(fn, "replace") == 0 ||
                strcmp(fn, "input") == 0) {
                // These return strings, no conversion needed
                right_is_numeric = 0;
            } else {
                // Most other functions (abs, sqrt, pow, length, etc.) return numeric
                right_is_numeric = 1;
            }
        }
        
        int left_is_numeric = 0;
        if (expr->left && !expr->left->is_string && !expr->left->is_literal && expr->left->value) {
            left_is_numeric = !is_var_string_type(expr->left->value, func);
        } else if (expr->left && expr->left->is_literal && !expr->left->is_string) {
            left_is_numeric = 1;
        }
        // YZ_92: Check if left is a binary expression (likely arithmetic)
        if (expr->left && expr->left->left && expr->left->right && !expr->left->is_string) {
            left_is_numeric = 1;
        }
        // YZ_93: Check if left is a function call returning numeric
        if (expr->left && expr->left->is_function_call && expr->left->func_call) {
            const char* fn = expr->left->func_call->function_name;
            if (strcmp(fn, "mlp_number_to_string") == 0 ||
                strcmp(fn, "mlp_string_concat") == 0 ||
                strcmp(fn, "toString") == 0 ||
                strcmp(fn, "substring") == 0 ||
                strcmp(fn, "toUpperCase") == 0 ||
                strcmp(fn, "toLowerCase") == 0 ||
                strcmp(fn, "trim") == 0 ||
                strcmp(fn, "replace") == 0 ||
                strcmp(fn, "input") == 0) {
                left_is_numeric = 0;
            } else {
                left_is_numeric = 1;
            }
        }
        
        // Convert left if numeric
        if (left_is_numeric) {
            fprintf(output, "    movq %%r12, %%rdi  # Numeric to convert\n");
            fprintf(output, "    call mlp_number_to_string\n");
            fprintf(output, "    movq %%rax, %%r12  # String pointer\n");
        }
        
        // Convert right if numeric
        if (right_is_numeric) {
            fprintf(output, "    movq %%r13, %%rdi  # Numeric to convert\n");
            fprintf(output, "    call mlp_number_to_string\n");
            fprintf(output, "    movq %%rax, %%r13  # String pointer\n");
        }
        
        // Now concatenate (r12 and r13 are preserved)
        fprintf(output, "    movq %%r12, %%rdi  # arg1: first string\n");
        fprintf(output, "    movq %%r13, %%rsi  # arg2: second string\n");
        fprintf(output, "    call mlp_string_concat\n");
        fprintf(output, "    movq %%rax, %%r%d  # Result string pointer\n", target_reg + 8);
        
        // Restore callee-saved registers
        fprintf(output, "    popq %%r13  # Restore callee-saved\n");
        fprintf(output, "    popq %%r12  # Restore callee-saved\n");
        return;
    }
    
    // Generate code for left subtree (non-string case)
    generate_expr_code(output, expr->left, left_reg, func);
    
    // Generate code for right subtree
    generate_expr_code(output, expr->right, right_reg, func);
    
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
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call sto_bigdec_add\n");
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
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call sto_bigdec_sub\n");
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
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rdi # First BigDecimal\n");
                fprintf(output, "    movq %%r%d, %%rsi\n", right_reg + 8);
                fprintf(output, "    call sto_bigdec_from_int64\n");
                fprintf(output, "    movq %%rax, %%rsi # Second BigDecimal\n");
                fprintf(output, "    call sto_bigdec_mul\n");
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
