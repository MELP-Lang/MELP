#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "normalize.h"
#include "syntax_rules.h"

/**
 * Helper: Karakterin word boundary olup olmadığını kontrol et
 * (whitespace, punctuation, veya string sonu)
 */
static int is_word_boundary(char c) {
    return c == '\0' || isspace(c) || ispunct(c);
}

/**
 * Helper: Pattern'in string içinde word boundary ile bulunması
 * 
 * @param str Arama yapılacak string
 * @param pattern Aranacak pattern
 * @return Pattern'in bulunduğu pozisyon, bulunamazsa NULL
 */
static const char* find_word_pattern(const char* str, const char* pattern) {
    size_t pattern_len = strlen(pattern);
    const char* pos = str;
    
    while ((pos = strstr(pos, pattern)) != NULL) {
        // Pattern öncesi word boundary kontrolü
        int before_ok = (pos == str) || is_word_boundary(*(pos - 1));
        
        // Pattern sonrası word boundary kontrolü
        int after_ok = is_word_boundary(pos[pattern_len]);
        
        if (before_ok && after_ok) {
            return pos;  // Geçerli match bulundu
        }
        
        pos++;  // Sonraki pozisyondan devam et
    }
    
    return NULL;  // Match bulunamadı
}

/**
 * Bir string'de belirli pattern'i replacement ile değiştir
 * Word boundary kontrolü yapar (false positive önlemek için)
 */
char* replace_pattern(const char* input, const char* from, const char* to) {
    if (!input || !from || !to) {
        return NULL;
    }
    
    size_t input_len = strlen(input);
    size_t from_len = strlen(from);
    size_t to_len = strlen(to);
    
    // Kaç tane replacement yapılacak say
    int count = 0;
    const char* pos = input;
    while ((pos = find_word_pattern(pos, from)) != NULL) {
        count++;
        pos += from_len;
    }
    
    if (count == 0) {
        // Hiç match yok, orijinal string'i kopyala
        return strdup(input);
    }
    
    // Yeni string için yer ayır
    // input_len - (count * from_len) + (count * to_len) + 1
    size_t new_len = input_len + count * (to_len - from_len) + 1;
    char* result = (char*)malloc(new_len);
    if (!result) {
        return NULL;
    }
    
    // Replacement işlemi
    char* dest = result;
    const char* src = input;
    
    while (*src) {
        const char* match = find_word_pattern(src, from);
        
        if (match == NULL) {
            // Kalan kısmı kopyala
            strcpy(dest, src);
            break;
        }
        
        // Match öncesi kısmı kopyala
        size_t prefix_len = match - src;
        memcpy(dest, src, prefix_len);
        dest += prefix_len;
        
        // Replacement'i ekle
        strcpy(dest, to);
        dest += to_len;
        
        // Match sonrasından devam et
        src = match + from_len;
    }
    
    return result;
}

/**
 * User input'u PMPL syntax'ına normalize et
 * Tüm dönüşüm kurallarını sırasıyla uygular
 */
char* normalize_to_pmpl(const char* input) {
    if (!input) {
        return NULL;
    }
    
    // İlk kopyayı oluştur
    char* result = strdup(input);
    if (!result) {
        return NULL;
    }
    
    // Her kuralı sırasıyla uygula
    for (int i = 0; PMPL_RULES[i].from != NULL; i++) {
        char* temp = replace_pattern(result, PMPL_RULES[i].from, PMPL_RULES[i].to);
        
        if (temp == NULL) {
            free(result);
            return NULL;
        }
        
        free(result);
        result = temp;
    }
    
    return result;
}
