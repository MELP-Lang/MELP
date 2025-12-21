#include "function_pointer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// YZ_209: Function Pointer Implementation

// Create function pointer type
FunctionPointerType* function_pointer_type_create(const char* return_type, int param_count) {
    FunctionPointerType* type = malloc(sizeof(FunctionPointerType));
    if (!type) return NULL;
    
    type->return_type = strdup(return_type);
    type->param_types = malloc(sizeof(char*) * param_count);
    type->param_count = param_count;
    type->is_generic = 0;
    type->type_params = NULL;
    type->type_param_count = 0;
    
    for (int i = 0; i < param_count; i++) {
        type->param_types[i] = NULL;
    }
    
    return type;
}

void function_pointer_type_add_param(FunctionPointerType* type, const char* param_type) {
    if (!type || !param_type) return;
    
    for (int i = 0; i < type->param_count; i++) {
        if (type->param_types[i] == NULL) {
            type->param_types[i] = strdup(param_type);
            return;
        }
    }
}

void function_pointer_type_set_generic(FunctionPointerType* type, int type_param_count) {
    if (!type) return;
    
    type->is_generic = 1;
    type->type_param_count = type_param_count;
    type->type_params = malloc(sizeof(char*) * type_param_count);
    
    for (int i = 0; i < type_param_count; i++) {
        type->type_params[i] = NULL;
    }
}

// Create function reference
FunctionReference* function_reference_create(const char* function_name) {
    FunctionReference* ref = malloc(sizeof(FunctionReference));
    if (!ref) return NULL;
    
    ref->function_name = strdup(function_name);
    ref->type = NULL;
    ref->target = NULL;
    
    return ref;
}

int function_reference_resolve(FunctionReference* ref, FunctionDeclaration* functions) {
    if (!ref || !functions) return 0;
    
    // Search for function in function list
    FunctionDeclaration* current = functions;
    while (current) {
        if (strcmp(current->name, ref->function_name) == 0) {
            ref->target = current;
            
            // Build function pointer type from function signature
            ref->type = function_pointer_type_create(
                current->return_struct_type ? current->return_struct_type : "void",
                current->param_count
            );
            
            // Add parameter types
            FunctionParam* param = current->params;
            for (int i = 0; i < current->param_count && param; i++) {
                if (param->struct_type_name) {
                    function_pointer_type_add_param(ref->type, param->struct_type_name);
                } else {
                    // Map FunctionParamType to string
                    const char* type_str = "unknown";
                    switch (param->type) {
                        case FUNC_PARAM_NUMERIC: type_str = "numeric"; break;
                        case FUNC_PARAM_TEXT: type_str = "string"; break;
                        case FUNC_PARAM_BOOLEAN: type_str = "bool"; break;
                        case FUNC_PARAM_POINTER: type_str = "pointer"; break;
                        case FUNC_PARAM_ARRAY: type_str = "array"; break;
                        case FUNC_PARAM_LIST: type_str = "list"; break;
                        default: type_str = "unknown"; break;
                    }
                    function_pointer_type_add_param(ref->type, type_str);
                }
                param = param->next;
            }
            
            return 1; // Success
        }
        current = current->next;
    }
    
    return 0; // Function not found
}

// Create function pointer variable
FunctionPointerVar* function_pointer_var_create(const char* var_name, FunctionPointerType* type) {
    FunctionPointerVar* var = malloc(sizeof(FunctionPointerVar));
    if (!var) return NULL;
    
    var->var_name = strdup(var_name);
    var->type = type;
    var->assigned_func = NULL;
    
    return var;
}

int function_pointer_var_assign(FunctionPointerVar* var, FunctionReference* func) {
    if (!var || !func || !func->type) return 0;
    
    // Type check: function signature must match pointer type
    if (!function_pointer_signature_match(var->type, func->type)) {
        fprintf(stderr, "Error: Function signature mismatch in pointer assignment\n");
        return 0;
    }
    
    var->assigned_func = func;
    return 1;
}

