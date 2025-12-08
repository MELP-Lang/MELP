#include "print_parser.h"
#include "../lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PrintStatement* parse_print_statement(Lexer* lexer) {
    // Expect 'print' keyword
    Token* tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_PRINT) {
        fprintf(stderr, "Error: Expected 'print' keyword\n");
        token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // Expect '('
    tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after 'print'\n");
        token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // Expect string literal or identifier (variable)
    tok = lexer_next_token(lexer);
    
    PrintStatement* stmt = malloc(sizeof(PrintStatement));
    
    if (tok->type == TOKEN_STRING) {
        // String literal: print("Hello")
        stmt->type = PRINT_STRING_LITERAL;
        stmt->value = strdup(tok->value);
        token_free(tok);
    } else if (tok->type == TOKEN_IDENTIFIER) {
        // Variable name: print(x)
        stmt->type = PRINT_VARIABLE;
        stmt->value = strdup(tok->value);
        token_free(tok);
    } else {
        fprintf(stderr, "Error: Expected string literal or variable name in print()\n");
        token_free(tok);
        free(stmt);
        return NULL;
    }
    
    // Expect ')'
    tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_RPAREN) {
        fprintf(stderr, "Error: Expected ')' after string\n");
        token_free(tok);
        free(stmt->value);
        free(stmt);
        return NULL;
    }
    token_free(tok);
    
    return stmt;
}
