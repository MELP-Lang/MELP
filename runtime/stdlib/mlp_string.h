/**
 * MLP Standard Library - String Operations Header
 * 
 * TTO-compliant string operations for MELP compiler
 * 
 * Created: 9 Aralık 2025 (YZ_06)
 */

#ifndef MLP_STRING_H
#define MLP_STRING_H

#include <stddef.h>  // size_t

// String concatenation
char* mlp_string_concat(const char* str1, const char* str2);
char* mlp_string_concat3(const char* str1, const char* str2, const char* str3);

// String comparison
int mlp_string_compare(const char* str1, const char* str2);
int mlp_string_equals(const char* str1, const char* str2);
int mlp_string_not_equals(const char* str1, const char* str2);

// String properties
size_t mlp_string_length(const char* str);
int mlp_string_is_empty(const char* str);

// String manipulation
char* mlp_string_duplicate(const char* str);

// Memory management
void mlp_string_free(char* str);

#endif // MLP_STRING_H
