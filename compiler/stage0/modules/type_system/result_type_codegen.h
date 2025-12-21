#ifndef RESULT_TYPE_CODEGEN_H
#define RESULT_TYPE_CODEGEN_H

#include "result_type.h"
#include "../llvm_backend/llvm_backend.h"
#include "../expression/expression.h"

/**
 * Result Type LLVM IR Codegen - YZ_206
 * 
 * Runtime representation:
 *   struct Result {
 *       i8 variant;    // 0 = ok, 1 = error
 *       i64 data;      // union of value and error
 *   }
 * 
 * LLVM IR:
 *   %result = type { i8, i64 }
 */

// Generate LLVM type for result<T, E>
// Returns: %result_TYPE = type { i8, i64 } (or appropriate type)
char* llvm_gen_result_type(Type* result_type);

// Generate LLVM IR for ok(value) constructor
// Returns: %result = { i8 0, i64 %value }
char* llvm_gen_ok_constructor(Expression* ok_expr, LLVMContext* ctx);

// Generate LLVM IR for error(msg) constructor
// Returns: %result = { i8 1, i64 %error }
char* llvm_gen_error_constructor(Expression* error_expr, LLVMContext* ctx);

// Generate LLVM IR for match expression
// Generates: switch on variant field, branch to ok/error blocks
char* llvm_gen_result_match(ResultMatch* match, LLVMContext* ctx);

// Generate LLVM IR for ? operator (error propagation)
// Generates: check variant, if error then return, else unwrap value
char* llvm_gen_result_propagation(ResultPropagation* prop, LLVMContext* ctx);

// Helper: Extract variant field from result
// Returns: %variant = extractvalue %result_type %result, 0
char* llvm_extract_variant(const char* result_reg, LLVMContext* ctx);

// Helper: Extract data field from result
// Returns: %data = extractvalue %result_type %result, 1
char* llvm_extract_data(const char* result_reg, LLVMContext* ctx);

// Helper: Create result struct
// Returns: %result = insertvalue ... (build struct step by step)
char* llvm_create_result_struct(int variant, const char* data_reg, LLVMContext* ctx);

#endif
