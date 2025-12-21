/**
 * MLP Standard Library - Queue (FIFO) Implementation
 * 
 * Queue implementation using list wrapper (First In First Out)
 * YZ_212 Phase 3: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "mlp_queue.h"
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Core Queue Operations
// -----------------------------------------------------------------------------

MelpQueue* melp_queue_create(size_t element_size) {
    if (element_size == 0) {
        return NULL;
    }
    
    MelpQueue* queue = (MelpQueue*)malloc(sizeof(MelpQueue));
    if (!queue) {
        return NULL;
    }
    
    queue->internal = melp_list_create(element_size);
    if (!queue->internal) {
        free(queue);
        return NULL;
    }
    
    return queue;
}

void melp_queue_free(MelpQueue* queue) {
    if (!queue) {
        return;
    }
    
    if (queue->internal) {
        melp_list_free(queue->internal);
    }
    
    free(queue);
}

int melp_queue_enqueue(MelpQueue* queue, const void* element) {
    if (!queue || !element) {
        return -1;
    }
    
    // Add to end of list (FIFO: enqueue at back)
    return melp_list_append(queue->internal, (void*)element);
}

int melp_queue_dequeue(MelpQueue* queue, void* out_element) {
    if (!queue || !out_element) {
        return -1;
    }
    
    // Check if queue is empty
    if (melp_list_is_empty(queue->internal)) {
        return -1;
    }
    
    // Get front element (index 0)
    void* front = melp_list_get(queue->internal, 0);
    if (!front) {
        return -1;
    }
    
    // Copy element to output buffer
    memcpy(out_element, front, queue->internal->element_size);
    
    // Remove front element (FIFO: dequeue from front)
    return melp_list_remove(queue->internal, 0);
}

void* melp_queue_peek(MelpQueue* queue) {
    if (!queue) {
        return NULL;
    }
    
    // Check if queue is empty
    if (melp_list_is_empty(queue->internal)) {
        return NULL;
    }
    
    // Return front element without removing
    return melp_list_get(queue->internal, 0);
}

size_t melp_queue_size(MelpQueue* queue) {
    if (!queue) {
        return 0;
    }
    
    return melp_list_length(queue->internal);
}

int melp_queue_is_empty(MelpQueue* queue) {
    if (!queue) {
        return 1;
    }
    
    return melp_list_is_empty(queue->internal);
}

void melp_queue_clear(MelpQueue* queue) {
    if (!queue) {
        return;
    }
    
    melp_list_clear(queue->internal);
}
