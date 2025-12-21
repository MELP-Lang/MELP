/**
 * MLP Standard Library - Queue (FIFO) Header
 * 
 * Queue implementation using list wrapper (First In First Out)
 * YZ_212 Phase 3: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#ifndef MLP_QUEUE_H
#define MLP_QUEUE_H

#include "mlp_list.h"
#include <stddef.h>

/**
 * MelpQueue - FIFO (First In First Out) collection
 * 
 * Design Philosophy:
 * - Wraps MelpList for storage
 * - FIFO semantics: enqueue at end, dequeue from front
 * - O(n) dequeue (due to array shifting, acceptable for stage0)
 * - Can be optimized with circular buffer later
 */
typedef struct {
    MelpList* internal;   // Underlying list storage
} MelpQueue;

// -----------------------------------------------------------------------------
// Core Queue Operations
// -----------------------------------------------------------------------------

/**
 * Create a new queue with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to newly allocated queue, or NULL on failure
 */
MelpQueue* melp_queue_create(size_t element_size);

/**
 * Free a queue and all its allocated memory
 * @param queue Queue to free
 */
void melp_queue_free(MelpQueue* queue);

/**
 * Add element to the back of the queue (enqueue)
 * @param queue Target queue
 * @param element Pointer to element data to add
 * @return 0 on success, -1 on failure
 */
int melp_queue_enqueue(MelpQueue* queue, const void* element);

/**
 * Remove and return element from the front of the queue (dequeue)
 * @param queue Target queue
 * @param out_element Pointer to buffer where element will be copied
 * @return 0 on success, -1 on failure (empty queue)
 * 
 * NOTE: Caller must provide buffer of size >= element_size
 */
int melp_queue_dequeue(MelpQueue* queue, void* out_element);

/**
 * Peek at the front element without removing it
 * @param queue Target queue
 * @return Pointer to front element (do not free!), or NULL if empty
 */
void* melp_queue_peek(MelpQueue* queue);

/**
 * Get the number of elements in queue
 * @param queue Target queue
 * @return Number of elements
 */
size_t melp_queue_size(MelpQueue* queue);

/**
 * Check if queue is empty
 * @param queue Target queue
 * @return 1 if empty, 0 otherwise
 */
int melp_queue_is_empty(MelpQueue* queue);

/**
 * Clear all elements from queue
 * @param queue Queue to clear
 */
void melp_queue_clear(MelpQueue* queue);

#endif // MLP_QUEUE_H
