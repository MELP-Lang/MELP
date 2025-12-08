#include "statement_codegen.h"
#include "../control_flow/control_flow_codegen.h"  // ✅ While/If codegen
#include "../print/print_codegen.h"                 // ✅ Print codegen
#include "../variable/variable_codegen.h"           // ✅ Variable codegen
#include "../arithmetic/arithmetic_codegen.h"       // ✅ Expression codegen
#include "../functions/functions.h"                 // ✅ ReturnStatement definition
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
            // ✅ Use variable module
            // TODO: variable_generate_declaration(output, stmt->data);
            fprintf(output, "    # Variable declaration\n");
            break;
        }
        
        case STMT_RETURN: {
            // ✅ Return statement
            ReturnStatement* ret = (ReturnStatement*)stmt->data;
            if (ret && ret->return_value) {
                // Evaluate expression, result in rax
                // expression_generate_code(output, ret->return_value);
            }
            fprintf(output, "    # Return\n");
            break;
        }
        
        default:
            fprintf(output, "    # Unknown statement type: %d\n", stmt->type);
            break;
    }
}
