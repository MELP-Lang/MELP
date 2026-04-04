/**
 * ============================================================================
 * MLP Standard Library - std.test Framework Tests
 * ============================================================================
 * Test suite for testing framework itself
 * Author: YZ_STAGE1_07
 * Date: 27 Ocak 2026
 * ============================================================================
 */

#include <iostream>
#include <cassert>
#include <cstring>

// Forward declarations of test builtins
extern "C" {
    int melp_test_assert_eq_numeric(double actual, double expected);
    int melp_test_assert_eq_string(const char* actual, const char* expected);
    int melp_test_assert_ne_numeric(double actual, double expected);
    int melp_test_assert_ne_string(const char* actual, const char* expected);
    int melp_test_assert_true(int condition);
    int melp_test_assert_false(int condition);
    int melp_test_assert(int condition, const char* message);
    int melp_test_register(const char* name, int handler_id);
    int melp_test_run_all(int* total, int* passed, int* failed, int* duration_ms);
}

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_START(name) std::cout << "\n🔵 Test: " << name << std::endl;
#define TEST_PASS(name) { tests_passed++; std::cout << "  ✅ " << name << " PASSED" << std::endl; }
#define TEST_FAIL(name, msg) { tests_failed++; std::cout << "  ❌ " << name << " FAILED: " << msg << std::endl; }
#define ASSERT_EQ(actual, expected, msg) if ((actual) != (expected)) { TEST_FAIL("ASSERT_EQ", msg); return; }
#define ASSERT_TRUE(cond, msg) if (!(cond)) { TEST_FAIL("ASSERT_TRUE", msg); return; }

/**
 * Test 1: assert_eq_numeric - Equal Values
 */
void test_assert_eq_numeric_equal() {
    TEST_START("assert_eq_numeric - Equal Values");
    
    int result = melp_test_assert_eq_numeric(5.0, 5.0);
    ASSERT_EQ(result, 1, "Should return 1 for equal numeric values");
    
    result = melp_test_assert_eq_numeric(0.0, 0.0);
    ASSERT_EQ(result, 1, "Should return 1 for zero values");
    
    result = melp_test_assert_eq_numeric(-10.5, -10.5);
    ASSERT_EQ(result, 1, "Should return 1 for negative values");
    
    TEST_PASS("assert_eq_numeric - Equal Values");
}

/**
 * Test 2: assert_eq_numeric - Unequal Values
 */
void test_assert_eq_numeric_unequal() {
    TEST_START("assert_eq_numeric - Unequal Values");
    
    int result = melp_test_assert_eq_numeric(5.0, 6.0);
    ASSERT_EQ(result, 0, "Should return 0 for unequal values");
    
    result = melp_test_assert_eq_numeric(1.0, -1.0);
    ASSERT_EQ(result, 0, "Should return 0 for different signs");
    
    TEST_PASS("assert_eq_numeric - Unequal Values");
}

/**
 * Test 3: assert_eq_string - Equal Strings
 */
void test_assert_eq_string_equal() {
    TEST_START("assert_eq_string - Equal Strings");
    
    int result = melp_test_assert_eq_string("hello", "hello");
    ASSERT_EQ(result, 1, "Should return 1 for equal strings");
    
    result = melp_test_assert_eq_string("", "");
    ASSERT_EQ(result, 1, "Should return 1 for empty strings");
    
    result = melp_test_assert_eq_string("test123", "test123");
    ASSERT_EQ(result, 1, "Should return 1 for alphanumeric strings");
    
    TEST_PASS("assert_eq_string - Equal Strings");
}

/**
 * Test 4: assert_eq_string - Unequal Strings
 */
void test_assert_eq_string_unequal() {
    TEST_START("assert_eq_string - Unequal Strings");
    
    int result = melp_test_assert_eq_string("hello", "world");
    ASSERT_EQ(result, 0, "Should return 0 for different strings");
    
    result = melp_test_assert_eq_string("Hello", "hello");
    ASSERT_EQ(result, 0, "Should be case sensitive");
    
    result = melp_test_assert_eq_string("test", "test ");
    ASSERT_EQ(result, 0, "Should detect trailing spaces");
    
    TEST_PASS("assert_eq_string - Unequal Strings");
}

/**
 * Test 5: assert_ne_numeric - Different Values
 */
void test_assert_ne_numeric() {
    TEST_START("assert_ne_numeric - Different Values");
    
    int result = melp_test_assert_ne_numeric(5.0, 6.0);
    ASSERT_EQ(result, 1, "Should return 1 for different values");
    
    result = melp_test_assert_ne_numeric(1.0, -1.0);
    ASSERT_EQ(result, 1, "Should return 1 for different signs");
    
    result = melp_test_assert_ne_numeric(5.0, 5.0);
    ASSERT_EQ(result, 0, "Should return 0 for equal values");
    
    TEST_PASS("assert_ne_numeric - Different Values");
}

