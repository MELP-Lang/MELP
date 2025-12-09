#include "lexer.h"
#include "../comments/comments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* lexer_create(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->pushback_token = NULL;  // ✅ Initialize pushback
    return lexer;
}

void lexer_free(Lexer* lexer) {
    if (lexer->pushback_token) {
        token_free(lexer->pushback_token);  // ✅ Free pushback if any
    }
    free(lexer);
}

void token_free(Token* token) {
    if (token->value) free(token->value);
    free(token);
}

// ✅ Push token back - next lexer_next_token() will return this
void lexer_unget_token(Lexer* lexer, Token* token) {
    if (lexer->pushback_token) {
        // Already have pushback! This shouldn't happen
        fprintf(stderr, "Warning: lexer_unget_token overwriting existing pushback!\n");
        token_free(lexer->pushback_token);
    }
    lexer->pushback_token = token;
}

static void skip_whitespace(Lexer* lexer) {
    while (lexer->source[lexer->pos] == ' ' || 
           lexer->source[lexer->pos] == '\t' ||
           lexer->source[lexer->pos] == '\r') {
        lexer->pos++;
    }
    if (lexer->source[lexer->pos] == '\n') {
        lexer->line++;
        lexer->pos++;
        skip_whitespace(lexer);
    }
}

// Removed static skip_comment - now using comments module

static Token* make_token(TokenType type, const char* value, int line) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    return token;
}

static Token* read_number(Lexer* lexer) {
    int start = lexer->pos;
    while (isdigit(lexer->source[lexer->pos])) {
        lexer->pos++;
    }
    
    // Check for decimal point
    if (lexer->source[lexer->pos] == '.') {
        lexer->pos++;  // consume '.'
        while (isdigit(lexer->source[lexer->pos])) {
            lexer->pos++;
        }
    }
    
    int len = lexer->pos - start;
    char* value = malloc(len + 1);
    strncpy(value, &lexer->source[start], len);
    value[len] = '\0';
    Token* token = make_token(TOKEN_NUMBER, value, lexer->line);
    free(value);
    return token;
}

static Token* read_string(Lexer* lexer) {
    lexer->pos++; // skip opening "
    int start = lexer->pos;
    while (lexer->source[lexer->pos] != '"' && lexer->source[lexer->pos] != '\0') {
        lexer->pos++;
    }
    int len = lexer->pos - start;
    char* value = malloc(len + 1);
    strncpy(value, &lexer->source[start], len);
    value[len] = '\0';
    lexer->pos++; // skip closing "
    Token* token = make_token(TOKEN_STRING, value, lexer->line);
    free(value);
    return token;
}

