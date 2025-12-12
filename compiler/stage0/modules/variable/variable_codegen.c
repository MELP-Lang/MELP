#include "variable_codegen.h"
#include <stdlib.h>
#include <string.h>

// Simple arithmetic expression evaluator for compile-time constant folding
static double simple_evaluate(const char* expr) {
    // Very simple evaluator: "10 + 5" -> 15.0
    // Parse first number
    double result = 0;
    const char* ptr = expr;
    
    // Skip whitespace
    while (*ptr == ' ') ptr++;
    
    // Parse first number
    char num_buf[64];
    int num_pos = 0;
    while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.') {
        if (num_pos < 63) num_buf[num_pos++] = *ptr;
        ptr++;
    }
    num_buf[num_pos] = '\0';
    result = atof(num_buf);
    
    // Process operations
    while (*ptr) {
        // Skip whitespace
        while (*ptr == ' ') ptr++;
        if (!*ptr) break;
        
        // Get operator
        char op = *ptr++;
        
        // Skip whitespace
        while (*ptr == ' ') ptr++;
        
        // Parse next number
        num_pos = 0;
        while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.') {
            if (num_pos < 63) num_buf[num_pos++] = *ptr;
            ptr++;
        }
        num_buf[num_pos] = '\0';
        double operand = atof(num_buf);
        
        // Apply operation
        switch (op) {
            case '+': result += operand; break;
            case '-': result -= operand; break;
            case '*': result *= operand; break;
            case '/': result /= operand; break;
            default: break;
        }
    }
    
    return result;
}

// Create codegen
VariableCodegen* variable_codegen_create(FILE* output) {
    VariableCodegen* codegen = malloc(sizeof(VariableCodegen));
    codegen->output = output;
    codegen->data_section_active = 0;
    codegen->bss_section_active = 0;
    return codegen;
}

// Free codegen
void variable_codegen_free(VariableCodegen* codegen) {
    free(codegen);
}

// Ensure .data section is active
void variable_codegen_data_section(VariableCodegen* codegen) {
    if (!codegen->data_section_active) {
        fprintf(codegen->output, "\nsection .data\n");
        codegen->data_section_active = 1;
        codegen->bss_section_active = 0;
    }
}

// Ensure .bss section is active
void variable_codegen_bss_section(VariableCodegen* codegen) {
    if (!codegen->bss_section_active) {
        fprintf(codegen->output, "\nsection .bss\n");
        codegen->bss_section_active = 1;
        codegen->data_section_active = 0;
    }
}

