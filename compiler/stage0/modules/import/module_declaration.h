#ifndef MODULE_DECLARATION_H
#define MODULE_DECLARATION_H

#include "../lexer/lexer.h"

// Module declaration structure
// Syntax: module math
typedef struct ModuleDeclaration {
    char* module_name;         // Name of the module (e.g., "math", "string_utils")
    char* module_path;         // Full path of the module file
    int is_current_module;     // 1 if this is the module being compiled, 0 otherwise
} ModuleDeclaration;

// Module declaration management
ModuleDeclaration* module_declaration_create(const char* module_name, const char* module_path);
void module_declaration_free(ModuleDeclaration* module_decl);

// Parse module declaration
// Syntax: module module_name
ModuleDeclaration* module_declaration_parse(Lexer* lexer, Token* module_token);

// Global module state
void module_set_current(ModuleDeclaration* module);
ModuleDeclaration* module_get_current(void);

// Check if a symbol belongs to the current module
int module_is_local_symbol(const char* symbol_name);

#endif
