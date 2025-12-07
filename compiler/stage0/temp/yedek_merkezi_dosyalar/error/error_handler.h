#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "../lexer.h"

// Error reporting functions
void error_unexpected_token(Token* token, const char* expected);
void error_unexpected_operator(Token* token);
void error_unexpected_literal(Token* token, const char* type);
void error_invalid_statement(Token* token);

#endif
