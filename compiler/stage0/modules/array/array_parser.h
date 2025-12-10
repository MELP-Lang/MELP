#ifndef ARRAY_PARSER_H
#define ARRAY_PARSER_H

#include "array.h"
#include "../lexer/lexer.h"

// ✅ STATELESS PARSER PATTERN - No struct, functions only!
// Token Ownership: All Token* parameters are BORROWED (caller owns)
// Memory: Caller must free returned Collection* or IndexAccess*

// Parse collection literals
// Array: [1, 2, 3, 4] - Homogeneous, comma-separated
// Token: lbracket_token must be '[' (BORROWED)
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token);

// List: (10; 20; 30;) - Heterogeneous, semicolon separated, trailing semicolon required
// Token: lparen_token must be '(' (BORROWED)
Collection* array_parse_list_literal(Lexer* lexer, Token* lparen_token);

// Tuple: <"name", 25, true> - Heterogeneous, immutable, comma-separated
// Token: less_token must be '<' (BORROWED)
Collection* array_parse_tuple_literal(Lexer* lexer, Token* less_token);

// Parse index access and return IndexAccess struct
// base_name: identifier name
// index_token: '[' for array, '(' for list, or '<' for tuple (BORROWED)
// Returns NULL if parse fails
IndexAccess* array_parse_index_access(Lexer* lexer, const char* base_name, Token* index_token);

// Free collection
void collection_free(Collection* coll);

#endif
