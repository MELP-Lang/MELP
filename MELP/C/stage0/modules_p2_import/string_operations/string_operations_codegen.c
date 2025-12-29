#include <stdio.h>
#include "string_operations_codegen.h"
#include "string_operations.h"

// Generate code for string concatenation
void string_concat_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String concatenation\n");
    
    // Evaluate first string (result in %rdi)
    fprintf(output, "    # First string operand\n");
    fprintf(output, "    leaq string1(%%rip), %%rdi\n");
    
    // Evaluate second string (result in %rsi)
    fprintf(output, "    # Second string operand\n");
    fprintf(output, "    leaq string2(%%rip), %%rsi\n");
    
    // Call string concatenation function
    fprintf(output, "    call mlp_string_concat\n");
    fprintf(output, "    # Result in %%rax\n");
}

// Generate code for string length
void string_length_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String length\n");
    
    // Load string address
    fprintf(output, "    leaq string_var(%%rip), %%rdi\n");
    
    // Call mlp_string_length
    fprintf(output, "    call mlp_string_length\n");
    fprintf(output, "    # Length in %%rax\n");
}

// Generate code for substring
void string_substring_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String substring\n");
    
    // Load string address
    fprintf(output, "    leaq string_var(%%rip), %%rdi\n");
    
    // Load start index
    fprintf(output, "    movq $0, %%rsi  # start index\n");
    
    // Load length
    fprintf(output, "    movq $10, %%rdx  # length\n");
    
    // Call substring function
    fprintf(output, "    call mlp_string_substring\n");
    fprintf(output, "    # Result in %%rax\n");
}

// Generate code for string comparison
void string_compare_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String comparison\n");
    
    // Load first string
    fprintf(output, "    leaq string1(%%rip), %%rdi\n");
    
    // Load second string
    fprintf(output, "    leaq string2(%%rip), %%rsi\n");
    
    // Call mlp_string_compare
    fprintf(output, "    call mlp_string_compare\n");
    
    // Convert result to boolean (0 = equal, non-zero = not equal)
    fprintf(output, "    testq %%rax, %%rax\n");
    fprintf(output, "    setz %%al\n");
    fprintf(output, "    movzbq %%al, %%rax\n");
}

// Phase 5: Generate code for toUpperCase (YZ_29)
void string_toUpperCase_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String toUpperCase\n");
    
    // String argument should be in %rdi
    fprintf(output, "    call mlp_string_toUpperCase\n");
    fprintf(output, "    # Result (uppercase string) in %%rax\n");
}

// Phase 5: Generate code for toLowerCase (YZ_29)
void string_toLowerCase_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String toLowerCase\n");
    
    // String argument should be in %rdi
    fprintf(output, "    call mlp_string_toLowerCase\n");
    fprintf(output, "    # Result (lowercase string) in %%rax\n");
}

// Phase 5: Generate code for trim (YZ_29)
void string_trim_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String trim\n");
    
    // String argument should be in %rdi
    fprintf(output, "    call mlp_string_trim\n");
    fprintf(output, "    # Result (trimmed string) in %%rax\n");
}

// Phase 5: Generate code for trimStart (YZ_29)
void string_trimStart_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String trimStart\n");
    
    // String argument should be in %rdi
    fprintf(output, "    call mlp_string_trimStart\n");
    fprintf(output, "    # Result (trimmed string) in %%rax\n");
}

// Phase 5: Generate code for trimEnd (YZ_29)
void string_trimEnd_generate(FILE* output, StringOperation* op) {
    fprintf(output, "    # String trimEnd\n");
    
    // String argument should be in %rdi
    fprintf(output, "    call mlp_string_trimEnd\n");
    fprintf(output, "    # Result (trimmed string) in %%rax\n");
}

// Main code generation dispatcher
void string_op_generate_code(FILE* output, StringOperation* op) {
    if (!op) return;
    
    switch (op->op_type) {
        case STRING_OP_CONCAT:
            string_concat_generate(output, op);
            break;
        case STRING_OP_LENGTH:
            string_length_generate(output, op);
            break;
        case STRING_OP_SUBSTRING:
            string_substring_generate(output, op);
            break;
        case STRING_OP_COMPARE:
            string_compare_generate(output, op);
            break;
        case STRING_OP_INDEXOF:
            fprintf(output, "    # String indexOf\n");
            fprintf(output, "    call mlp_string_indexOf\n");
            fprintf(output, "    # Result (index or -1) in %%rax\n");
            break;
        case STRING_OP_CHARAT:
            fprintf(output, "    # String charAt - TODO\n");
            break;
        case STRING_OP_UPPER:
            string_toUpperCase_generate(output, op);
            break;
        case STRING_OP_LOWER:
            string_toLowerCase_generate(output, op);
            break;
        case STRING_OP_TRIM:
            string_trim_generate(output, op);
            break;
        case STRING_OP_TRIM_START:
            string_trimStart_generate(output, op);
            break;
        case STRING_OP_TRIM_END:
            string_trimEnd_generate(output, op);
            break;
        case STRING_OP_INTERPOLATE:
            fprintf(output, "    # TIER 1: String interpolation - TODO\n");
            break;
        case STRING_OP_MULTILINE:
            fprintf(output, "    # TIER 1: Multi-line string - TODO\n");
            break;
    }
}
