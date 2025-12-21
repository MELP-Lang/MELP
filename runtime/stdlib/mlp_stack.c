/**
 * MLP Standard Library - Stack (LIFO) Implementation
 * 
 * Stack implementation using list wrapper (Last In First Out)
 * YZ_212 Phase 3: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "mlp_stack.h"
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Core Stack Operations
// -----------------------------------------------------------------------------

MelpStack* melp_stack_create(size_t element_size) {
    if (element_size == 0) {
        return NULL;
    }
    
    MelpStack* stack = (MelpStack*)malloc(sizeof(MelpStack));
    if (!stack) {
        return NULL;
    }
    
    stack->internal = melp_list_create(element_size);
    if (!stack->internal) {
        free(stack);
        return NULL;
    }
    
    return stack;
}

void melp_stack_free(MelpStack* stack) {
    if (!stack) {
        return;
    }
    
    if (stack->internal) {
        melp_list_free(stack->internal);
    }
    
    free(stack);
}

int melp_stack_push(MelpStack* stack, const void* element) {
    if (!stack || !element) {
        return -1;
    }
    
    // Add to end of list (LIFO: push at top/end)
    return melp_list_append(stack->internal, (void*)element);
}

int melp_stack_pop(MelpStack* stack, void* out_element) {
    if (!stack || !out_element) {
        return -1;
    }
    
    // Check if stack is empty
    if (melp_list_is_empty(stack->internal)) {
        return -1;
    }
    
    // Get top element (last index)
    size_t top_index = melp_list_length(stack->internal) - 1;
    void* top = melp_list_get(stack->internal, top_index);
    if (!top) {
        return -1;
    }
    
    // Copy element to output buffer
    memcpy(out_element, top, stack->internal->element_size);
    
    // Remove top element (LIFO: pop from top/end)
    return melp_list_remove(stack->internal, top_index);
}

void* melp_stack_peek(MelpStack* stack) {
    if (!stack) {
        return NULL;
    }
    
    // Check if empty
    if (melp_list_is_empty(stack->internal)) {
        return NULL;
    }
    
    // Return top element without removing
    size_t top_index = melp_list_length(stack->internal) - 1;
    return melp_list_get(stack->internal, top_index);
}

size_t melp_stack_size(MelpStack* stack) {
    if (!stack) {
        return 0;
    }
    
    return melp_list_length(stack->internal);
}

int melp_stack_is_empty(MelpStack* stack) {
    if (!stack) {
        return 1;
    }
    
    return melp_list_is_empty(stack->internal);
}

void melp_stack_clear(MelpStack* stack) {
    if (!stack) {
        return;
    }
    
    melp_list_clear(stack->internal);
}
