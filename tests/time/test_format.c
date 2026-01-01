// ============================================
// TEST: Date Formatting
// ============================================
// Tests: mlp_time_format_iso(), mlp_time_format()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== Test 5: Date Formatting ===\n\n");
    
    // Test 1: Format as ISO 8601
    printf("Test 5.1: mlp_time_format_iso()\n");
    int64_t timestamp = 1735689600;  // 2026-01-01 00:00:00 UTC
    char* iso_str = mlp_time_format_iso(timestamp);
    
    printf("Timestamp: %lld\n", (long long)timestamp);
    printf("ISO format: %s\n", iso_str);
    
    // Check format (should be YYYY-MM-DD HH:MM:SS)
    if (iso_str != NULL && strlen(iso_str) >= 19) {
        printf("✅ PASS: ISO format generated\n\n");
    } else {
        printf("❌ FAIL: ISO format incorrect\n\n");
        if (iso_str) free(iso_str);
        return 1;
    }
    free(iso_str);
    
    // Test 2: Custom format - date only
    printf("Test 5.2: Custom format (%%Y-%%m-%%d)\n");
    char* date_str = mlp_time_format(timestamp, "%Y-%m-%d");
    printf("Date format: %s\n", date_str);
    
    if (date_str != NULL && strlen(date_str) >= 10) {
        printf("✅ PASS: Date format generated\n\n");
    } else {
        printf("❌ FAIL: Date format incorrect\n\n");
        if (date_str) free(date_str);
        return 1;
    }
    free(date_str);
    
    // Test 3: Custom format - time only
    printf("Test 5.3: Custom format (%%H:%%M:%%S)\n");
    char* time_str = mlp_time_format(timestamp, "%H:%M:%S");
    printf("Time format: %s\n", time_str);
    
    if (time_str != NULL && strlen(time_str) >= 8) {
        printf("✅ PASS: Time format generated\n\n");
    } else {
        printf("❌ FAIL: Time format incorrect\n\n");
        if (time_str) free(time_str);
        return 1;
    }
    free(time_str);
    
    // Test 4: Format current time
    printf("Test 5.4: Format current time\n");
    int64_t now = mlp_time_now();
    char* now_str = mlp_time_format_iso(now);
    printf("Current time: %s\n", now_str);
    
    if (now_str != NULL && strlen(now_str) >= 19) {
        printf("✅ PASS: Current time formatted\n\n");
    } else {
        printf("❌ FAIL: Current time format failed\n\n");
        if (now_str) free(now_str);
        return 1;
    }
    free(now_str);
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
