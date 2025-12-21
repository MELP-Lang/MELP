#ifndef FUNCTION_POINTER_CODEGEN_H
#define FUNCTION_POINTER_CODEGEN_H

#include "function_pointer.h"
#include "../llvm_backend/llvm_backend.h"
#include <stdio.h>

// YZ_209: Function Pointer Code Generation
// Generates LLVM IR for function pointers

// Generate LLVM type for function pointer
// Example: i64 (i64, i64)*
void codegen_function_pointer_type(FILE* output, FunctionPointerType* type);

// Generate function pointer variable declaration
// Allocates stack space for function pointer
void codegen_function_pointer_var(FILE* output, FunctionPointerVar* var, LLVMContext* ctx);

// Generate function reference (get function address)
// Example: %ptr = bitcast i64 (i64, i64)* @add to i64 (i64, i64)*
void codegen_function_reference(FILE* output, FunctionReference* ref, LLVMContext* ctx);

// Generate function pointer assignment
// Stores function pointer to variable
void codegen_function_pointer_assign(FILE* output, FunctionPointerVar* var, 
                                     FunctionReference* ref, LLVMContext* ctx);

// Generate indirect function call
// Calls function through pointer
// Example: %result = call i64 %ptr(i64 10, i64 20)
LLVMValue* codegen_indirect_call(FILE* output, IndirectCall* call, LLVMContext* ctx);

// Helper: Get LLVM type string from MELP type
const char* get_llvm_type_for_melp(const char* melp_type);

// Helper: Generate function type string
// Example: "i64 (i64, i64)*"
char* generate_function_type_string(FunctionPointerType* type);

#endif // FUNCTION_POINTER_CODEGEN_H
