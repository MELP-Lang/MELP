#ifndef SWITCH_CODEGEN_H
#define SWITCH_CODEGEN_H

#include "switch.h"
#include <stdio.h>

// Generate assembly for switch statement
void switch_codegen(FILE* output, SwitchStatement* stmt, void* func);

#endif // SWITCH_CODEGEN_H
