#include "string_interpolation.h"
#include "arithmetic.h"
#include "arithmetic_parser.h"
#include "../../lexer/lexer.h"
#include "../../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Check if string contains ${...} interpolation
int string_has_interpolation(const char* str) {
    if (!str) return 0;
    return strstr(str, "${") != NULL;
}

// YZ_92: Helper to check if expression contains operators or function calls
static int is_simple_identifier(const char* expr_str) {
    if (!expr_str) return 0;
    
    // Skip whitespace
    while (*expr_str && isspace(*expr_str)) expr_str++;
    
    // Check if starts with letter or underscore (valid identifier start)
    if (!(*expr_str == '_' || isalpha(*expr_str))) return 0;
    
    // Check rest: only alphanumeric and underscore = simple identifier
    while (*expr_str) {
        if (isalnum(*expr_str) || *expr_str == '_') {
            expr_str++;
        } else if (isspace(*expr_str)) {
            // Skip trailing whitespace
            while (*expr_str && isspace(*expr_str)) expr_str++;
            return *expr_str == '\0';  // Valid if nothing after whitespace
        } else {
            // Found operator or other character - not simple
            return 0;
        }
    }
    return 1;
}

// YZ_92: Parse expression string using full expression parser
static ArithmeticExpr* parse_interpolation_expression(const char* expr_str) {
    if (!expr_str || !*expr_str) return NULL;
    
    // Create mini-lexer for the expression
    Lexer* mini_lexer = lexer_create(expr_str);
    if (!mini_lexer) return NULL;
    
    // Get first token
    Token* first_token = lexer_next_token(mini_lexer);
    if (!first_token || first_token->type == TOKEN_EOF || first_token->type == TOKEN_ERROR) {
        if (first_token) token_free(first_token);
        lexer_free(mini_lexer);
        return NULL;
    }
    
    // Parse expression using stateless API
    // NULL func for string interpolation context (no local variable scope)
    ArithmeticExpr* expr = arithmetic_parse_expression_stateless(mini_lexer, first_token, NULL);
    
    // First token is consumed by parser, don't free it
    lexer_free(mini_lexer);
    
    return expr;
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
// YZ_92: Now supports full expressions: "Sum: ${x + y}"
// Example: "Hello ${name}, sum is ${a + b}" 
// -> "Hello " + name + ", sum is " + mlp_number_to_string(a + b)
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
        
        // YZ_92: Handle nested braces - find matching closing }
        const char* interp_end = NULL;
        int brace_depth = 1;
        const char* scan = interp_start + 2;  // Start after ${
        
        while (*scan && brace_depth > 0) {
            if (*scan == '{') {
                brace_depth++;
            } else if (*scan == '}') {
                brace_depth--;
                if (brace_depth == 0) {
                    interp_end = scan;
                }
            }
            scan++;
        }
        
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
        
        // YZ_92: Use full expression parser for complex expressions
        ArithmeticExpr* interp_expr = NULL;
        
        if (is_simple_identifier(buffer)) {
            // Simple variable: create basic expression
            ArithmeticExpr* var_expr = malloc(sizeof(ArithmeticExpr));
            memset(var_expr, 0, sizeof(ArithmeticExpr));
            var_expr->is_literal = 0;
            var_expr->value = strdup(buffer);
            // Skip whitespace for the value
            char* trimmed = var_expr->value;
            while (*trimmed && isspace(*trimmed)) trimmed++;
            if (trimmed != var_expr->value) {
                char* new_val = strdup(trimmed);
                free(var_expr->value);
                var_expr->value = new_val;
            }
            // Trim trailing whitespace
            size_t len = strlen(var_expr->value);
            while (len > 0 && isspace(var_expr->value[len-1])) {
                var_expr->value[--len] = '\0';
            }
            interp_expr = var_expr;
        } else {
            // Complex expression: use full parser
            interp_expr = parse_interpolation_expression(buffer);
            
            if (!interp_expr) {
                // Fallback: treat as simple variable
                fprintf(stderr, "Warning: Could not parse interpolation expression: %s\n", buffer);
                ArithmeticExpr* var_expr = malloc(sizeof(ArithmeticExpr));
                memset(var_expr, 0, sizeof(ArithmeticExpr));
                var_expr->is_literal = 0;
                var_expr->value = strdup(buffer);
                interp_expr = var_expr;
            }
        }
        
        // YZ_92: Check if expression is likely numeric (needs to_string conversion)
        // An expression with operators is numeric unless it's string concatenation
        int needs_conversion = 0;
        if (interp_expr) {
            // If it has operators (left/right), it's likely numeric arithmetic
            if (interp_expr->left && interp_expr->right) {
                // Has binary operation - check if it's not string concat
                // String concat would have is_string=1 on operands
                if (!interp_expr->is_string) {
                    needs_conversion = 1;
                }
            }
            // Numeric literals need conversion
            else if (interp_expr->is_literal && !interp_expr->is_string) {
                needs_conversion = 1;
            }
        }
        
        // Wrap in to_string if numeric expression
        ArithmeticExpr* str_expr = interp_expr;
        if (needs_conversion && interp_expr) {
            str_expr = create_to_string_call(interp_expr);
        }
        
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
