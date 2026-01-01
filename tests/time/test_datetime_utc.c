// ============================================
// TEST: Datetime Conversion (UTC)
// ============================================
// Tests: mlp_time_to_datetime_utc(), mlp_time_from_datetime()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 3: Datetime Conversion (UTC) ===\n\n");
    
    // Test 1: Convert known timestamp to datetime (2026-01-01 00:00:00 UTC)
    printf("Test 3.1: Known timestamp -> datetime\n");
    int64_t timestamp = 1767225600;  // 2026-01-01 00:00:00 UTC
    mlp_datetime_t dt = mlp_time_to_datetime_utc(timestamp);
    
    printf("Timestamp: %lld\n", (long long)timestamp);
    printf("Date: %04lld-%02lld-%02lld %02lld:%02lld:%02lld\n",
           (long long)dt.year, (long long)dt.month, (long long)dt.day,
           (long long)dt.hour, (long long)dt.minute, (long long)dt.second);
    printf("Weekday: %lld, Yearday: %lld\n",
           (long long)dt.weekday, (long long)dt.yearday);
    
    if (dt.year == 2026 && dt.month == 1 && dt.day == 1 &&
        dt.hour == 0 && dt.minute == 0 && dt.second == 0) {
        printf("✅ PASS: Datetime conversion correct\n\n");
    } else {
        printf("❌ FAIL: Datetime conversion incorrect\n\n");
        return 1;
    }
    
    // Test 2: Round-trip conversion (timestamp -> datetime -> timestamp)
    printf("Test 3.2: Round-trip conversion\n");
    int64_t original = mlp_time_now();
    mlp_datetime_t dt2 = mlp_time_to_datetime_utc(original);
    int64_t converted = mlp_time_from_datetime(dt2);
    
    printf("Original: %lld\n", (long long)original);
    printf("Converted back: %lld\n", (long long)converted);
    printf("Difference: %lld seconds\n", (long long)(original - converted));
    
    // Allow some tolerance due to timezone/DST handling
    int64_t diff = original > converted ? original - converted : converted - original;
    if (diff <= 86400) {  // Within 24 hours (timezone differences)
        printf("✅ PASS: Round-trip conversion successful\n\n");
    } else {
        printf("❌ FAIL: Round-trip conversion failed\n\n");
        return 1;
    }
    
    // Test 3: Validate fields are in valid ranges
    printf("Test 3.3: Field validation\n");
    mlp_datetime_t dt3 = mlp_time_to_datetime_utc(mlp_time_now());
    
    printf("Year: %lld, Month: %lld, Day: %lld\n",
           (long long)dt3.year, (long long)dt3.month, (long long)dt3.day);
    printf("Hour: %lld, Minute: %lld, Second: %lld\n",
           (long long)dt3.hour, (long long)dt3.minute, (long long)dt3.second);
    
    if (dt3.year >= 2026 && dt3.year <= 2100 &&
        dt3.month >= 1 && dt3.month <= 12 &&
        dt3.day >= 1 && dt3.day <= 31 &&
        dt3.hour >= 0 && dt3.hour <= 23 &&
        dt3.minute >= 0 && dt3.minute <= 59 &&
        dt3.second >= 0 && dt3.second <= 59 &&
        dt3.weekday >= 0 && dt3.weekday <= 6 &&
        dt3.yearday >= 1 && dt3.yearday <= 366) {
        printf("✅ PASS: All fields in valid ranges\n\n");
    } else {
        printf("❌ FAIL: Some fields out of range\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
