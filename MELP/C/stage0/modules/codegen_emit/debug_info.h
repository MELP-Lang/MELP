#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#include <stdio.h>
#include <stdbool.h>

// Debug info configuration
typedef struct {
    bool enabled;              // Debug info enabled?
    const char* source_file;   // Original MLP source file
    int current_line;          // Current MLP line number
    FILE* output;              // C output file
} DebugContext;

// Initialize debug context
void debug_init(DebugContext* ctx, FILE* output, const char* source_file, bool enabled);

// Emit DWARF line information (#line directives)
void emit_dwarf_line_info(DebugContext* ctx, int mlp_line);

// Emit variable debug information (as comments for GDB)
void emit_dwarf_variable_info(DebugContext* ctx, const char* var_name, const char* type);

// Emit function debug information
void emit_dwarf_function_info(DebugContext* ctx, const char* func_name, int start_line);

// Emit scope debug information
void emit_dwarf_scope_start(DebugContext* ctx, const char* scope_type);
void emit_dwarf_scope_end(DebugContext* ctx);

// Update current line number
void debug_set_line(DebugContext* ctx, int line);

// Cleanup debug context
void debug_finalize(DebugContext* ctx);

#endif // DEBUG_INFO_H
