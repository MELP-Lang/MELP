#ifndef ARITHMETIC_OPTIMIZE_H
#define ARITHMETIC_OPTIMIZE_H

#include "arithmetic.h"

/**
 * Constant Folding Optimization
 * 
 * Evaluates constant expressions at compile time:
 * - 2 + 3 → 5
 * - 10 * 5 → 50
 * - "Hello" + " " + "World" → "Hello World"
 * 
 * Only applies to literals, not variables.
 */
ArithmeticExpr* arithmetic_optimize_constant_fold(ArithmeticExpr* expr);

/**
 * Helper: Check if expression is a constant (literal only)
 */
int arithmetic_is_constant(ArithmeticExpr* expr);

/**
 * Helper: Evaluate binary operation on two numeric constants
 */
double arithmetic_eval_numeric(double left, double right, ArithmeticOp op);

#endif
