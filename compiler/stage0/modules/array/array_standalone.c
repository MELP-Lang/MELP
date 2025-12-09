// ARRAY MODULE - STANDALONE TEST COMPILER
// Stateless parser pattern - following functions_standalone.c
// Tests array, list, and tuple parsing

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../error/error.h"
#include "array.h"
#include "array_parser.h"
#include "array_codegen.h"

// Statistics
typedef struct {
    int array_literals;
    int list_literals;
    int tuple_literals;
    int index_accesses;
} ArrayStats;

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
    // Initialize error handling
    error_init();
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.mlp> <output.s>\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "Array Module Test Compiler\n");
        fprintf(stderr, "  Tests: [1,2,3], (1;2;), <1,2,3>\n");
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    
    printf("📦 Array Module - Standalone Compiler\n");
    printf("==========================================\n");
    printf("Input:  %s\n", input_file);
    printf("Output: %s\n\n", output_file);
    
    // Read input file
    char* source = read_file(input_file);
    if (!source) {
        perror("Error reading input file");
        return 1;
    }
    
    printf("[1/3] 📖 Reading source... (%ld bytes)\n", strlen(source));
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Failed to create lexer\n");
        free(source);
        return 1;
    }
    
    printf("[2/3] 🔍 Parsing arrays...\n");
    
    // Parse arrays (STATELESS PATTERN)
    ArrayStats stats = {0};
    Collection** collections = NULL;
    int coll_count = 0;
    int coll_capacity = 10;
    collections = malloc(sizeof(Collection*) * coll_capacity);
    
    // Token loop - stateless style
    Token* tok = lexer_next_token(lexer);
    int iterations = 0;
    const int MAX_ITERATIONS = 1000;
    
    while (tok && tok->type != TOKEN_EOF && iterations++ < MAX_ITERATIONS) {
        
        if (tok->type == TOKEN_LBRACKET) {
            // Parse array literal: [1, 2, 3]
            Collection* arr = array_parse_literal(lexer, tok);  // tok is BORROWED
            if (arr) {
                stats.array_literals++;
                printf("      ✓ Array: %d elements\n", arr->data.array.length);
                
                // Store collection
                if (coll_count >= coll_capacity) {
                    coll_capacity *= 2;
                    collections = realloc(collections, sizeof(Collection*) * coll_capacity);
                }
                collections[coll_count++] = arr;
            }
            token_free(tok);
        }
        else if (tok->type == TOKEN_LPAREN) {
            // Could be list literal: (1; 2; 3;)
            // Or function call - for now assume list
            Collection* list = array_parse_list_literal(lexer, tok);  // BORROWED
            if (list) {
                stats.list_literals++;
                printf("      ✓ List: %d elements\n", list->data.list.length);
                
                if (coll_count >= coll_capacity) {
                    coll_capacity *= 2;
                    collections = realloc(collections, sizeof(Collection*) * coll_capacity);
                }
                collections[coll_count++] = list;
            }
            token_free(tok);
        }
        else if (tok->type == TOKEN_LESS) {
            // Parse tuple literal: <1, 2, 3>
            Collection* tuple = array_parse_tuple_literal(lexer, tok);  // BORROWED
            if (tuple) {
                stats.tuple_literals++;
                printf("      ✓ Tuple: %d elements\n", tuple->data.tuple.length);
                
                if (coll_count >= coll_capacity) {
                    coll_capacity *= 2;
                    collections = realloc(collections, sizeof(Collection*) * coll_capacity);
                }
                collections[coll_count++] = tuple;
            }
            token_free(tok);
        }
        else {
            // Skip other tokens
            token_free(tok);
        }
        
        tok = lexer_next_token(lexer);  // OWNED - next token
    }
    
    if (tok) token_free(tok);  // Cleanup final token
    
    printf("\n      Statistics:\n");
    printf("        - Arrays: %d\n", stats.array_literals);
    printf("        - Lists:  %d\n", stats.list_literals);
    printf("        - Tuples: %d\n", stats.tuple_literals);
    printf("        - Total:  %d\n\n", coll_count);
    
    // Close lexer
    lexer_free(lexer);
    free(source);
    
    // Check for errors
    if (error_has_errors()) {
        fprintf(stderr, "Compilation stopped due to errors\n");
        // Cleanup
        for (int i = 0; i < coll_count; i++) {
            collection_free(collections[i]);
        }
        free(collections);
        return 1;
    }
    
    printf("[3/3] ⚙️  Code generation...\n");
    
    // Open output file
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening output file");
        for (int i = 0; i < coll_count; i++) {
            collection_free(collections[i]);
        }
        free(collections);
        return 1;
    }
    
    // Generate assembly header
    fprintf(output, "; MLP Array Module - Generated Assembly\n");
    fprintf(output, "; Source: %s\n", input_file);
    fprintf(output, "; Arrays: %d, Lists: %d, Tuples: %d\n\n", 
           stats.array_literals, stats.list_literals, stats.tuple_literals);
    
    fprintf(output, "section .data\n");
    
    // Reserve space for array/list/tuple pointers
    for (int i = 0; i < coll_count; i++) {
        fprintf(output, "    array_%d: dq 0\n", i);
        if (collections[i]->type == COLL_LIST) {
            fprintf(output, "    list_%d: dq 0\n", i);
        } else if (collections[i]->type == COLL_TUPLE) {
            fprintf(output, "    tuple_%d: dq 0\n", i);
        }
    }
    
    fprintf(output, "\n    msg: db \"Array test OK!\", 10, 0\n");
    fprintf(output, "    msg_len: equ $ - msg\n\n");
    
    // External TTO functions
    fprintf(output, "extern tto_array_alloc\n");
    fprintf(output, "extern tto_list_alloc\n");
    fprintf(output, "extern tto_list_set\n");
    fprintf(output, "extern malloc\n");
    fprintf(output, "extern free\n\n");
    
    fprintf(output, "section .text\n");
    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n");
    
    // Generate code for each collection
    for (int i = 0; i < coll_count; i++) {
        fprintf(output, "    ; Collection %d (type=%d)\n", i, collections[i]->type);
        codegen_collection(output, collections[i]);
    }
    
    // Print success message
    fprintf(output, "\n    ; Print success message\n");
    fprintf(output, "    mov rax, 1\n");
    fprintf(output, "    mov rdi, 1\n");
    fprintf(output, "    lea rsi, [msg]\n");
    fprintf(output, "    mov rdx, msg_len\n");
    fprintf(output, "    syscall\n\n");
    
    // Exit
    fprintf(output, "    ; Exit\n");
    fprintf(output, "    mov rax, 60\n");
    fprintf(output, "    xor rdi, rdi\n");
    fprintf(output, "    syscall\n");
    
    fclose(output);
    printf("      ✓ Assembly generated\n\n");
    
    printf("✅ Compilation complete!\n");
    printf("==========================================\n");
    printf("📄 Output: %s\n", output_file);
    printf("📊 Collections: %d total\n", coll_count);
    printf("\nNext steps:\n");
    printf("  nasm -f elf64 %s -o output.o\n", output_file);
    printf("  ld output.o -o program\n");
    printf("  ./program\n");
    
    // Cleanup
    for (int i = 0; i < coll_count; i++) {
        collection_free(collections[i]);
    }
    free(collections);
    
    return 0;
}
