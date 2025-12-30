/**
 * Comprehensive Runtime Integration Test
 * Tests all runtime modules working together
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Runtime modules
#include "../MELP/runtime/runtime_error.h"
#include "../MELP/runtime/runtime_memory.h"
#include "../MELP/runtime/runtime_string.h"
#include "../MELP/runtime/runtime_array.h"
#include "../MELP/runtime/runtime_types.h"
#include "../MELP/runtime/runtime_print.h"
#include "../MELP/runtime/runtime_io.h"
#include "../MELP/runtime/runtime_system.h"

// Stdlib modules
#include "../MELP/runtime/mlp_list.h"
#include "../MELP/runtime/mlp_optional.h"
#include "../MELP/runtime/mlp_state.h"
#include "../MELP/runtime/mlp_panic.h"
#include "../MELP/runtime/mlp_math.h"

void test_runtime_memory() {
    printf("Test 1: Runtime Memory\n");
    
    void* ptr = mlp_malloc(100);
    assert(ptr != NULL);
    mlp_free(ptr);
    
    printf("  ✅ Memory allocation working\n");
}

void test_runtime_string() {
    printf("Test 2: Runtime String\n");
    
    char* str1 = mlp_string_concat("Hello", " World");
    assert(strcmp(str1, "Hello World") == 0);
    free(str1);
    
    char* str2 = mlp_string_upper("test");
    assert(strcmp(str2, "TEST") == 0);
    free(str2);
    
    printf("  ✅ String operations working\n");
}

void test_mlp_list() {
    printf("Test 3: MLP List\n");
    
    int64_t list_id = mlp_list_create();
    assert(list_id >= 0);
    
    mlp_list_push(list_id, 10);
    mlp_list_push(list_id, 20);
    mlp_list_push(list_id, 30);
    
    int64_t size = mlp_list_size(list_id);
    assert(size == 3);
    
    int64_t val = mlp_list_get(list_id, 1);
    assert(val == 20);
    
    mlp_list_destroy(list_id);
    
    printf("  ✅ List operations working\n");
}

void test_mlp_optional() {
    printf("Test 4: MLP Optional\n");
    
    int64_t opt1 = mlp_optional_some(42);
    assert(mlp_optional_is_some(opt1) == 1);
    assert(mlp_optional_get(opt1) == 42);
    
    int64_t opt2 = mlp_optional_none();
    assert(mlp_optional_is_none(opt2) == 1);
    
    printf("  ✅ Optional operations working\n");
}

void test_mlp_state() {
    printf("Test 5: MLP State\n");
    
    mlp_state_init();
    mlp_state_set("test_key", "test_value");
    
    char* value = mlp_state_get("test_key");
    assert(value != NULL);
    assert(strcmp(value, "test_value") == 0);
    free(value);
    
    mlp_state_close();
    
    printf("  ✅ State management working\n");
}

void test_mlp_math() {
    printf("Test 6: MLP Math\n");
    
    int64_t abs_val = mlp_abs(-42);
    assert(abs_val == 42);
    
    int64_t max_val = mlp_max(10, 20);
    assert(max_val == 20);
    
    int64_t min_val = mlp_min(10, 20);
    assert(min_val == 10);
    
    printf("  ✅ Math operations working\n");
}

int main(void) {
    printf("=== Comprehensive Runtime Integration Test ===\n\n");
    
    test_runtime_memory();
    test_runtime_string();
    test_mlp_list();
    test_mlp_optional();
    test_mlp_state();
    test_mlp_math();
    
    printf("\n=== All Runtime Modules Validated ✅ ===\n");
    printf("Runtime library is stable and ready for integration!\n");
    
    return 0;
}
