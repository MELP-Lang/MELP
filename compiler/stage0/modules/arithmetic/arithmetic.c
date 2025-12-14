#include "arithmetic.h"
#include "../codegen_context/codegen_context.h"
#include "../array/array.h"  // YZ_14: For IndexAccess
#include "../array/array_parser.h"  // YZ_17: For collection_free
#include "../struct/struct.h"  // YZ_86: For MethodCall
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

// Free arithmetic expression
void arithmetic_expr_free(ArithmeticExpr* expr) {
    if (!expr) return;
    
    if (expr->left) arithmetic_expr_free(expr->left);
    if (expr->right) arithmetic_expr_free(expr->right);
    if (expr->value) free(expr->value);
    
    // Phase 3.5: Free function call
    if (expr->func_call) {
        if (expr->func_call->function_name) {
            free(expr->func_call->function_name);
        }
        if (expr->func_call->arguments) {
            for (int i = 0; i < expr->func_call->arg_count; i++) {
                arithmetic_expr_free(expr->func_call->arguments[i]);
            }
            free(expr->func_call->arguments);
        }
        free(expr->func_call);
    }
    
    // YZ_14: Free array access
    if (expr->array_access) {
        if (expr->array_access->collection_name) {
            free(expr->array_access->collection_name);
        }
        if (expr->array_access->index_type == 1 && expr->array_access->index.var_index) {
            free(expr->array_access->index.var_index);
        }
        if (expr->array_access->index_type == 2 && expr->array_access->index.expr_index) {
            arithmetic_expr_free((ArithmeticExpr*)expr->array_access->index.expr_index);
        }
        free(expr->array_access);
    }
    
    // YZ_17: Free collection
    if (expr->collection) {
        collection_free(expr->collection);
    }
    
    // YZ_86: Free method call
    if (expr->method_call) {
        method_call_free((MethodCall*)expr->method_call);
    }
    
    // Phase 2.3: Free STO info
    if (expr->sto_info) free(expr->sto_info);
    
    free(expr);
}

// Arithmetic operations module
// This module only contains operation type enums
// No implementation needed for Stage 0

// ============================================================================
// Phase 2.3: STO Type Propagation Implementation
// ============================================================================

// Note: sto_infer_numeric_type() is implemented in codegen_context.c

// Propagate types through binary operations
STOTypeInfo arithmetic_propagate_binary_types(STOTypeInfo* left, STOTypeInfo* right, ArithmeticOp op) {
    STOTypeInfo result = {0};
    result.is_constant = left->is_constant && right->is_constant;
    result.needs_promotion = false;
    
    // Rule 1: If either operand is BIGDECIMAL, result is BIGDECIMAL
    if (left->type == INTERNAL_TYPE_BIGDECIMAL || right->type == INTERNAL_TYPE_BIGDECIMAL) {
        result.type = INTERNAL_TYPE_BIGDECIMAL;
        result.mem_location = MEM_HEAP;
        return result;
    }
    
    // Rule 2: If either operand is DOUBLE, result is DOUBLE
    if (left->type == INTERNAL_TYPE_DOUBLE || right->type == INTERNAL_TYPE_DOUBLE) {
        result.type = INTERNAL_TYPE_DOUBLE;
        result.mem_location = MEM_REGISTER;  // XMM register
        return result;
    }
    
    // Rule 3: Both are INT64
    if (left->type == INTERNAL_TYPE_INT64 && right->type == INTERNAL_TYPE_INT64) {
        // Division always produces DOUBLE (to preserve precision)
        if (op == ARITH_DIV) {
            result.type = INTERNAL_TYPE_DOUBLE;
            result.mem_location = MEM_REGISTER;
            return result;
        }
        
        // For other operations, result is INT64 but may overflow
        result.type = INTERNAL_TYPE_INT64;
        result.mem_location = MEM_REGISTER;
        result.needs_promotion = true;  // May need runtime promotion to BIGDECIMAL
        return result;
    }
    
    // Fallback: BIGDECIMAL for safety
    result.type = INTERNAL_TYPE_BIGDECIMAL;
    result.mem_location = MEM_HEAP;
    return result;
}

// Infer STO type for arithmetic expression (recursive)
void arithmetic_infer_sto_type(ArithmeticExpr* expr) {
    if (!expr || expr->sto_analyzed) {
        return;  // Already analyzed
    }
    
    // Analyze children first
    if (expr->left) {
        arithmetic_infer_sto_type(expr->left);
    }
    if (expr->right) {
        arithmetic_infer_sto_type(expr->right);
    }
    
    // If both children have STO info, propagate
    if (expr->left && expr->right && 
        expr->left->sto_analyzed && expr->right->sto_analyzed &&
        expr->left->sto_info && expr->right->sto_info) {
        
        STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
        *propagated = arithmetic_propagate_binary_types(expr->left->sto_info, expr->right->sto_info, expr->op);
        
        expr->sto_info = propagated;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
    }
}
