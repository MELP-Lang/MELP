#ifndef FOR_LOOP_PARSER_H
#define FOR_LOOP_PARSER_H

#include "for_loop.h"
#include "../lexer/lexer.h"

// Stateless parser function
// Borrows for_token, caller must free it after call
ForLoop* for_loop_parse(Lexer* lexer, Token* for_token);

#endif // FOR_LOOP_PARSER_H
