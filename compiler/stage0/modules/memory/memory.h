#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include "../lexer/lexer.h"

// Bellek ayırma stratejileri
typedef enum AllocStrategy {
    ALLOC_STACK,        // Stack'te ayır (hızlı, sınırlı)
    ALLOC_HEAP,         // Heap'te ayır (yavaş, esnek)
    ALLOC_STATIC        // Static/Global (program ömrü)
} AllocStrategy;

// Bellek ayırma talebi
typedef struct MemoryAlloc {
    size_t size;                // Talep edilen boyut (byte)
    AllocStrategy strategy;     // Ayırma stratejisi
    char* var_name;             // İlişkili değişken adı
    int is_array;               // Dizi mi?
    size_t element_count;       // Eleman sayısı (dizi için)
    void* address;              // Ayrılan bellek adresi (runtime)
} MemoryAlloc;

// Bellek serbest bırakma
typedef struct MemoryFree {
    char* var_name;             // Serbest bırakılacak değişken
    void* address;              // Serbest bırakılacak adres
} MemoryFree;

// Bellek kopyalama
typedef struct MemoryCopy {
    char* dest_var;             // Hedef değişken
    char* src_var;              // Kaynak değişken
    size_t size;                // Kopyalanacak boyut
} MemoryCopy;

// Bellek sıfırlama
typedef struct MemoryZero {
    char* var_name;             // Sıfırlanacak değişken
    size_t size;                // Sıfırlanacak boyut
} MemoryZero;

// Bellek yeniden boyutlandırma
typedef struct MemoryRealloc {
    char* var_name;             // Yeniden boyutlandırılacak değişken
    size_t old_size;            // Eski boyut
    size_t new_size;            // Yeni boyut
} MemoryRealloc;

// =============================================================================
// REFERENCE COUNTING GC (YZ_210)
// =============================================================================

// Reference counted memory header
// This is prepended to each heap allocation
typedef struct RefCountHeader {
    size_t ref_count;           // Reference counter
    size_t size;                // Allocated size (bytes)
    void* destructor;           // Optional destructor function
} RefCountHeader;

// Reference counting operations
typedef struct RefCountIncrement {
    char* var_name;             // Variable to increment
    void* address;              // Memory address
} RefCountIncrement;

typedef struct RefCountDecrement {
    char* var_name;             // Variable to decrement
    void* address;              // Memory address
    int auto_free;              // Auto-free when count reaches 0
} RefCountDecrement;

// API fonksiyonları
MemoryAlloc* memory_create_alloc(size_t size, AllocStrategy strategy, const char* var_name);
MemoryFree* memory_create_free(const char* var_name);
MemoryCopy* memory_create_copy(const char* dest, const char* src, size_t size);
MemoryZero* memory_create_zero(const char* var_name, size_t size);
MemoryRealloc* memory_create_realloc(const char* var_name, size_t old_size, size_t new_size);

void memory_alloc_free(MemoryAlloc* alloc);
void memory_free_free(MemoryFree* free_op);
void memory_copy_free(MemoryCopy* copy);
void memory_zero_free(MemoryZero* zero);
void memory_realloc_free(MemoryRealloc* realloc);

// =============================================================================
// REFERENCE COUNTING API (YZ_210)
// =============================================================================

// Allocate memory with reference counting
void* rc_malloc(size_t size);

// Increment reference count
void rc_retain(void* ptr);

// Decrement reference count (auto-free if reaches 0)
void rc_release(void* ptr);

// Get current reference count
size_t rc_get_count(void* ptr);

// Set destructor callback
void rc_set_destructor(void* ptr, void (*destructor)(void*));

// Create ref count structures
RefCountIncrement* rc_create_increment(const char* var_name);
RefCountDecrement* rc_create_decrement(const char* var_name, int auto_free);

void rc_increment_free(RefCountIncrement* inc);
void rc_decrement_free(RefCountDecrement* dec);

#endif // MEMORY_H
