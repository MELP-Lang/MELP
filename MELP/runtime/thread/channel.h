/**
 * MLP Standard Library - Channel Implementation
 * 
 * Message passing channels for thread communication
 * Based on bounded buffer (producer-consumer) pattern
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#ifndef MLP_CHANNEL_H
#define MLP_CHANNEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Channel operations
int64_t mlp_channel_create(const char* key, int64_t capacity);
int64_t mlp_channel_send(int64_t handle, int64_t value, int64_t sender_id);
int64_t mlp_channel_receive(int64_t handle);
int64_t mlp_channel_try_receive(int64_t handle);
int64_t mlp_channel_count(int64_t handle);
int64_t mlp_channel_destroy(int64_t handle);

// Get last error message
const char* mlp_channel_get_error(void);

#ifdef __cplusplus
}
#endif

#endif // MLP_CHANNEL_H
