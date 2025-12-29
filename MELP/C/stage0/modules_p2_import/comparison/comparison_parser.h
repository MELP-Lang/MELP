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

#endif // COMPARISON_PARSER_H
