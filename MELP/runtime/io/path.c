/**
 * MELP Runtime - Path Utilities Implementation
 * 
 * Cross-platform path manipulation for module loading
 * Critical for Stage2 import system
 * 
 * Platform Support:
 * - Linux/Unix (primary)
 * - Windows (future)
 */

#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

// Platform-specific path separator
#ifdef _WIN32
    #define PATH_SEP '\\'
    #define PATH_SEP_STR "\\"
#else
    #define PATH_SEP '/'
    #define PATH_SEP_STR "/"
#endif

// ============================================
// HELPER FUNCTIONS
// ============================================

// Safe string duplication
static char* safe_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

// Check if character is path separator
static int is_separator(char c) {
    return (c == '/' || c == '\\');
}

// ============================================
// PATH JOIN
// ============================================

char* mlp_path_join(const char* part1, const char* part2) {
    if (!part1 || !part2) return NULL;
    
    size_t len1 = strlen(part1);
    size_t len2 = strlen(part2);
    
    // Check if part1 ends with separator
    int has_sep = (len1 > 0 && is_separator(part1[len1 - 1]));
    
    // Check if part2 starts with separator
    int starts_sep = (len2 > 0 && is_separator(part2[0]));
    
    // Allocate result
    size_t result_len = len1 + len2 + (has_sep || starts_sep ? 0 : 1) + 1;
    char* result = (char*)malloc(result_len);
    if (!result) return NULL;
    
    // Build path
    strcpy(result, part1);
    if (!has_sep && !starts_sep && len1 > 0 && len2 > 0) {
        strcat(result, PATH_SEP_STR);
    }
    strcat(result, part2);
    
    return result;
}

char* mlp_path_join3(const char* part1, const char* part2, const char* part3) {
    char* temp = mlp_path_join(part1, part2);
    if (!temp) return NULL;
    
    char* result = mlp_path_join(temp, part3);
    free(temp);
    return result;
}

// ============================================
// PATH NORMALIZE
// ============================================

char* mlp_path_normalize(const char* path) {
    if (!path) return NULL;
    
    size_t len = strlen(path);
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    // Split path into components
    char** components = (char**)malloc(sizeof(char*) * (len + 1));
    if (!components) {
        free(result);
        return NULL;
    }
    
    int comp_count = 0;
    char* path_copy = safe_strdup(path);
    if (!path_copy) {
        free(result);
        free(components);
        return NULL;
    }
    
    // Tokenize path
    char* token = strtok(path_copy, "/\\");
    while (token) {
        if (strcmp(token, ".") == 0) {
            // Skip current directory
        } else if (strcmp(token, "..") == 0) {
            // Go up one level
            if (comp_count > 0) {
                comp_count--;
                free(components[comp_count]);
            }
        } else if (strlen(token) > 0) {
            components[comp_count] = safe_strdup(token);
            comp_count++;
        }
        token = strtok(NULL, "/\\");
    }
    
    // Build normalized path
    result[0] = '\0';
    for (int i = 0; i < comp_count; i++) {
        if (i > 0) strcat(result, PATH_SEP_STR);
        strcat(result, components[i]);
        free(components[i]);
    }
    
    free(components);
    free(path_copy);
    
    // Empty path becomes "."
    if (result[0] == '\0') {
        strcpy(result, ".");
    }
    
    return result;
}

// ============================================
// ABSOLUTE PATH
// ============================================

char* mlp_path_absolute(const char* path) {
    if (!path) return NULL;
    
    char* abs_path = (char*)malloc(PATH_MAX);
    if (!abs_path) return NULL;
    
    #ifdef _WIN32
        if (_fullpath(abs_path, path, PATH_MAX) == NULL) {
            free(abs_path);
            return NULL;
        }
    #else
        if (realpath(path, abs_path) == NULL) {
            // If realpath fails, try manual construction
            if (path[0] == PATH_SEP) {
                // Already absolute
                free(abs_path);
                return safe_strdup(path);
            } else {
                // Prepend current directory
                if (getcwd(abs_path, PATH_MAX) == NULL) {
                    free(abs_path);
                    return NULL;
                }
                char* joined = mlp_path_join(abs_path, path);
                free(abs_path);
                return joined;
            }
        }
    #endif
    
    return abs_path;
}

// ============================================
// DIRNAME / BASENAME
// ============================================

