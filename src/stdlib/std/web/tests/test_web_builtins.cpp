/**
 * ============================================================================
 * MLP Standard Library - std.web Module Tests
 * ============================================================================
 * Test suite for web framework builtins
 * Author: YZ_STAGE1_05
 * Date: 27 Ocak 2026
 * ============================================================================
 */

#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <thread>
#include <chrono>

// Forward declarations of builtins
extern "C" {
    int melp_web_create_server(int port, void** daemon_ptr, int* is_running);
    int melp_web_server_stop(void* daemon_ptr);
    int melp_web_register_route(void* daemon_ptr, const char* method, const char* path, int handler_id);
    int melp_web_parse_query_param(const char* query_string, const char* key, char** value);
    int melp_web_parse_path_param(const char* path_pattern, const char* actual_path, const char* param_name, char** value);
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
 * Test 1: Server Creation
 */
void test_server_creation() {
    TEST_START("Server Creation");
    
    void* daemon_ptr = nullptr;
    int is_running = 0;
    
    int result = melp_web_create_server(9090, &daemon_ptr, &is_running);
    
    ASSERT_EQ(result, 1, "Server creation should succeed");
    ASSERT_NOT_NULL(daemon_ptr, "Daemon pointer should not be null");
    ASSERT_EQ(is_running, 1, "Server should be marked as running");
    
    // Cleanup
    if (daemon_ptr) {
        melp_web_server_stop(daemon_ptr);
    }
    
    TEST_PASS("Server Creation");
}

/**
 * Test 2: Route Registration
 */
void test_route_registration() {
    TEST_START("Route Registration");
    
    void* daemon_ptr = nullptr;
    int is_running = 0;
    
    melp_web_create_server(9091, &daemon_ptr, &is_running);
    ASSERT_NOT_NULL(daemon_ptr, "Server must be created first");
    
    int result = melp_web_register_route(daemon_ptr, "GET", "/hello", 1);
    ASSERT_EQ(result, 1, "Route registration should succeed");
    
    result = melp_web_register_route(daemon_ptr, "POST", "/users", 2);
    ASSERT_EQ(result, 1, "Second route registration should succeed");
    
    // Cleanup
    melp_web_server_stop(daemon_ptr);
    
    TEST_PASS("Route Registration");
}

/**
 * Test 3: Query Parameter Parsing - Basic
 */
void test_query_param_basic() {
    TEST_START("Query Parameter Parsing - Basic");
    
    const char* query = "id=123&name=test";
    char* value = nullptr;
    
    int found = melp_web_parse_query_param(query, "id", &value);
    ASSERT_EQ(found, 1, "Should find 'id' parameter");
    ASSERT_TRUE(strcmp(value, "123") == 0, "Value should be '123'");
    free(value);
    
    found = melp_web_parse_query_param(query, "name", &value);
    ASSERT_EQ(found, 1, "Should find 'name' parameter");
    ASSERT_TRUE(strcmp(value, "test") == 0, "Value should be 'test'");
    free(value);
    
    TEST_PASS("Query Parameter Parsing - Basic");
}

/**
 * Test 4: Query Parameter Parsing - Not Found
 */
void test_query_param_not_found() {
    TEST_START("Query Parameter Parsing - Not Found");
    
    const char* query = "id=123&name=test";
    char* value = nullptr;
    
    int found = melp_web_parse_query_param(query, "missing", &value);
    ASSERT_EQ(found, 0, "Should not find 'missing' parameter");
    ASSERT_TRUE(strcmp(value, "") == 0, "Value should be empty string");
    free(value);
    
    TEST_PASS("Query Parameter Parsing - Not Found");
}

/**
 * Test 5: Query Parameter Parsing - Last Parameter
 */
void test_query_param_last() {
    TEST_START("Query Parameter Parsing - Last Parameter");
    
    const char* query = "foo=bar&baz=qux";
    char* value = nullptr;
    
    int found = melp_web_parse_query_param(query, "baz", &value);
    ASSERT_EQ(found, 1, "Should find 'baz' parameter");
    ASSERT_TRUE(strcmp(value, "qux") == 0, "Value should be 'qux'");
    free(value);
    
    TEST_PASS("Query Parameter Parsing - Last Parameter");
}

/**
 * Test 6: Path Parameter Parsing - Basic
 */
void test_path_param_basic() {
    TEST_START("Path Parameter Parsing - Basic");
    
    const char* pattern = "/users/:id";
    const char* actual = "/users/123";
    char* value = nullptr;
    
    int found = melp_web_parse_path_param(pattern, actual, "id", &value);
    ASSERT_EQ(found, 1, "Should find 'id' parameter");
    ASSERT_TRUE(strcmp(value, "123") == 0, "Value should be '123'");
    free(value);
    
    TEST_PASS("Path Parameter Parsing - Basic");
}

/**
 * Test 7: Path Parameter Parsing - Multiple Segments
 */
void test_path_param_multiple() {
    TEST_START("Path Parameter Parsing - Multiple Segments");
    
    const char* pattern = "/users/:userId/posts/:postId";
    const char* actual = "/users/42/posts/789";
    char* value = nullptr;
    
    int found = melp_web_parse_path_param(pattern, actual, "userId", &value);
    ASSERT_EQ(found, 1, "Should find 'userId' parameter");
    ASSERT_TRUE(strcmp(value, "42") == 0, "Value should be '42'");
    free(value);
    
    found = melp_web_parse_path_param(pattern, actual, "postId", &value);
    ASSERT_EQ(found, 1, "Should find 'postId' parameter");
    ASSERT_TRUE(strcmp(value, "789") == 0, "Value should be '789'");
    free(value);
    
    TEST_PASS("Path Parameter Parsing - Multiple Segments");
}

/**
 * Test 8: Path Parameter Parsing - Not Found
 */
void test_path_param_not_found() {
    TEST_START("Path Parameter Parsing - Not Found");
    
    const char* pattern = "/users/:id";
    const char* actual = "/users/123";
    char* value = nullptr;
    
    int found = melp_web_parse_path_param(pattern, actual, "name", &value);
    ASSERT_EQ(found, 0, "Should not find 'name' parameter");
    free(value);
    
    TEST_PASS("Path Parameter Parsing - Not Found");
}

/**
 * Test 9: Empty Query String
 */
void test_empty_query() {
    TEST_START("Empty Query String");
    
    const char* query = "";
    char* value = nullptr;
    
    int found = melp_web_parse_query_param(query, "id", &value);
    ASSERT_EQ(found, 0, "Should not find parameter in empty query");
    free(value);
    
    TEST_PASS("Empty Query String");
}

/**
 * Test 10: Server Creation Failure (Invalid Port)
 */
void test_server_creation_invalid_port() {
    TEST_START("Server Creation - Invalid Port");
    
    void* daemon_ptr = nullptr;
    int is_running = 0;
    
    // Try to create server on port 0 (should fail or handle gracefully)
    int result = melp_web_create_server(0, &daemon_ptr, &is_running);
    
    // Either fails (result=0) or succeeds with random port
    // Just verify function doesn't crash
    std::cout << "  ℹ️  Port 0 result: " << (result ? "success" : "failure") << std::endl;
    
    if (daemon_ptr) {
        melp_web_server_stop(daemon_ptr);
    }
    
    TEST_PASS("Server Creation - Invalid Port");
}

/**
 * Main test runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.web Module - Builtin Tests" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all tests
    test_server_creation();
    test_route_registration();
    test_query_param_basic();
    test_query_param_not_found();
    test_query_param_last();
    test_path_param_basic();
    test_path_param_multiple();
    test_path_param_not_found();
    test_empty_query();
    test_server_creation_invalid_port();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << tests_passed << std::endl;
    std::cout << "  ❌ Failed: " << tests_failed << std::endl;
    std::cout << "  📊 Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All tests passed! std.web builtins are ready.\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some tests failed. Review errors above.\n" << std::endl;
        return 1;
    }
}
