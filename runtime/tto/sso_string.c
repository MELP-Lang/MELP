// ============================================================================
// SSO String - Small String Optimization
// ============================================================================
// Optimized string storage:
// - Strings ≤23 bytes: Stored inline (stack) - NO heap allocation
// - Strings >23 bytes: Stored on heap with pointer
//
// Architecture: Modular TTO Runtime Component
// Author: MLP Compiler Team
// Date: 7 Aralık 2025

#define _POSIX_C_SOURCE 200809L

#include "runtime_tto.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// SSO String Implementation
// ============================================================================

// Get string length (works for both SSO and heap)
size_t tto_sso_length(SSOString* sso) {
    if (!sso) return 0;
    
    if (sso->flags & 1) {
        // Heap string
        return sso->data.heap.length;
    } else {
        // SSO string - length stored in upper 7 bits of flags
        return sso->flags >> 1;
    }
}

// SSO string concatenation
SSOString* tto_sso_concat(SSOString* a, SSOString* b) {
    if (!a || !b) return NULL;
    
    const char* str_a = tto_sso_data(a);
    const char* str_b = tto_sso_data(b);
    
    if (!str_a || !str_b) return NULL;
    
    size_t len_a = tto_sso_length(a);
    size_t len_b = tto_sso_length(b);
    size_t total_len = len_a + len_b;
    
    SSOString* result = (SSOString*)malloc(sizeof(SSOString));
    if (!result) return NULL;
    
    if (total_len <= 23) {
        // Result fits in SSO
        memset(result->data.inline_data, 0, 24);
        memcpy(result->data.inline_data, str_a, len_a);
        memcpy(result->data.inline_data + len_a, str_b, len_b);
        result->data.inline_data[total_len] = '\0';
        result->flags = (total_len << 1);  // Store length, bit 0 = 0 (not heap)
    } else {
        // Result needs heap
        result->data.heap.heap_ptr = (char*)malloc(total_len + 1);
        if (!result->data.heap.heap_ptr) {
            free(result);
            return NULL;
        }
        memcpy(result->data.heap.heap_ptr, str_a, len_a);
        memcpy(result->data.heap.heap_ptr + len_a, str_b, len_b);
        result->data.heap.heap_ptr[total_len] = '\0';
        result->data.heap.length = total_len;
        result->data.heap.capacity = total_len + 1;
        result->flags = 1;  // Bit 0 = 1 (is heap)
    }
    
    return result;
}

// String comparison
int tto_sso_compare(SSOString* a, SSOString* b) {
    if (!a || !b) return 0;
    
    const char* str_a = tto_sso_data(a);
    const char* str_b = tto_sso_data(b);
    
    if (!str_a || !str_b) return 0;
    
    return strcmp(str_a, str_b);
}

// String equality check
bool tto_sso_equals(SSOString* a, SSOString* b) {
    return tto_sso_compare(a, b) == 0;
}

// Substring extraction
SSOString* tto_sso_substring(SSOString* str, size_t start, size_t length) {
    if (!str) return NULL;
    
    size_t str_len = tto_sso_length(str);
    if (start >= str_len) return NULL;
    
    // Adjust length if it exceeds string bounds
    if (start + length > str_len) {
        length = str_len - start;
    }
    
    const char* data = tto_sso_data(str);
    if (!data) return NULL;
    
    SSOString* result = (SSOString*)malloc(sizeof(SSOString));
    if (!result) return NULL;
    
    if (length <= 23) {
        // Result fits in SSO
        memset(result->data.inline_data, 0, 24);
        memcpy(result->data.inline_data, data + start, length);
        result->data.inline_data[length] = '\0';
        result->flags = (length << 1);
    } else {
        // Result needs heap
        result->data.heap.heap_ptr = (char*)malloc(length + 1);
        if (!result->data.heap.heap_ptr) {
            free(result);
            return NULL;
        }
        memcpy(result->data.heap.heap_ptr, data + start, length);
        result->data.heap.heap_ptr[length] = '\0';
        result->data.heap.length = length;
        result->data.heap.capacity = length + 1;
        result->flags = 1;
    }
    
    return result;
}

// String to integer conversion
int64_t tto_sso_to_int64(SSOString* str) {
    if (!str) return 0;
    
    const char* data = tto_sso_data(str);
    if (!data) return 0;
    
    return atoll(data);
}

// Integer to string conversion
SSOString* tto_sso_from_int64(int64_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lld", (long long)value);
    return tto_sso_create(buffer);
}

// String find - returns index of first occurrence, or -1 if not found
int tto_sso_find(SSOString* haystack, const char* needle) {
    if (!haystack || !needle) return -1;
    
    const char* data = tto_sso_data(haystack);
    if (!data) return -1;
    
    const char* found = strstr(data, needle);
    if (!found) return -1;
    
    return (int)(found - data);
}

// String copy - creates a new independent copy
SSOString* tto_sso_copy(SSOString* str) {
    if (!str) return NULL;
    
    const char* data = tto_sso_data(str);
    if (!data) return NULL;
    
    return tto_sso_create(data);
}

// Convert to C string (caller must free)
char* tto_sso_to_cstring(SSOString* str) {
    if (!str) return NULL;
    
    const char* data = tto_sso_data(str);
    if (!data) return NULL;
    
    size_t len = tto_sso_length(str);
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    strcpy(result, data);
    return result;
}

// String starts with
bool tto_sso_starts_with(SSOString* str, const char* prefix) {
    if (!str || !prefix) return false;
    
    const char* data = tto_sso_data(str);
    if (!data) return false;
    
    size_t prefix_len = strlen(prefix);
    size_t str_len = tto_sso_length(str);
    
    if (prefix_len > str_len) return false;
    
    return strncmp(data, prefix, prefix_len) == 0;
}

// String ends with
bool tto_sso_ends_with(SSOString* str, const char* suffix) {
    if (!str || !suffix) return false;
    
    const char* data = tto_sso_data(str);
    if (!data) return false;
    
    size_t suffix_len = strlen(suffix);
    size_t str_len = tto_sso_length(str);
    
    if (suffix_len > str_len) return false;
    
    return strcmp(data + (str_len - suffix_len), suffix) == 0;
}
