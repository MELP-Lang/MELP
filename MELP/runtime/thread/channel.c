/**
 * MLP Standard Library - Channel Implementation
 * 
 * Message passing channels for thread communication
 * Bounded buffer implementation with condition variables
 * 
 * Design: Key-based channel registry for functional pattern support
 * Same key = same channel (enables multi-thread communication)
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#include "channel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

// Thread-local error storage
static __thread char last_error[256] = {0};

// Message structure
typedef struct {
    int64_t value;
    int64_t sender_id;
} message_t;

// Channel structure (bounded buffer)
typedef struct channel {
    char* key;
    message_t* buffer;
    int64_t capacity;
    int64_t count;
    int64_t read_pos;
    int64_t write_pos;
    
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;  // Signal when buffer has data
    pthread_cond_t not_full;   // Signal when buffer has space
    
    int ref_count;
    struct channel* next;
} channel_t;

// Global channel registry (protected by registry_lock)
static channel_t* channel_registry = NULL;
static pthread_mutex_t registry_lock = PTHREAD_MUTEX_INITIALIZER;

// ============================================================================
// Internal Helper Functions
// ============================================================================

static void set_error(const char* msg) {
    snprintf(last_error, sizeof(last_error), "%s: %s", msg, strerror(errno));
}

static void clear_error(void) {
    last_error[0] = '\0';
}

const char* mlp_channel_get_error(void) {
    return last_error;
}

// Find or create channel by key
static channel_t* find_or_create_channel(const char* key, int64_t capacity) {
    pthread_mutex_lock(&registry_lock);
    
    // Search for existing channel
    channel_t* ch = channel_registry;
    while (ch) {
        if (strcmp(ch->key, key) == 0) {
            ch->ref_count++;
            pthread_mutex_unlock(&registry_lock);
            return ch;
        }
        ch = ch->next;
    }
    
    // Create new channel
    ch = (channel_t*)malloc(sizeof(channel_t));
    if (!ch) {
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    ch->key = strdup(key);
    if (!ch->key) {
        free(ch);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    // Allocate buffer
    ch->capacity = capacity > 0 ? capacity : 10;  // Default: 10
    ch->buffer = (message_t*)malloc(sizeof(message_t) * ch->capacity);
    if (!ch->buffer) {
        free(ch->key);
        free(ch);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    ch->count = 0;
    ch->read_pos = 0;
    ch->write_pos = 0;
    ch->ref_count = 1;
    
    // Initialize synchronization primitives
    if (pthread_mutex_init(&ch->mutex, NULL) != 0) {
        free(ch->buffer);
        free(ch->key);
        free(ch);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    if (pthread_cond_init(&ch->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&ch->mutex);
        free(ch->buffer);
        free(ch->key);
        free(ch);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    if (pthread_cond_init(&ch->not_full, NULL) != 0) {
        pthread_cond_destroy(&ch->not_empty);
        pthread_mutex_destroy(&ch->mutex);
        free(ch->buffer);
        free(ch->key);
        free(ch);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    // Add to registry
    ch->next = channel_registry;
    channel_registry = ch;
    
    pthread_mutex_unlock(&registry_lock);
    return ch;
}

// Remove channel if ref_count reaches zero
static void release_channel(channel_t* ch) {
    pthread_mutex_lock(&registry_lock);
    
    ch->ref_count--;
    
    if (ch->ref_count <= 0) {
        // Remove from registry
        channel_t** ptr = &channel_registry;
        while (*ptr) {
            if (*ptr == ch) {
                *ptr = ch->next;
                break;
            }
            ptr = &(*ptr)->next;
        }
        
        // Cleanup
        pthread_cond_destroy(&ch->not_full);
        pthread_cond_destroy(&ch->not_empty);
        pthread_mutex_destroy(&ch->mutex);
        free(ch->buffer);
        free(ch->key);
        free(ch);
    }
    
    pthread_mutex_unlock(&registry_lock);
}

// ============================================================================
// Public Channel Operations
// ============================================================================

/**
 * Create or get channel by key
 * 
 * Args:
 *   key: Unique string identifier
 *   capacity: Buffer size (0 = synchronous, >0 = buffered)
 * 
 * Returns:
 *   Channel handle (>0) on success, -1 on error
 */
