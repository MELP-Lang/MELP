// ============================================================================
// Phase 6: Enhanced Error System for MELP Compiler
// YZ_30: Better compiler diagnostics with colors, source display, suggestions
// ============================================================================

#include "error.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Global error state
static ErrorState global_error_state = {
    .error_count = 0,
    .warning_count = 0,
    .max_errors = 10,      // Stop after 10 errors by default
    .output = NULL,        // Will be set to stderr in error_init()
    .use_colors = 1,       // Colors enabled by default (if terminal supports)
    .source = NULL,
    .filename = NULL
};

// Recovery state
static int recovery_mode = 0;      // Are we in recovery mode?
static int recovery_count = 0;     // How many times have we recovered?

// ============================================================================
// Phase 11: Error Context Stack (YZ_37)
// For saving/restoring error context during module imports
// ============================================================================

#define MAX_CONTEXT_STACK 32

typedef struct {
    const char* source;
    const char* filename;
} ErrorContextFrame;

static ErrorContextFrame context_stack[MAX_CONTEXT_STACK];
static int context_stack_top = 0;

// ============================================================================
// Category names for display
// ============================================================================

static const char* category_names[] = {
    "Lexer",
    "Parser",
    "Semantic",
    "Codegen",
    "I/O",
    "Memory",
    "Internal"
};

// ============================================================================
// Helper Functions
// ============================================================================

// Check if output is a terminal (for color support)
static int is_terminal(FILE* f) {
    return isatty(fileno(f));
}

// Get color string (returns empty string if colors disabled)
static const char* get_color(const char* color) {
    if (!global_error_state.use_colors) {
        return "";
    }
    return color;
}

// Get level color
static const char* get_level_color(ErrorLevel level) {
    switch (level) {
        case ERROR_LEVEL_WARNING: return get_color(COLOR_BOLD_YELLOW);
        case ERROR_LEVEL_ERROR:   return get_color(COLOR_BOLD_RED);
        case ERROR_LEVEL_FATAL:   return get_color(COLOR_BOLD_RED);
        case ERROR_LEVEL_NOTE:    return get_color(COLOR_BOLD_CYAN);
        case ERROR_LEVEL_HINT:    return get_color(COLOR_BOLD_GREEN);
        default:                  return get_color(COLOR_RESET);
    }
}

// Get level name
static const char* get_level_name(ErrorLevel level) {
    switch (level) {
        case ERROR_LEVEL_WARNING: return "warning";
        case ERROR_LEVEL_ERROR:   return "error";
        case ERROR_LEVEL_FATAL:   return "fatal error";
        case ERROR_LEVEL_NOTE:    return "note";
        case ERROR_LEVEL_HINT:    return "hint";
        default:                  return "unknown";
    }
}

// ============================================================================
// Source Line Functions
// ============================================================================

// Get source line at given line number (1-based)
const char* error_get_source_line(int line, int* line_length) {
    if (!global_error_state.source || line < 1) {
        if (line_length) *line_length = 0;
        return NULL;
    }
    
    const char* src = global_error_state.source;
    int current_line = 1;
    const char* line_start = src;
    
    // Find the start of the requested line
    while (*src && current_line < line) {
        if (*src == '\n') {
            current_line++;
            line_start = src + 1;
        }
        src++;
    }
    
    if (current_line != line) {
        if (line_length) *line_length = 0;
        return NULL;
    }
    
    // Find line length
    const char* line_end = line_start;
    while (*line_end && *line_end != '\n') {
        line_end++;
    }
    
    if (line_length) *line_length = (int)(line_end - line_start);
    return line_start;
}

// Print source line with caret pointing to error
static void print_source_line(FILE* out, int line, int column, int length) {
    int line_length = 0;
    const char* src_line = error_get_source_line(line, &line_length);
    
    if (!src_line || line_length == 0) {
        return;
    }
    
    // Print line number
    fprintf(out, "%s%5d | %s", get_color(COLOR_BLUE), line, get_color(COLOR_RESET));
    
    // Print the source line
    fwrite(src_line, 1, line_length, out);
    fprintf(out, "\n");
    
    // Print caret line
    fprintf(out, "      | ");
    
    // Spaces until column
    int col = (column > 0) ? column - 1 : 0;
    for (int i = 0; i < col && i < line_length; i++) {
        if (src_line[i] == '\t') {
            fprintf(out, "\t");
        } else {
            fprintf(out, " ");
        }
    }
    
    // Print caret and tildes
    fprintf(out, "%s^", get_color(COLOR_BOLD_GREEN));
    if (length > 1) {
        for (int i = 1; i < length && (col + i) < line_length; i++) {
            fprintf(out, "~");
        }
    }
    fprintf(out, "%s\n", get_color(COLOR_RESET));
}

