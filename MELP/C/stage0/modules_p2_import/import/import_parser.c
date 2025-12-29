#include "import_parser.h"
#include "../error/error.h"
#include <stdlib.h>
#include <stdio.h>

// Parse import statement
// Syntax: import "module_name"  (YZ_02: Updated to accept string literal)
ImportStatement* import_parse(Lexer* lexer, Token* import_token) {
    // import_token is already consumed (TOKEN_IMPORT)
    
    // Expect string literal or identifier (module name)
    Token* name_token = lexer_next_token(lexer);
    if (!name_token) {
        error_parser(import_token->line, "Expected module name after 'import'");
        return NULL;
    }
    
    // Accept both string literal and identifier for compatibility
    if (name_token->type != TOKEN_STRING && name_token->type != TOKEN_IDENTIFIER) {
        error_parser(name_token->line, "Expected module name (string or identifier), got '%s'", 
                     name_token->value ? name_token->value : "unknown");
        token_free(name_token);
        return NULL;
    }
    
    // Create import statement
    ImportStatement* import_stmt = import_statement_create(name_token->value);
    
    // Resolve module path
    char* resolved_path = import_resolve_module_path(import_stmt->module_name);
    if (!resolved_path) {
        error_parser(name_token->line, "Module '%s' not found in search paths", import_stmt->module_name);
        // Continue anyway (allow compilation, runtime will fail)
        import_stmt->is_resolved = 0;
    } else {
        import_stmt->module_path = resolved_path;
        import_stmt->is_resolved = 1;
    }
    
    token_free(name_token);
    return import_stmt;
}
