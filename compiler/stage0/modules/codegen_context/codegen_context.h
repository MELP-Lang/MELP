#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "sto_types.h"  // STO type definitions

// ============================================================================
// Forward Declarations & Constants
// ============================================================================

#define MAX_LOOP_DEPTH 32
#define MAX_GLOBAL_VARS 1024

// Loop context for break/continue
typedef struct {
    int exit_label;
    int continue_label;
} LoopContext;

// Variable info (legacy)
typedef struct {
    char* name;
    int type;  // 0=numeric, 1=string, 2=boolean
} VarInfo;

// Function context (forward declaration)
typedef struct FunctionContext {
    char* name;
    int stack_size;
    int param_count;
    void** params;  // Array of parameter structs (opaque pointer)
} FunctionContext;

// ============================================================================
// STO (Transparent Type Optimization) Definitions
// ============================================================================
// Note: STOTypeInfo and InternalType are defined in sto_types.h

// Variable info with STO
typedef struct {
    char* name;
    InternalType internal_type;  // STO-inferred type
    STOTypeInfo sto_info;        // Full STO analysis
    int offset;                  // Stack offset or heap pointer
} STOVarInfo;

// ============================================================================
// Original Codegen Context
// ============================================================================

// Global codegen state - centralized context
typedef struct {
    // Label generation
    int label_counter;
    
    // String literal tracking
    int print_string_counter;
    
    // Loop context stack (for exit/continue)
    LoopContext loop_stack[MAX_LOOP_DEPTH];
    int loop_depth;
    
    // Global variable type tracking
    VarInfo global_var_types[MAX_GLOBAL_VARS];
    int global_var_count;
    
    // STO: Enhanced variable tracking
    STOVarInfo sto_var_types[MAX_GLOBAL_VARS];
    int sto_var_count;
    
    // Current function context
    FunctionContext* current_function_ctx;
    
    // Output file handle
    FILE* output_file;
    
    // STO: Optimization flags
    bool sto_enabled;            // Enable STO optimization
    bool sto_aggressive;         // Aggressive int64 usage
    bool sto_track_overflow;     // Runtime overflow detection
} CodegenContext;

// Context management
CodegenContext* codegen_context_create(FILE* output_file);
void codegen_context_destroy(CodegenContext* ctx);

// Label generation
int codegen_context_next_label(CodegenContext* ctx);

// String counter
int codegen_context_next_string_id(CodegenContext* ctx);

// Loop stack management
void codegen_context_push_loop(CodegenContext* ctx, int exit_label, int continue_label);
void codegen_context_pop_loop(CodegenContext* ctx);
LoopContext* codegen_context_current_loop(CodegenContext* ctx);

// Variable type tracking
void codegen_context_add_var(CodegenContext* ctx, const char* name, int type);
int codegen_context_get_var_type(CodegenContext* ctx, const char* name);

// Function context
void codegen_context_set_function(CodegenContext* ctx, FunctionContext* func_ctx);
FunctionContext* codegen_context_get_function(CodegenContext* ctx);

// ============================================================================
// STO Functions
// ============================================================================

// Type inference (compile-time, renamed to avoid clash with runtime STO)
STOTypeInfo codegen_tto_infer_numeric_type(const char* literal);
STOTypeInfo codegen_tto_infer_string_type(const char* literal, bool is_constant);

// Variable tracking
void sto_add_var(CodegenContext* ctx, const char* name, STOTypeInfo info);
STOVarInfo* sto_get_var(CodegenContext* ctx, const char* name);

// Optimization decisions
bool sto_should_use_int64(int64_t value);
bool sto_should_use_double(const char* literal);
bool sto_should_use_sso(size_t string_length);

// Promotion helpers
void sto_generate_int64_to_bigdec_promotion(CodegenContext* ctx, const char* var_name);
void sto_generate_overflow_check(CodegenContext* ctx, const char* operation);

#endif
