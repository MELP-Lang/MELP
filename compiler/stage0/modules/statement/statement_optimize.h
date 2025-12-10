#ifndef STATEMENT_OPTIMIZE_H
#define STATEMENT_OPTIMIZE_H

#include "statement.h"

/**
 * Dead Code Elimination
 * 
 * Removes unreachable code:
 * - if false then ... end if → removed
 * - if true then ... else ... end if → remove else block
 * - while false ... end while → removed
 * - Code after return statement → removed
 */
Statement* statement_optimize_dead_code(Statement* stmt);

/**
 * Helper: Check if condition is constant true/false
 */
int statement_is_constant_condition(void* condition, int* is_true);

#endif
