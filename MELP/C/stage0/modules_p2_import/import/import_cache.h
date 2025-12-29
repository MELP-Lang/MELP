#ifndef IMPORT_CACHE_H
#define IMPORT_CACHE_H

#include <time.h>
#include "../functions/functions.h"

// YZ_42: Module cache system for incremental compilation

// Cache entry for a single module
typedef struct ModuleCache {
    char* module_path;              // Full path to .mlp file
    time_t source_mtime;            // Last modification time of source
    FunctionDeclaration* functions; // Cached function declarations
    char** dependencies;            // List of imported module paths
    int dependency_count;           // Number of dependencies
    struct ModuleCache* next;       // Linked list
} ModuleCache;

// Global cache registry
extern ModuleCache* g_module_cache_head;

// Initialize cache system
void cache_init(void);

// Cleanup cache system
void cache_cleanup(void);

// Check if module is cached and up-to-date
// Returns cached functions if valid, NULL if needs recompilation
FunctionDeclaration* cache_get(const char* module_path);

// Store module in cache
void cache_put(const char* module_path, FunctionDeclaration* functions, 
               char** dependencies, int dependency_count);

// Check if source file has been modified since cache creation
int cache_is_stale(const char* module_path, time_t cached_mtime);

// Invalidate cache for a specific module
void cache_invalidate(const char* module_path);

// Get file modification time
time_t cache_get_mtime(const char* file_path);

// Print cache statistics (for debugging)
void cache_print_stats(void);

#endif // IMPORT_CACHE_H
