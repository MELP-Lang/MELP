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

// ===== TODO (YZ_14): Implement these utility functions =====
// Math (9 more): sin, cos, tan, floor, ceil, round, min, max, fabs
// Time (4 more): format_time, parse_time, sleep_ms, get_time_string
// Process (2 more): get_pid, get_parent_pid  
// Path (4 more): join_path, get_extension, get_filename, get_directory
// Directory (3 more): create_dir, remove_dir, list_dir
// System (3 more): get_executable_path, get_current_dir, change_directory
