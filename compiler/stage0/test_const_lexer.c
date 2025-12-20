// ============================================================================
// test_const_lexer.c - Lexer Const Test (YZ_CONST)
// ============================================================================
// Purpose: Test TOKEN_CONST lexer support only
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "modules/lexer/lexer.h"

int main() {
    printf("=== YZ_CONST: Lexer Test ===\n\n");
    
    // Test 1: const keyword
    const char* test1 = "const numeric PI = 3.14";
    printf("Test 1: %s\n", test1);
    
    Lexer* lexer = lexer_create(test1);
    Token* tok;
    int token_count = 0;
    
    while ((tok = lexer_next_token(lexer)) != NULL && tok->type != TOKEN_EOF) {
        token_count++;
        
        printf("  Token %d: ", token_count);
        
        switch (tok->type) {
            case TOKEN_CONST:
                printf("TOKEN_CONST");
                break;
            case TOKEN_NUMERIC:
                printf("TOKEN_NUMERIC");
                break;
            case TOKEN_IDENTIFIER:
                printf("TOKEN_IDENTIFIER (%s)", tok->value);
                break;
            case TOKEN_ASSIGN:
                printf("TOKEN_ASSIGN");
                break;
            case TOKEN_NUMBER:
                printf("TOKEN_NUMBER (%s)", tok->value);
                break;
            default:
                printf("OTHER (type=%d)", tok->type);
        }
        
        printf("\n");
        token_free(tok);
    }
    
    if (tok) token_free(tok);
    lexer_free(lexer);
    
    printf("\n");
    
    // Test 2: Multiple const declarations
    const char* test2 = "const numeric X = 10\nconst string NAME = \"Test\"";
    printf("Test 2: Multi-line const\n");
    
    lexer = lexer_create(test2);
    token_count = 0;
    
    while ((tok = lexer_next_token(lexer)) != NULL && tok->type != TOKEN_EOF) {
        token_count++;
        
        if (tok->type == TOKEN_CONST) {
            printf("  ✓ Found TOKEN_CONST at position %d\n", token_count);
        }
        
        token_free(tok);
    }
    
    if (tok) token_free(tok);
    lexer_free(lexer);
    
    printf("\n=== Test Complete ===\n");
    return 0;
}
