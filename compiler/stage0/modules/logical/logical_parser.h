#ifndef LOGICAL_PARSER_H
#define LOGICAL_PARSER_H

#include "logical.h"
#include "../lexer/lexer.h"

// STATELESS PATTERN: Parse functions take Lexer and Token directly
// No parser struct needed (follows ARCHITECTURE.md pattern)

// Parse logical expression (entry point)
// first_token is BORROWED - don't free inside function
LogicalExpr* logical_parse_expression_stateless(Lexer* lexer, Token* first_token);

// Internal parsing functions (not typically called directly)
LogicalExpr* logical_parse_or_stateless(Lexer* lexer, Token** current);
LogicalExpr* logical_parse_and_stateless(Lexer* lexer, Token** current);
LogicalExpr* logical_parse_not_stateless(Lexer* lexer, Token** current);
LogicalExpr* logical_parse_primary_stateless(Lexer* lexer, Token** current);

#endif // LOGICAL_PARSER_H
