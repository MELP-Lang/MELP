#ifndef STATEMENT_CODEGEN_H
#define STATEMENT_CODEGEN_H

#include "statement.h"
#include <stdio.h>

// Generate code for statement
// context: FunctionDeclaration* or other context for variable scoping
void statement_generate_code(FILE* output, Statement* stmt, void* context);

#endif
