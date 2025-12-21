#include "print_parser.h"
#include "../array/arithmetic/arithmetic_parser.h"  // ✅ For expression parsing
#include "../lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// YZ_61: Stateless pattern - receives already-read 'print' token
// YZ_23: Support both print(...) and print ... syntax
PrintStatement* parse_print_statement(Lexer* lexer, Token* print_token) {
    // Verify it's actually a print token (sanity check)
    if (print_token->type != TOKEN_PRINT) {
        fprintf(stderr, "Error: Expected 'print' keyword\n");
        return NULL;
    }
    
    // YZ_23: Check if next token is '(' (optional parentheses)
    Token* tok = lexer_next_token(lexer);
    int has_parens = 0;
    
    if (tok->type == TOKEN_LPAREN) {
        // Variant 1: print(expr) - with parentheses
        has_parens = 1;
        token_free(tok);
        tok = lexer_next_token(lexer);
    }
    // else: Variant 2: print expr - without parentheses (tok already has first token)
    
    // Parse expression using arithmetic parser (supports all expressions including array indexing)
    
    PrintStatement* stmt = malloc(sizeof(PrintStatement));
    
    if (tok->type == TOKEN_STRING) {
        // String literal: print("Hello")
        stmt->type = PRINT_STRING_LITERAL;
        stmt->value = strdup(tok->value);
        token_free(tok);
    } else if (tok->type == TOKEN_IDENTIFIER) {
        // Could be variable or expression starting with identifier
        // Peek ahead to check for operators or indexing
        Token* next = lexer_next_token(lexer);
        
        if (next && (next->type == TOKEN_LBRACKET ||   // arr[i]
                     next->type == TOKEN_PLUS ||        // x + y
                     next->type == TOKEN_MINUS ||
                     next->type == TOKEN_MULTIPLY ||
                     next->type == TOKEN_DIVIDE)) {
            // It's an expression - use arithmetic parser
            lexer_unget_token(lexer, next);
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok, NULL);
            if (!expr) {
                token_free(tok);
                free(stmt);
                return NULL;
            }
            
            stmt->type = PRINT_EXPRESSION;
            stmt->value = (char*)expr;  // Store ArithmeticExpr* as void*
            // Don't free tok - arithmetic parser consumed it
        } else {
            // Simple variable: print(x) - convert to expression
            if (next) lexer_unget_token(lexer, next);
            
            // Create simple variable expression
            ArithmeticExpr* var_expr = arithmetic_parse_expression_stateless(lexer, tok, NULL);
            if (!var_expr) {
                token_free(tok);
                free(stmt);
                return NULL;
            }
            
            stmt->type = PRINT_EXPRESSION;
            stmt->value = (char*)var_expr;
            // Don't free tok - arithmetic parser consumed it
        }
    } else {
        // Try to parse as general expression (numbers, etc.)
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok, NULL);
        if (!expr) {
            fprintf(stderr, "Error: Expected expression in print()\n");
            token_free(tok);
            free(stmt);
            return NULL;
        }
        
        stmt->type = PRINT_EXPRESSION;
        stmt->value = (char*)expr;  // Store ArithmeticExpr* as void*
        // Don't free tok - arithmetic parser consumed it
    }
    
    // YZ_23: Expect ')' only if we had opening '('
    if (has_parens) {
        tok = lexer_next_token(lexer);
        if (tok->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')' after expression in print()\n");
            token_free(tok);
            free(stmt->value);
            free(stmt);
            return NULL;
        }
        token_free(tok);
    }
    
    return stmt;
}
