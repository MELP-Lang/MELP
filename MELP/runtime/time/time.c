// ============================================
// MELP RUNTIME - Time/Date Operations Implementation
// ============================================
// Time and date C runtime implementation
// File: MELP/runtime/time/time.c
//
// This file implements time/date operations for MELP
// Cross-platform support: Linux, macOS, Windows

// Define POSIX features before any includes
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include "mlp_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
    #include <sys/timeb.h>
#else
    #include <sys/time.h>
    #include <unistd.h>
#endif

// ============================================
// CLOCK OPERATIONS
// ============================================

// Get current Unix timestamp (seconds since epoch)
int64_t mlp_time_now(void) {
    return (int64_t)time(NULL);
}

// Get current timestamp in milliseconds
int64_t mlp_time_now_millis(void) {
#ifdef _WIN32
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (int64_t)(timebuffer.time * 1000 + timebuffer.millitm);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

// Get current timestamp in microseconds
int64_t mlp_time_now_micros(void) {
#ifdef _WIN32
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (int64_t)((counter.QuadPart * 1000000) / frequency.QuadPart);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
#endif
}

// Sleep for specified seconds
int64_t mlp_time_sleep(int64_t seconds) {
    if (seconds < 0) {
        return -1;
    }
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));
#else
    sleep((unsigned int)seconds);
#endif
    return 0;
}

// Sleep for specified milliseconds
int64_t mlp_time_sleep_millis(int64_t milliseconds) {
    if (milliseconds < 0) {
        return -1;
    }
#ifdef _WIN32
    Sleep((DWORD)milliseconds);
#else
    usleep((useconds_t)(milliseconds * 1000));
#endif
    return 0;
}

// ============================================
// DATETIME CONVERSION
// ============================================

// Convert timestamp to datetime struct (UTC timezone)
mlp_datetime_t mlp_time_to_datetime_utc(int64_t timestamp) {
    mlp_datetime_t dt = {0};
    time_t t = (time_t)timestamp;
    struct tm* tm_info = gmtime(&t);
    
    if (tm_info == NULL) {
        return dt;
    }
    
    dt.year = tm_info->tm_year + 1900;
    dt.month = tm_info->tm_mon + 1;
    dt.day = tm_info->tm_mday;
    dt.hour = tm_info->tm_hour;
    dt.minute = tm_info->tm_min;
    dt.second = tm_info->tm_sec;
    dt.weekday = tm_info->tm_wday;
    dt.yearday = tm_info->tm_yday + 1;
    dt.is_dst = tm_info->tm_isdst > 0;
    
    return dt;
}

// Convert timestamp to datetime struct (local timezone)
mlp_datetime_t mlp_time_to_datetime_local(int64_t timestamp) {
    mlp_datetime_t dt = {0};
    time_t t = (time_t)timestamp;
    struct tm* tm_info = localtime(&t);
    
    if (tm_info == NULL) {
        return dt;
    }
    
    dt.year = tm_info->tm_year + 1900;
    dt.month = tm_info->tm_mon + 1;
    dt.day = tm_info->tm_mday;
    dt.hour = tm_info->tm_hour;
    dt.minute = tm_info->tm_min;
    dt.second = tm_info->tm_sec;
    dt.weekday = tm_info->tm_wday;
    dt.yearday = tm_info->tm_yday + 1;
    dt.is_dst = tm_info->tm_isdst > 0;
    
    return dt;
}

// Convert datetime struct to Unix timestamp
int64_t mlp_time_from_datetime(mlp_datetime_t dt) {
    struct tm tm_info = {0};
    
    tm_info.tm_year = (int)dt.year - 1900;
    tm_info.tm_mon = (int)dt.month - 1;
    tm_info.tm_mday = (int)dt.day;
    tm_info.tm_hour = (int)dt.hour;
    tm_info.tm_min = (int)dt.minute;
    tm_info.tm_sec = (int)dt.second;
    tm_info.tm_isdst = dt.is_dst ? 1 : 0;
    
    time_t timestamp = mktime(&tm_info);
    if (timestamp == -1) {
        return -1;
    }
    
    return (int64_t)timestamp;
}

// ============================================
// DATE FORMATTING
// ============================================

