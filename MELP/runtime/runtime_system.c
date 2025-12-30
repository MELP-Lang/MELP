/**
 * runtime_system.c - System utilities (CORE FUNCTIONS ONLY)
 * 
 * Part of MLP Runtime - Modularized from runtime.c (3245 lines)
 * Module size: ~100 lines (MODULAR ✓)
 * Dependencies: runtime_memory.h, runtime_string.h, runtime_error.h
 * 
 * PRAGMATIC APPROACH: Core 6 functions implemented
 * TODO (YZ_14): Add remaining 25 utility functions
 * 
 * 6 Temel Esas: MODULAR ✓, STATELESS ✓, GCC ✓, STO ✓, STRUCT+FUNC ✓
 */

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 700

#include "runtime_system.h"
#include "runtime_memory.h"
#include "runtime_string.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// ===== CORE Math Functions (3 functions) =====

double mlp_sqrt(double x) {
    return sqrt(x);
}

double mlp_pow(double x, double y) {
    return pow(x, y);
}

int64_t mlp_abs(int64_t x) {
    return (x < 0) ? -x : x;
}

// ===== CORE System Functions (3 functions) =====

/**
 * execute - Execute shell command and return exit code
 * CRITICAL: Most used system operation
 */
int64_t execute(const char* cmd) {
    if (!cmd) return -1;
    int result = system(cmd);
    return (int64_t)result;
}

/**
 * get_env - Get environment variable value
 * CRITICAL: Configuration and path resolution
 */
char* get_env(const char* name) {
    if (!name) return strdup("");
    
    char* value = getenv(name);
    if (!value) return strdup("");
    
    return strdup(value);
}

/**
 * time_now - Get current Unix timestamp
 * CRITICAL: Timing and logging
 */
int64_t time_now(void) {
    return (int64_t)time(NULL);
}

// ===== Math Functions (9 additional) =====

double mlp_sin(double x) {
    return sin(x);
}

double mlp_cos(double x) {
    return cos(x);
}

double mlp_tan(double x) {
    return tan(x);
}

double mlp_floor(double x) {
    return floor(x);
}

double mlp_ceil(double x) {
    return ceil(x);
}

double mlp_round(double x) {
    return round(x);
}

double mlp_min(double a, double b) {
    return (a < b) ? a : b;
}

double mlp_max(double a, double b) {
    return (a > b) ? a : b;
}

double mlp_fabs(double x) {
    return fabs(x);
}

// ===== Time Functions (4 additional) =====

char* format_time(int64_t timestamp, const char* format) {
    if (!format) format = "%Y-%m-%d %H:%M:%S";
    
    time_t t = (time_t)timestamp;
    struct tm* tm_info = localtime(&t);
    
    char* buffer = (char*)mlp_malloc(100);
    if (!buffer) return strdup("");
    
    strftime(buffer, 100, format, tm_info);
    return buffer;
}

int64_t parse_time(const char* str, const char* format) {
    if (!str || !format) return 0;
    
    struct tm tm_info = {0};
    strptime(str, format, &tm_info);
    return (int64_t)mktime(&tm_info);
}

void sleep_ms(int64_t milliseconds) {
    usleep((useconds_t)(milliseconds * 1000));
}

char* get_time_string(void) {
    return format_time(time_now(), "%Y-%m-%d %H:%M:%S");
}

// ===== Process Functions (2 additional) =====

int64_t get_pid(void) {
    return (int64_t)getpid();
}

int64_t get_parent_pid(void) {
    return (int64_t)getppid();
}

// ===== Path Functions (4 additional) =====

char* join_path(const char* path1, const char* path2) {
    if (!path1) path1 = "";
    if (!path2) path2 = "";
    
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    int need_sep = (len1 > 0 && path1[len1-1] != '/' && path2[0] != '/') ? 1 : 0;
    
    char* result = (char*)mlp_malloc(len1 + len2 + need_sep + 1);
    if (!result) return strdup("");
    
    strcpy(result, path1);
    if (need_sep) strcat(result, "/");
    strcat(result, path2);
    
    return result;
}

char* get_extension(const char* path) {
    if (!path) return strdup("");
    
    const char* dot = strrchr(path, '.');
    const char* sep = strrchr(path, '/');
    
    // Dot must come after last separator
    if (dot && (!sep || dot > sep)) {
        return strdup(dot + 1);
    }
    
    return strdup("");
}

char* get_filename(const char* path) {
    if (!path) return strdup("");
    
    const char* sep = strrchr(path, '/');
    if (sep) {
        return strdup(sep + 1);
    }
    
    return strdup(path);
}

char* get_directory(const char* path) {
    if (!path) return strdup("");
    
    const char* sep = strrchr(path, '/');
    if (!sep) return strdup(".");
    
    size_t len = sep - path;
    if (len == 0) return strdup("/");
    
    char* result = (char*)mlp_malloc(len + 1);
    if (!result) return strdup("");
    
    strncpy(result, path, len);
    result[len] = '\0';
    
    return result;
}

// ===== Directory Functions (3 additional) =====

int create_dir(const char* path) {
    if (!path) return 0;
    return (mkdir(path, 0755) == 0) ? 1 : 0;
}

int remove_dir(const char* path) {
    if (!path) return 0;
    return (rmdir(path) == 0) ? 1 : 0;
}

char** list_dir(const char* path, int* count) {
    (void)path;  // Unused parameter - placeholder implementation
    *count = 0;
    // Note: Full implementation requires dirent.h and proper memory management
    // This is a placeholder that returns empty list
    return NULL;
}

// ===== System Functions (3 additional) =====

char* get_executable_path(void) {
    char* buffer = (char*)mlp_malloc(1024);
    if (!buffer) return strdup("");
    
    ssize_t len = readlink("/proc/self/exe", buffer, 1023);
    if (len == -1) {
        mlp_free(buffer);
        return strdup("");
    }
    
    buffer[len] = '\0';
    return buffer;
}

char* get_current_dir(void) {
    char* buffer = (char*)mlp_malloc(1024);
    if (!buffer) return strdup("");
    
    if (getcwd(buffer, 1024) == NULL) {
        mlp_free(buffer);
        return strdup("");
    }
    
    return buffer;
}

int change_directory(const char* path) {
    if (!path) return 0;
    return (chdir(path) == 0) ? 1 : 0;
}
