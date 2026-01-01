/**
 * JSON Parser/Serializer Implementation
 * Recursive descent parser for JSON
 */

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// ============================================
// PARSER STATE
// ============================================

typedef struct {
    const char* start;
    const char* current;
    int line;
    int column;
    JsonError* error;
} Parser;

// ============================================
// HELPER FUNCTIONS
// ============================================

static void set_error(Parser* p, const char* message) {
    if (p->error) {
        p->error->has_error = 1;
        p->error->line = p->line;
        p->error->column = p->column;
        snprintf(p->error->message, sizeof(p->error->message), "%s", message);
    }
}

static char peek(Parser* p) {
    return *p->current;
}

static char advance(Parser* p) {
    char c = *p->current++;
    if (c == '\n') {
        p->line++;
        p->column = 1;
    } else {
        p->column++;
    }
    return c;
}

static void skip_whitespace(Parser* p) {
    while (1) {
        char c = peek(p);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(p);
        } else {
            break;
        }
    }
}

static int match(Parser* p, char expected) {
    if (peek(p) != expected) return 0;
    advance(p);
    return 1;
}

// Forward declarations
static JsonValue* parse_value(Parser* p);

// ============================================
// VALUE CREATION
// ============================================

JsonValue* json_create_null(void) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (v) v->type = JSON_NULL;
    return v;
}

JsonValue* json_create_boolean(int value) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (v) {
        v->type = JSON_BOOLEAN;
        v->value.boolean = value ? 1 : 0;
    }
    return v;
}

JsonValue* json_create_number(double value) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (v) {
        v->type = JSON_NUMBER;
        v->value.number = value;
    }
    return v;
}

JsonValue* json_create_string(const char* value) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (v) {
        v->type = JSON_STRING;
        v->value.string = strdup(value);
    }
    return v;
}

JsonValue* json_create_array(void) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (!v) return NULL;
    
    v->type = JSON_ARRAY;
    v->value.array = malloc(sizeof(JsonArray));
    if (!v->value.array) {
        free(v);
        return NULL;
    }
    
    v->value.array->values = NULL;
    v->value.array->count = 0;
    v->value.array->capacity = 0;
    return v;
}

JsonValue* json_create_object(void) {
    JsonValue* v = malloc(sizeof(JsonValue));
    if (!v) return NULL;
    
    v->type = JSON_OBJECT;
    v->value.object = malloc(sizeof(JsonObject));
    if (!v->value.object) {
        free(v);
        return NULL;
    }
    
    v->value.object->entries = NULL;
    v->value.object->count = 0;
    v->value.object->capacity = 0;
    return v;
}

// ============================================
// MEMORY MANAGEMENT
// ============================================

void json_free(JsonValue* value) {
    if (!value) return;
    
    switch (value->type) {
        case JSON_STRING:
            free(value->value.string);
            break;
            
        case JSON_ARRAY:
            if (value->value.array) {
                for (size_t i = 0; i < value->value.array->count; i++) {
                    json_free(value->value.array->values[i]);
                }
                free(value->value.array->values);
                free(value->value.array);
            }
            break;
            
        case JSON_OBJECT:
            if (value->value.object) {
                for (size_t i = 0; i < value->value.object->count; i++) {
                    free(value->value.object->entries[i].key);
                    json_free(value->value.object->entries[i].value);
                }
                free(value->value.object->entries);
                free(value->value.object);
            }
            break;
            
        default:
            break;
    }
    
    free(value);
}

// ============================================
// PARSING - NULL, BOOLEAN
// ============================================

static JsonValue* parse_null(Parser* p) {
    if (match(p, 'n') && match(p, 'u') && match(p, 'l') && match(p, 'l')) {
        return json_create_null();
    }
    set_error(p, "Invalid null value");
    return NULL;
}

static JsonValue* parse_boolean(Parser* p) {
    if (match(p, 't') && match(p, 'r') && match(p, 'u') && match(p, 'e')) {
        return json_create_boolean(1);
    }
    if (match(p, 'f') && match(p, 'a') && match(p, 'l') && match(p, 's') && match(p, 'e')) {
        return json_create_boolean(0);
    }
    set_error(p, "Invalid boolean value");
    return NULL;
}

// ============================================
// PARSING - NUMBER
// ============================================

