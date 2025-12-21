/**
 * MELP Standard Library - Memory Management (RC)
 * YZ_210: Reference Counting Garbage Collection
 */

#include "mlp_memory.h"
#include <stdlib.h>
#include <string.h>

/**
 * Reference counting header
 * Prepended to each heap allocation
 */
typedef struct RefCountHeader {
    size_t ref_count;       // Reference counter
    size_t size;            // Allocated size (bytes)
    void* destructor;       // Optional destructor function
} RefCountHeader;

/**
 * Get header from user pointer
 */
static RefCountHeader* get_header(void* ptr) {
    if (!ptr) return NULL;
    return (RefCountHeader*)((char*)ptr - sizeof(RefCountHeader));
}

/**
 * Get user pointer from header
 */
static void* get_user_ptr(RefCountHeader* header) {
    if (!header) return NULL;
    return (void*)((char*)header + sizeof(RefCountHeader));
}

/**
 * Allocate memory with reference counting
 */
void* rc_malloc(size_t size) {
    // Allocate: header + user data
    RefCountHeader* header = (RefCountHeader*)malloc(sizeof(RefCountHeader) + size);
    if (!header) return NULL;
    
    // Initialize header
    header->ref_count = 1;  // Start with count = 1
    header->size = size;
    header->destructor = NULL;
    
    // Return user pointer (skip header)
    return get_user_ptr(header);
}

/**
 * Increment reference count
 */
void rc_retain(void* ptr) {
    if (!ptr) return;
    
    RefCountHeader* header = get_header(ptr);
    if (header) {
        header->ref_count++;
    }
}

/**
 * Decrement reference count (auto-free if reaches 0)
 */
void rc_release(void* ptr) {
    if (!ptr) return;
    
    RefCountHeader* header = get_header(ptr);
    if (!header) return;
    
    // Decrement
    if (header->ref_count > 0) {
        header->ref_count--;
    }
    
    // Free if count reaches 0
    if (header->ref_count == 0) {
        // Call destructor if set
        if (header->destructor) {
            void (*destructor)(void*) = (void (*)(void*))header->destructor;
            destructor(ptr);
        }
        
        // Free the entire block (header + data)
        free(header);
    }
}

/**
 * Get current reference count
 */
size_t rc_get_count(void* ptr) {
    if (!ptr) return 0;
    
    RefCountHeader* header = get_header(ptr);
    return header ? header->ref_count : 0;
}

/**
 * Set destructor callback
 */
void rc_set_destructor(void* ptr, void (*destructor)(void*)) {
    if (!ptr) return;
    
    RefCountHeader* header = get_header(ptr);
    if (header) {
        header->destructor = (void*)destructor;
    }
}
