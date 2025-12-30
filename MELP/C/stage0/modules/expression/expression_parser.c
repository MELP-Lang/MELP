#include "expression_parser.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../arithmetic/arithmetic.h"
#include <stdlib.h>
#include <string.h>

// Helper: Peek next token without consuming
static Token* peek_next_token(Parser* parser) {
    if (!parser || !parser->lexer) return NULL;
    
    // Save current position (simplified - lexer needs to support this)
    Token* current = parser->current_token;
    Token* next = lexer_next_token(parser->lexer);
    
    // Note: This is simplified. Real implementation needs lexer state save/restore
    return next;
}

// Helper: Convert ArithmeticExpr to Expression
static Expression* arithmetic_to_expression(ArithmeticExpr* arith) {
    if (!arith) return NULL;
    
    Expression* expr = expression_create(EXPR_BINARY);
    
    // Leaf node (literal or variable)
    if (!arith->left && !arith->right) {
        if (arith->is_literal) {
            if (arith->is_float) {
                expr->type = EXPR_NUMBER;
                expr->data.number_value = atol(arith->value);
            } else {
                expr->type = EXPR_NUMBER;
                expr->data.number_value = atol(arith->value);
            }
        } else {
            expr->type = EXPR_VARIABLE;
            expr->data.var_name = strdup(arith->value);
        }
        return expr;
    }
    
    // Binary operation
    expr->type = EXPR_BINARY;
    expr->data.binary.left = arithmetic_to_expression(arith->left);
    expr->data.binary.right = arithmetic_to_expression(arith->right);
    
    // Map operator
    switch (arith->op) {
        case ARITH_ADD: expr->data.binary.operator = strdup("+"); break;
        case ARITH_SUB: expr->data.binary.operator = strdup("-"); break;
        case ARITH_MUL: expr->data.binary.operator = strdup("*"); break;
        case ARITH_DIV: expr->data.binary.operator = strdup("/"); break;
        case ARITH_MOD: expr->data.binary.operator = strdup("%"); break;
        case ARITH_POW: expr->data.binary.operator = strdup("**"); break;
        default: expr->data.binary.operator = strdup("+"); break;
    }
    
    return expr;
}

// Parse function call as expression
static Expression* parse_function_call_expr(Parser* parser) {
    if (!parser || !parser->current_token) return NULL;
    
    Expression* expr = expression_create(EXPR_CALL);
    expr->data.call.func_name = strdup(parser->current_token->value);
    expr->data.call.arg_count = 0;
    expr->data.call.args = NULL;
    
    // Consume function name
    Token* old = parser->current_token;
    parser->current_token = lexer_next_token(parser->lexer);
    token_free(old);
    
    // Expect '('
    if (!parser->current_token || parser->current_token->type != TOKEN_LPAREN) {
        return expr;
    }
    
    old = parser->current_token;
    parser->current_token = lexer_next_token(parser->lexer);
    token_free(old);
    
    // Parse arguments (simplified - just count for now)
    int max_args = 10;
    expr->data.call.args = malloc(sizeof(Expression*) * max_args);
    
    while (parser->current_token && parser->current_token->type != TOKEN_RPAREN) {
        // Recursively parse argument expression
        Expression* arg = expression_parse(parser);
        if (arg && expr->data.call.arg_count < max_args) {
            expr->data.call.args[expr->data.call.arg_count++] = arg;
        }
        
        // Check for comma
        if (parser->current_token && parser->current_token->type == TOKEN_COMMA) {
            old = parser->current_token;
            parser->current_token = lexer_next_token(parser->lexer);
            token_free(old);
        }
    }
    
    // Consume ')'
    if (parser->current_token && parser->current_token->type == TOKEN_RPAREN) {
        old = parser->current_token;
        parser->current_token = lexer_next_token(parser->lexer);
        token_free(old);
    }
    
    return expr;
}

// Main expression parser
Expression* expression_parse(Parser* parser) {
    if (!parser || !parser->current_token) return NULL;
    
    Token* token = parser->current_token;
    
    // Check for function call: identifier followed by '('
    if (token->type == TOKEN_IDENTIFIER) {
        // Peek ahead for '('
        Token* next = peek_next_token(parser);
        if (next && next->type == TOKEN_LPAREN) {
            token_free(next);
            return parse_function_call_expr(parser);
        }
        if (next) token_free(next);
    }
    
    // Handle string literals
    if (token->type == TOKEN_STRING) {
        Expression* expr = expression_create(EXPR_STRING);
        expr->data.string_value = strdup(token->value);
        
        Token* old = parser->current_token;
        parser->current_token = lexer_next_token(parser->lexer);
        token_free(old);
        
        return expr;
    }
    
    // Handle boolean literals
    if (token->type == TOKEN_TRUE || token->type == TOKEN_FALSE) {
        Expression* expr = expression_create(EXPR_BOOLEAN);
        expr->data.boolean_value = (token->type == TOKEN_TRUE) ? 1 : 0;
        
        Token* old = parser->current_token;
        parser->current_token = lexer_next_token(parser->lexer);
        token_free(old);
        
        return expr;
    }
    
    // Delegate to arithmetic parser for numeric expressions
    if (token->type == TOKEN_NUMBER || 
        token->type == TOKEN_IDENTIFIER ||
        token->type == TOKEN_LPAREN) {
        
        ArithmeticParser* arith = arithmetic_parser_create(parser->lexer);
        arith->current_token = parser->current_token;
        parser->current_token = NULL;  // Transfer ownership
        
        ArithmeticExpr* arith_expr = arithmetic_parse_expression(arith);
        
        // Convert ArithmeticExpr to Expression
        Expression* expr = arithmetic_to_expression(arith_expr);
        
        // Take back token
        parser->current_token = arith->current_token;
        arith->current_token = NULL;
        arithmetic_parser_free(arith);
        
        if (arith_expr) {
            arithmetic_expr_free(arith_expr);
        }
        
        return expr;
    }
    
    // Fallback: return NULL
    return NULL;
}
