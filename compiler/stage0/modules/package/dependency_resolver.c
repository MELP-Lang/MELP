#include "dependency_resolver.h"
#include "package_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// VERSION COMPARISON
// ============================================================================

/**
 * Check if a dependency is satisfied
 * Simple version: exact match or greater
 */
bool dependency_version_satisfied(SemanticVersion required, SemanticVersion available) {
    // Major version must match
    if (available.major != required.major) {
        return available.major > required.major;
    }
    
    // Minor version must be >= required
    if (available.minor < required.minor) {
        return false;
    }
    
    if (available.minor > required.minor) {
        return true;
    }
    
    // Patch version must be >= required
    return available.patch >= required.patch;
}

/**
 * Format version mismatch error message
 */
char* dependency_format_version_error(
    const char* package_name,
    SemanticVersion required,
    SemanticVersion available
) {
    char* msg = (char*)malloc(256);
    if (!msg) return NULL;
    
    snprintf(msg, 256, 
            "Version mismatch for package '%s': required %d.%d.%d, available %d.%d.%d",
            package_name,
            required.major, required.minor, required.patch,
            available.major, available.minor, available.patch);
    
    return msg;
}

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
) {
    DependencyResolutionResult result;
    result.success = true;
    result.error_message = NULL;
    result.graph = graph;
    result.load_order = load_order;
    result.package_count = package_count;
    result.cycle = NULL;
    return result;
}

/**
 * Create error result
 */
DependencyResolutionResult dependency_result_error(const char* error_message) {
    DependencyResolutionResult result;
    result.success = false;
    result.error_message = strdup(error_message);
    result.graph = NULL;
    result.load_order = NULL;
    result.package_count = 0;
    result.cycle = NULL;
    return result;
}

/**
 * Create circular dependency error result
 */
DependencyResolutionResult dependency_result_circular(CircularDependencyPath* cycle) {
    DependencyResolutionResult result;
    result.success = false;
    
    char* cycle_str = circular_path_to_string(cycle);
    char* msg = (char*)malloc(512);
    snprintf(msg, 512, "Circular dependency detected: %s", cycle_str);
    free(cycle_str);
    
    result.error_message = msg;
    result.graph = NULL;
    result.load_order = NULL;
    result.package_count = 0;
    result.cycle = cycle;
    return result;
}

/**
 * Free dependency resolution result
 */
void dependency_result_free(DependencyResolutionResult* result) {
    if (!result) return;
    
    free(result->error_message);
    
    if (result->graph) {
        dependency_graph_free(result->graph);
    }
    
    free(result->load_order);
    
    if (result->cycle) {
        circular_path_free(result->cycle);
    }
}

// ============================================================================
// GRAPH BUILDING
// ============================================================================

/**
 * Recursively load dependencies and build graph
 */
bool dependency_load_recursive(
    DependencyGraph* graph,
    DependencyNode* parent_node,
    Dependency* dep,
    PackageLoaderFunc loader
) {
    if (!graph || !parent_node || !dep) return false;
    
    // Check if dependency already in graph
    DependencyNode* dep_node = dependency_graph_find_node(graph, dep->name);
    
    if (!dep_node) {
        // Load the package
        Package* dep_package = NULL;
        
        if (loader) {
            dep_package = loader(dep->name, dep->version_required);
        }
        
        if (!dep_package) {
            // Package not found - create placeholder
            dep_package = package_create();
            dep_package->name = strdup(dep->name);
            dep_package->version_str = strdup(dep->version_required);
            dep_package->version = dep->version;
            dep_package->is_valid = false;  // Mark as placeholder
        }
        
        // Validate version
        if (dep_package->is_valid) {
            if (!dependency_version_satisfied(dep->version, dep_package->version)) {
                return false;  // Version mismatch
            }
        }
        
        // Add to graph
        dep_node = dependency_graph_add_node(graph, dep_package);
        if (!dep_node) return false;
        
        // Recursively load dependencies of this package
        if (dep_package->is_valid) {
            for (int i = 0; i < dep_package->dependency_count; i++) {
                if (!dependency_load_recursive(graph, dep_node, dep_package->dependencies[i], loader)) {
                    return false;
                }
            }
        }
    }
    
    // Add edge from parent to dependency
    return dependency_graph_add_edge(parent_node, dep_node);
}

/**
 * Build dependency graph from package
 */
DependencyGraph* dependency_build_graph(Package* package, PackageLoaderFunc loader) {
    if (!package) return NULL;
    
    // Create graph
    DependencyGraph* graph = dependency_graph_create();
    if (!graph) return NULL;
    
    // Add root package
    DependencyNode* root = dependency_graph_add_node(graph, package);
    if (!root) {
        dependency_graph_free(graph);
        return NULL;
    }
    
    // Load all dependencies recursively
    for (int i = 0; i < package->dependency_count; i++) {
        if (!dependency_load_recursive(graph, root, package->dependencies[i], loader)) {
            dependency_graph_free(graph);
            return NULL;
        }
    }
    
    return graph;
}

