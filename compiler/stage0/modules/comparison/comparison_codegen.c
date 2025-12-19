#include "comparison_codegen.h"
#include "../functions/functions.h"  // For variable offset lookup
#include "../enum/enum.h"  // YZ_35: For enum value access
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // YZ_29: For isdigit()

// ✅ Phase 3.2: Label counter for logical short-circuit
static int logical_label_counter = 0;

// YZ_11: Label counter for string literals in comparisons
static int string_literal_counter = 0;

// Load value into register
// YZ_11: Enhanced to handle string literals properly (added is_string parameter)
static void load_value(FILE* output, const char* value, int is_literal, int reg_num, int is_float, int is_string, void* context) {
    if (is_float) {
        if (is_literal) {
            fprintf(output, "    .section .data\n");
            fprintf(output, "    _cmp_float_%d: .quad %s\n", reg_num, value);
            fprintf(output, "    .section .text\n");
            fprintf(output, "    movsd _cmp_float_%d(%%rip), %%xmm%d\n", reg_num, reg_num);
        } else {
            fprintf(output, "    movsd (%s), %%xmm%d\n", value, reg_num);
        }
    } else if (is_string && is_literal) {
        // YZ_11: String literal - create .rodata label and load address
        // Note: lexer strips quotes, so value is just the content (e.g., "secret" -> secret)
        int str_id = string_literal_counter++;
        fprintf(output, "\n.section .rodata\n");
        fprintf(output, ".str_cmp_%d:\n", str_id);
        fprintf(output, "    .string \"%s\"\n", value);  // Add quotes back
        fprintf(output, ".text\n");
        fprintf(output, "    leaq .str_cmp_%d(%%rip), %%r%d  # Load string address\n", str_id, reg_num + 8);
    } else {
        if (is_literal) {
            // Numeric literal
            fprintf(output, "    movq $%s, %%r%d  # Literal\n", value, reg_num + 8);
        } else {
            // YZ_29: Check for member access (variable.member) or array access (variable[index])
            const char* dot = strchr(value, '.');
            const char* lbracket = strchr(value, '[');
            
            if (dot && (!lbracket || dot < lbracket)) {
                // Member access detected (and comes before any array access)
                size_t base_len = dot - value;
                char* base_var = malloc(base_len + 1);
                strncpy(base_var, value, base_len);
                base_var[base_len] = '\0';
                const char* member_name = dot + 1;
                
                // YZ_35: Check if this is an enum value access (e.g., Status.ACTIVE)
                int64_t enum_val = enum_lookup_value(base_var, member_name);
                if (enum_val != -1) {
                    fprintf(output, "    # YZ_35: Enum value: %s.%s = %ld\n", base_var, member_name, enum_val);
                    fprintf(output, "    movq $%ld, %%r%d  # Load enum value\n", enum_val, reg_num + 8);
                    free(base_var);
                    return;
                }
                
                // Currently only support .length member
                if (strcmp(member_name, "length") == 0) {
                    // Load list/array length
                    // List structure: [capacity (8 bytes), size (8 bytes), data pointer (8 bytes)]
                    // Length is at offset +8 from base
                    if (context) {
                        FunctionDeclaration* func = (FunctionDeclaration*)context;
                        int offset = function_get_var_offset(func, base_var);
                        fprintf(output, "    movq %d(%%rbp), %%r%d  # Load %s address\n", offset, reg_num + 8, base_var);
                        fprintf(output, "    movq 8(%%r%d), %%r%d  # Load .length\n", reg_num + 8, reg_num + 8);
                    } else {
                        fprintf(output, "    movq (%s), %%r%d  # Load %s address\n", base_var, reg_num + 8, base_var);
                        fprintf(output, "    movq 8(%%r%d), %%r%d  # Load .length\n", reg_num + 8, reg_num + 8);
                    }
                } else {
                    // Unsupported member - fallback to variable lookup
                    fprintf(output, "    # Warning: Unsupported member access: %s\n", value);
                    if (context) {
                        FunctionDeclaration* func = (FunctionDeclaration*)context;
                        int offset = function_get_var_offset(func, value);
                        fprintf(output, "    movq %d(%%rbp), %%r%d  # Load %s\n", offset, reg_num + 8, value);
                    } else {
                        fprintf(output, "    movq (%s), %%r%d\n", value, reg_num + 8);
                    }
                }
                
                free(base_var);
            } else if (lbracket) {
                // Array access detected: variable[index]
                size_t base_len = lbracket - value;
                char* base_var = malloc(base_len + 1);
                strncpy(base_var, value, base_len);
                base_var[base_len] = '\0';
                
                // Extract index (between [ and ])
                const char* rbracket = strchr(lbracket, ']');
                if (rbracket) {
                    size_t index_len = rbracket - lbracket - 1;
                    char* index_str = malloc(index_len + 1);
                    strncpy(index_str, lbracket + 1, index_len);
                    index_str[index_len] = '\0';
                    
                    // Load array element
                    // Array structure: [capacity, size, data_pointer]
                    // Element at index i: data_pointer + (i * 8)
                    if (context) {
                        FunctionDeclaration* func = (FunctionDeclaration*)context;
                        int base_offset = function_get_var_offset(func, base_var);
                        
                        // Load array base address
                        fprintf(output, "    movq %d(%%rbp), %%rax  # Load %s address\n", base_offset, base_var);
                        
                        // Load data pointer (offset +16 in array structure)
                        fprintf(output, "    movq 16(%%rax), %%rax  # Load data pointer\n");
                        
                        // Check if index is literal or variable
                        if (isdigit(index_str[0]) || (index_str[0] == '-' && isdigit(index_str[1]))) {
                            // Literal index
                            long index_val = atol(index_str);
                            fprintf(output, "    movq %ld(%%rax), %%r%d  # Load %s[%s]\n", 
                                    index_val * 8, reg_num + 8, base_var, index_str);
                        } else {
                            // Variable index
                            int index_offset = function_get_var_offset(func, index_str);
                            fprintf(output, "    movq %d(%%rbp), %%rcx  # Load index %s\n", index_offset, index_str);
                            fprintf(output, "    shl $3, %%rcx  # Multiply by 8\n");
                            fprintf(output, "    addq %%rcx, %%rax  # Add offset\n");
                            fprintf(output, "    movq (%%rax), %%r%d  # Load %s[%s]\n", reg_num + 8, base_var, index_str);
                        }
                    } else {
                        fprintf(output, "    # Error: Array access without context: %s\n", value);
                        fprintf(output, "    movq $0, %%r%d\n", reg_num + 8);
                    }
                    
                    free(index_str);
                } else {
                    // Malformed array access
                    fprintf(output, "    # Error: Malformed array access: %s\n", value);
                    fprintf(output, "    movq $0, %%r%d\n", reg_num + 8);
                }
                
                free(base_var);
            } else {
                // Variable - lookup offset
                if (context) {
                    FunctionDeclaration* func = (FunctionDeclaration*)context;
                    int offset = function_get_var_offset(func, value);
                    fprintf(output, "    movq %d(%%rbp), %%r%d  # Load %s\n", offset, reg_num + 8, value);
                } else {
                    fprintf(output, "    movq (%s), %%r%d\n", value, reg_num + 8);
                }
            }
        }
    }
}