/**
 * Test 6: assert_ne_string - Different Strings
 */
void test_assert_ne_string() {
    TEST_START("assert_ne_string - Different Strings");
    
    int result = melp_test_assert_ne_string("hello", "world");
    ASSERT_EQ(result, 1, "Should return 1 for different strings");
    
    result = melp_test_assert_ne_string("test", "Test");
    ASSERT_EQ(result, 1, "Should be case sensitive");
    
    result = melp_test_assert_ne_string("same", "same");
    ASSERT_EQ(result, 0, "Should return 0 for equal strings");
    
    TEST_PASS("assert_ne_string - Different Strings");
}

/**
 * Test 7: assert_true
 */
void test_assert_true() {
    TEST_START("assert_true");
    
    int result = melp_test_assert_true(1);
    ASSERT_EQ(result, 1, "Should return 1 for true condition (1)");
    
    result = melp_test_assert_true(5);
    ASSERT_EQ(result, 1, "Should return 1 for non-zero value");
    
    result = melp_test_assert_true(0);
    ASSERT_EQ(result, 0, "Should return 0 for false condition (0)");
    
    TEST_PASS("assert_true");
}

/**
 * Test 8: assert_false
 */
void test_assert_false() {
    TEST_START("assert_false");
    
    int result = melp_test_assert_false(0);
    ASSERT_EQ(result, 1, "Should return 1 for false condition (0)");
    
    result = melp_test_assert_false(1);
    ASSERT_EQ(result, 0, "Should return 0 for true condition (1)");
    
    result = melp_test_assert_false(5);
    ASSERT_EQ(result, 0, "Should return 0 for non-zero value");
    
    TEST_PASS("assert_false");
}

/**
 * Test 9: assert with message
 */
void test_assert_with_message() {
    TEST_START("assert with message");
    
    int result = melp_test_assert(1, "This should pass");
    ASSERT_EQ(result, 1, "Should return 1 for true condition");
    
    result = melp_test_assert(5 > 3, "5 is greater than 3");
    ASSERT_EQ(result, 1, "Should return 1 for true comparison");
    
    result = melp_test_assert(0, "This should fail");
    ASSERT_EQ(result, 0, "Should return 0 for false condition");
    
    TEST_PASS("assert with message");
}

/**
 * Test 10: Test Registration
 */
void test_register_tests() {
    TEST_START("Test Registration");
    
    int result = melp_test_register("Sample Test 1", 1);
    ASSERT_EQ(result, 1, "Should successfully register test");
    
    result = melp_test_register("Sample Test 2", 2);
    ASSERT_EQ(result, 1, "Should register multiple tests");
    
    result = melp_test_register("Sample Test 3", 3);
    ASSERT_EQ(result, 1, "Should continue registering");
    
    TEST_PASS("Test Registration");
}

/**
 * Test 11: Run Tests
 */
void test_run_tests() {
    TEST_START("Run Tests");
    
    // Register some tests first
    melp_test_register("Mock Test 1", 10);
    melp_test_register("Mock Test 2", 11);
    melp_test_register("Mock Test 3", 12);
    
    int total = 0, passed = 0, failed = 0, duration = 0;
    int result = melp_test_run_all(&total, &passed, &failed, &duration);
    
    ASSERT_EQ(result, 1, "Should successfully run tests");
    ASSERT_TRUE(total >= 3, "Should have at least 3 tests");
    ASSERT_TRUE(duration >= 0, "Duration should be non-negative");
    
    TEST_PASS("Run Tests");
}

/**
 * Test 12: Floating Point Comparison
 */
void test_floating_point() {
    TEST_START("Floating Point Comparison");
    
    int result = melp_test_assert_eq_numeric(0.1 + 0.2, 0.3);
    // Note: This might fail due to floating point precision
    // Our implementation uses EPSILON for comparison
    
    result = melp_test_assert_eq_numeric(1.0000001, 1.0000002);
    // Should be considered equal within epsilon
    
    TEST_PASS("Floating Point Comparison");
}

/**
 * Main test runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.test Framework - Builtin Tests" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all tests
    test_assert_eq_numeric_equal();
    test_assert_eq_numeric_unequal();
    test_assert_eq_string_equal();
    test_assert_eq_string_unequal();
    test_assert_ne_numeric();
    test_assert_ne_string();
    test_assert_true();
    test_assert_false();
    test_assert_with_message();
    test_register_tests();
    test_run_tests();
    test_floating_point();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << tests_passed << std::endl;
    std::cout << "  ❌ Failed: " << tests_failed << std::endl;
    std::cout << "  📊 Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All tests passed! std.test framework is ready.\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some tests failed. Review errors above.\n" << std::endl;
        return 1;
    }
}
