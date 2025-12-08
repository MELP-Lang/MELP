#ifndef CONTROL_FLOW_CODEGEN_H
#define CONTROL_FLOW_CODEGEN_H

#include <stdio.h>
#include "control_flow.h"

// Forward declaration to avoid circular dependency
typedef struct Statement Statement;

// Code generation - context for variable scoping/function info
void control_flow_generate_if(FILE* output, IfStatement* stmt, void* context);
void control_flow_generate_while(FILE* output, WhileStatement* stmt, void* context);
void control_flow_generate_for(FILE* output, ForStatement* stmt, void* context);

#endif // CONTROL_FLOW_CODEGEN_H