// ============================================================================
// VALIDATION
// ============================================================================

/**
 * Validate dependency graph (check for cycles, missing packages)
 */
bool dependency_validate_graph(DependencyGraph* graph) {
    if (!graph) return false;
    
    // Check for circular dependencies
    CircularDependencyPath* cycle = dependency_graph_detect_cycle(graph);
    if (cycle) {
        circular_path_free(cycle);
        return false;
    }
    
    // Check for missing packages (invalid nodes)
    for (int i = 0; i < graph->node_count; i++) {
        if (!graph->nodes[i]->package->is_valid) {
            return false;  // Placeholder node (package not loaded)
        }
    }
    
    return true;
}

// ============================================================================
// LOAD ORDER
// ============================================================================

/**
 * Get load order from dependency graph
 */
Package** dependency_get_load_order(DependencyGraph* graph, int* out_count) {
    if (!graph || !out_count) return NULL;
    
    // Get topological sort
    int node_count = 0;
    DependencyNode** sorted_nodes = dependency_graph_topological_sort(graph, &node_count);
    
    if (!sorted_nodes) {
        *out_count = 0;
        return NULL;
    }
    
    // Extract packages
    Package** packages = (Package**)malloc(node_count * sizeof(Package*));
    if (!packages) {
        free(sorted_nodes);
        *out_count = 0;
        return NULL;
    }
    
    for (int i = 0; i < node_count; i++) {
        packages[i] = sorted_nodes[i]->package;
    }
    
    free(sorted_nodes);
    *out_count = node_count;
    return packages;
}

// ============================================================================
// MAIN RESOLUTION
// ============================================================================

/**
 * Resolve dependencies for a package
 */
DependencyResolutionResult dependency_resolve(Package* package, PackageLoaderFunc loader) {
    if (!package) {
        return dependency_result_error("NULL package provided");
    }
    
    // Build dependency graph
    DependencyGraph* graph = dependency_build_graph(package, loader);
    if (!graph) {
        return dependency_result_error("Failed to build dependency graph");
    }
    
    // Check for circular dependencies
    CircularDependencyPath* cycle = dependency_graph_detect_cycle(graph);
    if (cycle) {
        dependency_graph_free(graph);
        return dependency_result_circular(cycle);
    }
    
    // Get load order
    int package_count = 0;
    Package** load_order = dependency_get_load_order(graph, &package_count);
    
    if (!load_order) {
        dependency_graph_free(graph);
        return dependency_result_error("Failed to calculate load order");
    }
    
    return dependency_result_success(graph, load_order, package_count);
}

/**
 * Resolve dependencies from a package file path
 */
DependencyResolutionResult dependency_resolve_file(const char* package_file, PackageLoaderFunc loader) {
    if (!package_file) {
        return dependency_result_error("NULL package file path");
    }
    
    // Parse package file
    PackageParseResult parse_result = package_parse_file(package_file);
    
    if (!parse_result.success) {
        DependencyResolutionResult result = dependency_result_error(parse_result.error_message);
        free(parse_result.error_message);
        return result;
    }
    
    // Resolve dependencies
    DependencyResolutionResult result = dependency_resolve(parse_result.package, loader);
    
    // Note: We don't free parse_result.package here because it's now owned by the graph
    
    return result;
}

// ============================================================================
// DEBUG PRINTING
// ============================================================================

/**
 * Print dependency resolution result (for debugging)
 */
void dependency_result_print(DependencyResolutionResult* result) {
    if (!result) {
        printf("(null result)\n");
        return;
    }
    
    printf("\n=== Dependency Resolution Result ===\n");
    
    if (result->success) {
        printf("Status: ✅ SUCCESS\n");
        printf("Packages to load: %d\n", result->package_count);
        
        if (result->load_order) {
            printf("\nLoad order:\n");
            for (int i = 0; i < result->package_count; i++) {
                printf("  %d. %s (v%s)\n", 
                       i + 1,
                       result->load_order[i]->name,
                       result->load_order[i]->version_str);
            }
        }
        
        if (result->graph) {
            printf("\n");
            dependency_graph_print(result->graph);
        }
    } else {
        printf("Status: ❌ FAILED\n");
        printf("Error: %s\n", result->error_message ? result->error_message : "(unknown)");
        
        if (result->cycle) {
            char* cycle_str = circular_path_to_string(result->cycle);
            printf("Cycle: %s\n", cycle_str);
            free(cycle_str);
        }
    }
    
    printf("====================================\n\n");
}
