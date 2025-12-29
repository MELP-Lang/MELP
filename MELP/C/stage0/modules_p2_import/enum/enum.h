#ifndef ENUM_H
#define ENUM_H

#include <stdint.h>

// ============================================================================
// YZ_96: Enum Type Support
// ============================================================================
// Syntax:
//   enum Status
//       PENDING = 0
//       ACTIVE = 1
//       DONE = 2
//   end_enum
//
//   Status s = Status.ACTIVE
//   switch s
//       case Status.PENDING: ...
//   end_switch

// Forward declaration
typedef struct EnumValue EnumValue;

// Enum value (member)
struct EnumValue {
    char* name;           // Value name (e.g., "PENDING")
    int64_t value;        // Numeric value (e.g., 0)
    EnumValue* next;      // Next value in list
};

// Enum definition
typedef struct EnumDefinition {
    char* name;                    // Enum name (e.g., "Status")
    EnumValue* values;             // Linked list of values
    int value_count;               // Number of values
    struct EnumDefinition* next;   // Next enum in registry
} EnumDefinition;

// ============================================================================
// Enum Value Functions
// ============================================================================

// Create enum value
EnumValue* enum_value_create(const char* name, int64_t value);

// Free enum value
void enum_value_free(EnumValue* val);

// ============================================================================
// Enum Definition Functions
// ============================================================================

// Create enum definition
EnumDefinition* enum_definition_create(const char* name);

// Free enum definition
void enum_definition_free(EnumDefinition* def);

// Add value to enum
void enum_definition_add_value(EnumDefinition* def, EnumValue* val);

// ============================================================================
// Enum Registry (Global)
// ============================================================================

// Register enum definition
void enum_register(EnumDefinition* def);

// Lookup enum by name
EnumDefinition* enum_lookup(const char* name);

// Lookup enum value by qualified name (e.g., "Status.ACTIVE")
// Returns numeric value, or -1 if not found
int64_t enum_lookup_value(const char* enum_name, const char* value_name);

// YZ_29: Lookup enum value by unqualified name (e.g., "T_FUNCTION")
// Searches all registered enums for value name
// Returns numeric value, or -1 if not found
int64_t enum_lookup_value_unqualified(const char* value_name);

// Check if name is an enum type
int enum_is_type(const char* name);

// Free all registered enums
void enum_registry_free(void);

// ============================================================================
// YZ_101: Enum Variable Declaration
// ============================================================================
// Represents: Color c = Color.Red

typedef struct EnumVariable {
    char* enum_type;        // Enum type name (e.g., "Color")
    char* var_name;         // Variable name (e.g., "c")
    int64_t init_value;     // Initial value (e.g., 0 for Color.Red)
    int has_initializer;    // 1 if initialized, 0 if not
} EnumVariable;

// Create enum variable
EnumVariable* enum_variable_create(const char* enum_type, const char* var_name, 
                                    int64_t init_value, int has_initializer);

// Free enum variable
void enum_variable_free(EnumVariable* var);

#endif // ENUM_H
