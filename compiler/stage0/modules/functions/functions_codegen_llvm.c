// LLVM IR Code Generator for Functions
// Wrapper around llvm_backend module for functions compiler
// YZ_58 - Phase 13.5 Part 3

#include "functions_codegen_llvm.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../variable/variable.h"
#include "../statement/statement.h"  // YZ_58: Statement types
#include <stdlib.h>
#include <string.h>

// Initialize LLVM codegen context
FunctionLLVMContext* function_llvm_context_create(FILE* output) {
    FunctionLLVMContext* ctx = malloc(sizeof(FunctionLLVMContext));
    ctx->llvm_ctx = llvm_context_create(output);
    ctx->current_func = NULL;
    return ctx;
}

// Free LLVM codegen context
void function_llvm_context_free(FunctionLLVMContext* ctx) {
    if (ctx) {
        llvm_context_free(ctx->llvm_ctx);
        free(ctx);
    }
}

// Generate LLVM IR module header
void function_generate_module_header_llvm(FILE* output) {
    LLVMContext* ctx = llvm_context_create(output);
    llvm_emit_module_header(ctx);
    llvm_context_free(ctx);
}

// Generate LLVM IR module footer
void function_generate_module_footer_llvm(FILE* output) {
    LLVMContext* ctx = llvm_context_create(output);
    llvm_emit_module_footer(ctx);
    llvm_context_free(ctx);
}

// Forward declaration for statement generation
static LLVMValue* generate_statement_llvm(FunctionLLVMContext* ctx, Statement* stmt);
static LLVMValue* generate_expression_llvm(FunctionLLVMContext* ctx, void* expr);

// Generate LLVM IR for expression
static LLVMValue* generate_expression_llvm(FunctionLLVMContext* ctx, void* expr) {
    if (!expr) {
        return llvm_const_i64(0);
    }
    
    ArithmeticExpr* arith = (ArithmeticExpr*)expr;
    
    // Handle literals
    if (arith->is_literal) {
        // Parse numeric literal
        long value = atol(arith->value);
        return llvm_const_i64(value);
    }
    
    // Handle variables
    if (!arith->is_literal && !arith->is_function_call && arith->value) {
        // Check if this is a parameter (no need to load, already in register)
        FunctionParam* param = ctx->current_func->params;
        int is_param = 0;
        while (param) {
            if (strcmp(param->name, arith->value) == 0) {
                is_param = 1;
                break;
            }
            param = param->next;
        }
        
        if (is_param) {
            // Parameter - already a value, just return register reference
            return llvm_reg(arith->value);
        } else {
            // Local variable - need to load from stack
            LLVMValue* var_ptr = llvm_reg(arith->value);
            LLVMValue* loaded = llvm_emit_load(ctx->llvm_ctx, var_ptr);
            llvm_value_free(var_ptr);
            return loaded;
        }
    }
    
    // Handle function calls
    if (arith->is_function_call && arith->func_call) {
        FunctionCallExpr* call = arith->func_call;
        
        // Special case: println
        if (strcmp(call->function_name, "println") == 0 && call->arg_count == 1) {
            LLVMValue* arg = generate_expression_llvm(ctx, call->arguments[0]);
            LLVMValue* result = llvm_emit_println(ctx->llvm_ctx, arg);
            // Don't free arg - println returns it
            return result;
        }
        
        // General function call
        LLVMValue** args = malloc(sizeof(LLVMValue*) * call->arg_count);
        for (int i = 0; i < call->arg_count; i++) {
            args[i] = generate_expression_llvm(ctx, call->arguments[i]);
        }
        
        LLVMValue* result = llvm_emit_call(ctx->llvm_ctx, call->function_name, args, call->arg_count);
        
        // Free args
        for (int i = 0; i < call->arg_count; i++) {
            llvm_value_free(args[i]);
        }
        free(args);
        
        return result;
    }
    
    // Handle binary operations
    if (arith->left && arith->right) {
        LLVMValue* left = generate_expression_llvm(ctx, arith->left);
        LLVMValue* right = generate_expression_llvm(ctx, arith->right);
        
        LLVMValue* result = NULL;
        
        switch (arith->op) {
            case ARITH_ADD:
                result = llvm_emit_add(ctx->llvm_ctx, left, right);
                break;
            case ARITH_SUB:
                result = llvm_emit_sub(ctx->llvm_ctx, left, right);
                break;
            case ARITH_MUL:
                result = llvm_emit_mul(ctx->llvm_ctx, left, right);
                break;
            case ARITH_DIV:
                result = llvm_emit_div(ctx->llvm_ctx, left, right);
                break;
            default:
                result = llvm_const_i64(0);
        }
        
        llvm_value_free(left);
        llvm_value_free(right);
        
        return result;
    }
    
    // Default: return 0
    return llvm_const_i64(0);
}

