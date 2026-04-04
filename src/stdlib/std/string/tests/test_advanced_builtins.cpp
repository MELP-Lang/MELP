#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace melp_runtime;

void test_string_replace() {
    std::cout << "Test 1: string_replace..." << std::endl;
    
    assert(string_replace("hello world", "world", "universe") == "hello universe");
    assert(string_replace("foo bar foo", "foo", "baz") == "baz bar baz");
    assert(string_replace("test", "x", "y") == "test");  // No match
    assert(string_replace("", "old", "new") == "");      // Empty string
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_trim() {
    std::cout << "Test 2: string_trim..." << std::endl;
    
    assert(string_trim("  hello  ") == "hello");
    assert(string_trim("\t\ntest\r\n") == "test");
    assert(string_trim("no spaces") == "no spaces");
    assert(string_trim("   ") == "");
    assert(string_trim("") == "");
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_case() {
    std::cout << "Test 3: string case conversion..." << std::endl;
    
    assert(string_to_upper("hello") == "HELLO");
    assert(string_to_upper("Hello World") == "HELLO WORLD");
    assert(string_to_upper("123abc") == "123ABC");
    
    assert(string_to_lower("HELLO") == "hello");
    assert(string_to_lower("Hello World") == "hello world");
    assert(string_to_lower("123ABC") == "123abc");
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_starts_ends_with() {
    std::cout << "Test 4: starts_with / ends_with..." << std::endl;
    
    assert(string_starts_with("hello world", "hello") == true);
    assert(string_starts_with("hello world", "world") == false);
    assert(string_starts_with("test", "testing") == false);
    
    assert(string_ends_with("hello world", "world") == true);
    assert(string_ends_with("hello world", "hello") == false);
    assert(string_ends_with("test", "est") == true);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_contains() {
    std::cout << "Test 5: string_contains..." << std::endl;
    
    assert(string_contains("hello world", "world") == true);
    assert(string_contains("hello world", "lo wo") == true);
    assert(string_contains("hello world", "xyz") == false);
    assert(string_contains("test", "") == true);  // Empty string always contained
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_string_index_of() {
    std::cout << "Test 6: string_index_of..." << std::endl;
    
    assert(string_index_of("hello world", "world") == 6);
    assert(string_index_of("hello world", "hello") == 0);
    assert(string_index_of("hello world", "o") == 4);  // First occurrence
    assert(string_index_of("hello world", "xyz") == -1);  // Not found
    
    std::cout << "  ✅ PASSED" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "String Module Advanced Operations Test" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    try {
        test_string_replace();
        test_string_trim();
        test_string_case();
        test_string_starts_ends_with();
        test_string_contains();
        test_string_index_of();
        
        std::cout << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "✅ ALL TESTS PASSED! (6/6)" << std::endl;
        std::cout << "==========================================" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
}
