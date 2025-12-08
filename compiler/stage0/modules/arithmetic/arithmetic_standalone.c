#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "arithmetic_parser.h"
#include "arithmetic_codegen.h"
#include "../variable/variable.h"
#include "../variable/variable_parser.h"

// List of parsed assignments
typedef struct AssignmentNode {
    char* var_name;
    ArithmeticExpr* expr;
    struct AssignmentNode* next;
} AssignmentNode;

static AssignmentNode* assignment_list = NULL;
static int expr_count = 0;

// Add assignment to list
static void add_assignment(char* var_name, ArithmeticExpr* expr) {
    AssignmentNode* node = malloc(sizeof(AssignmentNode));
    node->var_name = var_name;
    node->expr = expr;
    node->next = assignment_list;
    assignment_list = node;
    expr_count++;
}

// Parse file and collect arithmetic assignments
static void arithmetic_parse_file(Lexer* lexer) {
    ArithmeticParser* parser = arithmetic_parser_create(lexer);
    
    while (parser->current_token && parser->current_token->type != TOKEN_EOF) {
        // Check for variable declaration with assignment
        if (parser->current_token->type == TOKEN_NUMERIC) {
            token_free(parser->current_token);
            parser->current_token = lexer_next_token(parser->lexer);
            
            if (parser->current_token->type == TOKEN_IDENTIFIER) {
                char* var_name = strdup(parser->current_token->value);
                token_free(parser->current_token);
                parser->current_token = lexer_next_token(parser->lexer);
                
                if (parser->current_token->type == TOKEN_ASSIGN) {
                    token_free(parser->current_token);
                    parser->current_token = lexer_next_token(parser->lexer);
                    
                    // Parse the expression
                    ArithmeticExpr* expr = arithmetic_parse_expression(parser);
                    
                    if (expr) {
                        add_assignment(var_name, expr);
                        printf("  ✓ Parsed: %s = <expression>\n", var_name);
                    } else {
                        free(var_name);
                    }
                } else {
                    free(var_name);
                }
            }
        } else if (parser->current_token->type == TOKEN_IDENTIFIER) {
            // Check for assignment to existing variable
            char* var_name = strdup(parser->current_token->value);
            token_free(parser->current_token);
            parser->current_token = lexer_next_token(parser->lexer);
            
            if (parser->current_token->type == TOKEN_ASSIGN) {
                token_free(parser->current_token);
                parser->current_token = lexer_next_token(parser->lexer);
                
                ArithmeticExpr* expr = arithmetic_parse_expression(parser);
                
                if (expr) {
                    add_assignment(var_name, expr);
                    printf("  ✓ Parsed: %s = <expression>\n", var_name);
                } else {
                    free(var_name);
                }
            } else {
                free(var_name);
            }
        } else {
            // Skip unknown tokens
            token_free(parser->current_token);
            parser->current_token = lexer_next_token(parser->lexer);
        }
    }
    
    arithmetic_parser_free(parser);
}

// Generate assembly code
static void arithmetic_codegen_file(FILE* output) {
    fprintf(output, "; Arithmetic expressions - Full parser/codegen\n");
    
    // External TTO runtime functions
    fprintf(output, "extern tto_bigdec_from_int64\n");
    fprintf(output, "extern tto_bigdec_add\n");
    fprintf(output, "extern tto_bigdec_sub\n");
    fprintf(output, "extern tto_bigdec_mul\n");
    fprintf(output, "extern tto_bigdec_div\n");
    
    fprintf(output, "\nsection .data\n");
    
    // Declare variables
    AssignmentNode* node = assignment_list;
    while (node) {
        fprintf(output, "    %s: dq 0  ; Variable\n", node->var_name);
        node = node->next;
    }
    
    fprintf(output, "\nsection .text\n");
    fprintf(output, "global _start\n");
    fprintf(output, "_start:\n");
    
    // Generate code for each assignment
    node = assignment_list;
    while (node) {
        arithmetic_generate_assignment(output, node->var_name, node->expr);
        node = node->next;
    }
    
    // Exit
    fprintf(output, "\n    ; Exit program\n");
    fprintf(output, "    mov rax, 60  ; sys_exit\n");
    fprintf(output, "    xor rdi, rdi  ; exit code 0\n");
    fprintf(output, "    syscall\n");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input.mlp> <output.s>\n", argv[0]);
        return 1;
    }

    printf("🔧 Arithmetic Module - Full Parser\n");
    printf("====================================\n");
    printf("Input:  %s\n", argv[1]);
    printf("Output: %s\n\n", argv[2]);

    // Read input file
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "❌ Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    char *source = malloc(file_size + 1);
    fread(source, 1, file_size, input);
    source[file_size] = '\0';
    fclose(input);

    // Create lexer and parse
    Lexer *lexer = lexer_create(source);
    arithmetic_parse_file(lexer);
    lexer_free(lexer);

    printf("\n  ✓ Expressions parsed: %d\n", expr_count);

    // Generate output
    FILE *output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "❌ Error: Cannot create output file %s\n", argv[2]);
        free(source);
        return 1;
    }

    arithmetic_codegen_file(output);
    fclose(output);
    free(source);

    // Cleanup
    while (assignment_list) {
        AssignmentNode* node = assignment_list;
        assignment_list = node->next;
        free(node->var_name);
        arithmetic_expr_free(node->expr);
        free(node);
    }

    printf("\n✅ Arithmetic module complete!\n");
    return 0;
}