// Generate comparison code
void comparison_generate_code(FILE* output, ComparisonExpr* expr, void* context) {
    if (!output || !expr) return;
    
    fprintf(output, "\n    ; Comparison expression\n");
    
    // Load left operand into r8 (or xmm0)
    load_value(output, expr->left_value, expr->left_is_literal, 0, expr->is_float, expr->is_string, context);
    
    // Load right operand into r9 (or xmm1)
    load_value(output, expr->right_value, expr->right_is_literal, 1, expr->is_float, expr->is_string, context);
    
    // YZ_07: Handle string comparison
    if (expr->is_string) {
        fprintf(output, "    # String comparison (text vs text)\n");
        fprintf(output, "    movq %%r8, %%rdi  # arg1: first string\n");
        fprintf(output, "    movq %%r9, %%rsi  # arg2: second string\n");
        fprintf(output, "    call mlp_string_compare  # Returns <0, 0, or >0 (int/32-bit)\n");
        fprintf(output, "    cmpl $0, %%eax  # Compare result with 0 (YZ_69: use eax for int)\n");
    }
    // Compare
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Set result based on comparison (for non-string comparisons)
    if (!expr->is_string) {
        fprintf(output, "    movq $0, %%rax          # Default false\n");
    }
    
    // YZ_69: String comparison - use sign-based set instructions after cmpl $0
    if (expr->is_string) {
        switch (expr->op) {
            case CMP_EQUAL:
                fprintf(output, "    sete %%al             # Set if equal (ZF=1)\n");
                break;
            case CMP_NOT_EQUAL:
                fprintf(output, "    setne %%al            # Set if not equal (ZF=0)\n");
                break;
            case CMP_LESS:
                fprintf(output, "    sets %%al             # Set if sign (eax < 0, SF=1)\n");
                break;
            case CMP_LESS_EQUAL:
                fprintf(output, "    setle %%al            # Set if <= (ZF=1 or SF=1)\n");
                break;
            case CMP_GREATER:
                fprintf(output, "    setg %%al             # Set if > (ZF=0 and SF=0)\n");
                break;
            case CMP_GREATER_EQUAL:
                fprintf(output, "    setns %%al            # Set if not sign (eax >= 0, SF=0)\n");
                break;
        }
        fprintf(output, "    movzbl %%al, %%eax        # Zero-extend al to eax (YZ_69)\n");
        fprintf(output, "    movslq %%eax, %%rax       # Sign-extend eax to rax\n");
    } else {
        switch (expr->op) {
            case CMP_EQUAL:
                fprintf(output, "    sete %%al             # Set if equal\n");
                break;
            case CMP_NOT_EQUAL:
                fprintf(output, "    setne %%al            # Set if not equal\n");
                break;
            case CMP_LESS:
                fprintf(output, "    setl %%al             # Set if less\n");
                break;
            case CMP_LESS_EQUAL:
                fprintf(output, "    setle %%al            # Set if less or equal\n");
                break;
            case CMP_GREATER:
                fprintf(output, "    setg %%al             # Set if greater\n");
                break;
            case CMP_GREATER_EQUAL:
                fprintf(output, "    setge %%al            # Set if greater or equal\n");
                break;
        }
    }
    
    fprintf(output, "    # Result in rax (0=false, 1=true)\n");
}

