/**
 * ============================================================================
 * MLP Standard Library - std.web Integration Test
 * ============================================================================
 * End-to-end integration test for web framework
 * Tests complete request/response cycle
 * Author: YZ_STAGE1_05
 * Date: 27 Ocak 2026 (Day 3)
 * ============================================================================
 */

#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <thread>
#include <chrono>

// Forward declarations
extern "C" {
    // Server management
    int melp_web_create_server(int port, void** daemon_ptr, int* is_running);
    int melp_web_server_stop(void* daemon_ptr);
    int melp_web_register_route(void* daemon_ptr, const char* method, const char* path, int handler_id);
    
    // Request/Response
    int melp_web_parse_query_param(const char* query_string, const char* key, char** value);
    int melp_web_parse_path_param(const char* path_pattern, const char* actual_path, const char* param_name, char** value);
    
    // Middleware
    int melp_web_log_request(const char* method, const char* path, const char* query);
    int melp_web_add_cors_headers(void* headers_ptr);
    int melp_web_check_rate_limit(const char* path);
}

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_START(name) std::cout << "\n🔵 Integration Test: " << name << std::endl;
#define TEST_PASS(name) { tests_passed++; std::cout << "  ✅ " << name << " PASSED" << std::endl; }
#define TEST_FAIL(name, msg) { tests_failed++; std::cout << "  ❌ " << name << " FAILED: " << msg << std::endl; }
#define ASSERT_EQ(actual, expected, msg) if ((actual) != (expected)) { TEST_FAIL("ASSERT", msg); return; }
#define ASSERT_TRUE(cond, msg) if (!(cond)) { TEST_FAIL("ASSERT", msg); return; }

/**
 * Integration Test 1: Complete Server Lifecycle
 */
void test_server_lifecycle() {
    TEST_START("Complete Server Lifecycle");
    
    void* daemon_ptr = nullptr;
    int is_running = 0;
    
    // Create server
    int result = melp_web_create_server(9999, &daemon_ptr, &is_running);
    ASSERT_EQ(result, 1, "Server creation should succeed");
    ASSERT_TRUE(daemon_ptr != nullptr, "Daemon pointer should be valid");
    ASSERT_EQ(is_running, 1, "Server should be running");
    
    // Register routes
    result = melp_web_register_route(daemon_ptr, "GET", "/", 1);
    ASSERT_EQ(result, 1, "Route 1 registration should succeed");
    
    result = melp_web_register_route(daemon_ptr, "POST", "/api/data", 2);
    ASSERT_EQ(result, 1, "Route 2 registration should succeed");
    
    result = melp_web_register_route(daemon_ptr, "GET", "/api/users/:id", 3);
    ASSERT_EQ(result, 1, "Route 3 (dynamic) registration should succeed");
    
    // Stop server
    result = melp_web_server_stop(daemon_ptr);
    ASSERT_EQ(result, 1, "Server stop should succeed");
    
    TEST_PASS("Complete Server Lifecycle");
}

/**
 * Integration Test 2: Request Processing Pipeline
 */
void test_request_pipeline() {
    TEST_START("Request Processing Pipeline");
    
    // Simulate incoming request: GET /api/users/123?details=true&format=json
    const char* method = "GET";
    const char* path = "/api/users/123";
    const char* query = "details=true&format=json";
    
    // 1. Log request (middleware)
    int log_result = melp_web_log_request(method, path, query);
    ASSERT_EQ(log_result, 1, "Request logging should succeed");
    
    // 2. Check rate limit (middleware)
    int rate_ok = melp_web_check_rate_limit(path);
    ASSERT_EQ(rate_ok, 1, "Rate limit should allow request");
    
    // 3. Parse path parameter
    char* user_id = nullptr;
    int found = melp_web_parse_path_param("/api/users/:id", path, "id", &user_id);
    ASSERT_EQ(found, 1, "Should find user_id parameter");
    ASSERT_TRUE(strcmp(user_id, "123") == 0, "User ID should be 123");
    
    // 4. Parse query parameters
    char* details = nullptr;
    found = melp_web_parse_query_param(query, "details", &details);
    ASSERT_EQ(found, 1, "Should find details parameter");
    ASSERT_TRUE(strcmp(details, "true") == 0, "Details should be true");
    
    char* format = nullptr;
    found = melp_web_parse_query_param(query, "format", &format);
    ASSERT_EQ(found, 1, "Should find format parameter");
    ASSERT_TRUE(strcmp(format, "json") == 0, "Format should be json");
    
    // 5. Add CORS headers (middleware)
    void* headers_ptr = nullptr;
    int cors_result = melp_web_add_cors_headers(headers_ptr);
    ASSERT_EQ(cors_result, 1, "CORS headers should be added");
    
    // Cleanup
    free(user_id);
    free(details);
    free(format);
    
    TEST_PASS("Request Processing Pipeline");
}

