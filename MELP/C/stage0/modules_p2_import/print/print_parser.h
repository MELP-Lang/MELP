#ifndef PRINT_PARSER_H
#define PRINT_PARSER_H

#include "print.h"
#include "../lexer/lexer.h"

// YZ_61: Parse print statement: print(expression) - stateless pattern
// print_token: The already-read TOKEN_PRINT token (passed for verification)
PrintStatement* parse_print_statement(Lexer* lexer, Token* print_token);

#endif
