/**
 * runtime_io.c - File I/O operations (CORE FUNCTIONS ONLY)
 * 
 * Part of MLP Runtime - Modularized from runtime.c (3245 lines)
 * Module size: ~150 lines (MODULAR ✓)
 * Dependencies: runtime_memory.h, runtime_string.h, runtime_error.h
 * 
 * PRAGMATIC APPROACH: Core 4 functions implemented
 * TODO (YZ_14): Add remaining 9 utility functions
 * 
 * 6 Temel Esas: MODULAR ✓, STATELESS ✓, GCC ✓, STO ✓, STRUCT+FUNC ✓
 */

#include "runtime_io.h"
#include "runtime_memory.h"
#include "runtime_string.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// ===== CORE FILE I/O (4 functions) =====

/**
 * read_file - Read entire file into string
 * CRITICAL: Most used file operation
 */
char* read_file(const char* path) {
    if (!path) {
        set_error_code(101); // ERR_FILE_NOT_FOUND
        return NULL;
    }
    
    FILE* file = fopen(path, "r");
    if (!file) {
        set_error_code(101);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)mlp_malloc(size + 1);
    if (!content) {
        fclose(file);
        set_error_code(104); // ERR_OUT_OF_MEMORY
        return NULL;
    }
    
    size_t read_size = fread(content, 1, size, file);
    content[read_size] = '\0';
    fclose(file);
    
    return content;
}

/**
 * write_file - Write string to file
 * CRITICAL: Most used file write operation
 */
int write_file(const char* path, const char* content) {
    if (!path || !content) {
        set_error_code(105); // ERR_INVALID_ARGUMENT
        return 0;
    }
    
    FILE* file = fopen(path, "w");
    if (!file) {
        set_error_code(102); // ERR_PERMISSION_DENIED
        return 0;
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 1 : 0;
}

/**
 * dosya_oku - Turkish API for read_file
 */
char* dosya_oku(int64_t dosya_ptr) {
    FILE* file = (FILE*)dosya_ptr;
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)mlp_malloc(size + 1);
    if (!content) return NULL;
    
    size_t read_size = fread(content, 1, size, file);
    content[read_size] = '\0';
    
    return content;
}

/**
 * dosya_yaz - Turkish API for write_file
 */
int64_t dosya_yaz(int64_t dosya_ptr, const char* veri) {
    FILE* file = (FILE*)dosya_ptr;
    if (!file || !veri) return 0;
    
    size_t len = strlen(veri);
    size_t written = fwrite(veri, 1, len, file);
    
    return (int64_t)written;
}

// ===== TODO (YZ_14): Implement these utility functions =====
// - int64_t dosya_ac(const char* yol, const char* mod);
// - int64_t dosya_kapat(int64_t dosya_ptr);
// - int append_file(const char* path, const char* content);
// - int file_exists(const char* path);
// - long file_size(const char* path);
// - void* read_binary(const char* path);
// - int write_binary(const char* path, const void* data, long size);
// - char** read_lines(const char* path, int* line_count);
// - int copy_file(const char* source, const char* dest);

/**
 * STUBS for missing functions (to prevent linker errors)
 */
int64_t dosya_ac(const char* yol, const char* mod) {
    if (!yol || !mod) return 0;
    FILE* f = fopen(yol, mod);
    return (int64_t)f;
}

int64_t dosya_kapat(int64_t dosya_ptr) {
    FILE* f = (FILE*)dosya_ptr;
    if (f) fclose(f);
    return 1;
}

int append_file(const char* path, const char* content) {
    if (!path || !content) return 0;
    FILE* f = fopen(path, "a");
    if (!f) return 0;
    fputs(content, f);
    fclose(f);
    return 1;
}

int file_exists(const char* path) {
    if (!path) return 0;
    struct stat st;
    return (stat(path, &st) == 0) ? 1 : 0;
}

long file_size(const char* path) {
    if (!path) return 0;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (long)st.st_size;
}

void* read_binary(const char* path) {
    return (void*)read_file(path); // Simple implementation
}

int write_binary(const char* path, const void* data, long size) {
    if (!path || !data || size <= 0) return 0;
    FILE* f = fopen(path, "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == (size_t)size) ? 1 : 0;
}

char** read_lines(const char* path, int* line_count) {
    *line_count = 0;
    
    char* content = read_file(path);
    if (!content) return NULL;
    
    // Count lines
    int count = 1;
    for (const char* p = content; *p; p++) {
        if (*p == '\n') count++;
    }
    
    // Allocate array
    char** lines = (char**)mlp_malloc(sizeof(char*) * count);
    if (!lines) {
        mlp_free(content);
        return NULL;
    }
    
    // Split by newline
    int idx = 0;
    char* line_start = content;
    for (char* p = content; *p; p++) {
        if (*p == '\n') {
            *p = '\0';
            lines[idx] = string_concat("", line_start); // Copy line
            idx++;
            line_start = p + 1;
        }
    }
    // Last line (no trailing newline)
    if (*line_start) {
        lines[idx] = string_concat("", line_start);
        idx++;
    }
    
    mlp_free(content);
    *line_count = idx;
    return lines;
}

int copy_file(const char* source, const char* dest) {
    char* content = read_file(source);
    if (!content) return 0;
    int result = write_file(dest, content);
    mlp_free(content);
    return result;
}
