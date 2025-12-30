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

// ===== Additional Math Functions (9 functions) =====
double mlp_sin(double x);
double mlp_cos(double x);
double mlp_tan(double x);
double mlp_floor(double x);
double mlp_ceil(double x);
double mlp_round(double x);
double mlp_min(double a, double b);
double mlp_max(double a, double b);
double mlp_fabs(double x);

// ===== CORE System Functions (3 functions) =====
int64_t execute(const char* cmd);
char* get_env(const char* name);
int64_t time_now(void);

// ===== Time Functions (4 functions) =====
char* format_time(int64_t timestamp, const char* format);
int64_t parse_time(const char* str, const char* format);
void sleep_ms(int64_t milliseconds);
char* get_time_string(void);

// ===== Process Functions (2 functions) =====
int64_t get_pid(void);
int64_t get_parent_pid(void);

// ===== Path Functions (4 functions) =====
char* join_path(const char* path1, const char* path2);
char* get_extension(const char* path);
char* get_filename(const char* path);
char* get_directory(const char* path);

// ===== Directory Functions (3 functions) =====
int create_dir(const char* path);
int remove_dir(const char* path);
char** list_dir(const char* path, int* count);

// ===== System Functions (3 functions) =====
char* get_executable_path(void);
char* get_current_dir(void);
int change_directory(const char* path);

#endif /* RUNTIME_SYSTEM_H */