char* mlp_path_dirname(const char* path) {
    if (!path) return NULL;
    
    size_t len = strlen(path);
    if (len == 0) return safe_strdup(".");
    
    // Find last separator
    int last_sep = -1;
    for (int i = len - 1; i >= 0; i--) {
        if (is_separator(path[i])) {
            last_sep = i;
            break;
        }
    }
    
    if (last_sep == -1) {
        // No separator, return current directory
        return safe_strdup(".");
    }
    
    if (last_sep == 0) {
        // Root directory
        return safe_strdup(PATH_SEP_STR);
    }
    
    // Copy up to last separator
    char* result = (char*)malloc(last_sep + 1);
    if (!result) return NULL;
    memcpy(result, path, last_sep);
    result[last_sep] = '\0';
    
    return result;
}

char* mlp_path_basename(const char* path) {
    if (!path) return NULL;
    
    size_t len = strlen(path);
    if (len == 0) return safe_strdup("");
    
    // Find last separator
    int last_sep = -1;
    for (int i = len - 1; i >= 0; i--) {
        if (is_separator(path[i])) {
            last_sep = i;
            break;
        }
    }
    
    // Return everything after last separator
    return safe_strdup(path + last_sep + 1);
}

// ============================================
// EXTENSION
// ============================================

char* mlp_path_extension(const char* path) {
    if (!path) return safe_strdup("");
    
    // Get basename first
    char* base = mlp_path_basename(path);
    if (!base) return safe_strdup("");
    
    // Find last dot
    char* dot = strrchr(base, '.');
    char* result;
    
    if (dot && dot != base) {
        // Has extension
        result = safe_strdup(dot);
    } else {
        // No extension
        result = safe_strdup("");
    }
    
    free(base);
    return result;
}

// ============================================
// PATH CHECKS
// ============================================

int mlp_path_is_absolute(const char* path) {
    if (!path || strlen(path) == 0) return 0;
    
    #ifdef _WIN32
        // Windows: C:\ or \\
        return (strlen(path) >= 3 && path[1] == ':' && is_separator(path[2])) ||
               (strlen(path) >= 2 && is_separator(path[0]) && is_separator(path[1]));
    #else
        // Unix: starts with /
        return path[0] == PATH_SEP;
    #endif
}

int mlp_path_exists(const char* path) {
    if (!path) return 0;
    
    struct stat st;
    return (stat(path, &st) == 0);
}

int mlp_path_is_directory(const char* path) {
    if (!path) return 0;
    
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    
    return S_ISDIR(st.st_mode);
}

int mlp_path_is_file(const char* path) {
    if (!path) return 0;
    
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    
    return S_ISREG(st.st_mode);
}

// ============================================
// DIRECTORY LISTING
// ============================================

char** mlp_list_directory(const char* path, int* count) {
    if (!path) {
        if (count) *count = 0;
        return NULL;
    }
    
    DIR* dir = opendir(path);
    if (!dir) {
        if (count) *count = 0;
        return NULL;
    }
    
    // First pass: count entries
    int entry_count = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        closedir(dir);
        if (count) *count = 0;
        return NULL;
    }
    
    // Allocate array
    char** entries = (char**)malloc(sizeof(char*) * entry_count);
    if (!entries) {
        closedir(dir);
        if (count) *count = 0;
        return NULL;
    }
    
    // Second pass: collect entries
    rewinddir(dir);
    int i = 0;
    while ((entry = readdir(dir)) != NULL && i < entry_count) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            entries[i] = safe_strdup(entry->d_name);
            i++;
        }
    }
    
    closedir(dir);
    
    if (count) *count = entry_count;
    return entries;
}

void mlp_free_directory_listing(char** entries, int count) {
    if (!entries) return;
    
    for (int i = 0; i < count; i++) {
        free(entries[i]);
    }
    free(entries);
}

// ============================================
// FILE METADATA
// ============================================

int mlp_file_stat(const char* path, mlp_file_info* info) {
    if (!path || !info) return -1;
    
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    
    info->size = st.st_size;
    info->modified_time = st.st_mtime;
    info->created_time = st.st_ctime;
    info->is_directory = S_ISDIR(st.st_mode);
    info->is_file = S_ISREG(st.st_mode);
    info->is_readable = (access(path, R_OK) == 0);
    info->is_writable = (access(path, W_OK) == 0);
    
    return 0;
}
