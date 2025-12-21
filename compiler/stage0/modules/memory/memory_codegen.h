#ifndef MEMORY_CODEGEN_H
#define MEMORY_CODEGEN_H

#include "memory.h"
#include <stdio.h>

// Code generation fonksiyonları
void codegen_memory_alloc(FILE* out, MemoryAlloc* alloc);
void codegen_memory_free(FILE* out, MemoryFree* free_op);
void codegen_memory_copy(FILE* out, MemoryCopy* copy);
void codegen_memory_zero(FILE* out, MemoryZero* zero);
void codegen_memory_realloc(FILE* out, MemoryRealloc* realloc_op);

// Yardımcı fonksiyonlar
void codegen_malloc_call(FILE* out, size_t size);
void codegen_free_call(FILE* out, const char* var_name);
void codegen_memset_call(FILE* out, const char* var_name, int value, size_t size);

// =============================================================================
// REFERENCE COUNTING CODEGEN (YZ_210)
// =============================================================================

// Generate LLVM declarations for RC functions
void codegen_rc_declarations(FILE* out);

// Generate RC malloc call
void codegen_rc_malloc(FILE* out, const char* var_name, size_t size);

// Generate RC retain call
void codegen_rc_retain(FILE* out, const char* var_name);

// Generate RC release call
void codegen_rc_release(FILE* out, const char* var_name);

// Generate RC get_count call
void codegen_rc_get_count(FILE* out, const char* result_var, const char* ptr_var);

#endif // MEMORY_CODEGEN_H
