#include "struct_parser.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../statement/statement_parser.h"  // YZ_88: For method body parsing
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// YZ_88: Parse struct definition with Parser context for method bodies
// PMPL Syntax: 
//   struct Person
//       string name
//       numeric age
//       
//       method greet() returns string
//           return "Hello"
//       end_method
//   end_struct
StructDef* parse_struct_definition(Parser* parser) {
    Lexer* lexer = parser->lexer;  // YZ_88: Extract lexer from parser
    
    // Expect struct name
    Token* token = lexer_next_token(lexer);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected struct name after 'struct'\n");
        if (token) token_free(token);
        return NULL;
    }
    
    StructDef* def = struct_create_definition(token->value);
    token_free(token);
    
    // Parse members and methods until TOKEN_END_STRUCT
    while (1) {
        token = lexer_next_token(lexer);
        
        if (!token || token->type == TOKEN_EOF) {
            fprintf(stderr, "Error: Unexpected EOF in struct definition\n");
            if (token) token_free(token);
            struct_def_free(def);
            return NULL;
        }
        
        // Check for end_struct
        if (token->type == TOKEN_END_STRUCT) {
            token_free(token);
            break;
        }
        
        // YZ_86: Check for method definition
        if (token->type == TOKEN_METHOD) {
            token_free(token);
            
            // Parse method name
            token = lexer_next_token(lexer);
            if (!token || token->type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Error: Expected method name after 'method'\n");
                if (token) token_free(token);
                struct_def_free(def);
                return NULL;
            }
            
            char* method_name = strdup(token->value);
            token_free(token);
            
            // Expect '('
            token = lexer_next_token(lexer);
            if (!token || token->type != TOKEN_LPAREN) {
                fprintf(stderr, "Error: Expected '(' after method name %s\n", method_name);
                free(method_name);
                if (token) token_free(token);
                struct_def_free(def);
                return NULL;
            }
            token_free(token);
            
            // Parse parameters (excluding implicit 'self')
            StructMethod* method = struct_method_create(method_name, "void");  // Default return type
            free(method_name);
            
            while (1) {
                token = lexer_next_token(lexer);
                
                if (!token) {
                    fprintf(stderr, "Error: Unexpected EOF in method parameters\n");
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
                
                if (token->type == TOKEN_RPAREN) {
                    token_free(token);
                    break;
                }
                
                // Parse parameter type
                if (token->type != TOKEN_NUMERIC && 
                    token->type != TOKEN_STRING_TYPE && 
                    token->type != TOKEN_BOOLEAN &&
                    token->type != TOKEN_IDENTIFIER) {
                    fprintf(stderr, "Error: Expected parameter type\n");
                    token_free(token);
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
                
                char* param_type = strdup(token->value);
                token_free(token);
                
                // Parse parameter name
                token = lexer_next_token(lexer);
                if (!token || token->type != TOKEN_IDENTIFIER) {
                    fprintf(stderr, "Error: Expected parameter name\n");
                    free(param_type);
                    if (token) token_free(token);
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
                
                struct_method_add_param(method, token->value, param_type);
                free(param_type);
                token_free(token);
                
                // Check for comma or closing paren
                token = lexer_next_token(lexer);
                if (!token) {
                    fprintf(stderr, "Error: Unexpected EOF after parameter\n");
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
                
                if (token->type == TOKEN_RPAREN) {
                    token_free(token);
                    break;
                } else if (token->type == TOKEN_COMMA) {
                    token_free(token);
                    continue;
                } else {
                    fprintf(stderr, "Error: Expected ',' or ')' in parameter list\n");
                    token_free(token);
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
            }
            
            // Check for 'returns' keyword
            token = lexer_next_token(lexer);
            if (token && token->type == TOKEN_RETURNS) {
                token_free(token);
                
                // Parse return type
                token = lexer_next_token(lexer);
                if (!token || (token->type != TOKEN_NUMERIC && 
                               token->type != TOKEN_STRING_TYPE && 
                               token->type != TOKEN_BOOLEAN &&
                               token->type != TOKEN_IDENTIFIER)) {
                    fprintf(stderr, "Error: Expected return type after 'returns'\n");
                    if (token) token_free(token);
                    struct_method_free(method);
                    struct_def_free(def);
                    return NULL;
                }
                
                free(method->return_type);
                method->return_type = strdup(token->value);
                token_free(token);
            } else if (token) {
                // No returns keyword, push back token
                lexer_unget_token(lexer, token);
            }
            
            // YZ_88: Parse method body using full Parser context!
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Statement* body_stmt = statement_parse(parser);
                if (!body_stmt) {
                    break;  // end_method encountered
                }
                
                if (!body_head) {
                    body_head = body_stmt;
                    body_tail = body_stmt;
                } else {
                    body_tail->next = body_stmt;
                    body_tail = body_stmt;
                }
            }
            
            // Store parsed body
            method->body = body_head;
            
            // Add method to struct
            struct_add_method(def, method);
            continue;
        }
        
        // Parse member: type name
        // First token should be type (numeric, string, boolean, or custom struct)
        if (token->type != TOKEN_NUMERIC && 
            token->type != TOKEN_STRING_TYPE && 
            token->type != TOKEN_BOOLEAN &&
            token->type != TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Expected type in struct member\n");
            token_free(token);
            struct_def_free(def);
            return NULL;
        }
        
        char* member_type = strdup(token->value);
        token_free(token);
        
        // Expect member name
        token = lexer_next_token(lexer);
        if (!token || token->type != TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Expected member name after type %s\n", member_type);
            free(member_type);
            if (token) token_free(token);
            struct_def_free(def);
            return NULL;
        }
        
        struct_add_member(def, token->value, member_type);
        free(member_type);
        token_free(token);
    }
    
    struct_calculate_layout(def);
    return def;
}

// Parse struct instance
// Syntax: Person p = Person { name: "Ali", age: 25 }
StructInstance* parse_struct_instance(Lexer* lexer) {
    Token* token = lexer_next_token(lexer);
    
    // Parse struct type name
    if (token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected struct type name\n");
        token_free(token);
        return NULL;
    }
    
    char* struct_type = strdup(token->value);
    token_free(token);
    
    // Parse instance variable name
    token = lexer_next_token(lexer);
    if (token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected instance variable name\n");
        free(struct_type);
        token_free(token);
        return NULL;
    }
    
    char* instance_name = strdup(token->value);
    token_free(token);
    
    // Expect '='
    token = lexer_next_token(lexer);
    if (token->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' for struct initialization\n");
        free(struct_type);
        free(instance_name);
        token_free(token);
        return NULL;
    }
    token_free(token);
    
    // Parse struct initializer: struct_type { ... }
    token = lexer_next_token(lexer);
    if (token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected struct type in initializer\n");
        free(struct_type);
        free(instance_name);
        token_free(token);
        return NULL;
    }
    token_free(token);
    
    // Expect '{' and parse field initializers
    // Simplified: just skip to '}'
    while (1) {
        token = lexer_next_token(lexer);
        if (token->type == TOKEN_EOF) {
            token_free(token);
            break;
        }
        token_free(token);
        // In real implementation, parse field: value pairs
        break;  // Placeholder
    }
    
    // Create instance (requires finding definition - placeholder)
    StructInstance* inst = malloc(sizeof(StructInstance));
    inst->struct_name = struct_type;
    inst->instance_name = instance_name;
    inst->definition = NULL;  // Would lookup from symbol table
    
    return inst;
}

// Parse member access
// Syntax: p.name or p.age
MemberAccess* parse_member_access(Lexer* lexer) {
    Token* token = lexer_next_token(lexer);
    
    // Parse instance name
    if (token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected struct instance name\n");
        token_free(token);
        return NULL;
    }
    
    char* instance = strdup(token->value);
    token_free(token);
    
    // Expect '.'
    token = lexer_next_token(lexer);
    if (token->type != TOKEN_DOT) {
        fprintf(stderr, "Error: Expected '.' for member access\n");
        free(instance);
        token_free(token);
        return NULL;
    }
    token_free(token);
    
    // Parse member name
    token = lexer_next_token(lexer);
    if (token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected member name\n");
        free(instance);
        token_free(token);
        return NULL;
    }
    
    MemberAccess* access = struct_create_member_access(instance, token->value);
    free(instance);
    token_free(token);
    
    return access;
}

// YZ_82: Try to parse struct instance declaration
// Syntax: Point p
// Returns StructDef if this is a struct type, NULL otherwise
StructDef* struct_try_parse_instance_decl(Lexer* lexer, Token* first_token) {
    if (!first_token || first_token->type != TOKEN_IDENTIFIER) {
        return NULL;
    }
    
    // Look up if this identifier is a registered struct type
    StructDef* def = struct_lookup_definition(first_token->value);
    if (!def) {
        return NULL;  // Not a struct type
    }
    
    // This is a struct type! Next token should be instance name
    // Don't consume it here - let the caller handle it
    return def;
}
