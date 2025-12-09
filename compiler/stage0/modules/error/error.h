#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Error severity levels
typedef enum {
    ERROR_LEVEL_WARNING,  // Compilation continues
    ERROR_LEVEL_ERROR,    // Compilation fails
    ERROR_LEVEL_FATAL     // Immediate exit
} ErrorLevel;

// Error categories
typedef enum {
    ERROR_CAT_LEXER,
    ERROR_CAT_PARSER,
    ERROR_CAT_SEMANTIC,
    ERROR_CAT_CODEGEN,
    ERROR_CAT_IO,
    ERROR_CAT_MEMORY,
    ERROR_CAT_INTERNAL
} ErrorCategory;

// Error context structure
typedef struct {
    ErrorLevel level;
    ErrorCategory category;
    int line;
    int column;
    const char* file;
    const char* message;
} ErrorContext;

// Global error state
typedef struct {
    int error_count;
    int warning_count;
    int max_errors;  // Stop after this many errors (0 = unlimited)
    FILE* output;    // Where to write errors (default: stderr)
} ErrorState;

// Initialize error system
void error_init(void);

// Set error output destination
void error_set_output(FILE* output);

// Set maximum error count before stopping
void error_set_max_errors(int max);

// Report an error
void error_report(ErrorLevel level, ErrorCategory category, 
                 int line, const char* file, const char* format, ...);

// Report error with column information
void error_report_detailed(ErrorLevel level, ErrorCategory category,
                          int line, int column, const char* file,
                          const char* format, ...);

// Convenience macros for common errors
#define error_lexer(line, ...) \
    error_report(ERROR_LEVEL_ERROR, ERROR_CAT_LEXER, line, __FILE__, __VA_ARGS__)

#define error_parser(line, ...) \
    error_report(ERROR_LEVEL_ERROR, ERROR_CAT_PARSER, line, __FILE__, __VA_ARGS__)

#define error_semantic(line, ...) \
    error_report(ERROR_LEVEL_ERROR, ERROR_CAT_SEMANTIC, line, __FILE__, __VA_ARGS__)

#define error_codegen(line, ...) \
    error_report(ERROR_LEVEL_ERROR, ERROR_CAT_CODEGEN, line, __FILE__, __VA_ARGS__)

#define error_io(...) \
    error_report(ERROR_LEVEL_ERROR, ERROR_CAT_IO, 0, __FILE__, __VA_ARGS__)

#define error_warning(line, ...) \
    error_report(ERROR_LEVEL_WARNING, ERROR_CAT_PARSER, line, __FILE__, __VA_ARGS__)

#define error_fatal(...) \
    error_report(ERROR_LEVEL_FATAL, ERROR_CAT_INTERNAL, 0, __FILE__, __VA_ARGS__)

// Get error statistics
int error_get_count(void);
int error_get_warning_count(void);
int error_has_errors(void);

// Check if we should stop compilation
int error_should_stop(void);

// Reset error state
void error_reset(void);

#endif // ERROR_H
