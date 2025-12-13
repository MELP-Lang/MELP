// LLVM Backend Module - Implementation
// Converts MELP AST to LLVM IR (text format)
// YZ_58 - Phase 13.5 Part 2

#include "llvm_backend.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Context Management
// ============================================================================

LLVMContext* llvm_context_create(FILE* output) {
    LLVMContext* ctx = malloc(sizeof(LLVMContext));
    ctx->output = output;
    ctx->temp_counter = 1;
    ctx->label_counter = 1;
    ctx->string_counter = 1;
    return ctx;
}

void llvm_context_free(LLVMContext* ctx) {
    if (ctx) {
        free(ctx);
    }
}

char* llvm_new_temp(LLVMContext* ctx) {
    char* name = malloc(32);
    snprintf(name, 32, "%%tmp%d", ctx->temp_counter++);
    return name;
}

char* llvm_new_label(LLVMContext* ctx) {
    char* name = malloc(32);
    snprintf(name, 32, "label%d", ctx->label_counter++);
    return name;
}

// ============================================================================
// Module-Level Emission
// ============================================================================

void llvm_emit_module_header(LLVMContext* ctx) {
    fprintf(ctx->output, "; MELP Program - Generated LLVM IR\n");
    fprintf(ctx->output, "; Compiler: MELP Stage 0 with LLVM Backend\n");
    fprintf(ctx->output, "; Date: 13 Aralık 2025\n\n");
    
    // Type definitions
    fprintf(ctx->output, "; Type definitions\n");
    fprintf(ctx->output, "%%sto_value = type { i8, i64 }\n\n");
    
    // Printf declaration for println support
    llvm_emit_printf_support(ctx);
}

void llvm_emit_module_footer(LLVMContext* ctx) {
    // Nothing needed for now
}

// ============================================================================
// Function Emission
// ============================================================================

void llvm_emit_function_start(LLVMContext* ctx, const char* name,
                               const char** param_names, int param_count) {
    fprintf(ctx->output, "\n; Function: %s\n", name);
    fprintf(ctx->output, "define i64 @%s(", name);
    
    for (int i = 0; i < param_count; i++) {
        if (i > 0) fprintf(ctx->output, ", ");
        fprintf(ctx->output, "i64 %%%s", param_names[i]);
    }
    
    fprintf(ctx->output, ") {\n");
}

void llvm_emit_function_entry(LLVMContext* ctx) {
    fprintf(ctx->output, "entry:\n");
}

void llvm_emit_function_end(LLVMContext* ctx) {
    fprintf(ctx->output, "}\n");
}

LLVMValue* llvm_emit_return(LLVMContext* ctx, LLVMValue* value) {
    if (value->is_constant) {
        fprintf(ctx->output, "    ret i64 %ld\n", value->const_value);
    } else {
        fprintf(ctx->output, "    ret i64 %s\n", value->name);
    }
    return value;
}

// ============================================================================
// Variable Emission
// ============================================================================

LLVMValue* llvm_emit_alloca(LLVMContext* ctx, const char* var_name) {
    LLVMValue* ptr = malloc(sizeof(LLVMValue));
    ptr->name = malloc(strlen(var_name) + 2);
    sprintf(ptr->name, "%%%s", var_name);
    ptr->is_constant = 0;
    
    fprintf(ctx->output, "    %s = alloca i64, align 8\n", ptr->name);
    return ptr;
}

void llvm_emit_store(LLVMContext* ctx, LLVMValue* value, LLVMValue* ptr) {
    if (value->is_constant) {
        fprintf(ctx->output, "    store i64 %ld, i64* %s, align 8\n",
                value->const_value, ptr->name);
    } else {
        fprintf(ctx->output, "    store i64 %s, i64* %s, align 8\n",
                value->name, ptr->name);
    }
}

LLVMValue* llvm_emit_load(LLVMContext* ctx, LLVMValue* ptr) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    
    fprintf(ctx->output, "    %s = load i64, i64* %s, align 8\n",
            result->name, ptr->name);
    return result;
}

// ============================================================================
// Arithmetic Emission
// ============================================================================

static LLVMValue* llvm_emit_binop(LLVMContext* ctx, const char* op,
                                   LLVMValue* left, LLVMValue* right) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    
    const char* left_str = left->is_constant ? "" : left->name;
    const char* right_str = right->is_constant ? "" : right->name;
    
    if (left->is_constant && right->is_constant) {
        // Both constants - fold at compile time
        int64_t result_val = 0;
        if (strcmp(op, "add") == 0) {
            result_val = left->const_value + right->const_value;
        } else if (strcmp(op, "sub") == 0) {
            result_val = left->const_value - right->const_value;
        } else if (strcmp(op, "mul") == 0) {
            result_val = left->const_value * right->const_value;
        } else if (strcmp(op, "sdiv") == 0) {
            result_val = left->const_value / right->const_value;
        }
        result->is_constant = 1;
        result->const_value = result_val;
        return result;
    }
    
    fprintf(ctx->output, "    %s = %s nsw i64 ", result->name, op);
    
    if (left->is_constant) {
        fprintf(ctx->output, "%ld", left->const_value);
    } else {
        fprintf(ctx->output, "%s", left->name);
    }
    
    fprintf(ctx->output, ", ");
    
    if (right->is_constant) {
        fprintf(ctx->output, "%ld", right->const_value);
    } else {
        fprintf(ctx->output, "%s", right->name);
    }
    
    fprintf(ctx->output, "\n");
    return result;
}

