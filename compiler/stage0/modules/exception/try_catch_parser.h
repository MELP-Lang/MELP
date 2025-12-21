#ifndef TRY_CATCH_PARSER_H
#define TRY_CATCH_PARSER_H

#include "exception.h"
#include "../lexer/lexer.h"

// Try-catch-finally parser
TryBlock* parse_try_block(Token** tokens, int* pos);

// Helper functions
CatchClause* parse_catch_clause(Token** tokens, int* pos);
FinallyBlock* parse_finally_block(Token** tokens, int* pos);

#endif // TRY_CATCH_PARSER_H
