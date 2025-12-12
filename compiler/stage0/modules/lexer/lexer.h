#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMERIC,
    TOKEN_STRING_TYPE,     // string keyword (type declaration)
    TOKEN_BOOLEAN,
    TOKEN_LIST,            // list keyword (type declaration)
    TOKEN_TUPLE,           // tuple keyword (type declaration)
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_NUMBER,
    TOKEN_STRING,          // "string literal"
    TOKEN_TRUE,
    TOKEN_FALSE,
    // Arithmetic operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MOD,             // %
    TOKEN_POWER,           // **
    TOKEN_XOR,             // xor (also ^) - bitwise XOR
    // Comparison operators
    TOKEN_EQUAL,           // ==
    TOKEN_NOT_EQUAL,       // !=
    TOKEN_LESS,            // <
    TOKEN_GREATER,         // >
    TOKEN_LESS_EQUAL,      // <=
    TOKEN_GREATER_EQUAL,   // >=
    // Logical operators
    TOKEN_AND,             // &&
    TOKEN_OR,              // ||
    TOKEN_NOT,             // !
    // Control flow keywords
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_END,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_FOR,
    TOKEN_TO,
    TOKEN_DOWNTO,
    TOKEN_EACH,            // for each item in collection
    TOKEN_IN,              // for each item in collection
    TOKEN_EXIT,            // exit, exit for, exit while, exit if, exit function
    // Function keywords
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_RETURNS,         // returns (for return type declaration)
    // Struct keywords
    TOKEN_STRUCT,
    // Module keywords
    TOKEN_IMPORT,          // import module_name
    TOKEN_MODULE,          // module keyword
    // I/O keywords
    TOKEN_PRINT,
    TOKEN_PRINTLN,         // println builtin (YZ_48)
    // Delimiters
    TOKEN_LPAREN,          // (
    TOKEN_RPAREN,          // )
    TOKEN_LBRACKET,        // [
    TOKEN_RBRACKET,        // ]
    TOKEN_LANGLE,          // < (tuple literal, not comparison)
    TOKEN_RANGLE,          // > (tuple literal, not comparison)
    TOKEN_COMMA,           // ,
    TOKEN_COLON,           // :
    TOKEN_SEMICOLON,       // ;
    TOKEN_DOT,             // .
    TOKEN_CONTINUATION,    // _ or \ (line continuation)
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int has_leading_whitespace;  // YZ_24: 1 if whitespace precedes token, 0 otherwise
} Token;

typedef struct {
    const char* source;
    int pos;
    int line;
    Token* pushback_token;  // ✅ For parser coordination: last read token can be "ungot"
} Lexer;

Lexer* lexer_create(const char* source);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
void lexer_unget_token(Lexer* lexer, Token* token);  // ✅ Push token back
void token_free(Token* token);

#endif
