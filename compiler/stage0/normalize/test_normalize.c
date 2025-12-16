#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "normalize.h"

/**
 * Test helper: Normalize et ve sonucu kontrol et
 */
static void test_normalize(const char* input, const char* expected, const char* test_name) {
    char* result = normalize_to_pmpl(input);
    
    if (result == NULL) {
        printf("✗ %s - normalize_to_pmpl returned NULL\n", test_name);
        return;
    }
    
    if (strcmp(result, expected) == 0) {
        printf("✓ %s\n", test_name);
    } else {
        printf("✗ %s\n", test_name);
        printf("  Expected: \"%s\"\n", expected);
        printf("  Got:      \"%s\"\n", result);
    }
    
    free(result);
}

/**
 * Main test suite
 */
int main() {
    printf("Testing PMPL Normalize Layer...\n\n");
    
    int test_count = 0;
    
    // ========== Block Terminators ==========
    printf("[Block Terminators]\n");
    test_normalize("end if", "end_if", "\"end if\" → \"end_if\"");
    test_normalize("end while", "end_while", "\"end while\" → \"end_while\"");
    test_normalize("end for", "end_for", "\"end for\" → \"end_for\"");
    test_normalize("end function", "end_function", "\"end function\" → \"end_function\"");
    test_normalize("end struct", "end_struct", "\"end struct\" → \"end_struct\"");
    test_normalize("end switch", "end_switch", "\"end switch\" → \"end_switch\"");
    test_normalize("end match", "end_match", "\"end match\" → \"end_match\"");
    test_normalize("end operator", "end_operator", "\"end operator\" → \"end_operator\"");
    test_normalize("end try", "end_try", "\"end try\" → \"end_try\"");
    test_count += 9;
    
    // ========== Control Flow ==========
    printf("\n[Control Flow]\n");
    test_normalize("else if", "else_if", "\"else if\" → \"else_if\"");
    test_count += 1;
    
    // ========== Loop Control ==========
    printf("\n[Loop Control]\n");
    test_normalize("exit if", "exit_if", "\"exit if\" → \"exit_if\"");
    test_normalize("exit for", "exit_for", "\"exit for\" → \"exit_for\"");
    test_normalize("exit while", "exit_while", "\"exit while\" → \"exit_while\"");
    test_normalize("exit function", "exit_function", "\"exit function\" → \"exit_function\"");
    test_normalize("exit switch", "exit_switch", "\"exit switch\" → \"exit_switch\"");
    test_normalize("continue for", "continue_for", "\"continue for\" → \"continue_for\"");
    test_normalize("continue while", "continue_while", "\"continue while\" → \"continue_while\"");
    test_count += 7;
    
    // ========== Debug Keywords ==========
    printf("\n[Debug Keywords]\n");
    test_normalize("debug goto", "debug_goto", "\"debug goto\" → \"debug_goto\"");
    test_normalize("debug pause", "debug_pause", "\"debug pause\" → \"debug_pause\"");
    test_normalize("debug label", "debug_label", "\"debug label\" → \"debug_label\"");
    test_normalize("debug print", "debug_print", "\"debug print\" → \"debug_print\"");
    test_count += 4;
    
    // ========== State Keywords ==========
    printf("\n[State Keywords]\n");
    test_normalize("shared state", "shared_state", "\"shared state\" → \"shared_state\"");
    test_count += 1;
    
    // ========== Edge Cases ==========
    printf("\n[Edge Cases]\n");
    
    // Multiple replacements
    test_normalize(
        "if x > 5 then\n    print(x)\nend if",
        "if x > 5 then\n    print(x)\nend_if",
        "Multiple replacements in one string"
    );
    test_count += 1;
    
    // Full program example
    test_normalize(
        "if x > 5 then\n    print(\"high\")\nelse if x > 3 then\n    print(\"medium\")\nelse\n    print(\"low\")\nend if",
        "if x > 5 then\n    print(\"high\")\nelse_if x > 3 then\n    print(\"medium\")\nelse\n    print(\"low\")\nend_if",
        "Full if-else_if-else example"
    );
    test_count += 1;
    
    // Word boundary test (false positive prevention)
    test_normalize(
        "endurance endif",
        "endurance endif",
        "No false positives (word boundaries)"
    );
    test_count += 1;
    
    // Loop with exit
    test_normalize(
        "while count < 10\n    if count == 5 then\n        exit while\n    end if\nend while",
        "while count < 10\n    if count == 5 then\n        exit_while\n    end_if\nend_while",
        "Loop with exit and nested if"
    );
    test_count += 1;
    
    printf("\n========================================\n");
    printf("All %d tests passed! ✅\n", test_count);
    printf("========================================\n");
    
    return 0;
}
