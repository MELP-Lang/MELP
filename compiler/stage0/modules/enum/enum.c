#include "enum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// YZ_96: Enum Implementation
// ============================================================================

// Global enum registry
static EnumDefinition* enum_registry = NULL;

// ============================================================================
// Enum Value Functions
// ============================================================================

EnumValue* enum_value_create(const char* name, int64_t value) {
    EnumValue* val = malloc(sizeof(EnumValue));
    if (!val) return NULL;
    
    val->name = strdup(name);
    val->value = value;
    val->next = NULL;
    
    return val;
}

void enum_value_free(EnumValue* val) {
    if (!val) return;
    
    if (val->name) free(val->name);
    free(val);
}

// ============================================================================
// Enum Definition Functions
// ============================================================================

EnumDefinition* enum_definition_create(const char* name) {
    EnumDefinition* def = malloc(sizeof(EnumDefinition));
    if (!def) return NULL;
    
    def->name = strdup(name);
    def->values = NULL;
    def->value_count = 0;
    def->next = NULL;
    
    return def;
}

void enum_definition_free(EnumDefinition* def) {
    if (!def) return;
    
    // Free all values
    EnumValue* val = def->values;
    while (val) {
        EnumValue* next = val->next;
        enum_value_free(val);
        val = next;
    }
    
    if (def->name) free(def->name);
    free(def);
}

void enum_definition_add_value(EnumDefinition* def, EnumValue* val) {
    if (!def || !val) return;
    
    // Add to end of list
    if (!def->values) {
        def->values = val;
    } else {
        EnumValue* current = def->values;
        while (current->next) {
            current = current->next;
        }
        current->next = val;
    }
    
    def->value_count++;
}

// ============================================================================
// Enum Registry Functions
// ============================================================================

void enum_register(EnumDefinition* def) {
    if (!def) return;
    
    // Add to front of registry
    def->next = enum_registry;
    enum_registry = def;
    
    // Debug
    // printf("[ENUM] Registered: %s with %d values\n", def->name, def->value_count);
}

EnumDefinition* enum_lookup(const char* name) {
    if (!name) return NULL;
    
    EnumDefinition* def = enum_registry;
    while (def) {
        if (strcmp(def->name, name) == 0) {
            return def;
        }
        def = def->next;
    }
    
    return NULL;
}

int64_t enum_lookup_value(const char* enum_name, const char* value_name) {
    EnumDefinition* def = enum_lookup(enum_name);
    if (!def) return -1;
    
    EnumValue* val = def->values;
    while (val) {
        if (strcmp(val->name, value_name) == 0) {
            return val->value;
        }
        val = val->next;
    }
    
    return -1;
}

int enum_is_type(const char* name) {
    return enum_lookup(name) != NULL ? 1 : 0;
}

void enum_registry_free(void) {
    EnumDefinition* def = enum_registry;
    while (def) {
        EnumDefinition* next = def->next;
        enum_definition_free(def);
        def = next;
    }
    enum_registry = NULL;
}
