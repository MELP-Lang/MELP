#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions_parser.h"
#include "functions.h"
#include "../lexer/lexer.h"
#include "../statement/statement_parser.h"  // ✅ Statement parsing
#include "../statement/statement_optimize.h" // ✅ YZ_32: Dead code elimination
#include "../parser_core/parser_core.h"      // ✅ Parser structure (temp wrapper)
#include "../error/error.h"                  // ✅ Error handling system
#include "../struct/struct.h"                // YZ_84: Struct definitions for type lookup

// Helper: Convert type keyword to FunctionParamType
static FunctionParamType token_to_param_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMERIC: return FUNC_PARAM_NUMERIC;
        case TOKEN_STRING_TYPE: return FUNC_PARAM_TEXT;  // YZ_63: string type keyword
        case TOKEN_STRING: return FUNC_PARAM_TEXT;        // Legacy: string literal token
        case TOKEN_BOOLEAN: return FUNC_PARAM_BOOLEAN;
        case TOKEN_LIST: return FUNC_PARAM_LIST;          // YZ_21: list type keyword
        default: return FUNC_PARAM_NUMERIC;
    }
}

// Helper: Convert type keyword to FunctionReturnType
static FunctionReturnType token_to_return_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMERIC: return FUNC_RETURN_NUMERIC;
        case TOKEN_STRING_TYPE: return FUNC_RETURN_TEXT;  // YZ_63: string type keyword
        case TOKEN_STRING: return FUNC_RETURN_TEXT;        // Legacy: string literal token
        case TOKEN_BOOLEAN: return FUNC_RETURN_BOOLEAN;
        case TOKEN_LIST: return FUNC_RETURN_LIST;          // YZ_30: list return type
        case TOKEN_TUPLE: return FUNC_RETURN_LIST;         // YZ_30: tuple also returns list
        case TOKEN_ARRAY: return FUNC_RETURN_LIST;         // YZ_30: array returns list
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
        // First parameter: type name OR name: type
        // MLP format: function name(numeric x, string y)          [OLD - type param]
        // YZ_15 format: function name(x: numeric, y: string)      [NEW - param: type]
        // YZ_84: Also supports: function name(Point p1, Address addr)
        
        FunctionParamType param_type = FUNC_PARAM_NUMERIC;  // Default
        char* struct_type_name = NULL;
        char* param_name = NULL;
        
        // YZ_15: Check for type annotation syntax (param: type)
        // Lookahead: If IDENTIFIER followed by COLON, it's new syntax
        if (tok->type == TOKEN_IDENTIFIER) {
            // Save identifier (might be param name OR type name)
            char* first_token = strdup(tok->value);
            token_free(tok);
            tok = lexer_next_token(lexer);
            
            // Check if followed by colon (new syntax: param: type)
            if (tok->type == TOKEN_COLON) {
                // NEW SYNTAX: param: type
                param_name = first_token;  // First token was param name
                token_free(tok);  // Consume colon
                tok = lexer_next_token(lexer);  // Get type
                
                // Parse type after colon
                if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || 
                    tok->type == TOKEN_BOOLEAN || tok->type == TOKEN_LIST) {
                    param_type = token_to_param_type(tok->type);
                    token_free(tok);
                    tok = lexer_next_token(lexer);
                } else if (tok->type == TOKEN_IDENTIFIER) {
                    // Could be struct type
                    StructDef* struct_def = struct_lookup_definition(tok->value);
                    if (struct_def) {
                        param_type = FUNC_PARAM_STRUCT;
                        struct_type_name = strdup(tok->value);
                    }
                    token_free(tok);
                    tok = lexer_next_token(lexer);
                } else {
                    error_parser(tok->line, "Expected type after ':' in parameter");
                    free(first_token);
                    if (tok) token_free(tok);
                    function_free(func);
                    return NULL;
                }
            } else {
                // OLD SYNTAX: Could be struct type OR parameter name
                StructDef* struct_def = struct_lookup_definition(first_token);
                if (struct_def) {
                    // It's a struct type! (old syntax: Point p)
                    param_type = FUNC_PARAM_STRUCT;
                    struct_type_name = strdup(first_token);
                    free(first_token);
                    
                    // Get parameter name
                    if (tok->type != TOKEN_IDENTIFIER) {
                        error_parser(tok->line, "Expected parameter name after struct type");
                        token_free(tok);
                        function_free(func);
                        return NULL;
                    }
                    param_name = strdup(tok->value);
                    token_free(tok);
                    tok = lexer_next_token(lexer);
                } else {
                    // Not a struct, must be parameter name with default numeric type
                    param_name = first_token;
                    param_type = FUNC_PARAM_NUMERIC;
                    // tok already points to next token
                }
            }
        }
        // Type tokens first (old syntax: numeric x, boolean flag)
        else if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || 
                 tok->type == TOKEN_BOOLEAN || tok->type == TOKEN_LIST) {
            // OLD SYNTAX: type param
            param_type = token_to_param_type(tok->type);
            token_free(tok);
            tok = lexer_next_token(lexer);
            
            // Get parameter name
            if (tok->type != TOKEN_IDENTIFIER) {
                error_parser(tok->line, "Expected parameter name after type");
                token_free(tok);
                function_free(func);
                return NULL;
            }
            param_name = strdup(tok->value);
            token_free(tok);
            tok = lexer_next_token(lexer);
        } else {
            error_parser(tok->line, "Expected parameter type or name");
            token_free(tok);
            function_free(func);
            return NULL;
        }
        
        // Now we have: param_name, param_type, (optional) struct_type_name
        // tok points to next token (could be ASSIGN, COMMA, or RPAREN)
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
        
        // YZ_84: Add parameter (struct or regular)
        if (param_type == FUNC_PARAM_STRUCT && struct_type_name) {
            function_add_struct_param(func, param_name, struct_type_name);
            free(struct_type_name);
        } else {
            function_add_param(func, param_name, param_type);
        }
        // Set default value flag on last added parameter
        if (has_default && func->param_count > 0) {
            FunctionParam* last_param = func->params;
            while (last_param->next) last_param = last_param->next;
            last_param->has_default = 1;
        }
        free(param_name);
        
        // Additional parameters: , type name OR , name: type
        while (tok->type == TOKEN_COMMA) {
            token_free(tok);
            
            tok = lexer_next_token(lexer);
            
            // Reset for next parameter
            param_type = FUNC_PARAM_NUMERIC;
            struct_type_name = NULL;
            param_name = NULL;
            
            // YZ_15: Check for type annotation syntax (param: type)
            if (tok->type == TOKEN_IDENTIFIER) {
                char* first_token = strdup(tok->value);
                token_free(tok);
                tok = lexer_next_token(lexer);
                
                // Check if followed by colon (new syntax)
                if (tok->type == TOKEN_COLON) {
                    // NEW SYNTAX: param: type
                    param_name = first_token;
                    token_free(tok);  // Consume colon
                    tok = lexer_next_token(lexer);
                    
                    // Parse type
                    if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || 
                        tok->type == TOKEN_BOOLEAN || tok->type == TOKEN_LIST) {
                        param_type = token_to_param_type(tok->type);
                        token_free(tok);
                        tok = lexer_next_token(lexer);
                    } else if (tok->type == TOKEN_IDENTIFIER) {
                        StructDef* struct_def = struct_lookup_definition(tok->value);
                        if (struct_def) {
                            param_type = FUNC_PARAM_STRUCT;
                            struct_type_name = strdup(tok->value);
                        }
                        token_free(tok);
                        tok = lexer_next_token(lexer);
                    } else {
                        error_parser(tok->line, "Expected type after ':'");
                        free(first_token);
                        if (tok) token_free(tok);
                        function_free(func);
                        return NULL;
                    }
                } else {
                    // OLD SYNTAX: Check if it's struct type
                    StructDef* struct_def = struct_lookup_definition(first_token);
                    if (struct_def) {
                        param_type = FUNC_PARAM_STRUCT;
                        struct_type_name = strdup(first_token);
                        free(first_token);
                        
                        if (tok->type != TOKEN_IDENTIFIER) {
                            error_parser(tok->line, "Expected parameter name");
                            token_free(tok);
                            function_free(func);
                            return NULL;
                        }
                        param_name = strdup(tok->value);
                        token_free(tok);
                        tok = lexer_next_token(lexer);
                    } else {
                        // Parameter name with default numeric type
                        param_name = first_token;
                        param_type = FUNC_PARAM_NUMERIC;
                    }
                }
            }
            // OLD SYNTAX: Type token first
            else if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || 
                     tok->type == TOKEN_BOOLEAN || tok->type == TOKEN_LIST) {
                param_type = token_to_param_type(tok->type);
                token_free(tok);
                tok = lexer_next_token(lexer);
                
                if (tok->type != TOKEN_IDENTIFIER) {
                    error_parser(tok->line, "Expected parameter name after type");
                    token_free(tok);
                    function_free(func);
                    return NULL;
                }
                param_name = strdup(tok->value);
                token_free(tok);
                tok = lexer_next_token(lexer);
            } else {
                error_parser(tok->line, "Expected parameter after ','");
                token_free(tok);
                function_free(func);
                return NULL;
            }
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
            
            // YZ_84: Add parameter (struct or regular)
            if (param_type == FUNC_PARAM_STRUCT && struct_type_name) {
                function_add_struct_param(func, param_name, struct_type_name);
                free(struct_type_name);
            } else {
                function_add_param(func, param_name, param_type);
            }
            if (has_default && func->param_count > 0) {
                FunctionParam* last_param = func->params;
                while (last_param->next) last_param = last_param->next;
                last_param->has_default = 1;
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
    
    // Optional return type: returns numeric OR as numeric (YZ_31: 'as' alternative)
    tok = lexer_next_token(lexer);
    
    if (tok && (tok->type == TOKEN_RETURNS || tok->type == TOKEN_AS)) {
        token_free(tok);
        tok = lexer_next_token(lexer);
        
        // YZ_84: Check if return type is a struct
        if (tok->type == TOKEN_IDENTIFIER) {
            StructDef* struct_def = struct_lookup_definition(tok->value);
            if (struct_def) {
                // Returning a struct
                func->return_type = FUNC_RETURN_STRUCT;
                func->return_struct_type = strdup(tok->value);
            } else {
                // Unknown type, default to void
                func->return_type = FUNC_RETURN_VOID;
            }
        } else {
            // Regular return type
            func->return_type = token_to_return_type(tok->type);
        }
        token_free(tok);
        tok = NULL;  // YZ_74: Mark as consumed
    } else {
        // YZ_74: No return type - tok is first token of function body
        // Put it back so statement_parse can read it
        if (tok) {
            lexer_unget_token(lexer, tok);
        }
    }
    
    // ✅ Function body - use statement parser (modular!)
    // Create temporary Parser wrapper for statement_parse compatibility
    // YZ_74: Set current_token to NULL so statement_parse reads from lexer
    Parser temp_parser = { .lexer = lexer, .current_token = NULL };
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
    
    // ✅ YZ_32: Apply optimizations
    // Constant folding already done in arithmetic_parser
    // Dead code elimination on statement level
    func->body = statement_optimize_dead_code(func->body);
    
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
