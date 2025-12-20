// ============================================================================
// test_const_simple.c - Simple Const Test (YZ_CONST)
// ============================================================================
// Purpose: Test const keyword lexer and parser support
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "modules/lexer/lexer.h"
#include "modules/variable/variable_parser.h"

int main() {
    printf("=== YZ_CONST: Const Keyword Test ===\n\n");
    
    // Test 1: Const numeric
    const char* test1 = "const numeric PI = 3.14159";
    printf("Test 1: %s\n", test1);
    
    Lexer* lexer1 = lexer_create(test1);
    Token* tok1 = lexer_next_token(lexer1);
    
    if (tok1 && tok1->type == TOKEN_CONST) {
        printf("  ✓ TOKEN_CONST recognized\n");
        
        VariableDeclaration* decl1 = variable_parse_declaration(lexer1, tok1);
        token_free(tok1);
        
        if (decl1) {
            printf("  ✓ Parsed successfully\n");
            printf("    Name: %s\n", decl1->name);
            printf("    Type: %s\n", decl1->type == VAR_NUMERIC ? "numeric" : "other");
            printf("    Value: %s\n", decl1->value);
            printf("    Is Const: %s\n", decl1->is_const ? "yes" : "no");
            variable_declaration_free(decl1);
        } else {
            printf("  ✗ Parse failed\n");
        }
    } else {
        printf("  ✗ TOKEN_CONST not recognized\n");
        if (tok1) token_free(tok1);
    }
    
    lexer_free(lexer1);
    printf("\n");
    
    // Test 2: Const string
    const char* test2 = "const string APP_NAME = \"MELP\"";
    printf("Test 2: %s\n", test2);
    
    Lexer* lexer2 = lexer_create(test2);
    Token* tok2 = lexer_next_token(lexer2);
    
    if (tok2 && tok2->type == TOKEN_CONST) {
        printf("  ✓ TOKEN_CONST recognized\n");
        
        VariableDeclaration* decl2 = variable_parse_declaration(lexer2, tok2);
        token_free(tok2);
        
        if (decl2) {
            printf("  ✓ Parsed successfully\n");
            printf("    Name: %s\n", decl2->name);
            printf("    Type: %s\n", decl2->type == VAR_STRING ? "string" : "other");
            printf("    Value: %s\n", decl2->value);
            printf("    Is Const: %s\n", decl2->is_const ? "yes" : "no");
            variable_declaration_free(decl2);
        } else {
            printf("  ✗ Parse failed\n");
        }
    } else {
        printf("  ✗ TOKEN_CONST not recognized\n");
        if (tok2) token_free(tok2);
    }
    
    lexer_free(lexer2);
    printf("\n");
    
    // Test 3: Const without initializer (should fail)
    const char* test3 = "const numeric X";
    printf("Test 3: %s (should fail)\n", test3);
    
    Lexer* lexer3 = lexer_create(test3);
    Token* tok3 = lexer_next_token(lexer3);
    
    if (tok3 && tok3->type == TOKEN_CONST) {
        VariableDeclaration* decl3 = variable_parse_declaration(lexer3, tok3);
        token_free(tok3);
        
        if (decl3) {
            printf("  ✗ Should have failed (const requires initializer)\n");
            variable_declaration_free(decl3);
        } else {
            printf("  ✓ Correctly rejected (const requires initializer)\n");
        }
    }
    
    lexer_free(lexer3);
    printf("\n");
    
    printf("=== All tests complete ===\n");
    return 0;
}
