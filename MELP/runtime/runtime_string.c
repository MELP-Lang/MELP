/**
 * runtime_string.c - String operations module
 * Extracted from runtime.c (3245 lines)
 * Dependencies: runtime_memory.h, runtime_error.h
 */

#define _GNU_SOURCE  // For strdup
#include "runtime_string.h"
#include "runtime_memory.h" 
#include "runtime_error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* string_birlestir(const char* str1, const char* str2) {
    
    // ⚠️ KORUMA: Gelen string'lerden herhangi biri NULL ise
    // (Bellek hatası/Segfault riskini engellemek için)
    if (str1 == NULL || str2 == NULL) {
        // Hata ayıklama için terminale yazdıralım
        fprintf(stderr, "Hata: Birleştirme için NULL string argümanı alındı.\n");
        return NULL; // Bu, Assembly'ye de NULL dönecektir.
    }
    
    // 1. Yeni string'in toplam uzunluğunu hesapla (+1, null terminator için)
    size_t uzunluk = strlen(str1) + strlen(str2) + 1;

    // 2. Yeni hafıza bloğu ayır (Dinamik hafıza yönetimi)
    char* yeni_str = (char*)malloc(uzunluk);
    if (yeni_str == NULL) {
        fprintf(stderr, "Hata: Birleştirme için hafıza ayrılamadı.\n");
        return NULL; 
    }

    // 3. İlk string'i yeni bloğa kopyala
    strcpy(yeni_str, str1);

    // 4. İkinci string'i ilkinin sonuna ekle (birleştir)
    strcat(yeni_str, str2);

    // 5. Birleştirilmiş yeni string'in adresini döndür
    return yeni_str;
}

char* string_karakter_al(const char* str, int64_t indeks) {
    if (str == NULL) {
        fprintf(stderr, "HATA [STRING_KARAKTER_AL]: NULL string\n");
        return NULL;
    }

    int64_t uzunluk = strlen(str);
    if (indeks < 0 || indeks >= uzunluk) {
        fprintf(stderr, "HATA [STRING_KARAKTER_AL]: İndeks sınır dışı (indeks=%ld, uzunluk=%ld)\n",
                indeks, uzunluk);
        return NULL;
    }

    // Tek karakterlik string oluştur
    char* sonuc = (char*)malloc(2); // 1 karakter + null terminator
    if (sonuc == NULL) {
        fprintf(stderr, "HATA [STRING_KARAKTER_AL]: Hafıza ayırma hatası\n");
        return NULL;
    }

    sonuc[0] = str[indeks];
    sonuc[1] = '\0';

    return sonuc;
}

char* string_char_at(const char* str, int64_t index) {
    return string_karakter_al(str, index);
}

char* string_alt(const char* str, int64_t baslangic, int64_t uzunluk) {
    if (str == NULL) {
        fprintf(stderr, "HATA [STRING_ALT]: NULL string\n");
        return NULL;
    }

    int64_t str_uzunluk = strlen(str);

    if (baslangic < 0 || baslangic >= str_uzunluk) {
        fprintf(stderr, "HATA [STRING_ALT]: Başlangıç indeksi sınır dışı\n");
        return NULL;
    }

    if (uzunluk < 0) {
        fprintf(stderr, "HATA [STRING_ALT]: Negatif uzunluk\n");
        return NULL;
    }

    // Gerçek alınabilecek uzunluğu hesapla (string sonunu aşmamalı)
    int64_t gercek_uzunluk = uzunluk;
    if (baslangic + uzunluk > str_uzunluk) {
        gercek_uzunluk = str_uzunluk - baslangic;
    }

    // Yeni string için bellek ayır
    char* sonuc = (char*)malloc(gercek_uzunluk + 1);
    if (sonuc == NULL) {
        fprintf(stderr, "HATA [STRING_ALT]: Hafıza ayırma hatası\n");
        return NULL;
    }

    // Karakterleri kopyala
    strncpy(sonuc, str + baslangic, gercek_uzunluk);
    sonuc[gercek_uzunluk] = '\0';

    return sonuc;
}

char* kodu_karaktere(int64_t kod) {
    if (kod < 0 || kod > 127) {
        fprintf(stderr, "HATA [KODU_KARAKTERE]: Geçersiz ASCII kodu: %ld\n", kod);
        return NULL;
    }

    char* sonuc = (char*)malloc(2);
    if (sonuc == NULL) {
        fprintf(stderr, "HATA [KODU_KARAKTERE]: Hafıza ayırma hatası\n");
        return NULL;
    }

    sonuc[0] = (char)kod;
    sonuc[1] = '\0';

    return sonuc;
}

char* code_to_char(int64_t code) {
    return kodu_karaktere(code);
}

