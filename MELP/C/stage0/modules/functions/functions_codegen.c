#include <stdio.h>
#include <stdlib.h>
#include "functions_codegen.h"
#include "functions.h"
#include "../codegen_emit/codegen_emit.h"

// Forward declaration for expression (Module #9, not yet available)
// Using stub for now until expression module is integrated
typedef struct Expression Expression;

void expression_generate_code_stub(FILE* output, Expression* expr) {
    (void)expr;  // Unused parameter
    emit_c("        // Expression code generation (stub - Module #9 not integrated yet)\n");
    emit_c("        return 0;  // Placeholder\n");
}

#define expression_generate_code expression_generate_code_stub

// Map MLP types to C types
static const char* get_c_type(FunctionParamType type) {
    switch (type) {
        case FUNC_PARAM_NUMERIC: return "int64_t";
        case FUNC_PARAM_TEXT: return "const char*";
        case FUNC_PARAM_BOOLEAN: return "bool";
        case FUNC_PARAM_POINTER: return "void*";
        case FUNC_PARAM_ARRAY: return "void*";
        default: return "void*";
    }
}

static const char* get_c_return_type(FunctionReturnType type) {
    switch (type) {
        case FUNC_RETURN_NUMERIC: return "int64_t";
        case FUNC_RETURN_TEXT: return "const char*";
        case FUNC_RETURN_BOOLEAN: return "bool";
        case FUNC_RETURN_VOID: return "void";
        default: return "void";
    }
}

// Generate function prologue
void function_generate_prologue(FILE* output, FunctionDeclaration* func) {
    emit_c("\n// Function: %s\n", func->name);
    
    // Function signature
    emit_c("%s %s(", get_c_return_type(func->return_type), func->name);
    
    // Parameters
    FunctionParam* param = func->params;
    int param_index = 0;
    while (param) {
        if (param_index > 0) emit_c(", ");
        emit_c("%s %s", get_c_type(param->type), param->name);
        param = param->next;
        param_index++;
    }
    
    emit_c(") {\n");
}

// Generate function epilogue
void function_generate_epilogue(FILE* output, FunctionDeclaration* func) {
    emit_c("}\n");
}

// Generate function declaration
void function_generate_declaration(FILE* output, FunctionDeclaration* func) {
    if (!func) return;
    
    function_generate_prologue(output, func);
    
    // Function body code generation would go here
    // This is handled by statement codegen in main compilation loop
    emit_c("    // Function body goes here\n");
    
    function_generate_epilogue(output, func);
}

// Generate function call
void function_generate_call(FILE* output, FunctionCall* call) {
    if (!call) return;
    
    emit_c("    %s(", call->function_name);
    
    // Arguments
    for (int i = 0; i < call->arg_count; i++) {
        if (i > 0) emit_c(", ");
        if (call->arguments && call->arguments[i]) {
            // For now, just emit a placeholder
            // Real expression codegen will handle this
            emit_c("arg%d", i);
        } else {
            emit_c("0");
        }
    }
    
    emit_c(");\n");
}

// Generate return statement
void function_generate_return(FILE* output, ReturnStatement* ret) {
    if (!ret) return;
    
    if (ret->return_value) {
        emit_c("    return ");
        // Evaluate return expression
        // Real expression codegen will handle this
        emit_c("0");  // Placeholder
        emit_c(";\n");
    } else {
        emit_c("    return;\n");
    }
}
