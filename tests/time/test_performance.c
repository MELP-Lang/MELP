// ============================================
// TEST: Performance Benchmark
// ============================================
// Tests: Performance of time operations

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 10: Performance Benchmark ===\n\n");
    
    // Test 1: now() performance (should be <1µs per call)
    printf("Test 10.1: mlp_time_now() performance\n");
    int iterations = 10000;
    int64_t start = mlp_time_now_micros();
    
    for (int i = 0; i < iterations; i++) {
        mlp_time_now();
    }
    
    int64_t end = mlp_time_now_micros();
    int64_t elapsed = end - start;
    double avg_time = (double)elapsed / iterations;
    
    printf("Iterations: %d\n", iterations);
    printf("Total time: %lld µs\n", (long long)elapsed);
    printf("Average: %.2f µs per call\n", avg_time);
    
    if (avg_time < 10.0) {  // Should be much faster, but allow some overhead
        printf("✅ PASS: now() performance acceptable\n\n");
    } else {
        printf("❌ FAIL: now() too slow (>10µs per call)\n\n");
        return 1;
    }
    
    // Test 2: Datetime conversion performance
    printf("Test 10.2: Datetime conversion performance\n");
    int64_t timestamp = mlp_time_now();
    start = mlp_time_now_micros();
    
    for (int i = 0; i < iterations; i++) {
        mlp_time_to_datetime_utc(timestamp);
    }
    
    end = mlp_time_now_micros();
    elapsed = end - start;
    avg_time = (double)elapsed / iterations;
    
    printf("Iterations: %d\n", iterations);
    printf("Total time: %lld µs\n", (long long)elapsed);
    printf("Average: %.2f µs per conversion\n", avg_time);
    
    if (avg_time < 100.0) {
        printf("✅ PASS: Datetime conversion performance acceptable\n\n");
    } else {
        printf("❌ FAIL: Datetime conversion too slow\n\n");
        return 1;
    }
    
    // Test 3: Format performance
    printf("Test 10.3: Format performance\n");
    start = mlp_time_now_micros();
    
    for (int i = 0; i < 1000; i++) {
        char* formatted = mlp_time_format_iso(timestamp);
        free(formatted);
    }
    
    end = mlp_time_now_micros();
    elapsed = end - start;
    avg_time = (double)elapsed / 1000;
    
    printf("Iterations: 1000\n");
    printf("Total time: %lld µs\n", (long long)elapsed);
    printf("Average: %.2f µs per format\n", avg_time);
    
    if (avg_time < 1000.0) {  // <1ms per format
        printf("✅ PASS: Format performance acceptable\n\n");
    } else {
        printf("❌ FAIL: Format too slow\n\n");
        return 1;
    }
    
    // Test 4: Parse performance
    printf("Test 10.4: Parse performance\n");
    const char* test_str = "2026-01-01 12:30:45";
    start = mlp_time_now_micros();
    
    for (int i = 0; i < 1000; i++) {
        mlp_time_parse_iso(test_str);
    }
    
    end = mlp_time_now_micros();
    elapsed = end - start;
    avg_time = (double)elapsed / 1000;
    
    printf("Iterations: 1000\n");
    printf("Total time: %lld µs\n", (long long)elapsed);
    printf("Average: %.2f µs per parse\n", avg_time);
    
    if (avg_time < 1000.0) {  // <1ms per parse
        printf("✅ PASS: Parse performance acceptable\n\n");
    } else {
        printf("❌ FAIL: Parse too slow\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