// Create indirect call
IndirectCall* indirect_call_create(const char* pointer_var, int arg_count) {
    IndirectCall* call = malloc(sizeof(IndirectCall));
    if (!call) return NULL;
    
    call->pointer_var = strdup(pointer_var);
    call->arguments = malloc(sizeof(void*) * arg_count);
    call->arg_count = arg_count;
    call->resolved_ptr = NULL;
    
    for (int i = 0; i < arg_count; i++) {
        call->arguments[i] = NULL;
    }
    
    return call;
}

void indirect_call_add_argument(IndirectCall* call, void* arg) {
    if (!call || !arg) return;
    
    for (int i = 0; i < call->arg_count; i++) {
        if (call->arguments[i] == NULL) {
            call->arguments[i] = arg;
            return;
        }
    }
}

// Type checking
int function_pointer_type_match(FunctionPointerType* ptr_type, FunctionDeclaration* func) {
    if (!ptr_type || !func) return 0;
    
    // Check parameter count
    if (ptr_type->param_count != func->param_count) {
        return 0;
    }
    
    // Check return type
    const char* func_return = func->return_struct_type ? func->return_struct_type : "void";
    if (strcmp(ptr_type->return_type, func_return) != 0) {
        return 0;
    }
    
    // Check parameter types
    FunctionParam* param = func->params;
    for (int i = 0; i < ptr_type->param_count && param; i++) {
        if (!ptr_type->param_types[i]) {
            return 0;
        }
        
        // Get parameter type string
        const char* param_type_str = "unknown";
        if (param->struct_type_name) {
            param_type_str = param->struct_type_name;
        } else {
            switch (param->type) {
                case FUNC_PARAM_NUMERIC: param_type_str = "numeric"; break;
                case FUNC_PARAM_TEXT: param_type_str = "string"; break;
                case FUNC_PARAM_BOOLEAN: param_type_str = "bool"; break;
                default: param_type_str = "unknown"; break;
            }
        }
        
        if (strcmp(ptr_type->param_types[i], param_type_str) != 0) {
            return 0;
        }
        
        param = param->next;
    }
    
    return 1; // Match
}

int function_pointer_signature_match(FunctionPointerType* type1, FunctionPointerType* type2) {
    if (!type1 || !type2) return 0;
    
    // Check parameter count
    if (type1->param_count != type2->param_count) {
        return 0;
    }
    
    // Check return type
    if (strcmp(type1->return_type, type2->return_type) != 0) {
        return 0;
    }
    
    // Check parameter types
    for (int i = 0; i < type1->param_count; i++) {
        if (!type1->param_types[i] || !type2->param_types[i]) {
            return 0;
        }
        if (strcmp(type1->param_types[i], type2->param_types[i]) != 0) {
            return 0;
        }
    }
    
    return 1; // Match
}

// Cleanup functions
void function_pointer_type_free(FunctionPointerType* type) {
    if (!type) return;
    
    free(type->return_type);
    
    for (int i = 0; i < type->param_count; i++) {
        free(type->param_types[i]);
    }
    free(type->param_types);
    
    if (type->type_params) {
        for (int i = 0; i < type->type_param_count; i++) {
            free(type->type_params[i]);
        }
        free(type->type_params);
    }
    
    free(type);
}

void function_reference_free(FunctionReference* ref) {
    if (!ref) return;
    
    free(ref->function_name);
    if (ref->type) {
        function_pointer_type_free(ref->type);
    }
    // Don't free target - it's owned by function list
    
    free(ref);
}

void function_pointer_var_free(FunctionPointerVar* var) {
    if (!var) return;
    
    free(var->var_name);
    if (var->type) {
        function_pointer_type_free(var->type);
    }
    // Don't free assigned_func - it's shared
    
    free(var);
}

void indirect_call_free(IndirectCall* call) {
    if (!call) return;
    
    free(call->pointer_var);
    
    // Don't free arguments - they're owned by expression system
    free(call->arguments);
    
    // Don't free resolved_ptr - it's owned by variable table
    
    free(call);
}

void callback_param_free(CallbackParam* param) {
    if (!param) return;
    
    free(param->param_name);
    if (param->type) {
        function_pointer_type_free(param->type);
    }
    
    free(param);
}
