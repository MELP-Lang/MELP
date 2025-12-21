#ifndef FUNCTION_POINTER_H
#define FUNCTION_POINTER_H

#include <stddef.h>
#include "../lexer/lexer.h"
#include "../functions/functions.h"

// YZ_209: Function Pointer Support
// Function pointers allow passing functions as values
// Syntax: function<ReturnType, Param1Type, Param2Type, ...>

// Function pointer type definition
typedef struct FunctionPointerType {
    char* return_type;          // Return type (e.g., "numeric", "string")
    char** param_types;         // Parameter types
    int param_count;            // Number of parameters
    int is_generic;             // Generic function pointer (template)
    char** type_params;         // Generic type parameters (e.g., "T", "U")
    int type_param_count;       // Number of type parameters
} FunctionPointerType;

// Function reference: &function_name
typedef struct FunctionReference {
    char* function_name;        // Referenced function name
    FunctionPointerType* type;  // Function pointer type
    FunctionDeclaration* target;// Resolved function (after lookup)
} FunctionReference;

// Function pointer variable
typedef struct FunctionPointerVar {
    char* var_name;             // Variable name
    FunctionPointerType* type;  // Function pointer type
    FunctionReference* assigned_func; // Currently assigned function
} FunctionPointerVar;

// Indirect function call (call through pointer)
typedef struct IndirectCall {
    char* pointer_var;          // Function pointer variable name
    void** arguments;           // Call arguments
    int arg_count;              // Number of arguments
    FunctionPointerVar* resolved_ptr; // Resolved pointer variable
} IndirectCall;

// Callback pattern support
typedef struct CallbackParam {
    char* param_name;           // Parameter name
    FunctionPointerType* type;  // Function pointer type
} CallbackParam;

// API Functions

// Create function pointer type
FunctionPointerType* function_pointer_type_create(const char* return_type, int param_count);
void function_pointer_type_add_param(FunctionPointerType* type, const char* param_type);
void function_pointer_type_set_generic(FunctionPointerType* type, int type_param_count);

// Create function reference
FunctionReference* function_reference_create(const char* function_name);
int function_reference_resolve(FunctionReference* ref, FunctionDeclaration* functions);

// Create function pointer variable
FunctionPointerVar* function_pointer_var_create(const char* var_name, FunctionPointerType* type);
int function_pointer_var_assign(FunctionPointerVar* var, FunctionReference* func);

// Create indirect call
IndirectCall* indirect_call_create(const char* pointer_var, int arg_count);
void indirect_call_add_argument(IndirectCall* call, void* arg);

// Type checking
int function_pointer_type_match(FunctionPointerType* ptr_type, FunctionDeclaration* func);
int function_pointer_signature_match(FunctionPointerType* type1, FunctionPointerType* type2);

// Cleanup functions
void function_pointer_type_free(FunctionPointerType* type);
void function_reference_free(FunctionReference* ref);
void function_pointer_var_free(FunctionPointerVar* var);
void indirect_call_free(IndirectCall* call);
void callback_param_free(CallbackParam* param);

#endif // FUNCTION_POINTER_H
