/**
 * Test: List Extensions (insert, find, sort)
 * 
 * YZ_212 Phase 1: Testing new list operations
 * Tests: melp_list_insert, melp_list_find, melp_list_sort
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "../runtime/stdlib/mlp_list.h"
#include "../runtime/stdlib/mlp_list_ext.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void test_list_insert() {
    printf("🧪 Test 1: List Insert\n");
    
    MelpList* list = melp_list_create(sizeof(int64_t));
    assert(list != NULL);
    
    // Create list: (10, 20, 30, 40)
    int64_t val10 = 10, val20 = 20, val30 = 30, val40 = 40;
    melp_list_append(list, &val10);
    melp_list_append(list, &val20);
    melp_list_append(list, &val30);
    melp_list_append(list, &val40);
    
    assert(melp_list_length(list) == 4);
    
    // Insert 99 at index 2 → (10, 20, 99, 30, 40)
    int64_t val99 = 99;
    int result = melp_list_insert(list, 2, &val99);
    assert(result == 0);
    assert(melp_list_length(list) == 5);
    
    // Verify values
    int64_t* elem = (int64_t*)melp_list_get(list, 0);
    assert(*elem == 10);
    elem = (int64_t*)melp_list_get(list, 1);
    assert(*elem == 20);
    elem = (int64_t*)melp_list_get(list, 2);
    assert(*elem == 99);
    elem = (int64_t*)melp_list_get(list, 3);
    assert(*elem == 30);
    elem = (int64_t*)melp_list_get(list, 4);
    assert(*elem == 40);
    
    // Insert at beginning → (0, 10, 20, 99, 30, 40)
    int64_t val0 = 0;
    result = melp_list_insert(list, 0, &val0);
    assert(result == 0);
    assert(melp_list_length(list) == 6);
    elem = (int64_t*)melp_list_get(list, 0);
    assert(*elem == 0);
    
    // Insert at end → (0, 10, 20, 99, 30, 40, 100)
    int64_t val100 = 100;
    result = melp_list_insert(list, 6, &val100);
    assert(result == 0);
    assert(melp_list_length(list) == 7);
    elem = (int64_t*)melp_list_get(list, 6);
    assert(*elem == 100);
    
    melp_list_free(list);
    printf("✅ Test 1 passed!\n\n");
}

void test_list_find() {
    printf("🧪 Test 2: List Find\n");
    
    MelpList* list = melp_list_create(sizeof(int64_t));
    assert(list != NULL);
    
    // Create list: (5, 10, 15, 20, 25)
    int64_t values[] = {5, 10, 15, 20, 25};
    for (int i = 0; i < 5; i++) {
        melp_list_append(list, &values[i]);
    }
    
    // Find existing elements
    int64_t search = 10;
    int64_t index = melp_list_find(list, &search);
    assert(index == 1);
    
    search = 5;
    index = melp_list_find(list, &search);
    assert(index == 0);
    
    search = 25;
    index = melp_list_find(list, &search);
    assert(index == 4);
    
    // Find non-existing element
    search = 100;
    index = melp_list_find(list, &search);
    assert(index == -1);
    
    melp_list_free(list);
    printf("✅ Test 2 passed!\n\n");
}

void test_list_sort() {
    printf("🧪 Test 3: List Sort\n");
    
    MelpList* list = melp_list_create(sizeof(int64_t));
    assert(list != NULL);
    
    // Create unsorted list: (42, 7, 99, 13, 5, 88, 1)
    int64_t values[] = {42, 7, 99, 13, 5, 88, 1};
    for (int i = 0; i < 7; i++) {
        melp_list_append(list, &values[i]);
    }
    
    // Sort
    int result = melp_list_sort(list);
    assert(result == 0);
    
    // Verify sorted order: (1, 5, 7, 13, 42, 88, 99)
    int64_t expected[] = {1, 5, 7, 13, 42, 88, 99};
    for (int i = 0; i < 7; i++) {
        int64_t* elem = (int64_t*)melp_list_get(list, i);
        assert(*elem == expected[i]);
    }
    
    melp_list_free(list);
    printf("✅ Test 3 passed!\n\n");
}

void test_list_sort_empty() {
    printf("🧪 Test 4: List Sort (Edge Cases)\n");
    
    // Empty list
    MelpList* list = melp_list_create(sizeof(int64_t));
    assert(list != NULL);
    int result = melp_list_sort(list);
    assert(result == 0);
    assert(melp_list_length(list) == 0);
    melp_list_free(list);
    
    // Single element
    list = melp_list_create(sizeof(int64_t));
    int64_t val = 42;
    melp_list_append(list, &val);
    result = melp_list_sort(list);
    assert(result == 0);
    assert(melp_list_length(list) == 1);
    int64_t* elem = (int64_t*)melp_list_get(list, 0);
    assert(*elem == 42);
    melp_list_free(list);
    
    printf("✅ Test 4 passed!\n\n");
}

int main() {
    printf("🚀 YZ_212 Phase 1: List Extensions Test\n");
    printf("==========================================\n\n");
    
    test_list_insert();
    test_list_find();
    test_list_sort();
    test_list_sort_empty();
    
    printf("==========================================\n");
    printf("✅ ALL LIST EXTENSION TESTS PASSED! 🎉\n");
    
    return 0;
}
