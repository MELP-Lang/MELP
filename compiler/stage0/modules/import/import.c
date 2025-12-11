#include "import.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

ImportStatement* import_statement_create(const char* module_name) {
    ImportStatement* stmt = malloc(sizeof(ImportStatement));
    stmt->module_name = strdup(module_name);
    stmt->module_path = NULL;
    stmt->is_resolved = 0;
    return stmt;
}

void import_statement_free(ImportStatement* import_stmt) {
    if (!import_stmt) return;
    if (import_stmt->module_name) free(import_stmt->module_name);
    if (import_stmt->module_path) free(import_stmt->module_path);
    free(import_stmt);
}

// Helper: Check if file exists
static int file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

char* import_resolve_module_path(const char* module_name) {
    char path[512];
    
    // Search order:
    // 1. modules/core/module_name.mlp
    snprintf(path, sizeof(path), "modules/core/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 2. modules/advanced/module_name.mlp
    snprintf(path, sizeof(path), "modules/advanced/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 3. modules/experimental/module_name.mlp
    snprintf(path, sizeof(path), "modules/experimental/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 4. module_name.mlp (current directory)
    snprintf(path, sizeof(path), "%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // Not found
    return NULL;
}
