#ifndef COMPARISON_PARSER_H
#define COMPARISON_PARSER_H

#include "comparison.h"
#include "../lexer/lexer.h"

// ============================================================================
// STATELESS TEMPLATE FUNCTION
// ============================================================================
// Parse comparison expression (borrowing pattern)
// Input: first_token - caller owns it, won't be freed
// Returns: ComparisonExpr* or NULL
// Note: Reads additional tokens from lexer, frees only what it reads
ComparisonExpr* comparison_parse_expression_stateless(Lexer* lexer, Token* first_token);

// ============================================================================
// DEPRECATED - Old stateful parser API
// ============================================================================
typedef struct {
    Lexer* lexer;
    Token* current_token;
} ComparisonParser;

ComparisonParser* comparison_parser_create(Lexer* lexer);
void comparison_parser_free(ComparisonParser* parser);
ComparisonExpr* comparison_parse_expression(ComparisonParser* parser);

#endif // COMPARISON_PARSER_H
