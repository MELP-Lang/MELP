/**
 * MLP Standard Library - Threading Implementation
 * 
 * POSIX threads (pthread) implementation for multi-threading
 * Compatible with Linux and macOS
 * Windows support via pthreads-win32 (future)
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#ifndef MLP_THREAD_H
#define MLP_THREAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Thread operations
int64_t mlp_thread_create(int64_t worker_id);
int64_t mlp_thread_join(int64_t handle);
int64_t mlp_thread_current_id(void);
void mlp_thread_sleep(int64_t milliseconds);

// Get last error message
const char* mlp_thread_get_error(void);

#ifdef __cplusplus
}
#endif

#endif // MLP_THREAD_H
