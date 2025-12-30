/**
 * runtime_error.c - Error handling and assertions
 * 
 * Part of MLP Runtime - Modularized from runtime.c (3245 lines)
 * Module size: ~100 lines (MODULAR ✓)
 * Dependencies: None (base module)
 * 
 * 6 Temel Esas:
 * 1. MODULAR ✓ (< 500 satır)
 * 2. GCC ✓
 * 3. STO ✓ (N/A for error module)
 * 4. STATELESS ⚠️ (mlp_last_error_code static - thread-local in future)
 * 5. STRUCT+FUNC ✓
 * 6. MODÜL=ŞABLON ✓
 */

#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>

// Global error code storage
// TODO(STATELESS): Convert to thread-local storage (__thread) for thread safety
static int mlp_last_error_code = 0;

/**
 * panic - Print error message to stderr and exit with code 1
 * @param message: Error message to print
 *
 * MLP Usage: panic("Fatal error: division by zero");
 */
void panic(const char* message) {
    if (message) {
        fprintf(stderr, "PANIC: %s\n", message);
    } else {
        fprintf(stderr, "PANIC: Unknown error\n");
    }
    exit(1);
}

/**
 * mlp_assert - Check condition, panic if false
 * @param condition: Condition to check (0 = false, non-zero = true)
 * @param message: Error message if condition fails
 *
 * MLP Usage: mlp_assert(x > 0, "x must be positive");
 */
void mlp_assert(long condition, const char* message) {
    if (!condition) {
        if (message) {
            fprintf(stderr, "ASSERTION FAILED: %s\n", message);
        } else {
            fprintf(stderr, "ASSERTION FAILED\n");
        }
        exit(1);
    }
}

/**
 * exit_with_code - Exit program with specific return code
 * @param code: Exit code (0 = success, non-zero = error)
 *
 * MLP Usage: exit_with_code(1);
 */
void exit_with_code(long code) {
    exit((int)code);
}

/**
 * get_error_code - Get last error code
 * @return: Last error code set
 *
 * MLP Usage: long code = get_error_code();
 */
long get_error_code(void) {
    return mlp_last_error_code;
}

/**
 * set_error_code - Set error code
 * @param code: Error code to set
 *
 * MLP Usage: set_error_code(ERR_FILE_NOT_FOUND);
 */
void set_error_code(long code) {
    mlp_last_error_code = code;
}
