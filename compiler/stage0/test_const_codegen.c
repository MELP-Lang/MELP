// ============================================================================
// test_const_codegen.c - Const Codegen Test (YZ_CONST)
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/variable/variable.h"
#include "modules/variable/variable_codegen.h"

int main() {
    printf("=== YZ_CONST: Codegen Test ===\n\n");
    
    FILE* output = fopen("test_const_output.asm", "w");
    if (!output) {
        fprintf(stderr, "Failed to open output file\n");
        return 1;
    }
    
    VariableCodegen* codegen = variable_codegen_create(output);
    
    // Test 1: const numeric
    printf("Test 1: const numeric PI = 3.14159\n");
    VariableDeclaration* decl1 = malloc(sizeof(VariableDeclaration));
    memset(decl1, 0, sizeof(VariableDeclaration));
    decl1->name = strdup("PI");
    decl1->type = VAR_NUMERIC;
    decl1->base_type = VAR_NUMERIC;
    decl1->value = strdup("3.14159");
    decl1->is_const = true;
    decl1->is_pointer = 0;
    decl1->is_array = 0;
    decl1->storage = STORAGE_DATA;
    decl1->internal_num_type = INTERNAL_DOUBLE;
    
    variable_codegen_declaration(codegen, decl1);
    printf("  ✓ Generated\n\n");
    
    // Test 2: const string
    printf("Test 2: const string APP_NAME = \"MELP\"\n");
    VariableDeclaration* decl2 = malloc(sizeof(VariableDeclaration));
    memset(decl2, 0, sizeof(VariableDeclaration));
    decl2->name = strdup("APP_NAME");
    decl2->type = VAR_STRING;
    decl2->base_type = VAR_STRING;
    decl2->value = strdup("MELP");
    decl2->is_const = true;
    decl2->is_pointer = 0;
    decl2->is_array = 0;
    decl2->storage = STORAGE_DATA;
    decl2->internal_str_type = INTERNAL_RODATA;
    
    variable_codegen_declaration(codegen, decl2);
    printf("  ✓ Generated\n\n");
    
    // Test 3: const boolean
    printf("Test 3: const boolean DEBUG_MODE = true\n");
    VariableDeclaration* decl3 = malloc(sizeof(VariableDeclaration));
    memset(decl3, 0, sizeof(VariableDeclaration));
    decl3->name = strdup("DEBUG_MODE");
    decl3->type = VAR_BOOLEAN;
    decl3->base_type = VAR_BOOLEAN;
    decl3->value = strdup("true");
    decl3->is_const = true;
    decl3->is_pointer = 0;
    decl3->is_array = 0;
    decl3->storage = STORAGE_DATA;
    
    variable_codegen_declaration(codegen, decl3);
    printf("  ✓ Generated\n\n");
    
    // Test 4: Regular (non-const) variable
    printf("Test 4: numeric x = 42 (non-const)\n");
    VariableDeclaration* decl4 = malloc(sizeof(VariableDeclaration));
    memset(decl4, 0, sizeof(VariableDeclaration));
    decl4->name = strdup("x");
    decl4->type = VAR_NUMERIC;
    decl4->base_type = VAR_NUMERIC;
    decl4->value = strdup("42");
    decl4->is_const = false;  // Not const
    decl4->is_pointer = 0;
    decl4->is_array = 0;
    decl4->storage = STORAGE_BSS;
    decl4->internal_num_type = INTERNAL_INT64;
    
    variable_codegen_declaration(codegen, decl4);
    printf("  ✓ Generated\n\n");
    
    // Cleanup (manual free since we can't link variable_parser)
    if (decl1->name) free(decl1->name);
    if (decl1->value) free(decl1->value);
    free(decl1);
    
    if (decl2->name) free(decl2->name);
    if (decl2->value) free(decl2->value);
    free(decl2);
    
    if (decl3->name) free(decl3->name);
    if (decl3->value) free(decl3->value);
    free(decl3);
    
    if (decl4->name) free(decl4->name);
    if (decl4->value) free(decl4->value);
    free(decl4);
    
    variable_codegen_free(codegen);
    fclose(output);
    
    // Display generated assembly
    printf("=== Generated Assembly ===\n");
    output = fopen("test_const_output.asm", "r");
    if (output) {
        char line[256];
        while (fgets(line, sizeof(line), output)) {
            printf("%s", line);
        }
        fclose(output);
    }
    
    printf("\n=== Test Complete ===\n");
    return 0;
}
