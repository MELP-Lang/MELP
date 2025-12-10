#include <stdlib.h>
#include <string.h>
#include "functions.h"

FunctionDeclaration* function_create(const char* name, FunctionReturnType return_type) {
    FunctionDeclaration* func = malloc(sizeof(FunctionDeclaration));
    func->name = strdup(name);
    func->params = NULL;
    func->return_type = return_type;
    func->body = NULL;
    func->param_count = 0;
    func->local_var_count = 0;
    func->local_vars = NULL;  // Initialize local variable table
    func->next = NULL;  // Initialize linked list pointer
    return func;
}

void function_add_param(FunctionDeclaration* func, const char* name, FunctionParamType type) {
    FunctionParam* param = malloc(sizeof(FunctionParam));
    param->name = strdup(name);
    param->type = type;
    param->next = NULL;
    
    if (func->params == NULL) {
        func->params = param;
    } else {
        FunctionParam* curr = func->params;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = param;
    }
    func->param_count++;
}

void function_set_body(FunctionDeclaration* func, Statement* body) {
    func->body = body;
}

void function_free(FunctionDeclaration* func) {
    if (!func) return;
    
    free(func->name);
    
    FunctionParam* param = func->params;
    while (param) {
        FunctionParam* next = param->next;
        free(param->name);
        free(param);
        param = next;
    }
    
    // Note: body statements freed elsewhere in AST cleanup
    free(func);
}

FunctionCall* function_call_create(const char* name) {
    FunctionCall* call = malloc(sizeof(FunctionCall));
    call->function_name = strdup(name);
    call->arguments = NULL;
    call->arg_count = 0;
    return call;
}

void function_call_add_arg(FunctionCall* call, struct Expression* arg) {
    call->arguments = realloc(call->arguments, 
                             sizeof(struct Expression*) * (call->arg_count + 1));
    call->arguments[call->arg_count] = arg;
    call->arg_count++;
}

void function_call_free(FunctionCall* call) {
    if (!call) return;
    
    free(call->function_name);
    
    // Note: arguments freed elsewhere in AST cleanup
    free(call->arguments);
    free(call);
}

ReturnStatement* return_statement_create(struct Expression* value) {
    ReturnStatement* ret = malloc(sizeof(ReturnStatement));
    ret->return_value = value;
    return ret;
}

void return_statement_free(ReturnStatement* ret) {
    if (!ret) return;
    // Note: return_value freed elsewhere in AST cleanup
    free(ret);
}

// Register a local variable in function's symbol table
void function_register_local_var_with_type(FunctionDeclaration* func, const char* name, int is_numeric) {
    if (!func || !name) return;
    
    // Check if variable already exists
    LocalVariable* existing = func->local_vars;
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            return;  // Already registered
        }
        existing = existing->next;
    }
    
    // Create new local variable entry
    LocalVariable* var = malloc(sizeof(LocalVariable));
    var->name = strdup(name);
    var->is_numeric = is_numeric;  // TTO: 1=numeric, 0=text
    var->is_array = 0;             // Default: not an array
    var->array_length = 0;         // Default: no length
    var->is_tuple = 0;             // YZ_21: Default: not a tuple
    var->tuple_length = 0;         // YZ_21: Default: no length
    var->is_list = 0;              // YZ_22: Default: not a list
    var->list_length = 0;          // YZ_22: Default: no length
    
    // Assign stack offset: -8, -16, -24, ...
    func->local_var_count++;
    var->stack_offset = -8 * func->local_var_count;
    
    // Add to front of list
    var->next = func->local_vars;
    func->local_vars = var;
}

// Backward compatibility wrapper
void function_register_local_var(FunctionDeclaration* func, const char* name) {
    function_register_local_var_with_type(func, name, 1);  // Default to numeric
}

// Lookup variable's stack offset
int function_get_var_offset(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return var->stack_offset;
        }
        var = var->next;
    }
    
    return 0;  // Not found
}

// Lookup variable's type (1 = numeric, 0 = text)
int function_get_var_is_numeric(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 1;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return var->is_numeric;
        }
        var = var->next;
    }
    
    return 1;  // Not found, assume numeric
}

