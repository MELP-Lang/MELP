#ifndef STRING_INTERPOLATION_H
#define STRING_INTERPOLATION_H

#include "../lexer/lexer.h"

// Forward declaration
typedef struct ArithmeticExpr ArithmeticExpr;

// Check if string contains interpolation ${...}
int string_has_interpolation(const char* str);

// Parse string interpolation and convert to concatenation expression
// Input: "Hello ${name}, you are ${age} years old"
// Output: ArithmeticExpr tree representing: "Hello " + name + ", you are " + age + " years old"
ArithmeticExpr* parse_string_interpolation(Lexer* lexer, const char* str_with_interp);

#endif // STRING_INTERPOLATION_H
