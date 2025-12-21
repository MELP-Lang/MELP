#ifndef EXCEPTION_PARSER_H
#define EXCEPTION_PARSER_H

#include "exception.h"
#include "../lexer/lexer.h"

// Exception definition parser (for custom exceptions)
// Syntax: exception CustomException extends Exception ... end_exception
int parse_exception_definition(Token** tokens, int* pos);

#endif // EXCEPTION_PARSER_H
