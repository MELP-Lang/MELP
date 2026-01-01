/* fmt_formatter.c - MLP Code Formatter Implementation */
#include "fmt_formatter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Create formatter */
Formatter* formatter_create(Token** tokens, int token_count, FormatOptions options) {
    Formatter* fmt = malloc(sizeof(Formatter));
    if (!fmt) return NULL;
    
    fmt->tokens = tokens;
    fmt->token_count = token_count;
    fmt->current_token = 0;
    fmt->indent_level = 0;
    
    fmt->output_capacity = 1024;
    fmt->output = malloc(fmt->output_capacity);
    fmt->output[0] = '\0';
    fmt->output_size = 0;
    
    fmt->options = options;
    
    return fmt;
}

/* Free formatter */
void formatter_free(Formatter* fmt) {
    if (!fmt) return;
    free(fmt->output);
    free(fmt);
}

/* Append string to output */
void formatter_append(Formatter* fmt, const char* str) {
    int len = strlen(str);
    while (fmt->output_size + len + 1 >= fmt->output_capacity) {
        fmt->output_capacity *= 2;
        fmt->output = realloc(fmt->output, fmt->output_capacity);
    }
    strcpy(fmt->output + fmt->output_size, str);
    fmt->output_size += len;
}

/* Append indentation */
void formatter_append_indent(Formatter* fmt) {
    for (int i = 0; i < fmt->indent_level * fmt->options.indent_size; i++) {
        formatter_append(fmt, " ");
    }
}

/* Append space */
void formatter_append_space(Formatter* fmt) {
    formatter_append(fmt, " ");
}

/* Append newline */
void formatter_append_newline(Formatter* fmt) {
    formatter_append(fmt, "\n");
}

/* Current token */
static Token* current_token(Formatter* fmt) {
    if (fmt->current_token >= fmt->token_count) {
        return NULL;
    }
    return fmt->tokens[fmt->current_token];
}

/* Peek next token */
static Token* peek_token(Formatter* fmt, int offset) {
    int index = fmt->current_token + offset;
    if (index >= fmt->token_count) {
        return NULL;
    }
    return fmt->tokens[index];
}

/* Advance to next token */
static void advance_token(Formatter* fmt) {
    fmt->current_token++;
}

/* Check if token is block starter (fn, if, while, etc.) */
static bool is_block_starter(Token* token) {
    if (!token || token->type != TOKEN_KEYWORD) return false;
    return strcmp(token->value, "fn") == 0 ||
           strcmp(token->value, "if") == 0 ||
           strcmp(token->value, "elif") == 0 ||
           strcmp(token->value, "else") == 0 ||
           strcmp(token->value, "while") == 0 ||
           strcmp(token->value, "for") == 0;
}

/* Format tokens */
char* formatter_format(Formatter* fmt) {
    bool start_of_line = true;
    bool after_colon = false;
    
    while (current_token(fmt) && current_token(fmt)->type != TOKEN_EOF) {
        Token* token = current_token(fmt);
        Token* next = peek_token(fmt, 1);
        
        // Handle newlines
        if (token->type == TOKEN_NEWLINE) {
            formatter_append_newline(fmt);
            start_of_line = true;
            after_colon = false;
            advance_token(fmt);
            continue;
        }
        
        // Handle comments
        if (token->type == TOKEN_COMMENT) {
            if (!start_of_line) {
                formatter_append_space(fmt);
                formatter_append_space(fmt);
            }
            formatter_append(fmt, token->value);
            advance_token(fmt);
            continue;
        }
        
        // Add indentation at start of line
        if (start_of_line) {
            formatter_append_indent(fmt);
            start_of_line = false;
        }
        
        // Add token value
        formatter_append(fmt, token->value);
        
        // Handle indentation after colon
        if (token->type == TOKEN_COLON) {
            after_colon = true;
            // Check if next token is newline
            if (next && next->type == TOKEN_NEWLINE) {
                fmt->indent_level++;
            }
        }
        
        // Decrease indent for certain keywords
        if (token->type == TOKEN_KEYWORD) {
            if (strcmp(token->value, "elif") == 0 || 
                strcmp(token->value, "else") == 0) {
                // Temporarily decrease for elif/else
            }
            if (strcmp(token->value, "return") == 0 ||
                strcmp(token->value, "break") == 0 ||
                strcmp(token->value, "continue") == 0) {
                // These might end a block
                if (next && next->type == TOKEN_NEWLINE) {
                    Token* next_next = peek_token(fmt, 2);
                    if (next_next && !is_block_starter(next_next) && 
                        next_next->type == TOKEN_KEYWORD) {
                        // Likely end of block
                    }
                }
            }
        }
        
        // Add spacing
        if (next && next->type != TOKEN_EOF && next->type != TOKEN_NEWLINE) {
            // Space after keyword
            if (token->type == TOKEN_KEYWORD) {
                formatter_append_space(fmt);
            }
            // Space around operators
            else if (token->type == TOKEN_OPERATOR && fmt->options.spaces_around_operators) {
                formatter_append_space(fmt);
            }
            // Space after comma
            else if (token->type == TOKEN_COMMA) {
                formatter_append_space(fmt);
            }
            // Space before colon (type annotations)
            else if (next->type == TOKEN_COLON && 
                     (token->type == TOKEN_IDENTIFIER || token->type == TOKEN_RPAREN)) {
                // No space before colon in type annotations
            }
            // Space after colon (but not if followed by newline)
            else if (token->type == TOKEN_COLON && next->type != TOKEN_NEWLINE) {
                formatter_append_space(fmt);
            }
            // Space before operator
            else if (next->type == TOKEN_OPERATOR && fmt->options.spaces_around_operators) {
                formatter_append_space(fmt);
            }
            // Space after arrow
            else if (token->type == TOKEN_ARROW) {
                formatter_append_space(fmt);
            }
            // No space before punctuation
            else if (next->type == TOKEN_COMMA || next->type == TOKEN_COLON ||
                     next->type == TOKEN_RPAREN || next->type == TOKEN_RBRACKET) {
                // No space
            }
            // No space after opening brackets
            else if (token->type == TOKEN_LPAREN || token->type == TOKEN_LBRACKET) {
                // No space
            }
        }
        
        advance_token(fmt);
    }
    
    // Ensure final newline
    if (fmt->output_size > 0 && fmt->output[fmt->output_size - 1] != '\n') {
        formatter_append_newline(fmt);
    }
    
    return fmt->output;
}
