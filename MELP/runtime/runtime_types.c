/**
 * runtime_types.c - Type conversion and Dict operations
 * Dependencies: runtime_memory.h, runtime_string.h, runtime_error.h
 * STATELESS ⚠️: Dict uses global state (bootstrap acceptable, refactor later)
 */

#include "runtime_types.h"
#include "runtime_memory.h"
#include "runtime_string.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Type Conversion Functions =====

char* int_to_string(int64_t num) {
    char* buffer = (char*)mlp_malloc(32);
    if (!buffer) {
        panic("Memory allocation failed in int_to_string");
        return NULL;
    }
    snprintf(buffer, 32, "%ld", num);
    return buffer;
}

char* float_to_string(double num) {
    char* buffer = (char*)mlp_malloc(64);
    if (!buffer) {
        panic("Memory allocation failed in float_to_string");
        return NULL;
    }
    snprintf(buffer, 64, "%g", num);
    return buffer;
}

char* bool_to_string(int value) {
    return strdup(value ? "true" : "false");
}

char* char_to_string(int ch) {
    char* str = (char*)malloc(2);
    if (!str) {
        panic("Memory allocation failed in char_to_string");
        return NULL;
    }
    str[0] = (char)ch;
    str[1] = '\0';
    return str;
}

// ===== Dict Implementation (Global state - STATELESS violation) =====
// NOTE: Using global state for bootstrap phase. Future: struct-based context.

// SozlukGirdisi struct defined in header

static SozlukGirdisi* sozluk_tablosu = NULL;
static int sozluk_boyutu = 0;

unsigned long hash_fonksiyonu(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void sozluk_yok_et(void) {
    if (sozluk_tablosu == NULL) return;
    for (int i = 0; i < sozluk_boyutu; i++) {
        if (sozluk_tablosu[i].kullanilmis && sozluk_tablosu[i].anahtar != NULL) {
            free(sozluk_tablosu[i].anahtar);
        }
    }
    free(sozluk_tablosu);
    sozluk_tablosu = NULL;
    sozluk_boyutu = 0;
}

void sozluk_olustur(int boyut) {
    if (sozluk_tablosu != NULL) {
        sozluk_yok_et();
    }
    sozluk_boyutu = boyut;
    sozluk_tablosu = (SozlukGirdisi*)calloc(sozluk_boyutu, sizeof(SozlukGirdisi));
    if (sozluk_tablosu == NULL) {
        panic("Dictionary memory allocation failed");
    }
}

void sozluk_ekle(const char* anahtar, int64_t deger) {
    if (sozluk_tablosu == NULL) {
        panic("Dictionary not initialized");
        return;
    }
    unsigned long hash = hash_fonksiyonu(anahtar);
    int indeks = hash % sozluk_boyutu;
    int baslangic_indeksi = indeks;
    while (sozluk_tablosu[indeks].kullanilmis) {
        if (strcmp(sozluk_tablosu[indeks].anahtar, anahtar) == 0) {
            sozluk_tablosu[indeks].deger = deger;
            return;
        }
        indeks = (indeks + 1) % sozluk_boyutu;
        if (indeks == baslangic_indeksi) {
            panic("Dictionary full!");
            return;
        }
    }
    sozluk_tablosu[indeks].anahtar = strdup(anahtar);
    sozluk_tablosu[indeks].deger = deger;
    sozluk_tablosu[indeks].kullanilmis = 1;
}

int64_t sozluk_getir(const char* anahtar) {
    if (sozluk_tablosu == NULL) {
        panic("Dictionary not initialized");
        return 0;
    }
    unsigned long hash = hash_fonksiyonu(anahtar);
    int indeks = hash % sozluk_boyutu;
    int baslangic_indeksi = indeks;
    while (sozluk_tablosu[indeks].kullanilmis) {
        if (strcmp(sozluk_tablosu[indeks].anahtar, anahtar) == 0) {
            return sozluk_tablosu[indeks].deger;
        }
        indeks = (indeks + 1) % sozluk_boyutu;
        if (indeks == baslangic_indeksi) break;
    }
    return 0; // Not found
}
