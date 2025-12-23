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
    ctx->string_globals = NULL;  // YZ_61: Initialize globals list
    return ctx;
}

void llvm_context_free(LLVMContext* ctx) {
    if (ctx) {
        // YZ_61: Free string globals list
        StringGlobal* current = ctx->string_globals;
        while (current) {
            StringGlobal* next = current->next;
            free(current->name);
            free(current->content);
            free(current);
            current = next;
        }
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
    // YZ_201: Emit all string globals at end of module
    llvm_emit_all_string_globals(ctx);
}

// ============================================================================
// Function Emission
// ============================================================================

void llvm_emit_function_start(LLVMContext* ctx, const char* name,
                               const char** param_names, int* param_types, int param_count,
                               int return_type) {
    fprintf(ctx->output, "\n; Function: %s\n", name);
    
    // YZ_23: return_type == 1 means string (i8*), 0 means numeric (i64)
    const char* ret_type_str = (return_type == 1) ? "i8*" : "i64";
    fprintf(ctx->output, "define %s @%s(", ret_type_str, name);
    
    for (int i = 0; i < param_count; i++) {
        if (i > 0) fprintf(ctx->output, ", ");
        // YZ_63: param_types[i] == 1 means string (i8*), 0 means numeric (i64)
        if (param_types && param_types[i] == 1) {
            fprintf(ctx->output, "i8* %%%s", param_names[i]);
        } else {
            fprintf(ctx->output, "i64 %%%s", param_names[i]);
        }
    }
    
    fprintf(ctx->output, ") {\n");
}

void llvm_emit_function_entry(LLVMContext* ctx) {
    fprintf(ctx->output, "entry:\n");
}

void llvm_emit_function_end(LLVMContext* ctx) {
    fprintf(ctx->output, "}\n");
}

LLVMValue* llvm_emit_return(LLVMContext* ctx, LLVMValue* value, int return_type) {
    // YZ_23: return_type == 1 means string (i8*), 0 means numeric (i64)
    const char* ret_type_str = (return_type == 1) ? "i8*" : "i64";
    
    if (value->is_constant) {
        fprintf(ctx->output, "    ret %s %ld\n", ret_type_str, value->const_value);
    } else {
        fprintf(ctx->output, "    ret %s %s\n", ret_type_str, value->name);
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
    ptr->type = LLVM_TYPE_I64;  // YZ_64: Alloca returns pointer (treated as i64)
    
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
    result->type = LLVM_TYPE_I64;  // YZ_64: Load always returns i64 for now
    
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
    result->type = LLVM_TYPE_I64;  // YZ_64: Arithmetic always returns i64
    
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
    result->type = LLVM_TYPE_I64;  // YZ_64: Division always returns i64
    
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
// Logical Operations (Boolean AND/OR)
// ============================================================================

static LLVMValue* llvm_emit_logical_binop(LLVMContext* ctx, const char* op,
                                           LLVMValue* left, LLVMValue* right) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    result->type = LLVM_TYPE_I64;  // YZ_64: Logical ops return i64
    
    if (left->is_constant && right->is_constant) {
        // Both constants - fold at compile time
        int64_t result_val = 0;
        if (strcmp(op, "and") == 0) {
            result_val = left->const_value & right->const_value;
        } else if (strcmp(op, "or") == 0) {
            result_val = left->const_value | right->const_value;
        }
        result->is_constant = 1;
        result->const_value = result_val;
        return result;
    }
    
    // Logical ops don't use nsw flag
    fprintf(ctx->output, "    %s = %s i64 ", result->name, op);
    
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

LLVMValue* llvm_emit_and(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    return llvm_emit_logical_binop(ctx, "and", left, right);
}

LLVMValue* llvm_emit_or(LLVMContext* ctx, LLVMValue* left, LLVMValue* right) {
    return llvm_emit_logical_binop(ctx, "or", left, right);
}

// ============================================================================
// Comparison Emission
// ============================================================================

LLVMValue* llvm_emit_icmp(LLVMContext* ctx, const char* op,
                          LLVMValue* left, LLVMValue* right) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    result->type = LLVM_TYPE_I1;  // YZ_64: Comparison returns i1 (boolean)
    
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

// YZ_202: Emit select instruction (ternary)
// %result = select i1 %cond, double %true_val, double %false_val
LLVMValue* llvm_emit_select(LLVMContext* ctx, LLVMValue* condition,
                            LLVMValue* true_val, LLVMValue* false_val) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    result->type = LLVM_TYPE_I64;  // Use i64 for now (works for numeric)
    
    fprintf(ctx->output, "    %s = select i1 ", result->name);
    
    if (condition->is_constant) {
        fprintf(ctx->output, "%ld", condition->const_value);
    } else {
        fprintf(ctx->output, "%s", condition->name);
    }
    
    fprintf(ctx->output, ", ");
    
    if (true_val->is_constant) {
        fprintf(ctx->output, "i64 %ld", true_val->const_value);
    } else {
        fprintf(ctx->output, "i64 %s", true_val->name);
    }
    
    fprintf(ctx->output, ", ");
    
    if (false_val->is_constant) {
        fprintf(ctx->output, "i64 %ld", false_val->const_value);
    } else {
        fprintf(ctx->output, "i64 %s", false_val->name);
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
                          LLVMValue** args, int arg_count, const char* return_type_str) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    // Set type for result (for codegen use)
    if (strcmp(return_type_str, "i8*") == 0) {
        result->type = LLVM_TYPE_I8_PTR;
    } else if (strcmp(return_type_str, "i32") == 0) {
        result->type = 2; // Not used, but for completeness
    } else {
        result->type = LLVM_TYPE_I64;
    }
    fprintf(ctx->output, "    %s = call %s @%s(", result->name, return_type_str, func_name);
    for (int i = 0; i < arg_count; i++) {
        if (i > 0) fprintf(ctx->output, ", ");
        if (args[i]->type == LLVM_TYPE_I8_PTR) {
            fprintf(ctx->output, "i8* ");
        } else {
            fprintf(ctx->output, "i64 ");
        }
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
    result->type = LLVM_TYPE_I64;  // YZ_64: Default to i64
    return result;
}

LLVMValue* llvm_reg(const char* name) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = malloc(strlen(name) + 2);
    sprintf(result->name, "%%%s", name);
    result->is_constant = 0;
    result->type = LLVM_TYPE_I64;  // YZ_64: Default to i64
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

// YZ_61: Phase 17 - String Support
// Register string global constant (doesn't emit immediately)
char* llvm_emit_string_global(LLVMContext* ctx, const char* str_value) {
    // Generate unique global name
    char* global_name = malloc(64);
    snprintf(global_name, 64, "@.str.%d", ctx->string_counter++);
    
    // Create string global node
    StringGlobal* sg = malloc(sizeof(StringGlobal));
    sg->name = strdup(global_name);
    sg->content = strdup(str_value);
    sg->next = ctx->string_globals;
    ctx->string_globals = sg;
    
    return global_name;
}

// YZ_61: Emit all collected string globals
void llvm_emit_all_string_globals(LLVMContext* ctx) {
    if (!ctx->string_globals) {
        return;  // No strings to emit
    }
    
    fprintf(ctx->output, "\n; String Constants\n");
    
    // Reverse the list to maintain declaration order
    StringGlobal* reversed = NULL;
    StringGlobal* current = ctx->string_globals;
    while (current) {
        StringGlobal* next = current->next;
        current->next = reversed;
        reversed = current;
        current = next;
    }
    
    // Emit each string global
    current = reversed;
    while (current) {
        size_t len = strlen(current->content) + 1;
        
        fprintf(ctx->output, "%s = private unnamed_addr constant [%zu x i8] c\"", 
                current->name, len);
        
        // Emit string content with escape sequences
        for (const char* p = current->content; *p != '\0'; p++) {
            switch (*p) {
                case '\n': fprintf(ctx->output, "\\0A"); break;
                case '\t': fprintf(ctx->output, "\\09"); break;
                case '\r': fprintf(ctx->output, "\\0D"); break;
                case '\\': fprintf(ctx->output, "\\\\"); break;
                case '"':  fprintf(ctx->output, "\\22"); break;
                default:
                    if (*p >= 32 && *p <= 126) {
                        fprintf(ctx->output, "%c", *p);
                    } else {
                        fprintf(ctx->output, "\\%02X", (unsigned char)*p);
                    }
            }
        }
        
        // Null terminator
        fprintf(ctx->output, "\\00\", align 1\n");
        
        current = current->next;
    }
    
    fprintf(ctx->output, "\n");
}

// YZ_64: Emit getelementptr to get i8* from string global
LLVMValue* llvm_emit_string_ptr(LLVMContext* ctx, const char* global_name, size_t str_len) {
    LLVMValue* result = malloc(sizeof(LLVMValue));
    result->name = llvm_new_temp(ctx);
    result->is_constant = 0;
    result->type = LLVM_TYPE_I8_PTR;  // String pointer type
    
    // getelementptr inbounds [N x i8], [N x i8]* @.str.X, i64 0, i64 0
    fprintf(ctx->output, "    %s = getelementptr inbounds [%zu x i8], [%zu x i8]* %s, i64 0, i64 0\n",
            result->name, str_len, str_len, global_name);
    
    return result;
}

// ============================================================================
// MLP Standard Library Integration (YZ_61 - Phase 15)
// ============================================================================

void llvm_emit_printf_support(LLVMContext* ctx) {
    fprintf(ctx->output, "; MLP Standard Library - I/O Functions\n");
    fprintf(ctx->output, "; void mlp_println_numeric(void* value, uint8_t sto_type)\n");
    fprintf(ctx->output, "declare void @mlp_println_numeric(i8*, i8)\n");
    fprintf(ctx->output, "; void mlp_println_string(const char* str)\n");
    fprintf(ctx->output, "declare void @mlp_println_string(i8*)\n\n");
    
    fprintf(ctx->output, "; C Standard Library - Memory Allocation\n");
    fprintf(ctx->output, "; void* malloc(size_t size)\n");
    fprintf(ctx->output, "declare i8* @malloc(i64)\n\n");
    
    fprintf(ctx->output, "; MLP Standard Library - String Functions (Task 0.2 + 0.3)\n");
    fprintf(ctx->output, "; char* mlp_string_concat(const char* str1, const char* str2)\n");
    fprintf(ctx->output, "declare i8* @mlp_string_concat(i8*, i8*)\n");
    fprintf(ctx->output, "; char* mlp_string_char_at(const char* str, size_t index)\n");
    fprintf(ctx->output, "declare i8* @mlp_string_char_at(i8*, i64)\n\n");
    
    fprintf(ctx->output, "; MLP Standard Library - List Functions (YZ_200)\n");
    fprintf(ctx->output, "; MelpList* melp_list_create(size_t element_size)\n");
    fprintf(ctx->output, "declare i8* @melp_list_create(i64)\n");
    fprintf(ctx->output, "; int melp_list_append(MelpList* list, void* element)\n");
    fprintf(ctx->output, "declare i32 @melp_list_append(i8*, i8*)\n");
    fprintf(ctx->output, "; int melp_list_prepend(MelpList* list, void* element)\n");
    fprintf(ctx->output, "declare i32 @melp_list_prepend(i8*, i8*)\n");
    fprintf(ctx->output, "; size_t melp_list_length(MelpList* list)\n");
    fprintf(ctx->output, "declare i64 @melp_list_length(i8*)\n");
    fprintf(ctx->output, "; void* melp_list_get(MelpList* list, size_t index)\n");
    fprintf(ctx->output, "declare i8* @melp_list_get(i8*, i64)\n");
    fprintf(ctx->output, "; int melp_list_set(MelpList* list, size_t index, void* element)\n");
    fprintf(ctx->output, "declare i32 @melp_list_set(i8*, i64, i8*)\n\n");
    
    fprintf(ctx->output, "; MLP Standard Library - Map Functions (YZ_201)\n");
    fprintf(ctx->output, "; MelpMap* melp_map_create(size_t value_size)\n");
    fprintf(ctx->output, "declare i8* @melp_map_create(i64)\n");
    fprintf(ctx->output, "; int melp_map_insert(MelpMap* map, const char* key, const void* value)\n");
    fprintf(ctx->output, "declare i32 @melp_map_insert(i8*, i8*, i8*)\n");
    fprintf(ctx->output, "; void* melp_map_get(MelpMap* map, const char* key)\n");
    fprintf(ctx->output, "declare i8* @melp_map_get(i8*, i8*)\n");
    fprintf(ctx->output, "; int melp_map_remove(MelpMap* map, const char* key)\n");
    fprintf(ctx->output, "declare i32 @melp_map_remove(i8*, i8*)\n");
    fprintf(ctx->output, "; int melp_map_has_key(MelpMap* map, const char* key)\n");
    fprintf(ctx->output, "declare i32 @melp_map_has_key(i8*, i8*)\n");
    fprintf(ctx->output, "; size_t melp_map_length(MelpMap* map)\n");
    fprintf(ctx->output, "declare i64 @melp_map_length(i8*)\n\n");
}

LLVMValue* llvm_emit_println(LLVMContext* ctx, LLVMValue* value) {
    // Task 0.2/0.3: Check if value is a string (i8*)
    if (value->type == LLVM_TYPE_I8_PTR) {
        // String: call mlp_println_string
        fprintf(ctx->output, "    call void @mlp_println_string(i8* ");
        if (value->is_constant) {
            fprintf(ctx->output, "null");  // Shouldn't happen
        } else {
            fprintf(ctx->output, "%s", value->name);
        }
        fprintf(ctx->output, ")\n");
        return value;
    }
    
    // Numeric: use printf with format string
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
