#include "enum_codegen.h"

// ============================================================================
// YZ_96: Enum Codegen Implementation
// ============================================================================
// Enums are compile-time constants. No runtime code needed.
// This file generates optional comments for debugging.

void enum_codegen_comment(FILE* output, EnumDefinition* def) {
    if (!output || !def) return;
    
    fprintf(output, "\n    # Enum: %s\n", def->name);
    
    EnumValue* val = def->values;
    while (val) {
        fprintf(output, "    #   %s.%s = %ld\n", def->name, val->name, val->value);
        val = val->next;
    }
}
