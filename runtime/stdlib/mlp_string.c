/**
 * MLP Standard Library - String Operations
 * 
 * Architecture: TTO-compliant string operations
 * Works with both literal strings and heap-allocated strings
 * 
 * Created: 9 Aralık 2025 (YZ_06)
 */

#define _POSIX_C_SOURCE 200809L

#include "mlp_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// For strdup on older systems
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
char* strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}
#endif

// ============================================================================
// String Concatenation
// ============================================================================

/**
 * Concatenate two strings
 * 
 * @param str1 First string (can be literal or heap)
 * @param str2 Second string (can be literal or heap)
 * @return Newly allocated concatenated string (caller must free)
 * 
 * Example:
 *   text greeting = "Hello"
 *   text target = "World"
 *   text message = greeting + " " + target  // "Hello World"
 */
char* mlp_string_concat(const char* str1, const char* str2) {
    // Null safety
    if (!str1 && !str2) return strdup("");
    if (!str1) return strdup(str2);
    if (!str2) return strdup(str1);
    
    // Calculate total length
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2;
    
    // Allocate result (heap)
    char* result = (char*)malloc(total_len + 1);
    if (!result) {
        fprintf(stderr, "Error: mlp_string_concat - malloc failed\n");
        return NULL;
    }
    
    // Copy strings
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    result[total_len] = '\0';
    
    return result;
}

/**
 * Concatenate three strings (optimization for multi-operand expressions)
 * 
 * @param str1 First string
 * @param str2 Second string
 * @param str3 Third string
 * @return Newly allocated concatenated string
 * 
 * Example:
 *   text a = "Hello"
 *   text b = " "
 *   text c = "World"
 *   text result = a + b + c  // Uses mlp_string_concat3 for efficiency
 */
char* mlp_string_concat3(const char* str1, const char* str2, const char* str3) {
    if (!str1) str1 = "";
    if (!str2) str2 = "";
    if (!str3) str3 = "";
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t total_len = len1 + len2 + len3;
    
    char* result = (char*)malloc(total_len + 1);
    if (!result) {
        fprintf(stderr, "Error: mlp_string_concat3 - malloc failed\n");
        return NULL;
    }
    
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    memcpy(result + len1 + len2, str3, len3);
    result[total_len] = '\0';
    
    return result;
}

// ============================================================================
// String Comparison
// ============================================================================

/**
 * Compare two strings lexicographically
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return < 0 if str1 < str2
 *         = 0 if str1 == str2
 *         > 0 if str1 > str2
 * 
 * Usage in MLP:
 *   if password == "admin"   → mlp_string_compare(password, "admin") == 0
 *   if name != "guest"       → mlp_string_compare(name, "guest") != 0
 *   if word < "banana"       → mlp_string_compare(word, "banana") < 0
 */
int mlp_string_compare(const char* str1, const char* str2) {
    // Null handling: NULL < any string, NULL == NULL
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;
    
    return strcmp(str1, str2);
}

/**
 * Check string equality
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return 1 if equal, 0 if not equal
 * 
 * Usage in MLP:
 *   if msg == "test"  → mlp_string_equals(msg, "test")
 */
int mlp_string_equals(const char* str1, const char* str2) {
    return mlp_string_compare(str1, str2) == 0;
}

/**
 * Check string inequality
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return 1 if not equal, 0 if equal
 */
int mlp_string_not_equals(const char* str1, const char* str2) {
    return mlp_string_compare(str1, str2) != 0;
}

// ============================================================================
// Additional String Operations (Future expansion)
// ============================================================================

/**
 * Get string length
 * 
 * @param str String
 * @return Length of string (0 for NULL)
 */
size_t mlp_string_length(const char* str) {
    return str ? strlen(str) : 0;
}

/**
 * Check if string is empty
 * 
 * @param str String
 * @return 1 if empty or NULL, 0 otherwise
 */
int mlp_string_is_empty(const char* str) {
    return !str || str[0] == '\0';
}

/**
 * Duplicate a string
 * 
 * @param str Source string
 * @return New heap-allocated copy (caller must free)
 */
char* mlp_string_duplicate(const char* str) {
    return str ? strdup(str) : NULL;
}

/**
 * YZ_22: Extract substring
 * 
 * @param str Source string
 * @param start Starting index (0-based)
 * @param length Number of characters to extract
 * @return New heap-allocated substring (caller must free)
 */
char* mlp_string_substring(const char* str, size_t start, size_t length) {
    if (!str) return NULL;
    
    size_t str_len = strlen(str);
    if (start >= str_len) return strdup("");  // Out of bounds
    
    // Adjust length if it goes beyond string end
    if (start + length > str_len) {
        length = str_len - start;
    }
    
    // Allocate memory for substring + null terminator
    char* result = malloc(length + 1);
    if (!result) return NULL;
    
    // Copy substring
    memcpy(result, str + start, length);
    result[length] = '\0';
    
    return result;
}

/**
 * YZ_22: Find first occurrence of substring
 * 
 * @param str String to search in
 * @param substr Substring to find
 * @return Index of first occurrence, or -1 if not found
 */
int mlp_string_indexOf(const char* str, const char* substr) {
    if (!str || !substr) return -1;
    
    const char* found = strstr(str, substr);
    if (!found) return -1;
    
    return (int)(found - str);  // Calculate index
}

// ============================================================================
// Memory Management Helpers
// ============================================================================

/**
 * Free a string allocated by MLP stdlib
 * 
 * @param str String to free
 * 
 * Note: Do NOT free string literals (.rodata)!
 * Only free heap-allocated strings returned by concat/duplicate.
 */
void mlp_string_free(char* str) {
    if (str) {
        free(str);
    }
}
