#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// ============================================================================
// Phase 6: Enhanced Error System with Colors, Source Display, and Suggestions
// YZ_30: Better compiler diagnostics for MELP
// ============================================================================

// Error severity levels
typedef enum {
    ERROR_LEVEL_WARNING,  // Compilation continues
    ERROR_LEVEL_ERROR,    // Compilation fails
    ERROR_LEVEL_FATAL,    // Immediate exit
    ERROR_LEVEL_NOTE,     // Additional information (non-error)
    ERROR_LEVEL_HINT      // Helpful suggestion
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
    int max_errors;       // Stop after this many errors (0 = unlimited)
    FILE* output;         // Where to write errors (default: stderr)
    int use_colors;       // Enable/disable colored output
    const char* source;   // Source code for line display
    const char* filename; // Current file being compiled
} ErrorState;

// ============================================================================
// ANSI Color Codes (for terminal output)
// ============================================================================
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

// Bold colors
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"
#define COLOR_BOLD_WHITE   "\033[1;37m"

// ============================================================================
// Core Functions
// ============================================================================

// Initialize error system
void error_init(void);

// Set error output destination
void error_set_output(FILE* output);

// Set maximum error count before stopping
void error_set_max_errors(int max);

// Enable/disable colored output
void error_set_colors(int enabled);

// Set source code for line display
void error_set_source(const char* source, const char* filename);

// Report an error (basic)
void error_report(ErrorLevel level, ErrorCategory category, 
                 int line, const char* file, const char* format, ...);

// Report error with column information
void error_report_detailed(ErrorLevel level, ErrorCategory category,
                          int line, int column, const char* file,
                          const char* format, ...);

// ============================================================================
// Phase 6: Enhanced Error Reporting
// ============================================================================

// Report error with source line display and caret pointing
void error_report_with_source(ErrorLevel level, ErrorCategory category,
                              int line, int column, int length,
                              const char* format, ...);

// Report error with suggestion ("Did you mean...?")
void error_report_with_suggestion(ErrorLevel level, ErrorCategory category,
                                  int line, int column, int length,
                                  const char* suggestion,
                                  const char* format, ...);

// Display a note (additional information after an error)
void error_note(const char* format, ...);

// Display a hint (helpful suggestion)
void error_hint(const char* format, ...);

// Get the source line at given line number
const char* error_get_source_line(int line, int* line_length);

// ============================================================================
// Convenience Macros (backward compatible)
// ============================================================================

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

// ============================================================================
// Phase 6: New Convenience Macros
// ============================================================================

// Error with source display
#define error_parser_src(line, col, len, ...) \
    error_report_with_source(ERROR_LEVEL_ERROR, ERROR_CAT_PARSER, line, col, len, __VA_ARGS__)

#define error_lexer_src(line, col, len, ...) \
    error_report_with_source(ERROR_LEVEL_ERROR, ERROR_CAT_LEXER, line, col, len, __VA_ARGS__)

// Error with suggestion
#define error_parser_suggest(line, col, len, suggestion, ...) \
    error_report_with_suggestion(ERROR_LEVEL_ERROR, ERROR_CAT_PARSER, line, col, len, suggestion, __VA_ARGS__)

// ============================================================================
// Statistics and Control
// ============================================================================

// Get error statistics
int error_get_count(void);
int error_get_warning_count(void);
int error_has_errors(void);

// Check if we should stop compilation
int error_should_stop(void);

// Reset error state
void error_reset(void);

// Print summary at end of compilation
void error_print_summary(void);

// ============================================================================
// Phase 6: Spell Checking / Did You Mean
// ============================================================================

// Calculate Levenshtein distance between two strings
int error_levenshtein_distance(const char* s1, const char* s2);

// Find closest match from a list of candidates
// Returns NULL if no close match found (distance > max_distance)
const char* error_find_similar(const char* input, const char** candidates, int count, int max_distance);

// Common MELP keywords for spell checking
extern const char* MELP_KEYWORDS[];
extern const int MELP_KEYWORD_COUNT;

#endif // ERROR_H
