/**
 * ============================================================================
 * MLP Standard Library - std.test Framework Day 2 Tests
 * ============================================================================
 * Test suite for test suites (describe) and hooks
 * Author: YZ_STAGE1_07
 * Date: 27 Ocak 2026
 * ============================================================================
 */

#include <iostream>
#include <cassert>

// Forward declarations of test builtins
extern "C" {
    // Basic assertions
    int melp_test_assert_eq_numeric(double actual, double expected);
    int melp_test_assert_true(int condition);
    int melp_test_register(const char* name, int handler_id);
    int melp_test_run_all(int* total, int* passed, int* failed, int* duration_ms);
    
    // Day 2: Suites and hooks
    int melp_test_describe(const char* suite_name);
    int melp_test_end_describe();
    int melp_test_before_each(int handler_id);
    int melp_test_after_each(int handler_id);
    int melp_test_before_all(int handler_id);
    int melp_test_after_all(int handler_id);
}

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

// Hook execution counters
static int before_each_count = 0;
static int after_each_count = 0;
static int before_all_count = 0;
static int after_all_count = 0;

#define TEST_START(name) std::cout << "\n🔵 Test: " << name << std::endl;
#define TEST_PASS(name) { tests_passed++; std::cout << "  ✅ " << name << " PASSED" << std::endl; }
#define TEST_FAIL(name, msg) { tests_failed++; std::cout << "  ❌ " << name << " FAILED: " << msg << std::endl; }
#define ASSERT_EQ(actual, expected, msg) if ((actual) != (expected)) { TEST_FAIL("ASSERT_EQ", msg); return; }
#define ASSERT_TRUE(cond, msg) if (!(cond)) { TEST_FAIL("ASSERT_TRUE", msg); return; }

/**
 * Test 1: describe() - Create test suite
 */
void test_describe_create_suite() {
    TEST_START("describe() - Create test suite");
    
    int result = melp_test_describe("Math Operations");
    ASSERT_EQ(result, 1, "Should successfully create suite");
    
    result = melp_test_end_describe();
    ASSERT_EQ(result, 1, "Should successfully end suite");
    
    TEST_PASS("describe() - Create test suite");
}

/**
 * Test 2: describe() with tests
 */
void test_describe_with_tests() {
    TEST_START("describe() with tests");
    
    melp_test_describe("Calculator Tests");
    
    // Register tests inside suite
    melp_test_register("Addition", 101);
    melp_test_register("Subtraction", 102);
    melp_test_register("Multiplication", 103);
    
    int result = melp_test_end_describe();
    ASSERT_EQ(result, 1, "Should end suite successfully");
    
    TEST_PASS("describe() with tests");
}

/**
 * Test 3: Multiple test suites
 */
void test_multiple_suites() {
    TEST_START("Multiple test suites");
    
    // First suite
    melp_test_describe("Suite 1");
    melp_test_register("Test 1A", 201);
    melp_test_register("Test 1B", 202);
    melp_test_end_describe();
    
    // Second suite
    melp_test_describe("Suite 2");
    melp_test_register("Test 2A", 211);
    melp_test_register("Test 2B", 212);
    melp_test_end_describe();
    
    TEST_PASS("Multiple test suites");
}

/**
 * Test 4: before_each() hook
 */
void test_before_each_hook() {
    TEST_START("before_each() hook");
    
    melp_test_describe("Suite with before_each");
    
    int result = melp_test_before_each(301);
    ASSERT_EQ(result, 1, "Should register before_each hook");
    
    melp_test_register("Test A", 302);
    melp_test_register("Test B", 303);
    
    melp_test_end_describe();
    
    TEST_PASS("before_each() hook");
}

/**
 * Test 5: after_each() hook
 */
void test_after_each_hook() {
    TEST_START("after_each() hook");
    
    melp_test_describe("Suite with after_each");
    
    int result = melp_test_after_each(401);
    ASSERT_EQ(result, 1, "Should register after_each hook");
    
    melp_test_register("Test X", 402);
    melp_test_register("Test Y", 403);
    
    melp_test_end_describe();
    
    TEST_PASS("after_each() hook");
}

/**
 * Test 6: before_all() hook
 */
