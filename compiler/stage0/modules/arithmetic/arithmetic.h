#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stdbool.h>
#include "../lexer/lexer.h"
#include "../codegen_context/sto_types.h"
#include "../array/array.h"  // YZ_14: For IndexAccess struct

// Arithmetic operation types
typedef enum {
    ARITH_ADD,       // +
    ARITH_SUB,       // -
    ARITH_MUL,       // *
    ARITH_DIV,       // /
    ARITH_MOD,       // %
    ARITH_POW,       // **
    ARITH_AND,       // & (bitwise AND)
    ARITH_OR,        // | (bitwise OR)
    ARITH_XOR        // ^ (bitwise XOR)
} ArithmeticOp;

// Forward declaration for function calls
typedef struct FunctionCallExpr FunctionCallExpr;

// Arithmetic expression structure
typedef struct ArithmeticExpr {
    ArithmeticOp op;              // Operation type
    struct ArithmeticExpr* left;  // Left operand (can be nested expression)
    struct ArithmeticExpr* right; // Right operand (can be nested expression)
    
    // Leaf node data (for literals/variables)
    int is_literal;               // 1 if literal value, 0 if variable
    char* value;                  // Literal value or variable name
    int is_float;                 // 1 if floating point number
    int is_string;                // 1 if string literal/variable
    int is_boolean;               // 1 if boolean literal/variable (true/false)
    
    // ========== Phase 3.5: Function Calls ==========
    int is_function_call;         // 1 if this is a function call
    FunctionCallExpr* func_call;  // Function call details (if is_function_call)
    
    // ========== YZ_14: Array Index Access ==========
    int is_array_access;          // 1 if this is array indexing (arr[i])
    IndexAccess* array_access;    // Array access details (if is_array_access)
    
    // ========== YZ_17: Collection Literals (Array/List/Tuple) ==========
    int is_collection;            // 1 if this is a collection literal
    Collection* collection;       // Collection details (if is_collection)
    
    // ========== Phase 2.3: STO Type Propagation ==========
    STOTypeInfo* sto_info;        // STO analysis result (heap allocated)
    bool sto_analyzed;            // Has STO analysis been performed?
    bool needs_overflow_check;    // Runtime overflow detection needed?
} ArithmeticExpr;

// ========== Phase 3.5: Function Call Expression ==========
typedef struct FunctionCallExpr {
    char* function_name;           // Name of function being called
    ArithmeticExpr** arguments;    // Array of argument expressions
    int arg_count;                 // Number of arguments
} FunctionCallExpr;

// Free arithmetic expression
void arithmetic_expr_free(ArithmeticExpr* expr);

// ========== Phase 2.3: STO Type Propagation Functions ==========
// Infer STO type for arithmetic expression
void arithmetic_infer_sto_type(ArithmeticExpr* expr);

// Propagate types through binary operations
STOTypeInfo arithmetic_propagate_binary_types(STOTypeInfo* left, STOTypeInfo* right, ArithmeticOp op);

// Note: sto_infer_numeric_type() is defined in codegen_context.h

#endif
