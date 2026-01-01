/* fmt_lexer.c - MLP Formatter Lexer Implementation */
#include "fmt_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* Safe strdup implementation */
static char* safe_strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* copy = malloc(len + 1);
    if (copy) {
        memcpy(copy, str, len + 1);
    }
    return copy;
}

/* MLP keywords */
static const char* KEYWORDS[] = {
    "fn", "let", "if", "else", "elif", "while", "for", "in",
    "return", "break", "continue", "pass", "import", "from",
    "type", "struct", "enum", "True", "False", "None",
    "and", "or", "not", "is", "as", NULL
};

/* Check if word is keyword */
static bool is_keyword(const char* word) {
    for (int i = 0; KEYWORDS[i] != NULL; i++) {
        if (strcmp(word, KEYWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Create lexer */
Lexer* lexer_create(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    if (!lexer) return NULL;
    
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 0;
    lexer->column = 0;
    
    return lexer;
}

/* Free lexer */
void lexer_free(Lexer* lexer) {
    if (lexer) {
        free(lexer);
    }
}

/* Create token */
Token* token_create(TokenType type, const char* value, int line, int column) {
    Token* token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = type;
    token->value = value ? safe_strdup(value) : NULL;
    token->line = line;
    token->column = column;
    
    return token;
}

/* Free token */
void token_free(Token* token) {
    if (!token) return;
    free(token->value);
    free(token);
}

/* Current char */
static char current_char(Lexer* lexer) {
    if (lexer->source[lexer->position] == '\0') {
        return '\0';
    }
    return lexer->source[lexer->position];
}

/* Advance position */
static void advance(Lexer* lexer) {
    if (current_char(lexer) == '\n') {
        lexer->line++;
        lexer->column = 0;
    } else {
        lexer->column++;
    }
    lexer->position++;
}

/* Skip whitespace (except newlines) */
static void skip_whitespace(Lexer* lexer) {
    while (current_char(lexer) == ' ' || current_char(lexer) == '\t') {
        advance(lexer);
    }
}

/* Read identifier or keyword */
static Token* read_identifier(Lexer* lexer) {
    int start_line = lexer->line;
    int start_col = lexer->column;
    int start = lexer->position;
    
    while (isalnum(current_char(lexer)) || current_char(lexer) == '_') {
        advance(lexer);
    }
    
    int len = lexer->position - start;
    char* value = malloc(len + 1);
    strncpy(value, lexer->source + start, len);
    value[len] = '\0';
    
    TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    Token* token = token_create(type, value, start_line, start_col);
    free(value);
    
    return token;
}

/* Read number */
static Token* read_number(Lexer* lexer) {
    int start_line = lexer->line;
    int start_col = lexer->column;
    int start = lexer->position;
    
    while (isdigit(current_char(lexer)) || current_char(lexer) == '.') {
        advance(lexer);
    }
    
    int len = lexer->position - start;
    char* value = malloc(len + 1);
    strncpy(value, lexer->source + start, len);
    value[len] = '\0';
    
    Token* token = token_create(TOKEN_NUMBER, value, start_line, start_col);
    free(value);
    
    return token;
}

/* Read string */
static Token* read_string(Lexer* lexer) {
    int start_line = lexer->line;
    int start_col = lexer->column;
    char quote = current_char(lexer);
    advance(lexer); // Skip opening quote
    
    int start = lexer->position;
    while (current_char(lexer) != quote && current_char(lexer) != '\0') {
        if (current_char(lexer) == '\\') {
            advance(lexer); // Skip escape char
        }
        advance(lexer);
    }
    
    int len = lexer->position - start;
    char* value = malloc(len + 3); // +3 for quotes and null
    value[0] = quote;
    strncpy(value + 1, lexer->source + start, len);
    value[len + 1] = quote;
    value[len + 2] = '\0';
    
    if (current_char(lexer) == quote) {
        advance(lexer); // Skip closing quote
    }
    
    Token* token = token_create(TOKEN_STRING, value, start_line, start_col);
    free(value);
    
    return token;
}

/* Read comment */
static Token* read_comment(Lexer* lexer) {
    int start_line = lexer->line;
    int start_col = lexer->column;
    int start = lexer->position;
    
    while (current_char(lexer) != '\n' && current_char(lexer) != '\0') {
        advance(lexer);
    }
    
    int len = lexer->position - start;
    char* value = malloc(len + 1);
    strncpy(value, lexer->source + start, len);
    value[len] = '\0';
    
    Token* token = token_create(TOKEN_COMMENT, value, start_line, start_col);
    free(value);
    
    return token;
}

/* Tokenize source */
Token** lexer_tokenize(Lexer* lexer, int* token_count) {
    int capacity = 100;
    Token** tokens = malloc(sizeof(Token*) * capacity);
    int count = 0;
    
    while (current_char(lexer) != '\0') {
        skip_whitespace(lexer);
        
        char c = current_char(lexer);
        if (c == '\0') break;
        
        Token* token = NULL;
        int line = lexer->line;
        int col = lexer->column;
        
        // Newline
        if (c == '\n') {
            token = token_create(TOKEN_NEWLINE, "\\n", line, col);
            advance(lexer);
        }
        // Comment
        else if (c == '#') {
            token = read_comment(lexer);
        }
        // String
        else if (c == '"' || c == '\'') {
            token = read_string(lexer);
        }
        // Number
        else if (isdigit(c)) {
            token = read_number(lexer);
        }
        // Identifier or keyword
        else if (isalpha(c) || c == '_') {
            token = read_identifier(lexer);
        }
        // Operators and punctuation
        else if (c == '-' && lexer->source[lexer->position + 1] == '>') {
            token = token_create(TOKEN_ARROW, "->", line, col);
            advance(lexer);
            advance(lexer);
        }
        else if (c == '=' && lexer->source[lexer->position + 1] == '=') {
            token = token_create(TOKEN_OPERATOR, "==", line, col);
            advance(lexer);
            advance(lexer);
        }
        else if (c == '!' && lexer->source[lexer->position + 1] == '=') {
            token = token_create(TOKEN_OPERATOR, "!=", line, col);
            advance(lexer);
            advance(lexer);
        }
        else if (strchr("+-*/%=<>!", c)) {
            char op[2] = {c, '\0'};
            token = token_create(TOKEN_OPERATOR, op, line, col);
            advance(lexer);
        }
        else if (c == '(') {
            token = token_create(TOKEN_LPAREN, "(", line, col);
            advance(lexer);
        }
        else if (c == ')') {
            token = token_create(TOKEN_RPAREN, ")", line, col);
            advance(lexer);
        }
        else if (c == '[') {
            token = token_create(TOKEN_LBRACKET, "[", line, col);
            advance(lexer);
        }
        else if (c == ']') {
            token = token_create(TOKEN_RBRACKET, "]", line, col);
            advance(lexer);
        }
        else if (c == '{') {
            token = token_create(TOKEN_LBRACE, "{", line, col);
            advance(lexer);
        }
        else if (c == '}') {
            token = token_create(TOKEN_RBRACE, "}", line, col);
            advance(lexer);
        }
        else if (c == ',') {
            token = token_create(TOKEN_COMMA, ",", line, col);
            advance(lexer);
        }
        else if (c == ':') {
            token = token_create(TOKEN_COLON, ":", line, col);
            advance(lexer);
        }
        else {
            // Unknown token, skip
            advance(lexer);
            continue;
        }
        
        // Add token to list
        if (token) {
            if (count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, sizeof(Token*) * capacity);
            }
            tokens[count++] = token;
        }
    }
    
    // Add EOF token
    Token* eof = token_create(TOKEN_EOF, NULL, lexer->line, lexer->column);
    if (count >= capacity) {
        capacity++;
        tokens = realloc(tokens, sizeof(Token*) * capacity);
    }
    tokens[count++] = eof;
    
    *token_count = count;
    return tokens;
}

/* Token type to string */
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_COLON: return "COLON";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
