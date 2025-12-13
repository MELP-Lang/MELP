// LLVM IR Code Generator for Functions
// Wrapper around llvm_backend module for functions compiler
// YZ_58 - Phase 13.5 Part 3

#ifndef FUNCTIONS_CODEGEN_LLVM_H
#define FUNCTIONS_CODEGEN_LLVM_H

#include <stdio.h>
#include "functions.h"
#include "../llvm_backend/llvm_backend.h"

// Context for LLVM code generation
typedef struct {
    LLVMContext* llvm_ctx;
    FunctionDeclaration* current_func;
} FunctionLLVMContext;

// Initialize LLVM codegen context
FunctionLLVMContext* function_llvm_context_create(FILE* output);

// Free LLVM codegen context
void function_llvm_context_free(FunctionLLVMContext* ctx);

// Generate LLVM IR for function declaration
void function_generate_declaration_llvm(FunctionLLVMContext* ctx, FunctionDeclaration* func);

// Generate LLVM IR module header
void function_generate_module_header_llvm(FILE* output);

// Generate LLVM IR module footer
void function_generate_module_footer_llvm(FILE* output);

#endif // FUNCTIONS_CODEGEN_LLVM_H
