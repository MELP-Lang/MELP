#include "module_declaration.h"
#include "../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global current module being compiled
static ModuleDeclaration* g_current_module = NULL;

ModuleDeclaration* module_declaration_create(const char* module_name, const char* module_path) {
    ModuleDeclaration* module = malloc(sizeof(ModuleDeclaration));
    module->module_name = strdup(module_name);
    module->module_path = module_path ? strdup(module_path) : NULL;
    module->is_current_module = 0;
    return module;
}

void module_declaration_free(ModuleDeclaration* module_decl) {
    if (!module_decl) return;
    if (module_decl->module_name) free(module_decl->module_name);
    if (module_decl->module_path) free(module_decl->module_path);
    free(module_decl);
}

// Parse module declaration
// Syntax: module module_name
ModuleDeclaration* module_declaration_parse(Lexer* lexer, Token* module_token) {
    // module_token is already consumed (TOKEN_MODULE)
    
    // Expect identifier (module name)
    Token* name_token = lexer_next_token(lexer);
    if (!name_token) {
        error_parser(module_token->line, "Expected module name after 'module'");
        return NULL;
    }
    
    if (name_token->type != TOKEN_IDENTIFIER) {
        error_parser(name_token->line, "Expected module name (identifier), got '%s'", 
                     name_token->value ? name_token->value : "unknown");
        token_free(name_token);
        return NULL;
    }
    
    // Create module declaration
    ModuleDeclaration* module = module_declaration_create(name_token->value, NULL);
    module->is_current_module = 1;
    
    token_free(name_token);
    
    // Set as current module
    module_set_current(module);
    
    return module;
}

// Global module state management
void module_set_current(ModuleDeclaration* module) {
    // Free previous module if exists
    if (g_current_module && g_current_module != module) {
        module_declaration_free(g_current_module);
    }
    g_current_module = module;
}

ModuleDeclaration* module_get_current(void) {
    return g_current_module;
}

// Check if a symbol belongs to the current module
// For now, always return 1 (local) if no module is declared
int module_is_local_symbol(const char* symbol_name) {
    if (!g_current_module) {
        return 1;  // No module declared, all symbols are local
    }
    
    // Check if symbol has namespace prefix (e.g., "math.add")
    const char* dot = strchr(symbol_name, '.');
    if (!dot) {
        return 1;  // No namespace prefix, assume local
    }
    
    // Extract namespace
    size_t namespace_len = dot - symbol_name;
    char namespace[256];
    if (namespace_len >= sizeof(namespace)) {
        return 0;  // Namespace too long, not local
    }
    strncpy(namespace, symbol_name, namespace_len);
    namespace[namespace_len] = '\0';
    
    // Check if namespace matches current module
    return strcmp(namespace, g_current_module->module_name) == 0;
}
