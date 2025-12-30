/**
 * runtime_memory.c - Memory allocation and tracking
 * 
 * Part of MLP Runtime - Modularized from runtime.c (3245 lines)
 * Module size: ~180 lines (MODULAR ✓)
 * Dependencies: runtime_error.h
 * 
 * 6 Temel Esas:
 * 1. MODULAR ✓ (< 500 satır)
 * 2. GCC ✓
 * 3. STO ✓
 * 4. STATELESS ⚠️ (memory tracking list - acceptable for GC)
 * 5. STRUCT+FUNC ✓
 * 6. MODÜL=ŞABLON ✓
 */

#include "runtime_memory.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory tracking structure
typedef struct MemoryBlock {
    void* ptr;
    size_t size;
    struct MemoryBlock* next;
} MemoryBlock;

// Global tracking state (acceptable for GC purposes)
static MemoryBlock* memory_list_head = NULL;
static size_t total_allocated = 0;
static size_t total_freed = 0;

/**
 * track_allocation - Add allocation to tracking list (internal)
 */
static void track_allocation(void* ptr, size_t size) {
    if (!ptr) return;

    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (!block) return;  // Tracking failure shouldn't crash program

    block->ptr = ptr;
    block->size = size;
    block->next = memory_list_head;
    memory_list_head = block;
    total_allocated += size;
}

/**
 * untrack_allocation - Remove allocation from tracking list (internal)
 */
static void untrack_allocation(void* ptr) {
    if (!ptr) return;

    MemoryBlock** current = &memory_list_head;
    while (*current) {
        if ((*current)->ptr == ptr) {
            MemoryBlock* to_free = *current;
            total_freed += to_free->size;
            *current = to_free->next;
            free(to_free);
            return;
        }
        current = &(*current)->next;
    }
}

/**
 * mlp_malloc - Allocate memory with tracking
 */
void* mlp_malloc(long size) {
    if (size <= 0) return NULL;

    void* ptr = malloc((size_t)size);
    if (ptr) {
        track_allocation(ptr, (size_t)size);
    }
    return ptr;
}

/**
 * mlp_free - Free memory with tracking
 */
void mlp_free(void* ptr) {
    if (!ptr) return;

    untrack_allocation(ptr);
    free(ptr);
}

/**
 * mlp_realloc - Reallocate memory with tracking
 */
void* mlp_realloc(void* ptr, long size) {
    if (size <= 0) {
        mlp_free(ptr);
        return NULL;
    }

    if (!ptr) {
        return mlp_malloc(size);
    }

    // Find old size for tracking
    size_t old_size = 0;
    MemoryBlock* current = memory_list_head;
    while (current) {
        if (current->ptr == ptr) {
            old_size = current->size;
            break;
        }
        current = current->next;
    }

    void* new_ptr = realloc(ptr, (size_t)size);
    if (new_ptr) {
        untrack_allocation(ptr);
        track_allocation(new_ptr, (size_t)size);
    }
    return new_ptr;
}

/**
 * mlp_calloc - Allocate zeroed memory with tracking
 */
void* mlp_calloc(long count, long size) {
    if (count <= 0 || size <= 0) return NULL;

    void* ptr = calloc((size_t)count, (size_t)size);
    if (ptr) {
        track_allocation(ptr, (size_t)(count * size));
    }
    return ptr;
}

/**
 * get_allocated_bytes - Get total allocated memory
 */
long get_allocated_bytes(void) {
    return (long)(total_allocated - total_freed);
}

/**
 * check_memory_leaks - Check for memory leaks and report
 */
long check_memory_leaks(void) {
    long leak_count = 0;
    size_t leaked_bytes = 0;

    MemoryBlock* current = memory_list_head;
    while (current) {
        leak_count++;
        leaked_bytes += current->size;
        fprintf(stderr, "MEMORY LEAK: %zu bytes at %p\n",
                current->size, current->ptr);
        current = current->next;
    }

    if (leak_count > 0) {
        fprintf(stderr, "TOTAL LEAKS: %ld blocks, %zu bytes\n",
                leak_count, leaked_bytes);
    }

    return leak_count;
}

// ===== C Helper Functions (Low-level primitives) =====

void* c_malloc(int64_t size) {
    return malloc((size_t)size);
}

void c_free(void* ptr) {
    free(ptr);
}

void c_memcpy(void* dest, const void* src, int64_t n) {
    memcpy(dest, src, (size_t)n);
}

void c_memset(void* dest, int64_t value, int64_t n) {
    memset(dest, (int)value, (size_t)n);
}
