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
    token->has_leading_whitespace = 0;  // YZ_24: Default to no whitespace
    return token;
}

// YZ_24: Helper to create token with whitespace flag
static Token* make_token_ws(TokenType type, const char* value, int line, int had_whitespace) {
    Token* token = make_token(type, value, line);
    token->has_leading_whitespace = had_whitespace;
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
    else if (strcmp(value, "string") == 0) type = TOKEN_STRING_TYPE;  // STRING type keyword (MLP standard)
    else if (strcmp(value, "text") == 0) type = TOKEN_STRING_TYPE;    // TEXT type keyword (legacy alias, deprecated)
    else if (strcmp(value, "boolean") == 0) type = TOKEN_BOOLEAN;
    else if (strcmp(value, "Array") == 0) type = TOKEN_ARRAY;         // YZ_74: Array type keyword
    else if (strcmp(value, "list") == 0) type = TOKEN_LIST;
    else if (strcmp(value, "tuple") == 0) type = TOKEN_TUPLE;
    else if (strcmp(value, "true") == 0) type = TOKEN_TRUE;
    else if (strcmp(value, "false") == 0) type = TOKEN_FALSE;
    
    // ⭐ PMPL Block Terminators (underscore keywords - RF_YZ_2)
    else if (strcmp(value, "end_if") == 0) type = TOKEN_END_IF;
    else if (strcmp(value, "end_while") == 0) type = TOKEN_END_WHILE;
    else if (strcmp(value, "end_for") == 0) type = TOKEN_END_FOR;
    else if (strcmp(value, "end_function") == 0) type = TOKEN_END_FUNCTION;
    else if (strcmp(value, "end_struct") == 0) type = TOKEN_END_STRUCT;
    else if (strcmp(value, "end_enum") == 0) type = TOKEN_END_ENUM;
    else if (strcmp(value, "end_switch") == 0) type = TOKEN_END_SWITCH;
    else if (strcmp(value, "end_match") == 0) type = TOKEN_END_MATCH;
    else if (strcmp(value, "end_operator") == 0) type = TOKEN_END_OPERATOR;
    else if (strcmp(value, "end_try") == 0) type = TOKEN_END_TRY;
    
    // ⭐ PMPL Control Flow (RF_YZ_2)
    else if (strcmp(value, "else_if") == 0) type = TOKEN_ELSE_IF;
    
    // ⭐ PMPL Loop Control (RF_YZ_2)
    else if (strcmp(value, "continue") == 0) type = TOKEN_CONTINUE;
    else if (strcmp(value, "exit_if") == 0) type = TOKEN_EXIT_IF;
    else if (strcmp(value, "exit_for") == 0) type = TOKEN_EXIT_FOR;
    else if (strcmp(value, "exit_while") == 0) type = TOKEN_EXIT_WHILE;
    else if (strcmp(value, "exit_function") == 0) type = TOKEN_EXIT_FUNCTION;
    else if (strcmp(value, "exit_switch") == 0) type = TOKEN_EXIT_SWITCH;
    else if (strcmp(value, "continue_for") == 0) type = TOKEN_CONTINUE_FOR;
    else if (strcmp(value, "continue_while") == 0) type = TOKEN_CONTINUE_WHILE;
    
    // ⭐ PMPL Debug Block (RF_YZ_2)
    else if (strcmp(value, "debug") == 0) type = TOKEN_DEBUG;
    else if (strcmp(value, "end_debug") == 0) type = TOKEN_END_DEBUG;
    else if (strcmp(value, "goto") == 0) type = TOKEN_GOTO;
    else if (strcmp(value, "pause") == 0) type = TOKEN_PAUSE;
    
    // ⭐ PMPL State Keywords (RF_YZ_2)
    else if (strcmp(value, "shared_state") == 0) type = TOKEN_SHARED_STATE;
    
    // Control flow keywords (single keywords - existing)
    else if (strcmp(value, "if") == 0) type = TOKEN_IF;
    else if (strcmp(value, "then") == 0) type = TOKEN_THEN;
    else if (strcmp(value, "else") == 0) type = TOKEN_ELSE;
    else if (strcmp(value, "end") == 0) type = TOKEN_END;
    else if (strcmp(value, "while") == 0) type = TOKEN_WHILE;
    else if (strcmp(value, "do") == 0) type = TOKEN_DO;
    else if (strcmp(value, "for") == 0) type = TOKEN_FOR;
    else if (strcmp(value, "to") == 0) type = TOKEN_TO;
    else if (strcmp(value, "downto") == 0) type = TOKEN_DOWNTO;
    else if (strcmp(value, "each") == 0) type = TOKEN_EACH;       // YZ_28: for each
    else if (strcmp(value, "in") == 0) type = TOKEN_IN;           // YZ_28: for each item in
    else if (strcmp(value, "exit") == 0) type = TOKEN_EXIT;       // YZ_28: exit, exit for, exit while, exit if
    // Function keywords
    else if (strcmp(value, "function") == 0) type = TOKEN_FUNCTION;
    else if (strcmp(value, "return") == 0) type = TOKEN_RETURN;
    else if (strcmp(value, "returns") == 0) type = TOKEN_RETURNS;
    // Module keywords
    else if (strcmp(value, "import") == 0) type = TOKEN_IMPORT;
    // Struct keywords
    else if (strcmp(value, "struct") == 0) type = TOKEN_STRUCT;
    else if (strcmp(value, "enum") == 0) type = TOKEN_ENUM;
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
    else if (strcmp(value, "println") == 0) type = TOKEN_PRINTLN;  // YZ_48
    
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
    
    // YZ_24: Track if whitespace exists before next token
    int had_whitespace = 0;
    if (lexer->pos > 0 && (lexer->source[lexer->pos - 1] == ' ' || 
                           lexer->source[lexer->pos - 1] == '\t' ||
                           lexer->source[lexer->pos - 1] == '\r' ||
                           lexer->source[lexer->pos - 1] == '\n')) {
        had_whitespace = 1;
    }
    
    while (1) {
        // YZ_24: Check whitespace BEFORE skipping
        if (lexer->source[lexer->pos] == ' ' || 
            lexer->source[lexer->pos] == '\t' ||
            lexer->source[lexer->pos] == '\r' ||
            lexer->source[lexer->pos] == '\n') {
            had_whitespace = 1;
        }
        
        skip_whitespace(lexer);
        
        // Check for comment using comments module
        if (is_comment_start(lexer->source, lexer->pos)) {
            lexer->pos = skip_comment(lexer->source, lexer->pos, &lexer->line);
            had_whitespace = 1;  // YZ_24: Comments count as whitespace separator
            continue;  // Re-check for more whitespace/comments
        }
        break;
    }
    
    if (lexer->source[lexer->pos] == '\0') {
        Token* token = make_token(TOKEN_EOF, NULL, lexer->line);
        token->has_leading_whitespace = had_whitespace;
        return token;
    }
    
    char c = lexer->source[lexer->pos];
    
    // Two-character operators (must check before single-char)
    if (c == '=' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_EQUAL, "==", lexer->line, had_whitespace);
    }
    
    if (c == '!' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_NOT_EQUAL, "!=", lexer->line, had_whitespace);
    }
    
    if (c == '<' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_LESS_EQUAL, "<=", lexer->line, had_whitespace);
    }
    
    if (c == '>' && lexer->source[lexer->pos + 1] == '=') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_GREATER_EQUAL, ">=", lexer->line, had_whitespace);
    }
    
    // Logical operators (two-character)
    if (c == '&' && lexer->source[lexer->pos + 1] == '&') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_AND, "&&", lexer->line, had_whitespace);
    }
    
    if (c == '|' && lexer->source[lexer->pos + 1] == '|') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_OR, "||", lexer->line, had_whitespace);
    }
    
    // Power operator (two-character **)
    if (c == '*' && lexer->source[lexer->pos + 1] == '*') {
        lexer->pos += 2;
        return make_token_ws(TOKEN_POWER, "**", lexer->line, had_whitespace);
    }
    
    // Single-character operators
    if (c == '=') {
        lexer->pos++;
        return make_token_ws(TOKEN_ASSIGN, "=", lexer->line, had_whitespace);
    }
    
    if (c == '<') {
        lexer->pos++;
        // Lookahead: Tuple literal <x,y> vs comparison x < y
        // Key insight: In comparison "x < 3", there's whitespace AFTER <
        // In tuple "<3,4>", there's NO whitespace after <
        char next = lexer->source[lexer->pos];
        
        // If there's whitespace immediately after <, it's a comparison operator
        // e.g., "x < 3" has space after <
        if (next == ' ' || next == '\t') {
            return make_token_ws(TOKEN_LESS, "<", lexer->line, had_whitespace);
        }
        
        // If there's whitespace BEFORE < (had_whitespace), it's likely comparison
        // e.g., "x < 3" - the < has whitespace before it
        if (had_whitespace) {
            return make_token_ws(TOKEN_LESS, "<", lexer->line, had_whitespace);
        }
        
        // No whitespace before or after - likely tuple literal <x,y>
        // or could be <> empty tuple
        return make_token_ws(TOKEN_LANGLE, "<", lexer->line, had_whitespace);
    }
    
    if (c == '>') {
        lexer->pos++;
        // Always TOKEN_GREATER for comparison
        // Parser converts to tuple close if needed
        return make_token_ws(TOKEN_GREATER, ">", lexer->line, had_whitespace);
    }
    
    // Arithmetic operators
    if (c == '+') {
        lexer->pos++;
        return make_token_ws(TOKEN_PLUS, "+", lexer->line, had_whitespace);
    }
    
    if (c == '-') {
        lexer->pos++;
        return make_token_ws(TOKEN_MINUS, "-", lexer->line, had_whitespace);
    }
    
    if (c == '*') {
        lexer->pos++;
        return make_token_ws(TOKEN_MULTIPLY, "*", lexer->line, had_whitespace);
    }
    
    if (c == '/') {
        lexer->pos++;
        return make_token_ws(TOKEN_DIVIDE, "/", lexer->line, had_whitespace);
    }
    
    if (c == '%') {
        lexer->pos++;
        return make_token_ws(TOKEN_MOD, "%", lexer->line, had_whitespace);
    }
    
    // Bitwise operators (single-character, checked after && and ||)
    if (c == '&') {
        lexer->pos++;
        return make_token_ws(TOKEN_AND, "&", lexer->line, had_whitespace);
    }
    
    if (c == '|') {
        lexer->pos++;
        return make_token_ws(TOKEN_OR, "|", lexer->line, had_whitespace);
    }
    
    if (c == '^') {
        lexer->pos++;
        return make_token_ws(TOKEN_XOR, "^", lexer->line, had_whitespace);
    }
    
    // Logical NOT (single-character, checked after !=)
    if (c == '!') {
        lexer->pos++;
        return make_token_ws(TOKEN_NOT, "!", lexer->line, had_whitespace);
    }
    
    // Delimiters
    if (c == '(') {
        lexer->pos++;
        return make_token_ws(TOKEN_LPAREN, "(", lexer->line, had_whitespace);  // YZ_24: Critical for list validation!
    }
    
    if (c == ')') {
        lexer->pos++;
        return make_token_ws(TOKEN_RPAREN, ")", lexer->line, had_whitespace);
    }
    
    if (c == ',') {
        lexer->pos++;
        return make_token_ws(TOKEN_COMMA, ",", lexer->line, had_whitespace);
    }
    
    if (c == ':') {
        lexer->pos++;
        return make_token_ws(TOKEN_COLON, ":", lexer->line, had_whitespace);
    }
    
    if (c == ';') {
        lexer->pos++;
        return make_token_ws(TOKEN_SEMICOLON, ";", lexer->line, had_whitespace);
    }
    
    if (c == '[') {
        lexer->pos++;
        return make_token_ws(TOKEN_LBRACKET, "[", lexer->line, had_whitespace);
    }
    
    if (c == ']') {
        lexer->pos++;
        return make_token_ws(TOKEN_RBRACKET, "]", lexer->line, had_whitespace);
    }
    
    if (c == '.') {
        lexer->pos++;
        return make_token_ws(TOKEN_DOT, ".", lexer->line, had_whitespace);
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
            Token* token = read_identifier(lexer);
            token->has_leading_whitespace = had_whitespace;  // YZ_24: Set flag
            return token;
        }
        return make_token_ws(TOKEN_CONTINUATION, c == '_' ? "_" : "\\", lexer->line, had_whitespace);
    }
    
    if (isdigit(c)) {
        Token* token = read_number(lexer);
        token->has_leading_whitespace = had_whitespace;  // YZ_24: Set flag
        return token;
    }
    
    if (c == '"') {
        Token* token = read_string(lexer);
        token->has_leading_whitespace = had_whitespace;  // YZ_24: Set flag
        return token;
    }
    
    if (isalpha(c) || c == '_') {
        Token* token = read_identifier(lexer);
        token->has_leading_whitespace = had_whitespace;  // YZ_24: Set flag
        return token;
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
        return make_token_ws(TOKEN_ERROR, error_msg, lexer->line, had_whitespace);
    }
    
    // YZ_41: Unknown character - advance position to avoid infinite loop
    lexer->pos++;
    char error_msg[100];
    snprintf(error_msg, sizeof(error_msg), "Unexpected character '%c' (0x%02X) at line %d", c, (unsigned char)c, lexer->line);
    return make_token_ws(TOKEN_ERROR, error_msg, lexer->line, had_whitespace);
}
