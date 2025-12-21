/**
 * MLP Standard Library - Map Extensions Header
 * 
 * Extended map operations for YZ_212 Collections Library
 * Adds: keys, values, clear operations
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#ifndef MLP_MAP_EXT_H
#define MLP_MAP_EXT_H

#include "mlp_map.h"
#include "mlp_list.h"
#include <stddef.h>

// -----------------------------------------------------------------------------
// Extended Map Operations
// -----------------------------------------------------------------------------

/**
 * Get all keys from map as a list
 * @param map Source map
 * @return New list containing all keys (string pointers), or NULL on failure
 * 
 * NOTE: Returned list contains pointers to keys (owned by map)
 * Do not free the strings, only free the list itself
 * List element_size = sizeof(char*)
 */
MelpList* melp_map_keys(MelpMap* map);

/**
 * Get all values from map as a list
 * @param map Source map
 * @return New list containing copies of all values, or NULL on failure
 * 
 * NOTE: Returned list contains copies of values (owned by list)
 * Free the list normally (melp_list_free)
 * List element_size = map->value_size
 */
MelpList* melp_map_values(MelpMap* map);

/**
 * Clear all key-value pairs from map
 * @param map Map to clear
 * @return 0 on success, -1 on failure
 * 
 * NOTE: Frees all keys, values, and nodes, but keeps the map structure
 * Map length becomes 0, buckets are reset
 */
int melp_map_clear(MelpMap* map);

#endif // MLP_MAP_EXT_H
