/**
 * MLP Standard Library - List Extensions Implementation
 * 
 * Extended list operations for YZ_212 Collections Library
 * Adds: insert, find, sort operations
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "mlp_list_ext.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Extended List Operations
// -----------------------------------------------------------------------------

int melp_list_insert(MelpList* list, size_t index, void* element) {
    if (!list || !element) {
        return -1;
    }
    
    // Allow insertion at end (index == length)
    if (index > list->length) {
        return -1;  // Out of bounds
    }
    
    // Check if we need to grow the array
    if (list->length >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        void** new_elements = (void**)realloc(list->elements, sizeof(void*) * new_capacity);
        if (!new_elements) {
            return -1;  // Allocation failed
        }
        list->elements = new_elements;
        list->capacity = new_capacity;
    }
    
    // Allocate memory for new element
    void* new_element = malloc(list->element_size);
    if (!new_element) {
        return -1;
    }
    memcpy(new_element, element, list->element_size);
    
    // Shift elements right from index to end
    if (index < list->length) {
        memmove(&list->elements[index + 1], 
                &list->elements[index], 
                sizeof(void*) * (list->length - index));
    }
    
    // Insert new element
    list->elements[index] = new_element;
    list->length++;
    
    return 0;
}

int64_t melp_list_find(MelpList* list, const void* element) {
    if (!list || !element) {
        return -1;
    }
    
    // Linear search through list
    for (size_t i = 0; i < list->length; i++) {
        if (list->elements[i] && 
            memcmp(list->elements[i], element, list->element_size) == 0) {
            return (int64_t)i;
        }
    }
    
    return -1;  // Not found
}

int melp_list_sort(MelpList* list) {
    if (!list) {
        return -1;
    }
    
    // Only support numeric lists (int64_t) for default sort
    if (list->element_size != sizeof(int64_t)) {
        fprintf(stderr, "melp_list_sort: Only supports numeric (int64_t) lists\n");
        return -1;
    }
    
    // Empty or single-element lists are already sorted
    if (list->length <= 1) {
        return 0;
    }
    
    // Use standard library qsort with our comparison function
    return melp_list_sort_with(list, melp_list_compare_int64);
}

int melp_list_sort_with(MelpList* list, int (*compare)(const void*, const void*)) {
    if (!list || !compare) {
        return -1;
    }
    
    // Empty or single-element lists are already sorted
    if (list->length <= 1) {
        return 0;
    }
    
    // Create temporary array of element pointers for sorting
    // We need to dereference the void** to get the actual elements
    void** temp = (void**)malloc(sizeof(void*) * list->length);
    if (!temp) {
        return -1;
    }
    
    // Copy pointers
    memcpy(temp, list->elements, sizeof(void*) * list->length);
    
    // Sort using qsort (sorts the pointers, comparison function dereferences them)
    qsort(temp, list->length, sizeof(void*), compare);
    
    // Copy sorted pointers back
    memcpy(list->elements, temp, sizeof(void*) * list->length);
    
    free(temp);
    return 0;
}

// -----------------------------------------------------------------------------
// Internal Helper Functions
// -----------------------------------------------------------------------------

int melp_list_compare_int64(const void* a, const void* b) {
    // a and b are pointers to void* (the list elements)
    // Dereference twice to get actual int64_t values
    int64_t val_a = **(int64_t**)a;
    int64_t val_b = **(int64_t**)b;
    
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}
