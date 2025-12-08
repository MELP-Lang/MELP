// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "functions.h"
#include "functions_parser.h"
#include "functions_codegen.h"

// Read entire file into string
static char* read_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    
    return content;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.mlp> <output.s>\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    
    // Read input file
    char* source = read_file(input_file);
    if (!source) {
        perror("Error reading input file");
        return 1;
    }
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Failed to create lexer\n");
        free(source);
        return 1;
    }
    
    // Parse all functions
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    
    while (1) {
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) break;
        
        if (!functions) {
            functions = func;
        } else {
            last_func->next = func;
        }
        last_func = func;
    }
    
    // Close lexer
    lexer_free(lexer);
    free(source);
    
    // Open output file
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening output file");
        return 1;
    }
    
    // Generate assembly header
    fprintf(output, ".intel_syntax noprefix\n");
    fprintf(output, ".text\n\n");
    
    // Generate code for each function
    FunctionDeclaration* func = functions;
    while (func) {
        function_generate_declaration(output, func);
        func = func->next;
    }
    
    // Close output
    fclose(output);
    
    printf("✅ Compiled %s -> %s\n", input_file, output_file);
    return 0;
}
