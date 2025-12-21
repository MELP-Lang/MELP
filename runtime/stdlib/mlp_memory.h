/**
 * MELP Standard Library - Memory Management (RC)
 * YZ_210: Reference Counting Garbage Collection
 * 
 * Provides reference counting memory management:
 * - rc_malloc: Allocate with ref count = 1
 * - rc_retain: Increment reference count
 * - rc_release: Decrement, auto-free when count = 0
 * - rc_get_count: Query current count
 * - rc_set_destructor: Set cleanup callback
 */

#ifndef MLP_MEMORY_H
#define MLP_MEMORY_H

#include <stddef.h>

/**
 * Allocate memory with reference counting
 * Initial ref_count = 1
 * 
 * @param size Size in bytes
 * @return Pointer to allocated memory (NULL on failure)
 */
void* rc_malloc(size_t size);

/**
 * Increment reference count
 * 
 * @param ptr Pointer returned by rc_malloc
 */
void rc_retain(void* ptr);

/**
 * Decrement reference count
 * Auto-frees memory when count reaches 0
 * 
 * @param ptr Pointer returned by rc_malloc
 */
void rc_release(void* ptr);

/**
 * Get current reference count
 * 
 * @param ptr Pointer returned by rc_malloc
 * @return Current reference count (0 if NULL)
 */
size_t rc_get_count(void* ptr);

/**
 * Set destructor callback
 * Called before freeing memory (when count reaches 0)
 * 
 * @param ptr Pointer returned by rc_malloc
 * @param destructor Callback function
 */
void rc_set_destructor(void* ptr, void (*destructor)(void*));

#endif // MLP_MEMORY_H
