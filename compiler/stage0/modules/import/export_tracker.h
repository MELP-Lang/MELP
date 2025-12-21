#ifndef EXPORT_TRACKER_H
#define EXPORT_TRACKER_H

#include "../lexer/lexer.h"
#include "../functions/functions_parser.h"

// Export entry structure
// Tracks exported symbols from a module
typedef struct ExportEntry {
    char* symbol_name;              // Name of the exported symbol (e.g., "add")
    enum {
        EXPORT_FUNCTION,
        EXPORT_VARIABLE,
        EXPORT_STRUCT,
        EXPORT_ENUM
    } symbol_type;
    void* symbol_ptr;               // Pointer to the symbol (FunctionDeclaration*, etc.)
    struct ExportEntry* next;       // Linked list of exports
} ExportEntry;

// Module exports structure
typedef struct ModuleExports {
    char* module_name;              // Module name (e.g., "math")
    ExportEntry* exports;           // Linked list of exported symbols
    struct ModuleExports* next;     // Linked list of modules
} ModuleExports;

// Export tracking management
ExportEntry* export_entry_create(const char* symbol_name, int symbol_type, void* symbol_ptr);
void export_entry_free(ExportEntry* entry);

ModuleExports* module_exports_create(const char* module_name);
void module_exports_free(ModuleExports* exports);

// Add export to current module
void export_add_symbol(const char* symbol_name, int symbol_type, void* symbol_ptr);

// Get exports for a module
ModuleExports* export_get_module_exports(const char* module_name);

// Check if a symbol is exported from a module
int export_is_symbol_exported(const char* module_name, const char* symbol_name);

// Get exported symbol pointer
void* export_get_symbol(const char* module_name, const char* symbol_name);

// Parse export statement
// Syntax: export function add() ... end_function
// Returns 1 if export keyword was consumed, 0 otherwise
int export_parse_keyword(Lexer* lexer, Token* export_token);

// Global export registry
void export_registry_init(void);
void export_registry_free(void);

#endif
