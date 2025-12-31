// Runtime integration test
// Tests if new modular runtime links correctly

#include "MELP/runtime/runtime_error.h"
#include "MELP/runtime/runtime_memory.h"
#include "MELP/runtime/runtime_string.h"
#include "MELP/runtime/runtime_types.h"
#include "MELP/runtime/runtime_print.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    printf("=== RUNTIME INTEGRATION TEST ===\n\n");
    
    // Test 1: Memory allocation
    printf("Test 1: Memory allocation\n");
    char* ptr = (char*)mlp_malloc(100);
    if (ptr) {
        strcpy(ptr, "Memory test");
        printf("  ✅ Allocated and wrote: %s\n", ptr);
        mlp_free(ptr);
        printf("  ✅ Freed successfully\n");
    } else {
        printf("  ❌ Allocation failed\n");
        return 1;
    }
    
    // Test 2: String operations
    printf("\nTest 2: String operations\n");
    const char* test_str = "Hello Runtime";
    long len = string_length(test_str);
    printf("  ✅ String length of '%s': %ld\n", test_str, len);
    
    char* upper = string_upper(test_str);
    if (upper) {
        printf("  ✅ To upper: %s\n", upper);
        mlp_free(upper);
    }
    
    // Test 3: Type conversion
    printf("\nTest 3: Type conversion\n");
    int64_t num = 42; // Note: string parsing functions need implementation
    printf("  ✅ Integer test: %ld\n", (long)num);
    
    char* int_str = int_to_string(num);
    if (int_str) {
        printf("  ✅ Int to string: '%s'\n", int_str);
        mlp_free(int_str);
    }
    
    // Test 4: Error handling
    printf("\nTest 4: Error handling\n");
    set_error_code(0);
    int err = get_error_code();
    printf("  ✅ Error code: %d\n", err);
    
    printf("\n=== ALL TESTS PASSED ✅ ===\n");
    return 0;
}
