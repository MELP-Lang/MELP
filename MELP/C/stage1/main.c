#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

// Read file to string
char* read_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    
    return content;
}

// Write string to file
int write_file(const char* filename, const char* content) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: Cannot create file '%s'\n", filename);
        return 0;
    }
    
    fprintf(f, "%s", content);
    fclose(f);
    return 1;
}

// Tokenize source
Token** tokenize_source(const char* source, int* token_count) {
    Lexer* lexer = lexer_create(source);
    
    // Allocate token array
    int capacity = 1000;
    Token** tokens = malloc(sizeof(Token*) * capacity);
    int count = 0;
    
    while (1) {
        Token* token = lexer_next_token(lexer);
        if (!token) break;
        
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, sizeof(Token*) * capacity);
        }
        
        tokens[count++] = token;
        
        if (token->type == TOKEN_EOF) break;
    }
    
    lexer_free(lexer);
    *token_count = count;
    return tokens;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "MLP Stage1 Bridge Compiler\n");
        fprintf(stderr, "Usage: %s <input.mlp> <output.c>\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    
    printf("=== MLP Stage1 Compiler ===\n");
    printf("Input:  %s\n", input_file);
    printf("Output: %s\n", output_file);
    printf("\n");
    
    // Read source
    printf("[1/4] Reading source...\n");
    char* source = read_file(input_file);
    if (!source) return 1;
    
    // Tokenize
    printf("[2/4] Lexical analysis...\n");
    int token_count = 0;
    Token** tokens = tokenize_source(source, &token_count);
    printf("  Generated %d tokens\n", token_count);
    
    // Parse
    printf("[3/4] Parsing...\n");
    ParserState* parser = parser_create(tokens, token_count);
    ASTNode* ast = parse_program(parser);
    
    if (parser->has_error) {
        fprintf(stderr, "Parse error: %s\n", parser->error_message);
        parser_free(parser);
        return 1;
    }
    printf("  AST constructed\n");
    
    // Generate C code
    printf("[4/4] Code generation...\n");
    CodegenState* codegen = codegen_create();
    codegen_program(codegen, ast);
    char* c_code = codegen_get_output(codegen);
    
    // Write output
    if (!write_file(output_file, c_code)) {
        return 1;
    }
    
    printf("\n✅ Compilation successful!\n");
    printf("Generated: %s\n", output_file);
    
    // Cleanup
    free(source);
    for (int i = 0; i < token_count; i++) {
        token_free(tokens[i]);
    }
    free(tokens);
    parser_free(parser);
    ast_free(ast);
    codegen_free(codegen);
    
    return 0;
}
