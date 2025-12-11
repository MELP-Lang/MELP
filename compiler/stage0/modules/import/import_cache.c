#include "import_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// YZ_42: Module cache implementation for incremental compilation

// Global cache registry
ModuleCache* g_module_cache_head = NULL;

// Initialize cache system
void cache_init(void) {
    g_module_cache_head = NULL;
}

// Cleanup cache system
void cache_cleanup(void) {
    ModuleCache* current = g_module_cache_head;
    while (current) {
        ModuleCache* next = current->next;
        
        // Free module path
        free(current->module_path);
        
        // Free dependencies
        for (int i = 0; i < current->dependency_count; i++) {
            free(current->dependencies[i]);
        }
        free(current->dependencies);
        
        // Note: We don't free functions here because they're owned by the caller
        // The caller (import system) will free them when needed
        
        free(current);
        current = next;
    }
    g_module_cache_head = NULL;
}

// Get file modification time
time_t cache_get_mtime(const char* file_path) {
    struct stat st;
    if (stat(file_path, &st) != 0) {
        return 0;  // File doesn't exist or can't access
    }
    return st.st_mtime;
}

// Check if source file has been modified since cache creation
int cache_is_stale(const char* module_path, time_t cached_mtime) {
    time_t current_mtime = cache_get_mtime(module_path);
    if (current_mtime == 0) {
        return 1;  // File doesn't exist - cache is stale
    }
    return current_mtime > cached_mtime;  // Source is newer than cache
}

// Find cache entry by module path
static ModuleCache* cache_find(const char* module_path) {
    ModuleCache* current = g_module_cache_head;
    while (current) {
        if (strcmp(current->module_path, module_path) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Check if module is cached and up-to-date
FunctionDeclaration* cache_get(const char* module_path) {
    ModuleCache* entry = cache_find(module_path);
    if (!entry) {
        return NULL;  // Not cached
    }
    
    // Check if cache is stale (source file modified)
    if (cache_is_stale(module_path, entry->source_mtime)) {
        printf("   ♻️  Cache stale for: %s (source modified)\n", module_path);
        return NULL;  // Cache is outdated
    }
    
    // Check if any dependencies have been modified
    for (int i = 0; i < entry->dependency_count; i++) {
        ModuleCache* dep_entry = cache_find(entry->dependencies[i]);
        if (!dep_entry) {
            printf("   ♻️  Cache stale for: %s (dependency %s not cached)\n", 
                   module_path, entry->dependencies[i]);
            return NULL;  // Dependency not cached
        }
        
        if (cache_is_stale(entry->dependencies[i], dep_entry->source_mtime)) {
            printf("   ♻️  Cache stale for: %s (dependency %s modified)\n", 
                   module_path, entry->dependencies[i]);
            return NULL;  // Dependency modified
        }
    }
    
    printf("   ⚡ Cache hit for: %s (skipping parse)\n", module_path);
    return entry->functions;
}

// Deep copy function declarations (for caching)
static FunctionDeclaration* functions_deep_copy(FunctionDeclaration* funcs) {
    if (!funcs) return NULL;
    
    FunctionDeclaration* new_head = NULL;
    FunctionDeclaration* new_tail = NULL;
    
    FunctionDeclaration* current = funcs;
    while (current) {
        // Allocate new function declaration
        FunctionDeclaration* new_func = malloc(sizeof(FunctionDeclaration));
        if (!new_func) return NULL;
        
        // Copy basic fields
        new_func->name = strdup(current->name);
        new_func->return_type = current->return_type;
        new_func->param_count = current->param_count;
        new_func->local_var_count = current->local_var_count;
        new_func->next = NULL;
        
        // For cache purposes, we only need function signatures
        // Body, params, and local_vars can be shallow copied or set to NULL
        // Since we're only using this for function registry lookup
        new_func->body = NULL;
        new_func->params = NULL;
        new_func->local_vars = NULL;
        
        // Add to list
        if (!new_head) {
            new_head = new_func;
            new_tail = new_func;
        } else {
            new_tail->next = new_func;
            new_tail = new_func;
        }
        
        current = current->next;
    }
    
    return new_head;
}

// Store module in cache
void cache_put(const char* module_path, FunctionDeclaration* functions,
               char** dependencies, int dependency_count) {
    // Remove existing cache entry if present
    cache_invalidate(module_path);
    
    // Create new cache entry
    ModuleCache* entry = malloc(sizeof(ModuleCache));
    if (!entry) return;
    
    entry->module_path = strdup(module_path);
    entry->source_mtime = cache_get_mtime(module_path);
    entry->functions = functions_deep_copy(functions);  // Deep copy for cache
    entry->dependency_count = dependency_count;
    
    // Copy dependencies
    if (dependency_count > 0) {
        entry->dependencies = malloc(sizeof(char*) * dependency_count);
        for (int i = 0; i < dependency_count; i++) {
            entry->dependencies[i] = strdup(dependencies[i]);
        }
    } else {
        entry->dependencies = NULL;
    }
    
    // Add to head of list
    entry->next = g_module_cache_head;
    g_module_cache_head = entry;
    
    printf("   💾 Cached: %s (%d function(s), %d dep(s))\n", 
           module_path, 
           functions ? 1 : 0,  // Simple count
           dependency_count);
}

// Invalidate cache for a specific module
void cache_invalidate(const char* module_path) {
    ModuleCache* current = g_module_cache_head;
    ModuleCache* prev = NULL;
    
    while (current) {
        if (strcmp(current->module_path, module_path) == 0) {
            // Remove from list
            if (prev) {
                prev->next = current->next;
            } else {
                g_module_cache_head = current->next;
            }
            
            // Free entry
            free(current->module_path);
            for (int i = 0; i < current->dependency_count; i++) {
                free(current->dependencies[i]);
            }
            free(current->dependencies);
            
            // Free cached functions
            FunctionDeclaration* func = current->functions;
            while (func) {
                FunctionDeclaration* next = func->next;
                free(func->name);
                free(func);
                func = next;
            }
            
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Print cache statistics (for debugging)
void cache_print_stats(void) {
    int count = 0;
    ModuleCache* current = g_module_cache_head;
    
    printf("\n📊 Module Cache Statistics:\n");
    while (current) {
        count++;
        printf("   %d. %s (mtime: %ld, deps: %d)\n", 
               count, current->module_path, 
               (long)current->source_mtime,
               current->dependency_count);
        current = current->next;
    }
    printf("   Total cached modules: %d\n\n", count);
}
