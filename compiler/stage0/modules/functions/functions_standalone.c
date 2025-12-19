// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../import/import.h"
#include "../import/import_parser.h"
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
    
    // YZ_30: Set current source file for relative import resolution
    import_set_current_source_file(input_file);
    
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
    
    // Parse all functions (and handle imports)
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    
    while (1) {
        // Peek next token to check for import
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // YZ_30: Handle import statements at top level
        if (tok->type == TOKEN_IMPORT) {
            ImportStatement* import_stmt = import_parse(lexer, tok);
            token_free(tok);
            
            if (import_stmt && import_stmt->is_resolved && import_stmt->module_path) {
                printf("📦 Import: %s\n", import_stmt->module_name);
                
                // Load and parse the imported module
                FunctionDeclaration* imported_funcs = import_load_module(import_stmt->module_path);
                if (imported_funcs) {
                    // Add imported functions to our list
                    if (!functions) {
                        functions = imported_funcs;
                        last_func = imported_funcs;
                    } else {
                        last_func->next = imported_funcs;
                    }
                    // Find new last function
                    while (last_func->next) last_func = last_func->next;
                }
            }
            
            if (import_stmt) import_statement_free(import_stmt);
            continue;
        }
        
        // Put token back for function parser
        lexer_unget_token(lexer, tok);
        
        // Parse function declaration
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
