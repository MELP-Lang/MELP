#ifndef RUNTIME_STRING_H
#define RUNTIME_STRING_H

/**
 * runtime_string.h - String operations
 * 
 * Part of MLP Runtime - Modularized from runtime.c
 * 6 Temel Esas: STATELESS, MODULAR (< 500 satır)
 */

#include <stdint.h>

// ===== Basic String Operations =====

char* string_birlestir(const char* str1, const char* str2);
char* string_concat(const char* s1, const char* s2);
int string_karsilastir(const char* str1, const char* str2);
int64_t string_esit_mi(const char* str1, const char* str2);
int64_t string_uzunluk(const char* str);
long string_length(const char* str);
long len(const char* str);

// ===== Substring Operations =====

char* string_karakter_al(const char* str, int64_t indeks);
char* string_char_at(const char* str, int64_t index);
char* string_alt(const char* str, int64_t baslangic, int64_t uzunluk);
char* string_substring(const char* str, long start, long length);
char* mid(const char* str, long start, long length);
char* left(const char* str, long length);
char* right(const char* str, long length);

// ===== Character Operations =====

int64_t karakter_kodu(const char* karakter_str);
char* kodu_karaktere(int64_t kod);
int64_t char_code(const char* ch);
char* code_to_char(int64_t code);
long char_code_at(const char* str, long index);
char* char_at(const char* str, long index);
char* chr(long code);

// ===== Search Operations =====

long string_index_of(const char* str, const char* needle);
long string_last_index_of(const char* str, const char* needle);
long contains(const char* str, const char* needle);
long startswith(const char* str, const char* prefix);
long endswith(const char* str, const char* suffix);

// ===== Transformation Operations =====

char* string_upper(const char* str);
char* string_lower(const char* str);
char* string_trim(const char* str);
char* string_replace(const char* str, const char* old, const char* new);

// ===== Split/Join Operations =====

char** string_split(const char* str, const char* delim, int* count);
char* string_join(char** parts, int count, const char* delim);

// ===== C Helper Functions =====

int64_t c_string_length(const char* str);
int64_t c_char_at(const char* str, int64_t index);
void c_set_char_at(char* str, int64_t index, int64_t ch);

#endif /* RUNTIME_STRING_H */
