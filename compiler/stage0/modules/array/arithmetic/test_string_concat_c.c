#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../../error/error.h"
#include "arithmetic.h"
#include "arithmetic_parser.h"
#include "arithmetic_codegen.h"

int main() {
    error_init();
    
    // Test: "Hello" + " " + "World"
    const char* source = "\"Hello\" + \" \" + \"World\"";
    
    printf("Testing string concatenation\n");
    printf("Source: %s\n\n", source);
    
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Failed to create lexer\n");
        return 1;
    }
    
    // Get first token
    Token* tok = lexer_next_token(lexer);
    
    // Parse
    ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, &tok);
    
    if (!expr) {
        fprintf(stderr, "Failed to parse expression\n");
        lexer_free(lexer);
        return 1;
    }
    
    printf("✅ Parsed successfully!\n");
    printf("Expression type: ");
    if (expr->is_string) printf("STRING\n");
    else if (expr->is_float) printf("FLOAT\n");
    else printf("INTEGER\n");
    
    // Generate assembly
    printf("\nGenerated Assembly:\n");
    printf("==================\n");
    arithmetic_generate_code(stdout, expr, NULL);
    printf("\n");
    
    arithmetic_expr_free(expr);
    lexer_free(lexer);
    
    return 0;
}