int64_t mlp_channel_create(const char* key, int64_t capacity) {
    clear_error();
    
    if (!key || strlen(key) == 0) {
        set_error("Invalid channel key");
        return -1;  // CHANNEL_ERROR_CREATE
    }
    
    if (capacity < 0) {
        set_error("Invalid capacity");
        return -1;
    }
    
    channel_t* ch = find_or_create_channel(key, capacity);
    if (!ch) {
        set_error("Failed to create channel");
        return -1;
    }
    
    return (int64_t)ch;
}

/**
 * Send message to channel (blocks if buffer full)
 * 
 * Args:
 *   handle: Channel handle
 *   value: Message value
 *   sender_id: Sender thread ID
 * 
 * Returns:
 *   0 on success, -2 on error
 */
int64_t mlp_channel_send(int64_t handle, int64_t value, int64_t sender_id) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid channel handle");
        return -2;  // CHANNEL_ERROR_SEND
    }
    
    channel_t* ch = (channel_t*)handle;
    
    pthread_mutex_lock(&ch->mutex);
    
    // Wait while buffer is full
    while (ch->count >= ch->capacity) {
        pthread_cond_wait(&ch->not_full, &ch->mutex);
    }
    
    // Add message to buffer
    ch->buffer[ch->write_pos].value = value;
    ch->buffer[ch->write_pos].sender_id = sender_id;
    ch->write_pos = (ch->write_pos + 1) % ch->capacity;
    ch->count++;
    
    // Signal receivers
    pthread_cond_signal(&ch->not_empty);
    
    pthread_mutex_unlock(&ch->mutex);
    return 0;
}

/**
 * Receive message from channel (blocks if buffer empty)
 * 
 * Args:
 *   handle: Channel handle
 * 
 * Returns:
 *   Message value on success, -3 on error
 */
int64_t mlp_channel_receive(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid channel handle");
        return -3;  // CHANNEL_ERROR_RECEIVE
    }
    
    channel_t* ch = (channel_t*)handle;
    
    pthread_mutex_lock(&ch->mutex);
    
    // Wait while buffer is empty
    while (ch->count == 0) {
        pthread_cond_wait(&ch->not_empty, &ch->mutex);
    }
    
    // Read message from buffer
    int64_t value = ch->buffer[ch->read_pos].value;
    ch->read_pos = (ch->read_pos + 1) % ch->capacity;
    ch->count--;
    
    // Signal senders
    pthread_cond_signal(&ch->not_full);
    
    pthread_mutex_unlock(&ch->mutex);
    return value;
}

/**
 * Try to receive message without blocking
 * 
 * Args:
 *   handle: Channel handle
 * 
 * Returns:
 *   Message value on success, -4 if empty, -3 on error
 */
int64_t mlp_channel_try_receive(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid channel handle");
        return -3;  // CHANNEL_ERROR_RECEIVE
    }
    
    channel_t* ch = (channel_t*)handle;
    
    pthread_mutex_lock(&ch->mutex);
    
    // Check if buffer is empty
    if (ch->count == 0) {
        pthread_mutex_unlock(&ch->mutex);
        return -4;  // CHANNEL_ERROR_TIMEOUT (no data)
    }
    
    // Read message
    int64_t value = ch->buffer[ch->read_pos].value;
    ch->read_pos = (ch->read_pos + 1) % ch->capacity;
    ch->count--;
    
    pthread_cond_signal(&ch->not_full);
    
    pthread_mutex_unlock(&ch->mutex);
    return value;
}

/**
 * Get current number of messages in channel
 * 
 * Args:
 *   handle: Channel handle
 * 
 * Returns:
 *   Message count (>=0) on success, -1 on error
 */
int64_t mlp_channel_count(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid channel handle");
        return -1;
    }
    
    channel_t* ch = (channel_t*)handle;
    
    pthread_mutex_lock(&ch->mutex);
    int64_t count = ch->count;
    pthread_mutex_unlock(&ch->mutex);
    
    return count;
}

/**
 * Destroy channel and release resources
 * 
 * Note: Only destroys when ref_count reaches zero
 * 
 * Args:
 *   handle: Channel handle
 * 
 * Returns:
 *   0 on success, -5 on error
 */
int64_t mlp_channel_destroy(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid channel handle");
        return -5;  // CHANNEL_ERROR_CLOSED
    }
    
    channel_t* ch = (channel_t*)handle;
    release_channel(ch);
    
    return 0;
}