/**
 * Integration Test 3: REST API Flow
 */
void test_rest_api_flow() {
    TEST_START("REST API Flow");
    
    void* daemon_ptr = nullptr;
    int is_running = 0;
    
    // Setup server
    melp_web_create_server(10000, &daemon_ptr, &is_running);
    ASSERT_TRUE(daemon_ptr != nullptr, "Server should be created");
    
    // Register REST routes
    melp_web_register_route(daemon_ptr, "GET", "/api/todos", 10);
    melp_web_register_route(daemon_ptr, "POST", "/api/todos", 11);
    melp_web_register_route(daemon_ptr, "GET", "/api/todos/:id", 12);
    melp_web_register_route(daemon_ptr, "PUT", "/api/todos/:id", 13);
    melp_web_register_route(daemon_ptr, "DELETE", "/api/todos/:id", 14);
    
    // Simulate requests
    melp_web_log_request("GET", "/api/todos", "");
    melp_web_log_request("POST", "/api/todos", "");
    melp_web_log_request("GET", "/api/todos/1", "");
    melp_web_log_request("PUT", "/api/todos/1", "");
    melp_web_log_request("DELETE", "/api/todos/1", "");
    
    // Cleanup
    melp_web_server_stop(daemon_ptr);
    
    TEST_PASS("REST API Flow");
}

/**
 * Integration Test 4: Multiple Query Parameters
 */
void test_multiple_query_params() {
    TEST_START("Multiple Query Parameters");
    
    const char* query = "page=2&limit=10&sort=name&order=asc&filter=active";
    
    char* page = nullptr;
    melp_web_parse_query_param(query, "page", &page);
    ASSERT_TRUE(strcmp(page, "2") == 0, "Page should be 2");
    
    char* limit = nullptr;
    melp_web_parse_query_param(query, "limit", &limit);
    ASSERT_TRUE(strcmp(limit, "10") == 0, "Limit should be 10");
    
    char* sort = nullptr;
    melp_web_parse_query_param(query, "sort", &sort);
    ASSERT_TRUE(strcmp(sort, "name") == 0, "Sort should be name");
    
    char* order = nullptr;
    melp_web_parse_query_param(query, "order", &order);
    ASSERT_TRUE(strcmp(order, "asc") == 0, "Order should be asc");
    
    char* filter = nullptr;
    melp_web_parse_query_param(query, "filter", &filter);
    ASSERT_TRUE(strcmp(filter, "active") == 0, "Filter should be active");
    
    // Cleanup
    free(page);
    free(limit);
    free(sort);
    free(order);
    free(filter);
    
    TEST_PASS("Multiple Query Parameters");
}

/**
 * Integration Test 5: Complex Dynamic Routes
 */
void test_complex_routes() {
    TEST_START("Complex Dynamic Routes");
    
    // Test nested path parameters
    const char* pattern = "/api/users/:userId/posts/:postId/comments/:commentId";
    const char* actual = "/api/users/42/posts/789/comments/1001";
    
    char* user_id = nullptr;
    melp_web_parse_path_param(pattern, actual, "userId", &user_id);
    ASSERT_TRUE(strcmp(user_id, "42") == 0, "User ID should be 42");
    
    char* post_id = nullptr;
    melp_web_parse_path_param(pattern, actual, "postId", &post_id);
    ASSERT_TRUE(strcmp(post_id, "789") == 0, "Post ID should be 789");
    
    char* comment_id = nullptr;
    melp_web_parse_path_param(pattern, actual, "commentId", &comment_id);
    ASSERT_TRUE(strcmp(comment_id, "1001") == 0, "Comment ID should be 1001");
    
    // Cleanup
    free(user_id);
    free(post_id);
    free(comment_id);
    
    TEST_PASS("Complex Dynamic Routes");
}

/**
 * Main test runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.web Module - Integration Tests" << std::endl;
    std::cout << "Testing complete request/response cycles" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all integration tests
    test_server_lifecycle();
    test_request_pipeline();
    test_rest_api_flow();
    test_multiple_query_params();
    test_complex_routes();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Integration Test Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << tests_passed << std::endl;
    std::cout << "  ❌ Failed: " << tests_failed << std::endl;
    std::cout << "  📊 Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All integration tests passed!" << std::endl;
        std::cout << "std.web is production ready! 🚀\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some tests failed. Review errors above.\n" << std::endl;
        return 1;
    }
}