// Generate assembly for variable declaration
void variable_codegen_declaration(VariableCodegen* codegen, VariableDeclaration* decl) {
    if (!codegen || !decl) return;
    
    FILE* f = codegen->output;
    
    // Type description for comment
    const char* type_desc;
    if (decl->is_pointer) {
        type_desc = "pointer";
    } else if (decl->is_array) {
        type_desc = "array";
    } else if (decl->type == VAR_NUMERIC) {
        type_desc = "numeric";
    } else if (decl->type == VAR_STRING) {
        type_desc = "string";
    } else {
        type_desc = "boolean";
    }
    
    fprintf(f, "    ; Variable: %s (type: %s)\n", decl->name, type_desc);
    
    // Handle pointer types
    if (decl->is_pointer) {
        fprintf(f, "    ; Pointer to %s\n", 
                decl->base_type == VAR_NUMERIC ? "numeric" :
                decl->base_type == VAR_STRING ? "string" : "boolean");
        variable_codegen_bss_section(codegen);
        fprintf(f, "    var_%s: resq 1  ; 64-bit pointer\n", decl->name);
        
        // If initialized, set pointer value
        if (decl->value) {
            fprintf(f, "\nsection .text\n");
            codegen->data_section_active = 0;
            codegen->bss_section_active = 0;
            fprintf(f, "    mov rax, %s\n", decl->value);
            fprintf(f, "    mov [var_%s], rax\n", decl->name);
        }
        return;
    }
    
    // Handle array types
    if (decl->is_array) {
        fprintf(f, "    ; Array of %s", 
                decl->base_type == VAR_NUMERIC ? "numeric" :
                decl->base_type == VAR_STRING ? "string" : "boolean");
        
        if (decl->array_size > 0) {
            fprintf(f, " [size: %d]\n", decl->array_size);
        } else {
            fprintf(f, " [dynamic]\n");
        }
        
        // No .bss allocation - arrays are always heap-allocated via STO runtime
        
        // Array initialization code with STO runtime
        if (decl->value && decl->value[0] == '[') {
            fprintf(f, "\n    ; Initialize array: %s = %s\n", decl->name, decl->value);
            
            // Parse array literal: [10, 20, 30] - count elements
            char* value_copy = strdup(decl->value);
            char* ptr = value_copy + 1;  // Skip '['
            int element_count = 0;
            
            // Count elements
            while (*ptr && *ptr != ']') {
                while (*ptr == ' ' || *ptr == ',') ptr++;
                if (*ptr == ']') break;
                if (*ptr == '"') {
                    ptr++;
                    while (*ptr && *ptr != '"') ptr++;
                    if (*ptr == '"') ptr++;
                } else {
                    while (*ptr && *ptr != ',' && *ptr != ']' && *ptr != ' ') ptr++;
                }
                element_count++;
                while (*ptr == ' ') ptr++;
                if (*ptr == ',') ptr++;
            }
            
            fprintf(f, "    # Allocate array with %d elements\n", element_count);
            fprintf(f, "    movq $%d, %%rdi      # count\n", element_count);
            fprintf(f, "    movq $8, %%rsi       # elem_size (8 bytes)\n");
            fprintf(f, "    call sto_array_alloc # Returns pointer in %%rax\n");
            fprintf(f, "    movq %%rax, -%d(%%rbp)  # Store array pointer for %s\n", 
                    8, decl->name);  // Use stack offset (simplified)
            
            // Initialize elements
            ptr = value_copy + 1;  // Reset to start
            int index = 0;
            
            while (*ptr && *ptr != ']') {
                while (*ptr == ' ' || *ptr == ',') ptr++;
                if (*ptr == ']') break;
                
                // Parse element value
                if (*ptr == '"') {
                    // String element - not supported in this minimal version
                    ptr++;
                    while (*ptr && *ptr != '"') ptr++;
                    if (*ptr == '"') ptr++;
                } else {
                    // Numeric element
                    char num_str[64];
                    int num_len = 0;
                    while (*ptr && *ptr != ',' && *ptr != ']' && *ptr != ' ' && num_len < 63) {
                        num_str[num_len++] = *ptr++;
                    }
                    num_str[num_len] = '\0';
                    
                    if (num_len > 0) {
                        fprintf(f, "    movq $%s, %%r8       # Element value\n", num_str);
                        fprintf(f, "    movq -%d(%%rbp), %%rbx # Load array pointer\n", 8);
                        fprintf(f, "    movq %%r8, %d(%%rbx)  # Store at index %d\n", 
                                index * 8, index);
                    }
                }
                
                index++;
            }
            
            free(value_copy);
            fprintf(f, "    # Array %s: %d elements initialized\n", decl->name, element_count);
        }
        
        return;
    }
    
    if (decl->type == VAR_NUMERIC) {
        // Numeric variable
        if (decl->internal_num_type == INTERNAL_INT64) {
            fprintf(f, "    ; STO: INT64 optimization\n");
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 1  ; 64-bit integer\n", decl->name);
            
            // Initialization in .text section
            fprintf(f, "\nsection .text\n");
            codegen->data_section_active = 0;
            codegen->bss_section_active = 0;
            
            // Check if this is an expression or simple value
            if (decl->init_expr) {
                // Arithmetic expression - evaluate at compile-time
                fprintf(f, "    ; Expression: %s\n", decl->init_expr);
                
                // Evaluate the expression
                double result = simple_evaluate(decl->init_expr);
                long long int_result = (long long)result;
                
                fprintf(f, "    ; Evaluated to: %lld\n", int_result);
                fprintf(f, "    mov rax, %lld\n", int_result);
                fprintf(f, "    mov [var_%s], rax\n", decl->name);
            } else if (decl->value) {
                // Simple literal value
                fprintf(f, "    mov rax, %s\n", decl->value);
                fprintf(f, "    mov [var_%s], rax\n", decl->name);
            }
            
        } else if (decl->internal_num_type == INTERNAL_DOUBLE) {
            fprintf(f, "    ; STO: DOUBLE optimization\n");
            variable_codegen_data_section(codegen);
            fprintf(f, "    var_%s: dq %s  ; 64-bit float\n", decl->name, decl->value);
            
        } else {  // INTERNAL_BIGDECIMAL
            fprintf(f, "    ; STO: BIGDECIMAL (large number)\n");
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 2  ; 128-bit for big decimal\n", decl->name);
        }
        
    } else if (decl->type == VAR_STRING) {
        // String variable
        if (decl->internal_str_type == INTERNAL_SSO) {
            fprintf(f, "    ; STO: SSO optimization (≤23 chars)\n");
            variable_codegen_data_section(codegen);
            fprintf(f, "    var_%s: db \"%s\", 0  ; Small string inline\n", 
                    decl->name, decl->value);
            
        } else if (decl->internal_str_type == INTERNAL_RODATA) {
            fprintf(f, "    ; STO: RODATA optimization (constant)\n");
            variable_codegen_data_section(codegen);
            fprintf(f, "    var_%s: db \"%s\", 0  ; Read-only data\n", 
                    decl->name, decl->value);
            
        } else {  // INTERNAL_HEAP
            fprintf(f, "    ; STO: HEAP allocation (>23 chars)\n");
            variable_codegen_data_section(codegen);
            fprintf(f, "    str_%s_data: db \"%s\", 0\n", decl->name, decl->value);
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 1  ; Pointer to heap string\n", decl->name);
        }
        
    } else if (decl->type == VAR_BOOLEAN) {
        // Boolean variable
        fprintf(f, "    ; Boolean: %s\n", decl->value);
        variable_codegen_bss_section(codegen);
        fprintf(f, "    var_%s: resb 1  ; 1 byte for boolean\n", decl->name);
        
        fprintf(f, "\nsection .text\n");
        codegen->data_section_active = 0;
        codegen->bss_section_active = 0;
        
        int bool_val = strcmp(decl->value, "true") == 0 ? 1 : 0;
        fprintf(f, "    mov byte [var_%s], %d\n", decl->name, bool_val);
    }
}