// Register array variable with length
void function_register_array_var(FunctionDeclaration* func, const char* name, int length, int is_numeric) {
    if (!func || !name) return;
    
    // Check if variable already exists
    LocalVariable* existing = func->local_vars;
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            // Update array info
            existing->is_array = 1;
            existing->array_length = length;
            return;
        }
        existing = existing->next;
    }
    
    // Create new array variable entry
    LocalVariable* var = malloc(sizeof(LocalVariable));
    var->name = strdup(name);
    var->is_numeric = is_numeric;
    var->is_array = 1;
    var->array_length = length;
    var->is_tuple = 0;             // YZ_21: Not a tuple
    var->tuple_length = 0;         // YZ_21
    var->is_list = 0;              // YZ_22: Not a list
    var->list_length = 0;          // YZ_22
    
    // Assign stack offset
    func->local_var_count++;
    var->stack_offset = -8 * func->local_var_count;
    
    // Add to front
    var->next = func->local_vars;
    func->local_vars = var;
}

// Get array length
int function_get_array_length(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0 && var->is_array) {
            return var->array_length;
        }
        var = var->next;
    }
    
    return 0;  // Not found or not an array
}

// YZ_21: Register tuple variable with length
void function_register_tuple_var(FunctionDeclaration* func, const char* name, int length) {
    if (!func || !name) return;
    
    // Check if variable already exists
    LocalVariable* existing = func->local_vars;
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            // Update tuple info
            existing->is_tuple = 1;
            existing->tuple_length = length;
            return;
        }
        existing = existing->next;
    }
    
    // Create new tuple variable entry
    LocalVariable* var = malloc(sizeof(LocalVariable));
    var->name = strdup(name);
    var->is_numeric = 0;  // Tuples are heterogeneous
    var->is_array = 0;
    var->array_length = 0;
    var->is_tuple = 1;
    var->tuple_length = length;
    var->is_list = 0;              // YZ_22: Not a list
    var->list_length = 0;          // YZ_22
    
    // Assign stack offset
    func->local_var_count++;
    var->stack_offset = -8 * func->local_var_count;
    
    // Add to front
    var->next = func->local_vars;
    func->local_vars = var;
}

// YZ_21: Check if variable is a tuple
int function_is_tuple(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return var->is_tuple;
        }
        var = var->next;
    }
    
    return 0;  // Not found or not a tuple
}

// YZ_21: Get tuple length
int function_get_tuple_length(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0 && var->is_tuple) {
            return var->tuple_length;
        }
        var = var->next;
    }
    
    return 0;  // Not found or not a tuple
}

// YZ_22: Register list variable with length
void function_register_list_var(FunctionDeclaration* func, const char* name, int length) {
    if (!func || !name) return;
    
    // Check if variable already exists
    LocalVariable* existing = func->local_vars;
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            // Update list info
            existing->is_list = 1;
            existing->list_length = length;
            return;
        }
        existing = existing->next;
    }
    
    // Create new list variable entry
    LocalVariable* var = malloc(sizeof(LocalVariable));
    var->name = strdup(name);
    var->is_numeric = 0;  // Lists are heterogeneous
    var->is_array = 0;
    var->array_length = 0;
    var->is_tuple = 0;
    var->tuple_length = 0;
    var->is_list = 1;
    var->list_length = length;
    
    // Assign stack offset
    func->local_var_count++;
    var->stack_offset = -8 * func->local_var_count;
    
    // Add to front
    var->next = func->local_vars;
    func->local_vars = var;
}

// YZ_22: Check if variable is a list
int function_is_list(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return var->is_list;
        }
        var = var->next;
    }
    
    return 0;  // Not found or not a list
}

// YZ_22: Get list length
int function_get_list_length(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0 && var->is_list) {
            return var->list_length;
        }
        var = var->next;
    }
    
    return 0;  // Not found or not a list
}

// Check if function is a builtin (stdlib function)
int function_is_builtin(const char* name) {
    if (!name) return 0;
    
    // I/O functions
    if (strcmp(name, "println") == 0) return 1;
    if (strcmp(name, "print") == 0) return 1;
    
    // Conversion functions
    if (strcmp(name, "toString") == 0) return 1;
    
    // YZ_22: String methods
    if (strcmp(name, "length") == 0) return 1;
    if (strcmp(name, "substring") == 0) return 1;
    if (strcmp(name, "indexOf") == 0) return 1;
    
    return 0;
}