// ============================================================================
// Core Functions
// ============================================================================

void error_init(void) {
    global_error_state.error_count = 0;
    global_error_state.warning_count = 0;
    global_error_state.output = stderr;
    global_error_state.source = NULL;
    global_error_state.filename = NULL;
    
    // Auto-detect color support
    global_error_state.use_colors = is_terminal(stderr);
}

void error_set_output(FILE* output) {
    if (output != NULL) {
        global_error_state.output = output;
        // Update color support for new output
        global_error_state.use_colors = is_terminal(output);
    }
}

void error_set_max_errors(int max) {
    global_error_state.max_errors = max;
}

void error_set_colors(int enabled) {
    global_error_state.use_colors = enabled;
}

void error_set_source(const char* source, const char* filename) {
    global_error_state.source = source;
    global_error_state.filename = filename;
}

// ============================================================================
// Error Reporting Functions
// ============================================================================

void error_report(ErrorLevel level, ErrorCategory category, 
                 int line, const char* file, const char* format, ...) {
    (void)file;  // Unused for now (we use filename from error_set_source)
    
    // Initialize if not done yet
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else if (level != ERROR_LEVEL_NOTE && level != ERROR_LEVEL_HINT) {
        global_error_state.error_count++;
    }

    // Print filename if available
    if (global_error_state.filename) {
        fprintf(out, "%s%s%s:", 
                get_color(COLOR_BOLD_WHITE),
                global_error_state.filename,
                get_color(COLOR_RESET));
    }
    
    // Print line number
    if (line > 0) {
        fprintf(out, "%s%d%s:", 
                get_color(COLOR_BOLD_WHITE),
                line,
                get_color(COLOR_RESET));
    }
    
    // Print level and category
    fprintf(out, " %s%s%s [%s]: ", 
            get_level_color(level),
            get_level_name(level),
            get_color(COLOR_RESET),
            category_names[category]);
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    
    // Print source line if available
    if (line > 0 && global_error_state.source) {
        print_source_line(out, line, 1, 1);
    }
    
    fflush(out);

    // Stop compilation if too many errors or fatal
    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        error_print_summary();
        exit(1);
    }
}

void error_report_detailed(ErrorLevel level, ErrorCategory category,
                          int line, int column, const char* file,
                          const char* format, ...) {
    (void)file;
    
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else if (level != ERROR_LEVEL_NOTE && level != ERROR_LEVEL_HINT) {
        global_error_state.error_count++;
    }

    // Print filename if available
    if (global_error_state.filename) {
        fprintf(out, "%s%s%s:", 
                get_color(COLOR_BOLD_WHITE),
                global_error_state.filename,
                get_color(COLOR_RESET));
    }
    
    // Print line:column
    if (line > 0) {
        fprintf(out, "%s%d%s:", 
                get_color(COLOR_BOLD_WHITE),
                line,
                get_color(COLOR_RESET));
        if (column > 0) {
            fprintf(out, "%s%d%s:", 
                    get_color(COLOR_BOLD_WHITE),
                    column,
                    get_color(COLOR_RESET));
        }
    }
    
    // Print level and category
    fprintf(out, " %s%s%s [%s]: ", 
            get_level_color(level),
            get_level_name(level),
            get_color(COLOR_RESET),
            category_names[category]);
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    
    // Print source line if available
    if (line > 0 && global_error_state.source) {
        print_source_line(out, line, column, 1);
    }
    
    fflush(out);

    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        error_print_summary();
        exit(1);
    }
}

// ============================================================================
// Phase 6: Enhanced Error Reporting
// ============================================================================

