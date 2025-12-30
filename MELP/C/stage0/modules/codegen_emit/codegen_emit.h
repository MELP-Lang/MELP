#ifndef CODEGEN_EMIT_H
#define CODEGEN_EMIT_H

#include <stdio.h>
#include <stdarg.h>

// ============================================================================
// C Code Emission Module
// ============================================================================
// Purpose: Generate C code instead of assembly
// Part of: MLP-GCC Self-Hosting Phase 0
// Target: GCC C99

// Global output file for C code generation
extern FILE* c_output_file;

// Initialize C code generator
void emit_c_init(FILE* output);

// Emit C code with printf-style formatting
void emit_c(const char* format, ...);

// Emit C code with indentation
void emit_c_indent(int level, const char* format, ...);

// Cleanup C code generator
void emit_c_finalize(void);

// Helper: Set current output file
void emit_c_set_output(FILE* output);

// Helper: Get current output file
FILE* emit_c_get_output(void);

#endif // CODEGEN_EMIT_H
