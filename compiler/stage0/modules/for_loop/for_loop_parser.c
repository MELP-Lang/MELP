#include "for_loop_parser.h"
#include "../statement/statement_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// YZ_28: Parse for-each loop: for each item in collection
// Returns ForLoop with loop_type = FOR_TYPE_EACH
static ForLoop* for_each_parse(Lexer* lexer, Token* for_token, Token* each_token) {
    ForLoop* loop = (ForLoop*)malloc(sizeof(ForLoop));
    loop->loop_type = FOR_TYPE_EACH;
    loop->var_name = NULL;
    loop->collection_name = NULL;
    loop->start_value = 0;
    loop->end_value = 0;
    loop->direction = FOR_TO;
    loop->body = NULL;
    
    // each_token is TOKEN_EACH, already consumed
    // Don't free it - caller manages it
    
    // Expect: identifier (loop variable - e.g., "item")
    Token* var_tok = lexer_next_token(lexer);
    if (var_tok->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected variable name after 'for each' at line %d\n", for_token->line);
        token_free(var_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->var_name = strdup(var_tok->value);
    token_free(var_tok);
    
    // Expect: 'in' keyword
    Token* in_tok = lexer_next_token(lexer);
    if (in_tok->type != TOKEN_IN) {
        fprintf(stderr, "Error: Expected 'in' after variable name at line %d\n", for_token->line);
        token_free(in_tok);
        for_loop_free(loop);
        return NULL;
    }
    token_free(in_tok);
    
    // Expect: identifier (collection name - e.g., "arr")
    Token* coll_tok = lexer_next_token(lexer);
    if (coll_tok->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected collection name after 'in' at line %d\n", for_token->line);
        token_free(coll_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->collection_name = strdup(coll_tok->value);
    token_free(coll_tok);
    
    // Body will be parsed by statement_parser (like while loop)
    loop->body = NULL;
    
    return loop;
}

// Parse: for i = 0 to 10 ... end
// or:    for i = 10 downto 0 ... end
// or:    for each item in collection ... end (YZ_28)
ForLoop* for_loop_parse(Lexer* lexer, Token* for_token) {
    // for_token is borrowed, don't free!
    
    // Peek next token to check for "each" keyword (for-each loop)
    Token* next_tok = lexer_next_token(lexer);
    if (next_tok->type == TOKEN_EACH) {
        // YZ_28: for each item in collection
        ForLoop* loop = for_each_parse(lexer, for_token, next_tok);
        token_free(next_tok);  // We consumed TOKEN_EACH
        return loop;
    }
    
    // Not for-each, continue with range for loop
    // next_tok should be the loop variable identifier
    ForLoop* loop = (ForLoop*)malloc(sizeof(ForLoop));
    loop->loop_type = FOR_TYPE_RANGE;
    loop->var_name = NULL;
    loop->collection_name = NULL;
    loop->start_value = 0;
    loop->end_value = 0;
    loop->direction = FOR_TO;
    loop->body = NULL;
    
    // next_tok should be identifier (loop variable)
    if (next_tok->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected variable name after 'for' at line %d\n", for_token->line);
        token_free(next_tok);
        for_loop_free(loop);
        return NULL;
    }
    loop->var_name = strdup(next_tok->value);
    token_free(next_tok);
    
    // Expect: 'from' keyword (YZ_87: for i from 0 to 10)
    Token* from_tok = lexer_next_token(lexer);
    if (from_tok->type != TOKEN_FROM) {
        fprintf(stderr, "Error: Expected 'from' after variable name at line %d (syntax: for i from 0 to 10)\n", for_token->line);
        token_free(from_tok);
        for_loop_free(loop);
        return NULL;
    }
    token_free(from_tok);
    
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
