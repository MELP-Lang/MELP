// ============================================================================
// MLP Runtime - Collections HashMap Test Suite
// ============================================================================
// Tests for HashMap (string->numeric map) operations
// Author: YZ_STAGE1_01
// Date: 28 Ocak 2026
// ============================================================================

#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace melp_runtime;

// Test 1: HashMap Creation and Basic Properties
void test_creation_and_properties() {
    std::cout << "Test 1: HashMap Creation and Basic Properties..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    assert(map != nullptr);
    assert(collections_hashmap_size(map) == 0);
    assert(collections_hashmap_is_empty(map));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 2: HashMap Insert and Get
void test_insert_and_get() {
    std::cout << "Test 2: HashMap Insert and Get..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    
    // Insert values
    collections_hashmap_insert(map, "age", 25.0);
    collections_hashmap_insert(map, "score", 95.5);
    collections_hashmap_insert(map, "count", 42.0);
    
    // Check size
    assert(collections_hashmap_size(map) == 3);
    assert(!collections_hashmap_is_empty(map));
    
    // Get values
    assert(collections_hashmap_get(map, "age") == 25.0);
    assert(collections_hashmap_get(map, "score") == 95.5);
    assert(collections_hashmap_get(map, "count") == 42.0);
    
    // Get non-existent key (should return 0.0)
    assert(collections_hashmap_get(map, "missing") == 0.0);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 3: HashMap Contains
void test_contains() {
    std::cout << "Test 3: HashMap Contains..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "name", 123.0);
    collections_hashmap_insert(map, "value", 456.0);
    
    assert(collections_hashmap_contains(map, "name"));
    assert(collections_hashmap_contains(map, "value"));
    assert(!collections_hashmap_contains(map, "missing"));
    assert(!collections_hashmap_contains(map, "other"));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 4: HashMap Update (Insert Existing Key)
void test_update() {
    std::cout << "Test 4: HashMap Update..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "counter", 10.0);
    
    assert(collections_hashmap_get(map, "counter") == 10.0);
    
    // Update same key
    collections_hashmap_insert(map, "counter", 20.0);
    assert(collections_hashmap_get(map, "counter") == 20.0);
    assert(collections_hashmap_size(map) == 1);  // Size should remain 1
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 5: HashMap Remove
void test_remove() {
    std::cout << "Test 5: HashMap Remove..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "a", 1.0);
    collections_hashmap_insert(map, "b", 2.0);
    collections_hashmap_insert(map, "c", 3.0);
    
    assert(collections_hashmap_size(map) == 3);
    
    // Remove key
    collections_hashmap_remove(map, "b");
    assert(collections_hashmap_size(map) == 2);
    assert(!collections_hashmap_contains(map, "b"));
    assert(collections_hashmap_contains(map, "a"));
    assert(collections_hashmap_contains(map, "c"));
    
    // Remove non-existent key (should not error)
    collections_hashmap_remove(map, "missing");
    assert(collections_hashmap_size(map) == 2);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 6: HashMap Clear
void test_clear() {
    std::cout << "Test 6: HashMap Clear..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "x", 10.0);
    collections_hashmap_insert(map, "y", 20.0);
    collections_hashmap_insert(map, "z", 30.0);
    
    assert(collections_hashmap_size(map) == 3);
    
    collections_hashmap_clear(map);
    assert(collections_hashmap_size(map) == 0);
    assert(collections_hashmap_is_empty(map));
    assert(!collections_hashmap_contains(map, "x"));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 7: HashMap Keys
void test_keys() {
    std::cout << "Test 7: HashMap Keys..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "apple", 1.0);
    collections_hashmap_insert(map, "banana", 2.0);
    collections_hashmap_insert(map, "cherry", 3.0);
    
    int keys_len;
    char** keys = collections_hashmap_keys(map, &keys_len);
    
    assert(keys_len == 3);
    assert(keys != nullptr);
    
    // Check that all keys are present (order may vary)
    bool has_apple = false, has_banana = false, has_cherry = false;
    for (int i = 0; i < keys_len; i++) {
        if (std::strcmp(keys[i], "apple") == 0) has_apple = true;
        if (std::strcmp(keys[i], "banana") == 0) has_banana = true;
        if (std::strcmp(keys[i], "cherry") == 0) has_cherry = true;
        delete[] keys[i];
    }
    delete[] keys;
    
    assert(has_apple && has_banana && has_cherry);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 8: HashMap Values
void test_values() {
    std::cout << "Test 8: HashMap Values..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    collections_hashmap_insert(map, "one", 1.0);
    collections_hashmap_insert(map, "two", 2.0);
    collections_hashmap_insert(map, "three", 3.0);
    
    int values_len;
    double* values = collections_hashmap_values(map, &values_len);
    
    assert(values_len == 3);
    assert(values != nullptr);
    
    // Sum of values should be 6.0
    double sum = 0.0;
    for (int i = 0; i < values_len; i++) {
        sum += values[i];
    }
    assert(sum == 6.0);
    
    delete[] values;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 9: HashMap Empty Keys/Values
void test_empty_keys_values() {
    std::cout << "Test 9: HashMap Empty Keys/Values..." << std::endl;
    
    HashMap map = collections_hashmap_create();
    
    int keys_len, values_len;
    char** keys = collections_hashmap_keys(map, &keys_len);
    double* values = collections_hashmap_values(map, &values_len);
    
    assert(keys_len == 0);
    assert(values_len == 0);
    assert(keys == nullptr);
    assert(values == nullptr);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "Collections HashMap Test Suite" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    test_creation_and_properties();
    test_insert_and_get();
    test_contains();
    test_update();
    test_remove();
    test_clear();
    test_keys();
    test_values();
    test_empty_keys_values();
    
    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "✅ ALL TESTS PASSED! (9/9)" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    return 0;
}
