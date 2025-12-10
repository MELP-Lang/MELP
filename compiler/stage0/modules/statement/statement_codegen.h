#ifndef STATEMENT_CODEGEN_H
#define STATEMENT_CODEGEN_H

#include "statement.h"
#include <stdio.h>

// Forward declaration
typedef struct FunctionDeclaration FunctionDeclaration;

// Generate code for statement
// func: FunctionDeclaration* for variable scoping (type-safe)
void statement_generate_code(FILE* output, Statement* stmt, FunctionDeclaration* func);

// YZ_28: Loop label stack for exit support (VB.NET style)
// Maximum nested loop depth
#define MAX_LOOP_DEPTH 32

// Loop context for exit for/exit while
typedef struct {
    int exit_label;        // Label ID for exit target (loop end)
} LoopContext;

// Global loop stack
extern LoopContext loop_stack[MAX_LOOP_DEPTH];
extern int loop_stack_top;

// Push/pop loop context
void loop_push(int exit_label);
void loop_pop(void);
int get_break_label(void);  // Returns exit_label for current loop

#endif
