/**
 * MLP Standard Library - Stack (LIFO) Header
 * 
 * Stack implementation using list wrapper (Last In First Out)
 * YZ_212 Phase 3: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#ifndef MLP_STACK_H
#define MLP_STACK_H

#include "mlp_list.h"
#include <stddef.h>

/**
 * MelpStack - LIFO (Last In First Out) collection
 * 
 * Design Philosophy:
 * - Wraps MelpList for storage
 * - LIFO semantics: push/pop at end of list
 * - O(1) push/pop (amortized, due to list append)
 * - Efficient implementation using dynamic array
 */
typedef struct {
    MelpList* internal;   // Underlying list storage
} MelpStack;

// -----------------------------------------------------------------------------
// Core Stack Operations
// -----------------------------------------------------------------------------

/**
 * Create a new stack with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to newly allocated stack, or NULL on failure
 */
MelpStack* melp_stack_create(size_t element_size);

/**
 * Free a stack and all its allocated memory
 * @param stack Stack to free
 */
void melp_stack_free(MelpStack* stack);

/**
 * Push element onto the top of the stack
 * @param stack Target stack
 * @param element Pointer to element data to add
 * @return 0 on success, -1 on failure
 */
int melp_stack_push(MelpStack* stack, const void* element);

/**
 * Pop and return element from the top of the stack
 * @param stack Target stack
 * @param out_element Pointer to buffer where element will be copied
 * @return 0 on success, -1 on failure (empty stack)
 * 
 * NOTE: Caller must provide buffer of size >= element_size
 */
int melp_stack_pop(MelpStack* stack, void* out_element);

/**
 * Peek at the top element without removing it
 * @param stack Target stack
 * @return Pointer to top element (do not free!), or NULL if empty
 */
void* melp_stack_peek(MelpStack* stack);

/**
 * Get the number of elements in stack
 * @param stack Target stack
 * @return Number of elements
 */
size_t melp_stack_size(MelpStack* stack);

/**
 * Check if stack is empty
 * @param stack Target stack
 * @return 1 if empty, 0 otherwise
 */
int melp_stack_is_empty(MelpStack* stack);

/**
 * Clear all elements from stack
 * @param stack Stack to clear
 */
void melp_stack_clear(MelpStack* stack);

#endif // MLP_STACK_H
