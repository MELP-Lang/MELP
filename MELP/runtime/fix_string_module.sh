#!/bin/bash
# Quick fix for runtime_string.c

# Remove broken file
rm -f runtime_string.c runtime_string.o

# Create clean version
cat > runtime_string.c << 'HEADER'
/**
 * runtime_string.c - String operations module
 * Extracted from runtime.c (3245 lines)
 * Dependencies: runtime_memory.h, runtime_error.h
 */

#include "runtime_string.h"
#include "runtime_memory.h" 
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

HEADER

# Extract complete functions using better sed pattern
# This gets complete function bodies (handles multi-line)

# String concatenation functions
awk '/^char\* string_birlestir\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long string_karsilastir\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long string_esit_mi\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long string_uzunluk\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_karakter_al\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_char_at\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_alt\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long karakter_kodu\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* kodu_karaktere\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long char_code\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* code_to_char\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# List operations (returns MLP_List* containing strings)
awk '/^MLP_List\* string_split\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_join\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# String manipulation
awk '/^char\* string_replace\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_trim\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_upper\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_lower\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# String concat (alias)
awk '/^char\* string_concat\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# Query functions
awk '/^long string_length\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* string_substring\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long string_index_of\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long string_last_index_of\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# Character operations
awk '/^long char_code_at\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* char_at\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long len\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* chr\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# Substring helpers
awk '/^char\* mid\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* left\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char\* right\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# Boolean checks
awk '/^long contains\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long startswith\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^long endswith\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

# C helpers
awk '/^long c_string_length\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^char c_char_at\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c
awk '/^void c_set_char_at\(/ {p=1} p {print} /^}$/ && p {print ""; p=0}' runtime.c >> runtime_string.c

echo "✅ runtime_string.c created"
wc -l runtime_string.c
