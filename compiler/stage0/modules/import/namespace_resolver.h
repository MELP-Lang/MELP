#ifndef NAMESPACE_RESOLVER_H
#define NAMESPACE_RESOLVER_H

#include "../lexer/lexer.h"

// Qualified name structure
// Represents: module.symbol (e.g., "math.add")
typedef struct QualifiedName {
    char* module_name;      // Module name (e.g., "math")
    char* symbol_name;      // Symbol name (e.g., "add")
    char* full_name;        // Full qualified name (e.g., "math.add")
} QualifiedName;

// Qualified name management
QualifiedName* qualified_name_create(const char* module_name, const char* symbol_name);
void qualified_name_free(QualifiedName* qname);

// Parse qualified name from string
// Input: "math.add" or "add"
// Returns: QualifiedName or NULL if not qualified
QualifiedName* qualified_name_parse(const char* name);

// Check if a name is qualified (contains dot)
int qualified_name_is_qualified(const char* name);

// Resolve qualified function call
// Syntax: math.add(10, 20)
// Returns: Function name mangled for LLVM (e.g., "math_add")
char* namespace_resolve_function(const char* qualified_name);

// Check if qualified symbol is accessible
// - Must be imported
// - Must be exported from the module
int namespace_is_accessible(const char* module_name, const char* symbol_name);

// Get mangled name for LLVM IR
// Example: "math.add" -> "math_add"
char* namespace_mangle_name(const char* module_name, const char* symbol_name);

#endif