// Generate conditional jump
void comparison_generate_conditional_jump(FILE* output, ComparisonExpr* expr, const char* label, void* context) {
    if (!output || !expr || !label) return;
    
    fprintf(output, "\n    ; Conditional jump\n");
    
    // Load operands
    load_value(output, expr->left_value, expr->left_is_literal, 0, expr->is_float, expr->is_string, context);
    load_value(output, expr->right_value, expr->right_is_literal, 1, expr->is_float, expr->is_string, context);
    
    // YZ_07: Handle string comparison
    if (expr->is_string) {
        fprintf(output, "    # String comparison for conditional\n");
        fprintf(output, "    movq %%r8, %%rdi  # arg1: first string\n");
        fprintf(output, "    movq %%r9, %%rsi  # arg2: second string\n");
        fprintf(output, "    call mlp_string_compare\n");
        fprintf(output, "    cmpl $0, %%eax  # Compare result with 0 (YZ_69: use eax for int)\n");
    }
    // Compare
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Jump based on condition
    // YZ_67: String comparison uses cmp so we can use normal jump instructions
    if (expr->is_string) {
        switch (expr->op) {
            case CMP_EQUAL:
                fprintf(output, "    je %s  # Jump if equal (rax == 0)\n", label);
                break;
            case CMP_NOT_EQUAL:
                fprintf(output, "    jne %s  # Jump if not equal (rax != 0)\n", label);
                break;
            case CMP_LESS:
                fprintf(output, "    jl %s  # Jump if less (rax < 0)\n", label);
                break;
            case CMP_LESS_EQUAL:
                fprintf(output, "    jle %s  # Jump if <= (rax <= 0)\n", label);
                break;
            case CMP_GREATER:
                fprintf(output, "    jg %s  # Jump if > (rax > 0)\n", label);
                break;
            case CMP_GREATER_EQUAL:
                fprintf(output, "    jge %s  # Jump if >= (rax >= 0)\n", label);
                break;
        }
    } else {
        switch (expr->op) {
            case CMP_EQUAL:
                fprintf(output, "    je %s\n", label);
                break;
            case CMP_NOT_EQUAL:
                fprintf(output, "    jne %s\n", label);
                break;
            case CMP_LESS:
                fprintf(output, "    jl %s\n", label);
                break;
            case CMP_LESS_EQUAL:
                fprintf(output, "    jle %s\n", label);
                break;
            case CMP_GREATER:
                fprintf(output, "    jg %s\n", label);
                break;
            case CMP_GREATER_EQUAL:
                fprintf(output, "    jge %s\n", label);
                break;
        }
    }
}

