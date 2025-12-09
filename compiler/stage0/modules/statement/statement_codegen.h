#ifndef STATEMENT_CODEGEN_H
#define STATEMENT_CODEGEN_H

#include "statement.h"
#include <stdio.h>

// Forward declaration
typedef struct FunctionDeclaration FunctionDeclaration;

// Generate code for statement
// func: FunctionDeclaration* for variable scoping (type-safe)
void statement_generate_code(FILE* output, Statement* stmt, FunctionDeclaration* func);

#endif
