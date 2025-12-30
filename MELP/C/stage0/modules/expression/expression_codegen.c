#include "expression_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>
#include <string.h>

// Forward declaration for recursive calls
void expression_generate_code(FILE* output, Expression* expr);

void expression_generate_code(FILE* output, Expression* expr) {
    if (!expr) {
        emit_c("0");  // Fallback
        return;
    }
    
    switch (expr->type) {
        case EXPR_NUMBER:
            emit_c("%ld", expr->data.number_value);
            break;
            
        case EXPR_STRING:
            emit_c("\"%s\"", expr->data.string_value ? expr->data.string_value : "");
            break;
            
        case EXPR_BOOLEAN:
            emit_c("%s", expr->data.boolean_value ? "true" : "false");
            break;
            
        case EXPR_VARIABLE:
            if (expr->data.var_name) {
                emit_c("var_%s", expr->data.var_name);
            } else {
                emit_c("0");
            }
            break;
            
        case EXPR_BINARY:
            if (!expr->data.binary.left || !expr->data.binary.right) {
                emit_c("0");
                break;
            }
            
            emit_c("(");
            expression_generate_code(output, expr->data.binary.left);
            
            // Map operator to C
            const char* op = " + ";
            if (expr->data.binary.operator) {
                if (strcmp(expr->data.binary.operator, "+") == 0) op = " + ";
                else if (strcmp(expr->data.binary.operator, "-") == 0) op = " - ";
                else if (strcmp(expr->data.binary.operator, "*") == 0) op = " * ";
                else if (strcmp(expr->data.binary.operator, "/") == 0) op = " / ";
                else if (strcmp(expr->data.binary.operator, "%") == 0) op = " % ";
                else if (strcmp(expr->data.binary.operator, "**") == 0) {
                    // Power operator - needs special handling
                    emit_c("pow(");
                    expression_generate_code(output, expr->data.binary.left);
                    emit_c(", ");
                    expression_generate_code(output, expr->data.binary.right);
                    emit_c(")");
                    return;  // Early return
                }
                else if (strcmp(expr->data.binary.operator, "==") == 0) op = " == ";
                else if (strcmp(expr->data.binary.operator, "!=") == 0) op = " != ";
                else if (strcmp(expr->data.binary.operator, "<") == 0) op = " < ";
                else if (strcmp(expr->data.binary.operator, ">") == 0) op = " > ";
                else if (strcmp(expr->data.binary.operator, "<=") == 0) op = " <= ";
                else if (strcmp(expr->data.binary.operator, ">=") == 0) op = " >= ";
                else if (strcmp(expr->data.binary.operator, "&&") == 0) op = " && ";
                else if (strcmp(expr->data.binary.operator, "||") == 0) op = " || ";
                else if (strcmp(expr->data.binary.operator, "&") == 0) op = " & ";
                else if (strcmp(expr->data.binary.operator, "|") == 0) op = " | ";
                else if (strcmp(expr->data.binary.operator, "^") == 0) op = " ^ ";
            }
            
            emit_c(op);
            expression_generate_code(output, expr->data.binary.right);
            emit_c(")");
            break;
            
        case EXPR_UNARY:
            if (!expr->data.unary.operand) {
                emit_c("0");
                break;
            }
            
            if (expr->data.unary.operator) {
                emit_c("%s", expr->data.unary.operator);
            }
            expression_generate_code(output, expr->data.unary.operand);
            break;
            
        case EXPR_CALL:
            if (!expr->data.call.func_name) {
                emit_c("0");
                break;
            }
            
            // Special handling for yazdir/print
            if (strcmp(expr->data.call.func_name, "yazdir") == 0 ||
                strcmp(expr->data.call.func_name, "print") == 0) {
                emit_c("printf(");
                
                // First argument (format string)
                if (expr->data.call.arg_count > 0 && expr->data.call.args[0]) {
                    expression_generate_code(output, expr->data.call.args[0]);
                } else {
                    emit_c("\"\"");
                }
                
                emit_c("\\n");
                
                // Additional arguments (if any)
                for (int i = 1; i < expr->data.call.arg_count; i++) {
                    emit_c(", ");
                    expression_generate_code(output, expr->data.call.args[i]);
                }
                
                emit_c(")");
            } else {
                // Regular function call
                emit_c("%s(", expr->data.call.func_name);
                
                for (int i = 0; i < expr->data.call.arg_count; i++) {
                    if (i > 0) emit_c(", ");
                    if (expr->data.call.args[i]) {
                        expression_generate_code(output, expr->data.call.args[i]);
                    } else {
                        emit_c("0");
                    }
                }
                
                emit_c(")");
            }
            break;
            
        case EXPR_ARRAY_ACCESS:
            emit_c("/* array access - TODO */");
            break;
            
        case EXPR_STRUCT_ACCESS:
            emit_c("/* struct access - TODO */");
            break;
            
        default:
            emit_c("0");  // Unknown type
            break;
    }
}
