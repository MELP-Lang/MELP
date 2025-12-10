#include <stdio.h>
#include <stdlib.h>
#include "modules/lexer/lexer.h"

static char* read_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    return content;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_FUNCTION: return "FUNCTION";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_NUMERIC: return "NUMERIC";
        case TOKEN_RETURNS: return "RETURNS";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_DO: return "DO";
        case TOKEN_END: return "END";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_LT: return "LT";
        case TOKEN_PLUS: return "PLUS";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <file.mlp>\n", argv[0]);
        return 1;
    }
    
    char* source = read_file(argv[1]);
    if (!source) {
        printf("Failed to read file\n");
        return 1;
    }
    
    Lexer* lexer = lexer_create(source);
    
    int count = 0;
    while (1) {
        Token* tok = lexer_next_token(lexer);
        if (!tok) break;
        
        printf("%3d: Line %d: %-12s", count++, tok->line, token_type_name(tok->type));
        if (tok->value) {
            printf(" '%s'", tok->value);
        }
        printf("\n");
        
        if (tok->type == TOKEN_EOF) {
            token_free(tok);
            break;
        }
        token_free(tok);
    }
    
    lexer_free(lexer);
    free(source);
    return 0;
}