char* string_join(char** parts, int count, const char* delim) {
    if (count == 0) return strdup("");

    // Calculate total length
    size_t total_len = 0;
    size_t delim_len = strlen(delim);

    for (int i = 0; i < count; i++) {
        total_len += strlen(parts[i]);
        if (i < count - 1) total_len += delim_len;
    }

    // Allocate buffer
    char* result = malloc(total_len + 1);
    result[0] = '\0';

    // Join parts
    for (int i = 0; i < count; i++) {
        strcat(result, parts[i]);
        if (i < count - 1) strcat(result, delim);
    }

    return result;
}

char* string_replace(const char* str, const char* old, const char* new) {
    char* result;
    int i, count = 0;
    size_t new_len = strlen(new);
    size_t old_len = strlen(old);

    // Count occurrences
    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], old) == &str[i]) {
            count++;
            i += old_len - 1;
        }
    }

    // Allocate result
    result = malloc(i + count * (new_len - old_len) + 1);

    // Replace
    i = 0;
    while (*str) {
        if (strstr(str, old) == str) {
            strcpy(&result[i], new);
            i += new_len;
            str += old_len;
        } else {
            result[i++] = *str++;
        }
    }

    result[i] = '\0';
    return result;
}

char* string_trim(const char* str) {
    while (isspace(*str)) str++;
    if (*str == '\0') return strdup("");

    const char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    size_t len = end - str + 1;
    char* result = malloc(len + 1);
    memcpy(result, str, len);
    result[len] = '\0';
    return result;
}

char* string_upper(const char* str) {
    char* result = strdup(str);
    for (char* p = result; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return result;
}

char* string_lower(const char* str) {
    char* result = strdup(str);
    for (char* p = result; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return result;
}

char* string_concat(const char* s1, const char* s2) {
    if (!s1) s1 = "";
    if (!s2) s2 = "";

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*)malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "Memory allocation failed in string_concat\n");
        exit(1);
    }

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

long string_length(const char* str) {
    if (!str) return 0;
    return strlen(str);
}

char* string_substring(const char* str, long start, long length) {
    if (!str) return strdup("");

    long str_len = strlen(str);
    if (start < 0 || start >= str_len) return strdup("");
    if (length < 0) length = 0;

    // Adjust length if it exceeds string
    if (start + length > str_len) {
        length = str_len - start;
    }

    char* result = (char*)malloc(length + 1);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in string_substring\n");
        exit(1);
    }

    strncpy(result, str + start, length);
    result[length] = '\0';
    return result;
}

long string_index_of(const char* str, const char* needle) {
    if (!str || !needle) return -1;

    char* found = strstr(str, needle);
    if (!found) return -1;

    return found - str;
}

long string_last_index_of(const char* str, const char* needle) {
    if (!str || !needle) return -1;

    char* last_found = NULL;
    char* current = (char*)str;

    while ((current = strstr(current, needle)) != NULL) {
        last_found = current;
        current++;
    }

    if (!last_found) return -1;
    return last_found - str;
}

long char_code_at(const char* str, long index) {
    if (!str) return 0;
    
    long len = strlen(str);
    if (index < 0 || index >= len) return 0;
    
    return (unsigned char)str[index];
}

char* char_at(const char* str, long index) {
    if (!str) return strdup("");
    
    long len = strlen(str);
    if (index < 0 || index >= len) return strdup("");
    
    char* result = (char*)malloc(2);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in char_at\n");
        exit(1);
    }
    
    result[0] = str[index];
    result[1] = '\0';
    return result;
}

long len(const char* str) {
    return string_length(str);
}

char* chr(long code) {
    if (code < 0 || code > 255) return strdup("");
    
    char* result = (char*)malloc(2);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in chr\n");
        exit(1);
    }
    
    result[0] = (char)code;
    result[1] = '\0';
    return result;
}

char* mid(const char* str, long start, long length) {
    // VB.NET uses 1-based indexing, convert to 0-based
    if (start > 0) start--;
    return string_substring(str, start, length);
}

char* left(const char* str, long length) {
    return string_substring(str, 0, length);
}

char* right(const char* str, long length) {
    if (!str) return strdup("");
    long str_len = strlen(str);
    if (length >= str_len) return strdup(str);
    return string_substring(str, str_len - length, length);
}

long contains(const char* str, const char* needle) {
    return string_index_of(str, needle) >= 0 ? 1 : 0;
}

long startswith(const char* str, const char* prefix) {
    if (!str || !prefix) return 0;
    size_t prefix_len = strlen(prefix);
    if (strlen(str) < prefix_len) return 0;
    return strncmp(str, prefix, prefix_len) == 0 ? 1 : 0;
}

long endswith(const char* str, const char* suffix) {
    if (!str || !suffix) return 0;
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0 ? 1 : 0;
}

void c_set_char_at(char* str, int64_t index, int64_t ch) {
    if (!str) return;
    str[index] = (char)ch;
}

