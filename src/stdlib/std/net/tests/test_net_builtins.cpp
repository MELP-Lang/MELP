// ============================================================================
// Test Suite: std.net C++ Builtins
// ============================================================================
// Tests for network module builtin functions
// Author: YZ_STAGE1_03
// Date: 30 Ocak 2026
// ============================================================================

#include "builtins.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
#include <string>

using namespace melp_runtime;

// Test counters
int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) \
    std::cout << "\n🧪 TEST: " << name << std::endl; \
    try {

#define END_TEST \
        tests_passed++; \
        std::cout << "✅ PASSED" << std::endl; \
    } catch (const std::exception& e) { \
        tests_failed++; \
        std::cout << "❌ FAILED: " << e.what() << std::endl; \
    }

#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        throw std::runtime_error(message); \
    }

#define ASSERT_EQ(expected, actual, message) \
    if ((expected) != (actual)) { \
        throw std::runtime_error(std::string(message) + " (expected: " + std::to_string(expected) + ", got: " + std::to_string(actual) + ")"); \
    }

#define ASSERT_STR_EQ(expected, actual, message) \
    if (strcmp(expected, actual) != 0) { \
        throw std::runtime_error(std::string(message) + " (expected: " + expected + ", got: " + actual + ")"); \
    }

//=============================================================================
// TEST GROUP 1: URL Parsing
//=============================================================================

void test_url_parsing() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 1: URL Parsing" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Parse simple HTTP URL") {
        char* scheme = nullptr;
        char* host = nullptr;
        int port = 0;
        char* path = nullptr;
        char* query = nullptr;
        
        int result = net_parse_url("http://example.com/test", &scheme, &host, &port, &path, &query);
        
        ASSERT_EQ(1, result, "parse_url should succeed");
        ASSERT_STR_EQ("http", scheme, "scheme should be http");
        ASSERT_STR_EQ("example.com", host, "host should be example.com");
        ASSERT_EQ(80, port, "port should be 80 (default)");
        ASSERT_STR_EQ("/test", path, "path should be /test");
        ASSERT_STR_EQ("", query, "query should be empty");
        
        free(scheme);
        free(host);
        free(path);
        free(query);
    } END_TEST
    
    TEST("Parse HTTPS URL with port") {
        char* scheme = nullptr;
        char* host = nullptr;
        int port = 0;
        char* path = nullptr;
        char* query = nullptr;
        
        int result = net_parse_url("https://api.example.com:8080/v1/users", &scheme, &host, &port, &path, &query);
        
        ASSERT_EQ(1, result, "parse_url should succeed");
        ASSERT_STR_EQ("https", scheme, "scheme should be https");
        ASSERT_STR_EQ("api.example.com", host, "host should be api.example.com");
        ASSERT_EQ(8080, port, "port should be 8080");
        ASSERT_STR_EQ("/v1/users", path, "path should be /v1/users");
        ASSERT_STR_EQ("", query, "query should be empty");
        
        free(scheme);
        free(host);
        free(path);
        free(query);
    } END_TEST
    
    TEST("Parse URL with query string") {
        char* scheme = nullptr;
        char* host = nullptr;
        int port = 0;
        char* path = nullptr;
        char* query = nullptr;
        
        int result = net_parse_url("https://example.com/search?q=test&page=1", &scheme, &host, &port, &path, &query);
        
        ASSERT_EQ(1, result, "parse_url should succeed");
        ASSERT_STR_EQ("https", scheme, "scheme should be https");
        ASSERT_STR_EQ("example.com", host, "host should be example.com");
        ASSERT_EQ(443, port, "port should be 443 (default HTTPS)");
        ASSERT_STR_EQ("/search", path, "path should be /search");
        ASSERT_STR_EQ("q=test&page=1", query, "query should be q=test&page=1");
        
        free(scheme);
        free(host);
        free(path);
        free(query);
    } END_TEST
    
    TEST("Parse URL without path") {
        char* scheme = nullptr;
        char* host = nullptr;
        int port = 0;
        char* path = nullptr;
        char* query = nullptr;
        
        int result = net_parse_url("http://example.com", &scheme, &host, &port, &path, &query);
        
        ASSERT_EQ(1, result, "parse_url should succeed");
        ASSERT_STR_EQ("http", scheme, "scheme should be http");
        ASSERT_STR_EQ("example.com", host, "host should be example.com");
        ASSERT_EQ(80, port, "port should be 80");
        ASSERT_STR_EQ("/", path, "path should be /");
        ASSERT_STR_EQ("", query, "query should be empty");
        
        free(scheme);
        free(host);
        free(path);
        free(query);
    } END_TEST
    
    TEST("Parse URL with complex path and query") {
        char* scheme = nullptr;
        char* host = nullptr;
        int port = 0;
        char* path = nullptr;
        char* query = nullptr;
        
        int result = net_parse_url("https://api.github.com:443/repos/user/project/issues?state=open&labels=bug", 
                                   &scheme, &host, &port, &path, &query);
        
        ASSERT_EQ(1, result, "parse_url should succeed");
        ASSERT_STR_EQ("https", scheme, "scheme should be https");
        ASSERT_STR_EQ("api.github.com", host, "host should be api.github.com");
        ASSERT_EQ(443, port, "port should be 443");
        ASSERT_STR_EQ("/repos/user/project/issues", path, "path should match");
        ASSERT_STR_EQ("state=open&labels=bug", query, "query should match");
        
        free(scheme);
        free(host);
        free(path);
        free(query);
    } END_TEST
}

