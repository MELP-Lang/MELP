#include "exception_types.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global exception type registry
#define MAX_EXCEPTION_TYPES 100
static ExceptionTypeInfo* exception_registry[MAX_EXCEPTION_TYPES];
static int exception_count = 0;

// ============================================================================
// Exception Type Registry Management
// ============================================================================

void exception_types_init(void) {
    exception_count = 0;
    
    // Register built-in exception types
    register_exception_type(EXCEPTION_TYPE_BASE, EXCEPTION_BASE, NULL);
    register_exception_type(EXCEPTION_TYPE_DIVISION_BY_ZERO, 
                          EXCEPTION_DIVISION_BY_ZERO, 
                          EXCEPTION_TYPE_BASE);
    register_exception_type(EXCEPTION_TYPE_NULL_POINTER, 
                          EXCEPTION_NULL_POINTER, 
                          EXCEPTION_TYPE_BASE);
    register_exception_type(EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS, 
                          EXCEPTION_INDEX_OUT_OF_BOUNDS, 
                          EXCEPTION_TYPE_BASE);
    register_exception_type(EXCEPTION_TYPE_TYPE_MISMATCH, 
                          EXCEPTION_TYPE_MISMATCH, 
                          EXCEPTION_TYPE_BASE);
    register_exception_type(EXCEPTION_TYPE_FILE_NOT_FOUND, 
                          EXCEPTION_FILE_NOT_FOUND, 
                          EXCEPTION_TYPE_BASE);
}

void exception_types_cleanup(void) {
    for (int i = 0; i < exception_count; i++) {
        if (exception_registry[i]) {
            free(exception_registry[i]->name);
            free(exception_registry[i]);
        }
    }
    exception_count = 0;
}

void register_exception_type(const char* name, ExceptionType type, const char* parent_name) {
    if (exception_count >= MAX_EXCEPTION_TYPES) {
        fprintf(stderr, "Error: Exception type registry full\n");
        return;
    }
    
    ExceptionTypeInfo* info = (ExceptionTypeInfo*)malloc(sizeof(ExceptionTypeInfo));
    if (!info) {
        fprintf(stderr, "Error: Failed to allocate ExceptionTypeInfo\n");
        return;
    }
    
    info->name = strdup(name);
    info->type = type;
    info->parent = NULL;
    
    // Find parent
    if (parent_name) {
        for (int i = 0; i < exception_count; i++) {
            if (strcmp(exception_registry[i]->name, parent_name) == 0) {
                info->parent = exception_registry[i];
                break;
            }
        }
    }
    
    exception_registry[exception_count++] = info;
}

// ============================================================================
// Exception Type Query Functions
// ============================================================================

ExceptionType get_exception_type_by_name(const char* name) {
    if (!name) return EXCEPTION_BASE;
    
    for (int i = 0; i < exception_count; i++) {
        if (strcmp(exception_registry[i]->name, name) == 0) {
            return exception_registry[i]->type;
        }
    }
    
    // Default: treat as custom exception
    return EXCEPTION_CUSTOM;
}

const char* get_exception_name_by_type(ExceptionType type) {
    for (int i = 0; i < exception_count; i++) {
        if (exception_registry[i]->type == type) {
            return exception_registry[i]->name;
        }
    }
    return "UnknownException";
}

int is_subtype_of(const char* child_type, const char* parent_type) {
    if (!child_type || !parent_type) return 0;
    
    // Same type: compatible
    if (strcmp(child_type, parent_type) == 0) return 1;
    
    // Find child in registry
    ExceptionTypeInfo* child_info = NULL;
    for (int i = 0; i < exception_count; i++) {
        if (strcmp(exception_registry[i]->name, child_type) == 0) {
            child_info = exception_registry[i];
            break;
        }
    }
    
    if (!child_info) return 0;
    
    // Walk up parent chain
    ExceptionTypeInfo* current = child_info->parent;
    while (current) {
        if (strcmp(current->name, parent_type) == 0) {
            return 1;  // Found parent
        }
        current = current->parent;
    }
    
    return 0;  // Not a subtype
}

int is_exception_type_compatible(ExceptionType thrown, ExceptionType caught) {
    // Exact match
    if (thrown == caught) return 1;
    
    // Exception (base) catches all
    if (caught == EXCEPTION_BASE) return 1;
    
    // Check inheritance via names
    const char* thrown_name = get_exception_name_by_type(thrown);
    const char* caught_name = get_exception_name_by_type(caught);
    
    return is_subtype_of(thrown_name, caught_name);
}
