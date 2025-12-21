#include "type_inference.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// YZ_203.5: Generic Type Inference Implementation

/**
 * Get the type name of an arithmetic expression
 * 
 * Strategy:
 * 1. Literals → immediate type ("numeric", "string", "boolean")
 * 2. Variables → lookup in symbol table
 * 3. Function calls → recursive inference (TODO)
 * 4. Expressions → analyze recursively
 */
char* type_inference_get_arith_type(ArithmeticExpr* arith, void* ctx_void) {
    if (!arith) return NULL;
    
    // Cast context (unused for now)
    (void)ctx_void;
    
    // Literal numeric
    if (arith->is_literal && !arith->is_string && !arith->is_boolean) {
        return strdup("numeric");
    }
    
    // String literal
    if (arith->is_string) {
        return strdup("string");
    }
    
    // Boolean literal
    if (arith->is_boolean) {
        return strdup("boolean");
    }
    
    // Null literal (YZ_202)
    if (arith->is_null) {
        return strdup("null");
    }
    
    // Variable lookup
    if (arith->is_literal == 0 && arith->value) {
        // TODO: Lookup variable type in symbol table
        // For now, assume numeric (fallback)
        return strdup("numeric");
    }
    
    // Binary operation → infer from left operand
    if (arith->left) {
        return type_inference_get_arith_type(arith->left, ctx_void);
    }
    
    // Default fallback
    return NULL;
}

/**
 * Unify two types (check compatibility)
 * 
 * For now: simple string comparison
 * Future: Handle subtypes, coercion, generic constraints
 */
int type_inference_unify(const char* type1, const char* type2) {
    if (!type1 || !type2) return 0;
    return strcmp(type1, type2) == 0;
}

/**
 * Build type map from template parameters to concrete types
 * 
 * Algorithm:
 * 1. For each function parameter:
 *    - If parameter type is a type param (T, U, etc.)
 *    - Map it to the actual argument type
 * 2. Ensure consistency (all T's map to same type)
 * 
 * Example:
 *   Template: function max<T>(T a, T b) returns T
 *   Params: ["T", "T"]
 *   Actuals: ["numeric", "numeric"]
 *   Result: {"T" → "numeric"}
 */
char** type_inference_build_type_map(char** template_params,
                                      int param_count,
                                      char** actual_types,
                                      int type_param_count) {
    if (!template_params || !actual_types || param_count == 0) {
        return NULL;
    }
    
    // Allocate type map (one entry per type parameter)
    char** type_map = calloc(type_param_count, sizeof(char*));
    
    // For each function parameter
    for (int i = 0; i < param_count; i++) {
        // Check if this parameter's type is a type parameter
        for (int j = 0; j < type_param_count; j++) {
            if (strcmp(template_params[i], template_params[j]) == 0) {
                // Found a type parameter usage
                if (type_map[j] == NULL) {
                    // First occurrence → assign type
                    type_map[j] = strdup(actual_types[i]);
                } else {
                    // Already assigned → check consistency
                    if (!type_inference_unify(type_map[j], actual_types[i])) {
                        fprintf(stderr, "Type inference error: Conflicting types for '%s': '%s' vs '%s'\n",
                                template_params[j], type_map[j], actual_types[i]);
                        
                        // Cleanup and fail
                        for (int k = 0; k < type_param_count; k++) {
                            free(type_map[k]);
                        }
                        free(type_map);
                        return NULL;
                    }
                }
            }
        }
    }
    
    // Check that all type parameters were inferred
    for (int i = 0; i < type_param_count; i++) {
        if (type_map[i] == NULL) {
            fprintf(stderr, "Type inference error: Could not infer type for parameter '%s'\n",
                    template_params[i]);
            
            // Cleanup and fail
            for (int k = 0; k < type_param_count; k++) {
                free(type_map[k]);
            }
            free(type_map);
            return NULL;
        }
    }
    
    return type_map;
}

/**
 * Main inference function: Infer type arguments from function call arguments
 * 
 * Algorithm:
 * 1. Get template function parameters
 * 2. Get actual call argument types
 * 3. Build type parameter → concrete type map
 * 4. Return inferred types
 */
char** type_inference_infer_from_args(GenericTemplate* template,
                                       ArithmeticExpr** call_args,
                                       int arg_count,
                                       int* out_type_count) {
    if (!template || !call_args || !out_type_count) {
        return NULL;
    }
    
    FunctionDeclaration* template_func = template->template_func;
    if (!template_func) {
        fprintf(stderr, "Type inference error: Template function is NULL\n");
        return NULL;
    }
    
    // Check argument count matches
    int template_param_count = 0;
    FunctionParam* param = template_func->params;
    while (param) {
        template_param_count++;
        param = param->next;
    }
    
    if (arg_count != template_param_count) {
        fprintf(stderr, "Type inference error: Argument count mismatch (expected %d, got %d)\n",
                template_param_count, arg_count);
        return NULL;
    }
    
    // Get actual argument types
    char** actual_types = malloc(sizeof(char*) * arg_count);
    for (int i = 0; i < arg_count; i++) {
        actual_types[i] = type_inference_get_arith_type(call_args[i], NULL);
        if (!actual_types[i]) {
            fprintf(stderr, "Type inference error: Could not infer type for argument %d\n", i);
            
            // Cleanup
            for (int j = 0; j < i; j++) {
                free(actual_types[j]);
            }
            free(actual_types);
            return NULL;
        }
    }
    
    // Get template parameter types (in order)
    char** template_param_types = malloc(sizeof(char*) * template_param_count);
    param = template_func->params;
    int idx = 0;
    while (param) {
        // Use struct_type_name if it's a type parameter
        if (param->struct_type_name) {
            template_param_types[idx] = param->struct_type_name;
        } else {
            // Not a type parameter → use actual type name
            // (This shouldn't happen in a generic function, but handle it)
            template_param_types[idx] = "numeric";  // Fallback
        }
        idx++;
        param = param->next;
    }
    
    // Build type map
    char** type_map = type_inference_build_type_map(
        template_param_types,
        template_param_count,
        actual_types,
        template_func->type_param_count
    );
    
    // Cleanup temporary arrays
    for (int i = 0; i < arg_count; i++) {
        free(actual_types[i]);
    }
    free(actual_types);
    free(template_param_types);
    
    // Return result
    if (type_map) {
        *out_type_count = template_func->type_param_count;
        
        printf("🔍 Type inference successful: %s<", template->name);
        for (int i = 0; i < template_func->type_param_count; i++) {
            printf("%s%s", type_map[i], (i < template_func->type_param_count - 1) ? ", " : "");
        }
        printf(">\n");
    }
    
    return type_map;
}
