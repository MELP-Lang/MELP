// ============================================
// TEST: Time Now - Get Current Timestamp
// ============================================
// Tests: mlp_time_now(), mlp_time_now_millis(), mlp_time_now_micros()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 1: Time Now ===\n\n");
    
    // Test 1: Get current timestamp in seconds
    printf("Test 1.1: mlp_time_now()\n");
    int64_t now_sec = mlp_time_now();
    printf("Current timestamp (seconds): %lld\n", (long long)now_sec);
    if (now_sec > 1704067200) {  // After 2024-01-01
        printf("✅ PASS: Valid timestamp\n\n");
    } else {
        printf("❌ FAIL: Invalid timestamp\n\n");
        return 1;
    }
    
    // Test 2: Get current timestamp in milliseconds
    printf("Test 1.2: mlp_time_now_millis()\n");
    int64_t now_millis = mlp_time_now_millis();
    printf("Current timestamp (milliseconds): %lld\n", (long long)now_millis);
    if (now_millis > now_sec * 1000) {
        printf("✅ PASS: Valid millisecond timestamp\n\n");
    } else {
        printf("❌ FAIL: Invalid millisecond timestamp\n\n");
        return 1;
    }
    
    // Test 3: Get current timestamp in microseconds
    printf("Test 1.3: mlp_time_now_micros()\n");
    int64_t now_micros = mlp_time_now_micros();
    printf("Current timestamp (microseconds): %lld\n", (long long)now_micros);
    if (now_micros > now_sec * 1000000) {
        printf("✅ PASS: Valid microsecond timestamp\n\n");
    } else {
        printf("❌ FAIL: Invalid microsecond timestamp\n\n");
        return 1;
    }
    
    // Test 4: Check precision relationship
    printf("Test 1.4: Precision relationship\n");
    int64_t sec_diff = now_millis / 1000 - now_sec;
    printf("Seconds from millis: %lld (diff: %lld)\n", 
           (long long)(now_millis / 1000), (long long)sec_diff);
    if (sec_diff >= 0 && sec_diff < 2) {  // Allow 1 second difference due to timing
        printf("✅ PASS: Milliseconds consistent with seconds\n\n");
    } else {
        printf("❌ FAIL: Milliseconds inconsistent\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
