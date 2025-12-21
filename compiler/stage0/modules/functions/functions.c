#include <stdlib.h>
#include <string.h>
#include "functions.h"

FunctionDeclaration* function_create(const char* name, FunctionReturnType return_type) {
    FunctionDeclaration* func = malloc(sizeof(FunctionDeclaration));
    func->name = strdup(name);
    func->params = NULL;
    func->return_type = return_type;
    func->return_struct_type = NULL;  // YZ_84: Initialize struct return type
    func->body = NULL;
    func->param_count = 0;
    func->local_var_count = 0;
    func->local_vars = NULL;  // Initialize local variable table
    
    // YZ_203: Initialize generic type parameters
    func->type_params = NULL;
    func->type_param_count = 0;
    func->is_generic_template = 0;
    
    func->next = NULL;  // Initialize linked list pointer
    return func;
}

void function_add_param(FunctionDeclaration* func, const char* name, FunctionParamType type) {
    FunctionParam* param = malloc(sizeof(FunctionParam));
    param->name = strdup(name);
    param->type = type;
    param->struct_type_name = NULL;  // YZ_84: Initialize struct type
    // YZ_31: Initialize advanced parameter modifiers
    param->default_value = NULL;
    param->default_str = NULL;
    param->has_default = 0;
    param->is_ref = 0;
    param->is_out = 0;
    param->is_optional = 0;
    param->is_variadic = 0;
    param->precision_digits = 0;
    param->precision_decimals = 0;
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

// YZ_84: Add struct parameter
void function_add_struct_param(FunctionDeclaration* func, const char* name, const char* struct_type) {
    FunctionParam* param = malloc(sizeof(FunctionParam));
    param->name = strdup(name);
    param->type = FUNC_PARAM_STRUCT;
    param->struct_type_name = strdup(struct_type);
    // YZ_31: Initialize advanced parameter modifiers
    param->default_value = NULL;
    param->default_str = NULL;
    param->has_default = 0;
    param->is_ref = 0;
    param->is_out = 0;
    param->is_optional = 0;
    param->is_variadic = 0;
    param->precision_digits = 0;
    param->precision_decimals = 0;
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
    free(func->return_struct_type);  // YZ_84: Free struct return type
    
    // YZ_203: Free generic type parameters
    if (func->type_params) {
        for (int i = 0; i < func->type_param_count; i++) {
            free(func->type_params[i]);
        }
        free(func->type_params);
    }
    
    FunctionParam* param = func->params;
    while (param) {
        FunctionParam* next = param->next;
        free(param->name);
        free(param->struct_type_name);  // YZ_84: Free struct type name
        free(param->default_str);       // YZ_31: Free default value string
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
    var->is_numeric = is_numeric;  // STO: 1=numeric, 0=string
    var->is_array = 0;             // Default: not an array
    var->array_length = 0;         // Default: no length
    var->is_tuple = 0;             // YZ_21: Default: not a tuple
    var->tuple_length = 0;         // YZ_21: Default: no length
    var->is_list = 0;              // YZ_22: Default: not a list
    var->list_length = 0;          // YZ_22: Default: no length
    var->is_const = 0;             // YZ_121: Default: not const
    var->const_value = 0;          // YZ_121: Default: 0
    
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

// YZ_25: Check if variable exists in function scope
int function_var_exists(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return 0;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return 1;  // Found
        }
        var = var->next;
    }
    
    return 0;  // Not found
}

// YZ_25: Find similar variable name (for typo suggestions)
// Uses simple Levenshtein-like comparison
static int str_similarity(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int matches = 0;
    int min_len = len_a < len_b ? len_a : len_b;
    
    for (int i = 0; i < min_len; i++) {
        if (a[i] == b[i]) matches++;
    }
    
    // Also check for transposed characters
    for (int i = 0; i < min_len - 1; i++) {
        if (a[i] == b[i+1] && a[i+1] == b[i]) matches++;
    }
    
    return matches * 100 / (len_a > len_b ? len_a : len_b);
}

const char* function_find_similar_var(FunctionDeclaration* func, const char* name) {
    if (!func || !name) return NULL;
    
    const char* best_match = NULL;
    int best_score = 50;  // Minimum 50% similarity required
    
    LocalVariable* var = func->local_vars;
    while (var) {
        int score = str_similarity(name, var->name);
        if (score > best_score) {
            best_score = score;
            best_match = var->name;
        }
        var = var->next;
    }
    
    return best_match;
}

// Lookup variable's type (1 = numeric, 0 = string)
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

// YZ_121: Set variable as const with value
void function_set_var_const(FunctionDeclaration* func, const char* name, int64_t value) {
    if (!func || !name) return;
    
    LocalVariable* var = func->local_vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            var->is_const = 1;
            var->const_value = value;
            // YZ_121: Const doesn't need stack space - decrement count
            func->local_var_count--;
            var->stack_offset = 0;  // Const has no stack offset
            return;
        }
        var = var->next;
    }
}

