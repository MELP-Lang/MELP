#ifndef FOR_LOOP_CODEGEN_H
#define FOR_LOOP_CODEGEN_H

#include "for_loop.h"
#include <stdio.h>

// Generate assembly code for for loop
// Desugars to: init + while loop
void for_loop_generate_code(FILE* output, ForLoop* loop, void* context);

#endif // FOR_LOOP_CODEGEN_H
