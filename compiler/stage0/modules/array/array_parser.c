#include "array_parser.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../error/error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ✅ STATELESS PARSER PATTERN
// No ArrayParser struct, no create/free functions
// Token ownership: Parameters are BORROWED, lexer_next_token() returns OWNED

void collection_free(Collection* coll) {
    if (!coll) return;
    
    if (coll->type == COLL_ARRAY) {
        for (int i = 0; i < coll->data.array.length; i++) {
            // Note: elements might be Expression* or ArithmeticExpr*
            // For now we just free the pointer
            free(coll->data.array.elements[i]);
        }
        free(coll->data.array.elements);
    } else if (coll->type == COLL_LIST) {
        for (int i = 0; i < coll->data.list.length; i++) {
            free(coll->data.list.elements[i]);
        }
        free(coll->data.list.elements);
        free(coll->data.list.element_types);
    } else if (coll->type == COLL_TUPLE) {
        for (int i = 0; i < coll->data.tuple.length; i++) {
            free(coll->data.tuple.elements[i]);
        }
        free(coll->data.tuple.elements);
        free(coll->data.tuple.element_types);
    }
    
    free(coll);
}

// Parse array literal: [1, 2, 3, 4]
// Homogeneous, comma-separated
// lbracket_token: BORROWED (must be '[')
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token) {
    if (!lbracket_token || lbracket_token->type != TOKEN_LBRACKET) {
        error_parser(lbracket_token ? lbracket_token->line : 0, 
                    "Expected '[' to start array");
        return NULL;
    }
    // lbracket_token is borrowed - don't free!
    
    // Parse elements
    int capacity = 4;
    int length = 0;
    ArithmeticExpr** elements = malloc(sizeof(ArithmeticExpr*) * capacity);
    
    // Check if empty array []
    Token* tok = lexer_next_token(lexer);  // OWNED
    if (!tok) {
        error_parser(0, "Unexpected EOF in array literal");
        free(elements);
        return NULL;
    }
    
    if (tok->type == TOKEN_RBRACKET) {
        // Empty array
        token_free(tok);
        
        Collection* coll = malloc(sizeof(Collection));
        coll->type = COLL_ARRAY;
        coll->data.array.element_type = VAR_NUMERIC;
        coll->data.array.capacity = capacity;
        coll->data.array.length = 0;
        coll->data.array.elements = (void**)elements;
        coll->data.array.is_dynamic = 1;
        return coll;
    }
    
    // Parse first element using stateless arithmetic parser
    ArithmeticExpr* first_elem = arithmetic_parse_expression_stateless(lexer, tok);
    if (!first_elem) {
        error_parser(tok->line, "Failed to parse array element");
        token_free(tok);
        free(elements);
        return NULL;
    }
    elements[0] = first_elem;
    length = 1;
    
    // Parse remaining elements (semicolon separated - PMPL standard)
    tok = lexer_next_token(lexer);  // OWNED
    while (tok && tok->type == TOKEN_SEMICOLON) {
        token_free(tok);  // Free semicolon
        
        // Check capacity
        if (length >= capacity) {
            capacity *= 2;
            elements = realloc(elements, sizeof(ArithmeticExpr*) * capacity);
        }
        
        // Parse next element
        tok = lexer_next_token(lexer);  // OWNED
        if (!tok) {
            error_parser(0, "Unexpected EOF in array literal");
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            return NULL;
        }
        
        ArithmeticExpr* elem = arithmetic_parse_expression_stateless(lexer, tok);
        if (!elem) {
            error_parser(tok->line, "Failed to parse array element");
            token_free(tok);
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            return NULL;
        }
        elements[length++] = elem;
        
        tok = lexer_next_token(lexer);  // OWNED - next token (comma or ])
    }
    
    // Expect ']'
    if (!tok || tok->type != TOKEN_RBRACKET) {
        error_parser(tok ? tok->line : 0, "Expected ']' to close array");
        if (tok) token_free(tok);
        for (int i = 0; i < length; i++) {
            arithmetic_expr_free(elements[i]);
        }
        free(elements);
        return NULL;
    }
    token_free(tok);  // Free ']'
    
    // Build array collection
    Collection* coll = malloc(sizeof(Collection));
    coll->type = COLL_ARRAY;
    coll->data.array.element_type = VAR_NUMERIC;
    coll->data.array.capacity = capacity;
    coll->data.array.length = length;
    coll->data.array.elements = (void**)elements;
    coll->data.array.is_dynamic = 1;
    
    return coll;
}

