// ============================================
// TEST: Edge Cases
// ============================================
// Tests: Boundary conditions and error handling

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 9: Edge Cases ===\n\n");
    
    // Test 1: Year 2038 problem (32-bit Unix timestamp overflow)
    printf("Test 9.1: Year 2038 timestamp\n");
    int64_t year_2038 = 2147483647;  // Last second before 32-bit overflow
    mlp_datetime_t dt_2038 = mlp_time_to_datetime_utc(year_2038);
    
    printf("Timestamp: %lld\n", (long long)year_2038);
    printf("Date: %04lld-%02lld-%02lld %02lld:%02lld:%02lld\n",
           (long long)dt_2038.year, (long long)dt_2038.month, (long long)dt_2038.day,
           (long long)dt_2038.hour, (long long)dt_2038.minute, (long long)dt_2038.second);
    
    if (dt_2038.year == 2038 && dt_2038.month == 1) {
        printf("✅ PASS: Year 2038 handled correctly\n\n");
    } else {
        printf("❌ FAIL: Year 2038 problem detected\n\n");
        return 1;
    }
    
    // Test 2: Far future date
    printf("Test 9.2: Year 2100 timestamp\n");
    int64_t year_2100 = 4102444800;  // 2100-01-01 00:00:00 UTC
    mlp_datetime_t dt_2100 = mlp_time_to_datetime_utc(year_2100);
    
    printf("Timestamp: %lld\n", (long long)year_2100);
    printf("Date: %04lld-%02lld-%02lld\n",
           (long long)dt_2100.year, (long long)dt_2100.month, (long long)dt_2100.day);
    
    if (dt_2100.year == 2100 && dt_2100.month == 1 && dt_2100.day == 1) {
        printf("✅ PASS: Far future date works\n\n");
    } else {
        printf("❌ FAIL: Far future date incorrect\n\n");
        return 1;
    }
    
    // Test 3: Leap year handling (2024 was a leap year)
    printf("Test 9.3: Leap year (2024-02-29)\n");
    int64_t leap_day = 1709164800;  // 2024-02-29 00:00:00 UTC
    mlp_datetime_t dt_leap = mlp_time_to_datetime_utc(leap_day);
    
    printf("Timestamp: %lld\n", (long long)leap_day);
    printf("Date: %04lld-%02lld-%02lld\n",
           (long long)dt_leap.year, (long long)dt_leap.month, (long long)dt_leap.day);
    
    if (dt_leap.year == 2024 && dt_leap.month == 2 && dt_leap.day == 29) {
        printf("✅ PASS: Leap year date correct\n\n");
    } else {
        printf("❌ FAIL: Leap year handling incorrect\n\n");
        return 1;
    }
    
    // Test 4: End of day
    printf("Test 9.4: End of day (23:59:59)\n");
    int64_t end_of_day = 1735775999;  // 2026-01-01 23:59:59 UTC
    mlp_datetime_t dt_eod = mlp_time_to_datetime_utc(end_of_day);
    
    printf("Timestamp: %lld\n", (long long)end_of_day);
    printf("Time: %02lld:%02lld:%02lld\n",
           (long long)dt_eod.hour, (long long)dt_eod.minute, (long long)dt_eod.second);
    
    if (dt_eod.hour == 23 && dt_eod.minute == 59 && dt_eod.second == 59) {
        printf("✅ PASS: End of day correct\n\n");
    } else {
        printf("❌ FAIL: End of day incorrect\n\n");
        return 1;
    }
    
    // Test 5: Invalid input handling
    printf("Test 9.5: Invalid timestamp (negative)\n");
    int64_t invalid_ts = -1;
    mlp_datetime_t dt_invalid = mlp_time_to_datetime_utc(invalid_ts);
    
    printf("Timestamp: %lld\n", (long long)invalid_ts);
    printf("Result year: %lld\n", (long long)dt_invalid.year);
    
    // Should handle gracefully (year 1969 or 0)
    if (dt_invalid.year < 1970) {
        printf("✅ PASS: Invalid timestamp handled\n\n");
    } else {
        printf("❌ FAIL: Invalid timestamp not handled correctly\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
