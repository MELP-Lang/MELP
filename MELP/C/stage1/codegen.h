#ifndef STAGE1_CODEGEN_H
#define STAGE1_CODEGEN_H

#include "parser.h"

// Code generation state
typedef struct {
    char* output;
    int output_size;
    int output_capacity;
    int indent_level;
} CodegenState;

// Function prototypes
CodegenState* codegen_create();
void codegen_free(CodegenState* state);
void codegen_program(CodegenState* state, ASTNode* program);
char* codegen_get_output(CodegenState* state);

#endif // STAGE1_CODEGEN_H