// Two-phase code generation: Declaration only (.bss section)
void variable_codegen_declaration_only(VariableCodegen* codegen, VariableDeclaration* decl) {
    if (!codegen || !decl) return;
    
    FILE* f = codegen->output;
    
    // Only emit .bss section declarations
    if (decl->type == VAR_NUMERIC) {
        if (decl->internal_num_type == INTERNAL_INT64) {
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 1  ; %s (INT64)\n", decl->name, decl->name);
        } else if (decl->internal_num_type == INTERNAL_DOUBLE) {
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 1  ; %s (DOUBLE)\n", decl->name, decl->name);
        } else {
            variable_codegen_bss_section(codegen);
            fprintf(f, "    var_%s: resq 2  ; %s (BIGDECIMAL)\n", decl->name, decl->name);
        }
    } else if (decl->type == VAR_STRING) {
        variable_codegen_bss_section(codegen);
        fprintf(f, "    var_%s: resq 1  ; %s (STRING)\n", decl->name, decl->name);
    } else if (decl->type == VAR_BOOLEAN) {
        variable_codegen_bss_section(codegen);
        fprintf(f, "    var_%s: resb 1  ; %s (BOOLEAN)\n", decl->name, decl->name);
    }
}

// Two-phase code generation: Initialization only (.text section)
void variable_codegen_initialization_only(VariableCodegen* codegen, VariableDeclaration* decl) {
    if (!codegen || !decl) return;
    
    FILE* f = codegen->output;
    
    // Emit initialization code
    fprintf(f, "    ; Initialize: %s\n", decl->name);
    
    if (decl->type == VAR_NUMERIC) {
        if (decl->internal_num_type == INTERNAL_INT64) {
            // Check if this is an expression or simple value
            if (decl->init_expr) {
                // Arithmetic expression - evaluate at compile-time
                double result = simple_evaluate(decl->init_expr);
                long long int_result = (long long)result;
                
                fprintf(f, "    mov rax, %lld  ; %s = %s\n", int_result, decl->name, decl->init_expr);
                fprintf(f, "    mov [var_%s], rax\n", decl->name);
            } else if (decl->value) {
                // Simple literal value
                fprintf(f, "    mov rax, %s\n", decl->value);
                fprintf(f, "    mov [var_%s], rax\n", decl->name);
            }
        }
    }
}
