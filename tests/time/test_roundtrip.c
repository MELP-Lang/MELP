// ============================================
// TEST: Round-trip Consistency
// ============================================
// Tests: Complete format/parse round-trip

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== Test 8: Round-trip Consistency ===\n\n");
    
    // Test 1: Format ISO -> Parse ISO -> Format ISO
    printf("Test 8.1: ISO round-trip\n");
    int64_t original = 1735689600;  // 2026-01-01 00:00:00 UTC
    
    char* formatted1 = mlp_time_format_iso(original);
    printf("Original timestamp: %lld\n", (long long)original);
    printf("Formatted: %s\n", formatted1);
    
    int64_t parsed = mlp_time_parse_iso(formatted1);
    printf("Parsed back: %lld\n", (long long)parsed);
    
    char* formatted2 = mlp_time_format_iso(parsed);
    printf("Formatted again: %s\n", formatted2);
    
    // Strings should match
    if (strcmp(formatted1, formatted2) == 0) {
        printf("✅ PASS: ISO round-trip consistent\n\n");
    } else {
        printf("❌ FAIL: ISO round-trip inconsistent\n\n");
        free(formatted1);
        free(formatted2);
        return 1;
    }
    
    free(formatted1);
    free(formatted2);
    
    // Test 2: Current time round-trip
    printf("Test 8.2: Current time round-trip\n");
    int64_t now = mlp_time_now();
    printf("Current timestamp: %lld\n", (long long)now);
    
    char* now_formatted = mlp_time_format_iso(now);
    printf("Formatted: %s\n", now_formatted);
    
    int64_t now_parsed = mlp_time_parse_iso(now_formatted);
    printf("Parsed: %lld\n", (long long)now_parsed);
    
    // Allow small difference due to rounding
    int64_t diff = now > now_parsed ? now - now_parsed : now_parsed - now;
    printf("Difference: %lld seconds\n", (long long)diff);
    
    if (diff <= 1) {
        printf("✅ PASS: Current time round-trip successful\n\n");
    } else {
        printf("❌ FAIL: Current time round-trip has too much difference\n\n");
        free(now_formatted);
        return 1;
    }
    
    free(now_formatted);
    
    // Test 3: Multiple timestamps
    printf("Test 8.3: Multiple timestamp round-trips\n");
    int64_t test_timestamps[] = {
        1735689600,   // 2026-01-01
        1767225600,   // 2026-12-31
        1704067200,   // 2024-01-01
        mlp_time_now()
    };
    int num_tests = 4;
    int passed = 0;
    
    for (int i = 0; i < num_tests; i++) {
        int64_t ts = test_timestamps[i];
        char* fmt = mlp_time_format_iso(ts);
        int64_t parsed_ts = mlp_time_parse_iso(fmt);
        
        printf("  Test %d: %lld -> %s -> %lld ",
               i+1, (long long)ts, fmt, (long long)parsed_ts);
        
        int64_t d = ts > parsed_ts ? ts - parsed_ts : parsed_ts - ts;
        if (d <= 1) {
            printf("✅\n");
            passed++;
        } else {
            printf("❌ (diff: %lld)\n", (long long)d);
        }
        
        free(fmt);
    }
    
    if (passed == num_tests) {
        printf("✅ PASS: All round-trips successful (%d/%d)\n\n",
               passed, num_tests);
    } else {
        printf("❌ FAIL: Some round-trips failed (%d/%d)\n\n",
               passed, num_tests);
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
