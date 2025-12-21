/**
 * MLP Standard Library - List Extensions Header
 * 
 * Extended list operations for YZ_212 Collections Library
 * Adds: insert, find, sort operations
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#ifndef MLP_LIST_EXT_H
#define MLP_LIST_EXT_H

#include "mlp_list.h"
#include <stddef.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
// Extended List Operations
// -----------------------------------------------------------------------------

/**
 * Insert element at specified index (shifts elements right)
 * @param list List to modify
 * @param index Zero-based index where to insert
 * @param element Pointer to element data to copy
 * @return 0 on success, -1 on failure (index out of bounds or allocation error)
 * 
 * Example: insert(list, 2, &value) → list[0..1] + value + list[2..end]
 */
int melp_list_insert(MelpList* list, size_t index, void* element);

/**
 * Find first occurrence of element in list
 * @param list List to search
 * @param element Pointer to element to find (uses memcmp)
 * @return Index of first occurrence, or -1 if not found
 * 
 * NOTE: Uses memcmp for comparison (element_size bytes)
 * For string lists, compare pointer values (not string content)
 */
int64_t melp_list_find(MelpList* list, const void* element);

/**
 * Sort list in ascending order (for numeric lists only)
 * @param list List to sort (must contain int64_t elements)
 * @return 0 on success, -1 on failure
 * 
 * NOTE: Uses quicksort algorithm (qsort from stdlib)
 * Only supports numeric lists (element_size == sizeof(int64_t))
 */
int melp_list_sort(MelpList* list);

/**
 * Sort list with custom comparison function
 * @param list List to sort
 * @param compare Comparison function (returns <0, 0, >0 like strcmp)
 * @return 0 on success, -1 on failure
 * 
 * Comparison function signature:
 * int compare(const void* a, const void* b)
 */
int melp_list_sort_with(MelpList* list, int (*compare)(const void*, const void*));

// -----------------------------------------------------------------------------
// Internal Helper Functions
// -----------------------------------------------------------------------------

/**
 * Default comparison function for int64_t elements
 * @param a Pointer to first element
 * @param b Pointer to second element
 * @return Negative if a < b, 0 if a == b, positive if a > b
 */
int melp_list_compare_int64(const void* a, const void* b);

#endif // MLP_LIST_EXT_H