void error_report_with_source(ErrorLevel level, ErrorCategory category,
                              int line, int column, int length,
                              const char* format, ...) {
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else if (level != ERROR_LEVEL_NOTE && level != ERROR_LEVEL_HINT) {
        global_error_state.error_count++;
    }

    // Print filename if available
    if (global_error_state.filename) {
        fprintf(out, "%s%s%s:", 
                get_color(COLOR_BOLD_WHITE),
                global_error_state.filename,
                get_color(COLOR_RESET));
    }
    
    // Print line:column
    if (line > 0) {
        fprintf(out, "%s%d%s:", 
                get_color(COLOR_BOLD_WHITE),
                line,
                get_color(COLOR_RESET));
        if (column > 0) {
            fprintf(out, "%s%d%s:", 
                    get_color(COLOR_BOLD_WHITE),
                    column,
                    get_color(COLOR_RESET));
        }
    }
    
    // Print level and category
    fprintf(out, " %s%s%s [%s]: ", 
            get_level_color(level),
            get_level_name(level),
            get_color(COLOR_RESET),
            category_names[category]);
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    
    // Print source line with caret
    if (line > 0 && global_error_state.source) {
        print_source_line(out, line, column, length);
    }
    
    fflush(out);

    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        error_print_summary();
        exit(1);
    }
}

void error_report_with_suggestion(ErrorLevel level, ErrorCategory category,
                                  int line, int column, int length,
                                  const char* suggestion,
                                  const char* format, ...) {
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else if (level != ERROR_LEVEL_NOTE && level != ERROR_LEVEL_HINT) {
        global_error_state.error_count++;
    }

    // Print filename if available
    if (global_error_state.filename) {
        fprintf(out, "%s%s%s:", 
                get_color(COLOR_BOLD_WHITE),
                global_error_state.filename,
                get_color(COLOR_RESET));
    }
    
    // Print line:column
    if (line > 0) {
        fprintf(out, "%s%d%s:", 
                get_color(COLOR_BOLD_WHITE),
                line,
                get_color(COLOR_RESET));
        if (column > 0) {
            fprintf(out, "%s%d%s:", 
                    get_color(COLOR_BOLD_WHITE),
                    column,
                    get_color(COLOR_RESET));
        }
    }
    
    // Print level and category
    fprintf(out, " %s%s%s [%s]: ", 
            get_level_color(level),
            get_level_name(level),
            get_color(COLOR_RESET),
            category_names[category]);
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    
    // Print source line with caret
    if (line > 0 && global_error_state.source) {
        print_source_line(out, line, column, length);
    }
    
    // Print suggestion
    if (suggestion && *suggestion) {
        fprintf(out, "      %s= hint:%s Did you mean '%s%s%s'?\n",
                get_color(COLOR_BOLD_GREEN),
                get_color(COLOR_RESET),
                get_color(COLOR_BOLD_CYAN),
                suggestion,
                get_color(COLOR_RESET));
    }
    
    fflush(out);

    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        error_print_summary();
        exit(1);
    }
}

void error_note(const char* format, ...) {
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    fprintf(out, "      %s= note:%s ", 
            get_color(COLOR_BOLD_CYAN),
            get_color(COLOR_RESET));
    
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    fflush(out);
}

void error_hint(const char* format, ...) {
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    fprintf(out, "      %s= hint:%s ", 
            get_color(COLOR_BOLD_GREEN),
            get_color(COLOR_RESET));
    
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    fflush(out);
}

// ============================================================================
// Statistics and Control
// ============================================================================

int error_get_count(void) {
    return global_error_state.error_count;
}

int error_get_warning_count(void) {
    return global_error_state.warning_count;
}

int error_has_errors(void) {
    return global_error_state.error_count > 0;
}

int error_should_stop(void) {
    if (global_error_state.max_errors == 0) {
        return 0;  // No limit
    }
    return global_error_state.error_count >= global_error_state.max_errors;
}

void error_reset(void) {
    global_error_state.error_count = 0;
    global_error_state.warning_count = 0;
    recovery_mode = 0;
    recovery_count = 0;
}

void error_print_summary(void) {
    FILE* out = global_error_state.output;
    if (!out) out = stderr;
    
    fprintf(out, "\n%s=== Compilation Summary ===%s\n",
            get_color(COLOR_BOLD_WHITE),
            get_color(COLOR_RESET));
    
    if (global_error_state.error_count > 0) {
        fprintf(out, "%s%d error(s)%s",
                get_color(COLOR_BOLD_RED),
                global_error_state.error_count,
                get_color(COLOR_RESET));
    } else {
        fprintf(out, "%s0 errors%s",
                get_color(COLOR_BOLD_GREEN),
                get_color(COLOR_RESET));
    }
    
    if (global_error_state.warning_count > 0) {
        fprintf(out, ", %s%d warning(s)%s",
                get_color(COLOR_BOLD_YELLOW),
                global_error_state.warning_count,
                get_color(COLOR_RESET));
    }
    
    // Show recovery information if we attempted recovery
    if (recovery_count > 0) {
        fprintf(out, " (%s%d recovery attempt(s)%s)",
                get_color(COLOR_CYAN),
                recovery_count,
                get_color(COLOR_RESET));
    }
    
    fprintf(out, "\n");
    
    if (global_error_state.error_count > 0) {
        fprintf(out, "%sCompilation FAILED%s\n",
                get_color(COLOR_BOLD_RED),
                get_color(COLOR_RESET));
    }
    
    fflush(out);
}

