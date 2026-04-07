// ============================================================================
// Test Suite: std.cli C++ Builtins
// ============================================================================
// Tests for CLI module builtin functions
// Author: YZ_STAGE1_04
// Date: 27 Ocak 2026
// ============================================================================

#include "builtins.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>

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
// TEST GROUP 1: Argument Parsing
//=============================================================================

void test_argument_parsing() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 1: Argument Parsing" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Parse basic flags") {
        const char* argv[] = {"program", "--verbose", "--debug"};
        int argc = 3;
        char* arg_array[3];
        for (int i = 0; i < 3; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        int result = melp_cli_parse_args(argc, arg_array);
        
        ASSERT_EQ(2, result, "Should parse 2 arguments");
        ASSERT_EQ(1, melp_cli_get_flag("verbose"), "verbose flag should be present");
        ASSERT_EQ(1, melp_cli_get_flag("debug"), "debug flag should be present");
        ASSERT_EQ(0, melp_cli_get_flag("nonexistent"), "nonexistent flag should not be present");
    } END_TEST
    
    TEST("Parse options with values") {
        const char* argv[] = {"program", "--output=file.txt", "--count=42"};
        int argc = 3;
        char* arg_array[3];
        for (int i = 0; i < 3; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        melp_cli_parse_args(argc, arg_array);
        
        char* output = melp_cli_get_option("output", "default.txt");
        char* count = melp_cli_get_option("count", "0");
        char* missing = melp_cli_get_option("missing", "default");
        
        ASSERT_STR_EQ("file.txt", output, "output should be file.txt");
        ASSERT_STR_EQ("42", count, "count should be 42");
        ASSERT_STR_EQ("default", missing, "missing should return default");
        
        delete[] output;
        delete[] count;
        delete[] missing;
    } END_TEST
    
    TEST("Parse positional arguments") {
        const char* argv[] = {"program", "file1.txt", "file2.txt", "--flag", "file3.txt"};
        int argc = 5;
        char* arg_array[5];
        for (int i = 0; i < 5; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        melp_cli_parse_args(argc, arg_array);
        
        ASSERT_EQ(3, melp_cli_positional_count(), "Should have 3 positional arguments");
        
        char* pos0 = melp_cli_get_positional(0);
        char* pos1 = melp_cli_get_positional(1);
        char* pos2 = melp_cli_get_positional(2);
        
        ASSERT_STR_EQ("file1.txt", pos0, "First positional should be file1.txt");
        ASSERT_STR_EQ("file2.txt", pos1, "Second positional should be file2.txt");
        ASSERT_STR_EQ("file3.txt", pos2, "Third positional should be file3.txt");
        
        delete[] pos0;
        delete[] pos1;
        delete[] pos2;
    } END_TEST
    
    TEST("Parse mixed arguments") {
        const char* argv[] = {"program", "--verbose", "input.txt", "--output=result.txt", "extra.txt", "-d"};
        int argc = 6;
        char* arg_array[6];
        for (int i = 0; i < 6; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        melp_cli_parse_args(argc, arg_array);
        
        ASSERT_EQ(1, melp_cli_get_flag("verbose"), "verbose flag should be present");
        ASSERT_EQ(1, melp_cli_get_flag("d"), "d flag should be present");
        ASSERT_EQ(2, melp_cli_positional_count(), "Should have 2 positional arguments");
        
        char* output = melp_cli_get_option("output", "");
        ASSERT_STR_EQ("result.txt", output, "output should be result.txt");
        delete[] output;
    } END_TEST
    
    TEST("Parse short flags") {
        const char* argv[] = {"program", "-v", "-d", "-h"};
        int argc = 4;
        char* arg_array[4];
        for (int i = 0; i < 4; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        melp_cli_parse_args(argc, arg_array);
        
        ASSERT_EQ(1, melp_cli_get_flag("v"), "v flag should be present");
        ASSERT_EQ(1, melp_cli_get_flag("d"), "d flag should be present");
        ASSERT_EQ(1, melp_cli_get_flag("h"), "h flag should be present");
    } END_TEST
}

//=============================================================================
// TEST GROUP 2: Output Formatting
//=============================================================================

void test_output_formatting() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 2: Output Formatting" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Print colored text") {
        // This is a visual test, just check it doesn't crash
        int result = melp_cli_print_colored("Test red text", "red");
        ASSERT_EQ(0, result, "print_colored should return 0");
        
        result = melp_cli_print_colored(" | Test green text", "green");
        ASSERT_EQ(0, result, "print_colored should return 0");
        
        result = melp_cli_print_colored(" | Test blue text\n", "blue");
        ASSERT_EQ(0, result, "print_colored should return 0");
    } END_TEST
    
    TEST("Print separator") {
        int result = melp_cli_print_separator("-", 40);
        ASSERT_EQ(0, result, "print_separator should return 0");
        
        result = melp_cli_print_separator("=", 40);
        ASSERT_EQ(0, result, "print_separator should return 0");
    } END_TEST
    
    TEST("Progress bar") {
        std::cout << "\nProgress bar test:\n";
        for (int i = 0; i <= 10; i++) {
            melp_cli_progress_bar(i, 10, 50);
            // In real test, we'd wait a bit
        }
        std::cout << "\n";
        
        int result = melp_cli_progress_bar(50, 100, 30);
        ASSERT_EQ(0, result, "progress_bar should return 0");
        std::cout << "\n";
    } END_TEST
    
    TEST("Print table") {
        char* headers[] = {
            const_cast<char*>("Name"),
            const_cast<char*>("Age"),
            const_cast<char*>("City")
        };
        
        char* rows[] = {
            const_cast<char*>("Alice"),
            const_cast<char*>("30"),
            const_cast<char*>("New York"),
            const_cast<char*>("Bob"),
            const_cast<char*>("25"),
            const_cast<char*>("London")
        };
        
        int result = melp_cli_print_table(headers, rows, 2, 3);
        ASSERT_EQ(0, result, "print_table should return 0");
    } END_TEST
}

//=============================================================================
// TEST GROUP 3: Process Control
//=============================================================================

void test_process_control() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 3: Process Control" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Get environment variable") {
        // Set a test env var first
        melp_cli_set_env("TEST_VAR", "test_value");
        
        char* value = melp_cli_get_env("TEST_VAR");
        ASSERT_STR_EQ("test_value", value, "TEST_VAR should be test_value");
        delete[] value;
        
        // Try non-existent var
        char* missing = melp_cli_get_env("NONEXISTENT_VAR_12345");
        ASSERT_STR_EQ("", missing, "Non-existent var should return empty string");
        delete[] missing;
    } END_TEST
    
    TEST("Set environment variable") {
        int result = melp_cli_set_env("MY_TEST_VAR", "my_value");
        ASSERT_EQ(1, result, "set_env should return 1 on success");
        
        char* value = melp_cli_get_env("MY_TEST_VAR");
        ASSERT_STR_EQ("my_value", value, "MY_TEST_VAR should be my_value");
        delete[] value;
    } END_TEST
    
    TEST("Get current working directory") {
        char* cwd = melp_cli_get_cwd();
        ASSERT_TRUE(strlen(cwd) > 0, "cwd should not be empty");
        std::cout << "Current working directory: " << cwd << std::endl;
        delete[] cwd;
    } END_TEST
    
    TEST("Check if TTY") {
        int is_tty = melp_cli_is_tty();
        std::cout << "Is TTY: " << is_tty << std::endl;
        // Just check it returns 0 or 1
        ASSERT_TRUE(is_tty == 0 || is_tty == 1, "is_tty should return 0 or 1");
    } END_TEST
    
    TEST("Clear screen (visual test)") {
        // This is a visual test
        int result = melp_cli_clear_screen();
        ASSERT_EQ(0, result, "clear_screen should return 0");
    } END_TEST
}

