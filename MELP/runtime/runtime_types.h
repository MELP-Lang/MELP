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
 * hash_fonksiyonu - Hash function for strings
 * @param str: Input string
 * @return: Hash value
 */
unsigned long hash_fonksiyonu(const char *str);

/**
 * sozluk_olustur - Create/initialize dictionary
 * @param boyut: Dictionary size
 * 
 * NOTE: Original global implementation - needs refactor to struct
 */
void sozluk_olustur(int boyut);

/**
 * sozluk_ekle - Add key-value pair to dictionary
 * @param anahtar: Key (string)
 * @param deger: Value (int64)
 */
void sozluk_ekle(const char* anahtar, int64_t deger);

/**
 * sozluk_getir - Get value from dictionary
 * @param anahtar: Key (string)
 * @return: Value (int64), or 0 if not found
 */
int64_t sozluk_getir(const char* anahtar);

/**
 * sozluk_yok_et - Destroy dictionary and free memory
 */
void sozluk_yok_et(void);

#endif /* RUNTIME_TYPES_H */
