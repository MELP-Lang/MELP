#include "for_loop_parser.h"
#include "../statement/statement_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parse: for i = 0 to 10 ... end
// or:    for i = 10 downto 0 ... end
ForLoop* for_loop_parse(Lexer* lexer, Token* for_token) {
    // for_token is borrowed, don't free!
    
    ForLoop* loop = (ForLoop*)malloc(sizeof(ForLoop));
    loop->var_name = NULL;
    loop->start_value = 0;
    loop->end_value = 0;
    loop->direction = FOR_TO;
    loop->body = NULL;
    
    // Expect: identifier (loop variable)
    Token* var_tok = lexer_next_token(lexer);
    if (var_tok->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected variable name after 'for' at line %d\n", for_token->line);
        token_free(var_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->var_name = strdup(var_tok->value);
    token_free(var_tok);
    
    // Expect: =
    Token* assign_tok = lexer_next_token(lexer);
    if (assign_tok->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' after variable name at line %d\n", for_token->line);
        token_free(assign_tok);
        for_loop_free(loop);
        return NULL;
    }
    token_free(assign_tok);
    
    // Expect: start value (number)
    Token* start_tok = lexer_next_token(lexer);
    if (start_tok->type != TOKEN_NUMBER) {
        fprintf(stderr, "Error: Expected number after '=' at line %d\n", for_token->line);
        token_free(start_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->start_value = atoi(start_tok->value);
    token_free(start_tok);
    
    // Expect: 'to' or 'downto'
    Token* dir_tok = lexer_next_token(lexer);
    if (dir_tok->type == TOKEN_TO) {
        loop->direction = FOR_TO;
    } else if (dir_tok->type == TOKEN_DOWNTO) {
        loop->direction = FOR_DOWNTO;
    } else {
        fprintf(stderr, "Error: Expected 'to' or 'downto' at line %d\n", for_token->line);
        token_free(dir_tok);
        for_loop_free(loop);
        return NULL;
    }
    token_free(dir_tok);
    
    // Expect: end value (number)
    Token* end_tok = lexer_next_token(lexer);
    if (end_tok->type != TOKEN_NUMBER) {
        fprintf(stderr, "Error: Expected number after 'to'/'downto' at line %d\n", for_token->line);
        token_free(end_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->end_value = atoi(end_tok->value);
    token_free(end_tok);
    
    // Body will be parsed by statement_parser (like while loop)
    // Just return the ForLoop structure
    loop->body = NULL;  // Will be filled by statement_parser
    
    return loop;
}
