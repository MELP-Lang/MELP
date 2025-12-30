/**
 * Test: mlp_state Module
 * Tests state management (get, set, clear, persist)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../MELP/runtime/mlp_state.h"

void test_state_basic() {
    printf("Test 1: Basic set/get operations\n");
    
    mlp_state_init();
    
    mlp_state_set("name", "Alice");
    mlp_state_set("age", "25");
    mlp_state_set("city", "Istanbul");
    
    char* name = mlp_state_get("name");
    char* age = mlp_state_get("age");
    char* city = mlp_state_get("city");
    
    assert(name != NULL && strcmp(name, "Alice") == 0);
    assert(age != NULL && strcmp(age, "25") == 0);
    assert(city != NULL && strcmp(city, "Istanbul") == 0);
    
    printf("  ✅ Set/Get working\n");
    
    mlp_state_close();
}

void test_state_overwrite() {
    printf("Test 2: Overwrite existing values\n");
    
    mlp_state_init();
    
    mlp_state_set("counter", "1");
    char* v1 = mlp_state_get("counter");
    assert(v1 != NULL && strcmp(v1, "1") == 0);
    
    mlp_state_set("counter", "2");
    char* v2 = mlp_state_get("counter");
    assert(v2 != NULL && strcmp(v2, "2") == 0);
    
    mlp_state_set("counter", "100");
    char* v3 = mlp_state_get("counter");
    assert(v3 != NULL && strcmp(v3, "100") == 0);
    
    printf("  ✅ Overwrite working\n");
    
    mlp_state_close();
}

void test_state_nonexistent() {
    printf("Test 3: Get nonexistent key\n");
    
    mlp_state_init();
    
    char* result = mlp_state_get("nonexistent_key");
    assert(result == NULL);
    
    printf("  ✅ Nonexistent key returns NULL\n");
    
    mlp_state_close();
}

void test_state_delete() {
    printf("Test 4: Delete specific key\n");
    
    mlp_state_init();
    
    mlp_state_set("temp", "value");
    assert(mlp_state_get("temp") != NULL);
    
    mlp_state_delete("temp");
    assert(mlp_state_get("temp") == NULL);
    
    printf("  ✅ Delete working\n");
    
    mlp_state_close();
}

void test_state_sso_vs_heap() {
    printf("Test 5: SSO vs Heap allocation\n");
    
    mlp_state_init();
    
    // Small value (SSO)
    mlp_state_set("short", "abc");
    
    // Large value (Heap)
    char large[100];
    memset(large, 'X', 99);
    large[99] = '\0';
    mlp_state_set("large", large);
    
    char* short_val = mlp_state_get("short");
    char* large_val = mlp_state_get("large");
    
    assert(short_val != NULL && strcmp(short_val, "abc") == 0);
    assert(large_val != NULL && strcmp(large_val, large) == 0);
    
    printf("  ✅ SSO and Heap allocation working\n");
    
    mlp_state_close();
}

void test_state_multiple_entries() {
    printf("Test 6: Multiple entries\n");
    
    mlp_state_init();
    
    for (int i = 0; i < 50; i++) {
        char key[32], value[32];
        sprintf(key, "key_%d", i);
        sprintf(value, "value_%d", i);
        mlp_state_set(key, value);
    }
    
    for (int i = 0; i < 50; i++) {
        char key[32], expected[32];
        sprintf(key, "key_%d", i);
        sprintf(expected, "value_%d", i);
        
        char* result = mlp_state_get(key);
        assert(result != NULL);
        assert(strcmp(result, expected) == 0);
    }
    
    printf("  ✅ Multiple entries working\n");
    
    mlp_state_close();
}

void test_state_persistence() {
    printf("Test 7: Persistence (file I/O)\n");
    
    const char* test_file = "/tmp/test_mlp_state.json";
    
    mlp_state_init();
    mlp_state_config_set("persist_file", test_file);
    mlp_state_set("persistent_key", "persistent_value");
    mlp_state_save();
    mlp_state_close();
    
    mlp_state_init();
    mlp_state_config_set("persist_file", test_file);
    mlp_state_load();
    char* value = mlp_state_get("persistent_key");
    assert(value != NULL);
    assert(strcmp(value, "persistent_value") == 0);
    
    printf("  ✅ Persistence working\n");
    
    mlp_state_close();
    remove(test_file);
}

int main(void) {
    printf("=== MLP State Module Tests ===\n\n");
    
    test_state_basic();
    test_state_overwrite();
    test_state_nonexistent();
    test_state_delete();
    test_state_sso_vs_heap();
    test_state_multiple_entries();
    test_state_persistence();
    
    printf("\n=== All Tests Passed ✅ ===\n");
    
    return 0;
}
