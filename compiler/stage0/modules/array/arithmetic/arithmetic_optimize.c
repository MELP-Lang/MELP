#include "arithmetic_optimize.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Check if expression is a constant (literal only)
int arithmetic_is_constant(ArithmeticExpr* expr) {
    if (!expr) return 0;
    
    // Literal values are constants
    if (expr->is_literal) return 1;
    
    // Function calls, array access, collections are NOT constants
    if (expr->is_function_call || expr->is_array_access || expr->is_collection) {
        return 0;
    }
    
    // Binary operations are constant if both sides are constant
    if (expr->left && expr->right) {
        return arithmetic_is_constant(expr->left) && arithmetic_is_constant(expr->right);
    }
    
    return 0;
}

// Evaluate binary operation on two numeric constants
double arithmetic_eval_numeric(double left, double right, ArithmeticOp op) {
    switch (op) {
        case ARITH_ADD: return left + right;
        case ARITH_SUB: return left - right;
        case ARITH_MUL: return left * right;
        case ARITH_DIV: 
            if (right == 0.0) {
                fprintf(stderr, "Error: Division by zero in constant folding\n");
                return 0.0;
            }
            return left / right;
        case ARITH_MOD:
            if ((long long)right == 0) {
                fprintf(stderr, "Error: Modulo by zero in constant folding\n");
                return 0.0;
            }
            return (long long)left % (long long)right;
        case ARITH_POW: return pow(left, right);
        case ARITH_AND: return (long long)left & (long long)right;
        case ARITH_OR:  return (long long)left | (long long)right;
        case ARITH_XOR: return (long long)left ^ (long long)right;
        default:
            fprintf(stderr, "Error: Unknown operation in constant folding\n");
            return 0.0;
    }
}

// Constant folding optimization
ArithmeticExpr* arithmetic_optimize_constant_fold(ArithmeticExpr* expr) {
    if (!expr) return NULL;
    
    // If it's a function call, array access, or collection, no folding
    if (expr->is_function_call || expr->is_array_access || expr->is_collection) {
        return expr;
    }
    
    // If it's a literal, already constant
    if (expr->is_literal) {
        return expr;
    }
    
    // Binary operation: fold children first
    if (expr->left && expr->right) {
        expr->left = arithmetic_optimize_constant_fold(expr->left);
        expr->right = arithmetic_optimize_constant_fold(expr->right);
        
        // Check if both sides are now constants
        if (arithmetic_is_constant(expr->left) && arithmetic_is_constant(expr->right)) {
            
            // String concatenation folding
            if (expr->is_string && expr->op == ARITH_ADD) {
                if (expr->left->is_string && expr->right->is_string) {
                    // "Hello" + "World" → "HelloWorld"
                    size_t len1 = strlen(expr->left->value);
                    size_t len2 = strlen(expr->right->value);
                    char* result = malloc(len1 + len2 + 1);
                    strcpy(result, expr->left->value);
                    strcat(result, expr->right->value);
                    
                    // Free old expression tree
                    arithmetic_expr_free(expr->left);
                    arithmetic_expr_free(expr->right);
                    
                    // Create new literal
                    expr->left = NULL;
                    expr->right = NULL;
                    expr->is_literal = 1;
                    expr->value = result;
                    expr->is_string = 1;
                    expr->is_float = 0;
                    expr->is_boolean = 0;
                    
                    return expr;
                }
            }
            
            // Numeric folding
            if (!expr->is_string && expr->left->is_literal && expr->right->is_literal &&
                !expr->left->is_string && !expr->right->is_string) {
                
                // Parse numeric values
                double left_val = atof(expr->left->value);
                double right_val = atof(expr->right->value);
                
                // Evaluate
                double result = arithmetic_eval_numeric(left_val, right_val, expr->op);
                
                // Free old expression tree
                arithmetic_expr_free(expr->left);
                arithmetic_expr_free(expr->right);
                
                // Create new literal
                expr->left = NULL;
                expr->right = NULL;
                expr->is_literal = 1;
                expr->is_float = (expr->left && expr->left->is_float) || 
                                (expr->right && expr->right->is_float) ||
                                (result != (long long)result);
                expr->is_string = 0;
                expr->is_boolean = 0;
                
                // Format result
                char buffer[64];
                if (expr->is_float) {
                    snprintf(buffer, sizeof(buffer), "%.10g", result);
                } else {
                    snprintf(buffer, sizeof(buffer), "%lld", (long long)result);
                }
                expr->value = strdup(buffer);
                
                return expr;
            }
        }
    }
    
    return expr;
}