// Parse list literal: (10; 20; 30;)
// Heterogeneous, semicolon-separated, trailing semicolon REQUIRED
// lparen_token: BORROWED (must be '(')
Collection* array_parse_list_literal(Lexer* lexer, Token* lparen_token) {
    if (!lparen_token || lparen_token->type != TOKEN_LPAREN) {
        error_parser(lparen_token ? lparen_token->line : 0,
                    "Expected '(' to start list");
        return NULL;
    }
    // lparen_token is borrowed - don't free!
    
    // Parse elements
    int capacity = 4;
    int length = 0;
    ArithmeticExpr** elements = malloc(sizeof(ArithmeticExpr*) * capacity);
    VarType* types = malloc(sizeof(VarType) * capacity);
    
    // Parse elements until we hit ')'
    Token* tok = lexer_next_token(lexer);  // OWNED
    while (tok && tok->type != TOKEN_RPAREN) {
        // Check capacity
        if (length >= capacity) {
            capacity *= 2;
            elements = realloc(elements, sizeof(ArithmeticExpr*) * capacity);
            types = realloc(types, sizeof(VarType) * capacity);
        }
        
        // Parse element
        ArithmeticExpr* elem = arithmetic_parse_expression_stateless(lexer, tok);
        if (!elem) {
            error_parser(tok->line, "Failed to parse list element");
            token_free(tok);
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            free(types);
            return NULL;
        }
        elements[length] = elem;
        types[length] = VAR_NUMERIC;  // Default, should be inferred
        length++;
        
        // Expect ';' after each element (including last)
        tok = lexer_next_token(lexer);  // OWNED
        if (!tok || tok->type != TOKEN_SEMICOLON) {
            error_parser(tok ? tok->line : 0, "Expected ';' after list element");
            if (tok) token_free(tok);
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            free(types);
            return NULL;
        }
        token_free(tok);  // Free ';'
        
        tok = lexer_next_token(lexer);  // OWNED - next element or ')'
    }
    
    // Expect ')'
    if (!tok || tok->type != TOKEN_RPAREN) {
        error_parser(tok ? tok->line : 0, "Expected ')' to close list");
        if (tok) token_free(tok);
        for (int i = 0; i < length; i++) {
            arithmetic_expr_free(elements[i]);
        }
        free(elements);
        free(types);
        return NULL;
    }
    token_free(tok);  // Free ')'
    
    // Build list collection
    Collection* coll = malloc(sizeof(Collection));
    coll->type = COLL_LIST;
    coll->data.list.capacity = capacity;
    coll->data.list.length = length;
    coll->data.list.elements = (void**)elements;
    coll->data.list.element_types = types;
    
    return coll;
}

// Parse tuple literal: <"name", 25, true>
// Heterogeneous, immutable, comma-separated
// less_token: BORROWED (must be '<')
Collection* array_parse_tuple_literal(Lexer* lexer, Token* less_token) {
    if (!less_token || less_token->type != TOKEN_LESS) {
        error_parser(less_token ? less_token->line : 0,
                    "Expected '<' to start tuple");
        return NULL;
    }
    // less_token is borrowed - don't free!
    
    // Parse elements
    int capacity = 4;
    int length = 0;
    ArithmeticExpr** elements = malloc(sizeof(ArithmeticExpr*) * capacity);
    VarType* types = malloc(sizeof(VarType) * capacity);
    
    // Check if empty tuple <>
    Token* tok = lexer_next_token(lexer);  // OWNED
    if (!tok) {
        error_parser(0, "Unexpected EOF in tuple literal");
        free(elements);
        free(types);
        return NULL;
    }
    
    if (tok->type == TOKEN_GREATER) {
        // Empty tuple
        token_free(tok);
        
        Collection* coll = malloc(sizeof(Collection));
        coll->type = COLL_TUPLE;
        coll->data.tuple.length = 0;
        coll->data.tuple.elements = (void**)elements;
        coll->data.tuple.element_types = types;
        return coll;
    }
    
    // Parse first element
    ArithmeticExpr* first_elem = arithmetic_parse_expression_stateless(lexer, tok);
    if (!first_elem) {
        error_parser(tok->line, "Failed to parse tuple element");
        token_free(tok);
        free(elements);
        free(types);
        return NULL;
    }
    elements[0] = first_elem;
    types[0] = VAR_NUMERIC;
    length = 1;
    
    // Parse remaining elements (comma-separated)
    tok = lexer_next_token(lexer);  // OWNED
    while (tok && tok->type == TOKEN_COMMA) {
        token_free(tok);  // Free comma
        
        // Check capacity
        if (length >= capacity) {
            capacity *= 2;
            elements = realloc(elements, sizeof(ArithmeticExpr*) * capacity);
            types = realloc(types, sizeof(VarType) * capacity);
        }
        
        // Parse next element
        tok = lexer_next_token(lexer);  // OWNED
        if (!tok) {
            error_parser(0, "Unexpected EOF in tuple literal");
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            free(types);
            return NULL;
        }
        
        ArithmeticExpr* elem = arithmetic_parse_expression_stateless(lexer, tok);
        if (!elem) {
            error_parser(tok->line, "Failed to parse tuple element");
            token_free(tok);
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            free(types);
            return NULL;
        }
        elements[length] = elem;
        types[length] = VAR_NUMERIC;
        length++;
        
        tok = lexer_next_token(lexer);  // OWNED - next token (comma or >)
    }
    
    // Expect '>'
    if (!tok || tok->type != TOKEN_GREATER) {
        error_parser(tok ? tok->line : 0, "Expected '>' to close tuple");
        if (tok) token_free(tok);
        for (int i = 0; i < length; i++) {
            arithmetic_expr_free(elements[i]);
        }
        free(elements);
        free(types);
        return NULL;
    }
    token_free(tok);  // Free '>'
    
    // Build tuple collection
    Collection* coll = malloc(sizeof(Collection));
    coll->type = COLL_TUPLE;
    coll->data.tuple.length = length;
    coll->data.tuple.elements = (void**)elements;
    coll->data.tuple.element_types = types;
    
    return coll;
}

