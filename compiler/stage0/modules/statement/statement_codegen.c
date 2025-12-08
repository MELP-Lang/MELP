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
            // ✅ Variable declaration - allocate stack space
            VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
            if (decl) {
                FunctionDeclaration* func = (FunctionDeclaration*)context;
                fprintf(output, "    # Variable: %s\n", decl->name);
                
                // For local variables in functions, allocate on stack
                // Space already allocated in function prologue
                // Just need to initialize if there's an initial value
                
                if (decl->value) {
                    // TODO: Evaluate initial value and store
                    fprintf(output, "    # Initialize %s\n", decl->name);
                }
            }
            break;
        }
        
        case STMT_ASSIGNMENT: {
            // ✅ Variable assignment - evaluate expression and store
            VariableAssignment* assign = (VariableAssignment*)stmt->data;
            if (assign) {
                fprintf(output, "    # Assignment: %s = ...\n", assign->name);
                
                // Evaluate expression
                if (assign->value_expr) {
                    ArithmeticExpr* expr = (ArithmeticExpr*)assign->value_expr;
                    arithmetic_generate_code(output, expr);
                    
                    // Result is in r8, store to variable's stack location
                    // For now, assume it's a local variable at offset from rbp
                    // TODO: Look up variable's actual stack offset
                    fprintf(output, "    # Store result to %s (TODO: resolve offset)\n", assign->name);
                }
            }
            break;
        }
        
        case STMT_RETURN: {
            // ✅ Return statement - evaluate expression and result in rax
            ReturnStatement* ret = (ReturnStatement*)stmt->data;
            if (ret && ret->return_value) {
                // Evaluate expression, result goes to r8 (arithmetic convention)
                ArithmeticExpr* expr = (ArithmeticExpr*)ret->return_value;
                arithmetic_generate_code(output, expr);
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
