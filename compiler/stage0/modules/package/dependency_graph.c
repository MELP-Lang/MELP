#include "dependency_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// GRAPH CREATION
// ============================================================================

/**
 * Create a new empty dependency graph
 */
DependencyGraph* dependency_graph_create(void) {
    DependencyGraph* graph = (DependencyGraph*)calloc(1, sizeof(DependencyGraph));
    if (!graph) return NULL;
    
    graph->nodes = NULL;
    graph->node_count = 0;
    graph->capacity = 0;
    graph->root = NULL;
    
    return graph;
}

/**
 * Free a dependency node
 */
static void dependency_node_free(DependencyNode* node) {
    if (!node) return;
    
    free(node->dependencies);
    free(node);
}

/**
 * Free a dependency graph and all its nodes
 */
void dependency_graph_free(DependencyGraph* graph) {
    if (!graph) return;
    
    if (graph->nodes) {
        for (int i = 0; i < graph->node_count; i++) {
            dependency_node_free(graph->nodes[i]);
        }
        free(graph->nodes);
    }
    
    free(graph);
}

/**
 * Create a new dependency node
 */
static DependencyNode* dependency_node_create(Package* package) {
    DependencyNode* node = (DependencyNode*)calloc(1, sizeof(DependencyNode));
    if (!node) return NULL;
    
    node->package = package;
    node->dependencies = NULL;
    node->dependency_count = 0;
    node->visited = false;
    node->in_path = false;
    node->depth = 0;
    
    return node;
}

/**
 * Add a package node to the graph
 */
DependencyNode* dependency_graph_add_node(DependencyGraph* graph, Package* package) {
    if (!graph || !package) return NULL;
    
    // Check if node already exists
    DependencyNode* existing = dependency_graph_find_node(graph, package->name);
    if (existing) return existing;
    
    // Create new node
    DependencyNode* node = dependency_node_create(package);
    if (!node) return NULL;
    
    // Expand array if needed
    if (graph->node_count >= graph->capacity) {
        int new_capacity = (graph->capacity == 0) ? 8 : graph->capacity * 2;
        DependencyNode** new_nodes = (DependencyNode**)realloc(
            graph->nodes, new_capacity * sizeof(DependencyNode*)
        );
        if (!new_nodes) {
            dependency_node_free(node);
            return NULL;
        }
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    
    // Add node
    graph->nodes[graph->node_count++] = node;
    
    // Set as root if first node
    if (graph->node_count == 1) {
        graph->root = node;
    }
    
    return node;
}

/**
 * Find a node by package name
 */
DependencyNode* dependency_graph_find_node(DependencyGraph* graph, const char* package_name) {
    if (!graph || !package_name) return NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->package && 
            strcmp(graph->nodes[i]->package->name, package_name) == 0) {
            return graph->nodes[i];
        }
    }
    
    return NULL;
}

/**
 * Add a dependency edge between two nodes
 */
bool dependency_graph_add_edge(DependencyNode* from, DependencyNode* to) {
    if (!from || !to) return false;
    
    // Check if edge already exists
    for (int i = 0; i < from->dependency_count; i++) {
        if (from->dependencies[i] == to) {
            return true;  // Already connected
        }
    }
    
    // Expand dependencies array
    DependencyNode** new_deps = (DependencyNode**)realloc(
        from->dependencies, 
        (from->dependency_count + 1) * sizeof(DependencyNode*)
    );
    if (!new_deps) return false;
    
    from->dependencies = new_deps;
    from->dependencies[from->dependency_count++] = to;
    
    return true;
}

// ============================================================================
// CIRCULAR DEPENDENCY PATH
// ============================================================================

/**
 * Create circular dependency path
 */
CircularDependencyPath* circular_path_create(int initial_capacity) {
    CircularDependencyPath* path = (CircularDependencyPath*)calloc(1, sizeof(CircularDependencyPath));
    if (!path) return NULL;
    
    path->package_names = (char**)calloc(initial_capacity, sizeof(char*));
    path->path_length = 0;
    
    return path;
}

/**
 * Add package name to circular path
 */
void circular_path_add(CircularDependencyPath* path, const char* package_name) {
    if (!path || !package_name) return;
    
    path->package_names[path->path_length++] = strdup(package_name);
}

/**
 * Free circular dependency path
 */
void circular_path_free(CircularDependencyPath* path) {
    if (!path) return;
    
    if (path->package_names) {
        for (int i = 0; i < path->path_length; i++) {
            free(path->package_names[i]);
        }
        free(path->package_names);
    }
    
    free(path);
}

/**
 * Format circular path as string: "A -> B -> C -> A"
 */
char* circular_path_to_string(CircularDependencyPath* path) {
    if (!path || path->path_length == 0) return NULL;
    
    // Calculate required buffer size
    int total_len = 0;
    for (int i = 0; i < path->path_length; i++) {
        total_len += strlen(path->package_names[i]) + 4;  // " -> "
    }
    total_len += 1;  // null terminator
    
    char* result = (char*)malloc(total_len);
    if (!result) return NULL;
    
    result[0] = '\0';
    for (int i = 0; i < path->path_length; i++) {
        strcat(result, path->package_names[i]);
        if (i < path->path_length - 1) {
            strcat(result, " -> ");
        }
    }
    
    return result;
}

