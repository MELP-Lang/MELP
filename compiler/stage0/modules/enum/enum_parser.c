#include "enum_parser.h"
#include "../error/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// YZ_96: Enum Parser Implementation
// ============================================================================

// Parse enum definition
// Syntax:
//   enum Status
//       PENDING = 0
//       ACTIVE = 1
//       DONE = 2
//   end_enum
EnumDefinition* enum_parse(Lexer* lexer, Token* enum_tok) {
    if (!lexer || !enum_tok) {
        error_fatal("enum_parse - NULL lexer or token");
        return NULL;
    }
    
    // Get enum name
    Token* name_tok = lexer_next_token(lexer);
    if (!name_tok || name_tok->type != TOKEN_IDENTIFIER) {
        error_fatal("Expected enum name after 'enum' keyword");
        if (name_tok) token_free(name_tok);
        return NULL;
    }
    
    // Create enum definition
    EnumDefinition* def = enum_definition_create(name_tok->value);
    token_free(name_tok);
    
    if (!def) {
        error_fatal("Failed to create enum definition");
        return NULL;
    }
    
    // Parse enum values
    int64_t auto_value = 0;  // Auto-increment for values without explicit assignment
    
    while (1) {
        Token* tok = lexer_next_token(lexer);
        if (!tok) {
            error_fatal("Unexpected end of file in enum definition");
            enum_definition_free(def);
            return NULL;
        }
        
        // Check for end_enum
        if (tok->type == TOKEN_END_ENUM) {
            token_free(tok);
            break;
        }
        
        // Expect identifier (value name)
        if (tok->type != TOKEN_IDENTIFIER) {
            // Skip EOF token (shouldn't happen in well-formed code)
            if (tok->type == TOKEN_EOF) {
                token_free(tok);
                break;
            }
            error_fatal("Expected enum value name or 'end_enum'");
            token_free(tok);
            enum_definition_free(def);
            return NULL;
        }
        
        char* value_name = strdup(tok->value);
        token_free(tok);
        
        // Check for optional '=' and value
        Token* next = lexer_next_token(lexer);
        int64_t value;
        
        if (next && next->type == TOKEN_ASSIGN) {
            token_free(next);
            
            // Get the numeric value
            Token* num_tok = lexer_next_token(lexer);
            if (!num_tok || num_tok->type != TOKEN_NUMBER) {
                error_fatal("Expected numeric value after '=' in enum");
                if (num_tok) token_free(num_tok);
                free(value_name);
                enum_definition_free(def);
                return NULL;
            }
            
            value = atoll(num_tok->value);
            auto_value = value + 1;  // Next auto value
            token_free(num_tok);
        } else {
            // No explicit value, use auto-increment
            value = auto_value++;
            
            // Put back the token if it's not '='
            if (next) {
                lexer_unget_token(lexer, next);
            }
        }
        
        // Create and add enum value
        EnumValue* enum_val = enum_value_create(value_name, value);
        free(value_name);
        
        if (!enum_val) {
            error_fatal("Failed to create enum value");
            enum_definition_free(def);
            return NULL;
        }
        
        enum_definition_add_value(def, enum_val);
    }
    
    // Note: Caller is responsible for adding to their list
    // enum_register() removed to avoid double-linking
    
    return def;
}

// Parse enum value reference (e.g., "Status.ACTIVE")
// Returns the numeric value, or -1 if not found
int64_t enum_parse_value_reference(const char* reference) {
    if (!reference) return -1;
    
    // Find the dot separator
    const char* dot = strchr(reference, '.');
    if (!dot) return -1;
    
    // Extract enum name
    size_t enum_name_len = dot - reference;
    char* enum_name = malloc(enum_name_len + 1);
    if (!enum_name) return -1;
    
    strncpy(enum_name, reference, enum_name_len);
    enum_name[enum_name_len] = '\0';
    
    // Extract value name
    const char* value_name = dot + 1;
    
    // Lookup the value
    int64_t result = enum_lookup_value(enum_name, value_name);
    
    free(enum_name);
    return result;
}
