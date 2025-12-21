#include "namespace_resolver.h"
#include "export_tracker.h"
#include "import.h"
#include "../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

QualifiedName* qualified_name_create(const char* module_name, const char* symbol_name) {
    QualifiedName* qname = malloc(sizeof(QualifiedName));
    qname->module_name = module_name ? strdup(module_name) : NULL;
    qname->symbol_name = strdup(symbol_name);
    
    // Build full name
    if (module_name) {
        size_t len = strlen(module_name) + strlen(symbol_name) + 2; // +2 for '.' and '\0'
        qname->full_name = malloc(len);
        snprintf(qname->full_name, len, "%s.%s", module_name, symbol_name);
    } else {
        qname->full_name = strdup(symbol_name);
    }
    
    return qname;
}

void qualified_name_free(QualifiedName* qname) {
    if (!qname) return;
    if (qname->module_name) free(qname->module_name);
    if (qname->symbol_name) free(qname->symbol_name);
    if (qname->full_name) free(qname->full_name);
    free(qname);
}

// Parse qualified name from string
// Input: "math.add" or "add"
// Returns: QualifiedName or NULL if parse error
QualifiedName* qualified_name_parse(const char* name) {
    if (!name) return NULL;
    
    const char* dot = strchr(name, '.');
    if (!dot) {
        // Not qualified, just symbol name
        return qualified_name_create(NULL, name);
    }
    
    // Split into module and symbol
    size_t module_len = dot - name;
    char* module_name = strndup(name, module_len);
    const char* symbol_name = dot + 1;
    
    QualifiedName* qname = qualified_name_create(module_name, symbol_name);
    free(module_name);
    
    return qname;
}

// Check if a name is qualified (contains dot)
int qualified_name_is_qualified(const char* name) {
    return name && strchr(name, '.') != NULL;
}

// Resolve qualified function call
// Syntax: math.add(10, 20)
// Returns: Function name mangled for LLVM (e.g., "math_add")
char* namespace_resolve_function(const char* qualified_name) {
    if (!qualified_name) return NULL;
    
    QualifiedName* qname = qualified_name_parse(qualified_name);
    if (!qname) return NULL;
    
    // If not qualified, return as-is
    if (!qname->module_name) {
        char* result = strdup(qname->symbol_name);
        qualified_name_free(qname);
        return result;
    }
    
    // Check if accessible
    if (!namespace_is_accessible(qname->module_name, qname->symbol_name)) {
        error_fatal("Symbol '%s' not accessible from module '%s'", 
                   qname->symbol_name, qname->module_name);
        qualified_name_free(qname);
        return NULL;
    }
    
    // Mangle name for LLVM
    char* mangled = namespace_mangle_name(qname->module_name, qname->symbol_name);
    qualified_name_free(qname);
    
    return mangled;
}

// Check if qualified symbol is accessible
// - Must be imported
// - Must be exported from the module
int namespace_is_accessible(const char* module_name, const char* symbol_name) {
    if (!module_name || !symbol_name) return 0;
    
    // Check if module is imported
    // TODO: Check import list from current compilation unit
    // For now, assume all modules are accessible if they export the symbol
    
    // Check if symbol is exported
    return export_is_symbol_exported(module_name, symbol_name);
}

// Get mangled name for LLVM IR
// Example: "math.add" -> "math_add"
char* namespace_mangle_name(const char* module_name, const char* symbol_name) {
    if (!module_name || !symbol_name) {
        return symbol_name ? strdup(symbol_name) : NULL;
    }
    
    size_t len = strlen(module_name) + strlen(symbol_name) + 2; // +2 for '_' and '\0'
    char* mangled = malloc(len);
    snprintf(mangled, len, "%s_%s", module_name, symbol_name);
    
    return mangled;
}