// Check if function is a builtin (stdlib function)
int function_is_builtin(const char* name) {
    if (!name) return 0;
    
    // I/O functions
    if (strcmp(name, "println") == 0) return 1;
    if (strcmp(name, "print") == 0) return 1;
    
    // YZ_31: Input functions
    if (strcmp(name, "input") == 0) return 1;
    if (strcmp(name, "input_numeric") == 0) return 1;
    
    // Conversion functions
    if (strcmp(name, "toString") == 0) return 1;
    
    // YZ_22: String methods
    if (strcmp(name, "length") == 0) return 1;
    if (strcmp(name, "substring") == 0) return 1;
    if (strcmp(name, "indexOf") == 0) return 1;
    
    // YZ_29: Phase 5 - String case conversion & trimming
    if (strcmp(name, "toUpperCase") == 0) return 1;
    if (strcmp(name, "toLowerCase") == 0) return 1;
    if (strcmp(name, "trim") == 0) return 1;
    if (strcmp(name, "trimStart") == 0) return 1;
    if (strcmp(name, "trimEnd") == 0) return 1;
    
    // YZ_31: Replace & Split
    if (strcmp(name, "replace") == 0) return 1;
    if (strcmp(name, "replaceAll") == 0) return 1;
    if (strcmp(name, "split") == 0) return 1;
    
    // YZ_33: Phase 9 - File I/O
    if (strcmp(name, "read_file") == 0) return 1;
    if (strcmp(name, "write_file") == 0) return 1;
    if (strcmp(name, "append_file") == 0) return 1;
    
    // YZ_34: Phase 8 - State Manager
    if (strcmp(name, "state_init") == 0) return 1;
    if (strcmp(name, "state_close") == 0) return 1;
    if (strcmp(name, "state_set") == 0) return 1;
    if (strcmp(name, "state_get") == 0) return 1;
    if (strcmp(name, "state_has") == 0) return 1;
    if (strcmp(name, "state_delete") == 0) return 1;
    if (strcmp(name, "state_clear") == 0) return 1;
    if (strcmp(name, "state_config_set") == 0) return 1;
    if (strcmp(name, "state_save") == 0) return 1;
    if (strcmp(name, "state_load") == 0) return 1;
    
    // YZ_40: Math functions (stdlib math library)
    if (strcmp(name, "abs") == 0) return 1;
    if (strcmp(name, "pow") == 0) return 1;
    if (strcmp(name, "sqrt") == 0) return 1;
    if (strcmp(name, "sin") == 0) return 1;
    if (strcmp(name, "cos") == 0) return 1;
    if (strcmp(name, "tan") == 0) return 1;
    if (strcmp(name, "floor") == 0) return 1;
    if (strcmp(name, "ceil") == 0) return 1;
    if (strcmp(name, "round") == 0) return 1;
    
    // YZ_94: Min/Max functions for nested call support
    if (strcmp(name, "min") == 0) return 1;
    if (strcmp(name, "max") == 0) return 1;
    
    return 0;
}

// YZ_36: Function name registry for user-defined functions
static char** known_functions = NULL;
static int known_function_count = 0;
static int known_function_capacity = 0;

void function_register_name(const char* name) {
    if (!name) return;
    
    // Check if already registered
    for (int i = 0; i < known_function_count; i++) {
        if (strcmp(known_functions[i], name) == 0) {
            return;  // Already registered
        }
    }
    
    // Expand capacity if needed
    if (known_function_count >= known_function_capacity) {
        known_function_capacity = known_function_capacity == 0 ? 16 : known_function_capacity * 2;
        known_functions = realloc(known_functions, sizeof(char*) * known_function_capacity);
    }
    
    // Add to registry
    known_functions[known_function_count++] = strdup(name);
}

int function_is_known(const char* name) {
    // First check if it's builtin
    if (function_is_builtin(name)) {
        return 1;
    }
    
    // Then check user-defined functions
    for (int i = 0; i < known_function_count; i++) {
        if (strcmp(known_functions[i], name) == 0) {
            return 1;
        }
    }
    
    return 0;
}

// YZ_203: Generic type parameter management
void function_add_type_param(FunctionDeclaration* func, const char* type_name) {
    if (!func || !type_name) return;
    
    // Allocate or reallocate type_params array
    func->type_params = realloc(func->type_params, 
                                sizeof(char*) * (func->type_param_count + 1));
    func->type_params[func->type_param_count] = strdup(type_name);
    func->type_param_count++;
    func->is_generic_template = 1;
}

// YZ_203: Check if a type name is a generic parameter of this function
int function_is_type_param(FunctionDeclaration* func, const char* type_name) {
    if (!func || !type_name || !func->type_params) return 0;
    
    for (int i = 0; i < func->type_param_count; i++) {
        if (strcmp(func->type_params[i], type_name) == 0) {
            return 1;
        }
    }
    return 0;
}

// YZ_203: Generate mangled name for generic instantiation
// Example: max<numeric> → max_numeric, max<numeric, string> → max_numeric_string
char* function_mangle_name(const char* func_name, char** concrete_types, int type_count) {
    if (!func_name || !concrete_types || type_count == 0) {
        return strdup(func_name);  // No mangling needed
    }
    
    // Calculate required buffer size
    size_t total_len = strlen(func_name);
    for (int i = 0; i < type_count; i++) {
        total_len += 1 + strlen(concrete_types[i]);  // "_" + type_name
    }
    
    // Build mangled name
    char* mangled = malloc(total_len + 1);
    strcpy(mangled, func_name);
    
    for (int i = 0; i < type_count; i++) {
        strcat(mangled, "_");
        strcat(mangled, concrete_types[i]);
    }
    
    return mangled;
}

void function_clear_registry(void) {
    for (int i = 0; i < known_function_count; i++) {
        free(known_functions[i]);
    }
    free(known_functions);
    known_functions = NULL;
    known_function_count = 0;
    known_function_capacity = 0;
}