// ============================================================================
// CYCLE DETECTION (DFS)
// ============================================================================

/**
 * Reset graph traversal state
 */
void dependency_graph_reset_state(DependencyGraph* graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = false;
        graph->nodes[i]->in_path = false;
    }
}

/**
 * Internal DFS for cycle detection
 * Returns true if cycle detected
 */
bool dependency_graph_dfs_cycle(DependencyNode* node, CircularDependencyPath* path) {
    if (!node) return false;
    
    // If node is in current path, we found a cycle
    if (node->in_path) {
        // Add current node to complete the cycle
        circular_path_add(path, node->package->name);
        return true;
    }
    
    // If already visited (but not in path), no cycle from here
    if (node->visited) {
        return false;
    }
    
    // Mark as visited and in current path
    node->visited = true;
    node->in_path = true;
    
    // Add to path
    circular_path_add(path, node->package->name);
    
    // Check all dependencies
    for (int i = 0; i < node->dependency_count; i++) {
        if (dependency_graph_dfs_cycle(node->dependencies[i], path)) {
            return true;  // Cycle found
        }
    }
    
    // Remove from current path (backtrack)
    node->in_path = false;
    
    return false;
}

/**
 * Detect circular dependencies in the graph using DFS
 */
CircularDependencyPath* dependency_graph_detect_cycle(DependencyGraph* graph) {
    if (!graph) return NULL;
    
    dependency_graph_reset_state(graph);
    
    CircularDependencyPath* path = circular_path_create(16);
    if (!path) return NULL;
    
    // Check each node as potential starting point
    for (int i = 0; i < graph->node_count; i++) {
        if (!graph->nodes[i]->visited) {
            if (dependency_graph_dfs_cycle(graph->nodes[i], path)) {
                return path;  // Cycle found
            }
        }
        
        // Reset path for next try
        for (int j = 0; j < path->path_length; j++) {
            free(path->package_names[j]);
        }
        path->path_length = 0;
    }
    
    // No cycle found
    circular_path_free(path);
    return NULL;
}

// ============================================================================
// TOPOLOGICAL SORT
// ============================================================================

/**
 * DFS for topological sort
 */
static void topological_sort_dfs(DependencyNode* node, DependencyNode** result, int* index) {
    if (!node || node->visited) return;
    
    node->visited = true;
    
    // Visit all dependencies first
    for (int i = 0; i < node->dependency_count; i++) {
        topological_sort_dfs(node->dependencies[i], result, index);
    }
    
    // Add node to result (post-order)
    result[(*index)++] = node;
}

/**
 * Get topological sort order (dependency resolution order)
 */
DependencyNode** dependency_graph_topological_sort(DependencyGraph* graph, int* out_count) {
    if (!graph || !out_count) return NULL;
    
    dependency_graph_reset_state(graph);
    
    DependencyNode** result = (DependencyNode**)calloc(graph->node_count, sizeof(DependencyNode*));
    if (!result) return NULL;
    
    int index = 0;
    
    // Start from root node
    if (graph->root) {
        topological_sort_dfs(graph->root, result, &index);
    }
    
    // Visit any remaining unvisited nodes
    for (int i = 0; i < graph->node_count; i++) {
        if (!graph->nodes[i]->visited) {
            topological_sort_dfs(graph->nodes[i], result, &index);
        }
    }
    
    *out_count = index;
    return result;
}

// ============================================================================
// DEPTH CALCULATION
// ============================================================================

/**
 * Calculate dependency depth recursively
 */
static void calculate_depth_recursive(DependencyNode* node, int depth) {
    if (!node) return;
    
    node->depth = depth;
    
    for (int i = 0; i < node->dependency_count; i++) {
        calculate_depth_recursive(node->dependencies[i], depth + 1);
    }
}

/**
 * Calculate dependency depth (distance from root)
 */
void dependency_graph_calculate_depths(DependencyGraph* graph) {
    if (!graph || !graph->root) return;
    
    calculate_depth_recursive(graph->root, 0);
}

// ============================================================================
// DEBUG/VISUALIZATION
// ============================================================================

/**
 * Print a single node and its dependencies
 */
void dependency_node_print(DependencyNode* node, int indent) {
    if (!node || !node->package) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    
    printf("- %s (v%s)", node->package->name, node->package->version_str);
    
    if (node->dependency_count > 0) {
        printf(" [%d deps]", node->dependency_count);
    }
    
    printf("\n");
    
    for (int i = 0; i < node->dependency_count; i++) {
        dependency_node_print(node->dependencies[i], indent + 1);
    }
}

/**
 * Print dependency graph for debugging
 */
void dependency_graph_print(DependencyGraph* graph) {
    if (!graph) {
        printf("(null graph)\n");
        return;
    }
    
    printf("=== Dependency Graph ===\n");
    printf("Nodes: %d\n", graph->node_count);
    
    if (graph->root) {
        printf("\nRoot: %s\n", graph->root->package->name);
        dependency_node_print(graph->root, 0);
    } else {
        printf("\n(No root node)\n");
    }
    
    printf("========================\n");
}
