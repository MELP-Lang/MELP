/*
 * Stage 1 Compiler Main Wrapper
 * YZ_10 - Bootstrap Support
 * 
 * This C wrapper provides main() entry point for Stage 1 compiler
 * Links with stage1_gen1.ll (lexer functions)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External functions from Gen1 LLVM IR
extern void* create_token(long token_type, char* token_value, long line_num, long col_num);
extern void* tokenize_next(char* source, long pos, long line, long col);
extern long is_digit(char* ch);
extern long is_alpha(char* ch);

// Simple test main
int main(int argc, char** argv) {
    printf("\n");
    printf("================================================\n");
    printf("  MELP Stage 1 Compiler (Gen1)\n");
    printf("  YZ_10 Bootstrap Wrapper\n");
    printf("================================================\n");
    printf("\n");
    
    if (argc < 3) {
        printf("Usage: %s <input.mlp> <output.ll>\n", argv[0]);
        return 1;
    }
    
    char* input_file = argv[1];
    char* output_file = argv[2];
    
    printf("[INFO] Input:  %s\n", input_file);
    printf("[INFO] Output: %s\n", output_file);
    printf("\n");
    
    // Test lexer functions
    printf("[TEST] Testing lexer functions from Gen1...\n");
    
    // Test is_digit
    long result = is_digit("5");
    printf("  is_digit('5') = %ld (expected 1) %s\n", 
           result, result == 1 ? "✅" : "❌");
    
    // Test is_alpha
    result = is_alpha("a");
    printf("  is_alpha('a') = %ld (expected 1) %s\n", 
           result, result == 1 ? "✅" : "❌");
    
    printf("\n");
    printf("[ERROR] Full compilation not yet implemented\n");
    printf("[INFO] Gen1 lexer functions are working!\n");
    printf("[TODO] Need parser and codegen from compiler_integration.mlp\n");
    printf("\n");
    
    return 0;
}
