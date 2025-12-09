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
#include <string.h>

// Global counter for string literals
static int string_literal_counter = 0;

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
            VariableAssignment* assign = (VariableAssignment*)stmt->data;
            if (assign) {
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
                    int elem_offset = access->index.const_index * 8;
                    fprintf(output, "    popq %%r8            # Restore value\n");
                    fprintf(output, "    movq %%r8, %d(%%rbx)  # Store at index %d\n", 
                            elem_offset, access->index.const_index);
                } else if (access->index_type == 1) {
                    // Variable index: arr[i] = value
                    int idx_offset = function_get_var_offset(func, access->index.var_index);
                    fprintf(output, "    movq %d(%%rbp), %%rcx # Load index variable\n", idx_offset);
                    fprintf(output, "    shlq $3, %%rcx       # index * 8\n");
                    fprintf(output, "    popq %%r8            # Restore value\n");
                    fprintf(output, "    movq %%r8, (%%rbx,%%rcx) # Store at computed offset\n");
                } else if (access->index_type == 2) {
                    // Expression index: arr[x+1] = value (TODO)
                    fprintf(output, "    # TODO: Expression index not yet supported\n");
                    fprintf(output, "    popq %%r8            # Clean stack\n");
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
        
        default:
            fprintf(output, "    # Unknown statement type: %d\n", stmt->type);
            break;
    }
}
