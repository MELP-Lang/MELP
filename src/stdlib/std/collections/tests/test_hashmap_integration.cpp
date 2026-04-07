#include <iostream>
#include <string>
#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"

using namespace std;
using namespace melp_runtime;

int main() {
    cout << "=== HashMap PMPL Integration Test ===" << endl;
    cout << endl;
    
    // Test 1: Create HashMap
    cout << "Test 1: HashMap Creation" << endl;
    HashMap map = collections_hashmap_create();
    int size = collections_hashmap_size(map);
    cout << "Created map, size: " << size << endl;
    
    int is_empty = collections_hashmap_is_empty(map);
    if (is_empty == 1) {
        cout << "✅ Empty map check passed" << endl;
    } else {
        cout << "❌ Empty map check failed" << endl;
    }
    cout << endl;
    
    // Test 2: Insert operations
    cout << "Test 2: Insert Operations" << endl;
    collections_hashmap_insert(map, "age", 25.0);
    collections_hashmap_insert(map, "score", 98.5);
    collections_hashmap_insert(map, "temperature", 36.6);
    
    size = collections_hashmap_size(map);
    cout << "Inserted 3 entries, size: " << size << endl;
    
    if (size == 3) {
        cout << "✅ Size check passed" << endl;
    } else {
        cout << "❌ Size check failed" << endl;
    }
    cout << endl;
    
    // Test 3: Get operations
    cout << "Test 3: Get Operations" << endl;
    double age = collections_hashmap_get(map, "age");
    double score = collections_hashmap_get(map, "score");
    double temp = collections_hashmap_get(map, "temperature");
    
    cout << "age = " << age << endl;
    cout << "score = " << score << endl;
    cout << "temperature = " << temp << endl;
    
    if (age == 25.0) {
        cout << "✅ Get age passed" << endl;
    } else {
        cout << "❌ Get age failed" << endl;
    }
    
    if (score == 98.5) {
        cout << "✅ Get score passed" << endl;
    } else {
        cout << "❌ Get score failed" << endl;
    }
    cout << endl;
    
    // Test 4: Contains operations
    cout << "Test 4: Contains Operations" << endl;
    int has_age = collections_hashmap_contains(map, "age");
    int has_name = collections_hashmap_contains(map, "name");
    
    if (has_age == 1) {
        cout << "✅ Contains 'age' passed" << endl;
    } else {
        cout << "❌ Contains 'age' failed" << endl;
    }
    
    if (has_name == 0) {
        cout << "✅ Not contains 'name' passed" << endl;
    } else {
        cout << "❌ Not contains 'name' failed" << endl;
    }
    cout << endl;
    
    // Test 5: Update existing key
    cout << "Test 5: Update Operations" << endl;
    collections_hashmap_insert(map, "age", 26.0);
    age = collections_hashmap_get(map, "age");
    size = collections_hashmap_size(map);
    
    cout << "Updated age to 26, new value: " << age << endl;
    cout << "Size after update: " << size << endl;
    
    if (age == 26.0) {
        cout << "✅ Update value passed" << endl;
    } else {
        cout << "❌ Update value failed" << endl;
    }
    
    if (size == 3) {
        cout << "✅ Size unchanged after update" << endl;
    } else {
        cout << "❌ Size changed after update" << endl;
    }
    cout << endl;
    
    // Test 6: Remove operations
    cout << "Test 6: Remove Operations" << endl;
    collections_hashmap_remove(map, "temperature");
    size = collections_hashmap_size(map);
    int has_temp = collections_hashmap_contains(map, "temperature");
    
    cout << "Removed 'temperature', new size: " << size << endl;
    
    if (size == 2) {
        cout << "✅ Size after remove passed" << endl;
    } else {
        cout << "❌ Size after remove failed" << endl;
    }
    
    if (has_temp == 0) {
        cout << "✅ Key removed successfully" << endl;
    } else {
        cout << "❌ Key still exists" << endl;
    }
    cout << endl;
    
    // Test 7: Keys and Values
    cout << "Test 7: Keys and Values Extraction" << endl;
    collections_hashmap_insert(map, "height", 175.5);
    
    int keys_len = 0, vals_len = 0;
    char** keys = collections_hashmap_keys(map, &keys_len);
    double* values = collections_hashmap_values(map, &vals_len);
    size = collections_hashmap_size(map);
    
    cout << "Keys count: " << keys_len << endl;
    cout << "Keys: ";
    for (int i = 0; i < keys_len; i++) {
        cout << keys[i];
        if (i < keys_len - 1) cout << ", ";
    }
    cout << endl;
    
    cout << "Values: ";
    for (int i = 0; i < vals_len; i++) {
        cout << values[i];
        if (i < vals_len - 1) cout << ", ";
    }
    cout << endl;
    
    if (keys_len == 3 && vals_len == 3) {
        cout << "✅ Keys/Values count passed" << endl;
    } else {
        cout << "❌ Keys/Values count failed" << endl;
    }
    
    // Cleanup keys array
    for (int i = 0; i < keys_len; i++) {
        delete[] keys[i];
    }
    delete[] keys;
    delete[] values;
    cout << endl;
    
    // Test 8: Clear operation
    cout << "Test 8: Clear Operations" << endl;
    collections_hashmap_clear(map);
    size = collections_hashmap_size(map);
    is_empty = collections_hashmap_is_empty(map);
    
    cout << "After clear, size: " << size << endl;
    
    if (size == 0) {
        cout << "✅ Size after clear passed" << endl;
    } else {
        cout << "❌ Size after clear failed" << endl;
    }
    
    if (is_empty == 1) {
        cout << "✅ Empty after clear passed" << endl;
    } else {
        cout << "❌ Empty after clear failed" << endl;
    }
    cout << endl;
    
    // Test 9: Get from empty map (should return 0.0)
    cout << "Test 9: Get from Empty Map" << endl;
    double missing = collections_hashmap_get(map, "nonexistent");
    
    cout << "Get missing key: " << missing << endl;
    
    if (missing == 0.0) {
        cout << "✅ Missing key returns 0.0" << endl;
    } else {
        cout << "❌ Missing key doesn't return 0.0" << endl;
    }
    cout << endl;
    
    cout << "=== All HashMap Integration Tests Complete ===" << endl;
    
    return 0;
}
