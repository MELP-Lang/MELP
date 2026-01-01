#ifndef STAGE1_LEXER_H
#define STAGE1_LEXER_H

// Token types for Stage1 MLP compiler
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER = 1,
    TOKEN_NUMBER = 2,
    TOKEN_STRING = 3,
    
    // Keywords
    TOKEN_FUNCTION = 10,
    TOKEN_END_FUNCTION = 11,
    TOKEN_STRUCT = 12,
    TOKEN_END_STRUCT = 13,
    TOKEN_IF = 14,
    TOKEN_THEN = 15,
    TOKEN_ELSE = 16,
    TOKEN_END_IF = 17,
    TOKEN_WHILE = 18,
    TOKEN_END_WHILE = 19,
    TOKEN_RETURN = 20,
    TOKEN_IMPORT = 21,
    TOKEN_AS = 22,
    TOKEN_END = 23,
    
    // Types
    TOKEN_NUMERIC = 30,
    TOKEN_STRING_TYPE = 31,
    TOKEN_BOOLEAN = 32,
    TOKEN_LIST = 33,
    
    // Operators
    TOKEN_PLUS = 40,
    TOKEN_MINUS = 41,
    TOKEN_STAR = 42,
    TOKEN_SLASH = 43,
    TOKEN_PERCENT = 44,
    TOKEN_ASSIGN = 45,
    TOKEN_EQ = 46,
    TOKEN_NEQ = 47,
    TOKEN_LT = 48,
    TOKEN_GT = 49,
    TOKEN_LTE = 50,
    TOKEN_GTE = 51,
    TOKEN_AND = 52,
    TOKEN_OR = 53,
    TOKEN_NOT = 54,
    
    // Delimiters
    TOKEN_LPAREN = 60,
    TOKEN_RPAREN = 61,
    TOKEN_LBRACKET = 62,
    TOKEN_RBRACKET = 63,
    TOKEN_COMMA = 64,
    TOKEN_DOT = 65,
    TOKEN_COLON = 66,
    
    TOKEN_ERROR = 99
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

typedef struct {
    const char* source;
    int pos;
    int line;
    int column;
} Lexer;

// Function prototypes
Lexer* lexer_create(const char* source);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
Token* make_token(TokenType type, const char* value, int line, int column);
void token_free(Token* token);

#endif // STAGE1_LEXER_H
