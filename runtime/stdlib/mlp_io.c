/**
 * MLP Standard Library - I/O Functions
 * 
 * Architecture: Works with TTO (Transparent Type Optimization)
 * Handles int64, double, BigDecimal transparently
 */

#include "mlp_io.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

// For strdup on older systems
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
char* strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}
#endif

// External TTO runtime functions (from libtto_runtime.a)
extern char* tto_bigdec_to_string(void* bigdec);

// ============================================================================
// Core TTO-Aware Functions
// ============================================================================

// Print numeric value with newline (TTO-aware)
void mlp_println_numeric(void* value, uint8_t tto_type) {
    if (!value) {
        printf("(null)\n");
        return;
    }
    
    switch (tto_type) {
        case TTO_TYPE_INT64:
            printf("%" PRId64 "\n", *(int64_t*)value);
            break;
            
        case TTO_TYPE_DOUBLE:
            printf("%g\n", *(double*)value);
            break;
            
        case TTO_TYPE_BIGDECIMAL: {
            char* str = tto_bigdec_to_string(value);
            if (str) {
                printf("%s\n", str);
                free(str);
            } else {
                printf("(BigDecimal error)\n");
            }
            break;
        }
        
        default:
            printf("(unknown numeric type: %d)\n", tto_type);
    }
}

// Print without newline (TTO-aware)
void mlp_print_numeric(void* value, uint8_t tto_type) {
    if (!value) {
        printf("(null)");
        return;
    }
    
    switch (tto_type) {
        case TTO_TYPE_INT64:
            printf("%" PRId64, *(int64_t*)value);
            break;
            
        case TTO_TYPE_DOUBLE:
            printf("%g", *(double*)value);
            break;
            
        case TTO_TYPE_BIGDECIMAL: {
            char* str = tto_bigdec_to_string(value);
            if (str) {
                printf("%s", str);
                free(str);
            } else {
                printf("(BigDecimal error)");
            }
            break;
        }
        
        default:
            printf("(unknown numeric type: %d)", tto_type);
    }
}

// Convert numeric to string (TTO-aware)
char* mlp_toString_numeric(void* value, uint8_t tto_type) {
    if (!value) return strdup("null");
    
    char buffer[64];
    switch (tto_type) {
        case TTO_TYPE_INT64:
            snprintf(buffer, sizeof(buffer), "%" PRId64, *(int64_t*)value);
            return strdup(buffer);
            
        case TTO_TYPE_DOUBLE:
            snprintf(buffer, sizeof(buffer), "%g", *(double*)value);
            return strdup(buffer);
            
        case TTO_TYPE_BIGDECIMAL:
            return tto_bigdec_to_string(value);  // Already allocates
            
        default:
            return strdup("(unknown)");
    }
}

// ============================================================================
// String & Boolean Functions (unchanged)
// ============================================================================

// Print string with newline
void mlp_println_string(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("(null)\n");
    }
}

// Print boolean with newline
void mlp_println_bool(int value) {
    printf("%s\n", value ? "true" : "false");
}

// Print without newline variants
void mlp_print_string(const char* str) {
    if (str) {
        printf("%s", str);
    }
}

void mlp_print_bool(int value) {
    printf("%s", value ? "true" : "false");
}

// Convert to string
char* mlp_toString_bool(int value) {
    return strdup(value ? "true" : "false");
}
