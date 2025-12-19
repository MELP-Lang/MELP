#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    FUNC_PARAM_NUMERIC,
    FUNC_PARAM_TEXT,
    FUNC_PARAM_BOOLEAN,
    FUNC_PARAM_POINTER,
    FUNC_PARAM_ARRAY,
    FUNC_PARAM_STRUCT,  // YZ_84: Struct type parameter
    FUNC_PARAM_LIST     // YZ_21: List type parameter
} FunctionParamType;

typedef struct FunctionParam {
    char* name;
    FunctionParamType type;
    char* struct_type_name;  // YZ_84: Struct type name (if type == FUNC_PARAM_STRUCT)
    void* default_value;     // TIER 1: Default parameter value
    int has_default;         // TIER 1: Whether param has default
    struct FunctionParam* next;
} FunctionParam;

typedef enum {
    FUNC_RETURN_NUMERIC,
    FUNC_RETURN_TEXT,
    FUNC_RETURN_BOOLEAN,
    FUNC_RETURN_VOID,
    FUNC_RETURN_LIST,    // YZ_30: List return type
    FUNC_RETURN_STRUCT   // YZ_84: Struct return type
} FunctionReturnType;

typedef struct Statement Statement;

// Local variable info for stack offset tracking
typedef struct LocalVariable {
    char* name;
    int stack_offset;  // Offset from rbp (negative: -8, -16, -24...)
    int is_numeric;    // 1=numeric, 0=string (STO: only 2 types, 1 bit is enough!)
    int is_array;      // 1=array, 0=scalar
    int array_length;  // Array element count (for bounds checking)
    int is_tuple;      // YZ_21: 1=tuple, 0=other
    int tuple_length;  // YZ_21: Tuple element count
    int is_list;       // YZ_22: 1=list, 0=other
    int list_length;   // YZ_22: List element count
    struct LocalVariable* next;
} LocalVariable;

typedef struct FunctionDeclaration {
    char* name;
    FunctionParam* params;
    FunctionReturnType return_type;
    char* return_struct_type;  // YZ_84: Struct type name (if return_type == FUNC_RETURN_STRUCT)
    Statement* body;           // Linked list of statements
    int param_count;
    int local_var_count;       // For stack frame calculation
    LocalVariable* local_vars; // Local variable table for codegen
    struct FunctionDeclaration* next;  // Linked list of functions
} FunctionDeclaration;

typedef struct {
    char* function_name;
    struct Expression** arguments;  // Array of expressions
    int arg_count;
} FunctionCall;

typedef struct {
    struct Expression* return_value;  // NULL for void return
} ReturnStatement;

// Function management
FunctionDeclaration* function_create(const char* name, FunctionReturnType return_type);
void function_add_param(FunctionDeclaration* func, const char* name, FunctionParamType type);
void function_add_struct_param(FunctionDeclaration* func, const char* name, const char* struct_type);  // YZ_84
void function_set_body(FunctionDeclaration* func, Statement* body);
void function_free(FunctionDeclaration* func);

// Function call
FunctionCall* function_call_create(const char* name);
void function_call_add_arg(FunctionCall* call, struct Expression* arg);
void function_call_free(FunctionCall* call);

// Return statement
ReturnStatement* return_statement_create(struct Expression* value);
void return_statement_free(ReturnStatement* ret);

// Local variable management
void function_register_local_var(FunctionDeclaration* func, const char* name);
void function_register_local_var_with_type(FunctionDeclaration* func, const char* name, int is_numeric);
void function_register_array_var(FunctionDeclaration* func, const char* name, int length, int is_numeric);
int function_var_exists(FunctionDeclaration* func, const char* name);  // YZ_25: Check if variable is declared
const char* function_find_similar_var(FunctionDeclaration* func, const char* name);  // YZ_25: Find similar variable name
void function_register_tuple_var(FunctionDeclaration* func, const char* name, int length);  // YZ_21
void function_register_list_var(FunctionDeclaration* func, const char* name, int length);  // YZ_22
int function_get_var_offset(FunctionDeclaration* func, const char* name);
int function_get_var_is_numeric(FunctionDeclaration* func, const char* name);
int function_get_array_length(FunctionDeclaration* func, const char* name);
int function_is_tuple(FunctionDeclaration* func, const char* name);  // YZ_21
int function_get_tuple_length(FunctionDeclaration* func, const char* name);  // YZ_21
int function_is_list(FunctionDeclaration* func, const char* name);  // YZ_22
int function_get_list_length(FunctionDeclaration* func, const char* name);  // YZ_22

// Builtin function recognition
int function_is_builtin(const char* name);

// YZ_36: Function registry for user-defined functions (module support)
// Check if a function is known (builtin or user-defined)
int function_is_known(const char* name);
void function_register_name(const char* name);
void function_clear_registry(void);

#endif
