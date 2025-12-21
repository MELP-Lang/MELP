#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

#include "package_types.h"
#include <stdbool.h>

/**
 * Dependency Graph Node
 * Represents a package in the dependency graph
 */
typedef struct DependencyNode {
    Package* package;                   // Package data
    struct DependencyNode** dependencies; // Array of dependent nodes
    int dependency_count;               // Number of dependencies
    
    // Graph traversal state
    bool visited;                       // For DFS/BFS traversal
    bool in_path;                       // For cycle detection
    int depth;                          // Distance from root
} DependencyNode;

/**
 * Dependency Graph
 * Manages the full dependency tree
 */
typedef struct {
    DependencyNode** nodes;             // Array of all nodes
    int node_count;                     // Number of nodes
    int capacity;                       // Array capacity
    
    DependencyNode* root;               // Root package node
} DependencyGraph;

/**
 * Circular dependency path
 * Represents a detected cycle in the dependency graph
 */
typedef struct {
    char** package_names;               // Names in cycle path
    int path_length;                    // Length of cycle
} CircularDependencyPath;

// ============================================================================
// GRAPH CREATION
// ============================================================================

/**
 * Create a new empty dependency graph
 */
DependencyGraph* dependency_graph_create(void);

/**
 * Free a dependency graph and all its nodes
 */
void dependency_graph_free(DependencyGraph* graph);

/**
 * Add a package node to the graph
 */
DependencyNode* dependency_graph_add_node(DependencyGraph* graph, Package* package);

/**
 * Add a dependency edge between two nodes
 */
bool dependency_graph_add_edge(DependencyNode* from, DependencyNode* to);

/**
 * Find a node by package name
 */
DependencyNode* dependency_graph_find_node(DependencyGraph* graph, const char* package_name);

// ============================================================================
// GRAPH ALGORITHMS
// ============================================================================

/**
 * Detect circular dependencies in the graph using DFS
 * Returns NULL if no cycle found, otherwise returns the cycle path
 */
CircularDependencyPath* dependency_graph_detect_cycle(DependencyGraph* graph);

/**
 * Internal DFS for cycle detection
 */
bool dependency_graph_dfs_cycle(DependencyNode* node, CircularDependencyPath* path);

/**
 * Get topological sort order (dependency resolution order)
 * Returns array of nodes in the order they should be loaded
 */
DependencyNode** dependency_graph_topological_sort(DependencyGraph* graph, int* out_count);

/**
 * Calculate dependency depth (distance from root)
 */
void dependency_graph_calculate_depths(DependencyGraph* graph);

/**
 * Reset graph traversal state (visited, in_path flags)
 */
void dependency_graph_reset_state(DependencyGraph* graph);

// ============================================================================
// PATH UTILITIES
// ============================================================================

/**
 * Create circular dependency path
 */
CircularDependencyPath* circular_path_create(int initial_capacity);

/**
 * Add package name to circular path
 */
void circular_path_add(CircularDependencyPath* path, const char* package_name);

/**
 * Free circular dependency path
 */
void circular_path_free(CircularDependencyPath* path);

/**
 * Format circular path as string: "A -> B -> C -> A"
 */
char* circular_path_to_string(CircularDependencyPath* path);

// ============================================================================
// DEBUG/VISUALIZATION
// ============================================================================

/**
 * Print dependency graph for debugging
 */
void dependency_graph_print(DependencyGraph* graph);

/**
 * Print a single node and its dependencies
 */
void dependency_node_print(DependencyNode* node, int indent);

#endif // DEPENDENCY_GRAPH_H
