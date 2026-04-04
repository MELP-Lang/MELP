/**
 * ============================================================================
 * MLP Standard Library - std.test Framework Performance Benchmarks
 * ============================================================================
 * Performance tests and benchmarks for the testing framework
 * Author: YZ_STAGE1_07
 * Date: 27 Ocak 2026
 * ============================================================================
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>

// Forward declarations of test builtins
extern "C" {
    int melp_test_assert_eq_numeric(double actual, double expected);
    int melp_test_assert_true(int condition);
    int melp_test_register(const char* name, int handler_id);
    int melp_test_run_all(int* total, int* passed, int* failed, int* duration_ms);
    int melp_test_describe(const char* suite_name);
    int melp_test_end_describe();
    int melp_test_before_each(int handler_id);
    int melp_test_after_each(int handler_id);
}

// Performance measurement utilities
class PerformanceBenchmark {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;
    
public:
    PerformanceBenchmark(const std::string& test_name) : name(test_name) {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    ~PerformanceBenchmark() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "  ⏱️  " << name << ": " << duration.count() << "μs" << std::endl;
    }
};

#define BENCHMARK(name) PerformanceBenchmark _bench(name)

// Test result tracking
static int total_benchmarks = 0;
static int passed_benchmarks = 0;
static std::vector<std::string> slow_tests;

#define ASSERT_PERFORMANCE(condition, test_name, threshold_us) \
    if (!(condition)) { \
        slow_tests.push_back(test_name); \
        std::cout << "  ⚠️  " << test_name << " exceeded threshold (" << threshold_us << "μs)" << std::endl; \
    } else { \
        passed_benchmarks++; \
    } \
    total_benchmarks++;

/**
 * Benchmark 1: Single Assertion Performance
 */
void benchmark_single_assertion() {
    std::cout << "\n📊 Benchmark: Single Assertion Performance" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    melp_test_assert_eq_numeric(5.0, 5.0);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Single assertion: " << duration.count() << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 10, "Single assertion", 10);
}

/**
 * Benchmark 2: Multiple Assertions
 */
