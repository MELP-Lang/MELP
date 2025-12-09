#ifndef VARIABLE_PARSER_H
#define VARIABLE_PARSER_H

#include "../lexer/lexer.h"
#include "variable.h"

// STATELESS PATTERN: Parse functions take Lexer and Token directly
// No parser struct needed (follows ARCHITECTURE.md pattern)

// Parse variable declaration: numeric x = 10
// type_token is BORROWED - don't free inside function
VariableDeclaration* variable_parse_declaration(Lexer* lexer, Token* type_token);

// Parse variable assignment: x = 20
// identifier_token is BORROWED - don't free inside function
VariableAssignment* variable_parse_assignment(Lexer* lexer, Token* identifier_token);

// Free structures
void variable_declaration_free(VariableDeclaration* decl);
void variable_assignment_free(VariableAssignment* assign);

#endif
