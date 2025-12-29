#include "statement_codegen.h"
#include "../control_flow/control_flow_codegen.h"  // ✅ While/If codegen
#include "../for_loop/for_loop_codegen.h"           // ✅ For loop codegen
#include "../switch/switch_codegen.h"               // ✅ YZ_89: Switch codegen
#include "../print/print_codegen.h"                 // ✅ Print codegen
#include "../variable/variable_codegen.h"           // ✅ Variable codegen
#include "../variable/variable.h"                   // ✅ VariableDeclaration, VariableAssignment
#include "../arithmetic/arithmetic_codegen.h"       // ✅ Expression codegen
#include "../arithmetic/arithmetic.h"               // ✅ ArithmeticExpr
#include "../functions/functions.h"                 // ✅ ReturnStatement, FunctionDeclaration
#include "../array/array.h"                         // ✅ YZ_15: IndexAccess, ArrayAssignment
#include "../struct/struct_codegen.h"               // ✅ YZ_81: Struct codegen
#include "../enum/enum.h"                           // ✅ YZ_96: Enum definitions
#include "../enum/enum_codegen.h"                   // ✅ YZ_96: Enum codegen
#include <stdio.h>
#include <stdlib.h>   // YZ_21: For free()
#include <string.h>

// Global counter for string literals
static int string_literal_counter = 0;

// YZ_28: Loop stack for exit support (VB.NET style)
// YZ_90: Added continue support
LoopContext loop_stack[MAX_LOOP_DEPTH];
int loop_stack_top = -1;

// YZ_82: Current stack offset tracker (for struct instances)
static int current_stack_offset = 0;

void loop_push(int exit_label, int continue_label) {
    if (loop_stack_top < MAX_LOOP_DEPTH - 1) {
        loop_stack_top++;
        loop_stack[loop_stack_top].exit_label = exit_label;
        loop_stack[loop_stack_top].continue_label = continue_label;
    }
}

void loop_pop(void) {
    if (loop_stack_top >= 0) {
        loop_stack_top--;
    }
}

int get_break_label(void) {
    if (loop_stack_top >= 0) {
        return loop_stack[loop_stack_top].exit_label;
    }
    return -1;  // No loop context
}

int get_continue_label(void) {
    if (loop_stack_top >= 0) {
        return loop_stack[loop_stack_top].continue_label;
    }
    return -1;  // No loop context
}

// YZ_82: Reset stack offset for new function
void statement_reset_stack_offset(void) {
    current_stack_offset = 0;
    struct_clear_instances();  // Clear instance registry
}

// YZ_85: Set initial stack offset (for functions with hidden return pointer)
void statement_set_initial_stack_offset(int offset) {
    current_stack_offset = offset;
}