// ✅ Phase 3.2: Generate comparison with logical chaining and short-circuit evaluation
// YZ_11: Enhanced with string comparison support
void comparison_generate_code_with_chain(FILE* output, ComparisonExpr* expr, void* context) {
    if (!output || !expr) return;
    
    // Generate first comparison
    fprintf(output, "\n    # Comparison with logical chaining\n");
    
    // Load and compare first expression
    load_value(output, expr->left_value, expr->left_is_literal, 0, expr->is_float, expr->is_string, context);
    load_value(output, expr->right_value, expr->right_is_literal, 1, expr->is_float, expr->is_string, context);
    
    // YZ_11: Handle string comparison (same as comparison_generate_code)
    if (expr->is_string) {
        fprintf(output, "    # String comparison (text vs text)\n");
        fprintf(output, "    movq %%r8, %%rdi  # arg1: first string\n");
        fprintf(output, "    movq %%r9, %%rsi  # arg2: second string\n");
        fprintf(output, "    call mlp_string_compare  # Returns <0, 0, or >0 (int/32-bit)\n");
        fprintf(output, "    cmpl $0, %%eax  # Compare result with 0 (YZ_69: use eax for int)\n");
    }
    // Compare numeric values
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Set result for first comparison (for non-string comparisons)
    if (!expr->is_string) {
        fprintf(output, "    movq $0, %%rax\n");
    }
    
    // YZ_69: String comparison - use sign-based set instructions after cmpl $0
    if (expr->is_string) {
        switch (expr->op) {
            case CMP_EQUAL:       fprintf(output, "    sete %%al\n"); break;
            case CMP_NOT_EQUAL:   fprintf(output, "    setne %%al\n"); break;
            case CMP_LESS:        fprintf(output, "    sets %%al\n"); break;  // SF=1 (negative)
            case CMP_LESS_EQUAL:  fprintf(output, "    setle %%al\n"); break;
            case CMP_GREATER:     fprintf(output, "    setg %%al\n"); break;
            case CMP_GREATER_EQUAL: fprintf(output, "    setns %%al\n"); break; // SF=0 (non-negative)
        }
        fprintf(output, "    movzbl %%al, %%eax        # Zero-extend al to eax (YZ_69)\n");
        fprintf(output, "    movslq %%eax, %%rax       # Sign-extend eax to rax\n");
    } else {
        switch (expr->op) {
            case CMP_EQUAL:       fprintf(output, "    sete %%al\n"); break;
            case CMP_NOT_EQUAL:   fprintf(output, "    setne %%al\n"); break;
            case CMP_LESS:        fprintf(output, "    setl %%al\n"); break;
            case CMP_LESS_EQUAL:  fprintf(output, "    setle %%al\n"); break;
            case CMP_GREATER:     fprintf(output, "    setg %%al\n"); break;
            case CMP_GREATER_EQUAL: fprintf(output, "    setge %%al\n"); break;
        }
    }
    
    // Handle logical chaining
    if (expr->chain_op != LOG_NONE && expr->next) {
        int label_id = logical_label_counter++;
        
        if (expr->chain_op == LOG_AND) {
            // AND: Short-circuit if first is false
            fprintf(output, "    test %%rax, %%rax       # Check first result\n");
            fprintf(output, "    jz .logical_and_false_%d  # Skip if false (short-circuit)\n", label_id);
            
            // First was true, evaluate second
            comparison_generate_code_with_chain(output, expr->next, context);
            
            // Second result already in rax
            fprintf(output, ".logical_and_false_%d:\n", label_id);
            fprintf(output, "    # AND result in rax\n");
            
        } else if (expr->chain_op == LOG_OR) {
            // OR: Short-circuit if first is true
            fprintf(output, "    test %%rax, %%rax       # Check first result\n");
            fprintf(output, "    jnz .logical_or_true_%d  # Skip if true (short-circuit)\n", label_id);
            
            // First was false, evaluate second
            comparison_generate_code_with_chain(output, expr->next, context);
            
            fprintf(output, ".logical_or_true_%d:\n", label_id);
            fprintf(output, "    # OR result in rax\n");
        }
    } else {
        fprintf(output, "    # Single comparison result in rax\n");
    }
}
