#include "import.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

// YZ_36: Need to include lexer and parser for module loading
#include "../lexer/lexer.h"
#include "../functions/functions_parser.h"
#include "../error/error.h"

ImportStatement* import_statement_create(const char* module_name) {
    ImportStatement* stmt = malloc(sizeof(ImportStatement));
    stmt->module_name = strdup(module_name);
    stmt->module_path = NULL;
    stmt->is_resolved = 0;
    stmt->functions = NULL;   // YZ_36: No functions yet
    stmt->next = NULL;        // YZ_36: End of list
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

// YZ_36: Load and parse a module file
FunctionDeclaration* import_load_module(const char* module_path) {
    // Read module file
    FILE* file = fopen(module_path, "r");
    if (!file) {
        error_fatal("Cannot open module file: %s", module_path);
        return NULL;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* source = malloc(size + 1);
    if (!source) {
        fclose(file);
        error_fatal("Memory allocation failed for module: %s", module_path);
        return NULL;
    }
    
    fread(source, 1, size, file);
    source[size] = '\0';
    fclose(file);
    
    // YZ_36: Save current error context
    // Note: error_set_source() changes global state, we need to restore it later
    // For now, we'll parse without setting source (errors will be less helpful)
    // TODO: Implement error context save/restore
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        free(source);
        error_fatal("Failed to create lexer for module: %s", module_path);
        return NULL;
    }
    
    // Parse all functions in the module
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    
    while (1) {
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // Module files should only contain function declarations
        if (tok->type == TOKEN_IMPORT) {
            error_warning(tok->line, "Import statements in modules are not yet supported");
            token_free(tok);
            continue;
        }
        
        // Put token back for function parser
        lexer_unget_token(lexer, tok);
        
        // Parse function
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) {
            break;  // Parse error
        }
        
        // Add to list
        if (!functions) {
            functions = func;
            last_func = func;
        } else {
            last_func->next = func;
            last_func = func;
        }
    }
    
    lexer_free(lexer);
    free(source);
    
    return functions;
}
