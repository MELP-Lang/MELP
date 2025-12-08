#include "comparison_codegen.h"
#include "../functions/functions.h"  // For variable offset lookup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Load value into register
static void load_value(FILE* output, const char* value, int is_literal, int reg_num, int is_float, void* context) {
    if (is_float) {
        if (is_literal) {
            fprintf(output, "    section .data\n");
            fprintf(output, "    _cmp_float_%d: dq %s\n", reg_num, value);
            fprintf(output, "    section .text\n");
            fprintf(output, "    movsd xmm%d, [_cmp_float_%d]\n", reg_num, reg_num);
        } else {
            fprintf(output, "    movsd xmm%d, [%s]\n", reg_num, value);
        }
    } else {
        if (is_literal) {
            fprintf(output, "    mov r%d, %s\n", reg_num + 8, value);
        } else {
            // Variable - lookup offset
            if (context) {
                FunctionDeclaration* func = (FunctionDeclaration*)context;
                int offset = function_get_var_offset(func, value);
                fprintf(output, "    movq %d(%%rbp), %%r%d  ; Load %s\n", offset, reg_num + 8, value);
            } else {
                fprintf(output, "    mov r%d, [%s]\n", reg_num + 8, value);
            }
        }
    }
}

// Generate comparison code
void comparison_generate_code(FILE* output, ComparisonExpr* expr, void* context) {
    if (!output || !expr) return;
    
    fprintf(output, "\n    ; Comparison expression\n");
    
    // Load left operand into r8 (or xmm0)
    load_value(output, expr->left_value, expr->left_is_literal, 0, expr->is_float, context);
    
    // Load right operand into r9 (or xmm1)
    load_value(output, expr->right_value, expr->right_is_literal, 1, expr->is_float, context);
    
    // Compare
    if (expr->is_float) {
        fprintf(output, "    ucomisd xmm0, xmm1\n");
    } else {
        fprintf(output, "    cmp r8, r9\n");
    }
    
    // Set result based on comparison
    fprintf(output, "    mov rax, 0          ; Default false\n");
    
    switch (expr->op) {
        case CMP_EQUAL:
            fprintf(output, "    sete al             ; Set if equal\n");
            break;
        case CMP_NOT_EQUAL:
            fprintf(output, "    setne al            ; Set if not equal\n");
            break;
        case CMP_LESS:
            fprintf(output, "    setl al             ; Set if less\n");
            break;
        case CMP_LESS_EQUAL:
            fprintf(output, "    setle al            ; Set if less or equal\n");
            break;
        case CMP_GREATER:
            fprintf(output, "    setg al             ; Set if greater\n");
            break;
        case CMP_GREATER_EQUAL:
            fprintf(output, "    setge al            ; Set if greater or equal\n");
            break;
    }
    
    fprintf(output, "    ; Result in rax (0=false, 1=true)\n");
}

// Generate conditional jump
void comparison_generate_conditional_jump(FILE* output, ComparisonExpr* expr, const char* label, void* context) {
    if (!output || !expr || !label) return;
    
    fprintf(output, "\n    ; Conditional jump\n");
    
    // Load operands
    load_value(output, expr->left_value, expr->left_is_literal, 0, expr->is_float, context);
    load_value(output, expr->right_value, expr->right_is_literal, 1, expr->is_float, context);
    
    // Compare
    if (expr->is_float) {
        fprintf(output, "    ucomisd xmm0, xmm1\n");
    } else {
        fprintf(output, "    cmp r8, r9\n");
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
