#ifndef RESULT_TYPE_H
#define RESULT_TYPE_H

#include "type_system.h"
#include "../lexer/lexer.h"
#include <stdbool.h>

/**
 * Result Type System - YZ_206
 * 
 * Implements Rust-style Result<T, E> type for safe error handling.
 * 
 * Syntax:
 *   result<numeric, string> = divide(10, 2)
 *   match result
 *     case ok(value): return value
 *     case error(msg): return -1
 *   end_match
 * 
 * Features:
 *   - Result<T, E> generic type
 *   - ok(value) constructor
 *   - error(msg) constructor
 *   - Pattern matching support
 *   - ? operator for error propagation
 */

// Result type variant (ok or error)
typedef enum {
    RESULT_OK,
    RESULT_ERROR
} ResultVariant;

// Result type structure
// Runtime representation: { variant: i8, value: T | error: E }
typedef struct ResultType {
    Type* value_type;       // T type
    Type* error_type;       // E type
    bool is_generic;        // Is this result<T, E> generic?
} ResultType;

// Result value (runtime)
typedef struct ResultValue {
    ResultVariant variant;
    union {
        void* ok_value;     // Success value (type T)
        void* error_value;  // Error value (type E)
    } data;
} ResultValue;

// Forward declare Statement for match body
typedef struct Statement Statement;

// Result match case
typedef struct ResultMatchCase {
    ResultVariant variant;      // RESULT_OK or RESULT_ERROR
    char* binding_name;         // Variable name to bind (e.g., "value" in "case ok(value)")
    Statement* body;            // Statement to execute
} ResultMatchCase;

// Result match expression
typedef struct ResultMatch {
    void* result_expr;          // Expression* that evaluates to result<T, E>
    ResultMatchCase ok_case;    // ok(value) case
    ResultMatchCase error_case; // error(msg) case
    bool has_ok_case;           // Was ok case provided?
    bool has_error_case;        // Was error case provided?
} ResultMatch;

// Result propagation expression (? operator)
// Example: numeric x = divide(10, 2)?
typedef struct ResultPropagation {
    void* result_expr;          // Expression* that evaluates to result<T, E>
    Type* result_type;          // Type of the result<T, E>
} ResultPropagation;

// Parse result type declaration
// Example: result<numeric, string>
ResultType* parse_result_type(Token** tokens, int* index);

// Create result type
Type* result_type_create(Type* value_type, Type* error_type);

// Check if type is Result<T, E>
bool is_result_type(Type* type);

// Get result value type T
Type* result_get_value_type(Type* result);

// Get result error type E
Type* result_get_error_type(Type* result);

// Parse ok() constructor
// Example: return ok(42)
void* parse_ok_constructor(Token** tokens, int* index, Type* result_type);

// Parse error() constructor
// Example: return error("Division by zero")
void* parse_error_constructor(Token** tokens, int* index, Type* result_type);

// Parse match expression for Result
// Example: match result case ok(value): ... case error(msg): ... end_match
ResultMatch* parse_result_match(Token** tokens, int* index);

// Parse ? operator (error propagation)
// Example: numeric x = try_divide(10, 2)?
// Note: This is called AFTER the result expression has been parsed
ResultPropagation* parse_result_propagation(Token** tokens, int* index, void* result_expr);

// Type checking
bool result_type_check_ok(Type* result_type, Type* value_type);
bool result_type_check_error(Type* result_type, Type* error_type);
// Utility
char* result_type_to_string(ResultType* rt);
void result_type_free(ResultType* rt);
void result_match_free(ResultMatch* match);
void result_propagation_free(ResultPropagation* prop);

#endif
