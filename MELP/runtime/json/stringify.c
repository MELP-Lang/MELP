/**
 * JSON Serializer (Stringify) Implementation
 * Converts JSON values to strings
 */

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
static void stringify_value(JsonValue* value, char** buffer, size_t* len, size_t* cap, int pretty, int indent);

// ============================================
// STRING BUFFER OPERATIONS
// ============================================

static void append_string(char** buffer, size_t* len, size_t* cap, const char* str) {
    size_t str_len = strlen(str);
    while (*len + str_len >= *cap) {
        *cap = *cap == 0 ? 256 : *cap * 2;
        *buffer = realloc(*buffer, *cap);
    }
    memcpy(*buffer + *len, str, str_len);
    *len += str_len;
    (*buffer)[*len] = '\0';
}

static void append_char(char** buffer, size_t* len, size_t* cap, char c) {
    if (*len + 1 >= *cap) {
        *cap = *cap == 0 ? 256 : *cap * 2;
        *buffer = realloc(*buffer, *cap);
    }
    (*buffer)[(*len)++] = c;
    (*buffer)[*len] = '\0';
}

static void append_indent(char** buffer, size_t* len, size_t* cap, int indent) {
    for (int i = 0; i < indent * 2; i++) {
        append_char(buffer, len, cap, ' ');
    }
}

// ============================================
// TYPE-SPECIFIC STRINGIFICATION
// ============================================

static void stringify_string(const char* str, char** buffer, size_t* len, size_t* cap) {
    append_char(buffer, len, cap, '"');
    while (*str) {
        switch (*str) {
            case '"': append_string(buffer, len, cap, "\\\""); break;
            case '\\': append_string(buffer, len, cap, "\\\\"); break;
            case '\b': append_string(buffer, len, cap, "\\b"); break;
            case '\f': append_string(buffer, len, cap, "\\f"); break;
            case '\n': append_string(buffer, len, cap, "\\n"); break;
            case '\r': append_string(buffer, len, cap, "\\r"); break;
            case '\t': append_string(buffer, len, cap, "\\t"); break;
            default: append_char(buffer, len, cap, *str); break;
        }
        str++;
    }
    append_char(buffer, len, cap, '"');
}

static void stringify_array(JsonArray* arr, char** buffer, size_t* len, size_t* cap, int pretty, int indent) {
    append_char(buffer, len, cap, '[');
    
    if (arr->count > 0) {
        if (pretty) {
            append_char(buffer, len, cap, '\n');
        }
        
        for (size_t i = 0; i < arr->count; i++) {
            if (pretty) {
                append_indent(buffer, len, cap, indent + 1);
            }
            
            stringify_value(arr->values[i], buffer, len, cap, pretty, indent + 1);
            
            if (i < arr->count - 1) {
                append_char(buffer, len, cap, ',');
            }
            
            if (pretty) {
                append_char(buffer, len, cap, '\n');
            }
        }
        
        if (pretty) {
            append_indent(buffer, len, cap, indent);
        }
    }
    
    append_char(buffer, len, cap, ']');
}

static void stringify_object(JsonObject* obj, char** buffer, size_t* len, size_t* cap, int pretty, int indent) {
    append_char(buffer, len, cap, '{');
    
    if (obj->count > 0) {
        if (pretty) {
            append_char(buffer, len, cap, '\n');
        }
        
        for (size_t i = 0; i < obj->count; i++) {
            if (pretty) {
                append_indent(buffer, len, cap, indent + 1);
            }
            
            stringify_string(obj->entries[i].key, buffer, len, cap);
            append_char(buffer, len, cap, ':');
            if (pretty) {
                append_char(buffer, len, cap, ' ');
            }
            
            stringify_value(obj->entries[i].value, buffer, len, cap, pretty, indent + 1);
            
            if (i < obj->count - 1) {
                append_char(buffer, len, cap, ',');
            }
            
            if (pretty) {
                append_char(buffer, len, cap, '\n');
            }
        }
        
        if (pretty) {
            append_indent(buffer, len, cap, indent);
        }
    }
    
    append_char(buffer, len, cap, '}');
}

static void stringify_value(JsonValue* value, char** buffer, size_t* len, size_t* cap, int pretty, int indent) {
    if (!value) {
        append_string(buffer, len, cap, "null");
        return;
    }
    
    switch (value->type) {
        case JSON_NULL:
            append_string(buffer, len, cap, "null");
            break;
            
        case JSON_BOOLEAN:
            append_string(buffer, len, cap, value->value.boolean ? "true" : "false");
            break;
            
        case JSON_NUMBER: {
            char num_buf[64];
            snprintf(num_buf, sizeof(num_buf), "%.15g", value->value.number);
            append_string(buffer, len, cap, num_buf);
            break;
        }
            
        case JSON_STRING:
            stringify_string(value->value.string, buffer, len, cap);
            break;
            
        case JSON_ARRAY:
            stringify_array(value->value.array, buffer, len, cap, pretty, indent);
            break;
            
        case JSON_OBJECT:
            stringify_object(value->value.object, buffer, len, cap, pretty, indent);
            break;
    }
}

// ============================================
// PUBLIC API
// ============================================

char* json_stringify(JsonValue* value, int pretty) {
    char* buffer = NULL;
    size_t len = 0;
    size_t cap = 0;
    
    stringify_value(value, &buffer, &len, &cap, pretty, 0);
    
    return buffer;
}

void json_print(JsonValue* value) {
    char* str = json_stringify(value, 1);
    if (str) {
        printf("%s\n", str);
        free(str);
    }
}

JsonValue* json_clone(JsonValue* value) {
    if (!value) return NULL;
    
    switch (value->type) {
        case JSON_NULL:
            return json_create_null();
            
        case JSON_BOOLEAN:
            return json_create_boolean(value->value.boolean);
            
        case JSON_NUMBER:
            return json_create_number(value->value.number);
            
        case JSON_STRING:
            return json_create_string(value->value.string);
            
        case JSON_ARRAY: {
            JsonValue* arr = json_create_array();
            for (size_t i = 0; i < value->value.array->count; i++) {
                JsonValue* cloned = json_clone(value->value.array->values[i]);
                json_array_append(arr->value.array, cloned);
            }
            return arr;
        }
            
        case JSON_OBJECT: {
            JsonValue* obj = json_create_object();
            for (size_t i = 0; i < value->value.object->count; i++) {
                JsonValue* cloned = json_clone(value->value.object->entries[i].value);
                json_object_set(obj->value.object, 
                    value->value.object->entries[i].key, cloned);
            }
            return obj;
        }
    }
    
    return NULL;
}