// Format timestamp as ISO 8601 string (YYYY-MM-DD HH:MM:SS)
char* mlp_time_format_iso(int64_t timestamp) {
    time_t t = (time_t)timestamp;
    struct tm* tm_info = localtime(&t);
    
    if (tm_info == NULL) {
        char* error = malloc(20);
        snprintf(error, 20, "Invalid timestamp");
        return error;
    }
    
    char* buffer = malloc(64);
    if (buffer == NULL) {
        return NULL;
    }
    
    snprintf(buffer, 64, "%04d-%02d-%02d %02d:%02d:%02d",
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday,
             tm_info->tm_hour,
             tm_info->tm_min,
             tm_info->tm_sec);
    
    return buffer;
}

// Format timestamp with custom format string
char* mlp_time_format(int64_t timestamp, const char* format) {
    time_t t = (time_t)timestamp;
    struct tm* tm_info = localtime(&t);
    
    if (tm_info == NULL || format == NULL) {
        char* error = malloc(20);
        snprintf(error, 20, "Invalid input");
        return error;
    }
    
    // Allocate buffer for formatted string
    char* buffer = malloc(256);
    if (buffer == NULL) {
        return NULL;
    }
    
    size_t result = strftime(buffer, 256, format, tm_info);
    if (result == 0) {
        free(buffer);
        char* error = malloc(20);
        snprintf(error, 20, "Format error");
        return error;
    }
    
    return buffer;
}

// ============================================
// DATE PARSING
// ============================================

// Parse ISO 8601 datetime string to timestamp
int64_t mlp_time_parse_iso(const char* datetime_str) {
    if (datetime_str == NULL) {
        return -1;
    }
    
    struct tm tm_info = {0};
    
    // Parse YYYY-MM-DD HH:MM:SS format
    int parsed = sscanf(datetime_str, "%d-%d-%d %d:%d:%d",
                       &tm_info.tm_year,
                       &tm_info.tm_mon,
                       &tm_info.tm_mday,
                       &tm_info.tm_hour,
                       &tm_info.tm_min,
                       &tm_info.tm_sec);
    
    if (parsed < 3) {  // At least date must be present
        return -1;
    }
    
    // Adjust values
    tm_info.tm_year -= 1900;
    tm_info.tm_mon -= 1;
    tm_info.tm_isdst = -1;  // Let mktime determine DST
    
    time_t timestamp = mktime(&tm_info);
    if (timestamp == -1) {
        return -1;
    }
    
    return (int64_t)timestamp;
}

// Parse datetime string with custom format
int64_t mlp_time_parse(const char* datetime_str, const char* format) {
    if (datetime_str == NULL || format == NULL) {
        return -1;
    }
    
    struct tm tm_info = {0};
    
#ifdef _WIN32
    // Windows doesn't have strptime, use sscanf for basic formats
    // For now, only support %Y-%m-%d and similar basic patterns
    if (strcmp(format, "%Y-%m-%d") == 0) {
        int parsed = sscanf(datetime_str, "%d-%d-%d",
                           &tm_info.tm_year,
                           &tm_info.tm_mon,
                           &tm_info.tm_mday);
        if (parsed != 3) return -1;
        tm_info.tm_year -= 1900;
        tm_info.tm_mon -= 1;
    } else if (strcmp(format, "%m/%d/%Y") == 0) {
        int parsed = sscanf(datetime_str, "%d/%d/%d",
                           &tm_info.tm_mon,
                           &tm_info.tm_mday,
                           &tm_info.tm_year);
        if (parsed != 3) return -1;
        tm_info.tm_year -= 1900;
        tm_info.tm_mon -= 1;
    } else {
        return -1;  // Unsupported format on Windows
    }
#else
    // POSIX systems have strptime
    char* result = strptime(datetime_str, format, &tm_info);
    if (result == NULL) {
        return -1;
    }
#endif
    
    tm_info.tm_isdst = -1;  // Let mktime determine DST
    
    time_t timestamp = mktime(&tm_info);
    if (timestamp == -1) {
        return -1;
    }
    
    return (int64_t)timestamp;
}

// ============================================
// TIMEZONE OPERATIONS
// ============================================

// Get local timezone offset from UTC in seconds
int64_t mlp_time_get_timezone_offset(void) {
    time_t now = time(NULL);
    struct tm* utc_tm = gmtime(&now);
    struct tm* local_tm = localtime(&now);
    
    if (utc_tm == NULL || local_tm == NULL) {
        return 0;
    }
    
    // Convert both to time_t
    time_t utc_time = mktime(utc_tm);
    time_t local_time = mktime(local_tm);
    
    // Calculate difference
    double diff = difftime(local_time, utc_time);
    
    return (int64_t)diff;
}
