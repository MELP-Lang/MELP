#ifndef STATEMENT_CODEGEN_H
#define STATEMENT_CODEGEN_H

#include "statement.h"
#include <stdio.h>

// Forward declaration
typedef struct FunctionDeclaration FunctionDeclaration;

// Generate code for statement
// func: FunctionDeclaration* for variable scoping (type-safe)
void statement_generate_code(FILE* output, Statement* stmt, FunctionDeclaration* func);

// YZ_28: Loop label stack for break/continue support
// Maximum nested loop depth
#define MAX_LOOP_DEPTH 32

// Loop context for break/continue
typedef struct {
    int break_label;       // Label ID for break target
    int continue_label;    // Label ID for continue target
} LoopContext;

// Global loop stack
extern LoopContext loop_stack[MAX_LOOP_DEPTH];
extern int loop_stack_top;

// Push/pop loop context
void loop_push(int break_label, int continue_label);
void loop_pop(void);
int get_break_label(void);
int get_continue_label(void);

#endif
