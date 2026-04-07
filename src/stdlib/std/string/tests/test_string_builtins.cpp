#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>

using namespace melp_runtime;

void test_string_length() {
    std::cout << "Test 1: string_length..." << std::endl;
    
    assert(string_length("") == 0);
    assert(string_length("a") == 1);
    assert(string_length("hello") == 5);
    assert(string_length("hello world") == 11);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_concat() {
    std::cout << "Test 2: string_concat..." << std::endl;
    
    assert(string_concat("hello", " world") == "hello world");
    assert(string_concat("", "test") == "test");
    assert(string_concat("test", "") == "test");
    assert(string_concat("a", "b") == "ab");
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_substring() {
    std::cout << "Test 3: string_substring..." << std::endl;
    
    assert(string_substring("hello world", 0, 5) == "hello");
    assert(string_substring("hello world", 6, 11) == "world");
    assert(string_substring("test", 0, 4) == "test");
    assert(string_substring("hello", 1, 2) == "e");
    assert(string_substring("hello", 0, 0) == "");
    
    // Edge cases
    assert(string_substring("test", -1, 2) == "");  // Invalid start
    assert(string_substring("test", 0, 10) == "");  // Out of bounds end
    assert(string_substring("test", 2, 1) == "");   // start > end
    
    std::cout << "  ✅ PASSED" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "String Module Builtins Test" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    try {
        test_string_length();
        test_string_concat();
        test_string_substring();
        
        std::cout << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "✅ ALL TESTS PASSED!" << std::endl;
        std::cout << "==========================================" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
}
