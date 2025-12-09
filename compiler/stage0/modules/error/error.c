#include "error.h"
#include <string.h>
#include <time.h>

// Global error state
static ErrorState global_error_state = {
    .error_count = 0,
    .warning_count = 0,
    .max_errors = 10,  // Stop after 10 errors by default
    .output = NULL     // Will be set to stderr in error_init()
};

// Category names for display
static const char* category_names[] = {
    "Lexer",
    "Parser",
    "Semantic",
    "Codegen",
    "I/O",
    "Memory",
    "Internal"
};

// Level names for display
static const char* level_names[] = {
    "warning",
    "error",
    "fatal error"
};

void error_init(void) {
    global_error_state.error_count = 0;
    global_error_state.warning_count = 0;
    global_error_state.output = stderr;
}

void error_set_output(FILE* output) {
    if (output != NULL) {
        global_error_state.output = output;
    }
}

void error_set_max_errors(int max) {
    global_error_state.max_errors = max;
}

void error_report(ErrorLevel level, ErrorCategory category, 
                 int line, const char* file, const char* format, ...) {
    // Initialize if not done yet
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else {
        global_error_state.error_count++;
    }

    // Format: [Category] level: message
    fprintf(out, "[%s] %s", 
            category_names[category],
            level_names[level]);
    
    if (line > 0) {
        fprintf(out, " at line %d", line);
    }
    
    fprintf(out, ": ");
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    fflush(out);

    // Stop compilation if too many errors
    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        fprintf(out, "\n*** Compilation stopped due to errors ***\n");
        fprintf(out, "Errors: %d, Warnings: %d\n", 
                global_error_state.error_count,
                global_error_state.warning_count);
        exit(1);
    }
}

void error_report_detailed(ErrorLevel level, ErrorCategory category,
                          int line, int column, const char* file,
                          const char* format, ...) {
    // Initialize if not done yet
    if (global_error_state.output == NULL) {
        error_init();
    }

    FILE* out = global_error_state.output;
    
    // Count errors and warnings
    if (level == ERROR_LEVEL_WARNING) {
        global_error_state.warning_count++;
    } else {
        global_error_state.error_count++;
    }

    // Format: [Category] level at line:column: message
    fprintf(out, "[%s] %s", 
            category_names[category],
            level_names[level]);
    
    if (line > 0) {
        fprintf(out, " at line %d", line);
        if (column > 0) {
            fprintf(out, ", column %d", column);
        }
    }
    
    fprintf(out, ": ");
    
    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    fprintf(out, "\n");
    fflush(out);

    // Stop compilation if too many errors
    if (level == ERROR_LEVEL_FATAL || error_should_stop()) {
        fprintf(out, "\n*** Compilation stopped due to errors ***\n");
        fprintf(out, "Errors: %d, Warnings: %d\n", 
                global_error_state.error_count,
                global_error_state.warning_count);
        exit(1);
    }
}

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
}
