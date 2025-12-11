#ifndef IMPORT_H
#define IMPORT_H

// Forward declaration for FunctionDeclaration (defined in functions.h)
typedef struct FunctionDeclaration FunctionDeclaration;

// Import statement structure
// Syntax: import module_name
typedef struct ImportStatement {
    char* module_name;         // Name of the module to import (e.g., "math", "string_utils")
    char* module_path;         // Resolved file path (e.g., "modules/core/math.mlp")
    int is_resolved;           // 1 if path is resolved, 0 otherwise
    FunctionDeclaration* functions;  // YZ_36: Functions from the imported module
    struct ImportStatement* next;    // YZ_36: Linked list of imports
} ImportStatement;

// Import statement management
ImportStatement* import_statement_create(const char* module_name);
void import_statement_free(ImportStatement* import_stmt);

// Module path resolution
// Search order:
// 1. ./modules/core/module_name.mlp
// 2. ./modules/advanced/module_name.mlp  
// 3. ./modules/experimental/module_name.mlp
// 4. ./module_name.mlp (current directory)
char* import_resolve_module_path(const char* module_name);

// YZ_36: Module loading
// Load and parse a module file, returning its functions
// Returns NULL on error
FunctionDeclaration* import_load_module(const char* module_path);

// YZ_44 Part 5.1: Per-Module Assembly Generation
// Generate assembly file for a module
// Returns 1 on success, 0 on failure
int import_generate_module_assembly(const char* module_path, FunctionDeclaration* functions);

#endif