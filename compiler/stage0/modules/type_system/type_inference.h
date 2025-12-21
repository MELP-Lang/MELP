#ifndef TYPE_INFERENCE_H
#define TYPE_INFERENCE_H

#include "../functions/functions.h"
#include "../functions/functions_generic.h"
#include "../array/arithmetic/arithmetic.h"
#include "../expression/expression.h"  // For Expression type

// YZ_203.5: Generic Type Inference
// Infer type arguments from function call arguments

/**
 * Infer type arguments for a generic function call
 * 
 * Given:
 *   - Generic template: function max<T>(T a, T b) returns T
 *   - Call: max(10, 20)
 * 
 * Returns: ["numeric"] (inferred T = numeric)
 * 
 * @param template The generic template to instantiate
 * @param call_args Array of ArithmeticExpr* (actual arguments)
 * @param arg_count Number of arguments
 * @param out_type_count Output: Number of inferred types
 * @return Array of inferred type names (NULL on failure)
 */
char** type_inference_infer_from_args(GenericTemplate* template,
                                       ArithmeticExpr** call_args,
                                       int arg_count,
                                       int* out_type_count);

/**
 * Helper: Get the type name of an arithmetic expression
 * 
 * Examples:
 *   - 42 → "numeric"
 *   - "hello" → "string"
 *   - variable_name → lookup in symbol table
 * 
 * @param expr The arithmetic expression to analyze
 * @param ctx Code generation context (for variable lookup)
 * @return Type name string (caller must free)
 */
char* type_inference_get_arith_type(ArithmeticExpr* expr, void* ctx);

/**
 * Helper: Unify two types (check if they're compatible)
 * 
 * For now, simple string comparison.
 * Future: Handle subtypes, coercion, etc.
 * 
 * @param type1 First type name
 * @param type2 Second type name
 * @return 1 if compatible, 0 otherwise
 */
int type_inference_unify(const char* type1, const char* type2);

/**
 * Helper: Extract type mapping from template parameters
 * 
 * Given:
 *   - Template params: [T, T]
 *   - Actual types: ["numeric", "numeric"]
 * 
 * Returns: {"T" → "numeric"}
 * 
 * @param template_params Array of type parameter names
 * @param param_count Number of parameters
 * @param actual_types Array of actual type names
 * @return Array of concrete type assignments (NULL on failure)
 */
char** type_inference_build_type_map(char** template_params,
                                      int param_count,
                                      char** actual_types,
                                      int type_param_count);

#endif
