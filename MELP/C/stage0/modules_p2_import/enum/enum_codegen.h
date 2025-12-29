#ifndef ENUM_CODEGEN_H
#define ENUM_CODEGEN_H

#include "enum.h"
#include <stdio.h>

// ============================================================================
// YZ_96: Enum Codegen
// ============================================================================
// Note: Enums are resolved at compile-time to numeric values.
// No runtime code is generated for enum definitions.
// Enum values become literal integers.

// Generate assembly comment for enum (documentation only)
void enum_codegen_comment(FILE* output, EnumDefinition* def);

#endif // ENUM_CODEGEN_H
