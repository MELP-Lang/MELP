// Symbol Table Implementation
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

// Create symbol table
SymbolTable* symbol_table_create() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    table->head = NULL;
    table->count = 0;
    table->current_scope_level = 0;
    
    return table;
}

// Free symbol table
void symbol_table_free(SymbolTable* table) {
    if (!table) return;
    
    Symbol* current = table->head;
    while (current) {
        Symbol* next = current->next;
        free(current->name);
        free(current->type_name);
        free(current->signature);
        free(current->documentation);
        free(current->file_uri);
        free(current);
        current = next;
    }
    
    free(table);
}

// Create symbol
Symbol* symbol_create(const char* name, SymbolKind kind, const char* type_name) {
    Symbol* symbol = malloc(sizeof(Symbol));
    if (!symbol) return NULL;
    
    symbol->name = strdup(name);
    symbol->kind = kind;
    symbol->type_name = type_name ? strdup(type_name) : NULL;
    symbol->signature = NULL;
    symbol->documentation = NULL;
    symbol->file_uri = NULL;
    symbol->scope = SCOPE_GLOBAL;
    symbol->scope_level = 0;
    symbol->next = NULL;
    
    // Initialize position/range to 0
    symbol->position.line = 0;
    symbol->position.character = 0;
    symbol->range.start.line = 0;
    symbol->range.start.character = 0;
    symbol->range.end.line = 0;
    symbol->range.end.character = 0;
    
    return symbol;
}

// Add symbol to table
void symbol_table_add(SymbolTable* table, Symbol* symbol) {
    if (!table || !symbol) return;
    
    // Set scope level
    symbol->scope_level = table->current_scope_level;
    if (symbol->scope_level == 0) {
        symbol->scope = SCOPE_GLOBAL;
    } else {
        symbol->scope = SCOPE_FUNCTION;  // Or SCOPE_BLOCK
    }
    
    // Add to front of list
    symbol->next = table->head;
    table->head = symbol;
    table->count++;
}

// Find symbol by name (searches all scopes)
Symbol* symbol_table_find(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    Symbol* current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Find symbol in specific scope (or parent scopes)
Symbol* symbol_table_find_in_scope(SymbolTable* table, const char* name, int scope_level) {
    if (!table || !name) return NULL;
    
    Symbol* current = table->head;
    while (current) {
        // Match name and check if symbol is in current or parent scope
        if (strcmp(current->name, name) == 0 && current->scope_level <= scope_level) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Find all symbols with given name (returns array)
Symbol** symbol_table_find_all(SymbolTable* table, const char* name, int* count) {
    if (!table || !name || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    // Count matches first
    int match_count = 0;
    Symbol* current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            match_count++;
        }
        current = current->next;
    }
    
    if (match_count == 0) {
        *count = 0;
        return NULL;
    }
    
    // Allocate array
    Symbol** results = malloc(sizeof(Symbol*) * match_count);
    if (!results) {
        *count = 0;
        return NULL;
    }
    
    // Fill array
    int index = 0;
    current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            results[index++] = current;
        }
        current = current->next;
    }
    
    *count = match_count;
    return results;
}

// Get all symbols
Symbol** symbol_table_get_all(SymbolTable* table, int* count) {
    if (!table || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    if (table->count == 0) {
        *count = 0;
        return NULL;
    }
    
    Symbol** results = malloc(sizeof(Symbol*) * table->count);
    if (!results) {
        *count = 0;
        return NULL;
    }
    
    int index = 0;
    Symbol* current = table->head;
    while (current) {
        results[index++] = current;
        current = current->next;
    }
    
    *count = table->count;
    return results;
}

// Get symbols by kind
Symbol** symbol_table_get_by_kind(SymbolTable* table, SymbolKind kind, int* count) {
    if (!table || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    // Count matches
    int match_count = 0;
    Symbol* current = table->head;
    while (current) {
        if (current->kind == kind) {
            match_count++;
        }
        current = current->next;
    }
    
    if (match_count == 0) {
        *count = 0;
        return NULL;
    }
    
    // Allocate and fill
    Symbol** results = malloc(sizeof(Symbol*) * match_count);
    if (!results) {
        *count = 0;
        return NULL;
    }
    
    int index = 0;
    current = table->head;
    while (current) {
        if (current->kind == kind) {
            results[index++] = current;
        }
        current = current->next;
    }
    
    *count = match_count;
    return results;
}

// Enter new scope
void symbol_table_enter_scope(SymbolTable* table) {
    if (table) {
        table->current_scope_level++;
    }
}

// Exit scope (remove symbols from current scope)
void symbol_table_exit_scope(SymbolTable* table) {
    if (!table || table->current_scope_level == 0) return;
    
    // Remove symbols from current scope
    Symbol** prev_next = &table->head;
    Symbol* current = table->head;
    
    while (current) {
        if (current->scope_level == table->current_scope_level) {
            // Remove this symbol
            *prev_next = current->next;
            Symbol* to_free = current;
            current = current->next;
            
            free(to_free->name);
            free(to_free->type_name);
            free(to_free->signature);
            free(to_free->documentation);
            free(to_free->file_uri);
            free(to_free);
            
            table->count--;
        } else {
            prev_next = &current->next;
            current = current->next;
        }
    }
    
    table->current_scope_level--;
}

// Convert symbol kind to string
const char* symbol_kind_to_string(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_FUNCTION: return "function";
        case SYMBOL_VARIABLE: return "variable";
        case SYMBOL_PARAMETER: return "parameter";
        case SYMBOL_TYPE: return "type";
        case SYMBOL_IMPORT: return "import";
        case SYMBOL_KEYWORD: return "keyword";
        default: return "unknown";
    }
}

// Convert symbol kind to LSP completion kind
int symbol_kind_to_completion_kind(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_FUNCTION: return COMPLETION_FUNCTION;
        case SYMBOL_VARIABLE: return COMPLETION_VARIABLE;
        case SYMBOL_PARAMETER: return COMPLETION_VARIABLE;
        case SYMBOL_TYPE: return COMPLETION_CLASS;
        case SYMBOL_IMPORT: return COMPLETION_MODULE;
        case SYMBOL_KEYWORD: return COMPLETION_KEYWORD;
        default: return COMPLETION_TEXT;
    }
}
