/**
 * MLP Standard Library - I/O Functions Header
 * 
 * Architecture: Works with TTO (Transparent Type Optimization)
 * User only sees: numeric, string, boolean
 * Runtime handles: int64/BigDecimal, SSO/heap internally
 */

#ifndef MLP_IO_H
#define MLP_IO_H

#include <stdint.h>

// TTO Type codes (matches tto_types.h)
#define TTO_TYPE_INT64      0
#define TTO_TYPE_DOUBLE     1
#define TTO_TYPE_BIGDECIMAL 2
#define TTO_TYPE_STRING     3
#define TTO_TYPE_BOOLEAN    4

// ============================================================================
// Core I/O Functions (TTO-aware)
// ============================================================================

// Print numeric value with newline
// value: pointer to numeric (int64*, double*, or BigDecimal*)
// tto_type: TTO_TYPE_INT64, TTO_TYPE_DOUBLE, or TTO_TYPE_BIGDECIMAL
void mlp_println_numeric(void* value, uint8_t tto_type);

// Print string with newline
void mlp_println_string(const char* str);

// Print boolean with newline
void mlp_println_bool(int value);

// Print without newline
void mlp_print_numeric(void* value, uint8_t tto_type);
void mlp_print_string(const char* str);
void mlp_print_bool(int value);

// Convert numeric to string
// Returns: dynamically allocated string (caller must free)
char* mlp_toString_numeric(void* value, uint8_t tto_type);
char* mlp_toString_bool(int value);

#endif
