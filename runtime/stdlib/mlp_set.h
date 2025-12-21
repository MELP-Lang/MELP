/**
 * MLP Standard Library - Set (Unique Collection) Header
 * 
 * Set implementation using list wrapper with uniqueness guarantee
 * YZ_212 Phase 2: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#ifndef MLP_SET_H
#define MLP_SET_H

#include "mlp_list.h"
#include <stddef.h>
#include <stdint.h>

/**
 * MelpSet - Unique elements collection (wrapper around MelpList)
 * 
 * Design Philosophy:
 * - Wraps MelpList for storage
 * - Guarantees uniqueness (no duplicates)
 * - Uses memcmp for equality checking
 * - O(n) insertion/search (good for small sets, can optimize later)
 * - STO-compatible (generic void* elements)
 */
typedef struct {
    MelpList* internal;   // Underlying list storage
} MelpSet;

// -----------------------------------------------------------------------------
// Core Set Operations
// -----------------------------------------------------------------------------

/**
 * Create a new set with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to newly allocated set, or NULL on failure
 */
MelpSet* melp_set_create(size_t element_size);

/**
 * Free a set and all its allocated memory
 * @param set Set to free
 */
void melp_set_free(MelpSet* set);

/**
 * Add element to set (duplicate check, no-op if exists)
 * @param set Target set
 * @param element Pointer to element data to add
 * @return 1 if added (new), 0 if already exists, -1 on error
 */
int melp_set_add(MelpSet* set, const void* element);

/**
 * Check if element exists in set
 * @param set Target set
 * @param element Pointer to element to find
 * @return 1 if exists, 0 if not found
 */
int melp_set_contains(MelpSet* set, const void* element);

/**
 * Remove element from set
 * @param set Target set
 * @param element Pointer to element to remove
 * @return 1 if removed, 0 if not found, -1 on error
 */
int melp_set_remove(MelpSet* set, const void* element);

/**
 * Get the number of elements in set
 * @param set Target set
 * @return Number of unique elements
 */
size_t melp_set_size(MelpSet* set);

/**
 * Check if set is empty
 * @param set Target set
 * @return 1 if empty, 0 otherwise
 */
int melp_set_is_empty(MelpSet* set);

/**
 * Clear all elements from set
 * @param set Set to clear
 */
void melp_set_clear(MelpSet* set);

// -----------------------------------------------------------------------------
// Set Theory Operations
// -----------------------------------------------------------------------------

/**
 * Union of two sets (set1 ∪ set2)
 * @param set1 First set
 * @param set2 Second set
 * @return New set containing all unique elements from both sets, or NULL on failure
 */
MelpSet* melp_set_union(MelpSet* set1, MelpSet* set2);

/**
 * Intersection of two sets (set1 ∩ set2)
 * @param set1 First set
 * @param set2 Second set
 * @return New set containing only elements in both sets, or NULL on failure
 */
MelpSet* melp_set_intersection(MelpSet* set1, MelpSet* set2);

/**
 * Difference of two sets (set1 - set2)
 * @param set1 First set
 * @param set2 Second set
 * @return New set containing elements in set1 but not in set2, or NULL on failure
 */
MelpSet* melp_set_difference(MelpSet* set1, MelpSet* set2);

// -----------------------------------------------------------------------------
// Utility Operations
// -----------------------------------------------------------------------------

/**
 * Get internal list (for iteration, read-only!)
 * @param set Target set
 * @return Pointer to internal list (do not modify!)
 */
MelpList* melp_set_get_list(MelpSet* set);

/**
 * Create set from list (removes duplicates)
 * @param list Source list
 * @return New set with unique elements from list, or NULL on failure
 */
MelpSet* melp_set_from_list(MelpList* list);

#endif // MLP_SET_H
