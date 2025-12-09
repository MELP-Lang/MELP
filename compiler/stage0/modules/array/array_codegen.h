#ifndef ARRAY_CODEGEN_H
#define ARRAY_CODEGEN_H

#include "array.h"
#include <stdio.h>

// Code generation for arrays, lists, and tuples

// Main entry point: Generate code for any collection literal
void codegen_collection(FILE* output, Collection* coll);

// Generate specific collection types
void codegen_array_literal(FILE* output, Array* arr);
void codegen_list_literal(FILE* output, List* list);
void codegen_tuple_literal(FILE* output, Tuple* tuple);

// Legacy: Generate array allocation and initialization
// Stack array: Fixed size, compile-time known
void codegen_array_stack(FILE* output, Array* arr);

// Heap array: Dynamic size, runtime allocation
void codegen_array_heap(FILE* output, Array* arr);

// Generate list allocation (always heap)
void codegen_list_create(FILE* output, List* list);

// Generate tuple allocation (always stack)
void codegen_tuple_create(FILE* output, Tuple* tuple);

// Generate index access
// Array: mov rax, [array_base + index*8]
void codegen_array_index(FILE* output, const char* array_name, int index);

// Generate list index access (with type checking)
void codegen_list_index(FILE* output, const char* list_name, int index);

// Generate index access from IndexAccess struct
// Supports constant, variable, and expression indices
void codegen_index_access(FILE* output, IndexAccess* access);

// Generate index access with variable index (runtime)
void codegen_array_index_var(FILE* output, const char* array_name, const char* index_var);

#endif
