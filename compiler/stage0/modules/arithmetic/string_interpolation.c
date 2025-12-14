#include "string_interpolation.h"
#include "arithmetic.h"
#include "arithmetic_parser.h"
#include "../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Check if string contains ${...} interpolation
int string_has_interpolation(const char* str) {
    if (!str) return 0;
    return strstr(str, "${") != NULL;
}

// Helper: Create string literal expression
static ArithmeticExpr* create_string_literal(const char* str) {
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    memset(expr, 0, sizeof(ArithmeticExpr));
    
    expr->is_literal = 1;
    expr->value = strdup(str);
    expr->is_string = 1;
    expr->is_float = 0;
    expr->is_boolean = 0;
    
    return expr;
}

// Helper: Create number-to-string conversion call
static ArithmeticExpr* create_to_string_call(ArithmeticExpr* numeric_expr) {
    FunctionCallExpr* func_call = malloc(sizeof(FunctionCallExpr));
    func_call->function_name = strdup("mlp_number_to_string");
    func_call->arg_count = 1;
    func_call->arguments = malloc(sizeof(ArithmeticExpr*));
    func_call->arguments[0] = numeric_expr;
    
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    memset(expr, 0, sizeof(ArithmeticExpr));
    
    expr->is_function_call = 1;
    expr->func_call = func_call;
    expr->is_string = 1;  // Result is a string
    
    return expr;
}

// Helper: Create concatenation expression (left + right)
static ArithmeticExpr* create_concat(ArithmeticExpr* left, ArithmeticExpr* right) {
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    memset(expr, 0, sizeof(ArithmeticExpr));
    
    expr->op = ARITH_ADD;  // String concatenation uses +
    expr->left = left;
    expr->right = right;
    expr->is_string = 1;
    
    return expr;
}

// Parse string interpolation
// Example: "Hello ${name}, age ${age}" 
// -> "Hello " + name + ", age " + mlp_number_to_string(age)
ArithmeticExpr* parse_string_interpolation(Lexer* lexer, const char* str_with_interp) {
    if (!str_with_interp || !string_has_interpolation(str_with_interp)) {
        // No interpolation, return as-is
        return create_string_literal(str_with_interp);
    }
    
    ArithmeticExpr* result = NULL;
    const char* pos = str_with_interp;
    char buffer[4096];  // Buffer for string parts
    
    while (*pos) {
        // Find next ${
        const char* interp_start = strstr(pos, "${");
        
        if (!interp_start) {
            // No more interpolations, add remaining string
            if (*pos) {
                ArithmeticExpr* str_part = create_string_literal(pos);
                if (result) {
                    result = create_concat(result, str_part);
                } else {
                    result = str_part;
                }
            }
            break;
        }
        
        // Add string part before ${
        if (interp_start > pos) {
            size_t len = interp_start - pos;
            if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;
            strncpy(buffer, pos, len);
            buffer[len] = '\0';
            
            ArithmeticExpr* str_part = create_string_literal(buffer);
            if (result) {
                result = create_concat(result, str_part);
            } else {
                result = str_part;
            }
        }
        
        // Find closing }
        const char* interp_end = strchr(interp_start + 2, '}');
        if (!interp_end) {
            error_fatal("String interpolation: missing closing } after ${");
            return result;
        }
        
        // Extract expression between ${ and }
        size_t expr_len = interp_end - (interp_start + 2);
        if (expr_len >= sizeof(buffer)) {
            error_fatal("String interpolation: expression too long");
            return result;
        }
        
        strncpy(buffer, interp_start + 2, expr_len);
        buffer[expr_len] = '\0';
        
        // Parse the expression
        // For now, we'll handle simple cases: variables and numbers
        // Full expression parsing would require creating a mini lexer
        
        // Simple approach: check if it's a number literal or variable
        // If it looks like a number, wrap in to_string
        // Otherwise, treat as string variable
        
        int is_numeric = 1;
        const char* p = buffer;
        
        // Skip whitespace
        while (*p && isspace(*p)) p++;
        
        // Check if it starts with digit or minus (numeric literal)
        if (*p == '-' || (*p >= '0' && *p <= '9')) {
            // Looks like a number
            is_numeric = 1;
        } else {
            // Assume it's a variable - we can't know the type without context
            // For now, we'll NOT wrap in to_string - let codegen handle it
            // This means: string variables work as-is, numeric variables need explicit conversion
            // Better solution: check variable type from symbol table
            is_numeric = 0;  // Assume string variable for now
        }
        
        ArithmeticExpr* var_expr = malloc(sizeof(ArithmeticExpr));
        memset(var_expr, 0, sizeof(ArithmeticExpr));
        
        if (is_numeric && (*buffer >= '0' && *buffer <= '9')) {
            // It's a numeric literal
            var_expr->is_literal = 1;
            var_expr->value = strdup(buffer);
            var_expr->is_float = (strchr(buffer, '.') != NULL);
        } else {
            // It's a variable
            var_expr->is_literal = 0;
            var_expr->value = strdup(buffer);
        }
        
        // TODO YZ_90: Proper type inference from symbol table
        // For now: Always wrap in to_string (it should handle both strings and numbers)
        // But we need a smarter to_string that checks type at runtime or compile-time
        
        // Temporarily: Just use the expression as-is (assume correct type)
        ArithmeticExpr* str_expr = var_expr;
        
        if (result) {
            result = create_concat(result, str_expr);
        } else {
            result = str_expr;
        }
        
        // Move past the interpolation
        pos = interp_end + 1;
    }
    
    return result ? result : create_string_literal("");
}
