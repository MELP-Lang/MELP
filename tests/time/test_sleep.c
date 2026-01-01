// ============================================
// TEST: Sleep Operations
// ============================================
// Tests: mlp_time_sleep(), mlp_time_sleep_millis()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 2: Sleep Operations ===\n\n");
    
    // Test 1: Sleep for 1 second
    printf("Test 2.1: mlp_time_sleep(1)\n");
    int64_t start_sec = mlp_time_now();
    int result = mlp_time_sleep(1);
    int64_t end_sec = mlp_time_now();
    int64_t elapsed = end_sec - start_sec;
    
    printf("Start: %lld, End: %lld, Elapsed: %lld seconds\n",
           (long long)start_sec, (long long)end_sec, (long long)elapsed);
    
    if (result == 0 && elapsed >= 1 && elapsed <= 2) {
        printf("✅ PASS: Sleep for 1 second\n\n");
    } else {
        printf("❌ FAIL: Sleep duration incorrect (expected ~1s, got %lld)\n\n", 
               (long long)elapsed);
        return 1;
    }
    
    // Test 2: Sleep for 500 milliseconds
    printf("Test 2.2: mlp_time_sleep_millis(500)\n");
    int64_t start_millis = mlp_time_now_millis();
    result = mlp_time_sleep_millis(500);
    int64_t end_millis = mlp_time_now_millis();
    int64_t elapsed_millis = end_millis - start_millis;
    
    printf("Start: %lld, End: %lld, Elapsed: %lld ms\n",
           (long long)start_millis, (long long)end_millis, (long long)elapsed_millis);
    
    if (result == 0 && elapsed_millis >= 500 && elapsed_millis <= 600) {
        printf("✅ PASS: Sleep for 500 milliseconds\n\n");
    } else {
        printf("❌ FAIL: Sleep duration incorrect (expected ~500ms, got %lld)\n\n",
               (long long)elapsed_millis);
        return 1;
    }
    
    // Test 3: Error handling - negative sleep
    printf("Test 2.3: mlp_time_sleep(-1) - Error handling\n");
    result = mlp_time_sleep(-1);
    if (result == -1) {
        printf("✅ PASS: Negative sleep returns error\n\n");
    } else {
        printf("❌ FAIL: Should return -1 for negative value\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
