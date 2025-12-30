#include "logical_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generate C code recursively
static void generate_expr_code(FILE* output, LogicalExpr* expr, char* result_var) {
    if (!expr) return;
    
    // Leaf node
    if (expr->is_literal || (!expr->left && !expr->right)) {
        if (expr->is_literal) {
            sprintf(result_var, "%s", expr->value);
        } else {
            sprintf(result_var, "%s", expr->value);
        }
        return;
    }
    
    // NOT operator (unary)
    if (expr->op == LOG_NOT) {
        char left_var[64];
        generate_expr_code(output, expr->left, left_var);
        sprintf(result_var, "!(%s)", left_var);
        return;
    }
    
    // AND operator
    if (expr->op == LOG_AND) {
        char left_var[64], right_var[64];
        generate_expr_code(output, expr->left, left_var);
        generate_expr_code(output, expr->right, right_var);
        sprintf(result_var, "(%s && %s)", left_var, right_var);
        return;
    }
    
    // OR operator
    if (expr->op == LOG_OR) {
        char left_var[64], right_var[64];
        generate_expr_code(output, expr->left, left_var);
        generate_expr_code(output, expr->right, right_var);
        sprintf(result_var, "(%s || %s)", left_var, right_var);
        return;
    }
}

// Generate logical expression code
void logical_generate_code(FILE* output, LogicalExpr* expr) {
    if (!output || !expr) return;
    
    emit_c("\n    // Logical expression\n");
    char result_var[64];
    generate_expr_code(output, expr, result_var);
    emit_c("    // Result: %s\n", result_var);
}
