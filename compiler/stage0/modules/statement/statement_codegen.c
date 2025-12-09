#include "statement_codegen.h"
#include "../control_flow/control_flow_codegen.h"  // ✅ While/If codegen
#include "../for_loop/for_loop_codegen.h"           // ✅ For loop codegen
#include "../print/print_codegen.h"                 // ✅ Print codegen
#include "../variable/variable_codegen.h"           // ✅ Variable codegen
#include "../variable/variable.h"                   // ✅ VariableDeclaration, VariableAssignment
#include "../arithmetic/arithmetic_codegen.h"       // ✅ Expression codegen
#include "../arithmetic/arithmetic.h"               // ✅ ArithmeticExpr
#include "../functions/functions.h"                 // ✅ ReturnStatement, FunctionDeclaration
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
                
                // Initialize if there's an initial value
                if (decl->init_expr) {
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
