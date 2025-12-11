#ifndef IMPORT_CACHE_PERSIST_H
#define IMPORT_CACHE_PERSIST_H

#include "import_cache.h"

// YZ_43: Persistent cache system for incremental compilation
// Saves module cache to disk for cross-compilation speedup

// Cache file format: JSON
// .mlp.cache/
//   ├─ math.mlp.cache      (JSON metadata for math.mlp)
//   ├─ utils.mlp.cache     (JSON metadata for utils.mlp)
//   └─ main.mlp.cache      (JSON metadata for main.mlp)

// Cache metadata structure (saved to disk)
typedef struct CacheMetadata {
    char* module_path;              // Source file path
    time_t source_mtime;            // Source file modification time
    time_t object_mtime;            // Object file modification time (if exists)
    char* object_path;              // Object file path (e.g., math.o)
    char* assembly_path;            // Assembly file path (e.g., math.s)
    char** function_names;          // List of exported function names
    int function_count;             // Number of functions
    char** dependencies;            // List of imported module paths
    int dependency_count;           // Number of dependencies
    char* cache_version;            // Cache format version (for future compat)
} CacheMetadata;

// Initialize persistent cache directory
// Creates .mlp.cache/ if it doesn't exist
void cache_persist_init(void);

// Load cache from disk for a module
// Returns 1 if cache loaded successfully, 0 otherwise
int cache_persist_load(const char* module_path, CacheMetadata* out_metadata);

// Save cache to disk for a module
// Returns 1 if cache saved successfully, 0 otherwise
int cache_persist_save(const char* module_path, const CacheMetadata* metadata);

// Check if persistent cache is valid (source not modified, deps up-to-date)
// Returns 1 if valid, 0 if stale
int cache_persist_is_valid(const CacheMetadata* metadata);

// Invalidate persistent cache (delete .cache file)
void cache_persist_invalidate(const char* module_path);

// Clean all persistent cache (delete .mlp.cache/ directory)
void cache_persist_clean_all(void);

// Create CacheMetadata from current module state
CacheMetadata* cache_metadata_create(
    const char* module_path,
    FunctionDeclaration* functions,
    char** dependencies,
    int dependency_count,
    const char* object_path,
    const char* assembly_path
);

// Free CacheMetadata structure
void cache_metadata_free(CacheMetadata* metadata);

// Clean up CacheMetadata fields (without freeing the struct itself)
void cache_metadata_cleanup(CacheMetadata* metadata);

// Get cache file path for a module
// Returns path like ".mlp.cache/math.mlp.cache"
char* cache_get_cache_path(const char* module_path);

// Get object file path for a module
// Returns path like "math.o"
char* cache_get_object_path(const char* module_path);

// Get assembly file path for a module
// Returns path like "math.s"
char* cache_get_assembly_path(const char* module_path);

// Check if object file is up-to-date
// Returns 1 if .o is newer than .mlp, 0 otherwise
int cache_object_is_valid(const char* module_path, const char* object_path);

#endif // IMPORT_CACHE_PERSIST_H
