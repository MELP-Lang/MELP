#include "variable_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdlib.h>
#include <string.h>

// Create codegen
VariableCodegen* variable_codegen_create(FILE* output) {
    VariableCodegen* codegen = malloc(sizeof(VariableCodegen));
    codegen->output = output;
    codegen->data_section_active = 0;
    codegen->bss_section_active = 0;
    emit_c_set_output(output);
    return codegen;
}

// Free codegen
void variable_codegen_free(VariableCodegen* codegen) {
    free(codegen);
}

// Ensure .data section is active (not needed for C, kept for compatibility)
void variable_codegen_data_section(VariableCodegen* codegen) {
    codegen->data_section_active = 1;
    codegen->bss_section_active = 0;
}

// Ensure .bss section is active (not needed for C, kept for compatibility)
void variable_codegen_bss_section(VariableCodegen* codegen) {
    codegen->bss_section_active = 1;
    codegen->data_section_active = 0;
}

// Generate C code for variable declaration
void variable_codegen_declaration(VariableCodegen* codegen, VariableDeclaration* decl) {
    if (!codegen || !decl) return;
    
    // Type description for comment
    const char* type_desc;
    if (decl->is_pointer) {
        type_desc = "pointer";
    } else if (decl->is_array) {
        type_desc = "array";
    } else if (decl->type == VAR_NUMERIC) {
        type_desc = "numeric";
    } else if (decl->type == VAR_STRING) {
        type_desc = "text";
    } else {
        type_desc = "boolean";
    }
    
    emit_c_indent(1, "// Variable: %s (type: %s)\n", decl->name, type_desc);
    
    // Handle pointer types
    if (decl->is_pointer) {
        const char* base_type_c;
        if (decl->base_type == VAR_NUMERIC) {
            base_type_c = "int64_t*";
        } else if (decl->base_type == VAR_STRING) {
            base_type_c = "char**";
        } else {
            base_type_c = "bool*";
        }
        
        if (decl->value) {
            emit_c_indent(1, "%s var_%s = %s;\n", base_type_c, decl->name, decl->value);
        } else {
            emit_c_indent(1, "%s var_%s = NULL;\n", base_type_c, decl->name);
        }
        return;
    }
    
    // Handle array types
    if (decl->is_array) {
        const char* elem_type_c;
        if (decl->base_type == VAR_NUMERIC) {
            elem_type_c = "int64_t";
        } else if (decl->base_type == VAR_STRING) {
            elem_type_c = "char*";
        } else {
            elem_type_c = "bool";
        }
        
        if (decl->array_size > 0) {
            // Fixed-size array
            if (decl->value && decl->value[0] == '[') {
                emit_c_indent(1, "%s var_%s[%d] = %s;\n", 
                        elem_type_c, decl->name, decl->array_size, decl->value);
            } else {
                emit_c_indent(1, "%s var_%s[%d];\n", 
                        elem_type_c, decl->name, decl->array_size);
            }
        } else {
            // Dynamic array (pointer)
            emit_c_indent(1, "%s* var_%s = NULL; // Dynamic array\n", 
                    elem_type_c, decl->name);
        }
        return;
    }
    
    // Handle numeric types with STO
    if (decl->type == VAR_NUMERIC) {
        if (decl->internal_num_type == INTERNAL_INT64) {
            emit_c_indent(1, "// STO: INT64 optimization\n");
            if (decl->value) {
                emit_c_indent(1, "int64_t var_%s = %s;\n", decl->name, decl->value);
            } else {
                emit_c_indent(1, "int64_t var_%s = 0;\n", decl->name);
            }
        } else if (decl->internal_num_type == INTERNAL_DOUBLE) {
            emit_c_indent(1, "// STO: DOUBLE optimization\n");
            if (decl->value) {
                emit_c_indent(1, "double var_%s = %s;\n", decl->name, decl->value);
            } else {
                emit_c_indent(1, "double var_%s = 0.0;\n", decl->name);
            }
        } else {  // INTERNAL_BIGDECIMAL
            emit_c_indent(1, "// STO: BIGDECIMAL (large number)\n");
            emit_c_indent(1, "mlp_bigdecimal_t var_%s; // TODO: init bigdecimal\n", decl->name);
        }
        
    } else if (decl->type == VAR_STRING) {
        // String variable with STO
        if (decl->internal_str_type == INTERNAL_SSO || 
            decl->internal_str_type == INTERNAL_RODATA) {
            emit_c_indent(1, "// STO: Small/const string optimization\n");
            if (decl->value) {
                emit_c_indent(1, "const char* var_%s = \"%s\";\n", decl->name, decl->value);
            } else {
                emit_c_indent(1, "const char* var_%s = \"\";\n", decl->name);
            }
        } else {  // INTERNAL_HEAP
            emit_c_indent(1, "// STO: HEAP string (>23 chars)\n");
            if (decl->value) {
                emit_c_indent(1, "mlp_string_t var_%s = {.data = \"%s\", .len = %zu};\n", 
                        decl->name, decl->value, strlen(decl->value));
            } else {
                emit_c_indent(1, "mlp_string_t var_%s = {.data = NULL, .len = 0};\n", decl->name);
            }
        }
        
    } else if (decl->type == VAR_BOOLEAN) {
        // Boolean variable
        int bool_val = (decl->value && strcmp(decl->value, "true") == 0) ? 1 : 0;
        emit_c_indent(1, "bool var_%s = %s;\n", decl->name, bool_val ? "true" : "false");
    }
}
