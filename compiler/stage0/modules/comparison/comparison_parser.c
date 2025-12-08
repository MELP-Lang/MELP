#include "comparison_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// NEW STATELESS TEMPLATE FUNCTION
// ============================================================================

// Parse comparison expression (stateless - borrowing pattern)
ComparisonExpr* comparison_parse_expression_stateless(Lexer* lexer, Token* first_token) {
    // Caller owns first_token - we borrow it, don't free it!
    if (!lexer || !first_token) {
        return NULL;
    }
    
    ComparisonExpr* expr = malloc(sizeof(ComparisonExpr));
    expr->left = NULL;
    expr->right = NULL;
    expr->op = CMP_EQUAL;  // Default
    expr->left_is_literal = 0;
    expr->right_is_literal = 0;
    expr->left_value = NULL;
    expr->right_value = NULL;
    expr->is_float = 0;
    
    // Parse left operand (use first_token)
    if (first_token->type == TOKEN_NUMBER) {
        expr->left_value = strdup(first_token->value);
        expr->left_is_literal = 1;
        expr->is_float = (strchr(expr->left_value, '.') != NULL);
    } else if (first_token->type == TOKEN_IDENTIFIER) {
        expr->left_value = strdup(first_token->value);
        expr->left_is_literal = 0;
    } else {
        free(expr);
        return NULL;
    }
    
    // Read operator token - WE own this, WE free it
    Token* op_tok = lexer_next_token(lexer);
    if (!op_tok) {
        free(expr->left_value);
        free(expr);
        return NULL;
    }
    
    // Parse operator
    if (op_tok->type == TOKEN_EQUAL) expr->op = CMP_EQUAL;
    else if (op_tok->type == TOKEN_NOT_EQUAL) expr->op = CMP_NOT_EQUAL;
    else if (op_tok->type == TOKEN_LESS) expr->op = CMP_LESS;
    else if (op_tok->type == TOKEN_LESS_EQUAL) expr->op = CMP_LESS_EQUAL;
    else if (op_tok->type == TOKEN_GREATER) expr->op = CMP_GREATER;
    else if (op_tok->type == TOKEN_GREATER_EQUAL) expr->op = CMP_GREATER_EQUAL;
    else {
        // Not a comparison operator
        token_free(op_tok);
        free(expr->left_value);
        free(expr);
        return NULL;
    }
    
    token_free(op_tok);  // Done with operator
    
    // Read right operand - WE own this, WE free it
    Token* right_tok = lexer_next_token(lexer);
    if (!right_tok) {
        free(expr->left_value);
        free(expr);
        return NULL;
    }
    
    // Parse right operand
    if (right_tok->type == TOKEN_NUMBER) {
        expr->right_value = strdup(right_tok->value);
        expr->right_is_literal = 1;
        if (!expr->is_float) {
            expr->is_float = (strchr(expr->right_value, '.') != NULL);
        }
    } else if (right_tok->type == TOKEN_IDENTIFIER) {
        expr->right_value = strdup(right_tok->value);
        expr->right_is_literal = 0;
    } else {
        token_free(right_tok);
        free(expr->left_value);
        free(expr);
        return NULL;
    }
    
    token_free(right_tok);  // Done with right operand
    
    return expr;
}
