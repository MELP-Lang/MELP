#ifndef RUNTIME_SYSTEM_H
#define RUNTIME_SYSTEM_H

/**
 * runtime_system.h - System utilities (CORE FUNCTIONS)
 * Part of MLP Runtime - Modularized from runtime.c
 */

#include <stdint.h>

// ===== CORE Math Functions (3 functions) =====
double mlp_sqrt(double x);
double mlp_pow(double x, double y);
int64_t mlp_abs(int64_t x);

// ===== CORE System Functions (3 functions) =====
int64_t execute(const char* cmd);
char* get_env(const char* name);
int64_t time_now(void);

// TODO (YZ_14): Add remaining 25 utility functions
// Math: sin, cos, tan, floor, ceil, round, min, max
// Time: format_time, parse_time, sleep_ms
// Process: get_pid, get_parent_pid
// Path: join_path, get_extension, get_filename, get_directory
// Directory: create_dir, remove_dir, list_dir
// System: get_executable_path, get_current_dir, change_directory

#endif /* RUNTIME_SYSTEM_H */
