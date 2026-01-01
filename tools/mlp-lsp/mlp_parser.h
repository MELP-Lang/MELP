// MLP Parser for LSP
// Parses MLP source code and builds symbol table
#ifndef MLP_PARSER_H
#define MLP_PARSER_H

#include "symbol_table.h"

// Token types
typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_NEWLINE,
    TOKEN_INDENT,
    TOKEN_DEDENT,
    TOKEN_UNKNOWN
} TokenType;

// Token
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

// Parser context
typedef struct {
    const char* source;
    const char* file_uri;
    int position;
    int line;
    int column;
    
    Token current_token;
    SymbolTable* symbols;
} Parser;

// Parser operations
Parser* parser_create(const char* source, const char* file_uri);
void parser_free(Parser* parser);

// Main parsing function
bool parser_parse(Parser* parser);

// Get symbol table (ownership transferred to caller)
SymbolTable* parser_get_symbols(Parser* parser);

// Tokenization helpers
Token parser_next_token(Parser* parser);
void parser_free_token(Token* token);
bool parser_is_keyword(const char* word);

#endif // MLP_PARSER_H
