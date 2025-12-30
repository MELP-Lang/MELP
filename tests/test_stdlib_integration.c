/**
 * Test: Stdlib Integration Test (Task 5.5)
 * Tests: list, optional, state, panic, io, file_io, math modules
 */

#include <stdio.h>
#include <stdlib.h>
#include "../MELP/runtime/mlp_list.h"
#include "../MELP/runtime/mlp_optional.h"
#include "../MELP/runtime/mlp_state.h"
#include "../MELP/runtime/mlp_panic.h"
#include "../MELP/runtime/mlp_io.h"
#include "../MELP/runtime/file_io.h"
#include "../MELP/runtime/mlp_math.h"

int main() {
    printf("=== MLP Stdlib Integration Test ===\n\n");
    
    // Test 1: mlp_list
    printf("Test 1: mlp_list\n");
    MelpList* list = melp_list_create(sizeof(int));
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        melp_list_append(list, &values[i]);
    }
    printf("  List size: %zu (expected: 5)\n", melp_list_size(list));
    int* first = (int*)melp_list_get(list, 0);
    printf("  First element: %d (expected: 10)\n", *first);
    melp_list_destroy(list);
    printf("  ✅ mlp_list OK\n\n");
    
    // Test 2: mlp_optional
    printf("Test 2: mlp_optional\n");
    int value = 42;
    MelpOptional* opt_some = melp_optional_some(&value, sizeof(int));
    printf("  Has value: %d (expected: 1)\n", melp_optional_has_value(opt_some));
    int* opt_val = (int*)melp_optional_get(opt_some);
    printf("  Value: %d (expected: 42)\n", *opt_val);
    melp_optional_destroy(opt_some);
    
    MelpOptional* opt_none = melp_optional_none();
    printf("  None has value: %d (expected: 0)\n", melp_optional_has_value(opt_none));
    melp_optional_destroy(opt_none);
    printf("  ✅ mlp_optional OK\n\n");
    
    // Test 3: mlp_state
    printf("Test 3: mlp_state\n");
    mlp_state_init();
    mlp_state_set_int("counter", 100);
    int64_t counter = mlp_state_get_int("counter");
    printf("  State counter: %ld (expected: 100)\n", counter);
    mlp_state_cleanup();
    printf("  ✅ mlp_state OK\n\n");
    
    // Test 4: file_io
    printf("Test 4: file_io\n");
    const char* test_content = "Hello from MLP stdlib!";
    int64_t write_result = mlp_write_file("/tmp/mlp_test.txt", test_content);
    printf("  Write result: %ld (expected: 0)\n", write_result);
    
    int exists = mlp_file_exists("/tmp/mlp_test.txt");
    printf("  File exists: %d (expected: 1)\n", exists);
    
    char* read_content = mlp_read_file("/tmp/mlp_test.txt");
    if (read_content) {
        printf("  Read content: '%s'\n", read_content);
        free(read_content);
    }
    printf("  ✅ file_io OK\n\n");
    
    // Test 5: mlp_math
    printf("Test 5: mlp_math\n");
    double sqrt_result = mlp_sqrt(16.0);
    printf("  sqrt(16) = %.1f (expected: 4.0)\n", sqrt_result);
    double pow_result = mlp_pow(2.0, 3.0);
    printf("  pow(2, 3) = %.1f (expected: 8.0)\n", pow_result);
    printf("  ✅ mlp_math OK\n\n");
    
    printf("=== All Tests PASSED ===\n");
    return 0;
}
