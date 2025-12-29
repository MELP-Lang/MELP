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
// YZ_90: Added continue support
// Maximum nested loop depth
#define MAX_LOOP_DEPTH 32

// Loop context for exit/continue
typedef struct {
    int exit_label;        // Label ID for exit target (loop end)
    int continue_label;    // Label ID for continue target (loop start)
} LoopContext;

// Global loop stack
extern LoopContext loop_stack[MAX_LOOP_DEPTH];
extern int loop_stack_top;

// Push/pop loop context
void loop_push(int exit_label, int continue_label);  // YZ_90: Added continue_label
void loop_pop(void);
int get_break_label(void);     // Returns exit_label for current loop
int get_continue_label(void);  // YZ_90: Returns continue_label for current loop

// YZ_82: Reset stack offset tracking for new function
void statement_reset_stack_offset(void);
void statement_set_initial_stack_offset(int offset);  // YZ_85: Set initial offset

#endif
