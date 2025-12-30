#!/bin/bash
# Build runtime_string.c from runtime.c

cat > runtime_string.c << 'HEADER'
/**
 * runtime_string.c - String operations
 * 
 * Part of MLP Runtime - Modularized from runtime.c (3245 lines)
 * Dependencies: runtime_memory.h, runtime_error.h
 * 
 * 6 Temel Esas: MODULAR, STATELESS, GCC, STO, STRUCT+FUNC, MODÜL=ŞABLON
 */

#include "runtime_string.h"
#include "runtime_memory.h"
#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

HEADER

# Extract functions by line ranges (carefully verified)
awk 'NR>=130 && NR<=161' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=163 && NR<=173' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=175 && NR<=185' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=187 && NR<=198' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=434 && NR<=459' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=461 && NR<=463' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=474 && NR<=517' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=519 && NR<=533' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=535 && NR<=552' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=554 && NR<=561' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=936 && NR<=963' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=965 && NR<=997' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=999 && NR<=1037' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1039 && NR<=1058' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1060 && NR<=1073' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1075 && NR<=1088' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1134 && NR<=1143' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1237 && NR<=1255' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1606 && NR<=1618' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1620 && NR<=1649' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1651 && NR<=1666' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1668 && NR<=1689' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1691 && NR<=1706' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1708 && NR<=1726' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1736 && NR<=1747' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1759 && NR<=1773' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1795 && NR<=1807' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1809 && NR<=1819' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1821 && NR<=1834' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1836 && NR<=1846' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1848 && NR<=1861' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=1863 && NR<=1878' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=3217 && NR<=3221' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=3223 && NR<=3229' runtime.c >> runtime_string.c
echo "" >> runtime_string.c
awk 'NR>=3231 && NR<=3235' runtime.c >> runtime_string.c

echo "✅ runtime_string.c rebuilt"
wc -l runtime_string.c
