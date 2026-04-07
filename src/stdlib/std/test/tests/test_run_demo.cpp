// ============================================================================
// Test Run Demo - Shows hierarchical output with suites
// ============================================================================

#include "builtins.hpp"
#include <iostream>

extern "C" {
    int melp_test_describe(const char* suite_name);
    int melp_test_end_describe();
    int melp_test_before_each(int handler_id);
    int melp_test_after_each(int handler_id);
    int melp_test_before_all(int handler_id);
    int melp_test_after_all(int handler_id);
    int melp_test_register(const char* name, int handler_id);
    int melp_test_run_all(int* total, int* passed, int* failed, int* duration_ms);
}

int main() {
    std::cout << "Creating comprehensive test structure...\n" << std::endl;
    
    // Suite 1: String Operations
    melp_test_describe("String Operations");
    melp_test_before_each(1001);
    melp_test_after_each(1002);
    melp_test_register("concat works", 101);
    melp_test_register("length is correct", 102);
    melp_test_register("substring works", 103);
    melp_test_end_describe();
    
    // Suite 2: Math Operations
    melp_test_describe("Math Operations");
    melp_test_before_all(2001);
    melp_test_before_each(2002);
    melp_test_after_each(2003);
    melp_test_after_all(2004);
    melp_test_register("addition", 201);
    melp_test_register("subtraction", 202);
    melp_test_register("multiplication", 203);
    melp_test_register("division", 204);
    melp_test_end_describe();
    
    // Suite 3: Boolean Logic
    melp_test_describe("Boolean Logic");
    melp_test_register("AND operation", 301);
    melp_test_register("OR operation", 302);
    melp_test_register("NOT operation", 303);
    melp_test_end_describe();
    
    // Standalone tests
    melp_test_register("standalone test 1", 401);
    melp_test_register("standalone test 2", 402);
    
    // Run all tests with hierarchical output
    int total = 0, passed = 0, failed = 0, duration_ms = 0;
    melp_test_run_all(&total, &passed, &failed, &duration_ms);
    
    return 0;
}
