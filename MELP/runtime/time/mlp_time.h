// ============================================
// MELP RUNTIME - Time/Date Operations Header
// ============================================
// Time and date C runtime implementation
// File: MELP/runtime/time/mlp_time.h
//
// This header defines the C API for time/date operations
// used by stdlib/time/time.mlp

#ifndef MLP_TIME_H
#define MLP_TIME_H

#include <time.h>

#include <stdint.h>
#include <stdbool.h>

// ============================================
// DATETIME STRUCTURE
// ============================================

// Broken-down time structure (matches MLP datetime type)
typedef struct {
    int64_t year;       // Year (e.g., 2026)
    int64_t month;      // Month (1-12)
    int64_t day;        // Day (1-31)
    int64_t hour;       // Hour (0-23)
    int64_t minute;     // Minute (0-59)
    int64_t second;     // Second (0-59)
    int64_t weekday;    // Day of week (0=Sunday, 6=Saturday)
    int64_t yearday;    // Day of year (1-366)
    bool is_dst;        // Daylight Saving Time flag
} mlp_datetime_t;

// ============================================
// CLOCK OPERATIONS
// ============================================

// Get current Unix timestamp (seconds since epoch)
// Returns: Seconds since 1970-01-01 00:00:00 UTC
int64_t mlp_time_now(void);

// Get current timestamp in milliseconds
// Returns: Milliseconds since epoch
int64_t mlp_time_now_millis(void);

// Get current timestamp in microseconds
// Returns: Microseconds since epoch
int64_t mlp_time_now_micros(void);

// Sleep for specified seconds
// Args: seconds - Duration to sleep
// Returns: 0 on success, -1 on error
int64_t mlp_time_sleep(int64_t seconds);

// Sleep for specified milliseconds
// Args: milliseconds - Duration to sleep
// Returns: 0 on success, -1 on error
int64_t mlp_time_sleep_millis(int64_t milliseconds);

// ============================================
// DATETIME CONVERSION
// ============================================

// Convert timestamp to datetime struct (UTC timezone)
// Args: timestamp - Unix timestamp in seconds
// Returns: Filled datetime struct
mlp_datetime_t mlp_time_to_datetime_utc(int64_t timestamp);

// Convert timestamp to datetime struct (local timezone)
// Args: timestamp - Unix timestamp in seconds
// Returns: Filled datetime struct with local time
mlp_datetime_t mlp_time_to_datetime_local(int64_t timestamp);

// Convert datetime struct to Unix timestamp
// Args: dt - Pointer to datetime struct
// Returns: Unix timestamp in seconds
int64_t mlp_time_from_datetime(mlp_datetime_t dt);

// ============================================
// DATE FORMATTING
// ============================================

// Format timestamp as ISO 8601 string (YYYY-MM-DD HH:MM:SS)
// Args: timestamp - Unix timestamp in seconds
// Returns: Formatted string (caller must free)
char* mlp_time_format_iso(int64_t timestamp);

// Format timestamp with custom format string
// Args:
//   timestamp - Unix timestamp in seconds
//   format - Format string (strftime compatible)
// Returns: Formatted string (caller must free)
char* mlp_time_format(int64_t timestamp, const char* format);

// ============================================
// DATE PARSING
// ============================================

// Parse ISO 8601 datetime string to timestamp
// Args: datetime_str - ISO 8601 string (YYYY-MM-DD HH:MM:SS)
// Returns: Unix timestamp in seconds, or -1 on error
int64_t mlp_time_parse_iso(const char* datetime_str);

// Parse datetime string with custom format
// Args:
//   datetime_str - Date/time string to parse
//   format - Format string (strptime compatible)
// Returns: Unix timestamp in seconds, or -1 on error
int64_t mlp_time_parse(const char* datetime_str, const char* format);

// ============================================
// TIMEZONE OPERATIONS
// ============================================

// Get local timezone offset from UTC in seconds
// Returns: Offset in seconds (e.g., 10800 for UTC+3)
int64_t mlp_time_get_timezone_offset(void);

#endif // MLP_TIME_H
