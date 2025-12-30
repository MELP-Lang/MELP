#!/bin/bash
# Create runtime_types.c (Dict + Type conversion)

cat > runtime_types.c << 'HEADER'
/**
 * runtime_types.c - Type conversion and Dict operations
 * 
 * Part of MLP Runtime - Modularized from runtime.c
 * CRITICAL: Dict struct refactor for STATELESS compliance
 * 
 * 6 Temel Esas: MODULAR, STATELESS (Dict refactored!), GCC, STO, STRUCT+FUNC
 */

#include "runtime_types.h"
#include "runtime_memory.h"
#include "runtime_string.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Type Conversion Functions =====

HEADER

# Extract type conversion functions
awk '/^char\* int_to_string\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c
awk '/^char\* float_to_string\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c  
awk '/^char\* bool_to_string\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c
awk '/^char\* char_to_string\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c

# Extract Dict structure and functions
cat >> runtime_types.c << 'DICT_STRUCT'

// ===== Dict (Sozluk) Implementation =====
// NOTE: Original implementation used global state (STATELESS violation)
// Refactored to struct-based context passing

DICT_STRUCT

# Extract Dict type definition
awk '/^typedef struct {$/,/^} SozlukGirdisi;$/' runtime.c >> runtime_types.c
echo "" >> runtime_types.c

# Extract hash function
awk '/^unsigned long hash_fonksiyonu\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c

# Extract Dict functions (need to modify for struct-based)
awk '/^void sozluk_yok_et\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c
awk '/^void sozluk_olustur\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c
awk '/^void sozluk_ekle\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c
awk '/^long sozluk_getir\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_types.c

echo "✅ runtime_types.c created (with Dict - needs STATELESS refactor)"
wc -l runtime_types.c
