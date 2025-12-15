#ifndef ENUM_PARSER_H
#define ENUM_PARSER_H

#include "enum.h"
#include "../lexer/lexer.h"

// Parse enum definition
// Syntax:
//   enum Status
//       PENDING = 0
//       ACTIVE = 1
//       DONE = 2
//   end_enum
EnumDefinition* enum_parse(Lexer* lexer, Token* enum_tok);

// Parse enum value reference (e.g., Status.ACTIVE)
// Returns the numeric value
int64_t enum_parse_value_reference(const char* reference);

#endif // ENUM_PARSER_H
