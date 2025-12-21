// LLVM IR Code Generator for Functions
// Wrapper around llvm_backend module for functions compiler
// YZ_58 - Phase 13.5 Part 3

#ifndef FUNCTIONS_CODEGEN_LLVM_H
#define FUNCTIONS_CODEGEN_LLVM_H

#include <stdio.h>
#include "functions.h"
#include "../llvm_backend/llvm_backend.h"

// Forward declarations
typedef struct GenericRegistry GenericRegistry;  // YZ_203

// Variable type tracking entry (YZ_200)
typedef struct {
    char* name;           // Variable name
    int is_pointer;       // 0 = i64, 1 = i8* (string/list)
} VarTypeEntry;

// Context for LLVM code generation
typedef struct {
    LLVMContext* llvm_ctx;
    FunctionDeclaration* current_func;
    int globals_emitted;  // YZ_61: Flag to emit globals only once
    
    // YZ_200: Variable type tracking
    VarTypeEntry* var_types;  // Array of variable types
    int var_type_count;       // Number of tracked variables
    int var_type_capacity;    // Allocated capacity
    
    // YZ_203: Generic template registry
    GenericRegistry* generic_registry;
} FunctionLLVMContext;

// Initialize LLVM codegen context
FunctionLLVMContext* function_llvm_context_create(FILE* output, GenericRegistry* registry);

// Free LLVM codegen context
void function_llvm_context_free(FunctionLLVMContext* ctx);

// Generate LLVM IR for function declaration
void function_generate_declaration_llvm(FunctionLLVMContext* ctx, FunctionDeclaration* func);

// Generate LLVM IR module header
void function_generate_module_header_llvm(FILE* output);

// Generate LLVM IR module footer
void function_generate_module_footer_llvm(FILE* output);

#endif // FUNCTIONS_CODEGEN_LLVM_H