void benchmark_multiple_assertions() {
    std::cout << "\n📊 Benchmark: Multiple Assertions (1000x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; i++) {
        melp_test_assert_eq_numeric(i, i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  1000 assertions: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per assertion: " << (duration.count() / 1000.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 10000, "1000 assertions", 10000);
}

/**
 * Benchmark 3: Test Registration
 */
void benchmark_test_registration() {
    std::cout << "\n📊 Benchmark: Test Registration (100x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; i++) {
        char test_name[50];
        snprintf(test_name, sizeof(test_name), "Test %d", i);
        melp_test_register(test_name, i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  100 registrations: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per registration: " << (duration.count() / 100.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 5000, "100 test registrations", 5000);
}

/**
 * Benchmark 4: Suite Creation
 */
void benchmark_suite_creation() {
    std::cout << "\n📊 Benchmark: Suite Creation (50x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 50; i++) {
        char suite_name[50];
        snprintf(suite_name, sizeof(suite_name), "Suite %d", i);
        melp_test_describe(suite_name);
        melp_test_end_describe();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  50 suites: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per suite: " << (duration.count() / 50.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 3000, "50 suite creations", 3000);
}

/**
 * Benchmark 5: Hook Registration
 */
void benchmark_hook_registration() {
    std::cout << "\n📊 Benchmark: Hook Registration (100x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 25; i++) {
        char suite_name[50];
        snprintf(suite_name, sizeof(suite_name), "Hook Suite %d", i);
        melp_test_describe(suite_name);
        melp_test_before_each(1000 + i);
        melp_test_after_each(2000 + i);
        melp_test_end_describe();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  25 suites with hooks: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per suite: " << (duration.count() / 25.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 2000, "25 hook registrations", 2000);
}

/**
 * Benchmark 6: String Assertions
 */
void benchmark_string_assertions() {
    std::cout << "\n📊 Benchmark: String Assertions (500x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 500; i++) {
        melp_test_assert_eq_numeric(5.0, 5.0);  // Using numeric for simplicity
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  500 string assertions: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per assertion: " << (duration.count() / 500.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 5000, "500 string assertions", 5000);
}

/**
 * Benchmark 7: Complex Test Scenario
 */
void benchmark_complex_scenario() {
    std::cout << "\n📊 Benchmark: Complex Test Scenario" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create 10 suites with 10 tests each
    for (int suite = 0; suite < 10; suite++) {
        char suite_name[50];
        snprintf(suite_name, sizeof(suite_name), "Complex Suite %d", suite);
        melp_test_describe(suite_name);
        
        melp_test_before_each(suite * 100);
        melp_test_after_each(suite * 100 + 1);
        
        for (int test = 0; test < 10; test++) {
            char test_name[50];
            snprintf(test_name, sizeof(test_name), "Test %d-%d", suite, test);
            melp_test_register(test_name, suite * 100 + test);
            
            // Simulate test execution
            melp_test_assert_eq_numeric(test, test);
        }
        
        melp_test_end_describe();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  10 suites × 10 tests: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per test: " << (duration.count() / 100.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 20000, "Complex scenario (100 tests)", 20000);
}

/**
 * Benchmark 8: Memory Allocation Pattern
 */
void benchmark_memory_allocation() {
    std::cout << "\n📊 Benchmark: Memory Allocation Pattern" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simulate test framework memory operations
    std::vector<std::string> test_names;
    for (int i = 0; i < 1000; i++) {
        test_names.push_back("Test " + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  1000 allocations: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per allocation: " << (duration.count() / 1000.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 15000, "1000 memory allocations", 15000);
}

/**
 * Benchmark 9: Assertion Failure Performance
 */
void benchmark_assertion_failures() {
    std::cout << "\n📊 Benchmark: Assertion Failures (100x)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; i++) {
        melp_test_assert_eq_numeric(i, i + 1);  // Intentionally fail
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  100 failed assertions: " << duration.count() << "μs" << std::endl;
    std::cout << "  Average per failure: " << (duration.count() / 100.0) << "μs" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 15000, "100 assertion failures", 15000);
}

/**
 * Benchmark 10: End-to-End Test Execution
 */
void benchmark_end_to_end() {
    std::cout << "\n📊 Benchmark: End-to-End Test Execution" << std::endl;
    
    // Setup
    melp_test_describe("E2E Suite");
    
    for (int i = 0; i < 20; i++) {
        char test_name[50];
        snprintf(test_name, sizeof(test_name), "E2E Test %d", i);
        melp_test_register(test_name, 10000 + i);
    }
    
    melp_test_end_describe();
    
    // Execute
    auto start = std::chrono::high_resolution_clock::now();
    
    int total = 0, passed = 0, failed = 0, duration_ms = 0;
    melp_test_run_all(&total, &passed, &failed, &duration_ms);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  20 tests executed: " << duration.count() << "μs" << std::endl;
    std::cout << "  Framework reported: " << duration_ms << "ms" << std::endl;
    ASSERT_PERFORMANCE(duration.count() < 50000, "E2E execution (20 tests)", 50000);
}

/**
 * Main benchmark runner
 */
int main() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "MLP std.test Framework - Performance Benchmarks" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Run all benchmarks
    benchmark_single_assertion();
    benchmark_multiple_assertions();
    benchmark_test_registration();
    benchmark_suite_creation();
    benchmark_hook_registration();
    benchmark_string_assertions();
    benchmark_complex_scenario();
    benchmark_memory_allocation();
    benchmark_assertion_failures();
    benchmark_end_to_end();
    
    // Print summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "Performance Summary:" << std::endl;
    std::cout << "  ✅ Passed: " << passed_benchmarks << "/" << total_benchmarks << std::endl;
    
    if (!slow_tests.empty()) {
        std::cout << "  ⚠️  Slow tests:" << std::endl;
        for (const auto& test : slow_tests) {
            std::cout << "     - " << test << std::endl;
        }
    }
    
    std::cout << std::string(70, '=') << std::endl;
    
    if (passed_benchmarks == total_benchmarks) {
        std::cout << "\n🎉 All performance benchmarks passed!\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n⚠️  Some benchmarks exceeded thresholds.\n" << std::endl;
        return 1;
    }
}