LLVMValue* llvm_emit_add(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    return llvm_emit_binop(ctx, "add", left, right);
}

LLVMValue* llvm_emit_sub(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    return llvm_emit_binop(ctx, "sub", left, right);
}

LLVMValue* llvm_emit_mul(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    return llvm_emit_binop(ctx, "mul", left, right);
}

LLVMValue* llvm_emit_div(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    // Note: sdiv doesn't support nsw flag
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    
    fprintf(ctx->output, "    %s = sdiv i64 ", result->name);
    
    if (left->is_constant) {
        fprintf(ctx->output, "%ld", left->const_value);
    } else {
        fprintf(ctx->output, "%s", left->name);
    }
    
    fprintf(ctx->output, ", ");
    
    if (right->is_constant) {
        fprintf(ctx->output, "%ld", right->const_value);
    } else {
        fprintf(ctx->output, "%s", right->name);
    }
    
    fprintf(ctx->output, "\n");
    return result;
}

// ============================================================================
// Comparison Emission
// ============================================================================

LLVMValue* llvm_emit_icmp(LLVMContext* ctx, const char* op,
                          LLVMValue* left, LLVMValue* right) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    
    fprintf(ctx->output, "    %s = icmp %s i64 ", result->name, op);
    
    if (left->is_constant) {
        fprintf(ctx->output, "%ld", left->const_value);
    } else {
        fprintf(ctx->output, "%s", left->name);
    }
    
    fprintf(ctx->output, ", ");
    
    if (right->is_constant) {
        fprintf(ctx->output, "%ld", right->const_value);
    } else {
        fprintf(ctx->output, "%s", right->name);
    }
    
    fprintf(ctx->output, "\n");
    return result;
}

// ============================================================================
// Control Flow Emission
// ============================================================================

void llvm_emit_br(LLVMContext* ctx, const char* target_label) {
    fprintf(ctx->output, "    br label %%%s\n", target_label);
}

void llvm_emit_br_cond(LLVMContext* ctx, LLVMValue* condition,
                       const char* then_label, const char* else_label) {
    fprintf(ctx->output, "    br i1 %s, label %%%s, label %%%s\n",
            condition->name, then_label, else_label);
}

void llvm_emit_label(LLVMContext* ctx, const char* label_name) {
    fprintf(ctx->output, "%s:\n", label_name);
}

// ============================================================================
// Function Call Emission
// ============================================================================

LLVMValue* llvm_emit_call(LLVMContext* ctx, const char* func_name,
                          LLVMValue** args, int arg_count) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    
    fprintf(ctx->output, "    %s = call i64 @%s(", result->name, func_name);
    
    for (int i = 0; i < arg_count; i++) {
        if (i > 0) fprintf(ctx->output, ", ");
        
        fprintf(ctx->output, "i64 ");
        
        if (args[i]->is_constant) {
            fprintf(ctx->output, "%ld", args[i]->const_value);
        } else {
            fprintf(ctx->output, "%s", args[i]->name);
        }
    }
    
    fprintf(ctx->output, ")\n");
    return result;
}

// ============================================================================
// Constant & Literal Emission
// ============================================================================

LLVMValue* llvm_const_i64(int64_t value) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = NULL;
    result->is_constant = 1;
    result->const_value = value;
    return result;
}

LLVMValue* llvm_reg(const char* name) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = malloc(strlen(name) + 2);
    sprintf(result->name, "%%%s", name);
    result->is_constant = 0;
    return result;
}

void llvm_value_free(LLVMValue* value) {
    if (value) {
        if (value->name) {
            free(value->name);
        }
        free(value);
    }
}

// ============================================================================
// Printf Integration (for println)
// ============================================================================

void llvm_emit_printf_support(LLVMContext* ctx) {
    fprintf(ctx->output, "; External C library function\n");
    fprintf(ctx->output, "declare i32 @printf(i8*, ...)\n\n");
    
    fprintf(ctx->output, "; Format string for numeric println\n");
    fprintf(ctx->output, "@.fmt_num = private unnamed_addr constant [5 x i8] c\"%%ld\\0A\\00\", align 1\n\n");
}

LLVMValue* llvm_emit_println(LLVMContext* ctx, LLVMValue* value) {
    // Get pointer to format string
    char* fmt_ptr = llvm_new_temp(ctx);
    fprintf(ctx->output, "    %s = getelementptr inbounds [5 x i8], [5 x i8]* @.fmt_num, i64 0, i64 0\n",
            fmt_ptr);
    
    // Call printf
    fprintf(ctx->output, "    call i32 (i8*, ...) @printf(i8* %s, i64 ", fmt_ptr);
    
    if (value->is_constant) {
        fprintf(ctx->output, "%ld", value->const_value);
    } else {
        fprintf(ctx->output, "%s", value->name);
    }
    
    fprintf(ctx->output, ")\n");
    
    free(fmt_ptr);
    
    // println returns the value passed to it
    return value;
}
