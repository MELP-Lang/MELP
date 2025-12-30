/**
 * runtime_types.c - Type conversion and Dict operations
 * Dependencies: runtime_memory.h, runtime_string.h, runtime_error.h
 * STATELESS ✅: Dict now uses struct-based context (refactored)
 */

#define _DEFAULT_SOURCE

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

// ===== Dict Implementation =====
// NEW: Struct-based (STATELESS compliant)
// LEGACY: Global state (kept for backward compatibility)

// SozlukGirdisi struct defined in header

// ===== NEW STATELESS API (struct-based) =====

Sozluk* sozluk_yeni(int boyut) {
    if (boyut <= 0) {
        panic("Dictionary size must be positive");
        return NULL;
    }
    
    Sozluk* s = (Sozluk*)mlp_malloc(sizeof(Sozluk));
    if (!s) {
        panic("Dictionary allocation failed");
        return NULL;
    }
    
    s->boyut = boyut;
    s->kullanim = 0;
    s->tablo = (SozlukGirdisi*)calloc(boyut, sizeof(SozlukGirdisi));
    
    if (!s->tablo) {
        mlp_free(s);
        panic("Dictionary table allocation failed");
        return NULL;
    }
    
    return s;
}

void sozluk_sil(Sozluk* s) {
    if (!s) return;
    
    if (s->tablo) {
        for (int i = 0; i < s->boyut; i++) {
            if (s->tablo[i].kullanilmis && s->tablo[i].anahtar) {
                free(s->tablo[i].anahtar);
            }
        }
        free(s->tablo);
    }
    
    mlp_free(s);
}

void sozluk_koy(Sozluk* s, const char* anahtar, int64_t deger) {
    if (!s || !s->tablo) {
        panic("Dictionary not initialized");
        return;
    }
    
    unsigned long hash = hash_fonksiyonu(anahtar);
    int indeks = hash % s->boyut;
    int baslangic_indeksi = indeks;
    
    while (s->tablo[indeks].kullanilmis) {
        if (strcmp(s->tablo[indeks].anahtar, anahtar) == 0) {
            // Update existing
            s->tablo[indeks].deger = deger;
            return;
        }
        indeks = (indeks + 1) % s->boyut;
        if (indeks == baslangic_indeksi) {
            panic("Dictionary full!");
            return;
        }
    }
    
    // Insert new
    s->tablo[indeks].anahtar = strdup(anahtar);
    s->tablo[indeks].deger = deger;
    s->tablo[indeks].kullanilmis = 1;
    s->kullanim++;
}

int64_t sozluk_al(Sozluk* s, const char* anahtar) {
    if (!s || !s->tablo) {
        panic("Dictionary not initialized");
        return 0;
    }
    
    unsigned long hash = hash_fonksiyonu(anahtar);
    int indeks = hash % s->boyut;
    int baslangic_indeksi = indeks;
    
    while (s->tablo[indeks].kullanilmis) {
        if (strcmp(s->tablo[indeks].anahtar, anahtar) == 0) {
            return s->tablo[indeks].deger;
        }
        indeks = (indeks + 1) % s->boyut;
        if (indeks == baslangic_indeksi) break;
    }
    
    return 0; // Not found
}

// ===== LEGACY GLOBAL API (uses static state) =====

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
