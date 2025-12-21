#include "export_tracker.h"
#include "module_declaration.h"
#include "../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global export registry
static ModuleExports* g_export_registry = NULL;

// Initialize export registry
void export_registry_init(void) {
    g_export_registry = NULL;
}

// Free export registry
void export_registry_free(void) {
    ModuleExports* current = g_export_registry;
    while (current) {
        ModuleExports* next = current->next;
        module_exports_free(current);
        current = next;
    }
    g_export_registry = NULL;
}

ExportEntry* export_entry_create(const char* symbol_name, int symbol_type, void* symbol_ptr) {
    ExportEntry* entry = malloc(sizeof(ExportEntry));
    entry->symbol_name = strdup(symbol_name);
    entry->symbol_type = symbol_type;
    entry->symbol_ptr = symbol_ptr;
    entry->next = NULL;
    return entry;
}

void export_entry_free(ExportEntry* entry) {
    if (!entry) return;
    if (entry->symbol_name) free(entry->symbol_name);
    // Note: We don't free symbol_ptr as it's managed elsewhere
    free(entry);
}

ModuleExports* module_exports_create(const char* module_name) {
    ModuleExports* exports = malloc(sizeof(ModuleExports));
    exports->module_name = strdup(module_name);
    exports->exports = NULL;
    exports->next = NULL;
    return exports;
}

void module_exports_free(ModuleExports* exports) {
    if (!exports) return;
    if (exports->module_name) free(exports->module_name);
    
    // Free export entries
    ExportEntry* current = exports->exports;
    while (current) {
        ExportEntry* next = current->next;
        export_entry_free(current);
        current = next;
    }
    
    free(exports);
}

// Find or create module exports
static ModuleExports* find_or_create_module_exports(const char* module_name) {
    // Search for existing module
    ModuleExports* current = g_export_registry;
    while (current) {
        if (strcmp(current->module_name, module_name) == 0) {
            return current;
        }
        current = current->next;
    }
    
    // Create new module exports
    ModuleExports* new_exports = module_exports_create(module_name);
    new_exports->next = g_export_registry;
    g_export_registry = new_exports;
    return new_exports;
}

// Add export to current module
void export_add_symbol(const char* symbol_name, int symbol_type, void* symbol_ptr) {
    ModuleDeclaration* current_module = module_get_current();
    if (!current_module) {
        error_warning(0, "Export outside of module declaration: '%s'", symbol_name);
        return;
    }
    
    // Get or create module exports
    ModuleExports* module_exp = find_or_create_module_exports(current_module->module_name);
    
    // Check if symbol already exported
    ExportEntry* current = module_exp->exports;
    while (current) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            error_warning(0, "Symbol '%s' already exported from module '%s'", 
                         symbol_name, current_module->module_name);
            return;
        }
        current = current->next;
    }
    
    // Create and add new export entry
    ExportEntry* new_entry = export_entry_create(symbol_name, symbol_type, symbol_ptr);
    new_entry->next = module_exp->exports;
    module_exp->exports = new_entry;
    
    printf("✅ Exported: %s from module %s\n", symbol_name, current_module->module_name);
}

// Get exports for a module
ModuleExports* export_get_module_exports(const char* module_name) {
    ModuleExports* current = g_export_registry;
    while (current) {
        if (strcmp(current->module_name, module_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Check if a symbol is exported from a module
int export_is_symbol_exported(const char* module_name, const char* symbol_name) {
    ModuleExports* module_exp = export_get_module_exports(module_name);
    if (!module_exp) return 0;
    
    ExportEntry* current = module_exp->exports;
    while (current) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Get exported symbol pointer
void* export_get_symbol(const char* module_name, const char* symbol_name) {
    ModuleExports* module_exp = export_get_module_exports(module_name);
    if (!module_exp) return NULL;
    
    ExportEntry* current = module_exp->exports;
    while (current) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            return current->symbol_ptr;
        }
        current = current->next;
    }
    return NULL;
}

// Parse export statement
// Syntax: export function add() ... end_function
// Returns 1 if export keyword was consumed, 0 otherwise
int export_parse_keyword(Lexer* lexer, Token* export_token) {
    // export_token is already consumed (TOKEN_EXPORT)
    
    // Check if we're in a module
    ModuleDeclaration* current_module = module_get_current();
    if (!current_module) {
        error_parser(export_token->line, "Export statement outside of module declaration");
        return 0;
    }
    
    // For now, we just mark that an export was requested
    // The actual function/variable parsing will happen in the caller
    // and they will call export_add_symbol()
    
    return 1;  // Export keyword consumed successfully
}
