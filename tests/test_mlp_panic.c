/**
 * Test: mlp_panic Module
 * Tests runtime error handling and panic functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../MELP/runtime/mlp_panic.h"

// Test: mlp_runtime_error (will exit with code 43)
void test_runtime_error() {
    printf("Testing mlp_runtime_error (should exit with 43)...\n");
    mlp_runtime_error("Test error message");
    // Should not reach here
    printf("ERROR: mlp_runtime_error did not exit!\n");
}

// Test: mlp_panic_division_by_zero (will exit with 43)
void test_division_by_zero() {
    printf("Testing mlp_panic_division_by_zero (should exit with 43)...\n");
    mlp_panic_division_by_zero();
    // Should not reach here
    printf("ERROR: mlp_panic_division_by_zero did not exit!\n");
}

// Test: mlp_panic_array_bounds (will exit with 42)
void test_array_bounds() {
    printf("Testing mlp_panic_array_bounds (should exit with 42)...\n");
    mlp_panic_array_bounds(10, 5, "test_array");
    // Should not reach here
    printf("ERROR: mlp_panic_array_bounds did not exit!\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [runtime_error|division|bounds]\n", argv[0]);
        return 1;
    }
    
    const char* test = argv[1];
    
    if (strcmp(test, "runtime_error") == 0) {
        test_runtime_error();
    } else if (strcmp(test, "division") == 0) {
        test_division_by_zero();
    } else if (strcmp(test, "bounds") == 0) {
        test_array_bounds();
    } else {
        printf("Unknown test: %s\n", test);
        return 1;
    }
    
    return 0;
}
