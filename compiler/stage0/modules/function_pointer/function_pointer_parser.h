#ifndef FUNCTION_POINTER_PARSER_H
#define FUNCTION_POINTER_PARSER_H

#include "function_pointer.h"
#include "../parser_core/parser_core.h"
#include "../lexer/lexer.h"

// YZ_209: Function Pointer Parser
// Parses function pointer syntax:
//   function<numeric, numeric, numeric> operation
//   operation = &add
//   result = operation(10, 20)

// Parse function pointer type declaration
// Syntax: function<ReturnType, ParamType1, ParamType2, ...>
FunctionPointerType* parse_function_pointer_type(Token** current);

// Parse function reference
// Syntax: &function_name
FunctionReference* parse_function_reference(Token** current);

// Parse function pointer variable declaration
// Syntax: function<ReturnType, ...> var_name
FunctionPointerVar* parse_function_pointer_var_decl(Token** current);

// Parse function pointer assignment
// Syntax: var_name = &function_name
int parse_function_pointer_assignment(Token** current, FunctionPointerVar* var);

// Parse indirect function call
// Syntax: pointer_var(arg1, arg2, ...)
IndirectCall* parse_indirect_call(Token** current, const char* var_name);

// Helper: Check if current token is function pointer type
int is_function_pointer_type(Token* token);

// Helper: Check if expression is function reference
int is_function_reference(Token* token);

#endif // FUNCTION_POINTER_PARSER_H
