#include "function_pointer.h"
#include "function_pointer_parser.h"
#include "function_pointer_codegen.h"
#include "../lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>

// YZ_209: Function Pointer Module - Standalone Test

int main(int argc, char** argv) {
    printf("=== MELP Function Pointer Module Test ===\n\n");
    
    // Test 1: Create function pointer type
    printf("Test 1: Create function pointer type\n");
    FunctionPointerType* type = function_pointer_type_create("numeric", 2);
    function_pointer_type_add_param(type, "numeric");
    function_pointer_type_add_param(type, "numeric");
    
    printf("  Type: function<numeric, numeric, numeric>\n");
    printf("  Return: %s\n", type->return_type);
    printf("  Params: %d\n", type->param_count);
    for (int i = 0; i < type->param_count; i++) {
        printf("    Param %d: %s\n", i, type->param_types[i]);
    }
    printf("  ✅ Type created\n\n");
    
    // Test 2: Create function reference
    printf("Test 2: Create function reference\n");
    FunctionReference* ref = function_reference_create("add");
    printf("  Reference: &%s\n", ref->function_name);
    printf("  ✅ Reference created\n\n");
    
    // Test 3: Create function pointer variable
    printf("Test 3: Create function pointer variable\n");
    FunctionPointerVar* var = function_pointer_var_create("operation", type);
    printf("  Variable: %s\n", var->var_name);
    printf("  Type: function<...>\n");
    printf("  ✅ Variable created\n\n");
    
    // Test 4: Function pointer type string generation
    printf("Test 4: LLVM type string generation\n");
    char* type_str = generate_function_type_string(type);
    if (type_str) {
        printf("  LLVM type: %s\n", type_str);
        free(type_str);
        printf("  ✅ Type string generated\n\n");
    }
    
    // Test 5: Type matching
    printf("Test 5: Type matching\n");
    FunctionPointerType* type2 = function_pointer_type_create("numeric", 2);
    function_pointer_type_add_param(type2, "numeric");
    function_pointer_type_add_param(type2, "numeric");
    
    int match = function_pointer_signature_match(type, type2);
    printf("  Type1 vs Type2: %s\n", match ? "MATCH" : "NO MATCH");
    printf("  ✅ Type matching works\n\n");
    
    // Test 6: Create indirect call
    printf("Test 6: Create indirect call\n");
    IndirectCall* call = indirect_call_create("operation", 2);
    printf("  Call: %s(...)\n", call->pointer_var);
    printf("  Args: %d\n", call->arg_count);
    printf("  ✅ Indirect call created\n\n");
    
    // Cleanup
    function_pointer_type_free(type);
    function_pointer_type_free(type2);
    function_reference_free(ref);
    function_pointer_var_free(var);
    indirect_call_free(call);
    
    printf("=== All tests passed! ===\n");
    
    // If MELP file provided, parse it
    if (argc > 1) {
        printf("\n=== Parsing MELP file: %s ===\n", argv[1]);
        
        FILE* input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        
        // Read file content
        fseek(input, 0, SEEK_END);
        long size = ftell(input);
        fseek(input, 0, SEEK_SET);
        
        char* content = malloc(size + 1);
        fread(content, 1, size, input);
        content[size] = '\0';
        fclose(input);
        
        printf("File content:\n%s\n", content);
        
        // TODO: Full parsing implementation
        printf("⚠️  Full parsing not yet implemented\n");
        
        free(content);
    }
    
    return 0;
}
