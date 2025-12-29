#include "comparison.h"
#include <stdlib.h>

// Free comparison expression
void comparison_expr_free(ComparisonExpr* expr) {
    if (!expr) return;
    
    if (expr->left_value) {
        free(expr->left_value);
    }
    
    if (expr->right_value) {
        free(expr->right_value);
    }
    
    // ✅ Phase 3.2: Free chained comparisons
    if (expr->next) {
        comparison_expr_free(expr->next);  // Recursive free
    }
    
    // Note: left and right pointers are managed by caller
    // (they might be ArithmeticExpr or other types)
    
    free(expr);
}