static Token* read_identifier(Lexer* lexer) {
    int start = lexer->pos;
    // Only accept ASCII alphanumeric and underscore
    // Stop at any non-ASCII character (byte > 127) to avoid UTF-8 infinite loops
    while (lexer->source[lexer->pos] != '\0' &&
           (unsigned char)lexer->source[lexer->pos] < 128 &&
           (isalnum(lexer->source[lexer->pos]) || lexer->source[lexer->pos] == '_')) {
        lexer->pos++;
    }
    int len = lexer->pos - start;
    char* value = malloc(len + 1);
    strncpy(value, &lexer->source[start], len);
    value[len] = '\0';
    
    // Check keywords
    TokenType type = TOKEN_IDENTIFIER;
    if (strcmp(value, "numeric") == 0) type = TOKEN_NUMERIC;
    else if (strcmp(value, "string") == 0) type = TOKEN_STRING_TYPE;  // STRING type keyword (legacy)
    else if (strcmp(value, "text") == 0) type = TOKEN_STRING_TYPE;    // TEXT type keyword (MLP standard)
    else if (strcmp(value, "boolean") == 0) type = TOKEN_BOOLEAN;
    else if (strcmp(value, "true") == 0) type = TOKEN_TRUE;
    else if (strcmp(value, "false") == 0) type = TOKEN_FALSE;
    // Control flow keywords
    else if (strcmp(value, "if") == 0) type = TOKEN_IF;
    else if (strcmp(value, "then") == 0) type = TOKEN_THEN;
    else if (strcmp(value, "else") == 0) type = TOKEN_ELSE;
    else if (strcmp(value, "end") == 0) type = TOKEN_END;
    else if (strcmp(value, "while") == 0) type = TOKEN_WHILE;
    else if (strcmp(value, "do") == 0) type = TOKEN_DO;
    else if (strcmp(value, "for") == 0) type = TOKEN_FOR;
    else if (strcmp(value, "to") == 0) type = TOKEN_TO;
    else if (strcmp(value, "downto") == 0) type = TOKEN_DOWNTO;
    else if (strcmp(value, "exit") == 0) type = TOKEN_EXIT;
    // Function keywords
    else if (strcmp(value, "function") == 0) type = TOKEN_FUNCTION;
    else if (strcmp(value, "return") == 0) type = TOKEN_RETURN;
    else if (strcmp(value, "returns") == 0) type = TOKEN_RETURNS;
    // Struct keywords
    else if (strcmp(value, "struct") == 0) type = TOKEN_STRUCT;
    // Logical keywords (also bitwise for integers - VB.NET style)
    else if (strcasecmp(value, "and") == 0) type = TOKEN_AND;
    else if (strcasecmp(value, "or") == 0) type = TOKEN_OR;
    else if (strcasecmp(value, "xor") == 0) type = TOKEN_XOR;
    else if (strcasecmp(value, "not") == 0) type = TOKEN_NOT;
    // Arithmetic keywords
    else if (strcasecmp(value, "mod") == 0) type = TOKEN_MOD;
    // I/O keywords
    else if (strcmp(value, "print") == 0) type = TOKEN_PRINT;
    
    Token* token = make_token(type, value, lexer->line);
    free(value);
    return token;
}

