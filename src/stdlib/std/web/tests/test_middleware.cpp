/**
 * ============================================================================
 * MLP Standard Library - std.web Middleware Tests
 * ============================================================================
 * Test suite for middleware functionality
 * Author: YZ_STAGE1_05
 * Date: 27 Ocak 2026 (Day 3)
 * ============================================================================
 */

#include <iostream>
#include <cassert>
#include <cstring>
#include <string>

// Forward declarations of middleware builtins
extern "C" {
    int melp_web_log_request(const char* method, const char* path, const char* query);
    int melp_web_add_cors_headers(void* headers_ptr);
    char* melp_web_get_header(void* headers_ptr, const char* key);
    int melp_web_set_response_header(void* headers_ptr, const char* key, const char* value);
    int melp_web_check_rate_limit(const char* path);
}

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

// Test helper macros
#define TEST_START(name) std::cout << "\n🔵 Test: " << name << std::endl;
#define TEST_PASS(name) { tests_passed++; std::cout << "  ✅ " << name << " PASSED" << std::endl; }
#define TEST_FAIL(name, msg) { tests_failed++; std::cout << "  ❌ " << name << " FAILED: " << msg << std::endl; }
#define ASSERT_EQ(actual, expected, msg) if ((actual) != (expected)) { TEST_FAIL("ASSERT_EQ", msg); return; }
#define ASSERT_TRUE(cond, msg) if (!(cond)) { TEST_FAIL("ASSERT_TRUE", msg); return; }
#define ASSERT_NOT_NULL(ptr, msg) if ((ptr) == nullptr) { TEST_FAIL("ASSERT_NOT_NULL", msg); return; }

/**
 * Test 1: Logger Middleware - Basic Request
 */
void test_logger_basic() {
    TEST_START("Logger Middleware - Basic Request");
    
    int result = melp_web_log_request("GET", "/api/users", "id=123");
    ASSERT_EQ(result, 1, "Logger should return success");
    
    TEST_PASS("Logger Middleware - Basic Request");
}

/**
 * Test 2: Logger Middleware - No Query String
 */
void test_logger_no_query() {
    TEST_START("Logger Middleware - No Query String");
    
    int result = melp_web_log_request("POST", "/api/users", "");
    ASSERT_EQ(result, 1, "Logger should handle empty query");
    
    TEST_PASS("Logger Middleware - No Query String");
}

/**
 * Test 3: Logger Middleware - Null Query
 */
void test_logger_null_query() {
    TEST_START("Logger Middleware - Null Query");
    
    int result = melp_web_log_request("DELETE", "/api/users/1", nullptr);
    ASSERT_EQ(result, 1, "Logger should handle null query");
    
    TEST_PASS("Logger Middleware - Null Query");
}

/**
 * Test 4: CORS Middleware
 */
void test_cors_middleware() {
    TEST_START("CORS Middleware");
    
    void* headers_ptr = nullptr;  // Simplified
    int result = melp_web_add_cors_headers(headers_ptr);
    ASSERT_EQ(result, 1, "CORS middleware should succeed");
    
    TEST_PASS("CORS Middleware");
}

/**
 * Test 5: Get Header - Empty
 */
void test_get_header_empty() {
    TEST_START("Get Header - Empty");
    
    void* headers_ptr = nullptr;
    char* value = melp_web_get_header(headers_ptr, "Authorization");
    
    ASSERT_NOT_NULL(value, "Should return valid pointer");
    ASSERT_TRUE(strcmp(value, "") == 0, "Should return empty string for missing header");
    
    free(value);
    TEST_PASS("Get Header - Empty");
}

/**
 * Test 6: Set Response Header
 */
void test_set_response_header() {
    TEST_START("Set Response Header");
    
    void* headers_ptr = nullptr;
    int result = melp_web_set_response_header(headers_ptr, "Content-Type", "application/json");
    
    ASSERT_EQ(result, 1, "Should successfully set header");
    
    TEST_PASS("Set Response Header");
}

/**
 * Test 7: Rate Limiting - Allow
 */
void test_rate_limit_allow() {
    TEST_START("Rate Limiting - Allow");
    
    int allowed = melp_web_check_rate_limit("/api/users");
    ASSERT_EQ(allowed, 1, "Should allow request");
    
    TEST_PASS("Rate Limiting - Allow");
}

/**
 * Test 8: Rate Limiting - Multiple Paths
 */
void test_rate_limit_multiple() {
    TEST_START("Rate Limiting - Multiple Paths");
    
    int allowed1 = melp_web_check_rate_limit("/api/users");
    int allowed2 = melp_web_check_rate_limit("/api/posts");
    int allowed3 = melp_web_check_rate_limit("/api/comments");
    
    ASSERT_EQ(allowed1, 1, "Path 1 should be allowed");
    ASSERT_EQ(allowed2, 1, "Path 2 should be allowed");
    ASSERT_EQ(allowed3, 1, "Path 3 should be allowed");
    
    TEST_PASS("Rate Limiting - Multiple Paths");
}

/**
 * Test 9: Logger with Various HTTP Methods
 */
void test_logger_various_methods() {
    TEST_START("Logger - Various HTTP Methods");
    
    int r1 = melp_web_log_request("GET", "/", "");
    int r2 = melp_web_log_request("POST", "/api/data", "");
    int r3 = melp_web_log_request("PUT", "/api/data/1", "");
    int r4 = melp_web_log_request("DELETE", "/api/data/1", "");
    int r5 = melp_web_log_request("PATCH", "/api/data/1", "partial=true");
    
    ASSERT_EQ(r1, 1, "GET should be logged");
    ASSERT_EQ(r2, 1, "POST should be logged");
    ASSERT_EQ(r3, 1, "PUT should be logged");
    ASSERT_EQ(r4, 1, "DELETE should be logged");
    ASSERT_EQ(r5, 1, "PATCH should be logged");
    
    TEST_PASS("Logger - Various HTTP Methods");
}

/**
 * Test 10: Middleware Error Handling
 */
void test_middleware_error_handling() {
    TEST_START("Middleware Error Handling");
    
    // Test null inputs
    int r1 = melp_web_log_request(nullptr, "/path", "");
    ASSERT_EQ(r1, 0, "Should fail with null method");
    
    int r2 = melp_web_log_request("GET", nullptr, "");
    ASSERT_EQ(r2, 0, "Should fail with null path");
    
    int r3 = melp_web_set_response_header(nullptr, nullptr, "value");
    ASSERT_EQ(r3, 0, "Should fail with null key");
    
    int r4 = melp_web_check_rate_limit(nullptr);
    ASSERT_EQ(r4, 0, "Should fail with null path");
    
    TEST_PASS("Middleware Error Handling");
}

/**
 * Main test runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.web Module - Middleware Tests" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all tests
    test_logger_basic();
    test_logger_no_query();
    test_logger_null_query();
    test_cors_middleware();
    test_get_header_empty();
    test_set_response_header();
    test_rate_limit_allow();
    test_rate_limit_multiple();
    test_logger_various_methods();
    test_middleware_error_handling();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << tests_passed << std::endl;
    std::cout << "  ❌ Failed: " << tests_failed << std::endl;
    std::cout << "  📊 Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All middleware tests passed!\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some tests failed. Review errors above.\n" << std::endl;
        return 1;
    }
}
