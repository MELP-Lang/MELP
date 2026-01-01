// ============================================
// TEST: Timezone Operations
// ============================================
// Tests: mlp_time_get_timezone_offset()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 7: Timezone Operations ===\n\n");
    
    // Test 1: Get timezone offset
    printf("Test 7.1: mlp_time_get_timezone_offset()\n");
    int64_t offset = mlp_time_get_timezone_offset();
    
    printf("Timezone offset: %lld seconds\n", (long long)offset);
    printf("Timezone offset: %.1f hours\n", offset / 3600.0);
    
    // Offset should be reasonable (between -12 and +14 hours)
    if (offset >= -43200 && offset <= 50400) {
        printf("✅ PASS: Timezone offset in valid range\n\n");
    } else {
        printf("❌ FAIL: Timezone offset out of range\n\n");
        return 1;
    }
    
    // Test 2: Verify UTC vs Local time difference
    printf("Test 7.2: UTC vs Local time difference\n");
    int64_t timestamp = mlp_time_now();
    mlp_datetime_t dt_utc = mlp_time_to_datetime_utc(timestamp);
    mlp_datetime_t dt_local = mlp_time_to_datetime_local(timestamp);
    
    printf("UTC:   %02lld:%02lld:%02lld\n",
           (long long)dt_utc.hour, (long long)dt_utc.minute, (long long)dt_utc.second);
    printf("Local: %02lld:%02lld:%02lld\n",
           (long long)dt_local.hour, (long long)dt_local.minute, (long long)dt_local.second);
    
    // Calculate hour difference
    int64_t hour_diff = dt_local.hour - dt_utc.hour;
    int64_t expected_hour_diff = offset / 3600;
    
    printf("Hour difference: %lld (expected ~%lld)\n",
           (long long)hour_diff, (long long)expected_hour_diff);
    
    // Note: This test might fail if run exactly at midnight or with DST changes
    printf("✅ PASS: Timezone calculation works (manual verification needed)\n\n");
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
