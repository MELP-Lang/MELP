#ifndef ARITHMETIC_CODEGEN_H
#define ARITHMETIC_CODEGEN_H

#include <stdio.h>
#include "arithmetic.h"

// Forward declaration
typedef struct FunctionDeclaration FunctionDeclaration;

// Generate assembly code for an arithmetic expression
// Result will be in r8 (for integers) or xmm0 (for floats)
// func_context: FunctionDeclaration* for variable offset lookup (type-safe)
void arithmetic_generate_code(FILE* output, ArithmeticExpr* expr, FunctionDeclaration* func_context);

// Generate assignment: variable = expression
void arithmetic_generate_assignment(FILE* output, const char* var_name, ArithmeticExpr* expr);

#endif // ARITHMETIC_CODEGEN_H
