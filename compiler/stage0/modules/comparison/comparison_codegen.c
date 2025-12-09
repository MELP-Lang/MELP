#include "comparison_codegen.h"
#include "../functions/functions.h"  // For variable offset lookup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        fprintf(output, "    call mlp_string_compare  # Returns <0, 0, or >0\n");
        fprintf(output, "    # Compare result: <0 (less), 0 (equal), >0 (greater)\n");
        fprintf(output, "    cmpq $0, %%rax\n");
    }
    // Compare
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Set result based on comparison
    fprintf(output, "    movq $0, %%rax          # Default false\n");
    
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
        fprintf(output, "    cmpq $0, %%rax  # Compare result with 0\n");
    }
    // Compare
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Jump based on condition
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
        fprintf(output, "    call mlp_string_compare  # Returns <0, 0, or >0\n");
        fprintf(output, "    # Compare result: <0 (less), 0 (equal), >0 (greater)\n");
        fprintf(output, "    cmpq $0, %%rax\n");
    }
    // Compare numeric values
    else if (expr->is_float) {
        fprintf(output, "    ucomisd %%xmm1, %%xmm0\n");
    } else {
        fprintf(output, "    cmpq %%r9, %%r8\n");
    }
    
    // Set result for first comparison
    fprintf(output, "    movq $0, %%rax\n");
    switch (expr->op) {
        case CMP_EQUAL:       fprintf(output, "    sete %%al\n"); break;
        case CMP_NOT_EQUAL:   fprintf(output, "    setne %%al\n"); break;
        case CMP_LESS:        fprintf(output, "    setl %%al\n"); break;
        case CMP_LESS_EQUAL:  fprintf(output, "    setle %%al\n"); break;
        case CMP_GREATER:     fprintf(output, "    setg %%al\n"); break;
        case CMP_GREATER_EQUAL: fprintf(output, "    setge %%al\n"); break;
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