//=============================================================================
// TEST GROUP 2: HTTP GET (requires network connection)
//=============================================================================

void test_http_get() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 2: HTTP GET" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("HTTP GET to httpbin.org/get") {
        int status = 0;
        char* body = net_http_get("http://httpbin.org/get", &status);
        
        ASSERT_EQ(200, status, "HTTP status should be 200");
        ASSERT_TRUE(body != nullptr, "body should not be null");
        ASSERT_TRUE(strlen(body) > 0, "body should not be empty");
        
        // Check if response contains expected JSON keys
        std::string body_str(body);
        ASSERT_TRUE(body_str.find("\"url\"") != std::string::npos, "response should contain url field");
        
        free(body);
    } END_TEST
    
    TEST("HTTP GET with 404 error") {
        int status = 0;
        char* body = net_http_get("http://httpbin.org/status/404", &status);
        
        ASSERT_EQ(404, status, "HTTP status should be 404");
        ASSERT_TRUE(body != nullptr, "body should not be null");
        
        free(body);
    } END_TEST
    
    TEST("HTTPS GET to httpbin.org/get") {
        int status = 0;
        char* body = net_http_get("https://httpbin.org/get", &status);
        
        ASSERT_EQ(200, status, "HTTP status should be 200");
        ASSERT_TRUE(body != nullptr, "body should not be null");
        ASSERT_TRUE(strlen(body) > 0, "body should not be empty");
        
        free(body);
    } END_TEST
}

//=============================================================================
// TEST GROUP 3: HTTP POST (requires network connection)
//=============================================================================

void test_http_post() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 3: HTTP POST" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("HTTP POST with JSON body") {
        int status = 0;
        const char* json_body = "{\"key\":\"value\",\"number\":42}";
        char* response = net_http_post("http://httpbin.org/post", json_body, &status);
        
        ASSERT_EQ(200, status, "HTTP status should be 200");
        ASSERT_TRUE(response != nullptr, "response should not be null");
        ASSERT_TRUE(strlen(response) > 0, "response should not be empty");
        
        // Check if response contains our posted data
        std::string response_str(response);
        ASSERT_TRUE(response_str.find("\"key\"") != std::string::npos, "response should contain our key");
        ASSERT_TRUE(response_str.find("\"value\"") != std::string::npos, "response should contain our value");
        
        free(response);
    } END_TEST
    
    TEST("HTTPS POST to httpbin.org/post") {
        int status = 0;
        const char* json_body = "{\"test\":\"data\"}";
        char* response = net_http_post("https://httpbin.org/post", json_body, &status);
        
        ASSERT_EQ(200, status, "HTTP status should be 200");
        ASSERT_TRUE(response != nullptr, "response should not be null");
        ASSERT_TRUE(strlen(response) > 0, "response should not be empty");
        
        free(response);
    } END_TEST
}

//=============================================================================
// TEST GROUP 4: Edge Cases and Error Handling
//=============================================================================

void test_edge_cases() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 4: Edge Cases" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Invalid URL - null parameter") {
        int status = 0;
        char* body = net_http_get(nullptr, &status);
        
        ASSERT_EQ(0, status, "status should be 0 for null URL");
        ASSERT_TRUE(body != nullptr, "body should not be null (empty string)");
        ASSERT_EQ(0, strlen(body), "body should be empty");
        
        free(body);
    } END_TEST
    
    TEST("Invalid URL - malformed URL") {
        int status = 0;
        char* body = net_http_get("not-a-valid-url", &status);
        
        ASSERT_EQ(0, status, "status should be 0 for invalid URL");
        ASSERT_TRUE(body != nullptr, "body should not be null");
        
        free(body);
    } END_TEST
    
    TEST("Connection timeout/failure") {
        int status = 0;
        // Using a non-existent host (should timeout or fail)
        char* body = net_http_get("http://this-domain-does-not-exist-12345.com", &status);
        
        ASSERT_EQ(0, status, "status should be 0 for connection failure");
        ASSERT_TRUE(body != nullptr, "body should not be null");
        
        free(body);
    } END_TEST
}

//=============================================================================
// MAIN
//=============================================================================

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "🌐 MLP std.net C++ Builtin Tests" << std::endl;
    std::cout << "============================================" << std::endl;
    
    test_url_parsing();
    
    std::cout << "\n⚠️  Network tests (HTTP GET/POST) require internet connection." << std::endl;
    std::cout << "If tests fail, check your network connection." << std::endl;
    
    test_http_get();
    test_http_post();
    test_edge_cases();
    
    // Summary
    std::cout << "\n============================================" << std::endl;
    std::cout << "📊 TEST SUMMARY" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Tests Passed: " << tests_passed << std::endl;
    std::cout << "❌ Tests Failed: " << tests_failed << std::endl;
    std::cout << "📈 Total Tests:  " << (tests_passed + tests_failed) << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  SOME TESTS FAILED!" << std::endl;
        return 1;
    }
}
