#include "comparison_parser.h"
#include "../arithmetic/arithmetic_parser.h"  // YZ_29: For member access support
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
    expr->is_string = 0;  // YZ_07: Initialize string flag
    
    // ✅ Phase 3.2: Initialize logical chaining fields
    expr->chain_op = LOG_NONE;
    expr->next = NULL;
    expr->is_negated = 0;
    
    // YZ_18: Handle boolean literals and variables (if flag syntax)
    if (first_token->type == TOKEN_TRUE || first_token->type == TOKEN_FALSE) {
        // Boolean literal: if true or if false
        expr->left_value = strdup(first_token->value);
        expr->left_is_literal = 1;
        expr->op = CMP_EQUAL;  // Treat as "boolean == 1" implicitly
        expr->right_value = strdup("1");
        expr->right_is_literal = 1;
        return expr;  // Single boolean, no comparison operator
    } else if (first_token->type == TOKEN_IDENTIFIER) {
        // Check if this is a boolean variable without comparison operator
        Token* lookahead = lexer_next_token(lexer);
        if (lookahead && (lookahead->type == TOKEN_THEN || lookahead->type == TOKEN_AND || 
                          lookahead->type == TOKEN_OR || lookahead->type == TOKEN_RPAREN)) {
            // Boolean variable: if flag then...
            lexer_unget_token(lexer, lookahead);
            expr->left_value = strdup(first_token->value);
            expr->left_is_literal = 0;
            expr->op = CMP_EQUAL;  // Treat as "flag == 1" implicitly
            expr->right_value = strdup("1");
            expr->right_is_literal = 1;
            return expr;
        }
        // Not a boolean condition, continue with normal comparison
        if (lookahead) {
            lexer_unget_token(lexer, lookahead);
        }
    }
    
    // Parse left operand (use first_token)
    if (first_token->type == TOKEN_NUMBER) {
        expr->left_value = strdup(first_token->value);
        expr->left_is_literal = 1;
        expr->is_float = (strchr(expr->left_value, '.') != NULL);
    } else if (first_token->type == TOKEN_MINUS) {  // YZ_41: Negative number support
        // Read the number after minus
        Token* num_tok = lexer_next_token(lexer);
        if (!num_tok || num_tok->type != TOKEN_NUMBER) {
            token_free(num_tok);
            free(expr);
            return NULL;
        }
        // Create negative value: "-" + number
        char negative_buf[256];
        snprintf(negative_buf, sizeof(negative_buf), "-%s", num_tok->value);
        expr->left_value = strdup(negative_buf);
        expr->left_is_literal = 1;
        expr->is_float = (strchr(num_tok->value, '.') != NULL);
        token_free(num_tok);
    } else if (first_token->type == TOKEN_STRING) {  // YZ_07: String literal support
        expr->left_value = strdup(first_token->value);
        expr->left_is_literal = 1;
        expr->is_string = 1;
    } else if (first_token->type == TOKEN_LBRACKET) {  // YZ_26: Empty list literal support []
        // Check if next token is ']' for empty list
        Token* next = lexer_next_token(lexer);
        if (next && next->type == TOKEN_RBRACKET) {
            expr->left_value = strdup("[]");
            expr->left_is_literal = 1;
            token_free(next);
        } else {
            // Not empty list, unget token and fail
            if (next) lexer_unget_token(lexer, next);
            free(expr);
            return NULL;
        }
    } else if (first_token->type == TOKEN_IDENTIFIER) {
        // YZ_29: Check for member access (identifier.member) or array access (identifier[index])
        Token* lookahead = lexer_next_token(lexer);
        
        if (lookahead && lookahead->type == TOKEN_DOT) {
            // Member access detected: identifier.member
            Token* member_tok = lexer_next_token(lexer);
            if (!member_tok || member_tok->type != TOKEN_IDENTIFIER) {
                token_free(lookahead);
                if (member_tok) token_free(member_tok);
                free(expr);
                return NULL;
            }
            
            // Build member access string: "identifier.member"
            size_t len = strlen(first_token->value) + strlen(member_tok->value) + 2;  // +2 for '.' and '\0'
            expr->left_value = malloc(len);
            snprintf(expr->left_value, len, "%s.%s", first_token->value, member_tok->value);
            expr->left_is_literal = 0;  // Member access is not a literal
            
            token_free(lookahead);
            token_free(member_tok);
        } else if (lookahead && lookahead->type == TOKEN_LBRACKET) {
            // Array access detected: identifier[index]
            // Read index (should be number or identifier)
            Token* index_tok = lexer_next_token(lexer);
            if (!index_tok) {
                token_free(lookahead);
                free(expr);
                return NULL;
            }
            
            // Expect closing bracket
            Token* rbracket = lexer_next_token(lexer);
            if (!rbracket || rbracket->type != TOKEN_RBRACKET) {
                token_free(lookahead);
                token_free(index_tok);
                if (rbracket) token_free(rbracket);
                free(expr);
                return NULL;
            }
            
            // Build array access string: "identifier[index]"
            size_t len = strlen(first_token->value) + strlen(index_tok->value) + 3;  // +3 for '[', ']', '\0'
            expr->left_value = malloc(len);
            snprintf(expr->left_value, len, "%s[%s]", first_token->value, index_tok->value);
            expr->left_is_literal = 0;  // Array access is not a literal
            
            token_free(lookahead);
            token_free(index_tok);
            token_free(rbracket);
        } else {
            // Simple identifier, no member/array access
            if (lookahead) {
                lexer_unget_token(lexer, lookahead);
            }
            expr->left_value = strdup(first_token->value);
            expr->left_is_literal = 0;
        }
        // TODO: Check variable type from symbol table for is_string
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
    } else if (right_tok->type == TOKEN_MINUS) {  // YZ_41: Negative number support
        token_free(right_tok);  // Free the minus token first
        // Read the number after minus
        Token* num_tok = lexer_next_token(lexer);
        if (!num_tok || num_tok->type != TOKEN_NUMBER) {
            token_free(num_tok);
            free(expr->left_value);
            free(expr);
            return NULL;
        }
        // Create negative value: "-" + number
        char negative_buf[256];
        snprintf(negative_buf, sizeof(negative_buf), "-%s", num_tok->value);
        expr->right_value = strdup(negative_buf);
        expr->right_is_literal = 1;
        if (!expr->is_float) {
            expr->is_float = (strchr(num_tok->value, '.') != NULL);
        }
        token_free(num_tok);
        right_tok = NULL;  // We already freed it
    } else if (right_tok->type == TOKEN_STRING) {  // YZ_07: String literal support
        expr->right_value = strdup(right_tok->value);
        expr->right_is_literal = 1;
        expr->is_string = 1;
    } else if (right_tok->type == TOKEN_LBRACKET) {  // YZ_26: Empty list literal support []
        // Check if next token is ']' for empty list
        Token* next = lexer_next_token(lexer);
        if (next && next->type == TOKEN_RBRACKET) {
            expr->right_value = strdup("[]");
            expr->right_is_literal = 1;
            token_free(next);
            token_free(right_tok);
            right_tok = NULL;
        } else {
            // Not empty list, unget token and fail
            if (next) lexer_unget_token(lexer, next);
            token_free(right_tok);
            free(expr->left_value);
            free(expr);
            return NULL;
        }
    } else if (right_tok->type == TOKEN_IDENTIFIER) {
        // YZ_29: Check for member access (identifier.member) or array access (identifier[index])
        Token* lookahead = lexer_next_token(lexer);
        
        if (lookahead && lookahead->type == TOKEN_DOT) {
            // Member access detected: identifier.member
            Token* member_tok = lexer_next_token(lexer);
            if (!member_tok || member_tok->type != TOKEN_IDENTIFIER) {
                token_free(lookahead);
                if (member_tok) token_free(member_tok);
                token_free(right_tok);
                free(expr->left_value);
                free(expr);
                return NULL;
            }
            
            // Build member access string: "identifier.member"
            size_t len = strlen(right_tok->value) + strlen(member_tok->value) + 2;  // +2 for '.' and '\0'
            expr->right_value = malloc(len);
            snprintf(expr->right_value, len, "%s.%s", right_tok->value, member_tok->value);
            expr->right_is_literal = 0;  // Member access is not a literal
            
            token_free(lookahead);
            token_free(member_tok);
            token_free(right_tok);
            right_tok = NULL;
        } else if (lookahead && lookahead->type == TOKEN_LBRACKET) {
            // Array access detected: identifier[index]
            // Read index (should be number or identifier)
            Token* index_tok = lexer_next_token(lexer);
            if (!index_tok) {
                token_free(lookahead);
                token_free(right_tok);
                free(expr->left_value);
                free(expr);
                return NULL;
            }
            
            // Expect closing bracket
            Token* rbracket = lexer_next_token(lexer);
            if (!rbracket || rbracket->type != TOKEN_RBRACKET) {
                token_free(lookahead);
                token_free(index_tok);
                if (rbracket) token_free(rbracket);
                token_free(right_tok);
                free(expr->left_value);
                free(expr);
                return NULL;
            }
            
            // Build array access string: "identifier[index]"
            size_t len = strlen(right_tok->value) + strlen(index_tok->value) + 3;  // +3 for '[', ']', '\0'
            expr->right_value = malloc(len);
            snprintf(expr->right_value, len, "%s[%s]", right_tok->value, index_tok->value);
            expr->right_is_literal = 0;  // Array access is not a literal
            
            token_free(lookahead);
            token_free(index_tok);
            token_free(rbracket);
            token_free(right_tok);
            right_tok = NULL;
        } else {
            // Simple identifier, no member/array access
            if (lookahead) {
                lexer_unget_token(lexer, lookahead);
            }
            expr->right_value = strdup(right_tok->value);
            expr->right_is_literal = 0;
        }
        // TODO: Check variable type from symbol table for is_string
    } else {
        token_free(right_tok);
        free(expr->left_value);
        free(expr);
        return NULL;
    }
    
    if (right_tok) {  // YZ_41: Only free if not already freed (negative number case)
        token_free(right_tok);
    }
    
    // ✅ Phase 3.2: Check for logical operators (and, or)
    Token* chain_tok = lexer_next_token(lexer);
    if (chain_tok && (chain_tok->type == TOKEN_AND || chain_tok->type == TOKEN_OR)) {
        // Set chain operator
        if (chain_tok->type == TOKEN_AND) {
            expr->chain_op = LOG_AND;
        } else {
            expr->chain_op = LOG_OR;
        }
        token_free(chain_tok);
        
        // Parse next comparison recursively
        Token* next_first = lexer_next_token(lexer);
        if (next_first) {
            expr->next = comparison_parse_expression_stateless(lexer, next_first);
            token_free(next_first);
            
            if (!expr->next) {
                // Failed to parse next comparison
                comparison_expr_free(expr);
                return NULL;
            }
        }
    } else if (chain_tok) {
        // Not a logical operator, push it back
        lexer_unget_token(lexer, chain_tok);
    }
    
    return expr;
}
