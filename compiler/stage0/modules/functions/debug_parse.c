#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "../parser_core/parser_core.h"
#include "../functions/functions_parser.h"

char* read_file(const char* path) {
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

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    
    char* source = read_file(argv[1]);
    if (!source) return 1;
    
    printf("Source length: %ld\n", strlen(source));
    printf("First 50 chars: %.50s\n", source);
    
    Lexer* lexer = lexer_create(source);
    Parser* parser = parser_create(lexer);
    
    printf("\nFirst 5 tokens:\n");
    for (int i = 0; i < 5; i++) {
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) break;
        printf("  %d: type=%d value='%s'\n", i, tok->type, tok->value ? tok->value : "NULL");
        lexer_unget_token(lexer, tok);
        tok = lexer_next_token(lexer);
        token_free(tok);
    }
    
    parser_free(parser);
    lexer_free(lexer);
    free(source);
    return 0;
}
