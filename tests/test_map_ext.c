/**
 * Test: Map Extensions (keys, values, clear)
 * 
 * YZ_212 Phase 1: Testing new map operations
 * Tests: melp_map_keys, melp_map_values, melp_map_clear
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "../runtime/stdlib/mlp_map.h"
#include "../runtime/stdlib/mlp_map_ext.h"
#include "../runtime/stdlib/mlp_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

void test_map_keys() {
    printf("🧪 Test 1: Map Keys\n");
    
    MelpMap* map = melp_map_create(sizeof(int64_t));
    assert(map != NULL);
    
    // Insert some key-value pairs
    int64_t age = 30;
    melp_map_insert(map, "age", &age);
    
    int64_t score = 95;
    melp_map_insert(map, "score", &score);
    
    int64_t count = 42;
    melp_map_insert(map, "count", &count);
    
    // Get keys
    MelpList* keys = melp_map_keys(map);
    assert(keys != NULL);
    assert(melp_list_length(keys) == 3);
    
    // Verify keys exist (order may vary due to hash table)
    int found_age = 0, found_score = 0, found_count = 0;
    for (size_t i = 0; i < melp_list_length(keys); i++) {
        char** key = (char**)melp_list_get(keys, i);
        if (strcmp(*key, "age") == 0) found_age = 1;
        if (strcmp(*key, "score") == 0) found_score = 1;
        if (strcmp(*key, "count") == 0) found_count = 1;
    }
    assert(found_age && found_score && found_count);
    
    melp_list_free(keys);
    melp_map_free(map);
    printf("✅ Test 1 passed!\n\n");
}

void test_map_values() {
    printf("🧪 Test 2: Map Values\n");
    
    MelpMap* map = melp_map_create(sizeof(int64_t));
    assert(map != NULL);
    
    // Insert some key-value pairs
    int64_t values_in[] = {100, 200, 300};
    melp_map_insert(map, "a", &values_in[0]);
    melp_map_insert(map, "b", &values_in[1]);
    melp_map_insert(map, "c", &values_in[2]);
    
    // Get values
    MelpList* values = melp_map_values(map);
    assert(values != NULL);
    assert(melp_list_length(values) == 3);
    
    // Verify values exist (order may vary)
    int found_100 = 0, found_200 = 0, found_300 = 0;
    for (size_t i = 0; i < melp_list_length(values); i++) {
        int64_t* val = (int64_t*)melp_list_get(values, i);
        if (*val == 100) found_100 = 1;
        if (*val == 200) found_200 = 1;
        if (*val == 300) found_300 = 1;
    }
    assert(found_100 && found_200 && found_300);
    
    melp_list_free(values);
    melp_map_free(map);
    printf("✅ Test 2 passed!\n\n");
}

void test_map_clear() {
    printf("🧪 Test 3: Map Clear\n");
    
    MelpMap* map = melp_map_create(sizeof(int64_t));
    assert(map != NULL);
    
    // Insert multiple entries
    for (int i = 0; i < 10; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        int64_t val = i * 10;
        melp_map_insert(map, key, &val);
    }
    
    assert(melp_map_length(map) == 10);
    
    // Clear map
    int result = melp_map_clear(map);
    assert(result == 0);
    assert(melp_map_length(map) == 0);
    
    // Verify no keys exist
    assert(melp_map_has_key(map, "key0") == 0);
    assert(melp_map_has_key(map, "key5") == 0);
    assert(melp_map_has_key(map, "key9") == 0);
    
    // Should be able to insert after clear
    int64_t new_val = 999;
    melp_map_insert(map, "new_key", &new_val);
    assert(melp_map_length(map) == 1);
    assert(melp_map_has_key(map, "new_key") == 1);
    
    melp_map_free(map);
    printf("✅ Test 3 passed!\n\n");
}

void test_map_keys_values_empty() {
    printf("🧪 Test 4: Keys/Values on Empty Map\n");
    
    MelpMap* map = melp_map_create(sizeof(int64_t));
    assert(map != NULL);
    
    // Get keys from empty map
    MelpList* keys = melp_map_keys(map);
    assert(keys != NULL);
    assert(melp_list_length(keys) == 0);
    melp_list_free(keys);
    
    // Get values from empty map
    MelpList* values = melp_map_values(map);
    assert(values != NULL);
    assert(melp_list_length(values) == 0);
    melp_list_free(values);
    
    melp_map_free(map);
    printf("✅ Test 4 passed!\n\n");
}

int main() {
    printf("🚀 YZ_212 Phase 1: Map Extensions Test\n");
    printf("=========================================\n\n");
    
    test_map_keys();
    test_map_values();
    test_map_clear();
    test_map_keys_values_empty();
    
    printf("=========================================\n");
    printf("✅ ALL MAP EXTENSION TESTS PASSED! 🎉\n");
    
    return 0;
}
