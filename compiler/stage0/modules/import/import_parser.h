#ifndef IMPORT_PARSER_H
#define IMPORT_PARSER_H

#include "../lexer/lexer.h"
#include "import.h"

// Parse import statement
// Syntax: import module_name
// Returns: ImportStatement* or NULL on error
ImportStatement* import_parse(Lexer* lexer, Token* import_token);

#endif
