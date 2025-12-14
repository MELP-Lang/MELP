#include "print_codegen.h"
#include "print.h"
#include "../functions/functions.h"            // ✅ For FunctionDeclaration
#include "../arithmetic/arithmetic.h"          // ✅ For ArithmeticExpr
#include "../arithmetic/arithmetic_codegen.h"  // ✅ For expression codegen
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static int string_counter = 0;
static int is_first_call = 1;
static bool sto_declarations_emitted = false;

// YZ_91: Helper to check if a variable is a string type
static int is_variable_string(const char* var_name, FunctionDeclaration* func) {
    if (!func || !var_name) return 0;
    
    // Check local variables
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, var_name) == 0) {
            return !var->is_numeric;  // is_numeric=0 means string
        }
        var = var->next;
    }
    
    // Check parameters
    FunctionParam* param = func->params;
    while (param) {
        if (strcmp(param->name, var_name) == 0) {
            return (param->type == FUNC_PARAM_TEXT);
        }
        param = param->next;
    }
    
    return 0;  // Default to numeric
}

// YZ_91: Check if expression is string type (recursively)
static int is_expression_string(ArithmeticExpr* expr, FunctionDeclaration* func) {
    if (!expr) return 0;
    
    // Direct string flag
    if (expr->is_string) return 1;
    
    // String literal
    if (expr->is_literal && expr->is_string) return 1;
    
    // Variable - check type from symbol table
    if (!expr->is_literal && expr->value && !expr->left && !expr->right) {
        return is_variable_string(expr->value, func);
    }
    
    // Binary operation - if either operand is string (for concatenation)
    if (expr->left && is_expression_string(expr->left, func)) return 1;
    if (expr->right && is_expression_string(expr->right, func)) return 1;
    
    return 0;
}

// Emit STO runtime function declarations (only once)
static void emit_sto_declarations(FILE* f) {
    if (sto_declarations_emitted) return;
    
    // No need to declare extern in GAS - runtime library provides them
    fprintf(f, "    # STO Runtime Functions: sto_print_int64, printf, free\n\n");
    
    sto_declarations_emitted = true;
}

void codegen_print_statement(FILE* f, PrintStatement* stmt, FunctionDeclaration* func) {
    if (!stmt || !stmt->value) return;
    
    // On first call, emit program header (as comments in .text section)
    if (is_first_call) {
        fprintf(f, "    # MLP Print Module - Generated Assembly\n");
        fprintf(f, "    # Target: x86-64 Linux\n");
        fprintf(f, "    # STO Support: BigDecimal, SSO String\n\n");
        emit_sto_declarations(f);
        is_first_call = 0;
    }
    
    // Handle variable printing (DEPRECATED - now handled as PRINT_EXPRESSION)
    if (stmt->type == PRINT_VARIABLE) {
        fprintf(f, "\n    # Error: PRINT_VARIABLE deprecated - should use PRINT_EXPRESSION\n");
        string_counter++;
        return;
    }
    
    // Handle expression printing (including array indexing)
    if (stmt->type == PRINT_EXPRESSION) {
        ArithmeticExpr* expr = (ArithmeticExpr*)stmt->value;
        
        fprintf(f, "\n    # Print expression\n");
        
        // Generate expression code (result will be in r8 or xmm0)
        arithmetic_generate_code(f, expr, func);
        
        // YZ_91: Check if expression is a string (using symbol table for variables)
        int is_string_expr = is_expression_string(expr, func);
        
        if (is_string_expr) {
            // String: use puts (prints string + newline)
            fprintf(f, "    movq %%r8, %%rdi  # String pointer as argument\n");
            fprintf(f, "    call puts\n");
        } else {
            // Numeric: use sto_print_int64
            fprintf(f, "    mov %%r8, %%rdi  # Move result to first argument (AT&T syntax)\n");
            fprintf(f, "    call sto_print_int64\n");
        }
        
        string_counter++;
        return;
    }
    
    // String literal printing
    const char* string_content = stmt->value;
    
    // Generate unique label for this string
    fprintf(f, "section .data\n");
    fprintf(f, "    str_%d: db \"%s\", 10, 0  ; string with newline\n", string_counter, string_content);
    fprintf(f, "    str_%d_len: equ $-str_%d-2  ; length without null terminator and newline\n\n", 
            string_counter, string_counter);
    
    fprintf(f, "section .text\n");
    fprintf(f, "    ; Print: %s\n", string_content);
    
    // sys_write(stdout=1, buffer=str_N, length=str_N_len+1)
    fprintf(f, "    mov rax, 1              ; sys_write\n");
    fprintf(f, "    mov rdi, 1              ; stdout\n");
    fprintf(f, "    lea rsi, [rel str_%d]   ; string address\n", string_counter);
    fprintf(f, "    mov rdx, str_%d_len     ; string length\n", string_counter);
    fprintf(f, "    add rdx, 1              ; include newline\n");
    fprintf(f, "    syscall\n\n");
    
    string_counter++;
}

