#ifndef PACKAGE_BUILDER_H
#define PACKAGE_BUILDER_H

#include "package_types.h"
#include "dependency_resolver.h"
#include <stdbool.h>

/**
 * Build Configuration
 */
typedef struct {
    char* build_dir;                // Build output directory (default: "build/")
    char* dist_dir;                 // Distribution directory (default: "dist/")
    char* obj_dir;                  // Object files directory (default: "build/obj/")
    char* bin_dir;                  // Binary output directory (default: "build/bin/")
    
    bool clean_build;               // Clean before building
    bool verbose;                   // Verbose output
    bool debug_mode;                // Debug build
} BuildConfig;

/**
 * Build Result
 */
typedef struct {
    bool success;                   // Build successful
    char* error_message;            // Error message if failed
    
    char* output_path;              // Path to built artifact
    int files_compiled;             // Number of files compiled
    double build_time;              // Build time in seconds
} BuildResult;

// ============================================================================
// BUILD CONFIGURATION
// ============================================================================

/**
 * Create default build configuration
 */
BuildConfig* build_config_create(void);

/**
 * Free build configuration
 */
void build_config_free(BuildConfig* config);

/**
 * Load build configuration from package
 */
BuildConfig* build_config_from_package(Package* package);

/**
 * Print build configuration
 */
void build_config_print(BuildConfig* config);

// ============================================================================
// DIRECTORY MANAGEMENT
// ============================================================================

/**
 * Create build directory structure
 * Creates: build/, build/obj/, build/bin/, dist/
 */
bool build_create_directories(BuildConfig* config);

/**
 * Clean build directories (remove old artifacts)
 */
bool build_clean_directories(BuildConfig* config);

/**
 * Check if directory exists
 */
bool build_directory_exists(const char* path);

/**
 * Create directory (mkdir -p equivalent)
 */
bool build_create_directory(const char* path);

// ============================================================================
// PACKAGE BUILDING
// ============================================================================

/**
 * Build a package
 * 
 * @param package Package to build
 * @param config Build configuration
 * @return BuildResult
 */
BuildResult build_package(Package* package, BuildConfig* config);

/**
 * Build with dependency resolution
 * Resolves dependencies and builds in correct order
 * 
 * @param package_file Path to package.mlp
 * @param config Build configuration
 * @return BuildResult
 */
BuildResult build_package_with_deps(const char* package_file, BuildConfig* config);

/**
 * Compile a single source file
 * 
 * @param source_file Path to .mlp source file
 * @param output_file Path to output object file
 * @param config Build configuration
 * @return true on success
 */
bool build_compile_file(const char* source_file, const char* output_file, BuildConfig* config);

// ============================================================================
// BUILD RESULT
// ============================================================================

/**
 * Create success result
 */
BuildResult build_result_success(const char* output_path, int files_compiled, double build_time);

/**
 * Create error result
 */
BuildResult build_result_error(const char* error_message);

/**
 * Free build result
 */
void build_result_free(BuildResult* result);

/**
 * Print build result
 */
void build_result_print(BuildResult* result);

// ============================================================================
// UTILITIES
// ============================================================================

/**
 * Get output path for package
 * Returns: "build/bin/package_name"
 */
char* build_get_output_path(Package* package, BuildConfig* config);

/**
 * Get object file path for source file
 * Returns: "build/obj/filename.o"
 */
char* build_get_object_path(const char* source_file, BuildConfig* config);

/**
 * Check if rebuild is needed (source newer than object)
 */
bool build_needs_rebuild(const char* source_file, const char* object_file);

#endif // PACKAGE_BUILDER_H
