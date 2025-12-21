#ifndef THROW_PARSER_H
#define THROW_PARSER_H

#include "exception.h"
#include "../lexer/lexer.h"

// Throw statement parser
ThrowStatement* parse_throw_statement(Token** tokens, int* pos);

#endif // THROW_PARSER_H
