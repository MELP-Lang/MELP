#ifndef ARITHMETIC_PARSER_H
#define ARITHMETIC_PARSER_H

#include "arithmetic.h"
#include "../lexer/lexer.h"

// ✅ STATELESS API (Recommended)
// Parse arithmetic expression - stateless template
// first_token is borrowed from caller, function may read additional tokens
// Caller must manage any lookahead token after parsing
ArithmeticExpr* arithmetic_parse_expression_stateless(Lexer* lexer, Token* first_token);

// Parse assignment stateless: varname = expression
// first_token should be TOKEN_IDENTIFIER
// Returns: variable name (caller must free), fills expr pointer
char* arithmetic_parse_assignment_stateless(Lexer* lexer, Token* first_token, ArithmeticExpr** expr);

// ❌ DEPRECATED STATEFUL API (For backward compatibility only - will be removed)
// Use stateless API above instead

// Arithmetic parser structure (DEPRECATED)
typedef struct {
    Lexer* lexer;
    Token* current_token;
} ArithmeticParser;

// Create/free parser (DEPRECATED)
ArithmeticParser* arithmetic_parser_create(Lexer* lexer);
void arithmetic_parser_free(ArithmeticParser* parser);

// Parse arithmetic expression (DEPRECATED)
ArithmeticExpr* arithmetic_parse_expression(ArithmeticParser* parser);

// Parse binary operation (DEPRECATED)
ArithmeticExpr* arithmetic_parse_term(ArithmeticParser* parser);        // * / %
ArithmeticExpr* arithmetic_parse_factor(ArithmeticParser* parser);      // + -
ArithmeticExpr* arithmetic_parse_primary(ArithmeticParser* parser);     // numbers, variables

// Parse assignment (DEPRECATED)
char* arithmetic_parse_assignment(ArithmeticParser* parser, ArithmeticExpr** expr);

#endif