// Generate LLVM IR for statement
static LLVMValue* generate_statement_llvm(FunctionLLVMContext* ctx, Statement* stmt) {
    if (!stmt) {
        return NULL;
    }
    
    switch (stmt->type) {
        case STMT_VARIABLE_DECL: {
            VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
            
            // Allocate variable
            LLVMValue* var_ptr = llvm_emit_alloca(ctx->llvm_ctx, decl->name);
            
            // Generate initializer expression
            if (decl->init_expr) {
                LLVMValue* init_val = generate_expression_llvm(ctx, decl->init_expr);
                llvm_emit_store(ctx->llvm_ctx, init_val, var_ptr);
                llvm_value_free(init_val);
            } else if (decl->value) {
                // Fallback: use string value
                long val = atol(decl->value);
                LLVMValue* init_val = llvm_const_i64(val);
                llvm_emit_store(ctx->llvm_ctx, init_val, var_ptr);
                llvm_value_free(init_val);
            }
            
            llvm_value_free(var_ptr);
            return NULL;
        }
        
        case STMT_RETURN: {
            ReturnStatement* ret = (ReturnStatement*)stmt->data;
            
            if (ret->return_value) {
                LLVMValue* ret_val = generate_expression_llvm(ctx, ret->return_value);
                llvm_emit_return(ctx->llvm_ctx, ret_val);
                llvm_value_free(ret_val);
            } else {
                LLVMValue* zero = llvm_const_i64(0);
                llvm_emit_return(ctx->llvm_ctx, zero);
                llvm_value_free(zero);
            }
            return NULL;
        }
        
        case STMT_EXPRESSION: {
            // Expression statement (e.g., function call for side effects)
            void* expr = stmt->data;
            LLVMValue* result = generate_expression_llvm(ctx, expr);
            if (result) {
                llvm_value_free(result);
            }
            return NULL;
        }
        
        default:
            // TODO: Handle other statement types (if, while, for)
            return NULL;
    }
}

// Generate LLVM IR for function declaration
void function_generate_declaration_llvm(FunctionLLVMContext* ctx, FunctionDeclaration* func) {
    if (!func) return;
    
    ctx->current_func = func;
    
    // Collect parameter names
    const char** param_names = NULL;
    int param_count = 0;
    
    FunctionParam* param = func->params;
    while (param) {
        param_count++;
        param = param->next;
    }
    
    if (param_count > 0) {
        param_names = malloc(sizeof(char*) * param_count);
        param = func->params;
        int i = 0;
        while (param) {
            param_names[i++] = param->name;
            param = param->next;
        }
    }
    
    // Emit function start
    llvm_emit_function_start(ctx->llvm_ctx, func->name, param_names, param_count);
    llvm_emit_function_entry(ctx->llvm_ctx);
    
    // Generate body statements
    Statement* stmt = func->body;
    while (stmt) {
        generate_statement_llvm(ctx, stmt);
        stmt = stmt->next;
    }
    
    // Emit function end
    llvm_emit_function_end(ctx->llvm_ctx);
    
    // Cleanup
    if (param_names) {
        free((void*)param_names);
    }
}
