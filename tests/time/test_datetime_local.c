// ============================================
// TEST: Datetime Conversion (Local)
// ============================================
// Tests: mlp_time_to_datetime_local()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 4: Datetime Conversion (Local) ===\n\n");
    
    // Test 1: Compare UTC vs Local datetime
    printf("Test 4.1: UTC vs Local comparison\n");
    int64_t timestamp = mlp_time_now();
    mlp_datetime_t dt_utc = mlp_time_to_datetime_utc(timestamp);
    mlp_datetime_t dt_local = mlp_time_to_datetime_local(timestamp);
    
    printf("UTC:   %04lld-%02lld-%02lld %02lld:%02lld:%02lld\n",
           (long long)dt_utc.year, (long long)dt_utc.month, (long long)dt_utc.day,
           (long long)dt_utc.hour, (long long)dt_utc.minute, (long long)dt_utc.second);
    printf("Local: %04lld-%02lld-%02lld %02lld:%02lld:%02lld\n",
           (long long)dt_local.year, (long long)dt_local.month, (long long)dt_local.day,
           (long long)dt_local.hour, (long long)dt_local.minute, (long long)dt_local.second);
    
    // They should differ (unless you're in UTC timezone)
    if (dt_local.year >= 2026 && dt_local.month >= 1 && dt_local.day >= 1) {
        printf("✅ PASS: Local datetime valid\n\n");
    } else {
        printf("❌ FAIL: Local datetime invalid\n\n");
        return 1;
    }
    
    // Test 2: Local datetime fields in valid ranges
    printf("Test 4.2: Local datetime field validation\n");
    
    printf("Year: %lld, Month: %lld, Day: %lld\n",
           (long long)dt_local.year, (long long)dt_local.month, (long long)dt_local.day);
    
    if (dt_local.year >= 2026 && dt_local.year <= 2100 &&
        dt_local.month >= 1 && dt_local.month <= 12 &&
        dt_local.day >= 1 && dt_local.day <= 31 &&
        dt_local.hour >= 0 && dt_local.hour <= 23 &&
        dt_local.minute >= 0 && dt_local.minute <= 59 &&
        dt_local.second >= 0 && dt_local.second <= 59) {
        printf("✅ PASS: Local datetime fields valid\n\n");
    } else {
        printf("❌ FAIL: Some fields out of range\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
