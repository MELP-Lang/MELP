// Test program for LLVM backend
// YZ_58 - Phase 13.5 Part 2

#include "llvm_backend.h"
#include <stdio.h>

int main() {
    // Create LLVM context (output to stdout)
    LLVMContext* ctx = llvm_context_create(stdout);
    
    // Emit module header
    llvm_emit_module_header(ctx);
    
    // ========================================
    // Function: add(a, b)
    // ========================================
    const char* add_params[] = {"a", "b"};
    llvm_emit_function_start(ctx, "add", add_params, 2);
    llvm_emit_function_entry(ctx);
    
    // Create register values for parameters
    LLVMValue* a_reg = llvm_reg("a");
    LLVMValue* b_reg = llvm_reg("b");
    
    // result = a + b
    LLVMValue* add_result = llvm_emit_add(ctx, a_reg, b_reg);
    
    // return result
    llvm_emit_return(ctx, add_result);
    
    llvm_emit_function_end(ctx);
    
    // ========================================
    // Function: main()
    // ========================================
    llvm_emit_function_start(ctx, "main", NULL, 0);
    llvm_emit_function_entry(ctx);
    
    // numeric x = 10
    LLVMValue* x_ptr = llvm_emit_alloca(ctx, "x");
    LLVMValue* const_10 = llvm_const_i64(10);
    llvm_emit_store(ctx, const_10, x_ptr);
    
    // numeric y = 20
    LLVMValue* y_ptr = llvm_emit_alloca(ctx, "y");
    LLVMValue* const_20 = llvm_const_i64(20);
    llvm_emit_store(ctx, const_20, y_ptr);
    
    // Load x and y
    LLVMValue* x_val = llvm_emit_load(ctx, x_ptr);
    LLVMValue* y_val = llvm_emit_load(ctx, y_ptr);
    
    // Call add(x, y)
    LLVMValue* args[] = {x_val, y_val};
    LLVMValue* sum = llvm_emit_call(ctx, "add", args, 2);
    
    // println(sum)
    llvm_emit_println(ctx, sum);
    
    // return 0
    LLVMValue* const_0 = llvm_const_i64(0);
    llvm_emit_return(ctx, const_0);
    
    llvm_emit_function_end(ctx);
    
    // Emit module footer
    llvm_emit_module_footer(ctx);
    
    // Cleanup
    llvm_value_free(a_reg);
    llvm_value_free(b_reg);
    llvm_value_free(add_result);
    llvm_value_free(x_ptr);
    llvm_value_free(y_ptr);
    llvm_value_free(const_10);
    llvm_value_free(const_20);
    llvm_value_free(x_val);
    llvm_value_free(y_val);
    llvm_value_free(sum);
    llvm_value_free(const_0);
    llvm_context_free(ctx);
    
    return 0;
}
