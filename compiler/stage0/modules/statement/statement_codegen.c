#include "statement_codegen.h"
#include "../control_flow/control_flow_codegen.h"  // ✅ While/If codegen
#include "../for_loop/for_loop_codegen.h"           // ✅ For loop codegen
#include "../print/print_codegen.h"                 // ✅ Print codegen
#include "../variable/variable_codegen.h"           // ✅ Variable codegen
#include "../variable/variable.h"                   // ✅ VariableDeclaration, VariableAssignment
#include "../arithmetic/arithmetic_codegen.h"       // ✅ Expression codegen
#include "../arithmetic/arithmetic.h"               // ✅ ArithmeticExpr
#include "../functions/functions.h"                 // ✅ ReturnStatement, FunctionDeclaration
#include "../array/array.h"                         // ✅ YZ_15: IndexAccess, ArrayAssignment
#include <stdio.h>
#include <stdlib.h>   // YZ_21: For free()
#include <string.h>

// Global counter for string literals
static int string_literal_counter = 0;

// YZ_28: Loop stack for exit support (VB.NET style)
LoopContext loop_stack[MAX_LOOP_DEPTH];
int loop_stack_top = -1;

void loop_push(int exit_label) {
    if (loop_stack_top < MAX_LOOP_DEPTH - 1) {
        loop_stack_top++;
        loop_stack[loop_stack_top].exit_label = exit_label;
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
        
        case STMT_PRINT: {
            // ✅ Use print module
            // TODO: print_generate_code(output, stmt->data);
            fprintf(output, "    # Print statement\n");
            break;
        }
        
        case STMT_VARIABLE_DECL: {
            // ✅ Variable declaration - already registered in pre-scan, just initialize
            VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
            if (decl) {
                // Get offset (already registered)
                int offset = function_get_var_offset(func, decl->name);
                
                fprintf(output, "    # Variable: %s at %d(%%rbp)\n", decl->name, offset);
                
                // ✅ NEW: Handle array initialization
                if (decl->is_array && decl->value && decl->value[0] == '[') {
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
                    
                    // Allocate array via TTO runtime
                    fprintf(output, "    # Allocate array with %d elements\n", element_count);
                    fprintf(output, "    movq $%d, %%rdi      # count\n", element_count);
                    fprintf(output, "    movq $8, %%rsi       # elem_size (8 bytes)\n");
                    fprintf(output, "    call tto_array_alloc # Returns pointer in %%rax\n");
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
                // YZ_25: Check if variable exists
                int var_exists = function_var_exists(func, assign->name);
                
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
                
                int offset = function_get_var_offset(func, assign->name);
                
                fprintf(output, "    # Assignment: %s = ...\n", assign->name);
                
                // Evaluate expression
                if (assign->value_expr) {
                    ArithmeticExpr* expr = (ArithmeticExpr*)assign->value_expr;
                    arithmetic_generate_code(output, expr, func);
                    
                    // Result is in r8, store to variable's stack location
                    fprintf(output, "    movq %%r8, %d(%%rbp)  # Store to %s\n", 
                            offset, assign->name);
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
            if (ret && ret->return_value) {
                ArithmeticExpr* expr = (ArithmeticExpr*)ret->return_value;
                
                // Special case: simple variable reference
                if (!expr->left && !expr->right && expr->value && !expr->is_literal) {
                    // It's a variable reference - load from stack
                    int offset = function_get_var_offset(func, expr->value);
                    fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", 
                            offset, expr->value);
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
        
        default:
            fprintf(output, "    # Unknown statement type: %d\n", stmt->type);
            break;
    }
}
