#ifndef RUNTIME_TYPES_H
#define RUNTIME_TYPES_H

/**
 * runtime_types.h - Type conversion and Dict operations
 * 
 * Part of MLP Runtime - Modularized from runtime.c
 * 6 Temel Esas: STATELESS (Dict refactored to struct)
 */

#include <stdint.h>

// ===== Type Conversion =====

/**
 * int_to_string - Convert integer to string
 * @param num: Integer value
 * @return: String representation (caller must free)
 */
char* int_to_string(int64_t num);

/**
 * float_to_string - Convert float to string
 * @param num: Float value
 * @return: String representation (caller must free)
 */
char* float_to_string(double num);

/**
 * bool_to_string - Convert boolean to string
 * @param value: Boolean value (0=false, 1=true)
 * @return: "true" or "false" (caller must free)
 */
char* bool_to_string(int value);

/**
 * char_to_string - Convert character to string
 * @param ch: Character value
 * @return: Single-character string (caller must free)
 */
char* char_to_string(int ch);

// ===== Dict (Sozluk) Operations =====

/**
 * SozlukGirdisi - Hash table entry
 */
typedef struct {
    char* anahtar;
    int64_t deger;
    int kullanilmis;
} SozlukGirdisi;

/**
 * Sozluk - Dictionary context (STATELESS compliant)
 */
typedef struct {
    SozlukGirdisi* tablo;
    int boyut;
    int kullanim;
} Sozluk;

/**
 * hash_fonksiyonu - Hash function for strings
 * @param str: Input string
 * @return: Hash value
 */
unsigned long hash_fonksiyonu(const char *str);

// ===== NEW STATELESS API (struct-based) =====

/**
 * sozluk_yeni - Create new dictionary (STATELESS)
 * @param boyut: Dictionary size
 * @return: Dictionary pointer (caller must free with sozluk_sil)
 */
Sozluk* sozluk_yeni(int boyut);

/**
 * sozluk_sil - Destroy dictionary and free memory (STATELESS)
 * @param s: Dictionary pointer
 */
void sozluk_sil(Sozluk* s);

/**
 * sozluk_koy - Add key-value pair to dictionary (STATELESS)
 * @param s: Dictionary pointer
 * @param anahtar: Key (string)
 * @param deger: Value (int64)
 */
void sozluk_koy(Sozluk* s, const char* anahtar, int64_t deger);

/**
 * sozluk_al - Get value from dictionary (STATELESS)
 * @param s: Dictionary pointer
 * @param anahtar: Key (string)
 * @return: Value (int64), or 0 if not found
 */
int64_t sozluk_al(Sozluk* s, const char* anahtar);

// ===== LEGACY GLOBAL API (deprecated, kept for compatibility) =====

/**
 * sozluk_olustur - Create/initialize dictionary (LEGACY - uses global state)
 * @param boyut: Dictionary size
 * @deprecated Use sozluk_yeni() instead
 */
void sozluk_olustur(int boyut);

/**
 * sozluk_ekle - Add key-value pair to dictionary (LEGACY)
 * @param anahtar: Key (string)
 * @param deger: Value (int64)
 * @deprecated Use sozluk_koy() instead
 */
void sozluk_ekle(const char* anahtar, int64_t deger);

/**
 * sozluk_getir - Get value from dictionary (LEGACY)
 * @param anahtar: Key (string)
 * @return: Value (int64), or 0 if not found
 * @deprecated Use sozluk_al() instead
 */
int64_t sozluk_getir(const char* anahtar);

/**
 * sozluk_yok_et - Destroy dictionary and free memory (LEGACY)
 * @deprecated Use sozluk_sil() instead
 */
void sozluk_yok_et(void);

#endif /* RUNTIME_TYPES_H */