// Parse index access and return IndexAccess struct
// base_name: identifier name
// index_token: BORROWED (must be '[' for array or '(' for list)
IndexAccess* array_parse_index_access(Lexer* lexer, const char* base_name, Token* index_token) {
    if (!base_name || !index_token) return NULL;
    
    // YZ_23: Determine collection type by bracket style
    // Array: arr[i]   - TOKEN_LBRACKET
    // List:  lst(i)   - TOKEN_LPAREN (bitişik, no space!)
    // Tuple: tpl<i>   - TOKEN_LANGLE
    int is_list = 0;
    int is_tuple = 0;
    TokenType closing_type;
    
    if (index_token->type == TOKEN_LBRACKET) {
        // Array access: arr[i]
        is_list = 0;
        is_tuple = 0;
        closing_type = TOKEN_RBRACKET;
    } else if (index_token->type == TOKEN_LPAREN) {
        // List access: liste(i) - bitişik yazım zorunlu!
        is_list = 1;
        is_tuple = 0;
        closing_type = TOKEN_RPAREN;
        
        // YZ_24: Whitespace validation (bitişik yazım enforcement)
        if (index_token->has_leading_whitespace) {
            error_parser(index_token->line, 
                "Syntax error: List index must be bitişik (no space before '('). Use 'liste(i)' not 'liste (i)'");
            return NULL;
        }
    } else if (index_token->type == TOKEN_LANGLE) {
        // Tuple access: tuple<i>
        is_list = 0;
        is_tuple = 1;
        closing_type = TOKEN_GREATER;
    } else {
        error_parser(index_token->line, "Expected '[', '(', or '<' for index access");
        return NULL;
    }
    // index_token is borrowed - don't free!
    
    // Create IndexAccess struct
    IndexAccess* access = malloc(sizeof(IndexAccess));
    access->collection_name = strdup(base_name);
    access->is_list_access = is_list;
    access->is_tuple_access = is_tuple;  // YZ_23: Add tuple support
    
    // Parse index expression
    Token* tok = lexer_next_token(lexer);  // OWNED
    if (!tok) {
        error_parser(0, "Unexpected EOF in index access");
        free(access->collection_name);
        free(access);
        return NULL;
    }
    
    // Check what kind of index we have
    if (tok->type == TOKEN_NUMBER) {
        // Constant index
        access->index_type = 0;
        access->index.const_index = atoi(tok->value);
        token_free(tok);
        
        tok = lexer_next_token(lexer);  // OWNED - closing bracket/paren
    } else if (tok->type == TOKEN_IDENTIFIER) {
        // Check if this is a simple variable or start of expression
        Token* lookahead = lexer_next_token(lexer);  // OWNED
        if (lookahead && (lookahead->type == TOKEN_RBRACKET || lookahead->type == TOKEN_RPAREN)) {
            // Simple variable index: arr[i]
            access->index_type = 1;
            access->index.var_index = strdup(tok->value);
            token_free(tok);
            tok = lookahead;  // Use closing bracket/paren
        } else {
            // Expression starting with identifier: arr[i+1]
            lexer_unget_token(lexer, lookahead);  // Put back
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok);
            if (!expr) {
                error_parser(tok->line, "Failed to parse index expression");
                token_free(tok);
                free(access->collection_name);
                free(access);
                return NULL;
            }
            access->index_type = 2;
            access->index.expr_index = expr;
            
            tok = lexer_next_token(lexer);  // OWNED - closing bracket/paren
        }
    } else {
        // Expression index - use arithmetic parser
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok);
        if (!expr) {
            error_parser(tok->line, "Failed to parse index expression");
            token_free(tok);
            free(access->collection_name);
            free(access);
            return NULL;
        }
        access->index_type = 2;
        access->index.expr_index = expr;
        
        tok = lexer_next_token(lexer);  // OWNED - closing bracket/paren
    }
    
    // Expect closing bracket/paren/angle
    if (!tok || tok->type != closing_type) {
        const char* expected_msg = is_tuple ? "Expected '>' after tuple index" :
                                   is_list ? "Expected ')' after list index" :
                                   "Expected ']' after array index";
        error_parser(tok ? tok->line : 0, expected_msg);
        if (tok) token_free(tok);
        free(access->collection_name);
        if (access->index_type == 1) free(access->index.var_index);
        if (access->index_type == 2) arithmetic_expr_free(access->index.expr_index);
        free(access);
        return NULL;
    }
    token_free(tok);  // Free closing bracket/paren/angle
    
    return access;
}
