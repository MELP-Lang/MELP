#ifndef RUNTIME_MEMORY_H
#define RUNTIME_MEMORY_H

/**
 * runtime_memory.h - Memory allocation and tracking
 * 
 * Part of MLP Runtime - Modularized from runtime.c
 * 6 Temel Esas: STATELESS (with tracking), MODULAR (< 500 satır)
 */

#include <stddef.h>
#include <stdint.h>

/**
 * mlp_malloc - Allocate memory with tracking
 * @param size: Number of bytes to allocate
 * @return: Pointer to allocated memory, NULL on failure
 *
 * MLP Usage: string buffer = mlp_malloc(256);
 */
void* mlp_malloc(long size);

/**
 * mlp_free - Free memory with tracking
 * @param ptr: Pointer to memory to free
 *
 * MLP Usage: mlp_free(buffer);
 */
void mlp_free(void* ptr);

/**
 * mlp_realloc - Reallocate memory with tracking
 * @param ptr: Pointer to existing memory (or NULL)
 * @param size: New size in bytes
 * @return: Pointer to reallocated memory
 *
 * MLP Usage: buffer = mlp_realloc(buffer, 512);
 */
void* mlp_realloc(void* ptr, long size);

/**
 * mlp_calloc - Allocate zeroed memory with tracking
 * @param count: Number of elements
 * @param size: Size of each element
 * @return: Pointer to allocated zeroed memory
 *
 * MLP Usage: int array = mlp_calloc(10, 8);
 */
void* mlp_calloc(long count, long size);

/**
 * get_allocated_bytes - Get total allocated memory
 * @return: Total bytes currently allocated
 *
 * MLP Usage: long total = get_allocated_bytes();
 */
long get_allocated_bytes(void);

/**
 * check_memory_leaks - Check for memory leaks and report
 * @return: Number of leaked blocks
 *
 * MLP Usage: long leaks = check_memory_leaks();
 */
long check_memory_leaks(void);

// ===== C Helper Functions (Low-level primitives) =====

/**
 * c_malloc - Raw malloc wrapper (no tracking)
 * @param size: Number of bytes to allocate
 * @return: Pointer to allocated memory
 */
void* c_malloc(int64_t size);

/**
 * c_free - Raw free wrapper (no tracking)
 * @param ptr: Pointer to memory to free
 */
void c_free(void* ptr);

/**
 * c_memcpy - Memory copy
 * @param dest: Destination buffer
 * @param src: Source buffer
 * @param n: Number of bytes to copy
 */
void c_memcpy(void* dest, const void* src, int64_t n);

/**
 * c_memset - Memory set
 * @param dest: Destination buffer
 * @param value: Value to set (0-255)
 * @param n: Number of bytes to set
 */
void c_memset(void* dest, int64_t value, int64_t n);

#endif /* RUNTIME_MEMORY_H */