static JsonValue* parse_number(Parser* p) {
    const char* start = p->current;
    
    // Optional minus
    if (peek(p) == '-') advance(p);
    
    // Integer part
    if (!isdigit(peek(p))) {
        set_error(p, "Invalid number");
        return NULL;
    }
    
    while (isdigit(peek(p))) advance(p);
    
    // Fractional part
    if (peek(p) == '.') {
        advance(p);
        if (!isdigit(peek(p))) {
            set_error(p, "Invalid number: expected digit after '.'");
            return NULL;
        }
        while (isdigit(peek(p))) advance(p);
    }
    
    // Exponent part
    if (peek(p) == 'e' || peek(p) == 'E') {
        advance(p);
        if (peek(p) == '+' || peek(p) == '-') advance(p);
        if (!isdigit(peek(p))) {
            set_error(p, "Invalid number: expected digit in exponent");
            return NULL;
        }
        while (isdigit(peek(p))) advance(p);
    }
    
    // Convert to double
    char buffer[256];
    size_t len = p->current - start;
    if (len >= sizeof(buffer)) {
        set_error(p, "Number too long");
        return NULL;
    }
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    
    return json_create_number(atof(buffer));
}

// ============================================
// PARSING - STRING
// ============================================

static JsonValue* parse_string(Parser* p) {
    if (!match(p, '"')) {
        set_error(p, "Expected '\"'");
        return NULL;
    }
    
    // Dynamic buffer for long strings
    size_t capacity = 256;
    size_t len = 0;
    char* buffer = malloc(capacity);
    if (!buffer) {
        set_error(p, "Memory allocation failed");
        return NULL;
    }
    
    while (peek(p) != '"' && peek(p) != '\0') {
        // Grow buffer if needed
        if (len >= capacity - 1) {
            capacity *= 2;
            char* new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                set_error(p, "Memory allocation failed");
                return NULL;
            }
            buffer = new_buffer;
        }
        
        if (peek(p) == '\\') {
            advance(p);
            char c = advance(p);
            switch (c) {
                case '"': buffer[len++] = '"'; break;
                case '\\': buffer[len++] = '\\'; break;
                case '/': buffer[len++] = '/'; break;
                case 'b': buffer[len++] = '\b'; break;
                case 'f': buffer[len++] = '\f'; break;
                case 'n': buffer[len++] = '\n'; break;
                case 'r': buffer[len++] = '\r'; break;
                case 't': buffer[len++] = '\t'; break;
                default:
                    free(buffer);
                    set_error(p, "Invalid escape sequence");
                    return NULL;
            }
        } else {
            buffer[len++] = advance(p);
        }
    }
    
    if (!match(p, '"')) {
        free(buffer);
        set_error(p, "Unterminated string");
        return NULL;
    }
    
    buffer[len] = '\0';
    JsonValue* result = json_create_string(buffer);
    free(buffer);
    return result;
}

// ============================================
// PARSING - ARRAY
// ============================================

static JsonValue* parse_array(Parser* p) {
    if (!match(p, '[')) {
        set_error(p, "Expected '['");
        return NULL;
    }
    
    JsonValue* array = json_create_array();
    if (!array) {
        set_error(p, "Memory allocation failed");
        return NULL;
    }
    
    skip_whitespace(p);
    
    // Empty array
    if (peek(p) == ']') {
        advance(p);
        return array;
    }
    
    // Parse elements
    while (1) {
        skip_whitespace(p);
        JsonValue* element = parse_value(p);
        if (!element) {
            json_free(array);
            return NULL;
        }
        
        json_array_append(array->value.array, element);
        
        skip_whitespace(p);
        if (peek(p) == ']') {
            advance(p);
            break;
        }
        
        if (!match(p, ',')) {
            set_error(p, "Expected ',' or ']'");
            json_free(array);
            return NULL;
        }
    }
    
    return array;
}

// ============================================
// PARSING - OBJECT
// ============================================

static JsonValue* parse_object(Parser* p) {
    if (!match(p, '{')) {
        set_error(p, "Expected '{'");
        return NULL;
    }
    
    JsonValue* object = json_create_object();
    if (!object) {
        set_error(p, "Memory allocation failed");
        return NULL;
    }
    
    skip_whitespace(p);
    
    // Empty object
    if (peek(p) == '}') {
        advance(p);
        return object;
    }
    
    // Parse key-value pairs
    while (1) {
        skip_whitespace(p);
        
        // Parse key (must be string)
        JsonValue* key_val = parse_string(p);
        if (!key_val) {
            json_free(object);
            return NULL;
        }
        
        char* key = key_val->value.string;
        key_val->value.string = NULL; // Take ownership
        json_free(key_val);
        
        skip_whitespace(p);
        if (!match(p, ':')) {
            free(key);
            json_free(object);
            set_error(p, "Expected ':'");
            return NULL;
        }
        
        skip_whitespace(p);
        JsonValue* value = parse_value(p);
        if (!value) {
            free(key);
            json_free(object);
            return NULL;
        }
        
        json_object_set(object->value.object, key, value);
        free(key);
        
        skip_whitespace(p);
        if (peek(p) == '}') {
            advance(p);
            break;
        }
        
        if (!match(p, ',')) {
            set_error(p, "Expected ',' or '}'");
            json_free(object);
            return NULL;
        }
    }
    
    return object;
}

