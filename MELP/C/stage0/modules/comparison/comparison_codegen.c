#include "comparison_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generate comparison code
void comparison_generate_code(FILE* output, ComparisonExpr* expr) {
    if (!output || !expr) return;
    
    emit_c("\n    // Comparison expression\n");
    
    // Get operator string
    const char* op_str = "";
    switch (expr->op) {
        case CMP_EQUAL: op_str = "=="; break;
        case CMP_NOT_EQUAL: op_str = "!="; break;
        case CMP_LESS: op_str = "<"; break;
        case CMP_LESS_EQUAL: op_str = "<="; break;
        case CMP_GREATER: op_str = ">"; break;
        case CMP_GREATER_EQUAL: op_str = ">="; break;
    }
    
    emit_c("    // Result: (%s %s %s)\n", 
           expr->left_value, op_str, expr->right_value);
}

// Generate conditional jump (not needed for C code generation)
void comparison_generate_conditional_jump(FILE* output, ComparisonExpr* expr, const char* label) {
    // This function is not needed for C code generation
    // C uses if statements directly
    (void)output;
    (void)expr;
    (void)label;
}
