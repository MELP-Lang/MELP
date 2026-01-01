/* fmt_types.h - MLP Formatter Type Definitions */
#ifndef FMT_TYPES_H
#define FMT_TYPES_H

#include <stdbool.h>

/* Token types */
typedef enum {
    TOKEN_KEYWORD,      // fn, let, if, etc.
    TOKEN_IDENTIFIER,   // variable/function names
    TOKEN_NUMBER,       // 42, 3.14
    TOKEN_STRING,       // "hello"
    TOKEN_OPERATOR,     // +, -, *, /, =, ==, etc.
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_COMMA,        // ,
    TOKEN_COLON,        // :
    TOKEN_ARROW,        // ->
    TOKEN_NEWLINE,      // \n
    TOKEN_INDENT,       // Indentation
    TOKEN_COMMENT,      // # comment
    TOKEN_EOF,          // End of file
    TOKEN_UNKNOWN
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

/* Formatting options */
typedef struct {
    int indent_size;        // Default: 4
    int max_line_width;     // Default: 80
    bool spaces_around_operators;  // Default: true
    bool newline_before_colon;     // Default: false
    bool check_only;        // Default: false (format in-place)
} FormatOptions;

/* Formatter state */
typedef struct {
    Token** tokens;
    int token_count;
    int current_token;
    int indent_level;
    char* output;
    int output_size;
    int output_capacity;
    FormatOptions options;
} Formatter;

#endif /* FMT_TYPES_H */
