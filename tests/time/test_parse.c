// ============================================
// TEST: Date Parsing
// ============================================
// Tests: mlp_time_parse_iso(), mlp_time_parse()

#include "../../MELP/runtime/time/mlp_time.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Test 6: Date Parsing ===\n\n");
    
    // Test 1: Parse ISO 8601 format
    printf("Test 6.1: mlp_time_parse_iso()\n");
    const char* iso_str = "2026-01-01 12:30:45";
    int64_t parsed = mlp_time_parse_iso(iso_str);
    
    printf("Input: %s\n", iso_str);
    printf("Parsed timestamp: %lld\n", (long long)parsed);
    
    if (parsed > 0) {
        // Verify by converting back
        char* formatted = mlp_time_format_iso(parsed);
        printf("Formatted back: %s\n", formatted);
        printf("✅ PASS: ISO parsing successful\n\n");
        free(formatted);
    } else {
        printf("❌ FAIL: ISO parsing failed\n\n");
        return 1;
    }
    
    // Test 2: Parse date only
    printf("Test 6.2: Parse date only (YYYY-MM-DD)\n");
    const char* date_str = "2026-06-15";
    int64_t parsed_date = mlp_time_parse_iso(date_str);
    
    printf("Input: %s\n", date_str);
    printf("Parsed timestamp: %lld\n", (long long)parsed_date);
    
    if (parsed_date > 0) {
        char* formatted = mlp_time_format_iso(parsed_date);
        printf("Formatted: %s\n", formatted);
        printf("✅ PASS: Date-only parsing successful\n\n");
        free(formatted);
    } else {
        printf("❌ FAIL: Date-only parsing failed\n\n");
        return 1;
    }
    
    // Test 3: Parse with custom format (YYYY-MM-DD)
    printf("Test 6.3: mlp_time_parse() with custom format\n");
    const char* custom_date = "2026-12-25";
    int64_t parsed_custom = mlp_time_parse(custom_date, "%Y-%m-%d");
    
    printf("Input: %s (format: %%Y-%%m-%%d)\n", custom_date);
    printf("Parsed timestamp: %lld\n", (long long)parsed_custom);
    
    if (parsed_custom > 0) {
        mlp_datetime_t dt = mlp_time_to_datetime_local(parsed_custom);
        printf("Date: %04lld-%02lld-%02lld\n",
               (long long)dt.year, (long long)dt.month, (long long)dt.day);
        
        if (dt.year == 2026 && dt.month == 12 && dt.day == 25) {
            printf("✅ PASS: Custom format parsing successful\n\n");
        } else {
            printf("❌ FAIL: Parsed date doesn't match expected\n\n");
            return 1;
        }
    } else {
        printf("❌ FAIL: Custom format parsing failed\n\n");
        return 1;
    }
    
    // Test 4: Error handling - invalid format
    printf("Test 6.4: Invalid date string\n");
    const char* invalid = "not-a-date";
    int64_t invalid_result = mlp_time_parse_iso(invalid);
    
    printf("Input: %s\n", invalid);
    printf("Result: %lld\n", (long long)invalid_result);
    
    if (invalid_result == -1) {
        printf("✅ PASS: Invalid date rejected\n\n");
    } else {
        printf("❌ FAIL: Should return -1 for invalid input\n\n");
        return 1;
    }
    
    printf("=== All Tests Passed ===\n");
    return 0;
}