// Generate code for printing BigDecimal
// Expects: rdi = pointer to BigDecimal structure
void codegen_print_bigdecimal(FILE* f, const char* bigdec_var) {
    fprintf(f, "    ; Print BigDecimal: %s\n", bigdec_var);
    fprintf(f, "    push rbp\n");
    fprintf(f, "    mov rbp, rsp\n");
    fprintf(f, "    sub rsp, 16\n");
    
    // Call sto_bigdec_to_string(bigdec_var)
    fprintf(f, "    mov rdi, %s             ; BigDecimal pointer\n", bigdec_var);
    fprintf(f, "    call sto_bigdec_to_string\n");
    fprintf(f, "    mov [rbp-8], rax        ; Save string pointer\n");
    
    // Print the string using printf
    fprintf(f, "    mov rdi, rax            ; String pointer\n");
    fprintf(f, "    xor rax, rax            ; No floating point args\n");
    fprintf(f, "    call printf\n");
    
    // Print newline
    fprintf(f, "    mov rax, 1              ; sys_write\n");
    fprintf(f, "    mov rdi, 1              ; stdout\n");
    fprintf(f, "    lea rsi, [rel newline]  ; newline char\n");
    fprintf(f, "    mov rdx, 1              ; length\n");
    fprintf(f, "    syscall\n");
    
    // Free the string
    fprintf(f, "    mov rdi, [rbp-8]        ; String pointer\n");
    fprintf(f, "    call free\n");
    
    fprintf(f, "    mov rsp, rbp\n");
    fprintf(f, "    pop rbp\n\n");
}

// Generate code for printing SSO string
// Expects: rdi = pointer to SSOString structure
void codegen_print_sso_string(FILE* f, const char* sso_var) {
    fprintf(f, "    ; Print SSO String: %s\n", sso_var);
    fprintf(f, "    push rbp\n");
    fprintf(f, "    mov rbp, rsp\n");
    fprintf(f, "    sub rsp, 16\n");
    
    // Call sto_sso_data(sso_var)
    fprintf(f, "    mov rdi, %s             ; SSOString pointer\n", sso_var);
    fprintf(f, "    call sto_sso_data\n");
    fprintf(f, "    mov [rbp-8], rax        ; Save string pointer\n");
    
    // Print the string using printf
    fprintf(f, "    mov rdi, rax            ; String pointer\n");
    fprintf(f, "    xor rax, rax            ; No floating point args\n");
    fprintf(f, "    call printf\n");
    
    // Print newline
    fprintf(f, "    mov rax, 1              ; sys_write\n");
    fprintf(f, "    mov rdi, 1              ; stdout\n");
    fprintf(f, "    lea rsi, [rel newline]  ; newline char\n");
    fprintf(f, "    mov rdx, 1              ; length\n");
    fprintf(f, "    syscall\n");
    
    fprintf(f, "    mov rsp, rbp\n");
    fprintf(f, "    pop rbp\n\n");
}

// Generate code for printing int64
// Expects: value in specified register (default: rax)
void codegen_print_int64(FILE* f, const char* register_name) {
    static int format_string_emitted = 0;
    
    // Emit format string once
    if (!format_string_emitted) {
        fprintf(f, "section .data\n");
        fprintf(f, "    int64_fmt: db \"%%ld\", 0\n");
        fprintf(f, "    newline: db 10, 0\n\n");
        fprintf(f, "section .text\n");
        format_string_emitted = 1;
    }
    
    fprintf(f, "    ; Print int64 from %s\n", register_name);
    fprintf(f, "    push rbp\n");
    fprintf(f, "    mov rbp, rsp\n");
    fprintf(f, "    sub rsp, 16\n");
    
    // Save value if not already in rsi
    if (strcmp(register_name, "rax") == 0) {
        fprintf(f, "    mov rsi, rax            ; Value to print\n");
    } else {
        fprintf(f, "    mov rsi, %s             ; Value to print\n", register_name);
    }
    
    // Call printf
    fprintf(f, "    lea rdi, [rel int64_fmt]; Format string\n");
    fprintf(f, "    xor rax, rax            ; No floating point args\n");
    fprintf(f, "    call printf\n");
    
    // Print newline
    fprintf(f, "    mov rax, 1              ; sys_write\n");
    fprintf(f, "    mov rdi, 1              ; stdout\n");
    fprintf(f, "    lea rsi, [rel newline]  ; newline char\n");
    fprintf(f, "    mov rdx, 1              ; length\n");
    fprintf(f, "    syscall\n");
    
    fprintf(f, "    mov rsp, rbp\n");
    fprintf(f, "    pop rbp\n\n");
}

void codegen_print_finalize(FILE* f) {
    // Emit program exit code
    fprintf(f, "    ; Program exit\n");
    fprintf(f, "    mov rax, 60             ; sys_exit\n");
    fprintf(f, "    xor rdi, rdi            ; exit code 0\n");
    fprintf(f, "    syscall\n");
}


