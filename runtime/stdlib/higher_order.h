// Higher-Order Functions Runtime Support
// YZ_208: Lambda/Anonymous Functions

#ifndef HIGHER_ORDER_H
#define HIGHER_ORDER_H

#include <stdint.h>
#include <stddef.h>
#include "mlp_list.h"

// Function pointer types
typedef int64_t (*LambdaFunc)(int64_t, void*);       // (value, context) -> result
typedef int (*PredicateFunc)(int64_t, void*);        // (value, context) -> bool
typedef int64_t (*ReduceFunc)(int64_t, int64_t, void*); // (acc, value, context) -> acc
typedef void (*ActionFunc)(int64_t, void*);          // (value, context) -> void

// Higher-order functions
List* mlp_map(List* items, LambdaFunc fn, void* context);
List* mlp_filter(List* items, PredicateFunc predicate, void* context);
int64_t mlp_reduce(List* items, ReduceFunc reducer, int64_t initial, void* context);
void mlp_foreach(List* items, ActionFunc action, void* context);

#endif // HIGHER_ORDER_H
