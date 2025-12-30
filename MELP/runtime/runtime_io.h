#ifndef RUNTIME_IO_H
#define RUNTIME_IO_H

/**
 * runtime_io.h - File I/O operations
 * Part of MLP Runtime - Modularized from runtime.c
 */

#include <stdint.h>

// File operations (Turkish API)
int64_t dosya_ac(const char* yol, const char* mod);
char* dosya_oku(int64_t dosya_ptr);
int64_t dosya_yaz(int64_t dosya_ptr, const char* veri);
int64_t dosya_kapat(int64_t dosya_ptr);

// File operations (English API)
char* read_file(const char* path);
int write_file(const char* path, const char* content);
int append_file(const char* path, const char* content);
int file_exists(const char* path);
long file_size(const char* path);
void* read_binary(const char* path);
int write_binary(const char* path, const void* data, long size);
char** read_lines(const char* path, int* line_count);
int copy_file(const char* source, const char* dest);

#endif /* RUNTIME_IO_H */
