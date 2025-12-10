// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../error/error.h"
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
    // Initialize error handling system
    error_init();
    
    if (argc != 3) {
        error_io("Usage: %s <input.mlp> <output.s>", argv[0]);
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
    
    // Phase 6: Set source code for enhanced error messages
    error_set_source(source, input_file);
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        error_fatal("Failed to create lexer");
        free(source);
        return 1;
    }
    
    // Parse all functions (STATELESS - no Parser struct needed)
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    
    while (1) {
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) {
            break;  // Stop on first error or EOF
        }
        
        if (!functions) {
            functions = func;
            last_func = func;
        } else {
            last_func->next = func;
            last_func = func;
        }
    }
    
    // Close lexer
    lexer_free(lexer);
    
    // Check if we have any errors
    if (error_has_errors()) {
        error_print_summary();
        free(source);
        return 1;
    }
    
    // Check if we parsed any functions
    if (!functions) {
        error_warning(0, "No functions found in input file");
        free(source);
        return 0;  // Not an error, just nothing to do
    }
    
    // Open output file
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening output file");
        return 1;
    }
    
    // Generate assembly header
    // Phase 6: Add .rodata section for runtime error messages
    fprintf(output, ".section .rodata\n");
    fprintf(output, ".div_zero_msg:\n");
    fprintf(output, "    .string \"Division by zero is not allowed!\"\n");
    fprintf(output, "\n");
    
    // Generate AT&T syntax assembly (default for GCC)
    // fprintf(output, ".intel_syntax noprefix\n");  // Disabled: using AT&T
    fprintf(output, ".text\n\n");
    
    // Generate code for each function
    FunctionDeclaration* func = functions;
    while (func) {
        function_generate_declaration(output, func);
        func = func->next;
    }
    
    // Close output
    fclose(output);
    
    // Free source code
    free(source);
    
    printf("✅ Compiled %s -> %s\n", input_file, output_file);
    return 0;
}
