#include "result_type_codegen.h"
#include "../error/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Result Type LLVM IR Codegen - YZ_206
 * Phase 5: Code generation
 */

// Generate LLVM type for result<T, E>
char* llvm_gen_result_type(Type* result_type) {
    if (!result_type || !is_result_type(result_type)) {
        return NULL;
    }

    // For now, use a simple representation: { i8, i64 }
    // i8 = variant (0 = ok, 1 = error)
    // i64 = data (value or error)
    return strdup("{ i8, i64 }");
}

// Generate LLVM IR for ok(value) constructor
char* llvm_gen_ok_constructor(Expression* ok_expr, LLVMContext* ctx) {
    if (!ok_expr || ok_expr->type != EXPR_RESULT_OK || !ctx) {
        return NULL;
    }

    // Get value expression
    Expression* value_expr = ok_expr->data.result_constructor.value;
    if (!value_expr) {
        return NULL;
    }

    char buffer[2048];
    char* result = NULL;

    // Generate code for value expression
    // For now, handle simple cases
    if (value_expr->type == EXPR_NUMBER) {
        // ok(42) -> { i8 0, i64 42 }
        int tmp_reg = ctx->temp_counter++;
        snprintf(buffer, sizeof(buffer),
                 "  %%tmp%d = insertvalue { i8, i64 } undef, i8 0, 0\n"
                 "  %%tmp%d = insertvalue { i8, i64 } %%tmp%d, i64 %ld, 1",
                 tmp_reg, tmp_reg + 1, tmp_reg, value_expr->data.number_value);
        ctx->temp_counter++;
        result = strdup(buffer);
    } else if (value_expr->type == EXPR_VARIABLE) {
        // ok(x) -> load x, create result
        int tmp_reg = ctx->temp_counter++;
        int load_reg = tmp_reg + 1;
        snprintf(buffer, sizeof(buffer),
                 "  %%tmp%d = load i64, i64* %%%s\n"
                 "  %%tmp%d = insertvalue { i8, i64 } undef, i8 0, 0\n"
                 "  %%tmp%d = insertvalue { i8, i64 } %%tmp%d, i64 %%tmp%d, 1",
                 load_reg, value_expr->data.var_name,
                 tmp_reg, tmp_reg + 2, tmp_reg, load_reg);
        ctx->temp_counter += 3;
        result = strdup(buffer);
    } else {
        error_codegen(0, "Unsupported value type in ok() constructor");
        return NULL;
    }

    return result;
}

// Generate LLVM IR for error(msg) constructor
char* llvm_gen_error_constructor(Expression* error_expr, LLVMContext* ctx) {
    if (!error_expr || error_expr->type != EXPR_RESULT_ERROR || !ctx) {
        return NULL;
    }

    // Get error expression
    Expression* err_value_expr = error_expr->data.result_constructor.value;
    if (!err_value_expr) {
        return NULL;
    }

    char buffer[2048];
    char* result = NULL;

    // Generate code for error expression
    if (err_value_expr->type == EXPR_STRING) {
        // error("msg") -> { i8 1, i64 <string_ptr> }
        // For now, store error code instead of string
        int tmp_reg = ctx->temp_counter++;
        snprintf(buffer, sizeof(buffer),
                 "  %%tmp%d = insertvalue { i8, i64 } undef, i8 1, 0\n"
                 "  %%tmp%d = insertvalue { i8, i64 } %%tmp%d, i64 0, 1  ; TODO: String error",
                 tmp_reg, tmp_reg + 1, tmp_reg);
        ctx->temp_counter += 2;
        result = strdup(buffer);
    } else if (err_value_expr->type == EXPR_NUMBER) {
        // error(code) -> { i8 1, i64 code }
        int tmp_reg = ctx->temp_counter++;
        snprintf(buffer, sizeof(buffer),
                 "  %%tmp%d = insertvalue { i8, i64 } undef, i8 1, 0\n"
                 "  %%tmp%d = insertvalue { i8, i64 } %%tmp%d, i64 %ld, 1",
                 tmp_reg, tmp_reg + 1, tmp_reg, err_value_expr->data.number_value);
        ctx->temp_counter += 2;
        result = strdup(buffer);
    } else {
        error_codegen(0, "Unsupported error type in error() constructor");
        return NULL;
    }

    return result;
}

// Generate LLVM IR for match expression
char* llvm_gen_result_match(ResultMatch* match, LLVMContext* ctx) {
    if (!match || !ctx) {
        return NULL;
    }

    // TODO: Full implementation
    // This requires:
    // 1. Extract variant from result
    // 2. Create switch on variant
    // 3. Generate ok block with binding
    // 4. Generate error block with binding
    // 5. Generate merge block

    error_codegen(0, "match expression codegen not yet implemented");
    return NULL;
}

// Generate LLVM IR for ? operator
char* llvm_gen_result_propagation(ResultPropagation* prop, LLVMContext* ctx) {
    if (!prop || !ctx) {
        return NULL;
    }

    // TODO: Full implementation
    // This requires:
    // 1. Extract variant from result
    // 2. Check if variant == 1 (error)
    // 3. If error, early return with error
    // 4. If ok, extract and return value

    error_codegen(0, "? operator codegen not yet implemented");
    return NULL;
}

// Helper: Extract variant field
char* llvm_extract_variant(const char* result_reg, LLVMContext* ctx) {
    if (!result_reg || !ctx) {
        return NULL;
    }

    char buffer[512];
    int tmp_reg = ctx->temp_counter++;
    snprintf(buffer, sizeof(buffer),
             "  %%tmp%d = extractvalue { i8, i64 } %s, 0",
             tmp_reg, result_reg);
    return strdup(buffer);
}

// Helper: Extract data field
char* llvm_extract_data(const char* result_reg, LLVMContext* ctx) {
    if (!result_reg || !ctx) {
        return NULL;
    }

    char buffer[512];
    int tmp_reg = ctx->temp_counter++;
    snprintf(buffer, sizeof(buffer),
             "  %%tmp%d = extractvalue { i8, i64 } %s, 1",
             tmp_reg, result_reg);
    return strdup(buffer);
}

// Helper: Create result struct
char* llvm_create_result_struct(int variant, const char* data_reg, LLVMContext* ctx) {
    if (!data_reg || !ctx) {
        return NULL;
    }

    char buffer[1024];
    int tmp_reg = ctx->temp_counter++;
    snprintf(buffer, sizeof(buffer),
             "  %%tmp%d = insertvalue { i8, i64 } undef, i8 %d, 0\n"
             "  %%tmp%d = insertvalue { i8, i64 } %%tmp%d, i64 %s, 1",
             tmp_reg, variant, tmp_reg + 1, tmp_reg, data_reg);
    ctx->temp_counter += 2;
    return strdup(buffer);
}
