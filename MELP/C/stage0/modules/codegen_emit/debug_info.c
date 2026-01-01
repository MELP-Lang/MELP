#include "debug_info.h"
#include <stdlib.h>
#include <string.h>

// Initialize debug context
void debug_init(DebugContext* ctx, FILE* output, const char* source_file, bool enabled) {
    ctx->enabled = enabled;
    ctx->source_file = source_file ? strdup(source_file) : NULL;
    ctx->current_line = 0;
    ctx->output = output;
    
    if (enabled && output && source_file) {
        // Emit initial debug information
        fprintf(output, "// Debug info enabled for: %s\n", source_file);
        fprintf(output, "// Compile with: gcc -g for full DWARF support\n");
        fprintf(output, "\n");
    }
}

// Emit DWARF line information (#line directives)
void emit_dwarf_line_info(DebugContext* ctx, int mlp_line) {
    if (!ctx || !ctx->enabled || !ctx->output || !ctx->source_file) {
        return;
    }
    
    // Only emit if line changed
    if (ctx->current_line != mlp_line) {
        ctx->current_line = mlp_line;
        
        // Emit #line directive for GDB/DWARF support
        // This maps the generated C line back to the MLP source line
        fprintf(ctx->output, "#line %d \"%s\"\n", mlp_line, ctx->source_file);
    }
}

// Emit variable debug information (as comments for GDB)
void emit_dwarf_variable_info(DebugContext* ctx, const char* var_name, const char* type) {
    if (!ctx || !ctx->enabled || !ctx->output || !var_name) {
        return;
    }
    
    // Emit debug comment that GDB can parse
    // Format: /* DWARF: var <name> : <type> @ line <line> */
    fprintf(ctx->output, "/* DWARF: var %s : %s @ line %d */\n", 
            var_name, type ? type : "unknown", ctx->current_line);
}

// Emit function debug information
void emit_dwarf_function_info(DebugContext* ctx, const char* func_name, int start_line) {
    if (!ctx || !ctx->enabled || !ctx->output || !func_name) {
        return;
    }
    
    // Update line and emit function boundary info
    emit_dwarf_line_info(ctx, start_line);
    
    // Emit function debug info as comment
    fprintf(ctx->output, "/* DWARF: function %s starts @ line %d */\n", 
            func_name, start_line);
}

// Emit scope debug information
void emit_dwarf_scope_start(DebugContext* ctx, const char* scope_type) {
    if (!ctx || !ctx->enabled || !ctx->output) {
        return;
    }
    
    // Mark scope entry (for variable lifetime tracking)
    fprintf(ctx->output, "/* DWARF: scope %s enter @ line %d */\n", 
            scope_type ? scope_type : "block", ctx->current_line);
}

void emit_dwarf_scope_end(DebugContext* ctx) {
    if (!ctx || !ctx->enabled || !ctx->output) {
        return;
    }
    
    // Mark scope exit
    fprintf(ctx->output, "/* DWARF: scope exit @ line %d */\n", ctx->current_line);
}

// Update current line number
void debug_set_line(DebugContext* ctx, int line) {
    if (ctx) {
        ctx->current_line = line;
    }
}

// Cleanup debug context
void debug_finalize(DebugContext* ctx) {
    if (!ctx) {
        return;
    }
    
    if (ctx->source_file) {
        free((void*)ctx->source_file);
        ctx->source_file = NULL;
    }
    
    ctx->output = NULL;
    ctx->enabled = false;
    ctx->current_line = 0;
}

// Advanced debug info functions for DWARF generation

// Emit debug info for array variables
void emit_dwarf_array_info(DebugContext* ctx, const char* var_name, const char* element_type, int size) {
    if (!ctx || !ctx->enabled || !ctx->output || !var_name) {
        return;
    }
    
    fprintf(ctx->output, "/* DWARF: array %s[%d] : %s @ line %d */\n", 
            var_name, size, element_type ? element_type : "unknown", ctx->current_line);
}

// Emit debug info for struct/type definitions
void emit_dwarf_type_info(DebugContext* ctx, const char* type_name, const char* type_kind) {
    if (!ctx || !ctx->enabled || !ctx->output || !type_name) {
        return;
    }
    
    fprintf(ctx->output, "/* DWARF: type %s kind=%s @ line %d */\n", 
            type_name, type_kind ? type_kind : "struct", ctx->current_line);
}

// Emit debug info for parameter variables
void emit_dwarf_parameter_info(DebugContext* ctx, const char* param_name, const char* type, int position) {
    if (!ctx || !ctx->enabled || !ctx->output || !param_name) {
        return;
    }
    
    fprintf(ctx->output, "/* DWARF: param %s : %s (pos %d) @ line %d */\n", 
            param_name, type ? type : "unknown", position, ctx->current_line);
}

// Emit breakpoint hint for debugger
void emit_dwarf_breakpoint_hint(DebugContext* ctx, const char* label) {
    if (!ctx || !ctx->enabled || !ctx->output) {
        return;
    }
    
    // This helps debuggers set breakpoints at logical MLP statement boundaries
    fprintf(ctx->output, "/* DWARF: breakpoint-hint %s @ line %d */\n", 
            label ? label : "stmt", ctx->current_line);
}

// Emit source correlation mapping (MLP line -> C line)
void emit_source_map_entry(DebugContext* ctx, int mlp_line, int c_line) {
    if (!ctx || !ctx->enabled || !ctx->output) {
        return;
    }
    
    // This is used by source mapper tool
    fprintf(ctx->output, "/* SOURCEMAP: mlp:%d -> c:%d */\n", mlp_line, c_line);
}

// Emit debug info for constant/literal values
void emit_dwarf_constant_info(DebugContext* ctx, const char* name, const char* value, const char* type) {
    if (!ctx || !ctx->enabled || !ctx->output || !name) {
        return;
    }
    
    fprintf(ctx->output, "/* DWARF: const %s = %s : %s @ line %d */\n", 
            name, value ? value : "?", type ? type : "unknown", ctx->current_line);
}

// Emit file-level debug initialization
void emit_dwarf_file_info(DebugContext* ctx) {
    if (!ctx || !ctx->enabled || !ctx->output || !ctx->source_file) {
        return;
    }
    
    // Emit compilation unit debug info
    fprintf(ctx->output, "/* DWARF: compilation-unit %s */\n", ctx->source_file);
    fprintf(ctx->output, "/* DWARF: producer MLP-GCC-Stage0 */\n");
    fprintf(ctx->output, "/* DWARF: language MLP */\n");
}

// Check if debug is enabled
bool debug_is_enabled(DebugContext* ctx) {
    return ctx && ctx->enabled;
}

// Get current line
int debug_get_current_line(DebugContext* ctx) {
    return ctx ? ctx->current_line : 0;
}

// Get source file
const char* debug_get_source_file(DebugContext* ctx) {
    return ctx ? ctx->source_file : NULL;
}
