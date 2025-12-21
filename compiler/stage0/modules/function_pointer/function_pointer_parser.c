#include "function_pointer_parser.h"
#include "../error/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// YZ_209: Function Pointer Parser Implementation

// Helper: Advance to next token
static Token* advance_token(Token** current) {
    if (*current) {
        Token* old = *current;
        *current = lexer_next_token(NULL);  // Get next from global lexer
        token_free(old);
    }
    return *current;
}

// Helper: Expect specific token type
static int expect_token(Token** current, TokenType type, const char* message) {
    if (!*current || (*current)->type != type) {
        error_parser(0, "%s (got: %s)", message, *current ? (*current)->value : "EOF");
        return 0;
    }
    advance_token(current);
    return 1;
}

// Check if token starts function pointer type
int is_function_pointer_type(Token* token) {
    return token && 
           token->type == TOKEN_IDENTIFIER && 
           strcmp(token->value, "function") == 0;
}

// Check if token is function reference (&)
int is_function_reference(Token* token) {
    return token && token->type == TOKEN_AND;
}

// Parse function pointer type
// Syntax: function<ReturnType, ParamType1, ParamType2, ...>
FunctionPointerType* parse_function_pointer_type(Token** current) {
    if (!is_function_pointer_type(*current)) {
        return NULL;
    }
    
    advance_token(current);  // consume 'function'
    
    // Expect '<'
    if (!expect_token(current, TOKEN_LESS, "Expected '<' after 'function'")) {
        return NULL;
    }
    
    // Parse return type
    if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
        error_parser(0, "Expected return type in function pointer type");
        return NULL;
    }
    
    char* return_type = strdup((*current)->value);
    advance_token(current);
    
    // Count parameters (simple: just count commas before >)
    int param_count = 0;
    Token* temp = *current;
    while (temp && temp->type != TOKEN_GREATER) {
        if (temp->type == TOKEN_COMMA) {
            param_count++;
        }
        // Note: This is simplified peek - in production use proper lookahead
        temp = NULL;  // Stop for now - will parse as we go
        break;
    }
    
    // Create function pointer type
    FunctionPointerType* type = function_pointer_type_create(return_type, param_count);
    free(return_type);
    
    // Parse parameter types
    while (*current && (*current)->type == TOKEN_COMMA) {
        advance_token(current);  // consume ','
        
        if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
            error_parser(0, "Expected parameter type in function pointer type");
            function_pointer_type_free(type);
            return NULL;
        }
        
        function_pointer_type_add_param(type, (*current)->value);
        advance_token(current);
    }
    
    // Expect '>'
    if (!expect_token(current, TOKEN_GREATER, "Expected '>' after function pointer type")) {
        function_pointer_type_free(type);
        return NULL;
    }
    
    return type;
}

// Parse function reference
// Syntax: &function_name
FunctionReference* parse_function_reference(Token** current) {
    if (!is_function_reference(*current)) {
        return NULL;
    }
    
    advance_token(current);  // consume '&'
    
    // Expect function name
    if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
        error_parser(0, "Expected function name after '&'");
        return NULL;
    }
    
    FunctionReference* ref = function_reference_create((*current)->value);
    advance_token(current);
    
    return ref;
}

// Parse function pointer variable declaration
// Syntax: function<ReturnType, ParamTypes...> var_name
FunctionPointerVar* parse_function_pointer_var_decl(Token** current) {
    // Parse function pointer type
    FunctionPointerType* type = parse_function_pointer_type(current);
    if (!type) {
        return NULL;
    }
    
    // Expect variable name
    if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
        error_parser(0, "Expected variable name after function pointer type");
        function_pointer_type_free(type);
        return NULL;
    }
    
    FunctionPointerVar* var = function_pointer_var_create((*current)->value, type);
    advance_token(current);
    
    return var;
}

// Parse function pointer assignment
// Syntax: var_name = &function_name
int parse_function_pointer_assignment(Token** current, FunctionPointerVar* var) {
    if (!var || !*current) {
        return 0;
    }
    
    // Expect '='
    if (!expect_token(current, TOKEN_ASSIGN, "Expected '=' in function pointer assignment")) {
        return 0;
    }
    
    // Parse function reference
    FunctionReference* ref = parse_function_reference(current);
    if (!ref) {
        error_parser(0, "Expected function reference in assignment");
        return 0;
    }
    
    // Assign (type checking will happen later during resolution)
    if (!function_pointer_var_assign(var, ref)) {
        error_parser(0, "Function pointer assignment failed");
        function_reference_free(ref);
        return 0;
    }
    
    return 1;
}

// Parse indirect function call
// Syntax: pointer_var(arg1, arg2, ...)
IndirectCall* parse_indirect_call(Token** current, const char* var_name) {
    if (!var_name || !*current) {
        return NULL;
    }
    
    // Expect '('
    if ((*current)->type != TOKEN_LPAREN) {
        return NULL;
    }
    advance_token(current);
    
    // Count arguments (simple count - stop at closing paren)
    int arg_count = 0;
    if (*current && (*current)->type != TOKEN_RPAREN) {
        arg_count = 1;  // At least one argument
        // Count commas for additional arguments
        Token* temp = *current;
        while (temp && temp->type != TOKEN_RPAREN) {
            if (temp->type == TOKEN_COMMA) {
                arg_count++;
            }
            temp = NULL;  // Simplified - will parse as we go
            break;
        }
    }
    
    // Create indirect call
    IndirectCall* call = indirect_call_create(var_name, arg_count);
    
    // Parse arguments
    if (*current && (*current)->type != TOKEN_RPAREN) {
        // For now, we'll store argument expressions as void*
        // In a full implementation, integrate with expression parser
        int arg_idx = 0;
        
        while (*current && (*current)->type != TOKEN_RPAREN) {
            // Parse expression (simplified - just skip for now)
            // TODO: Integrate with expression_parse()
            
            // Skip to next comma or ')'
            int depth = 0;
            while (*current && 
                   (depth > 0 || 
                    ((*current)->type != TOKEN_COMMA && (*current)->type != TOKEN_RPAREN))) {
                if ((*current)->type == TOKEN_LPAREN) depth++;
                if ((*current)->type == TOKEN_RPAREN) depth--;
                if (depth < 0) break;
                advance_token(current);
            }
            
            arg_idx++;
            
            // Skip comma
            if (*current && (*current)->type == TOKEN_COMMA) {
                advance_token(current);
            }
        }
    }
    
    // Expect ')'
    if (!expect_token(current, TOKEN_RPAREN, "Expected ')' after function call arguments")) {
        indirect_call_free(call);
        return NULL;
    }
    
    return call;
}
