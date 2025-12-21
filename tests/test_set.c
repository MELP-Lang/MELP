/**
 * Test: Set Operations
 * 
 * YZ_212 Phase 2: Testing Set collection
 * Tests: add, contains, remove, union, intersection, difference
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "../runtime/stdlib/mlp_set.h"
#include "../runtime/stdlib/mlp_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void test_set_create_and_add() {
    printf("🧪 Test 1: Set Create and Add\n");
    
    MelpSet* set = melp_set_create(sizeof(int64_t));
    assert(set != NULL);
    assert(melp_set_is_empty(set) == 1);
    
    // Add unique elements
    int64_t val1 = 10, val2 = 20, val3 = 30;
    int result = melp_set_add(set, &val1);
    assert(result == 1);  // New element added
    assert(melp_set_size(set) == 1);
    
    result = melp_set_add(set, &val2);
    assert(result == 1);
    assert(melp_set_size(set) == 2);
    
    result = melp_set_add(set, &val3);
    assert(result == 1);
    assert(melp_set_size(set) == 3);
    
    // Try adding duplicate
    result = melp_set_add(set, &val1);
    assert(result == 0);  // Already exists
    assert(melp_set_size(set) == 3);  // Size unchanged
    
    melp_set_free(set);
    printf("✅ Test 1 passed!\n\n");
}

void test_set_contains() {
    printf("🧪 Test 2: Set Contains\n");
    
    MelpSet* set = melp_set_create(sizeof(int64_t));
    assert(set != NULL);
    
    // Add elements: {1, 2, 3, 4, 5}
    int64_t values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        melp_set_add(set, &values[i]);
    }
    
    // Check existing elements
    int64_t search = 3;
    assert(melp_set_contains(set, &search) == 1);
    
    search = 1;
    assert(melp_set_contains(set, &search) == 1);
    
    search = 5;
    assert(melp_set_contains(set, &search) == 1);
    
    // Check non-existing element
    search = 100;
    assert(melp_set_contains(set, &search) == 0);
    
    melp_set_free(set);
    printf("✅ Test 2 passed!\n\n");
}

void test_set_remove() {
    printf("🧪 Test 3: Set Remove\n");
    
    MelpSet* set = melp_set_create(sizeof(int64_t));
    assert(set != NULL);
    
    // Add elements: {10, 20, 30, 40}
    int64_t values[] = {10, 20, 30, 40};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set, &values[i]);
    }
    assert(melp_set_size(set) == 4);
    
    // Remove existing element
    int64_t val = 20;
    int result = melp_set_remove(set, &val);
    assert(result == 1);
    assert(melp_set_size(set) == 3);
    assert(melp_set_contains(set, &val) == 0);
    
    // Try removing non-existing element
    val = 999;
    result = melp_set_remove(set, &val);
    assert(result == 0);
    assert(melp_set_size(set) == 3);
    
    melp_set_free(set);
    printf("✅ Test 3 passed!\n\n");
}

void test_set_union() {
    printf("🧪 Test 4: Set Union\n");
    
    MelpSet* set1 = melp_set_create(sizeof(int64_t));
    MelpSet* set2 = melp_set_create(sizeof(int64_t));
    
    // set1 = {1, 2, 3, 4}
    int64_t vals1[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set1, &vals1[i]);
    }
    
    // set2 = {3, 4, 5, 6}
    int64_t vals2[] = {3, 4, 5, 6};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set2, &vals2[i]);
    }
    
    // Union = {1, 2, 3, 4, 5, 6}
    MelpSet* result = melp_set_union(set1, set2);
    assert(result != NULL);
    assert(melp_set_size(result) == 6);
    
    // Verify all elements present
    for (int64_t i = 1; i <= 6; i++) {
        assert(melp_set_contains(result, &i) == 1);
    }
    
    melp_set_free(set1);
    melp_set_free(set2);
    melp_set_free(result);
    printf("✅ Test 4 passed!\n\n");
}

void test_set_intersection() {
    printf("🧪 Test 5: Set Intersection\n");
    
    MelpSet* set1 = melp_set_create(sizeof(int64_t));
    MelpSet* set2 = melp_set_create(sizeof(int64_t));
    
    // set1 = {1, 2, 3, 4}
    int64_t vals1[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set1, &vals1[i]);
    }
    
    // set2 = {3, 4, 5, 6}
    int64_t vals2[] = {3, 4, 5, 6};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set2, &vals2[i]);
    }
    
    // Intersection = {3, 4}
    MelpSet* result = melp_set_intersection(set1, set2);
    assert(result != NULL);
    assert(melp_set_size(result) == 2);
    
    int64_t val3 = 3, val4 = 4;
    assert(melp_set_contains(result, &val3) == 1);
    assert(melp_set_contains(result, &val4) == 1);
    
    int64_t val1 = 1, val6 = 6;
    assert(melp_set_contains(result, &val1) == 0);
    assert(melp_set_contains(result, &val6) == 0);
    
    melp_set_free(set1);
    melp_set_free(set2);
    melp_set_free(result);
    printf("✅ Test 5 passed!\n\n");
}

void test_set_difference() {
    printf("🧪 Test 6: Set Difference\n");
    
    MelpSet* set1 = melp_set_create(sizeof(int64_t));
    MelpSet* set2 = melp_set_create(sizeof(int64_t));
    
    // set1 = {1, 2, 3, 4}
    int64_t vals1[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set1, &vals1[i]);
    }
    
    // set2 = {3, 4, 5, 6}
    int64_t vals2[] = {3, 4, 5, 6};
    for (int i = 0; i < 4; i++) {
        melp_set_add(set2, &vals2[i]);
    }
    
    // Difference (set1 - set2) = {1, 2}
    MelpSet* result = melp_set_difference(set1, set2);
    assert(result != NULL);
    assert(melp_set_size(result) == 2);
    
    int64_t val1 = 1, val2 = 2;
    assert(melp_set_contains(result, &val1) == 1);
    assert(melp_set_contains(result, &val2) == 1);
    
    int64_t val3 = 3, val4 = 4;
    assert(melp_set_contains(result, &val3) == 0);
    assert(melp_set_contains(result, &val4) == 0);
    
    melp_set_free(set1);
    melp_set_free(set2);
    melp_set_free(result);
    printf("✅ Test 6 passed!\n\n");
}

void test_set_from_list() {
    printf("🧪 Test 7: Set from List (Deduplication)\n");
    
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    // List with duplicates: (1, 2, 2, 3, 3, 3, 4)
    int64_t values[] = {1, 2, 2, 3, 3, 3, 4};
    for (int i = 0; i < 7; i++) {
        melp_list_append(list, &values[i]);
    }
    
    // Convert to set → {1, 2, 3, 4}
    MelpSet* set = melp_set_from_list(list);
    assert(set != NULL);
    assert(melp_set_size(set) == 4);
    
    for (int64_t i = 1; i <= 4; i++) {
        assert(melp_set_contains(set, &i) == 1);
    }
    
    melp_list_free(list);
    melp_set_free(set);
    printf("✅ Test 7 passed!\n\n");
}

int main() {
    printf("🚀 YZ_212 Phase 2: Set Operations Test\n");
    printf("==========================================\n\n");
    
    test_set_create_and_add();
    test_set_contains();
    test_set_remove();
    test_set_union();
    test_set_intersection();
    test_set_difference();
    test_set_from_list();
    
    printf("==========================================\n");
    printf("✅ ALL SET TESTS PASSED! 🎉\n");
    
    return 0;
}
