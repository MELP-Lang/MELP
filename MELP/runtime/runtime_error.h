#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

/**
 * runtime_error.h - Error handling and assertions
 * 
 * Part of MLP Runtime - Modularized from runtime.c
 * 6 Temel Esas: STATELESS, MODULAR (< 500 satır)
 */

// Error codes
#define ERR_SUCCESS 0
#define ERR_FILE_NOT_FOUND 101
#define ERR_PERMISSION_DENIED 102
#define ERR_IO_ERROR 103
#define ERR_OUT_OF_MEMORY 104
#define ERR_INVALID_ARGUMENT 105
#define ERR_ASSERTION_FAILED 106

/**
 * panic - Print error message to stderr and exit with code 1
 * @param message: Error message to print
 *
 * MLP Usage: panic("Fatal error: division by zero");
 */
void panic(const char* message);

/**
 * mlp_assert - Check condition, panic if false
 * @param condition: Condition to check (0 = false, non-zero = true)
 * @param message: Error message if condition fails
 *
 * MLP Usage: mlp_assert(x > 0, "x must be positive");
 */
void mlp_assert(long condition, const char* message);

/**
 * exit_with_code - Exit program with specific return code
 * @param code: Exit code (0 = success, non-zero = error)
 *
 * MLP Usage: exit_with_code(1);
 */
void exit_with_code(long code);

/**
 * get_error_code - Get last error code
 * @return: Last error code set
 *
 * MLP Usage: long code = get_error_code();
 */
long get_error_code(void);

/**
 * set_error_code - Set error code
 * @param code: Error code to set
 *
 * MLP Usage: set_error_code(ERR_FILE_NOT_FOUND);
 */
void set_error_code(long code);

#endif /* RUNTIME_ERROR_H */
