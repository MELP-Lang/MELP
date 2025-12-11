#ifndef IMPORT_H
#define IMPORT_H

// Import statement structure
// Syntax: import module_name
typedef struct {
    char* module_name;     // Name of the module to import (e.g., "math", "string_utils")
    char* module_path;     // Resolved file path (e.g., "modules/core/math.mlp")
    int is_resolved;       // 1 if path is resolved, 0 otherwise
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

#endif
