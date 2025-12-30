#include "codegen_emit.h"
#include <stdlib.h>
#include <string.h>

// Global output file
FILE* c_output_file = NULL;

// Initialize C code generator
void emit_c_init(FILE* output) {
    c_output_file = output;
    
    // Emit standard C headers
    emit_c("// MLP-GCC Generated C Code\n");
    emit_c("// Target: GCC C99\n");
    emit_c("// STO: Smart Type Optimization enabled\n");
    emit_c("\n");
    emit_c("#include <stdint.h>\n");
    emit_c("#include <stdio.h>\n");
    emit_c("#include <stdlib.h>\n");
    emit_c("#include <string.h>\n");
    emit_c("#include <stdbool.h>\n");
    emit_c("\n");
    emit_c("// STO Runtime types\n");
    emit_c("typedef struct { /* bigdecimal */ } mlp_bigdecimal_t;\n");
    emit_c("typedef struct { char* data; size_t len; } mlp_string_t;\n");
    emit_c("\n");
}

// Emit C code with printf-style formatting
void emit_c(const char* format, ...) {
    if (!c_output_file) {
        fprintf(stderr, "ERROR: emit_c called before emit_c_init\n");
        return;
    }
    
    va_list args;
    va_start(args, format);
    vfprintf(c_output_file, format, args);
    va_end(args);
}

// Emit C code with indentation
void emit_c_indent(int level, const char* format, ...) {
    if (!c_output_file) {
        fprintf(stderr, "ERROR: emit_c_indent called before emit_c_init\n");
        return;
    }
    
    // Emit indentation (4 spaces per level)
    for (int i = 0; i < level; i++) {
        fprintf(c_output_file, "    ");
    }
    
    va_list args;
    va_start(args, format);
    vfprintf(c_output_file, format, args);
    va_end(args);
}

// Cleanup C code generator
void emit_c_finalize(void) {
    if (c_output_file) {
        fflush(c_output_file);
        // Don't close - caller owns the FILE*
        c_output_file = NULL;
    }
}

// Helper: Set current output file
void emit_c_set_output(FILE* output) {
    c_output_file = output;
}

// Helper: Get current output file
FILE* emit_c_get_output(void) {
    return c_output_file;
}