Token* lexer_next_token(Lexer* lexer) {
    // ✅ Check for pushback token first
    if (lexer->pushback_token) {
        Token* tok = lexer->pushback_token;
        lexer->pushback_token = NULL;  // Clear pushback
        return tok;
    }
    
    while (1) {
        skip_whitespace(lexer);
        
        // Check for comment using comments module
        if (is_comment_start(lexer->source, lexer->pos)) {
            lexer->pos = skip_comment(lexer->source, lexer->pos, &lexer->line);
            continue;  // Re-check for more whitespace/comments
        }
        break;
    }
    
    if (lexer->source[lexer->pos] == '\0') {
        return make_token(TOKEN_EOF, NULL, lexer->line);
    }
    
    char c = lexer->source[lexer->pos];
    
    // Two-character operators (must check before single-char)
    if (c == '=' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token(TOKEN_EQUAL, "==", lexer->line);
    }
    
    if (c == '!' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token(TOKEN_NOT_EQUAL, "!=", lexer->line);
    }
    
    if (c == '<' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token(TOKEN_LESS_EQUAL, "<=", lexer->line);
    }
    
    if (c == '>' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token(TOKEN_GREATER_EQUAL, ">=", lexer->line);
    }
    
    // Logical operators (two-character)
    if (c == '&' && lexer->source[lexer->pos + 1] == '&') {
        lexer->pos += 2;
        return make_token(TOKEN_AND, "&&", lexer->line);
    }
    
    if (c == '|' && lexer->source[lexer->pos + 1] == '|') {
        lexer->pos += 2;
        return make_token(TOKEN_OR, "||", lexer->line);
    }
    
    // Power operator (two-character **)
    if (c == '*' && lexer->source[lexer->pos + 1] == '*') {
        lexer->pos += 2;
        return make_token(TOKEN_POWER, "**", lexer->line);
    }
    
    // Single-character operators
    if (c == '=') {
        lexer->pos++;
        return make_token(TOKEN_ASSIGN, "=", lexer->line);
    }
    
    if (c == '<') {
        lexer->pos++;
        return make_token(TOKEN_LESS, "<", lexer->line);
    }
    
    if (c == '>') {
        lexer->pos++;
        return make_token(TOKEN_GREATER, ">", lexer->line);
    }
    
    // Arithmetic operators
    if (c == '+') {
        lexer->pos++;
        return make_token(TOKEN_PLUS, "+", lexer->line);
    }
    
    if (c == '-') {
        lexer->pos++;
        return make_token(TOKEN_MINUS, "-", lexer->line);
    }
    
    if (c == '*') {
        lexer->pos++;
        return make_token(TOKEN_MULTIPLY, "*", lexer->line);
    }
    
    if (c == '/') {
        lexer->pos++;
        return make_token(TOKEN_DIVIDE, "/", lexer->line);
    }
    
    if (c == '%') {
        lexer->pos++;
        return make_token(TOKEN_MOD, "%", lexer->line);
    }
    
    // Bitwise operators (single-character, checked after && and ||)
    if (c == '&') {
        lexer->pos++;
        return make_token(TOKEN_AND, "&", lexer->line);
    }
    
    if (c == '|') {
        lexer->pos++;
        return make_token(TOKEN_OR, "|", lexer->line);
    }
    
    if (c == '^') {
        lexer->pos++;
        return make_token(TOKEN_XOR, "^", lexer->line);
    }
    
    // Logical NOT (single-character, checked after !=)
    if (c == '!') {
        lexer->pos++;
        return make_token(TOKEN_NOT, "!", lexer->line);
    }
    
    // Delimiters
    if (c == '(') {
        lexer->pos++;
        return make_token(TOKEN_LPAREN, "(", lexer->line);
    }
    
    if (c == ')') {
        lexer->pos++;
        return make_token(TOKEN_RPAREN, ")", lexer->line);
    }
    
    if (c == ',') {
        lexer->pos++;
        return make_token(TOKEN_COMMA, ",", lexer->line);
    }
    
    if (c == ':') {
        lexer->pos++;
        return make_token(TOKEN_COLON, ":", lexer->line);
    }
    
    if (c == ';') {
        lexer->pos++;
        return make_token(TOKEN_SEMICOLON, ";", lexer->line);
    }
    
    if (c == '[') {
        lexer->pos++;
        return make_token(TOKEN_LBRACKET, "[", lexer->line);
    }
    
    if (c == ']') {
        lexer->pos++;
        return make_token(TOKEN_RBRACKET, "]", lexer->line);
    }
    
    if (c == '.') {
        lexer->pos++;
        return make_token(TOKEN_DOT, ".", lexer->line);
    }
    
    // Line continuation character
    if (c == '_' || c == '\\') {
        lexer->pos++;
        // Check if next character is newline
        if (lexer->source[lexer->pos] == '\n') {
            lexer->pos++;
            lexer->line++;
            // Skip continuation, return next token
            return lexer_next_token(lexer);
        }
        // If not followed by newline, treat _ as identifier start
        if (c == '_') {
            lexer->pos--;  // Back up
            return read_identifier(lexer);
        }
        return make_token(TOKEN_CONTINUATION, c == '_' ? "_" : "\\", lexer->line);
    }
    
    if (isdigit(c)) {
        return read_number(lexer);
    }
    
    if (c == '"') {
        return read_string(lexer);
    }
    
    if (isalpha(c) || c == '_') {
        return read_identifier(lexer);
    }
    
    // Check for UTF-8 multi-byte characters (non-ASCII)
    if ((unsigned char)c > 127) {
        // UTF-8 character detected - skip it and return error
        // Skip the full UTF-8 sequence (1-4 bytes)
        lexer->pos++;
        if (((unsigned char)c & 0xE0) == 0xC0) {
            // 2-byte UTF-8 (110xxxxx)
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
        } else if (((unsigned char)c & 0xF0) == 0xE0) {
            // 3-byte UTF-8 (1110xxxx)
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
        } else if (((unsigned char)c & 0xF8) == 0xF0) {
            // 4-byte UTF-8 (11110xxx)
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
            if (lexer->source[lexer->pos] != '\0') lexer->pos++;
        }
        // Return error token with helpful message
        char error_msg[100];
        snprintf(error_msg, sizeof(error_msg), "Unexpected non-ASCII character (byte: 0x%02X) at line %d", (unsigned char)c, lexer->line);
        return make_token(TOKEN_ERROR, error_msg, lexer->line);
    }
    
    return make_token(TOKEN_ERROR, NULL, lexer->line);
}
