#ifndef PACKAGE_TYPES_H
#define PACKAGE_TYPES_H

#include <stdbool.h>

/**
 * Semantic version structure
 */
typedef struct {
    int major;
    int minor;
    int patch;
} SemanticVersion;

/**
 * Dependency structure
 * Represents a single dependency in the package manifest
 */
typedef struct {
    char* name;                     // Package name (e.g., "stdlib")
    char* version_required;         // Required version string (e.g., "1.0.0")
    SemanticVersion version;        // Parsed semantic version
    void* resolved_package;         // Pointer to resolved Package (NULL if not loaded)
} Dependency;

/**
 * Package structure
 * Represents a complete package manifest (package.mlp)
 */
typedef struct {
    // Required fields
    char* name;                     // Package name
    char* version_str;              // Version string (e.g., "1.0.0")
    SemanticVersion version;        // Parsed semantic version
    
    // Optional fields
    char* author;                   // Author name (can be NULL)
    char* entry;                    // Entry point file path (can be NULL)
    char* description;              // Package description (can be NULL)
    
    // Dependencies
    Dependency** dependencies;      // Array of dependency pointers
    int dependency_count;           // Number of dependencies
    
    // Build configuration
    char* build_dir;                // Build output directory (default: "build/")
    char* dist_dir;                 // Distribution directory (default: "dist/")
    
    // Metadata
    char* manifest_path;            // Path to package.mlp file
    bool is_valid;                  // Validation status
} Package;

/**
 * Package parser result
 * Contains either a successful package or error information
 */
typedef struct {
    Package* package;               // Parsed package (NULL on error)
    bool success;                   // Parse success status
    char* error_message;            // Error message (NULL on success)
    int error_line;                 // Line number of error (-1 if not applicable)
} PackageParseResult;

#endif // PACKAGE_TYPES_H