// ============================================
// PARSING - VALUE (Dispatcher)
// ============================================

static JsonValue* parse_value(Parser* p) {
    skip_whitespace(p);
    
    char c = peek(p);
    
    if (c == 'n') return parse_null(p);
    if (c == 't' || c == 'f') return parse_boolean(p);
    if (c == '"') return parse_string(p);
    if (c == '[') return parse_array(p);
    if (c == '{') return parse_object(p);
    if (c == '-' || isdigit(c)) return parse_number(p);
    
    set_error(p, "Unexpected character");
    return NULL;
}

// ============================================
// PUBLIC PARSING API
// ============================================

JsonValue* json_parse(const char* json_string, JsonError* error) {
    Parser p = {
        .start = json_string,
        .current = json_string,
        .line = 1,
        .column = 1,
        .error = error
    };
    
    if (error) {
        error->has_error = 0;
    }
    
    JsonValue* value = parse_value(&p);
    
    if (value) {
        skip_whitespace(&p);
        if (peek(&p) != '\0') {
            set_error(&p, "Unexpected content after JSON value");
            json_free(value);
            return NULL;
        }
    }
    
    return value;
}

// ============================================
// OBJECT OPERATIONS
// ============================================

void json_object_set(JsonObject* obj, const char* key, JsonValue* value) {
    if (!obj || !key || !value) return;
    
    // Check if key already exists
    for (size_t i = 0; i < obj->count; i++) {
        if (strcmp(obj->entries[i].key, key) == 0) {
            json_free(obj->entries[i].value);
            obj->entries[i].value = value;
            return;
        }
    }
    
    // Add new entry
    if (obj->count >= obj->capacity) {
        size_t new_capacity = obj->capacity == 0 ? 8 : obj->capacity * 2;
        JsonObjectEntry* new_entries = realloc(obj->entries, 
            new_capacity * sizeof(JsonObjectEntry));
        if (!new_entries) return;
        
        obj->entries = new_entries;
        obj->capacity = new_capacity;
    }
    
    obj->entries[obj->count].key = strdup(key);
    obj->entries[obj->count].value = value;
    obj->count++;
}

JsonValue* json_object_get(JsonObject* obj, const char* key) {
    if (!obj || !key) return NULL;
    
    for (size_t i = 0; i < obj->count; i++) {
        if (strcmp(obj->entries[i].key, key) == 0) {
            return obj->entries[i].value;
        }
    }
    
    return NULL;
}

int json_object_has(JsonObject* obj, const char* key) {
    return json_object_get(obj, key) != NULL;
}

size_t json_object_size(JsonObject* obj) {
    return obj ? obj->count : 0;
}

// ============================================
// ARRAY OPERATIONS
// ============================================

void json_array_append(JsonArray* arr, JsonValue* value) {
    if (!arr || !value) return;
    
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity == 0 ? 8 : arr->capacity * 2;
        JsonValue** new_values = realloc(arr->values, 
            new_capacity * sizeof(JsonValue*));
        if (!new_values) return;
        
        arr->values = new_values;
        arr->capacity = new_capacity;
    }
    
    arr->values[arr->count++] = value;
}

JsonValue* json_array_get(JsonArray* arr, size_t index) {
    if (!arr || index >= arr->count) return NULL;
    return arr->values[index];
}

size_t json_array_length(JsonArray* arr) {
    return arr ? arr->count : 0;
}

// ============================================
// TYPE CHECKING
// ============================================

int json_is_null(JsonValue* value) { return value && value->type == JSON_NULL; }
int json_is_boolean(JsonValue* value) { return value && value->type == JSON_BOOLEAN; }
int json_is_number(JsonValue* value) { return value && value->type == JSON_NUMBER; }
int json_is_string(JsonValue* value) { return value && value->type == JSON_STRING; }
int json_is_array(JsonValue* value) { return value && value->type == JSON_ARRAY; }
int json_is_object(JsonValue* value) { return value && value->type == JSON_OBJECT; }

int json_get_boolean(JsonValue* value, int default_value) {
    return json_is_boolean(value) ? value->value.boolean : default_value;
}

double json_get_number(JsonValue* value, double default_value) {
    return json_is_number(value) ? value->value.number : default_value;
}

const char* json_get_string(JsonValue* value, const char* default_value) {
    return json_is_string(value) ? value->value.string : default_value;
}

JsonArray* json_get_array(JsonValue* value) {
    return json_is_array(value) ? value->value.array : NULL;
}

JsonObject* json_get_object(JsonValue* value) {
    return json_is_object(value) ? value->value.object : NULL;
}

// ============================================
// NOTE: Serialization functions moved to stringify.c
// ============================================
