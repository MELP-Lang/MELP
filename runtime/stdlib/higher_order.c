// Higher-Order Functions Runtime Support
// YZ_208: Lambda/Anonymous Functions

#include "higher_order.h"
#include <stdlib.h>
#include <string.h>

// Map: Apply lambda to each element
List* mlp_map(List* items, LambdaFunc fn, void* context) {
    if (!items || !fn) return NULL;
    
    List* result = mlp_list_create();
    
    for (size_t i = 0; i < items->count; i++) {
        int64_t value = items->data[i];
        int64_t transformed = fn(value, context);
        mlp_list_append(result, transformed);
    }
    
    return result;
}

// Filter: Keep elements that match predicate
List* mlp_filter(List* items, PredicateFunc predicate, void* context) {
    if (!items || !predicate) return NULL;
    
    List* result = mlp_list_create();
    
    for (size_t i = 0; i < items->count; i++) {
        int64_t value = items->data[i];
        if (predicate(value, context)) {
            mlp_list_append(result, value);
        }
    }
    
    return result;
}

// Reduce: Fold list into single value
int64_t mlp_reduce(List* items, ReduceFunc reducer, int64_t initial, void* context) {
    if (!items || !reducer) return initial;
    
    int64_t accumulator = initial;
    
    for (size_t i = 0; i < items->count; i++) {
        int64_t value = items->data[i];
        accumulator = reducer(accumulator, value, context);
    }
    
    return accumulator;
}

// ForEach: Execute lambda for each element (side effects)
void mlp_foreach(List* items, ActionFunc action, void* context) {
    if (!items || !action) return;
    
    for (size_t i = 0; i < items->count; i++) {
        int64_t value = items->data[i];
        action(value, context);
    }
}
