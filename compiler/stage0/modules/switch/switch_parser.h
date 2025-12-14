#ifndef SWITCH_PARSER_H
#define SWITCH_PARSER_H

#include "../lexer/lexer.h"
#include "../parser_core/parser_core.h"
#include "switch.h"

// Parse switch statement
SwitchStatement* switch_parse(Parser* parser, Token* switch_tok);

#endif // SWITCH_PARSER_H
