#include "import_cache_persist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

// YZ_43: Persistent cache implementation for incremental compilation

#define CACHE_DIR ".mlp.cache"
#define CACHE_VERSION "1.0"

// Initialize persistent cache directory
void cache_persist_init(void) {
    struct stat st = {0};
    
    // Check if cache directory exists
    if (stat(CACHE_DIR, &st) == -1) {
        // Create directory
        if (mkdir(CACHE_DIR, 0755) == -1) {
            fprintf(stderr, "Warning: Could not create cache directory: %s\n", strerror(errno));
        } else {
            printf("📁 Created cache directory: %s\n", CACHE_DIR);
        }
    }
}

// Get cache file path for a module
char* cache_get_cache_path(const char* module_path) {
    // Convert module path to cache filename
    // Example: modules/core/math.mlp -> .mlp.cache/modules_core_math.mlp.cache
    
    char* cache_path = malloc(512);
    if (!cache_path) return NULL;
    
    // Replace '/' with '_' in module path
    char safe_name[256];
    const char* p = module_path;
    char* q = safe_name;
    while (*p && (q - safe_name) < 255) {
        if (*p == '/') {
            *q++ = '_';
        } else {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';
    
    snprintf(cache_path, 512, "%s/%s.cache", CACHE_DIR, safe_name);
    return cache_path;
}

// Get object file path for a module
char* cache_get_object_path(const char* module_path) {
    // modules/core/math.mlp -> math.o
    const char* basename = strrchr(module_path, '/');
    if (basename) {
        basename++;  // Skip '/'
    } else {
        basename = module_path;
    }
    
    // Remove .mlp extension
    char* object_path = malloc(256);
    if (!object_path) return NULL;
    
    const char* ext = strrchr(basename, '.');
    if (ext) {
        size_t len = ext - basename;
        strncpy(object_path, basename, len);
        object_path[len] = '\0';
    } else {
        strcpy(object_path, basename);
    }
    
    strcat(object_path, ".o");
    return object_path;
}

// Get assembly file path for a module
char* cache_get_assembly_path(const char* module_path) {
    // modules/core/math.mlp -> math.s
    const char* basename = strrchr(module_path, '/');
    if (basename) {
        basename++;  // Skip '/'
    } else {
        basename = module_path;
    }
    
    // Remove .mlp extension
    char* assembly_path = malloc(256);
    if (!assembly_path) return NULL;
    
    const char* ext = strrchr(basename, '.');
    if (ext) {
        size_t len = ext - basename;
        strncpy(assembly_path, basename, len);
        assembly_path[len] = '\0';
    } else {
        strcpy(assembly_path, basename);
    }
    
    strcat(assembly_path, ".s");
    return assembly_path;
}

// Check if object file is up-to-date
int cache_object_is_valid(const char* module_path, const char* object_path) {
    struct stat source_st, object_st;
    
    // Get source file mtime
    if (stat(module_path, &source_st) != 0) {
        return 0;  // Source doesn't exist
    }
    
    // Get object file mtime
    if (stat(object_path, &object_st) != 0) {
        return 0;  // Object doesn't exist
    }
    
    // Object is valid if it's newer than source
    return object_st.st_mtime >= source_st.st_mtime;
}

// Create CacheMetadata from current module state
CacheMetadata* cache_metadata_create(
    const char* module_path,
    FunctionDeclaration* functions,
    char** dependencies,
    int dependency_count,
    const char* object_path,
    const char* assembly_path
) {
    CacheMetadata* metadata = malloc(sizeof(CacheMetadata));
    if (!metadata) return NULL;
    
    metadata->module_path = strdup(module_path);
    metadata->cache_version = strdup(CACHE_VERSION);
    
    // Get source file mtime
    struct stat st;
    if (stat(module_path, &st) == 0) {
        metadata->source_mtime = st.st_mtime;
    } else {
        metadata->source_mtime = 0;
    }
    
    // Get object file mtime (if exists)
    if (object_path && stat(object_path, &st) == 0) {
        metadata->object_mtime = st.st_mtime;
        metadata->object_path = strdup(object_path);
    } else {
        metadata->object_mtime = 0;
        metadata->object_path = NULL;
    }
    
    // Get assembly file path
    if (assembly_path) {
        metadata->assembly_path = strdup(assembly_path);
    } else {
        metadata->assembly_path = NULL;
    }
    
    // Extract function names
    int func_count = 0;
    FunctionDeclaration* func = functions;
    while (func) {
        func_count++;
        func = func->next;
    }
    
    metadata->function_count = func_count;
    if (func_count > 0) {
        metadata->function_names = malloc(sizeof(char*) * func_count);
        func = functions;
        int i = 0;
        while (func) {
            metadata->function_names[i++] = strdup(func->name);
            func = func->next;
        }
    } else {
        metadata->function_names = NULL;
    }
    
    // Copy dependencies
    metadata->dependency_count = dependency_count;
    if (dependency_count > 0) {
        metadata->dependencies = malloc(sizeof(char*) * dependency_count);
        for (int i = 0; i < dependency_count; i++) {
            metadata->dependencies[i] = strdup(dependencies[i]);
        }
    } else {
        metadata->dependencies = NULL;
    }
    
    return metadata;
}

// Clean up CacheMetadata fields (without freeing the struct itself)
void cache_metadata_cleanup(CacheMetadata* metadata) {
    if (!metadata) return;
    
    if (metadata->module_path) free(metadata->module_path);
    if (metadata->cache_version) free(metadata->cache_version);
    if (metadata->object_path) free(metadata->object_path);
    if (metadata->assembly_path) free(metadata->assembly_path);
    
    if (metadata->function_names) {
        for (int i = 0; i < metadata->function_count; i++) {
            if (metadata->function_names[i]) free(metadata->function_names[i]);
        }
        free(metadata->function_names);
    }
    
    if (metadata->dependencies) {
        for (int i = 0; i < metadata->dependency_count; i++) {
            if (metadata->dependencies[i]) free(metadata->dependencies[i]);
        }
        free(metadata->dependencies);
    }
}

// Free CacheMetadata structure
void cache_metadata_free(CacheMetadata* metadata) {
    cache_metadata_cleanup(metadata);
    free(metadata);
}

// Save cache to disk for a module (JSON format)
int cache_persist_save(const char* module_path, const CacheMetadata* metadata) {
    char* cache_path = cache_get_cache_path(module_path);
    if (!cache_path) return 0;
    
    FILE* file = fopen(cache_path, "w");
    if (!file) {
        fprintf(stderr, "Warning: Could not save cache: %s\n", cache_path);
        free(cache_path);
        return 0;
    }
    
    // Write JSON
    fprintf(file, "{\n");
    fprintf(file, "  \"version\": \"%s\",\n", metadata->cache_version);
    fprintf(file, "  \"module_path\": \"%s\",\n", metadata->module_path);
    fprintf(file, "  \"source_mtime\": %ld,\n", (long)metadata->source_mtime);
    fprintf(file, "  \"object_mtime\": %ld,\n", (long)metadata->object_mtime);
    
    if (metadata->object_path) {
        fprintf(file, "  \"object_path\": \"%s\",\n", metadata->object_path);
    } else {
        fprintf(file, "  \"object_path\": null,\n");
    }
    
    if (metadata->assembly_path) {
        fprintf(file, "  \"assembly_path\": \"%s\",\n", metadata->assembly_path);
    } else {
        fprintf(file, "  \"assembly_path\": null,\n");
    }
    
    // Functions
    fprintf(file, "  \"functions\": [\n");
    for (int i = 0; i < metadata->function_count; i++) {
        fprintf(file, "    \"%s\"", metadata->function_names[i]);
        if (i < metadata->function_count - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    fprintf(file, "  ],\n");
    
    // Dependencies
    fprintf(file, "  \"dependencies\": [\n");
    for (int i = 0; i < metadata->dependency_count; i++) {
        fprintf(file, "    \"%s\"", metadata->dependencies[i]);
        if (i < metadata->dependency_count - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
    
    fclose(file);
    
    printf("   💾 Saved cache: %s\n", cache_path);
    free(cache_path);
    return 1;
}

// Simple JSON parser for cache loading
static char* json_extract_string(const char* json, const char* key) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\": ", key);
    
    const char* start = strstr(json, search_key);
    if (!start) return NULL;
    
    start += strlen(search_key);
    
    // Check for null value
    if (strncmp(start, "null", 4) == 0) {
        return NULL;
    }
    
    // Skip opening quote
    if (*start != '"') return NULL;
    start++;
    
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    
    size_t len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    return value;
}

static long json_extract_long(const char* json, const char* key) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\": ", key);
    
    const char* start = strstr(json, search_key);
    if (!start) return 0;
    
    start += strlen(search_key);
    return atol(start);
}

// Load cache from disk for a module
int cache_persist_load(const char* module_path, CacheMetadata* out_metadata) {
    char* cache_path = cache_get_cache_path(module_path);
    if (!cache_path) return 0;
    
    FILE* file = fopen(cache_path, "r");
    if (!file) {
        free(cache_path);
        return 0;  // Cache doesn't exist
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* json = malloc(size + 1);
    if (!json) {
        fclose(file);
        free(cache_path);
        return 0;
    }
    
    fread(json, 1, size, file);
    json[size] = '\0';
    fclose(file);
    
    // Parse JSON
    out_metadata->cache_version = json_extract_string(json, "version");
    out_metadata->module_path = json_extract_string(json, "module_path");
    out_metadata->object_path = json_extract_string(json, "object_path");
    out_metadata->assembly_path = json_extract_string(json, "assembly_path");
    out_metadata->source_mtime = json_extract_long(json, "source_mtime");
    out_metadata->object_mtime = json_extract_long(json, "object_mtime");
    
    // Parse functions array (simplified - just count)
    const char* func_start = strstr(json, "\"functions\": [");
    if (func_start) {
        out_metadata->function_count = 0;
        const char* p = func_start;
        while ((p = strchr(p, '"')) != NULL && p < strstr(json, "],")) {
            out_metadata->function_count++;
            p++;
        }
        out_metadata->function_count /= 2;  // Each function has 2 quotes
    } else {
        out_metadata->function_count = 0;
    }
    out_metadata->function_names = NULL;  // Don't need names for validation
    
    // Parse dependencies array (simplified - just count)
    const char* dep_start = strstr(json, "\"dependencies\": [");
    if (dep_start) {
        out_metadata->dependency_count = 0;
        const char* p = dep_start;
        const char* dep_end = strstr(p, "]");
        while ((p = strchr(p, '"')) != NULL && p < dep_end) {
            out_metadata->dependency_count++;
            p++;
        }
        out_metadata->dependency_count /= 2;  // Each dependency has 2 quotes
    } else {
        out_metadata->dependency_count = 0;
    }
    out_metadata->dependencies = NULL;  // Don't need dep names for validation
    
    free(json);
    free(cache_path);
    
    printf("   📂 Loaded cache: %s\n", module_path);
    return 1;
}

// Check if persistent cache is valid
int cache_persist_is_valid(const CacheMetadata* metadata) {
    struct stat st;
    
    // Check if source file still exists
    if (stat(metadata->module_path, &st) != 0) {
        return 0;  // Source file missing
    }
    
    // Check if source file has been modified
    if (st.st_mtime > metadata->source_mtime) {
        printf("   ♻️  Cache stale: source modified (%s)\n", metadata->module_path);
        return 0;  // Source modified
    }
    
    // Check if object file exists and is valid
    if (metadata->object_path) {
        if (stat(metadata->object_path, &st) != 0) {
            printf("   ♻️  Cache stale: object missing (%s)\n", metadata->object_path);
            return 0;  // Object file missing
        }
        
        // Object should be newer than source
        if (st.st_mtime < metadata->source_mtime) {
            printf("   ♻️  Cache stale: object older than source\n");
            return 0;
        }
    }
    
    return 1;  // Cache is valid
}

// Invalidate persistent cache
void cache_persist_invalidate(const char* module_path) {
    char* cache_path = cache_get_cache_path(module_path);
    if (!cache_path) return;
    
    if (unlink(cache_path) == 0) {
        printf("   🗑️  Invalidated cache: %s\n", cache_path);
    }
    
    free(cache_path);
}

// Clean all persistent cache
void cache_persist_clean_all(void) {
    printf("🗑️  Cleaning cache directory: %s\n", CACHE_DIR);
    
    // Remove all .cache files
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s/*.cache", CACHE_DIR);
    system(command);
}