void test_before_all_hook() {
    TEST_START("before_all() hook");
    
    melp_test_describe("Suite with before_all");
    
    int result = melp_test_before_all(501);
    ASSERT_EQ(result, 1, "Should register before_all hook");
    
    melp_test_register("Test 1", 502);
    melp_test_register("Test 2", 503);
    
    melp_test_end_describe();
    
    TEST_PASS("before_all() hook");
}

/**
 * Test 7: after_all() hook
 */
void test_after_all_hook() {
    TEST_START("after_all() hook");
    
    melp_test_describe("Suite with after_all");
    
    int result = melp_test_after_all(601);
    ASSERT_EQ(result, 1, "Should register after_all hook");
    
    melp_test_register("Test Alpha", 602);
    melp_test_register("Test Beta", 603);
    
    melp_test_end_describe();
    
    TEST_PASS("after_all() hook");
}

/**
 * Test 8: All hooks together
 */
void test_all_hooks_combined() {
    TEST_START("All hooks together");
    
    melp_test_describe("Suite with all hooks");
    
    ASSERT_EQ(melp_test_before_all(701), 1, "before_all should register");
    ASSERT_EQ(melp_test_before_each(702), 1, "before_each should register");
    ASSERT_EQ(melp_test_after_each(703), 1, "after_each should register");
    ASSERT_EQ(melp_test_after_all(704), 1, "after_all should register");
    
    melp_test_register("Test 1", 710);
    melp_test_register("Test 2", 711);
    melp_test_register("Test 3", 712);
    
    melp_test_end_describe();
    
    TEST_PASS("All hooks together");
}

/**
 * Test 9: Nested structure
 */
void test_nested_structure() {
    TEST_START("Nested structure");
    
    // Outer suite
    melp_test_describe("Outer Suite");
    melp_test_before_all(801);
    
    melp_test_register("Outer Test 1", 802);
    
    // Tests outside any suite
    melp_test_end_describe();
    
    // Another suite
    melp_test_describe("Another Suite");
    melp_test_register("Another Test 1", 811);
    melp_test_end_describe();
    
    TEST_PASS("Nested structure");
}

/**
 * Test 10: Hooks without suite (edge case)
 */
void test_hooks_without_suite() {
    TEST_START("Hooks without suite (edge case)");
    
    // Try to register hooks outside of a suite - should fail
    int result = melp_test_before_each(901);
    ASSERT_EQ(result, 0, "Should fail when not in suite");
    
    result = melp_test_after_each(902);
    ASSERT_EQ(result, 0, "Should fail when not in suite");
    
    result = melp_test_before_all(903);
    ASSERT_EQ(result, 0, "Should fail when not in suite");
    
    result = melp_test_after_all(904);
    ASSERT_EQ(result, 0, "Should fail when not in suite");
    
    TEST_PASS("Hooks without suite (edge case)");
}

/**
 * Test 11: Empty suite
 */
void test_empty_suite() {
    TEST_START("Empty suite");
    
    melp_test_describe("Empty Suite");
    // No tests registered
    int result = melp_test_end_describe();
    ASSERT_EQ(result, 1, "Should handle empty suite");
    
    TEST_PASS("Empty suite");
}

/**
 * Test 12: Suite with only hooks
 */
void test_suite_only_hooks() {
    TEST_START("Suite with only hooks");
    
    melp_test_describe("Hooks Only Suite");
    melp_test_before_all(1001);
    melp_test_after_all(1002);
    // No tests registered
    melp_test_end_describe();
    
    TEST_PASS("Suite with only hooks");
}

/**
 * Main test runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.test Framework - Day 2 Tests (Suites & Hooks)" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all tests
    test_describe_create_suite();
    test_describe_with_tests();
    test_multiple_suites();
    test_before_each_hook();
    test_after_each_hook();
    test_before_all_hook();
    test_after_all_hook();
    test_all_hooks_combined();
    test_nested_structure();
    test_hooks_without_suite();
    test_empty_suite();
    test_suite_only_hooks();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << tests_passed << std::endl;
    std::cout << "  ❌ Failed: " << tests_failed << std::endl;
    std::cout << "  📊 Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All Day 2 tests passed! Suites & hooks are ready.\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some tests failed. Review errors above.\n" << std::endl;
        return 1;
    }
}
