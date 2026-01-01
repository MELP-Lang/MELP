/* fmt_lexer.h - MLP Formatter Lexer */
#ifndef FMT_LEXER_H
#define FMT_LEXER_H

#include "fmt_types.h"

/* Lexer state */
typedef struct {
    const char* source;
    int position;
    int line;
    int column;
} Lexer;

/* Create lexer */
Lexer* lexer_create(const char* source);

/* Free lexer */
void lexer_free(Lexer* lexer);

/* Tokenize source */
Token** lexer_tokenize(Lexer* lexer, int* token_count);

/* Token operations */
Token* token_create(TokenType type, const char* value, int line, int column);
void token_free(Token* token);
const char* token_type_to_string(TokenType type);

#endif /* FMT_LEXER_H */
