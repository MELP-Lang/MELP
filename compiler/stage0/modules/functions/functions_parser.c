#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions_parser.h"
#include "functions.h"
#include "../lexer/lexer.h"
#include "../statement/statement_parser.h"  // ✅ Statement parsing
#include "../parser_core/parser_core.h"      // ✅ Parser structure (temp wrapper)
#include "../error/error.h"                  // ✅ Error handling system

// Helper: Convert type keyword to FunctionParamType
static FunctionParamType token_to_param_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMERIC: return FUNC_PARAM_NUMERIC;
        case TOKEN_STRING: return FUNC_PARAM_TEXT;
        case TOKEN_BOOLEAN: return FUNC_PARAM_BOOLEAN;
        default: return FUNC_PARAM_NUMERIC;
    }
}

// Helper: Convert type keyword to FunctionReturnType
static FunctionReturnType token_to_return_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMERIC: return FUNC_RETURN_NUMERIC;
        case TOKEN_STRING: return FUNC_RETURN_TEXT;
        case TOKEN_BOOLEAN: return FUNC_RETURN_BOOLEAN;
        default: return FUNC_RETURN_VOID;
    }
}

// Parse function declaration (STATELESS PATTERN)
// Syntax: def func_name(param1: numeric, param2: text) -> numeric { ... }
FunctionDeclaration* parse_function_declaration(Lexer* lexer) {
    // Peek first - check if 'function' keyword exists
    Token* tok = lexer_next_token(lexer);
    if (!tok) return NULL;  // EOF
    
    if (tok->type != TOKEN_FUNCTION) {
        if (tok->type != TOKEN_EOF) {
            // Phase 6: Check if it's a misspelled keyword
            if (tok->type == TOKEN_IDENTIFIER && tok->value) {
                const char* suggestion = error_find_similar(tok->value, MELP_KEYWORDS, MELP_KEYWORD_COUNT, 3);
                if (suggestion) {
                    error_report_with_suggestion(ERROR_LEVEL_ERROR, ERROR_CAT_PARSER,
                        tok->line, 1, strlen(tok->value), suggestion,
                        "Expected 'function' keyword, got '%s'", tok->value);
                } else {
                    error_parser(tok->line, "Expected 'function' keyword");
                }
            } else {
                error_parser(tok->line, "Expected 'function' keyword");
            }
        }
        token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // Function name
    tok = lexer_next_token(lexer);
    if (!tok || tok->type != TOKEN_IDENTIFIER) {
        error_parser(tok ? tok->line : 0, "Expected function name");
        if (tok) token_free(tok);
        return NULL;
    }
    
    char* func_name = strdup(tok->value);
    token_free(tok);
    
    // Left paren
    tok = lexer_next_token(lexer);
    if (!tok || tok->type != TOKEN_LPAREN) {
        error_parser(tok ? tok->line : 0, "Expected '(' after function name");
        free(func_name);
        if (tok) token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // Create function with default return type (will be updated if return type specified)
    FunctionDeclaration* func = function_create(func_name, FUNC_RETURN_VOID);
    free(func_name);
    
    // Parse parameters
    tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_RPAREN) {
        // First parameter: type name
        // MLP format: function name(numeric x, string y)
        
        // Read type first
        FunctionParamType param_type = token_to_param_type(tok->type);
        if (param_type == FUNC_PARAM_NUMERIC) {  // Valid type
            token_free(tok);
            tok = lexer_next_token(lexer);
        } else {
            param_type = FUNC_PARAM_NUMERIC;  // Default if no type
        }
        
        // Then parameter name
        if (tok->type != TOKEN_IDENTIFIER) {
            error_parser(tok->line, "Expected parameter name");
            token_free(tok);
            function_free(func);
            return NULL;
        }
        
        char* param_name = strdup(tok->value);
        token_free(tok);
        
        tok = lexer_next_token(lexer);
        int has_default = 0;
        
        // Check for default value: = expr
        if (tok->type == TOKEN_ASSIGN) {
            token_free(tok);
            tok = lexer_next_token(lexer);
            has_default = 1;
            // Parse default value (simplified - just skip expression)
            while (tok->type != TOKEN_COMMA && tok->type != TOKEN_RPAREN && tok->type != TOKEN_EOF) {
                token_free(tok);
                tok = lexer_next_token(lexer);
            }
        }
        
        function_add_param(func, param_name, param_type);
        // Set default value flag on last added parameter
        if (has_default && func->param_count > 0) {
            func->params[func->param_count - 1].has_default = 1;
        }
        free(param_name);
        
        // Additional parameters: , type name
        while (tok->type == TOKEN_COMMA) {
            token_free(tok);
            
            tok = lexer_next_token(lexer);
            
            // Read type
            param_type = token_to_param_type(tok->type);
            if (param_type == FUNC_PARAM_NUMERIC) {  // Valid type
                token_free(tok);
                tok = lexer_next_token(lexer);
            } else {
                param_type = FUNC_PARAM_NUMERIC;  // Default
            }
            
            // Then parameter name
            if (tok->type != TOKEN_IDENTIFIER) {
                error_parser(tok->line, "Expected parameter name after ','");
                token_free(tok);
                function_free(func);
                return NULL;
            }
            
            param_name = strdup(tok->value);
            token_free(tok);
            
            tok = lexer_next_token(lexer);
            has_default = 0;
            
            // Check for default value
            if (tok->type == TOKEN_ASSIGN) {
                token_free(tok);
                tok = lexer_next_token(lexer);
                has_default = 1;
                // Skip default value expression
                while (tok->type != TOKEN_COMMA && tok->type != TOKEN_RPAREN && tok->type != TOKEN_EOF) {
                    token_free(tok);
                    tok = lexer_next_token(lexer);
                }
            }
            
            function_add_param(func, param_name, param_type);
            if (has_default && func->param_count > 0) {
                func->params[func->param_count - 1].has_default = 1;
            }
            free(param_name);
        }
    }
    
    // Right paren
    if (tok->type != TOKEN_RPAREN) {
        error_parser(tok->line, "Expected ')' after parameters");
        token_free(tok);
        function_free(func);
        return NULL;
    }
    token_free(tok);
    
    // Optional return type: returns numeric
    tok = lexer_next_token(lexer);
    
    if (tok->type == TOKEN_RETURNS) {
        token_free(tok);
        tok = lexer_next_token(lexer);
        func->return_type = token_to_return_type(tok->type);
        token_free(tok);
        tok = NULL;  // Will read fresh for body
    }
    
    // ✅ Function body - use statement parser (modular!)
    // Create temporary Parser wrapper for statement_parse compatibility
    Parser temp_parser = { .lexer = lexer, .current_token = tok };
    Parser* parser = &temp_parser;
    
    Statement* body_head = NULL;
    Statement* body_tail = NULL;
    
    int stmt_count = 0;  // Counter
    while (1) {
        Statement* stmt = statement_parse(parser);
        if (!stmt) {
            break;  // End of body (statement_parse detected 'end')
        }
        
        stmt_count++;
        
        // Add to body linked list
        if (!body_head) {
            body_head = stmt;
            body_tail = stmt;
        } else {
            body_tail->next = stmt;
            body_tail = stmt;
        }
    }
    
    // Store body in function
    func->body = body_head;
    return func;
}

// Parse function call: func_name(arg1, arg2)
FunctionCall* parse_function_call(Lexer* lexer, const char* func_name) {
    FunctionCall* call = function_call_create(func_name);
    
    Token* tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_LPAREN) {
        error_parser(tok->line, "Expected '(' after function name");
        token_free(tok);
        function_call_free(call);
        return NULL;
    }
    token_free(tok);
    
    // Parse arguments
    tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_RPAREN) {
        // For now, skip argument parsing - would need expression parser
        // Just count tokens until ')'
        int depth = 1;
        while (tok->type != TOKEN_EOF && depth > 0) {
            if (tok->type == TOKEN_LPAREN) depth++;
            else if (tok->type == TOKEN_RPAREN) depth--;
            token_free(tok);
            if (depth > 0) {
                tok = lexer_next_token(lexer);
            }
        }
    } else {
        token_free(tok);
    }
    
    return call;
}

// Parse return statement: return expr
ReturnStatement* parse_return_statement(Lexer* lexer) {
    Token* tok = lexer_next_token(lexer);
    if (tok->type != TOKEN_RETURN) {
        error_parser(tok->line, "Expected 'return' keyword");
        token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // For now, skip expression parsing - would need expression parser
    // Just create empty return
    ReturnStatement* ret = return_statement_create(NULL);
    
    // Skip to semicolon or newline
    tok = lexer_next_token(lexer);
    while (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_EOF) {
        token_free(tok);
        tok = lexer_next_token(lexer);
    }
    token_free(tok);
    
    return ret;
}
