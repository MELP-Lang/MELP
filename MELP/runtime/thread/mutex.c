/**
 * MLP Standard Library - Mutex Implementation
 * 
 * POSIX mutex (pthread_mutex) implementation for mutual exclusion
 * Compatible with Linux and macOS
 * 
 * Design: Key-based mutex registry for functional pattern support
 * Same key = same mutex (enables closure-based locking)
 * 
 * YZ_02 - TODO #2 Task 2: Threading & Concurrency
 */

#include "mutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

// Thread-local error storage
static __thread char last_error[256] = {0};

// Mutex registry entry
typedef struct mutex_entry {
    char* key;
    pthread_mutex_t mutex;
    int ref_count;
    struct mutex_entry* next;
} mutex_entry_t;

// Global mutex registry (protected by registry_lock)
static mutex_entry_t* mutex_registry = NULL;
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

const char* mlp_mutex_get_error(void) {
    return last_error;
}

// Find or create mutex entry by key
static mutex_entry_t* find_or_create_mutex(const char* key) {
    pthread_mutex_lock(&registry_lock);
    
    // Search for existing mutex
    mutex_entry_t* entry = mutex_registry;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->ref_count++;
            pthread_mutex_unlock(&registry_lock);
            return entry;
        }
        entry = entry->next;
    }
    
    // Create new mutex entry
    entry = (mutex_entry_t*)malloc(sizeof(mutex_entry_t));
    if (!entry) {
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    entry->key = strdup(key);
    if (!entry->key) {
        free(entry);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    // Initialize mutex
    if (pthread_mutex_init(&entry->mutex, NULL) != 0) {
        free(entry->key);
        free(entry);
        pthread_mutex_unlock(&registry_lock);
        return NULL;
    }
    
    entry->ref_count = 1;
    entry->next = mutex_registry;
    mutex_registry = entry;
    
    pthread_mutex_unlock(&registry_lock);
    return entry;
}

// Remove mutex entry if ref_count reaches zero
static void release_mutex(mutex_entry_t* entry) {
    pthread_mutex_lock(&registry_lock);
    
    entry->ref_count--;
    
    if (entry->ref_count <= 0) {
        // Remove from registry
        mutex_entry_t** ptr = &mutex_registry;
        while (*ptr) {
            if (*ptr == entry) {
                *ptr = entry->next;
                break;
            }
            ptr = &(*ptr)->next;
        }
        
        // Destroy mutex and free memory
        pthread_mutex_destroy(&entry->mutex);
        free(entry->key);
        free(entry);
    }
    
    pthread_mutex_unlock(&registry_lock);
}

// ============================================================================
// Public Mutex Operations
// ============================================================================

/**
 * Create or get mutex by key
 * 
 * Same key returns same mutex (reference counted)
 * 
 * Args:
 *   key: Unique string identifier for mutex
 * 
 * Returns:
 *   Mutex handle (>0) on success, -1 on error
 */
int64_t mlp_mutex_create(const char* key) {
    clear_error();
    
    if (!key || strlen(key) == 0) {
        set_error("Invalid mutex key");
        return -1;  // MUTEX_ERROR_CREATE
    }
    
    mutex_entry_t* entry = find_or_create_mutex(key);
    if (!entry) {
        set_error("Failed to create mutex");
        return -1;
    }
    
    return (int64_t)entry;
}

/**
 * Lock mutex (blocks if already locked)
 * 
 * Args:
 *   handle: Mutex handle from mlp_mutex_create
 * 
 * Returns:
 *   0 on success, -2 on error
 */
int64_t mlp_mutex_lock(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid mutex handle");
        return -2;  // MUTEX_ERROR_LOCK
    }
    
    mutex_entry_t* entry = (mutex_entry_t*)handle;
    
    int rc = pthread_mutex_lock(&entry->mutex);
    if (rc != 0) {
        set_error("Failed to lock mutex");
        return -2;
    }
    
    return 0;
}

/**
 * Unlock mutex
 * 
 * Args:
 *   handle: Mutex handle from mlp_mutex_create
 * 
 * Returns:
 *   0 on success, -3 on error
 */
int64_t mlp_mutex_unlock(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid mutex handle");
        return -3;  // MUTEX_ERROR_UNLOCK
    }
    
    mutex_entry_t* entry = (mutex_entry_t*)handle;
    
    int rc = pthread_mutex_unlock(&entry->mutex);
    if (rc != 0) {
        set_error("Failed to unlock mutex");
        return -3;
    }
    
    return 0;
}

/**
 * Destroy mutex and release resources
 * 
 * Note: Only destroys when ref_count reaches zero
 * (multiple with_lock() calls might share same key)
 * 
 * Args:
 *   handle: Mutex handle from mlp_mutex_create
 * 
 * Returns:
 *   0 on success, -4 on error
 */
int64_t mlp_mutex_destroy(int64_t handle) {
    clear_error();
    
    if (handle <= 0) {
        set_error("Invalid mutex handle");
        return -4;  // MUTEX_ERROR_DESTROY
    }
    
    mutex_entry_t* entry = (mutex_entry_t*)handle;
    release_mutex(entry);
    
    return 0;
}

// ============================================================================
// Future Enhancements
// ============================================================================

// TODO: Add trylock support for non-blocking mutex acquisition
// int64_t mlp_mutex_trylock(int64_t handle);

// TODO: Add timed lock support
// int64_t mlp_mutex_timedlock(int64_t handle, int64_t timeout_ms);

// TODO: Add recursive mutex support (if needed)
// int64_t mlp_mutex_create_recursive(const char* key);
