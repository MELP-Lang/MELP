/**
 * MLP Standard Library - Set (Unique Collection) Implementation
 * 
 * Set implementation using list wrapper with uniqueness guarantee
 * YZ_212 Phase 2: Collections Library
 * 
 * Created: 22 Aralık 2025 (YZ_212)
 */

#include "mlp_set.h"
#include "mlp_list_ext.h"
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Core Set Operations
// -----------------------------------------------------------------------------

MelpSet* melp_set_create(size_t element_size) {
    if (element_size == 0) {
        return NULL;
    }
    
    MelpSet* set = (MelpSet*)malloc(sizeof(MelpSet));
    if (!set) {
        return NULL;
    }
    
    set->internal = melp_list_create(element_size);
    if (!set->internal) {
        free(set);
        return NULL;
    }
    
    return set;
}

void melp_set_free(MelpSet* set) {
    if (!set) {
        return;
    }
    
    if (set->internal) {
        melp_list_free(set->internal);
    }
    
    free(set);
}

int melp_set_add(MelpSet* set, const void* element) {
    if (!set || !element) {
        return -1;
    }
    
    // Check if element already exists (duplicate check)
    if (melp_list_find(set->internal, element) >= 0) {
        return 0;  // Already exists, no-op
    }
    
    // Add to internal list
    if (melp_list_append(set->internal, (void*)element) != 0) {
        return -1;  // Error
    }
    
    return 1;  // Successfully added
}

int melp_set_contains(MelpSet* set, const void* element) {
    if (!set || !element) {
        return 0;
    }
    
    return melp_list_find(set->internal, element) >= 0 ? 1 : 0;
}

int melp_set_remove(MelpSet* set, const void* element) {
    if (!set || !element) {
        return -1;
    }
    
    int64_t index = melp_list_find(set->internal, element);
    if (index < 0) {
        return 0;  // Not found
    }
    
    if (melp_list_remove(set->internal, (size_t)index) != 0) {
        return -1;  // Error
    }
    
    return 1;  // Successfully removed
}

size_t melp_set_size(MelpSet* set) {
    if (!set) {
        return 0;
    }
    
    return melp_list_length(set->internal);
}

int melp_set_is_empty(MelpSet* set) {
    if (!set) {
        return 1;
    }
    
    return melp_list_is_empty(set->internal);
}

void melp_set_clear(MelpSet* set) {
    if (!set) {
        return;
    }
    
    melp_list_clear(set->internal);
}

// -----------------------------------------------------------------------------
// Set Theory Operations
// -----------------------------------------------------------------------------

MelpSet* melp_set_union(MelpSet* set1, MelpSet* set2) {
    if (!set1 || !set2) {
        return NULL;
    }
    
    // Check element sizes match
    if (set1->internal->element_size != set2->internal->element_size) {
        return NULL;
    }
    
    // Create new set
    MelpSet* result = melp_set_create(set1->internal->element_size);
    if (!result) {
        return NULL;
    }
    
    // Add all elements from set1
    for (size_t i = 0; i < melp_list_length(set1->internal); i++) {
        void* elem = melp_list_get(set1->internal, i);
        melp_set_add(result, elem);
    }
    
    // Add all elements from set2 (duplicates ignored)
    for (size_t i = 0; i < melp_list_length(set2->internal); i++) {
        void* elem = melp_list_get(set2->internal, i);
        melp_set_add(result, elem);
    }
    
    return result;
}

MelpSet* melp_set_intersection(MelpSet* set1, MelpSet* set2) {
    if (!set1 || !set2) {
        return NULL;
    }
    
    // Check element sizes match
    if (set1->internal->element_size != set2->internal->element_size) {
        return NULL;
    }
    
    // Create new set
    MelpSet* result = melp_set_create(set1->internal->element_size);
    if (!result) {
        return NULL;
    }
    
    // Add only elements that exist in both sets
    for (size_t i = 0; i < melp_list_length(set1->internal); i++) {
        void* elem = melp_list_get(set1->internal, i);
        if (melp_set_contains(set2, elem)) {
            melp_set_add(result, elem);
        }
    }
    
    return result;
}

MelpSet* melp_set_difference(MelpSet* set1, MelpSet* set2) {
    if (!set1 || !set2) {
        return NULL;
    }
    
    // Check element sizes match
    if (set1->internal->element_size != set2->internal->element_size) {
        return NULL;
    }
    
    // Create new set
    MelpSet* result = melp_set_create(set1->internal->element_size);
    if (!result) {
        return NULL;
    }
    
    // Add only elements from set1 that are NOT in set2
    for (size_t i = 0; i < melp_list_length(set1->internal); i++) {
        void* elem = melp_list_get(set1->internal, i);
        if (!melp_set_contains(set2, elem)) {
            melp_set_add(result, elem);
        }
    }
    
    return result;
}

// -----------------------------------------------------------------------------
// Utility Operations
// -----------------------------------------------------------------------------

MelpList* melp_set_get_list(MelpSet* set) {
    if (!set) {
        return NULL;
    }
    
    return set->internal;
}

MelpSet* melp_set_from_list(MelpList* list) {
    if (!list) {
        return NULL;
    }
    
    MelpSet* set = melp_set_create(list->element_size);
    if (!set) {
        return NULL;
    }
    
    // Add all elements (duplicates automatically ignored)
    for (size_t i = 0; i < melp_list_length(list); i++) {
        void* elem = melp_list_get(list, i);
        melp_set_add(set, elem);
    }
    
    return set;
}
