#include "arithmetic_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable counter for temporary values
static int temp_var_counter = 0;

// Generate C code for a value (literal or variable)
static void generate_value_expr(FILE* output, const char* value, int is_literal, int is_float, char* result_var) {
    if (is_literal) {
        sprintf(result_var, "%s", value);
    } else {
        sprintf(result_var, "%s", value);
    }
}

// Generate code recursively (postorder traversal)
static void generate_expr_code(FILE* output, ArithmeticExpr* expr, char* result_var) {
    if (!expr) return;
    
    // Leaf node (literal or variable)
    if (expr->is_literal || (!expr->left && !expr->right)) {
        if (expr->is_literal) {
            sprintf(result_var, "%s", expr->value);
        } else {
            sprintf(result_var, "%s", expr->value);
        }
        return;
    }
    
    // Binary operation
    char left_var[64], right_var[64];
    
    // Generate code for left subtree
    generate_expr_code(output, expr->left, left_var);
    
    // Generate code for right subtree
    generate_expr_code(output, expr->right, right_var);
    
    // Determine type
    int is_float = (expr->left && expr->left->is_float) || 
                   (expr->right && expr->right->is_float);
    
    const char* type = is_float ? "double" : "int64_t";
    sprintf(result_var, "temp_%d", temp_var_counter++);
    
    // Generate operation
    const char* op_str = "";
    switch (expr->op) {
        case ARITH_ADD: op_str = "+"; break;
        case ARITH_SUB: op_str = "-"; break;
        case ARITH_MUL: op_str = "*"; break;
        case ARITH_DIV: op_str = "/"; break;
        case ARITH_MOD: op_str = "%"; break;
        case ARITH_AND: op_str = "&"; break;
        case ARITH_OR: op_str = "|"; break;
        case ARITH_XOR: op_str = "^"; break;
        case ARITH_POW:
            // Power operation - use pow() function
            emit_c("    %s %s = pow(%s, %s);\n", type, result_var, left_var, right_var);
            return;
    }
    
    emit_c("    %s %s = %s %s %s;\n", type, result_var, left_var, op_str, right_var);
}

// Generate C code for arithmetic expression
void arithmetic_generate_code(FILE* output, ArithmeticExpr* expr) {
    if (!output || !expr) return;
    
    emit_c("\n    // Arithmetic expression\n");
    char result_var[64];
    generate_expr_code(output, expr, result_var);
    emit_c("    // Result: %s\n", result_var);
}

// Generate assignment statement: variable = expression
void arithmetic_generate_assignment(FILE* output, const char* var_name, ArithmeticExpr* expr) {
    if (!output || !var_name || !expr) return;
    
    emit_c("\n    // Assignment: %s = <expression>\n", var_name);
    
    char result_var[64];
    generate_expr_code(output, expr, result_var);
    
    // Generate assignment
    emit_c("    %s = %s;\n", var_name, result_var);
}
