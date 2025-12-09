#ifndef COMPARISON_H
#define COMPARISON_H

// Comparison operators
typedef enum {
    CMP_EQUAL,        // ==
    CMP_NOT_EQUAL,    // !=
    CMP_LESS,         // <
    CMP_LESS_EQUAL,   // <=
    CMP_GREATER,      // >
    CMP_GREATER_EQUAL // >=
} ComparisonOp;

// Logical operators for chaining comparisons
typedef enum {
    LOG_NONE,  // No logical operator (single comparison)
    LOG_AND,   // AND / &&
    LOG_OR,    // OR / ||
    LOG_NOT    // NOT / ! (unary, applied to next comparison)
} LogicalChainOp;

// Comparison expression structure
typedef struct ComparisonExpr {
    ComparisonOp op;
    void* left;           // Can be ArithmeticExpr or variable
    void* right;          // Can be ArithmeticExpr or variable
    int left_is_literal;
    int right_is_literal;
    char* left_value;
    char* right_value;
    int is_float;
    
    // ✅ Phase 3.2: Logical chaining
    LogicalChainOp chain_op;           // AND/OR to connect to next comparison
    struct ComparisonExpr* next;       // Next comparison in chain
    int is_negated;                    // True if NOT applied to this comparison
} ComparisonExpr;

// Free comparison expression
void comparison_expr_free(ComparisonExpr* expr);

#endif // COMPARISON_H
