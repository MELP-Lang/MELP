#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Create new lexer
Lexer* lexer_create(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    free(lexer);
}

// Create token
Token* make_token(TokenType type, const char* value, int line, int column) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;
    return token;
}

void token_free(Token* token) {
    if (token) {
        if (token->value) free(token->value);
        free(token);
    }
}

// Skip whitespace
static void skip_whitespace(Lexer* lexer) {
    while (lexer->source[lexer->pos] != '\0') {
        char c = lexer->source[lexer->pos];
        if (c == ' ' || c == '\t' || c == '\r') {
            lexer->pos++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->pos++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

// Skip comment (-- to end of line)
static void skip_comment(Lexer* lexer) {
    if (lexer->source[lexer->pos] == '-' && lexer->source[lexer->pos + 1] == '-') {
        while (lexer->source[lexer->pos] != '\0' && lexer->source[lexer->pos] != '\n') {
            lexer->pos++;
        }
    }
}

// Check if keyword
static TokenType get_keyword_type(const char* word) {
    if (strcmp(word, "function") == 0) return TOKEN_FUNCTION;
    if (strcmp(word, "end_function") == 0) return TOKEN_END_FUNCTION;
    if (strcmp(word, "struct") == 0) return TOKEN_STRUCT;
    if (strcmp(word, "end_struct") == 0) return TOKEN_END_STRUCT;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "then") == 0) return TOKEN_THEN;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "end_if") == 0) return TOKEN_END_IF;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    if (strcmp(word, "end_while") == 0) return TOKEN_END_WHILE;
    if (strcmp(word, "return") == 0) return TOKEN_RETURN;
    if (strcmp(word, "import") == 0) return TOKEN_IMPORT;
    if (strcmp(word, "as") == 0) return TOKEN_AS;
    if (strcmp(word, "end") == 0) return TOKEN_END;
    
    // Type keywords
    if (strcmp(word, "Numeric") == 0) return TOKEN_NUMERIC;
    if (strcmp(word, "String") == 0) return TOKEN_STRING_TYPE;
    if (strcmp(word, "Boolean") == 0) return TOKEN_BOOLEAN;
    if (strcmp(word, "list") == 0) return TOKEN_LIST;
    
    return TOKEN_IDENTIFIER;
}

// Tokenize identifier or keyword
static Token* tokenize_identifier(Lexer* lexer) {
    int start = lexer->pos;
    int line = lexer->line;
    int col = lexer->column;
    
    while (isalnum(lexer->source[lexer->pos]) || lexer->source[lexer->pos] == '_') {
        lexer->pos++;
        lexer->column++;
    }
    
    int length = lexer->pos - start;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    
    TokenType type = get_keyword_type(value);
    Token* token = make_token(type, value, line, col);
    free(value);
    return token;
}

// Tokenize number
static Token* tokenize_number(Lexer* lexer) {
    int start = lexer->pos;
    int line = lexer->line;
    int col = lexer->column;
    
    while (isdigit(lexer->source[lexer->pos]) || lexer->source[lexer->pos] == '.') {
        lexer->pos++;
        lexer->column++;
    }
    
    int length = lexer->pos - start;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    
    Token* token = make_token(TOKEN_NUMBER, value, line, col);
    free(value);
    return token;
}

// Tokenize string
static Token* tokenize_string(Lexer* lexer) {
    int line = lexer->line;
    int col = lexer->column;
    
    lexer->pos++; // Skip opening "
    lexer->column++;
    
    int start = lexer->pos;
    
    while (lexer->source[lexer->pos] != '\0' && lexer->source[lexer->pos] != '"') {
        if (lexer->source[lexer->pos] == '\\') {
            lexer->pos++; // Skip escape char
            lexer->column++;
        }
        if (lexer->source[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
    }
    
    int length = lexer->pos - start;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    
    if (lexer->source[lexer->pos] == '"') {
        lexer->pos++; // Skip closing "
        lexer->column++;
    }
    
    Token* token = make_token(TOKEN_STRING, value, line, col);
    free(value);
    return token;
}

// Get next token
Token* lexer_next_token(Lexer* lexer) {
    while (1) {
        skip_whitespace(lexer);
        skip_comment(lexer);
        
        if (lexer->source[lexer->pos] != '-' || lexer->source[lexer->pos + 1] != '-') {
            break;
        }
    }
    
    if (lexer->source[lexer->pos] == '\0') {
        return make_token(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    char c = lexer->source[lexer->pos];
    int line = lexer->line;
    int col = lexer->column;
    
    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        return tokenize_identifier(lexer);
    }
    
    // Numbers
    if (isdigit(c)) {
        return tokenize_number(lexer);
    }
    
    // Strings
    if (c == '"') {
        return tokenize_string(lexer);
    }
    
    // Two-character operators
    if (c == '=' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2; lexer->column += 2;
        return make_token(TOKEN_EQ, "==", line, col);
    }
    if (c == '!' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2; lexer->column += 2;
        return make_token(TOKEN_NEQ, "!=", line, col);
    }
    if (c == '<' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2; lexer->column += 2;
        return make_token(TOKEN_LTE, "<=", line, col);
    }
    if (c == '>' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2; lexer->column += 2;
        return make_token(TOKEN_GTE, ">=", line, col);
    }
    
    // Single-character tokens
    lexer->pos++;
    lexer->column++;
    
    switch (c) {
        case '+': return make_token(TOKEN_PLUS, "+", line, col);
        case '-': return make_token(TOKEN_MINUS, "-", line, col);
        case '*': return make_token(TOKEN_STAR, "*", line, col);
        case '/': return make_token(TOKEN_SLASH, "/", line, col);
        case '%': return make_token(TOKEN_PERCENT, "%", line, col);
        case '=': return make_token(TOKEN_ASSIGN, "=", line, col);
        case '<': return make_token(TOKEN_LT, "<", line, col);
        case '>': return make_token(TOKEN_GT, ">", line, col);
        case '(': return make_token(TOKEN_LPAREN, "(", line, col);
        case ')': return make_token(TOKEN_RPAREN, ")", line, col);
        case '[': return make_token(TOKEN_LBRACKET, "[", line, col);
        case ']': return make_token(TOKEN_RBRACKET, "]", line, col);
        case ',': return make_token(TOKEN_COMMA, ",", line, col);
        case '.': return make_token(TOKEN_DOT, ".", line, col);
        case ':': return make_token(TOKEN_COLON, ":", line, col);
        default:
            return make_token(TOKEN_ERROR, NULL, line, col);
    }
}
