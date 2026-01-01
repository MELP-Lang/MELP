/**
 * MELP Runtime - Path Utilities Header
 * 
 * Path manipulation functions for module loading and file system operations
 * Critical for Stage2 import system - module discovery and resolution
 * 
 * Design Principles:
 * - All functions are stateless (no global state)
 * - Return allocated strings (caller must free)
 * - Functional pattern: input -> process -> output
 * - No persistent handles or state
 */

#ifndef MELP_PATH_H
#define MELP_PATH_H

#include <stdint.h>

// ============================================
// PATH OPERATIONS
// ============================================

/**
 * Join path components with platform separator
 * Example: path_join("stdlib", "io", "file.mlp") -> "stdlib/io/file.mlp"
 * @return Allocated string (caller must free), NULL on error
 */
char* mlp_path_join(const char* part1, const char* part2);
char* mlp_path_join3(const char* part1, const char* part2, const char* part3);

/**
 * Normalize path (resolve . and .. , remove duplicate separators)
 * Example: "stdlib//io/../io/./file.mlp" -> "stdlib/io/file.mlp"
 * @return Allocated string (caller must free), NULL on error
 */
char* mlp_path_normalize(const char* path);

/**
 * Get absolute path
 * @return Allocated string (caller must free), NULL on error
 */
char* mlp_path_absolute(const char* path);

/**
 * Get directory name (parent directory)
 * Example: "stdlib/io/file.mlp" -> "stdlib/io"
 * @return Allocated string (caller must free), NULL on error
 */
char* mlp_path_dirname(const char* path);

/**
 * Get base name (filename)
 * Example: "stdlib/io/file.mlp" -> "file.mlp"
 * @return Allocated string (caller must free), NULL on error
 */
char* mlp_path_basename(const char* path);

/**
 * Get file extension
 * Example: "file.mlp" -> ".mlp"
 * @return Allocated string (caller must free), empty string if no extension
 */
char* mlp_path_extension(const char* path);

/**
 * Check if path is absolute
 * @return 1 if absolute, 0 if relative
 */
int mlp_path_is_absolute(const char* path);

/**
 * Check if path exists (file or directory)
 * @return 1 if exists, 0 otherwise
 */
int mlp_path_exists(const char* path);

/**
 * Check if path is a directory
 * @return 1 if directory, 0 otherwise
 */
int mlp_path_is_directory(const char* path);

/**
 * Check if path is a regular file
 * @return 1 if file, 0 otherwise
 */
int mlp_path_is_file(const char* path);

// ============================================
// DIRECTORY OPERATIONS
// ============================================

/**
 * List directory contents (array of strings)
 * @param path Directory path
 * @param count Output: number of entries (can be NULL)
 * @return Allocated array of strings (caller must free each string and array)
 *         NULL on error
 */
char** mlp_list_directory(const char* path, int* count);

/**
 * Free directory listing
 * @param entries Array returned by mlp_list_directory
 * @param count Number of entries
 */
void mlp_free_directory_listing(char** entries, int count);

// ============================================
// FILE METADATA
// ============================================

/**
 * File metadata structure
 */
typedef struct {
    int64_t size;           // File size in bytes
    int64_t modified_time;  // Last modified time (Unix timestamp)
    int64_t created_time;   // Creation time (Unix timestamp)
    int is_directory;       // 1 if directory, 0 if file
    int is_file;            // 1 if regular file
    int is_readable;        // 1 if readable
    int is_writable;        // 1 if writable
} mlp_file_info;

/**
 * Get file metadata
 * @param path File/directory path
 * @param info Output: file metadata structure
 * @return 0 on success, -1 on error
 */
int mlp_file_stat(const char* path, mlp_file_info* info);

#endif // MELP_PATH_H
