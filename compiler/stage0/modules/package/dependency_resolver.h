#ifndef DEPENDENCY_RESOLVER_H
#define DEPENDENCY_RESOLVER_H

#include "package_types.h"
#include "dependency_graph.h"
#include <stdbool.h>

/**
 * Dependency Resolution Result
 */
typedef struct {
    bool success;                       // Resolution successful
    char* error_message;                // Error message if failed
    
    DependencyGraph* graph;             // Dependency graph
    Package** load_order;               // Packages in load order
    int package_count;                  // Number of packages to load
    
    CircularDependencyPath* cycle;      // If circular dependency detected
} DependencyResolutionResult;

/**
 * Package Loader Function Type
 * Custom function to load a package by name
 * Returns Package* on success, NULL on failure
 */
typedef Package* (*PackageLoaderFunc)(const char* package_name, const char* version);

// ============================================================================
// MAIN RESOLUTION FUNCTIONS
// ============================================================================

/**
 * Resolve dependencies for a package
 * 
 * @param package Root package to resolve dependencies for
 * @param loader Function to load packages by name (can be NULL for testing)
 * @return DependencyResolutionResult
 */
DependencyResolutionResult dependency_resolve(Package* package, PackageLoaderFunc loader);

/**
 * Resolve dependencies from a package file path
 * 
 * @param package_file Path to package.mlp file
 * @param loader Function to load packages by name
 * @return DependencyResolutionResult
 */
DependencyResolutionResult dependency_resolve_file(const char* package_file, PackageLoaderFunc loader);

// ============================================================================
// GRAPH BUILDING
// ============================================================================

/**
 * Build dependency graph from package
 * 
 * @param package Root package
 * @param loader Function to load dependent packages
 * @return DependencyGraph or NULL on error
 */
DependencyGraph* dependency_build_graph(Package* package, PackageLoaderFunc loader);

/**
 * Recursively load dependencies and build graph
 */
bool dependency_load_recursive(
    DependencyGraph* graph,
    DependencyNode* parent_node,
    Dependency* dep,
    PackageLoaderFunc loader
);

// ============================================================================
// VALIDATION
// ============================================================================

/**
 * Check if a dependency is satisfied
 * Compares required version with available version
 * 
 * @param required Required semantic version
 * @param available Available semantic version
 * @return true if dependency is satisfied
 */
bool dependency_version_satisfied(SemanticVersion required, SemanticVersion available);

/**
 * Validate dependency graph (check for cycles, missing packages)
 * 
 * @param graph Dependency graph to validate
 * @return true if valid, false if invalid
 */
bool dependency_validate_graph(DependencyGraph* graph);

// ============================================================================
// RESULT MANAGEMENT
// ============================================================================

/**
 * Create success result
 */
DependencyResolutionResult dependency_result_success(
    DependencyGraph* graph,
    Package** load_order,
    int package_count
);

/**
 * Create error result
 */
DependencyResolutionResult dependency_result_error(const char* error_message);

/**
 * Create circular dependency error result
 */
DependencyResolutionResult dependency_result_circular(CircularDependencyPath* cycle);

/**
 * Free dependency resolution result
 */
void dependency_result_free(DependencyResolutionResult* result);

// ============================================================================
// UTILITIES
// ============================================================================

/**
 * Get load order from dependency graph
 * Returns packages in the order they should be loaded
 */
Package** dependency_get_load_order(DependencyGraph* graph, int* out_count);

/**
 * Print dependency resolution result (for debugging)
 */
void dependency_result_print(DependencyResolutionResult* result);

/**
 * Format version mismatch error message
 */
char* dependency_format_version_error(
    const char* package_name,
    SemanticVersion required,
    SemanticVersion available
);

#endif // DEPENDENCY_RESOLVER_H
