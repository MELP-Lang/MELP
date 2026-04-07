/**
 * ============================================================================
 * MLP Standard Library - Test Module Runtime (Standalone)
 * ============================================================================
 * Minimal runtime containing only test module functions
 * For testing the test framework itself without other dependencies
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <cmath>

namespace melp_runtime {

// ============================================================================
// Test Module Data Structures
// ============================================================================

struct TestCase {
    std::string name;
    int handler_id;
    bool passed;
    std::string error_message;
    std::string suite_name;
};

struct TestSuite {
    std::string name;
    std::vector<int> test_indices;
    int before_each_handler = -1;
    int after_each_handler = -1;
    int before_all_handler = -1;
    int after_all_handler = -1;
};

// Global test registry
static std::vector<TestCase> g_tests;
static std::vector<TestSuite> g_suites;
static int g_current_test_index = -1;
static int g_current_suite_index = -1;

// ============================================================================
// Test Module Functions
// ============================================================================

const double EPSILON = 1e-9;

/**
 * Assert numeric equality
 */
int test_assert_eq_numeric(double actual, double expected) {
    bool passed = std::abs(actual - expected) < EPSILON;
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = 
            "Expected " + std::to_string(expected) + 
            ", got " + std::to_string(actual);
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert string equality
 */
int test_assert_eq_string(const char* actual, const char* expected) {
    bool passed = (std::strcmp(actual, expected) == 0);
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = 
            "Expected \"" + std::string(expected) + 
            "\", got \"" + std::string(actual) + "\"";
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert numeric inequality
 */
int test_assert_ne_numeric(double actual, double expected) {
    bool passed = std::abs(actual - expected) >= EPSILON;
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = 
            "Expected values to be different, but both are " + 
            std::to_string(actual);
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert string inequality
 */
int test_assert_ne_string(const char* actual, const char* expected) {
    bool passed = (std::strcmp(actual, expected) != 0);
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = 
            "Expected strings to be different, but both are \"" + 
            std::string(actual) + "\"";
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert true condition
 */
int test_assert_true(int condition) {
    bool passed = (condition != 0);
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = "Expected true, got false";
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert false condition
 */
int test_assert_false(int condition) {
    bool passed = (condition == 0);
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = "Expected false, got true";
    }
    
    return passed ? 1 : 0;
}

/**
 * Assert with custom message
 */
int test_assert(int condition, const char* message) {
    bool passed = (condition != 0);
    
    if (!passed && g_current_test_index >= 0 && g_current_test_index < g_tests.size()) {
        g_tests[g_current_test_index].passed = false;
        g_tests[g_current_test_index].error_message = std::string(message);
    }
    
    return passed ? 1 : 0;
}

/**
 * Register a test
 */
int test_register(const char* name, int handler_id) {
    TestCase test;
    test.name = name;
    test.handler_id = handler_id;
    test.passed = true;
    test.error_message = "";
    
    g_tests.push_back(test);
    return 1;
}

/**
 * Run all tests
 */
int test_run_all(int* total, int* passed, int* failed, int* duration_ms) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    *total = g_tests.size();
    *passed = 0;
    *failed = 0;
    
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "🧪 Running " << *total << " tests..." << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    for (size_t i = 0; i < g_tests.size(); i++) {
        g_current_test_index = i;
        TestCase& test = g_tests[i];
        
        // In real implementation, we would call the test handler here
        // For now, we just report the test status based on previous assertions
        
        if (test.passed) {
            std::cout << "✅ " << test.name << std::endl;
            (*passed)++;
        } else {
            std::cout << "❌ " << test.name << std::endl;
            if (!test.error_message.empty()) {
                std::cout << "   Error: " << test.error_message << std::endl;
            }
            (*failed)++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    *duration_ms = duration.count();
    
    std::cout << std::string(70, '=') << std::endl;
    std::cout << "📊 Results: " << *passed << " passed, " << *failed << " failed";
    std::cout << " (Duration: " << *duration_ms << "ms)" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    g_current_test_index = -1;
    return 1;
}

// ============================================================================
// Test Suites (Day 2)
// ============================================================================

int test_describe(const char* suite_name) {
    if (!suite_name) {
        return 0;
    }
    
    TestSuite suite;
    suite.name = suite_name;
    g_suites.push_back(suite);
    g_current_suite_index = g_suites.size() - 1;
    
    std::cout << "\n📦 Test Suite: " << suite_name << std::endl;
    
    return 1;
}

int test_end_describe() {
    if (g_current_suite_index >= 0 && g_current_suite_index < g_suites.size()) {
        const TestSuite& suite = g_suites[g_current_suite_index];
        std::cout << "   └─ " << suite.test_indices.size() << " tests registered" << std::endl;
    }
    
    g_current_suite_index = -1;
    return 1;
}

// ============================================================================
// Test Hooks (Day 2)
// ============================================================================

int test_before_each(int handler_id) {
    if (g_current_suite_index >= 0 && g_current_suite_index < g_suites.size()) {
        g_suites[g_current_suite_index].before_each_handler = handler_id;
        std::cout << "   ⚙️  before_each hook registered" << std::endl;
        return 1;
    }
    return 0;
}

int test_after_each(int handler_id) {
    if (g_current_suite_index >= 0 && g_current_suite_index < g_suites.size()) {
        g_suites[g_current_suite_index].after_each_handler = handler_id;
        std::cout << "   ⚙️  after_each hook registered" << std::endl;
        return 1;
    }
    return 0;
}

int test_before_all(int handler_id) {
    if (g_current_suite_index >= 0 && g_current_suite_index < g_suites.size()) {
        g_suites[g_current_suite_index].before_all_handler = handler_id;
        std::cout << "   ⚙️  before_all hook registered" << std::endl;
        return 1;
    }
    return 0;
}

int test_after_all(int handler_id) {
    if (g_current_suite_index >= 0 && g_current_suite_index < g_suites.size()) {
        g_suites[g_current_suite_index].after_all_handler = handler_id;
        std::cout << "   ⚙️  after_all hook registered" << std::endl;
        return 1;
    }
    return 0;
}

} // namespace melp_runtime

// ============================================================================
// C Interface (extern "C")
// ============================================================================

extern "C" {

int melp_test_assert_eq_numeric(double actual, double expected) {
    return melp_runtime::test_assert_eq_numeric(actual, expected);
}

int melp_test_assert_eq_string(const char* actual, const char* expected) {
    return melp_runtime::test_assert_eq_string(actual, expected);
}

int melp_test_assert_ne_numeric(double actual, double expected) {
    return melp_runtime::test_assert_ne_numeric(actual, expected);
}

int melp_test_assert_ne_string(const char* actual, const char* expected) {
    return melp_runtime::test_assert_ne_string(actual, expected);
}

int melp_test_assert_true(int condition) {
    return melp_runtime::test_assert_true(condition);
}

int melp_test_assert_false(int condition) {
    return melp_runtime::test_assert_false(condition);
}

int melp_test_assert(int condition, const char* message) {
    return melp_runtime::test_assert(condition, message);
}

int melp_test_register(const char* name, int handler_id) {
    return melp_runtime::test_register(name, handler_id);
}

int melp_test_run_all(int* total, int* passed, int* failed, int* duration_ms) {
    return melp_runtime::test_run_all(total, passed, failed, duration_ms);
}

// Day 2: Test suites and hooks
int melp_test_describe(const char* suite_name) {
    return melp_runtime::test_describe(suite_name);
}

int melp_test_end_describe() {
    return melp_runtime::test_end_describe();
}

int melp_test_before_each(int handler_id) {
    return melp_runtime::test_before_each(handler_id);
}

int melp_test_after_each(int handler_id) {
    return melp_runtime::test_after_each(handler_id);
}

int melp_test_before_all(int handler_id) {
    return melp_runtime::test_before_all(handler_id);
}

int melp_test_after_all(int handler_id) {
    return melp_runtime::test_after_all(handler_id);
}

} // extern "C"
