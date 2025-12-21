#include "function_pointer_codegen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// YZ_209: Function Pointer Code Generation Implementation

// Helper: Get LLVM type from MELP type
const char* get_llvm_type_for_melp(const char* melp_type) {
    if (strcmp(melp_type, "numeric") == 0) return "i64";
    if (strcmp(melp_type, "float") == 0) return "double";
    if (strcmp(melp_type, "string") == 0) return "i8*";
    if (strcmp(melp_type, "bool") == 0) return "i1";
    if (strcmp(melp_type, "void") == 0) return "void";
    
    // Default to pointer for complex types
    return "i8*";
}

// Generate function type string
// Example: "i64 (i64, i64)*"
char* generate_function_type_string(FunctionPointerType* type) {
    if (!type) return NULL;
    
    // Calculate required buffer size
    int size = 100;  // Start with reasonable size
    for (int i = 0; i < type->param_count; i++) {
        size += 50;
    }
    
    char* result = malloc(size);
    if (!result) return NULL;
    
    // Build type string
    const char* return_llvm = get_llvm_type_for_melp(type->return_type);
    sprintf(result, "%s (", return_llvm);
    
    // Add parameter types
    for (int i = 0; i < type->param_count; i++) {
        const char* param_llvm = get_llvm_type_for_melp(type->param_types[i]);
        strcat(result, param_llvm);
        if (i < type->param_count - 1) {
            strcat(result, ", ");
        }
    }
    
    strcat(result, ")*");
    
    return result;
}

// Generate LLVM type for function pointer
void codegen_function_pointer_type(FILE* output, FunctionPointerType* type) {
    if (!output || !type) return;
    
    char* type_str = generate_function_type_string(type);
    if (type_str) {
        fprintf(output, "%s", type_str);
        free(type_str);
    }
}

// Generate function pointer variable declaration
void codegen_function_pointer_var(FILE* output, FunctionPointerVar* var, LLVMContext* ctx) {
    if (!output || !var || !ctx) return;
    
    // Allocate stack space for function pointer
    char* type_str = generate_function_type_string(var->type);
    if (!type_str) return;
    
    char* reg = llvm_new_temp(ctx);
    fprintf(output, "  %s = alloca %s, align 8\n", reg, type_str);
    
    // Note: In a full implementation, store variable mapping
    free(reg);
    free(type_str);
}

// Generate function reference
void codegen_function_reference(FILE* output, FunctionReference* ref, LLVMContext* ctx) {
    if (!output || !ref || !ctx) return;
    
    // Get function address using bitcast
    char* type_str = generate_function_type_string(ref->type);
    if (!type_str) return;
    
    char* reg = llvm_new_temp(ctx);
    
    // Get function type without pointer
    const char* return_llvm = get_llvm_type_for_melp(ref->type->return_type);
    fprintf(output, "  %s = bitcast %s (", reg, return_llvm);
    
    // Add parameter types
    for (int i = 0; i < ref->type->param_count; i++) {
        const char* param_llvm = get_llvm_type_for_melp(ref->type->param_types[i]);
        fprintf(output, "%s", param_llvm);
        if (i < ref->type->param_count - 1) {
            fprintf(output, ", ");
        }
    }
    
    fprintf(output, ")* @%s to %s\n", ref->function_name, type_str);
    
    free(reg);
    free(type_str);
}

// Generate function pointer assignment
void codegen_function_pointer_assign(FILE* output, FunctionPointerVar* var, 
                                     FunctionReference* ref, LLVMContext* ctx) {
    if (!output || !var || !ref || !ctx) return;
    
    // Generate function reference (get address)
    char* ref_reg = llvm_new_temp(ctx);
    codegen_function_reference(output, ref, ctx);
    
    // For simplified implementation, assume variable pointer is already known
    char* var_reg = llvm_new_temp(ctx);
    
    // Store function pointer
    char* type_str = generate_function_type_string(var->type);
    if (!type_str) {
        free(ref_reg);
        free(var_reg);
        return;
    }
    
    fprintf(output, "  store %s %s, %s* %s, align 8\n", 
            type_str, ref_reg, type_str, var_reg);
    
    free(ref_reg);
    free(var_reg);
    free(type_str);
}

// Generate indirect function call
LLVMValue* codegen_indirect_call(FILE* output, IndirectCall* call, LLVMContext* ctx) {
    if (!output || !call || !ctx) return NULL;
    
    // Simplified implementation for now
    if (!call->resolved_ptr || !call->resolved_ptr->type) {
        fprintf(stderr, "Error: Function pointer not resolved\n");
        return NULL;
    }
    
    char* type_str = generate_function_type_string(call->resolved_ptr->type);
    if (!type_str) return NULL;
    
    // Load function pointer
    char* load_reg = llvm_new_temp(ctx);
    char* var_reg = llvm_new_temp(ctx);
    
    fprintf(output, "  %s = load %s, %s* %s, align 8\n",
            load_reg, type_str, type_str, var_reg);
    
    // Generate call
    char* result_reg = llvm_new_temp(ctx);
    const char* return_llvm = get_llvm_type_for_melp(call->resolved_ptr->type->return_type);
    
    fprintf(output, "  %s = call %s %s(", result_reg, return_llvm, load_reg);
    
    // Add arguments (simplified - assumes arguments are already in registers)
    for (int i = 0; i < call->arg_count; i++) {
        const char* param_llvm = get_llvm_type_for_melp(call->resolved_ptr->type->param_types[i]);
        fprintf(output, "%s %%arg%d", param_llvm, i);
        if (i < call->arg_count - 1) {
            fprintf(output, ", ");
        }
    }
    
    fprintf(output, ")\n");
    
    free(load_reg);
    free(var_reg);
    free(type_str);
    
    // Create return value (simplified - just allocate without proper tracking)
    LLVMValue* result = malloc(sizeof(LLVMValue));
    if (result) {
        result->name = result_reg;
        result->is_constant = 0;
        result->const_value = 0;
        result->type = LLVM_TYPE_I64;  // Simplified
    }
    
    return result;
}
