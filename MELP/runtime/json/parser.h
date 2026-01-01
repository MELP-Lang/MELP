/**
 * JSON Parser/Serializer for MELP Standard Library
 * 
 * General-purpose JSON parser for module metadata and data interchange.
 * Supports: Object, Array, String, Number, Boolean, Null
 * 
 * DESIGN PRINCIPLE: Stateless, functional pattern
 * Each parse/stringify call is independent (born → used → died)
 */

#ifndef MLP_JSON_PARSER_H
#define MLP_JSON_PARSER_H

#include <stddef.h>

// ============================================
// JSON VALUE TYPES
// ============================================

typedef enum {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

// Forward declarations
typedef struct JsonValue JsonValue;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;

// ============================================
// JSON VALUE STRUCTURE
// ============================================

struct JsonValue {
    JsonType type;
    union {
        int boolean;           // 0 = false, 1 = true
        double number;
        char* string;
        JsonArray* array;
        JsonObject* object;
    } value;
};

// ============================================
// JSON OBJECT (Key-Value Pairs)
// ============================================

typedef struct {
    char* key;
    JsonValue* value;
} JsonObjectEntry;

struct JsonObject {
    JsonObjectEntry* entries;
    size_t count;
    size_t capacity;
};

// ============================================
// JSON ARRAY (Indexed Values)
// ============================================

struct JsonArray {
    JsonValue** values;
    size_t count;
    size_t capacity;
};

// ============================================
// ERROR HANDLING
// ============================================

typedef struct {
    int has_error;
    int line;
    int column;
    char message[256];
} JsonError;

// ============================================
// CORE PARSING API
// ============================================

/**
 * Parse JSON string into JsonValue tree
 * Returns NULL on error (check error struct for details)
 * 
 * Pattern: born → parse → return tree → died (no persistent state)
 */
JsonValue* json_parse(const char* json_string, JsonError* error);

/**
 * Free JsonValue tree (recursive)
 */
void json_free(JsonValue* value);

// ============================================
// VALUE CREATION (For building JSON programmatically)
// ============================================

JsonValue* json_create_null(void);
JsonValue* json_create_boolean(int value);
JsonValue* json_create_number(double value);
JsonValue* json_create_string(const char* value);
JsonValue* json_create_array(void);
JsonValue* json_create_object(void);

// ============================================
// OBJECT OPERATIONS
// ============================================

/**
 * Add key-value pair to object
 * Takes ownership of value (will be freed with object)
 */
void json_object_set(JsonObject* obj, const char* key, JsonValue* value);

/**
 * Get value by key (returns NULL if not found)
 * Returned value is still owned by object
 */
JsonValue* json_object_get(JsonObject* obj, const char* key);

/**
 * Check if key exists
 */
int json_object_has(JsonObject* obj, const char* key);

/**
 * Get object size (number of keys)
 */
size_t json_object_size(JsonObject* obj);

// ============================================
// ARRAY OPERATIONS
// ============================================

/**
 * Append value to array
 * Takes ownership of value
 */
void json_array_append(JsonArray* arr, JsonValue* value);

/**
 * Get value at index (returns NULL if out of bounds)
 * Returned value is still owned by array
 */
JsonValue* json_array_get(JsonArray* arr, size_t index);

/**
 * Get array length
 */
size_t json_array_length(JsonArray* arr);

// ============================================
// TYPE CHECKING & EXTRACTION
// ============================================

int json_is_null(JsonValue* value);
int json_is_boolean(JsonValue* value);
int json_is_number(JsonValue* value);
int json_is_string(JsonValue* value);
int json_is_array(JsonValue* value);
int json_is_object(JsonValue* value);

// Safe getters (return default if wrong type)
int json_get_boolean(JsonValue* value, int default_value);
double json_get_number(JsonValue* value, double default_value);
const char* json_get_string(JsonValue* value, const char* default_value);
JsonArray* json_get_array(JsonValue* value);
JsonObject* json_get_object(JsonValue* value);

// ============================================
// SERIALIZATION (Stringify)
// ============================================

/**
 * Convert JsonValue to JSON string
 * Returns malloc'd string (caller must free)
 * 
 * Args:
 *   value: JSON value tree
 *   pretty: 1 for pretty-print with indentation, 0 for compact
 */
char* json_stringify(JsonValue* value, int pretty);

// ============================================
// UTILITY FUNCTIONS
// ============================================

/**
 * Print JSON value (for debugging)
 */
void json_print(JsonValue* value);

/**
 * Clone JSON value (deep copy)
 */
JsonValue* json_clone(JsonValue* value);

#endif // MLP_JSON_PARSER_H