// ============================================================================
// Phase 6: Spell Checking / Did You Mean
// ============================================================================

// Common MELP keywords for spell checking
const char* MELP_KEYWORDS[] = {
    "function", "end", "return", "returns",
    "numeric", "string", "boolean", "list", "tuple",
    "if", "then", "else", "while", "for", "to", "downto", "each", "in",
    "true", "false", "and", "or", "not", "xor", "mod",
    "exit", "print", "struct"
};
const int MELP_KEYWORD_COUNT = sizeof(MELP_KEYWORDS) / sizeof(MELP_KEYWORDS[0]);

// Calculate minimum of three integers
static int min3(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

// Calculate Levenshtein distance between two strings
int error_levenshtein_distance(const char* s1, const char* s2) {
    if (!s1 || !s2) return -1;
    
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    
    // Handle empty strings
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;
    
    // Limit to reasonable string lengths
    if (len1 > 100 || len2 > 100) return 100;
    
    // Use two rows instead of full matrix for memory efficiency
    int* prev = malloc((len2 + 1) * sizeof(int));
    int* curr = malloc((len2 + 1) * sizeof(int));
    
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return -1;
    }
    
    // Initialize first row
    for (int j = 0; j <= len2; j++) {
        prev[j] = j;
    }
    
    // Fill in the rest
    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            curr[j] = min3(
                prev[j] + 1,      // deletion
                curr[j-1] + 1,    // insertion
                prev[j-1] + cost  // substitution
            );
        }
        
        // Swap rows
        int* temp = prev;
        prev = curr;
        curr = temp;
    }
    
    int result = prev[len2];
    free(prev);
    free(curr);
    
    return result;
}

// Find closest match from a list of candidates
const char* error_find_similar(const char* input, const char** candidates, int count, int max_distance) {
    if (!input || !candidates || count <= 0) return NULL;
    
    const char* best_match = NULL;
    int best_distance = max_distance + 1;
    
    for (int i = 0; i < count; i++) {
        int dist = error_levenshtein_distance(input, candidates[i]);
        if (dist >= 0 && dist < best_distance) {
            best_distance = dist;
            best_match = candidates[i];
        }
    }
    
    return best_match;
}

// ============================================================================
// Phase 6: Error Recovery Functions
// ============================================================================

int error_in_recovery_mode(void) {
    return recovery_mode;
}

void error_enter_recovery(void) {
    recovery_mode = 1;
}

void error_exit_recovery(void) {
    recovery_mode = 0;
}

int error_should_recover(void) {
    // Don't try to recover if we've had too many errors
    if (global_error_state.max_errors > 0 && 
        global_error_state.error_count >= global_error_state.max_errors) {
        return 0;
    }
    // Limit recovery attempts to prevent infinite loops
    if (recovery_count >= 20) {
        return 0;
    }
    return 1;
}

void error_mark_recovered(void) {
    recovery_count++;
    recovery_mode = 0;
}

int error_get_recovery_count(void) {
    return recovery_count;
}

// ============================================================================
// Phase 11: Error Context Management (YZ_37)
// ============================================================================

// Save current error context and return context ID
int error_save_context(void) {
    if (context_stack_top >= MAX_CONTEXT_STACK) {
        error_fatal("Error context stack overflow (max %d)", MAX_CONTEXT_STACK);
        return -1;
    }
    
    // Save current context
    context_stack[context_stack_top].source = global_error_state.source;
    context_stack[context_stack_top].filename = global_error_state.filename;
    
    int context_id = context_stack_top;
    context_stack_top++;
    
    return context_id;
}

// Restore error context from saved state
void error_restore_context(int context_id) {
    if (context_id < 0 || context_id >= context_stack_top) {
        error_fatal("Invalid error context ID: %d", context_id);
        return;
    }
    
    // Restore context
    global_error_state.source = context_stack[context_id].source;
    global_error_state.filename = context_stack[context_id].filename;
    
    // Pop stack back to this level
    context_stack_top = context_id;
}
