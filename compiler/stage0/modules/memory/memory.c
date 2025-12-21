#include "memory.h"
#include <stdlib.h>
#include <string.h>

// Bellek ayırma oluştur
MemoryAlloc* memory_create_alloc(size_t size, AllocStrategy strategy, const char* var_name) {
    MemoryAlloc* alloc = malloc(sizeof(MemoryAlloc));
    alloc->size = size;
    alloc->strategy = strategy;
    alloc->var_name = strdup(var_name);
    alloc->is_array = 0;
    alloc->element_count = 0;
    alloc->address = NULL; // Runtime'da doldurulacak
    return alloc;
}

// Bellek serbest bırakma oluştur
MemoryFree* memory_create_free(const char* var_name) {
    MemoryFree* free_op = malloc(sizeof(MemoryFree));
    free_op->var_name = strdup(var_name);
    free_op->address = NULL; // Runtime'da doldurulacak
    return free_op;
}

// Bellek kopyalama oluştur
MemoryCopy* memory_create_copy(const char* dest, const char* src, size_t size) {
    MemoryCopy* copy = malloc(sizeof(MemoryCopy));
    copy->dest_var = strdup(dest);
    copy->src_var = strdup(src);
    copy->size = size;
    return copy;
}

// Bellek sıfırlama oluştur
MemoryZero* memory_create_zero(const char* var_name, size_t size) {
    MemoryZero* zero = malloc(sizeof(MemoryZero));
    zero->var_name = strdup(var_name);
    zero->size = size;
    return zero;
}

// Bellek yeniden boyutlandırma oluştur
MemoryRealloc* memory_create_realloc(const char* var_name, size_t old_size, size_t new_size) {
    MemoryRealloc* realloc_op = malloc(sizeof(MemoryRealloc));
    realloc_op->var_name = strdup(var_name);
    realloc_op->old_size = old_size;
    realloc_op->new_size = new_size;
    return realloc_op;
}

// Bellek temizleme
void memory_alloc_free(MemoryAlloc* alloc) {
    if (!alloc) return;
    free(alloc->var_name);
    free(alloc);
}

void memory_free_free(MemoryFree* free_op) {
    if (!free_op) return;
    free(free_op->var_name);
    free(free_op);
}

void memory_copy_free(MemoryCopy* copy) {
    if (!copy) return;
    free(copy->dest_var);
    free(copy->src_var);
    free(copy);
}

void memory_zero_free(MemoryZero* zero) {
    if (!zero) return;
    free(zero->var_name);
    free(zero);
}

void memory_realloc_free(MemoryRealloc* realloc_op) {
    if (!realloc_op) return;
    free(realloc_op->var_name);
    free(realloc_op);
}

// =============================================================================
// REFERENCE COUNTING IMPLEMENTATION (YZ_210)
// =============================================================================

// Get header from user pointer
static RefCountHeader* get_header(void* ptr) {
    if (!ptr) return NULL;
    return (RefCountHeader*)((char*)ptr - sizeof(RefCountHeader));
}

// Get user pointer from header
static void* get_user_ptr(RefCountHeader* header) {
    if (!header) return NULL;
    return (void*)((char*)header + sizeof(RefCountHeader));
}

// Allocate memory with reference counting
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

// Increment reference count
void rc_retain(void* ptr) {
    if (!ptr) return;
    
    RefCountHeader* header = get_header(ptr);
    if (header) {
        header->ref_count++;
    }
}

// Decrement reference count (auto-free if reaches 0)
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

// Get current reference count
size_t rc_get_count(void* ptr) {
    if (!ptr) return 0;
    
    RefCountHeader* header = get_header(ptr);
    return header ? header->ref_count : 0;
}

// Set destructor callback
void rc_set_destructor(void* ptr, void (*destructor)(void*)) {
    if (!ptr) return;
    
    RefCountHeader* header = get_header(ptr);
    if (header) {
        header->destructor = (void*)destructor;
    }
}

// Create ref count increment structure
RefCountIncrement* rc_create_increment(const char* var_name) {
    RefCountIncrement* inc = malloc(sizeof(RefCountIncrement));
    inc->var_name = strdup(var_name);
    inc->address = NULL; // Runtime'da doldurulacak
    return inc;
}

// Create ref count decrement structure
RefCountDecrement* rc_create_decrement(const char* var_name, int auto_free) {
    RefCountDecrement* dec = malloc(sizeof(RefCountDecrement));
    dec->var_name = strdup(var_name);
    dec->address = NULL; // Runtime'da doldurulacak
    dec->auto_free = auto_free;
    return dec;
}

// Free ref count structures
void rc_increment_free(RefCountIncrement* inc) {
    if (!inc) return;
    free(inc->var_name);
    free(inc);
}

void rc_decrement_free(RefCountDecrement* dec) {
    if (!dec) return;
    free(dec->var_name);
    free(dec);
}
