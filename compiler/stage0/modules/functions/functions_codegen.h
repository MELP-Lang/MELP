#ifndef FUNCTIONS_CODEGEN_H
#define FUNCTIONS_CODEGEN_H

#include <stdio.h>
#include "functions.h"
#include "../variable/variable.h"  // YZ_121: For VariableDeclaration

// YZ_121: Set global consts for codegen (temporary hack)
void function_codegen_set_global_consts(VariableDeclaration* consts);

// Generate assembly code for function declaration
void function_generate_declaration(FILE* output, FunctionDeclaration* func);

// Generate assembly code for function call
void function_generate_call(FILE* output, FunctionCall* call);

// Generate assembly code for return statement
void function_generate_return(FILE* output, ReturnStatement* ret, void* context);

// Helper: Generate function prologue (stack setup)
void function_generate_prologue(FILE* output, FunctionDeclaration* func);

// Helper: Generate function epilogue (stack cleanup, return)
void function_generate_epilogue(FILE* output, FunctionDeclaration* func);

#endif