//=============================================================================
// TEST GROUP 4: Edge Cases
//=============================================================================

void test_edge_cases() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST GROUP 4: Edge Cases" << std::endl;
    std::cout << "========================================" << std::endl;
    
    TEST("Empty argument list") {
        const char* argv[] = {"program"};
        int argc = 1;
        char* arg_array[1];
        arg_array[0] = const_cast<char*>(argv[0]);
        
        int result = melp_cli_parse_args(argc, arg_array);
        
        ASSERT_EQ(0, result, "Should parse 0 arguments");
        ASSERT_EQ(0, melp_cli_positional_count(), "Should have 0 positional arguments");
    } END_TEST
    
    TEST("Get positional out of bounds") {
        const char* argv[] = {"program"};
        int argc = 1;
        char* arg_array[1];
        arg_array[0] = const_cast<char*>(argv[0]);
        
        melp_cli_parse_args(argc, arg_array);
        
        char* pos = melp_cli_get_positional(99);
        ASSERT_STR_EQ("", pos, "Out of bounds positional should return empty string");
        delete[] pos;
    } END_TEST
    
    TEST("Option with empty value") {
        const char* argv[] = {"program", "--empty="};
        int argc = 2;
        char* arg_array[2];
        for (int i = 0; i < 2; i++) {
            arg_array[i] = const_cast<char*>(argv[i]);
        }
        
        melp_cli_parse_args(argc, arg_array);
        
        char* value = melp_cli_get_option("empty", "default");
        ASSERT_STR_EQ("", value, "Empty option value should be empty string");
        delete[] value;
    } END_TEST
    
    TEST("Progress bar edge cases") {
        // Test 0%
        int result = melp_cli_progress_bar(0, 100, 20);
        ASSERT_EQ(0, result, "progress_bar at 0% should work");
        std::cout << "\n";
        
        // Test 100%
        result = melp_cli_progress_bar(100, 100, 20);
        ASSERT_EQ(0, result, "progress_bar at 100% should work");
        std::cout << "\n";
        
        // Test over 100%
        result = melp_cli_progress_bar(150, 100, 20);
        ASSERT_EQ(0, result, "progress_bar over 100% should clamp");
        std::cout << "\n";
    } END_TEST
}

//=============================================================================
// MAIN
//=============================================================================

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  std.cli Builtin Functions Test Suite" << std::endl;
    std::cout << "============================================" << std::endl;
    
    test_argument_parsing();
    test_output_formatting();
    test_process_control();
    test_edge_cases();
    
    std::cout << "\n============================================" << std::endl;
    std::cout << "         TEST SUMMARY" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Tests Passed: " << tests_passed << std::endl;
    std::cout << "❌ Tests Failed: " << tests_failed << std::endl;
    std::cout << "📊 Total Tests:  " << (tests_passed + tests_failed) << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  SOME TESTS FAILED!" << std::endl;
        return 1;
    }
}
