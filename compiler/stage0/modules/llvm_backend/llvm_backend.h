// LLVM Backend Module - Header File
// Converts MELP AST to LLVM IR (text format)
// YZ_58 - Phase 13.5 Part 2

#ifndef LLVM_BACKEND_H
#define LLVM_BACKEND_H

#include <stdio.h>
#include <stdint.h>

// Forward declarations
typedef struct LLVMContext LLVMContext;
typedef struct LLVMValue LLVMValue;
typedef struct StringGlobal StringGlobal;

// YZ_61: String global definition (linked list)
struct StringGlobal {
    char* name;         // @.str.1
    char* content;      // Original string value
    StringGlobal* next;
};

// LLVM Context - manages IR generation state
struct LLVMContext {
    FILE* output;           // Output file for IR
    int temp_counter;       // Counter for %1, %2, etc.
    int label_counter;      // Counter for labels
    int string_counter;     // Counter for string constants
    StringGlobal* string_globals;  // YZ_61: Linked list of string constants
};

// LLVM Value - represents a value in IR (register or constant)
struct LLVMValue {
    char* name;             // Register name (e.g., "%result")
    int is_constant;        // 1 if constant, 0 if register
    int64_t const_value;    // Value if constant
};

// ============================================================================
// Context Management
// ============================================================================

// Create new LLVM context
LLVMContext* llvm_context_create(FILE* output);

// Free LLVM context
void llvm_context_free(LLVMContext* ctx);

// Generate new temporary register name
char* llvm_new_temp(LLVMContext* ctx);

// Generate new label name
char* llvm_new_label(LLVMContext* ctx);

// ============================================================================
// Module-Level Emission
// ============================================================================

// Emit module header (type definitions, external declarations)
void llvm_emit_module_header(LLVMContext* ctx);

// Emit module footer (if needed)
void llvm_emit_module_footer(LLVMContext* ctx);

// ============================================================================
// Function Emission
// ============================================================================

// Emit function definition start
// Example: define i64 @add(i64 %a, i64 %b) {
void llvm_emit_function_start(LLVMContext* ctx, const char* name, 
                               const char** param_names, int param_count);

// Emit function entry label
// Example: entry:
void llvm_emit_function_entry(LLVMContext* ctx);

// Emit function end
// Example: }
void llvm_emit_function_end(LLVMContext* ctx);

// Emit return statement
// Example: ret i64 %result
LLVMValue* llvm_emit_return(LLVMContext* ctx, LLVMValue* value);

// ============================================================================
// Variable Emission
// ============================================================================

// Emit variable allocation
// Example: %x = alloca i64, align 8
LLVMValue* llvm_emit_alloca(LLVMContext* ctx, const char* var_name);

// Emit store instruction
// Example: store i64 10, i64* %x, align 8
void llvm_emit_store(LLVMContext* ctx, LLVMValue* value, LLVMValue* ptr);

// Emit load instruction
// Example: %x_val = load i64, i64* %x, align 8
LLVMValue* llvm_emit_load(LLVMContext* ctx, LLVMValue* ptr);

// ============================================================================
// Arithmetic Emission
// ============================================================================

// Emit addition
// Example: %result = add nsw i64 %a, %b
LLVMValue* llvm_emit_add(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// Emit subtraction
// Example: %result = sub nsw i64 %a, %b
LLVMValue* llvm_emit_sub(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// Emit multiplication
// Example: %result = mul nsw i64 %a, %b
LLVMValue* llvm_emit_mul(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// Emit division
// Example: %result = sdiv i64 %a, %b
LLVMValue* llvm_emit_div(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// ============================================================================
// Logical Emission (Boolean Operations)
// ============================================================================

// Emit logical AND
// Example: %result = and i64 %a, %b
LLVMValue* llvm_emit_and(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// Emit logical OR
// Example: %result = or i64 %a, %b
LLVMValue* llvm_emit_or(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// ============================================================================
// Comparison Emission
// ============================================================================

// Emit integer comparison
// Example: %cmp = icmp sgt i64 %a, %b
// op: "eq", "ne", "sgt", "sge", "slt", "sle"
LLVMValue* llvm_emit_icmp(LLVMContext* ctx, const char* op, 
                          LLVMValue* left, LLVMValue* right);

// ============================================================================
// Control Flow Emission
// ============================================================================

// Emit unconditional branch
// Example: br label %target
void llvm_emit_br(LLVMContext* ctx, const char* target_label);

// Emit conditional branch
// Example: br i1 %cond, label %then, label %else
void llvm_emit_br_cond(LLVMContext* ctx, LLVMValue* condition,
                       const char* then_label, const char* else_label);

// Emit label
// Example: then_block:
void llvm_emit_label(LLVMContext* ctx, const char* label_name);

// ============================================================================
// Function Call Emission
// ============================================================================

// Emit function call
// Example: %result = call i64 @add(i64 %a, i64 %b)
LLVMValue* llvm_emit_call(LLVMContext* ctx, const char* func_name,
                          LLVMValue** args, int arg_count);

// ============================================================================
// Constant & Literal Emission
// ============================================================================

// Create constant value
LLVMValue* llvm_const_i64(int64_t value);

// Create register value
LLVMValue* llvm_reg(const char* name);

// Free LLVM value
void llvm_value_free(LLVMValue* value);

// YZ_61: Phase 17 - String Support
// Create string global constant
// Returns global name (@.str.N) - doesn't emit immediately
char* llvm_emit_string_global(LLVMContext* ctx, const char* str_value);

// YZ_61: Emit all collected string globals to output
// Should be called after module header, before function definitions
void llvm_emit_all_string_globals(LLVMContext* ctx);

// ============================================================================
// Printf Integration (for println)
// ============================================================================

// Emit printf declaration and format strings
void llvm_emit_printf_support(LLVMContext* ctx);

// Emit println call (using printf)
// Example: call i32 @printf(i8* @.fmt_num, i64 %value)
LLVMValue* llvm_emit_println(LLVMContext* ctx, LLVMValue* value);

#endif // LLVM_BACKEND_H
