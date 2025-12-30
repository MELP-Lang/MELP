/**
 * STO Runtime Stubs for Bootstrap
 * Provides minimal BigDecimal support for mlp_io.c
 * 
 * NOTE: Full STO runtime is in MELP/runtime/sto/
 * This is TEMPORARY for Task 5.5 testing only
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Stub: Convert BigDecimal to string
// Real implementation in MELP/runtime/sto/bigdecimal.c
char* sto_bigdec_to_string(void* bigdec) {
    // For now, just return "0" (bootstrap stub)
    // Full STO integration in Stage2
    (void)bigdec;  // Suppress unused parameter warning
    
    char* result = malloc(2);
    if (result) {
        result[0] = '0';
        result[1] = '\0';
    }
    return result;
}
