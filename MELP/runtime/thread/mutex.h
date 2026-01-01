/**
 * MLP Standard Library - Mutex Implementation
 * 
 * POSIX mutex (pthread_mutex) implementation
 * Compatible with Linux and macOS
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#ifndef MLP_MUTEX_H
#define MLP_MUTEX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Mutex operations
int64_t mlp_mutex_create(const char* key);
int64_t mlp_mutex_lock(int64_t handle);
int64_t mlp_mutex_unlock(int64_t handle);
int64_t mlp_mutex_destroy(int64_t handle);

// Get last error message
const char* mlp_mutex_get_error(void);

#ifdef __cplusplus
}
#endif

#endif // MLP_MUTEX_H
