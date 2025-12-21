/**
 * MLP Standard Library - Map Extensions Implementation
 * 
 * Extended map operations for YZ_212 Collections Library
 * Adds: keys, values, clear operations
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "mlp_map_ext.h"
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Extended Map Operations
// -----------------------------------------------------------------------------

MelpList* melp_map_keys(MelpMap* map) {
    if (!map) {
        return NULL;
    }
    
    // Create list for keys (string pointers)
    MelpList* keys = melp_list_create(sizeof(char*));
    if (!keys) {
        return NULL;
    }
    
    // Reserve capacity for efficiency
    if (melp_list_reserve(keys, map->length) != 0) {
        melp_list_free(keys);
        return NULL;
    }
    
    // Iterate through all buckets
    for (size_t i = 0; i < map->capacity; i++) {
        MelpMapNode* node = map->buckets[i];
        
        // Walk the chain in this bucket
        while (node) {
            // Append key pointer to list (not copying the string)
            if (melp_list_append(keys, &node->key) != 0) {
                melp_list_free(keys);
                return NULL;
            }
            node = node->next;
        }
    }
    
    return keys;
}

MelpList* melp_map_values(MelpMap* map) {
    if (!map) {
        return NULL;
    }
    
    // Create list for values (copies)
    MelpList* values = melp_list_create(map->value_size);
    if (!values) {
        return NULL;
    }
    
    // Reserve capacity for efficiency
    if (melp_list_reserve(values, map->length) != 0) {
        melp_list_free(values);
        return NULL;
    }
    
    // Iterate through all buckets
    for (size_t i = 0; i < map->capacity; i++) {
        MelpMapNode* node = map->buckets[i];
        
        // Walk the chain in this bucket
        while (node) {
            // Append value copy to list
            if (melp_list_append(values, node->value) != 0) {
                melp_list_free(values);
                return NULL;
            }
            node = node->next;
        }
    }
    
    return values;
}

int melp_map_clear(MelpMap* map) {
    if (!map) {
        return -1;
    }
    
    // Free all nodes in all buckets
    for (size_t i = 0; i < map->capacity; i++) {
        MelpMapNode* node = map->buckets[i];
        
        while (node) {
            MelpMapNode* next = node->next;
            
            // Free key, value, and node itself
            free(node->key);
            free(node->value);
            free(node);
            
            node = next;
        }
        
        // Reset bucket head to NULL
        map->buckets[i] = NULL;
    }
    
    // Reset length
    map->length = 0;
    
    return 0;
}