// Statement code generation with modular imports
void statement_generate_code(FILE* output, Statement* stmt, FunctionDeclaration* func) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_WHILE: {
            // ✅ Use control_flow module for while codegen
            WhileStatement* while_stmt = (WhileStatement*)stmt->data;
            if (while_stmt) {
                control_flow_generate_while(output, while_stmt, func);
            }
            break;
        }
        
        case STMT_IF: {
            // ✅ Use control_flow module for if codegen
            IfStatement* if_stmt = (IfStatement*)stmt->data;
            if (if_stmt) {
                control_flow_generate_if(output, if_stmt, func);
            }
            break;
        }
        
        case STMT_FOR: {
            // ✅ Use for_loop module for codegen
            ForLoop* for_loop = (ForLoop*)stmt->data;
            if (for_loop) {
                for_loop_generate_code(output, for_loop, func);
            }
            break;
        }
        
        case STMT_SWITCH: {
            // ✅ YZ_89: Use switch module for codegen
            SwitchStatement* switch_stmt = (SwitchStatement*)stmt->data;
            if (switch_stmt) {
                switch_codegen(output, switch_stmt, func);
            }
            break;
        }
        
        case STMT_PRINT: {
            // ✅ Use print module for all print types (literal, variable, expression)
            PrintStatement* print_stmt = (PrintStatement*)stmt->data;
            if (print_stmt) {
                codegen_print_statement(output, print_stmt, func);
            }
            break;
        }
        
        // ✅ YZ_48: Expression statement (function calls like println)
        case STMT_EXPRESSION: {
            ArithmeticExpr* expr = (ArithmeticExpr*)stmt->data;
            if (expr) {
                arithmetic_generate_code(output, expr, func);
            }
            break;
        }
        
        case STMT_VARIABLE_DECL: {
            // ✅ Variable declaration - already registered in pre-scan, just initialize
            VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
            if (decl) {
                // Get offset (already registered)
                int offset = function_get_var_offset(func, decl->name);
                
                fprintf(output, "    # Variable: %s at %d(%%rbp)\n", decl->name, offset);
                
                // ✅ YZ_99: Handle array declaration with size (numeric[5] arr)
                if (decl->is_array && decl->array_size > 0 && (!decl->value || decl->value[0] != '[')) {
                    // Array declaration without initializer - allocate only
                    fprintf(output, "    # Array declaration: %s[%d]\n", decl->name, decl->array_size);
                    
                    // Register array for bounds checking
                    function_register_array_var(func, decl->name, decl->array_size, 1);
                    
                    // YZ_99: Create .rodata string literal for array name (for bounds check error messages)
                    fprintf(output, "\n.section .rodata\n");
                    fprintf(output, ".str_arr_%s:\n", decl->name);
                    fprintf(output, "    .string \"%s\"\n", decl->name);
                    fprintf(output, ".text\n\n");
                    
                    // Allocate array via STO runtime
                    fprintf(output, "    movq $%d, %%rdi      # count\n", decl->array_size);
                    fprintf(output, "    movq $8, %%rsi       # elem_size (8 bytes)\n");
                    fprintf(output, "    call sto_array_alloc # Returns pointer in %%rax\n");
                    fprintf(output, "    movq %%rax, %d(%%rbp)  # Store array pointer at %s\n", 
                            offset, decl->name);
                }
                // ✅ Handle array initialization with literal
                else if (decl->is_array && decl->value && decl->value[0] == '[') {
                    // Array literal: [1, 2, 3]
                    fprintf(output, "    # Array initialization: %s = %s\n", decl->name, decl->value);
                    
                    // Parse and count elements
                    char* value_copy = strdup(decl->value);
                    char* ptr = value_copy + 1;  // Skip '['
                    int element_count = 0;
                    
                    // Count elements
                    while (*ptr && *ptr != ']') {
                        while (*ptr == ' ' || *ptr == ',') ptr++;
                        if (*ptr == ']') break;
                        if (*ptr == '"') {
                            ptr++;
                            while (*ptr && *ptr != '"') ptr++;
                            if (*ptr == '"') ptr++;
                        } else {
                            while (*ptr && *ptr != ',' && *ptr != ']' && *ptr != ' ') ptr++;
                        }
                        element_count++;
                        while (*ptr == ' ') ptr++;
                        if (*ptr == ',') ptr++;
                    }
                    
                    // Register array with length for bounds checking
                    function_register_array_var(func, decl->name, element_count, 1);
                    
                    // YZ_99: Create .rodata string literal for array name (for bounds check error messages)
                    fprintf(output, "\n.section .rodata\n");
                    fprintf(output, ".str_arr_%s:\n", decl->name);
                    fprintf(output, "    .string \"%s\"\n", decl->name);
                    fprintf(output, ".text\n\n");
                    
                    // Allocate array via STO runtime
                    fprintf(output, "    # Allocate array with %d elements\n", element_count);
                    fprintf(output, "    movq $%d, %%rdi      # count\n", element_count);
                    fprintf(output, "    movq $8, %%rsi       # elem_size (8 bytes)\n");
                    fprintf(output, "    call sto_array_alloc # Returns pointer in %%rax\n");
                    fprintf(output, "    movq %%rax, %d(%%rbp)  # Store array pointer at %s\n", 
                            offset, decl->name);
                    
                    // Initialize elements
                    ptr = value_copy + 1;  // Reset to start
                    int index = 0;
                    
                    while (*ptr && *ptr != ']') {
                        while (*ptr == ' ' || *ptr == ',') ptr++;
                        if (*ptr == ']') break;
                        
                        // Parse numeric element
                        if (*ptr != '"') {
                            char num_str[64];
                            int num_len = 0;
                            while (*ptr && *ptr != ',' && *ptr != ']' && *ptr != ' ' && num_len < 63) {
                                num_str[num_len++] = *ptr++;
                            }
                            num_str[num_len] = '\0';
                            
                            if (num_len > 0) {
                                fprintf(output, "    movq $%s, %%r8       # Element value\n", num_str);
                                fprintf(output, "    movq %d(%%rbp), %%rbx # Load array pointer\n", offset);
                                fprintf(output, "    movq %%r8, %d(%%rbx)  # Store at index %d\n", 
                                        index * 8, index);
                            }
                        }
                        
                        index++;
                    }
                    
                    free(value_copy);
                    fprintf(output, "    # Array %s: %d elements initialized\n", decl->name, element_count);
                }
                // Initialize if there's an initial value
                else if (decl->init_expr) {
                    // ✅ NEW: init_expr is ArithmeticExpr*
                    ArithmeticExpr* expr = (ArithmeticExpr*)decl->init_expr;
                    
                    // YZ_21: Register tuple variables for indexing support
                    // YZ_22: Register list variables for indexing support
                    if (expr->is_collection && expr->collection) {
                        if (expr->collection->type == COLL_TUPLE) {
                            int tuple_len = expr->collection->data.tuple.length;
                            function_register_tuple_var(func, decl->name, tuple_len);
                            fprintf(output, "    # Tuple %s registered: %d elements\n", 
                                    decl->name, tuple_len);
                        } else if (expr->collection->type == COLL_LIST) {
                            int list_len = expr->collection->data.list.length;
                            function_register_list_var(func, decl->name, list_len);
                            fprintf(output, "    # List %s registered: %d elements\n", 
                                    decl->name, list_len);
                        }
                    }
                    
                    // Generate expression code
                    arithmetic_generate_code(output, expr, func);
                    
                    // Store result to variable
                    fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                            offset, decl->name);
                } else if (decl->value) {
                    // Check if this is a string variable (not a numeric literal)
                    if (decl->type == VAR_STRING) {
                        // String literal - create .rodata entry
                        // Note: lexer strips quotes, so value is raw string
                        fprintf(output, "\n.section .rodata\n");
                        fprintf(output, ".str_%d:\n", string_literal_counter);
                        fprintf(output, "    .string \"%s\"\n", decl->value);  // Add quotes back
                        fprintf(output, ".text\n");
                        fprintf(output, "    leaq .str_%d(%%rip), %%r8  # Load string address\n", 
                                string_literal_counter);
                        fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                                offset, decl->name);
                        string_literal_counter++;
                    } else if (decl->type == VAR_BOOLEAN) {
                        // Boolean literal - convert true/false to 1/0
                        int bool_value = (strcmp(decl->value, "true") == 0) ? 1 : 0;
                        fprintf(output, "    movq $%d, %%r8  # Boolean literal: %s\n", 
                                bool_value, decl->value);
                        fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                                offset, decl->name);
                    } else {
                        // Numeric literal
                        fprintf(output, "    movq $%s, %%r8  # Literal value\n", decl->value);
                        fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                                offset, decl->name);
                    }
                } else {
                    // No initializer - default to 0
                }
            }
            break;
        }
        
        case STMT_ASSIGNMENT: {
            // ✅ Variable assignment - evaluate expression and store
            // YZ_25: Check for implicit declaration or undefined variable
            VariableAssignment* assign = (VariableAssignment*)stmt->data;
            if (assign) {
                // YZ_85: First check if this is a struct instance (already allocated)
                StructInstanceInfo* target_inst = struct_lookup_instance(assign->name);
                
                // YZ_25: Check if variable exists (skip check for struct instances)
                int var_exists = target_inst != NULL || function_var_exists(func, assign->name);
                
                if (!var_exists && assign->is_implicit_declaration) {
                    // YZ_25: Implicit declaration with ';' - register the variable
                    // Infer type from expression
                    int is_numeric = 1;  // Default to numeric
                    if (assign->value_expr) {
                        ArithmeticExpr* expr = (ArithmeticExpr*)assign->value_expr;
                        if (expr->is_string) {
                            is_numeric = 0;
                        }
                    }
                    function_register_local_var_with_type(func, assign->name, is_numeric);
                    fprintf(output, "    # YZ_25: Implicit declaration of '%s' (type inferred)\n", assign->name);
                } else if (!var_exists && !assign->is_implicit_declaration) {
                    // YZ_25: Variable not declared and no ';' - ERROR (likely typo)
                    const char* similar = function_find_similar_var(func, assign->name);
                    if (similar) {
                        fprintf(stderr, "Error: Undefined variable '%s'. Did you mean '%s'?\n", 
                                assign->name, similar);
                        fprintf(stderr, "       Add ';' at end to declare new variable: %s = ...;\n", 
                                assign->name);
                    } else {
                        fprintf(stderr, "Error: Undefined variable '%s'.\n", assign->name);
                        fprintf(stderr, "       Add ';' at end to declare new variable: %s = ...;\n", 
                                assign->name);
                    }
                    return;  // Stop codegen for this statement
                }
                
                fprintf(output, "    # Assignment: %s = ...\n", assign->name);
                
                // Evaluate expression
                if (assign->value_expr) {
                    ArithmeticExpr* expr = (ArithmeticExpr*)assign->value_expr;
                    
                    // YZ_85: Check if this is a struct assignment from function return
                    // ONLY handle if target is a struct AND expression is a simple function call reference
                    if (target_inst && expr->func_call && !expr->left && !expr->right) {
                        // Struct assignment from function call: origin = create_point(25, 30)
                        fprintf(output, "    # Struct assignment from function call\n");
                        
                        FunctionCallExpr* call = expr->func_call;
                        
                        // YZ_85: Check struct size to determine if hidden pointer is needed
                        StructDef* return_struct = struct_lookup_definition(target_inst->definition->name);
                        StructReturnMethod method = STRUCT_RETURN_HIDDEN_PTR;  // Default
                        int needs_hidden_ptr = 1;
                        
                        if (return_struct) {
                            method = struct_get_return_method(return_struct);
                            needs_hidden_ptr = (method != STRUCT_RETURN_REGISTER);
                        }
                        
                        const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
                        int param_start = needs_hidden_ptr ? 1 : 0;
                        
                        // If hidden pointer needed, pass target struct address
                        if (needs_hidden_ptr) {
                            fprintf(output, "    leaq -%d(%%rbp), %%rdi  # Pass target struct address as hidden pointer\n",
                                    target_inst->stack_offset);
                        }
                        
                        // Arguments start at appropriate register
                        for (int i = 0; i < call->arg_count && (i + param_start) < 6; i++) {
                            ArithmeticExpr* arg_expr = call->arguments[i];
                            arithmetic_generate_code(output, arg_expr, func);
                            fprintf(output, "    movq %%r8, %s  # Argument %d\n", 
                                    param_regs[i + param_start], i + 1);
                        }
                        
                        // Call function
                        fprintf(output, "    call %s\n", call->function_name);
                        
                        // If register return, copy from RAX+RDX to target
                        if (method == STRUCT_RETURN_REGISTER) {
                            fprintf(output, "    # Copy register return to %s\n", assign->name);
                            fprintf(output, "    movq %%rax, -%d(%%rbp)  # First 8 bytes\n",
                                    target_inst->stack_offset);
                            if (struct_get_size(return_struct) > 8) {
                                fprintf(output, "    movq %%rdx, -%d(%%rbp)  # Next 8 bytes\n",
                                        target_inst->stack_offset - 8);
                            }
                        } else {
                            fprintf(output, "    # Struct copied via hidden pointer to %s\n", assign->name);
                        }
                    } else {
                        // Regular assignment
                        int offset = function_get_var_offset(func, assign->name);
                        arithmetic_generate_code(output, expr, func);
                        
                        // Result is in r8, store to variable's stack location
                        fprintf(output, "    movq %%r8, %d(%%rbp)  # Store to %s\n", 
                                offset, assign->name);
                    }
                }
            }
            break;
        }
        
        case STMT_ARRAY_ASSIGNMENT: {
            // ✅ YZ_15: Array element assignment - arr[i] = value
            ArrayAssignment* arr_assign = (ArrayAssignment*)stmt->data;
            if (arr_assign && arr_assign->index_access) {
                IndexAccess* access = (IndexAccess*)arr_assign->index_access;  // Cast from void*
                
                fprintf(output, "    # Array assignment: %s[...] = ...\n", access->collection_name);
                
                // Step 1: Evaluate the value expression
                if (arr_assign->value_expr) {
                    ArithmeticExpr* value_expr = (ArithmeticExpr*)arr_assign->value_expr;
                    arithmetic_generate_code(output, value_expr, func);
                    // Result in %r8
                    fprintf(output, "    pushq %%r8           # Save value to store\n");
                }
                
                // Step 2: Get array pointer from stack
                int arr_offset = function_get_var_offset(func, access->collection_name);
                fprintf(output, "    movq %d(%%rbp), %%rbx # Load array pointer\n", arr_offset);
                
                // Step 3: Compute element offset based on index type
                if (access->index_type == 0) {
                    // Constant index: arr[0] = value
                    // Bounds check (compile-time known)
                    int arr_length = function_get_array_length(func, access->collection_name);
                    if (arr_length > 0 && access->index.const_index >= arr_length) {
                        fprintf(output, "    # COMPILE ERROR: Index %d >= length %d\n", 
                                access->index.const_index, arr_length);
                    } else if (arr_length > 0) {
                        // Runtime bounds check for safety
                        fprintf(output, "    # Bounds check: index=%d, length=%d\n", 
                                access->index.const_index, arr_length);
                        fprintf(output, "    movq $%d, %%rcx      # Load index to register\n", access->index.const_index);
                        fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                        fprintf(output, "    jl .bounds_ok_%p     # Jump if within bounds\n", (void*)access);
                        fprintf(output, "    movq $%d, %%rdi      # index\n", access->index.const_index);
                        fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                        fprintf(output, "    leaq .str_arr_%s(%%rip), %%rdx # array name\n", access->collection_name);
                        fprintf(output, "    call mlp_panic_array_bounds\n");
                        fprintf(output, ".bounds_ok_%p:\n", (void*)access);
                    }
                    int elem_offset = access->index.const_index * 8;
                    fprintf(output, "    popq %%r8            # Restore value\n");
                    fprintf(output, "    movq %%r8, %d(%%rbx)  # Store at index %d\n", 
                            elem_offset, access->index.const_index);
                } else if (access->index_type == 1) {
                    // Variable index: arr[i] = value
                    int idx_offset = function_get_var_offset(func, access->index.var_index);
                    int arr_length = function_get_array_length(func, access->collection_name);
                    
                    fprintf(output, "    movq %d(%%rbp), %%rcx # Load index variable\n", idx_offset);
                    
                    // Runtime bounds check
                    if (arr_length > 0) {
                        fprintf(output, "    # Bounds check: index variable vs length=%d\n", arr_length);
                        fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                        fprintf(output, "    jl .bounds_ok_%p     # Jump if within bounds\n", (void*)access);
                        fprintf(output, "    movq %%rcx, %%rdi    # index (already in rcx)\n");
                        fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                        fprintf(output, "    leaq .str_arr_%s(%%rip), %%rdx # array name\n", access->collection_name);
                        fprintf(output, "    call mlp_panic_array_bounds\n");
                        fprintf(output, ".bounds_ok_%p:\n", (void*)access);
                    }
                    
                    fprintf(output, "    shlq $3, %%rcx       # index * 8\n");
                    fprintf(output, "    popq %%r8            # Restore value\n");
                    fprintf(output, "    movq %%r8, (%%rbx,%%rcx) # Store at computed offset\n");
                } else if (access->index_type == 2) {
                    // Expression index: arr[x+1] = value
                    ArithmeticExpr* idx_expr = (ArithmeticExpr*)access->index.expr_index;
                    int arr_length = function_get_array_length(func, access->collection_name);
                    
                    fprintf(output, "    # Expression index: evaluate to get offset\n");
                    arithmetic_generate_code(output, idx_expr, func);
                    // Index result in %r8
                    fprintf(output, "    movq %%r8, %%rcx     # Move index to rcx\n");
                    
                    // Runtime bounds check
                    if (arr_length > 0) {
                        fprintf(output, "    # Bounds check: expression index vs length=%d\n", arr_length);
                        fprintf(output, "    cmpq $%d, %%rcx      # Compare index with length\n", arr_length);
                        fprintf(output, "    jl .bounds_ok_%p     # Jump if within bounds\n", (void*)access);
                        fprintf(output, "    movq %%rcx, %%rdi    # index\n");
                        fprintf(output, "    movq $%d, %%rsi      # length\n", arr_length);
                        fprintf(output, "    leaq .str_arr_%s(%%rip), %%rdx # array name\n", access->collection_name);
                        fprintf(output, "    call mlp_panic_array_bounds\n");
                        fprintf(output, ".bounds_ok_%p:\n", (void*)access);
                    }
                    
                    fprintf(output, "    shlq $3, %%rcx       # index * 8\n");
                    fprintf(output, "    popq %%r8            # Restore value\n");
                    fprintf(output, "    movq %%r8, (%%rbx,%%rcx) # Store at computed offset\n");
                }
            }
            break;
        }
        
        case STMT_RETURN: {
            // ✅ Return statement - evaluate expression and result in rax
            ReturnStatement* ret = (ReturnStatement*)stmt->data;
            
            // YZ_85: STO-based struct return handling
            if (func->return_type == FUNC_RETURN_STRUCT && ret && ret->return_value) {
                ArithmeticExpr* expr = (ArithmeticExpr*)ret->return_value;
                
                // Struct return: determine method based on STO analysis
                if (!expr->left && !expr->right && expr->value && !expr->is_literal) {
                    // It's a struct variable reference
                    fprintf(output, "    # Return struct: %s\n", expr->value);
                    
                    // Look up struct instance and definition
                    StructInstanceInfo* inst_info = struct_lookup_instance(expr->value);
                    if (inst_info && func->return_struct_type) {
                        StructDef* struct_def = struct_lookup_definition(func->return_struct_type);
                        if (struct_def) {
                            StructReturnMethod method = struct_get_return_method(struct_def);
                            size_t struct_size = struct_get_size(struct_def);
                            
                            fprintf(output, "    # STO return method: ");
                            
                            switch (method) {
                                case STRUCT_RETURN_REGISTER:
                                    // ≤16 bytes: Return in RAX + RDX
                                    fprintf(output, "REGISTER (size=%zu)\n", struct_size);
                                    fprintf(output, "    # Load struct to registers (RAX + RDX)\n");
                                    
                                    if (inst_info->is_pointer) {
                                        // Parameter (pointer) - load from pointer
                                        fprintf(output, "    movq %d(%%rbp), %%rax  # Load struct pointer\n", 
                                                inst_info->stack_offset);
                                        fprintf(output, "    movq 0(%%rax), %%rax   # First 8 bytes\n");
                                        if (struct_size > 8) {
                                            fprintf(output, "    movq %d(%%rbp), %%rdx  # Load struct pointer again\n", 
                                                    inst_info->stack_offset);
                                            fprintf(output, "    movq 8(%%rdx), %%rdx   # Next 8 bytes\n");
                                        }
                                    } else {
                                        // Local variable - load from stack
                                        fprintf(output, "    movq -%d(%%rbp), %%rax  # First 8 bytes of %s\n",
                                                inst_info->stack_offset, expr->value);
                                        if (struct_size > 8) {
                                            fprintf(output, "    movq -%d(%%rbp), %%rdx  # Next 8 bytes\n",
                                                    inst_info->stack_offset - 8);
                                        } else {
                                            fprintf(output, "    xor %%rdx, %%rdx        # Clear RDX\n");
                                        }
                                    }
                                    break;
                                    
                                case STRUCT_RETURN_STACK_COPY:
                                case STRUCT_RETURN_HIDDEN_PTR:
                                    // 17-64 bytes OR >64 bytes: Use hidden pointer + memcpy
                                    if (method == STRUCT_RETURN_STACK_COPY) {
                                        fprintf(output, "STACK_COPY (size=%zu)\n", struct_size);
                                    } else {
                                        fprintf(output, "HIDDEN_PTR (size=%zu)\n", struct_size);
                                    }
                                    
                                    // Load hidden pointer (stored at -8(rbp))
                                    fprintf(output, "    movq -8(%%rbp), %%rdi  # Load hidden return pointer\n");
                                    
                                    // Load source struct address
                                    if (inst_info->is_pointer) {
                                        // Parameter - load pointer
                                        fprintf(output, "    movq %d(%%rbp), %%rsi  # Load struct pointer (param)\n",
                                                inst_info->stack_offset);
                                    } else {
                                        // Local variable - calculate address
                                        fprintf(output, "    leaq -%d(%%rbp), %%rsi  # Source struct %s\n",
                                                inst_info->stack_offset, expr->value);
                                    }
                                    
                                    // Copy size
                                    fprintf(output, "    movq $%zu, %%rdx  # Struct size\n", struct_size);
                                    
                                    // Call memcpy
                                    fprintf(output, "    call memcpy  # Copy struct to caller\n");
                                    
                                    // Return pointer in rax
                                    fprintf(output, "    movq -8(%%rbp), %%rax  # Return struct pointer\n");
                                    break;
                            }
                        }
                    }
                }
            } else if (ret && ret->return_value) {
                ArithmeticExpr* expr = (ArithmeticExpr*)ret->return_value;
                
                // Special case: simple variable reference
                if (!expr->left && !expr->right && expr->value && !expr->is_literal) {
                    // YZ_121: Check if variable is const
                    int is_const_var = 0;
                    int64_t const_val = 0;
                    LocalVariable* var = func->local_vars;
                    while (var) {
                        if (strcmp(var->name, expr->value) == 0) {
                            if (var->is_const) {
                                is_const_var = 1;
                                const_val = var->const_value;
                            }
                            break;
                        }
                        var = var->next;
                    }
                    
                    if (is_const_var) {
                        // Load immediate const value
                        fprintf(output, "    movq $%ld, %%r8  # Load const %s\n", 
                                const_val, expr->value);
                    } else {
                        // Load from stack
                        int offset = function_get_var_offset(func, expr->value);
                        fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", 
                                offset, expr->value);
                    }
                } else {
                    // Complex expression - use arithmetic codegen
                    arithmetic_generate_code(output, expr, func);
                }
                
                // Move result from r8 to rax (x86-64 return convention)
                fprintf(output, "    movq %%r8, %%rax  # Return value\n");
            } else {
                // No return value (void return)
                fprintf(output, "    # Return (void)\n");
                fprintf(output, "    xor %%rax, %%rax\n");
            }
            
            // YZ_41: Jump to function epilogue
            fprintf(output, "    jmp .L%s_return\n", func->name);
            break;
        }
        
        // YZ_28: Exit statement - VB.NET style exit from blocks
        // exit for, exit while - jump to loop end
        case STMT_EXIT:
        case STMT_EXIT_FOR:
        case STMT_EXIT_WHILE: {
            int break_label = get_break_label();
            if (break_label >= 0) {
                fprintf(output, "    # Exit - jump to loop end\n");
                fprintf(output, "    jmp .loop_end_%d\n", break_label);
            } else {
                fprintf(output, "    # Error: exit outside loop\n");
            }
            break;
        }
        
        // YZ_28: Exit if - jump to end of if block (guard clause pattern)
        // This is used instead of "continue" - put code after if block
        case STMT_EXIT_IF: {
            // exit if just ends the if block early, the code after if continues
            // This is handled by the if codegen - we just need to jump to end_if label
            // For now, this is a no-op since we're already at end of if
            fprintf(output, "    # Exit if - guard clause (code continues after if)\n");
            break;
        }
        
        // YZ_28: Exit function - early return (void)
        case STMT_EXIT_FUNCTION: {
            fprintf(output, "    # Exit function - early return\n");
            fprintf(output, "    xor %%rax, %%rax\n");
            fprintf(output, "    movq %%rbp, %%rsp\n");
            fprintf(output, "    popq %%rbp\n");
            fprintf(output, "    ret\n");
            break;
        }
        
        // YZ_90: Continue statement - jump to loop increment (not start!)
        case STMT_CONTINUE:
        case STMT_CONTINUE_FOR: {
            int continue_label = get_continue_label();
            if (continue_label >= 0) {
                fprintf(output, "    # Continue - jump to for loop increment\n");
                fprintf(output, "    jmp .for_continue_%d\n", continue_label);
            } else {
                fprintf(output, "    # Error: continue outside loop\n");
            }
            break;
        }
        
        case STMT_CONTINUE_WHILE: {
            int continue_label = get_continue_label();
            if (continue_label >= 0) {
                fprintf(output, "    # Continue - jump to while loop start\n");
                fprintf(output, "    jmp .while_start_%d\n", continue_label);
            } else {
                fprintf(output, "    # Error: continue outside loop\n");
            }
            break;
        }
        
        // YZ_81: Struct definition - just emit documentation comment
        case STMT_STRUCT: {
            StructDef* struct_def = (StructDef*)stmt->data;
            if (struct_def) {
                codegen_struct_definition(output, struct_def);
            }
            break;
        }
        
        // YZ_96: Enum definition - compile-time only, emit comment
        case STMT_ENUM: {
            EnumDefinition* enum_def = (EnumDefinition*)stmt->data;
            if (enum_def) {
                enum_codegen_comment(output, enum_def);
            }
            break;
        }
        
        // YZ_82: Struct instance declaration - allocate stack space
        case STMT_STRUCT_INSTANCE: {
            StructInstance* instance = (StructInstance*)stmt->data;
            if (instance && instance->definition) {
                fprintf(output, "    # Struct instance: %s %s (%zu bytes)\n", 
                        instance->struct_name, instance->instance_name, 
                        instance->definition->total_size);
                
                // Allocate stack space
                size_t aligned_size = instance->definition->total_size;
                if (aligned_size % 16 != 0) {
                    aligned_size = ((aligned_size / 16) + 1) * 16;
                }
                
                current_stack_offset += aligned_size;
                
                fprintf(output, "    subq $%zu, %%rsp  # Allocate %s\n", 
                        aligned_size, instance->instance_name);
                
                // Register instance for member access
                struct_register_instance(instance->instance_name, 
                                        instance->definition, 
                                        current_stack_offset);
                
                fprintf(output, "    # %s is at [rbp - %d] (REGISTERED)\n", 
                        instance->instance_name, current_stack_offset);
            }
            break;
        }
        
        // YZ_101: Enum variable declaration
        case STMT_ENUM_VARIABLE: {
            EnumVariable* enum_var = (EnumVariable*)stmt->data;
            if (enum_var) {
                fprintf(output, "    # Enum variable: %s %s\n", 
                        enum_var->enum_type, enum_var->var_name);
                
                // Allocate 8 bytes on stack (int64)
                current_stack_offset += 8;
                
                // YZ_102: Register enum variable in function context for lookups
                // Create LocalVariable entry manually to sync with current_stack_offset
                if (func) {
                    LocalVariable* var = malloc(sizeof(LocalVariable));
                    var->name = strdup(enum_var->var_name);
                    var->is_numeric = 1;  // Enums are numeric (int64)
                    var->is_array = 0;
                    var->array_length = 0;
                    var->is_tuple = 0;
                    var->tuple_length = 0;
                    var->is_list = 0;
                    var->list_length = 0;
                    var->stack_offset = -current_stack_offset;  // Negative offset from rbp
                    
                    // Add to front of local_vars list
                    var->next = func->local_vars;
                    func->local_vars = var;
                    func->local_var_count++;
                }
                
                if (enum_var->has_initializer) {
                    // Store initial value
                    fprintf(output, "    movq $%ld, %%rax  # %s = %ld\n", 
                            enum_var->init_value, enum_var->var_name, enum_var->init_value);
                    fprintf(output, "    movq %%rax, -%d(%%rbp)  # Store at [rbp - %d]\n",
                            current_stack_offset, current_stack_offset);
                } else {
                    // Uninitialized - just reserve space
                    fprintf(output, "    # %s at [rbp - %d] (uninitialized)\n",
                            enum_var->var_name, current_stack_offset);
                }
            }
            break;
        }
        
        // YZ_82/YZ_83: Member assignment - store value at struct offset (nested support)
        case STMT_MEMBER_ASSIGNMENT: {
            MemberAssignment* mem_assign = (MemberAssignment*)stmt->data;
            if (mem_assign) {
                fprintf(output, "    # Member assignment: %s", mem_assign->instance_name);
                for (int i = 0; i < mem_assign->chain_length; i++) {
                    fprintf(output, ".%s", mem_assign->member_chain[i]);
                }
                fprintf(output, " = ...\n");
                
                // Look up struct instance
                StructInstanceInfo* inst_info = struct_lookup_instance(mem_assign->instance_name);
                if (!inst_info) {
                    fprintf(output, "    # ERROR: Instance '%s' not found\n", 
                            mem_assign->instance_name);
                    break;
                }
                
                // YZ_83: Traverse member chain for nested assignment
                StructDef* current_def = inst_info->definition;
                int cumulative_offset = 0;
                
                for (int chain_idx = 0; chain_idx < mem_assign->chain_length; chain_idx++) {
                    const char* member_name = mem_assign->member_chain[chain_idx];
                    
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
                        goto end_member_assign;
                    }
                    
                    // Add member's offset to cumulative offset
                    cumulative_offset += member->offset;
                    
                    // If this is a nested struct and not the last in chain, continue
                    if (chain_idx < mem_assign->chain_length - 1) {
                        if (!member->nested_def) {
                            fprintf(output, "    # ERROR: Member '%s' is not a struct\n", member_name);
                            goto end_member_assign;
                        }
                        current_def = member->nested_def;  // Traverse into nested struct
                    }
                }
                
                // Evaluate expression
                ArithmeticExpr* expr = (ArithmeticExpr*)mem_assign->value_expr;
                arithmetic_generate_code(output, expr, func);  // YZ_85: Pass func context
                
                // Calculate final address: rbp - base_offset + cumulative offset
                int final_offset = inst_info->stack_offset - cumulative_offset;
                
                fprintf(output, "    # Store to [rbp - %d + %d] = [rbp - %d]\n",
                        inst_info->stack_offset, cumulative_offset, final_offset);
                
                // Store value (assuming numeric for now)
                fprintf(output, "    movq %%r8, %d(%%rbp)  # Store nested member\n",
                        -final_offset);
                
                end_member_assign:
                ; // Empty statement for label
            }
            break;
        }
        
        default:
            fprintf(output, "    # Unknown statement type: %d\n", stmt->type);
            break;
    }
}
