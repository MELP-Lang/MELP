/**
 * Test: Queue and Stack Operations
 * 
 * YZ_212 Phase 3: Testing Queue (FIFO) and Stack (LIFO)
 * Tests: enqueue, dequeue, push, pop, peek operations
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "../runtime/stdlib/mlp_queue.h"
#include "../runtime/stdlib/mlp_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

// =============================================================================
// QUEUE TESTS
// =============================================================================

void test_queue_create_and_enqueue() {
    printf("🧪 Test 1: Queue Create and Enqueue\n");
    
    MelpQueue* queue = melp_queue_create(sizeof(int64_t));
    assert(queue != NULL);
    assert(melp_queue_is_empty(queue) == 1);
    
    // Enqueue elements: [10, 20, 30]
    int64_t val10 = 10, val20 = 20, val30 = 30;
    assert(melp_queue_enqueue(queue, &val10) == 0);
    assert(melp_queue_size(queue) == 1);
    
    assert(melp_queue_enqueue(queue, &val20) == 0);
    assert(melp_queue_size(queue) == 2);
    
    assert(melp_queue_enqueue(queue, &val30) == 0);
    assert(melp_queue_size(queue) == 3);
    
    melp_queue_free(queue);
    printf("✅ Test 1 passed!\n\n");
}

void test_queue_dequeue() {
    printf("🧪 Test 2: Queue Dequeue (FIFO)\n");
    
    MelpQueue* queue = melp_queue_create(sizeof(int64_t));
    
    // Enqueue: [1, 2, 3, 4, 5]
    int64_t values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        melp_queue_enqueue(queue, &values[i]);
    }
    
    // Dequeue should return in FIFO order: 1, 2, 3, 4, 5
    int64_t result;
    assert(melp_queue_dequeue(queue, &result) == 0);
    assert(result == 1);
    assert(melp_queue_size(queue) == 4);
    
    assert(melp_queue_dequeue(queue, &result) == 0);
    assert(result == 2);
    
    assert(melp_queue_dequeue(queue, &result) == 0);
    assert(result == 3);
    
    assert(melp_queue_dequeue(queue, &result) == 0);
    assert(result == 4);
    
    assert(melp_queue_dequeue(queue, &result) == 0);
    assert(result == 5);
    assert(melp_queue_is_empty(queue) == 1);
    
    // Dequeue from empty queue should fail
    assert(melp_queue_dequeue(queue, &result) == -1);
    
    melp_queue_free(queue);
    printf("✅ Test 2 passed!\n\n");
}

void test_queue_peek() {
    printf("🧪 Test 3: Queue Peek\n");
    
    MelpQueue* queue = melp_queue_create(sizeof(int64_t));
    
    // Peek on empty queue
    assert(melp_queue_peek(queue) == NULL);
    
    // Enqueue: [100, 200, 300]
    int64_t val100 = 100, val200 = 200, val300 = 300;
    melp_queue_enqueue(queue, &val100);
    melp_queue_enqueue(queue, &val200);
    melp_queue_enqueue(queue, &val300);
    
    // Peek should show front (100) without removing
    int64_t* front = (int64_t*)melp_queue_peek(queue);
    assert(*front == 100);
    assert(melp_queue_size(queue) == 3);  // Size unchanged
    
    // Dequeue and peek again
    int64_t result;
    melp_queue_dequeue(queue, &result);
    front = (int64_t*)melp_queue_peek(queue);
    assert(*front == 200);
    
    melp_queue_free(queue);
    printf("✅ Test 3 passed!\n\n");
}

// =============================================================================
// STACK TESTS
// =============================================================================

void test_stack_create_and_push() {
    printf("🧪 Test 4: Stack Create and Push\n");
    
    MelpStack* stack = melp_stack_create(sizeof(int64_t));
    assert(stack != NULL);
    assert(melp_stack_is_empty(stack) == 1);
    
    // Push elements: [10, 20, 30]
    int64_t val10 = 10, val20 = 20, val30 = 30;
    assert(melp_stack_push(stack, &val10) == 0);
    assert(melp_stack_size(stack) == 1);
    
    assert(melp_stack_push(stack, &val20) == 0);
    assert(melp_stack_size(stack) == 2);
    
    assert(melp_stack_push(stack, &val30) == 0);
    assert(melp_stack_size(stack) == 3);
    
    melp_stack_free(stack);
    printf("✅ Test 4 passed!\n\n");
}

void test_stack_pop() {
    printf("🧪 Test 5: Stack Pop (LIFO)\n");
    
    MelpStack* stack = melp_stack_create(sizeof(int64_t));
    
    // Push: [1, 2, 3, 4, 5]
    int64_t values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        melp_stack_push(stack, &values[i]);
    }
    
    // Pop should return in LIFO order: 5, 4, 3, 2, 1
    int64_t result;
    assert(melp_stack_pop(stack, &result) == 0);
    assert(result == 5);
    assert(melp_stack_size(stack) == 4);
    
    assert(melp_stack_pop(stack, &result) == 0);
    assert(result == 4);
    
    assert(melp_stack_pop(stack, &result) == 0);
    assert(result == 3);
    
    assert(melp_stack_pop(stack, &result) == 0);
    assert(result == 2);
    
    assert(melp_stack_pop(stack, &result) == 0);
    assert(result == 1);
    assert(melp_stack_is_empty(stack) == 1);
    
    // Pop from empty stack should fail
    assert(melp_stack_pop(stack, &result) == -1);
    
    melp_stack_free(stack);
    printf("✅ Test 5 passed!\n\n");
}

void test_stack_peek() {
    printf("🧪 Test 6: Stack Peek\n");
    
    MelpStack* stack = melp_stack_create(sizeof(int64_t));
    
    // Peek on empty stack
    assert(melp_stack_peek(stack) == NULL);
    
    // Push: [100, 200, 300]
    int64_t val100 = 100, val200 = 200, val300 = 300;
    melp_stack_push(stack, &val100);
    melp_stack_push(stack, &val200);
    melp_stack_push(stack, &val300);
    
    // Peek should show top (300) without removing
    int64_t* top = (int64_t*)melp_stack_peek(stack);
    assert(*top == 300);
    assert(melp_stack_size(stack) == 3);  // Size unchanged
    
    // Pop and peek again
    int64_t result;
    melp_stack_pop(stack, &result);
    top = (int64_t*)melp_stack_peek(stack);
    assert(*top == 200);
    
    melp_stack_free(stack);
    printf("✅ Test 6 passed!\n\n");
}

void test_queue_stack_string() {
    printf("🧪 Test 7: Queue and Stack with Strings\n");
    
    // Queue with strings
    MelpQueue* queue = melp_queue_create(sizeof(char*));
    char* str1 = "first";
    char* str2 = "second";
    char* str3 = "third";
    
    melp_queue_enqueue(queue, &str1);
    melp_queue_enqueue(queue, &str2);
    melp_queue_enqueue(queue, &str3);
    
    char* result;
    melp_queue_dequeue(queue, &result);
    assert(strcmp(result, "first") == 0);
    
    melp_queue_free(queue);
    
    // Stack with strings
    MelpStack* stack = melp_stack_create(sizeof(char*));
    melp_stack_push(stack, &str1);
    melp_stack_push(stack, &str2);
    melp_stack_push(stack, &str3);
    
    melp_stack_pop(stack, &result);
    assert(strcmp(result, "third") == 0);
    
    melp_stack_free(stack);
    
    printf("✅ Test 7 passed!\n\n");
}

int main() {
    printf("🚀 YZ_212 Phase 3: Queue & Stack Test\n");
    printf("=========================================\n\n");
    
    test_queue_create_and_enqueue();
    test_queue_dequeue();
    test_queue_peek();
    test_stack_create_and_push();
    test_stack_pop();
    test_stack_peek();
    test_queue_stack_string();
    
    printf("=========================================\n");
    printf("✅ ALL QUEUE & STACK TESTS PASSED! 🎉\n");
    
    return 0;
}
