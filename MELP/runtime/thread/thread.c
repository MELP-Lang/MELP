/**
 * MLP Standard Library - Threading Implementation
 * 
 * POSIX threads (pthread) implementation for multi-threading
 * Compatible with Linux and macOS
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Thread-local error storage
static __thread char last_error[256] = {0};

// Thread worker context
typedef struct {
    int64_t worker_id;
    int64_t result;
    pthread_t thread;
} thread_context_t;

// ============================================================================
// Internal Helper Functions
// ============================================================================

static void set_error(const char* msg) {
    snprintf(last_error, sizeof(last_error), "%s: %s", msg, strerror(errno));
}

static void clear_error(void) {
    last_error[0] = '\0';
}

const char* mlp_thread_get_error(void) {
    return last_error;
}

// ============================================================================
// Thread Worker Function
// ============================================================================

// This is a placeholder worker function
// In a real implementation, this would call back into MLP runtime
// to execute the user's worker_function
static void* thread_worker(void* arg) {
    thread_context_t* ctx = (thread_context_t*)arg;
    
    // Placeholder: Simple computation based on worker_id
    // In full implementation, this would:
    // 1. Call into MLP runtime
    // 2. Execute user's lambda/function
    // 3. Return result
    
    // For now, just return worker_id * 10 as demo
    ctx->result = ctx->worker_id * 10;
    
    return NULL;
}

// ============================================================================
// Public Thread Operations
// ============================================================================

/**
 * Create and start a new thread
 * 
 * Args:
 *   worker_id: Unique ID passed to worker function
 * 
 * Returns:
 *   Thread handle (>0) on success, -1 on error
 */
int64_t mlp_thread_create(int64_t worker_id) {
    clear_error();
    
    // Allocate thread context
    thread_context_t* ctx = (thread_context_t*)malloc(sizeof(thread_context_t));
    if (!ctx) {
        set_error("Failed to allocate thread context");
        return -1;
    }
    
    ctx->worker_id = worker_id;
    ctx->result = 0;
    
    // Create pthread
    int rc = pthread_create(&ctx->thread, NULL, thread_worker, ctx);
    if (rc != 0) {
        set_error("Failed to create thread");
        free(ctx);
        return -1;
    }
    
    // Return context pointer as handle (cast to int64_t)
    return (int64_t)ctx;
}

/**
 * Wait for thread to complete and get result
 * 
 * Args:
 *   handle: Thread handle from mlp_thread_create
 * 
 * Returns:
 *   Result value from worker, -2 on error
 */
int64_t mlp_thread_join(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid thread handle");
        return -2;  // THREAD_ERROR_JOIN
    }
    
    thread_context_t* ctx = (thread_context_t*)handle;
    
    // Join thread (wait for completion)
    int rc = pthread_join(ctx->thread, NULL);
    if (rc != 0) {
        set_error("Failed to join thread");
        free(ctx);
        return -2;  // THREAD_ERROR_JOIN
    }
    
    // Get result
    int64_t result = ctx->result;
    
    // Cleanup context
    free(ctx);
    
    return result;
}

/**
 * Get current thread's ID
 * 
 * Returns:
 *   Platform-specific thread ID
 */
int64_t mlp_thread_current_id(void) {
    // pthread_self() returns pthread_t which might not be int64_t
    // Cast to unsigned long for portability
    return (int64_t)(unsigned long)pthread_self();
}

/**
 * Sleep current thread for specified milliseconds
 * 
 * Args:
 *   milliseconds: Time to sleep in milliseconds
 */
void mlp_thread_sleep(int64_t milliseconds) {
    if (milliseconds <= 0) return;
    
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    
    // Use nanosleep (handles interrupts gracefully)
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
        // Interrupted by signal, continue sleeping
    }
}

// ============================================================================
// Thread Pool Support (Future Enhancement)
// ============================================================================

// TODO: Add thread pool implementation for better resource management
// - mlp_threadpool_create(size)
// - mlp_threadpool_submit(pool, worker_id)
// - mlp_threadpool_wait_all(pool)
// - mlp_threadpool_destroy(pool)
