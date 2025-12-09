#include "logical_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// STATELESS PATTERN: No parser struct, no advance() helper
// Token ownership: Token** current is a mutable pointer we manage
// - We own *current and must free it before replacing
// - Caller owns the initial token passed to entry point

// Parse primary (boolean literal or variable)
// current is a pointer to our current token - we manage it
LogicalExpr* logical_parse_primary_stateless(Lexer* lexer, Token** current) {
    if (!lexer || !current || !*current) {
        return NULL;
    }
    
    LogicalExpr* expr = malloc(sizeof(LogicalExpr));
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    
    // Boolean literal
    if ((*current)->type == TOKEN_TRUE || 
        (*current)->type == TOKEN_FALSE) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        token_free(*current);
        *current = lexer_next_token(lexer);
        return expr;
    }
    
    // Variable
    if ((*current)->type == TOKEN_IDENTIFIER) {
        expr->is_literal = 0;
        expr->value = strdup((*current)->value);
        token_free(*current);
        *current = lexer_next_token(lexer);
        return expr;
    }
    
    // Parentheses
    if ((*current)->type == TOKEN_LPAREN) {
        token_free(*current);
        *current = lexer_next_token(lexer);
        free(expr);
        expr = logical_parse_or_stateless(lexer, current);
        
        if (!*current || (*current)->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            logical_expr_free(expr);
            return NULL;
        }
        token_free(*current);
        *current = lexer_next_token(lexer);
        return expr;
    }
    
    free(expr);
    return NULL;
}

// Parse NOT (highest precedence)
LogicalExpr* logical_parse_not_stateless(Lexer* lexer, Token** current) {
    if (!lexer || !current || !*current) {
        return NULL;
    }
    
    // Check for NOT operator
    if ((*current)->type == TOKEN_NOT) {
        token_free(*current);
        *current = lexer_next_token(lexer);
        
        LogicalExpr* expr = malloc(sizeof(LogicalExpr));
        expr->op = LOG_NOT;
        expr->left = logical_parse_not_stateless(lexer, current);  // Right-associative
        expr->right = NULL;
        expr->is_literal = 0;
        expr->value = NULL;
        
        if (!expr->left) {
            free(expr);
            return NULL;
        }
        
        return expr;
    }
    
    return logical_parse_primary_stateless(lexer, current);
}

// Parse AND
LogicalExpr* logical_parse_and_stateless(Lexer* lexer, Token** current) {
    LogicalExpr* left = logical_parse_not_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current && (*current)->type == TOKEN_AND) {
        token_free(*current);
        *current = lexer_next_token(lexer);
        
        LogicalExpr* right = logical_parse_not_stateless(lexer, current);
        if (!right) {
            logical_expr_free(left);
            return NULL;
        }
        
        LogicalExpr* binary = malloc(sizeof(LogicalExpr));
        binary->op = LOG_AND;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        
        left = binary;
    }
    
    return left;
}

// Parse OR (lowest precedence)
LogicalExpr* logical_parse_or_stateless(Lexer* lexer, Token** current) {
    LogicalExpr* left = logical_parse_and_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current && (*current)->type == TOKEN_OR) {
        token_free(*current);
        *current = lexer_next_token(lexer);
        
        LogicalExpr* right = logical_parse_and_stateless(lexer, current);
        if (!right) {
            logical_expr_free(left);
            return NULL;
        }
        
        LogicalExpr* binary = malloc(sizeof(LogicalExpr));
        binary->op = LOG_OR;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        
        left = binary;
    }
    
    return left;
}

// Parse expression (entry point)
// first_token is BORROWED - we don't free it, caller owns it
// We make a copy to manage internally
LogicalExpr* logical_parse_expression_stateless(Lexer* lexer, Token* first_token) {
    if (!lexer || !first_token) return NULL;
    
    // We need to manage our own token pointer for recursive calls
    // Copy the first token so we can free it later
    Token* current = malloc(sizeof(Token));
    current->type = first_token->type;
    current->value = first_token->value ? strdup(first_token->value) : NULL;
    current->line = first_token->line;
    
    LogicalExpr* result = logical_parse_or_stateless(lexer, &current);
    
    // Clean up our internal token if still exists
    if (current) {
        token_free(current);
    }
    
    return result;
}
