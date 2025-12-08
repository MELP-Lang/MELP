#ifndef PRINT_CODEGEN_H
#define PRINT_CODEGEN_H

#include "print.h"
#include <stdio.h>
#include <stdbool.h>

// Generate code for print statement
void codegen_print_statement(FILE* f, PrintStatement* stmt);

// Generate code for printing BigDecimal
void codegen_print_bigdecimal(FILE* f, const char* bigdec_var);

// Generate code for printing SSO string
void codegen_print_sso_string(FILE* f, const char* sso_var);

// Generate code for printing int64
void codegen_print_int64(FILE* f, const char* register_name);

// Finalize code generation (add exit code)
void codegen_print_finalize(FILE* f);

#endif
