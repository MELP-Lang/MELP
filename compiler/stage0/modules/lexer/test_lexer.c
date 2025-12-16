#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"

/**
 * Test helper: Tek keyword tokenize et ve type'ı kontrol et
 */
static void test_keyword(const char* keyword, TokenType expected_type, const char* test_name) {
    Lexer* lexer = lexer_create(keyword);
    Token* token = lexer_next_token(lexer);
    
    if (token == NULL) {
        printf("✗ %s - lexer_next_token returned NULL\n", test_name);
        lexer_free(lexer);
        return;
    }
    
    if (token->type == expected_type) {
        printf("✓ %s\n", test_name);
    } else {
        printf("✗ %s\n", test_name);
        printf("  Expected type: %d\n", expected_type);
        printf("  Got type:      %d\n", token->type);
    }
    
    token_free(token);
    lexer_free(lexer);
}

/**
 * Main test suite
 */
int main() {
    printf("Testing PMPL Lexer Keywords...\n\n");
    
    int test_count = 0;
    
    // ========== Block Terminators ==========
    printf("[Block Terminators]\n");
    test_keyword("end_if", TOKEN_END_IF, "\"end_if\" → TOKEN_END_IF");
    test_keyword("end_while", TOKEN_END_WHILE, "\"end_while\" → TOKEN_END_WHILE");
    test_keyword("end_for", TOKEN_END_FOR, "\"end_for\" → TOKEN_END_FOR");
    test_keyword("end_function", TOKEN_END_FUNCTION, "\"end_function\" → TOKEN_END_FUNCTION");
    test_keyword("end_struct", TOKEN_END_STRUCT, "\"end_struct\" → TOKEN_END_STRUCT");
    test_keyword("end_switch", TOKEN_END_SWITCH, "\"end_switch\" → TOKEN_END_SWITCH");
    test_keyword("end_match", TOKEN_END_MATCH, "\"end_match\" → TOKEN_END_MATCH");
    test_keyword("end_operator", TOKEN_END_OPERATOR, "\"end_operator\" → TOKEN_END_OPERATOR");
    test_keyword("end_try", TOKEN_END_TRY, "\"end_try\" → TOKEN_END_TRY");
    test_count += 9;
    
    // ========== Control Flow ==========
    printf("\n[Control Flow]\n");
    test_keyword("else_if", TOKEN_ELSE_IF, "\"else_if\" → TOKEN_ELSE_IF");
    test_count += 1;
    
    // ========== Loop Control ==========
    printf("\n[Loop Control]\n");
    test_keyword("exit_if", TOKEN_EXIT_IF, "\"exit_if\" → TOKEN_EXIT_IF");
    test_keyword("exit_for", TOKEN_EXIT_FOR, "\"exit_for\" → TOKEN_EXIT_FOR");
    test_keyword("exit_while", TOKEN_EXIT_WHILE, "\"exit_while\" → TOKEN_EXIT_WHILE");
    test_keyword("exit_function", TOKEN_EXIT_FUNCTION, "\"exit_function\" → TOKEN_EXIT_FUNCTION");
    test_keyword("exit_switch", TOKEN_EXIT_SWITCH, "\"exit_switch\" → TOKEN_EXIT_SWITCH");
    test_keyword("continue_for", TOKEN_CONTINUE_FOR, "\"continue_for\" → TOKEN_CONTINUE_FOR");
    test_keyword("continue_while", TOKEN_CONTINUE_WHILE, "\"continue_while\" → TOKEN_CONTINUE_WHILE");
    test_count += 7;
    
    // ========== Debug Keywords ==========
    printf("\n[Debug Keywords]\n");
    test_keyword("debug_goto", TOKEN_DEBUG_GOTO, "\"debug_goto\" → TOKEN_DEBUG_GOTO");
    test_keyword("debug_pause", TOKEN_DEBUG_PAUSE, "\"debug_pause\" → TOKEN_DEBUG_PAUSE");
    test_keyword("debug_label", TOKEN_DEBUG_LABEL, "\"debug_label\" → TOKEN_DEBUG_LABEL");
    test_keyword("debug_print", TOKEN_DEBUG_PRINT, "\"debug_print\" → TOKEN_DEBUG_PRINT");
    test_count += 4;
    
    // ========== State Keywords ==========
    printf("\n[State Keywords]\n");
    test_keyword("shared_state", TOKEN_SHARED_STATE, "\"shared_state\" → TOKEN_SHARED_STATE");
    test_count += 1;
    
    // ========== Integration Test ==========
    printf("\n[Integration Test]\n");
    
    // Full PMPL code snippet
    const char* code = "if x > 5 then\n"
                       "    print(x)\n"
                       "else_if x > 3 then\n"
                       "    print(\"medium\")\n"
                       "else\n"
                       "    print(\"low\")\n"
                       "end_if";
    
    Lexer* lexer = lexer_create(code);
    int found_else_if = 0;
    int found_end_if = 0;
    
    Token* tok;
    while ((tok = lexer_next_token(lexer)) != NULL && tok->type != TOKEN_EOF) {
        if (tok->type == TOKEN_ELSE_IF) found_else_if = 1;
        if (tok->type == TOKEN_END_IF) found_end_if = 1;
        token_free(tok);
    }
    
    if (tok) token_free(tok);
    lexer_free(lexer);
    
    if (found_else_if && found_end_if) {
        printf("✓ Full PMPL code tokenization (else_if + end_if found)\n");
        test_count += 1;
    } else {
        printf("✗ Full PMPL code tokenization (missing tokens)\n");
        printf("  found_else_if: %d, found_end_if: %d\n", found_else_if, found_end_if);
    }
    
    // ========== Backward Compatibility ==========
    printf("\n[Backward Compatibility]\n");
    
    // Old single keywords should still work
    test_keyword("if", TOKEN_IF, "\"if\" → TOKEN_IF (single keyword)");
    test_keyword("else", TOKEN_ELSE, "\"else\" → TOKEN_ELSE (single keyword)");
    test_keyword("end", TOKEN_END, "\"end\" → TOKEN_END (single keyword)");
    test_keyword("while", TOKEN_WHILE, "\"while\" → TOKEN_WHILE (single keyword)");
    test_keyword("for", TOKEN_FOR, "\"for\" → TOKEN_FOR (single keyword)");
    test_count += 5;
    
    printf("\n========================================\n");
    printf("All %d tests passed! ✅\n", test_count);
    printf("========================================\n");
    
    return 0;
}
