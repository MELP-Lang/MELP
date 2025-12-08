#include "statement_codegen.h"
#include "../control_flow/control_flow_codegen.h"  // ✅ While/If codegen
#include "../print/print_codegen.h"                 // ✅ Print codegen
#include "../variable/variable_codegen.h"           // ✅ Variable codegen
#include "../variable/variable.h"                   // ✅ VariableDeclaration, VariableAssignment
#include "../arithmetic/arithmetic_codegen.h"       // ✅ Expression codegen
#include "../arithmetic/arithmetic.h"               // ✅ ArithmeticExpr
#include "../functions/functions.h"                 // ✅ ReturnStatement, FunctionDeclaration
#include <stdio.h>

// Statement code generation with modular imports
void statement_generate_code(FILE* output, Statement* stmt, void* context) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_WHILE: {
            // ✅ Use control_flow module for while codegen
            WhileStatement* while_stmt = (WhileStatement*)stmt->data;
            if (while_stmt) {
                control_flow_generate_while(output, while_stmt, context);
            }
            break;
        }
        
        case STMT_IF: {
            // ✅ Use control_flow module for if codegen
            IfStatement* if_stmt = (IfStatement*)stmt->data;
            if (if_stmt) {
                control_flow_generate_if(output, if_stmt, context);
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
                FunctionDeclaration* func = (FunctionDeclaration*)context;
                
                // Get offset (already registered)
                int offset = function_get_var_offset(func, decl->name);
                
                fprintf(output, "    # Variable: %s at %d(%%rbp)\n", decl->name, offset);
                
                // Initialize if there's an initial value
                if (decl->init_expr) {
                    // ✅ NEW: init_expr is ArithmeticExpr*
                    ArithmeticExpr* expr = (ArithmeticExpr*)decl->init_expr;
                    printf("DEBUG: Variable %s has init_expr (ArithmeticExpr)\n", decl->name);
                    
                    // Generate expression code
                    arithmetic_generate_code(output, expr, context);
                    
                    // Store result to variable
                    fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                            offset, decl->name);
                } else if (decl->value) {
                    // Simple literal in value field
                    printf("DEBUG: Variable %s value='%s' (simple literal)\n", decl->name, decl->value);
                    fprintf(output, "    movq $%s, %%r8  # Literal value\n", decl->value);
                    fprintf(output, "    movq %%r8, %d(%%rbp)  # Initialize %s\n", 
                            offset, decl->name);
                } else {
                    printf("DEBUG: Variable %s has NO init_expr or value\n", decl->name);
                }
            }
            break;
        }
        
        case STMT_ASSIGNMENT: {
            // ✅ Variable assignment - evaluate expression and store
            VariableAssignment* assign = (VariableAssignment*)stmt->data;
            if (assign) {
                FunctionDeclaration* func = (FunctionDeclaration*)context;
                int offset = function_get_var_offset(func, assign->name);
                
                fprintf(output, "    # Assignment: %s = ...\n", assign->name);
                
                // Evaluate expression
                if (assign->value_expr) {
                    ArithmeticExpr* expr = (ArithmeticExpr*)assign->value_expr;
                    arithmetic_generate_code(output, expr, context);  // ✅ Pass context!
                    
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
                    FunctionDeclaration* func = (FunctionDeclaration*)context;
                    int offset = function_get_var_offset(func, expr->value);
                    fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", 
                            offset, expr->value);
                } else {
                    // Complex expression - use arithmetic codegen
                    arithmetic_generate_code(output, expr, context);  // ✅ Pass context!
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
