// Symbol Table for MLP LSP
// Tracks symbols (functions, variables, types) across the codebase
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include "lsp_types.h"

// Symbol kinds
typedef enum {
    SYMBOL_FUNCTION,
    SYMBOL_VARIABLE,
    SYMBOL_PARAMETER,
    SYMBOL_TYPE,
    SYMBOL_IMPORT,
    SYMBOL_KEYWORD
} SymbolKind;

// Scope types
typedef enum {
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_BLOCK
} ScopeType;

// Symbol entry
typedef struct Symbol {
    char* name;
    SymbolKind kind;
    char* type_name;        // e.g., "Integer", "String"
    char* signature;        // For functions: "fn(Integer, String) -> Bool"
    char* documentation;    // Optional documentation string
    
    // Location in source
    char* file_uri;
    Position position;      // Definition position
    Range range;            // Full definition range
    
    // Scope information
    ScopeType scope;
    int scope_level;        // 0 = global, 1+ = nested
    
    struct Symbol* next;    // Linked list
} Symbol;

// Symbol table
typedef struct {
    Symbol* head;
    int count;
    int current_scope_level;
} SymbolTable;

// Symbol table operations
SymbolTable* symbol_table_create();
void symbol_table_free(SymbolTable* table);

// Add symbols
void symbol_table_add(SymbolTable* table, Symbol* symbol);
Symbol* symbol_create(const char* name, SymbolKind kind, const char* type_name);

// Find symbols
Symbol* symbol_table_find(SymbolTable* table, const char* name);
Symbol* symbol_table_find_in_scope(SymbolTable* table, const char* name, int scope_level);
Symbol** symbol_table_find_all(SymbolTable* table, const char* name, int* count);

// Get all symbols (for completion)
Symbol** symbol_table_get_all(SymbolTable* table, int* count);
Symbol** symbol_table_get_by_kind(SymbolTable* table, SymbolKind kind, int* count);

// Scope management
void symbol_table_enter_scope(SymbolTable* table);
void symbol_table_exit_scope(SymbolTable* table);

// Utility
const char* symbol_kind_to_string(SymbolKind kind);
int symbol_kind_to_completion_kind(SymbolKind kind);

#endif // SYMBOL_TABLE_H
