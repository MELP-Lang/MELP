#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include "lexer.h"
#include <stdio.h>

// Context for passing state between modules
typedef struct {
    FILE* output;
    char* data_section;
    char* bss_section;
    char* code_section;
    int* data_len;
    int* bss_len;
    int* code_len;
    int* string_count;
    int* label_counter;
    int var_count;
} CompilerContext;

// Compilation result
typedef struct {
    int exit_code;
    char* message;
} CompileResult;

// Main orchestration function
CompileResult orchestrate_compilation(const char* input_path, const char* output_path);

#endif // ORCHESTRATOR_H
