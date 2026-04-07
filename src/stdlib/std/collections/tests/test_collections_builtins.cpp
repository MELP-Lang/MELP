// ============================================================================
// MLP Runtime - Collections Module Builtins Test Suite
// ============================================================================
// Comprehensive tests for array utility functions
// Author: YZ_STAGE1_01
// Date: 28 Ocak 2026
// ============================================================================

#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace melp_runtime;

// Test 1: Array Length and Empty
void test_length_and_empty() {
    std::cout << "Test 1: Array Length and Empty..." << std::endl;
    
    double arr1[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    assert(collections_array_length(arr1, 5) == 5);
    assert(!collections_array_is_empty(arr1, 5));
    
    double arr2[] = {};
    assert(collections_array_length(arr2, 0) == 0);
    assert(collections_array_is_empty(arr2, 0));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 2: Array Get and Set
void test_get_and_set() {
    std::cout << "Test 2: Array Get and Set..." << std::endl;
    
    double arr[] = {10.0, 20.0, 30.0, 40.0, 50.0};
    
    assert(collections_array_get(arr, 5, 0) == 10.0);
    assert(collections_array_get(arr, 5, 2) == 30.0);
    assert(collections_array_get(arr, 5, 4) == 50.0);
    
    collections_array_set(arr, 5, 2, 99.0);
    assert(collections_array_get(arr, 5, 2) == 99.0);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 3: Array Clone and Slice
void test_clone_and_slice() {
    std::cout << "Test 3: Array Clone and Slice..." << std::endl;
    
    double arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    int out_len;
    
    // Clone
    double* cloned = collections_array_clone(arr, 5, &out_len);
    assert(out_len == 5);
    for (int i = 0; i < 5; i++) {
        assert(cloned[i] == arr[i]);
    }
    delete[] cloned;
    
    // Slice [1, 4)
    double* slice = collections_array_slice(arr, 5, 1, 4, &out_len);
    assert(out_len == 3);
    assert(slice[0] == 2.0);
    assert(slice[1] == 3.0);
    assert(slice[2] == 4.0);
    delete[] slice;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 4: Array Reverse and Concat
void test_reverse_and_concat() {
    std::cout << "Test 4: Array Reverse and Concat..." << std::endl;
    
    double arr1[] = {1.0, 2.0, 3.0};
    double arr2[] = {4.0, 5.0, 6.0};
    int out_len;
    
    // Reverse
    double* reversed = collections_array_reverse(arr1, 3, &out_len);
    assert(out_len == 3);
    assert(reversed[0] == 3.0);
    assert(reversed[1] == 2.0);
    assert(reversed[2] == 1.0);
    delete[] reversed;
    
    // Concat
    double* concat = collections_array_concat(arr1, 3, arr2, 3, &out_len);
    assert(out_len == 6);
    assert(concat[0] == 1.0);
    assert(concat[2] == 3.0);
    assert(concat[3] == 4.0);
    assert(concat[5] == 6.0);
    delete[] concat;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 5: Array Push and Pop
void test_push_and_pop() {
    std::cout << "Test 5: Array Push and Pop..." << std::endl;
    
    double arr[] = {1.0, 2.0, 3.0};
    int out_len;
    
    // Push
    double* pushed = collections_array_push(arr, 3, 4.0, &out_len);
    assert(out_len == 4);
    assert(pushed[0] == 1.0);
    assert(pushed[3] == 4.0);
    
    // Pop
    int arr_len = 4;
    double popped = collections_array_pop(pushed, &arr_len);
    assert(popped == 4.0);
    assert(arr_len == 3);
    
    delete[] pushed;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 6: Array Search (index_of, contains, last_index_of)
void test_search() {
    std::cout << "Test 6: Array Search..." << std::endl;
    
    double arr[] = {10.0, 20.0, 30.0, 20.0, 40.0};
    
    // index_of
    assert(collections_array_index_of(arr, 5, 20.0) == 1);
    assert(collections_array_index_of(arr, 5, 30.0) == 2);
    assert(collections_array_index_of(arr, 5, 99.0) == -1);
    
    // contains
    assert(collections_array_contains(arr, 5, 20.0));
    assert(collections_array_contains(arr, 5, 40.0));
    assert(!collections_array_contains(arr, 5, 99.0));
    
    // last_index_of
    assert(collections_array_last_index_of(arr, 5, 20.0) == 3);
    assert(collections_array_last_index_of(arr, 5, 10.0) == 0);
    assert(collections_array_last_index_of(arr, 5, 99.0) == -1);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 7: Array Sorting
void test_sorting() {
    std::cout << "Test 7: Array Sorting..." << std::endl;
    
    double arr[] = {5.0, 2.0, 8.0, 1.0, 9.0, 3.0};
    int out_len;
    
    // Sort ascending
    double* asc = collections_array_sort_ascending(arr, 6, &out_len);
    assert(out_len == 6);
    assert(asc[0] == 1.0);
    assert(asc[1] == 2.0);
    assert(asc[2] == 3.0);
    assert(asc[5] == 9.0);
    delete[] asc;
    
    // Sort descending
    double* desc = collections_array_sort_descending(arr, 6, &out_len);
    assert(out_len == 6);
    assert(desc[0] == 9.0);
    assert(desc[1] == 8.0);
    assert(desc[2] == 5.0);
    assert(desc[5] == 1.0);
    delete[] desc;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 8: Array Statistics
void test_statistics() {
    std::cout << "Test 8: Array Statistics..." << std::endl;
    
    double arr[] = {10.0, 20.0, 30.0, 40.0, 50.0};
    
    // Sum
    assert(collections_array_sum(arr, 5) == 150.0);
    
    // Min
    assert(collections_array_min(arr, 5) == 10.0);
    
    // Max
    assert(collections_array_max(arr, 5) == 50.0);
    
    // Average
    assert(collections_array_average(arr, 5) == 30.0);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

// Test 9: Array Count and Fill
void test_count_and_fill() {
    std::cout << "Test 9: Array Count and Fill..." << std::endl;
    
    double arr[] = {1.0, 2.0, 3.0, 2.0, 4.0, 2.0};
    
    // Count
    assert(collections_array_count(arr, 6, 2.0) == 3);
    assert(collections_array_count(arr, 6, 1.0) == 1);
    assert(collections_array_count(arr, 6, 99.0) == 0);
    
    // Fill
    int out_len;
    double* filled = collections_array_fill(arr, 6, 7.0, &out_len);
    assert(out_len == 6);
    for (int i = 0; i < 6; i++) {
        assert(filled[i] == 7.0);
    }
    delete[] filled;
    
    std::cout << "  ✅ PASSED" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "Collections Module Builtins Test Suite" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    test_length_and_empty();
    test_get_and_set();
    test_clone_and_slice();
    test_reverse_and_concat();
    test_push_and_pop();
    test_search();
    test_sorting();
    test_statistics();
    test_count_and_fill();
    
    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "✅ ALL TESTS PASSED! (9/9)" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    return 0;
}
