#include "variable_parser.h"
#include "../codegen_context/codegen_context.h"
#include "../arithmetic/arithmetic_parser.h"  // ✅ For expression parsing
#include "../arithmetic/arithmetic.h"         // ✅ ArithmeticExpr
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// STATELESS PATTERN: No parser struct, no advance() helper
// Token ownership: type_token is BORROWED, other tokens are OWNED

// Parse variable declaration: numeric x = 10
// type_token is BORROWED from caller - don't free it!
VariableDeclaration* variable_parse_declaration(Lexer* lexer, Token* type_token) {
    if (!lexer || !type_token) {
        return NULL;
    }
    
    // Check for type keyword (type_token is BORROWED)
    VarType base_type;
    if (type_token->type == TOKEN_NUMERIC) {
        base_type = VAR_NUMERIC;
    } else if (type_token->type == TOKEN_STRING_TYPE) {
        base_type = VAR_STRING;
    } else if (type_token->type == TOKEN_BOOLEAN) {
        base_type = VAR_BOOLEAN;
    } else {
        return NULL;  // Not a variable declaration
    }
    
    // type_token consumed by reading, read next token (OWNED)
    Token* tok = lexer_next_token(lexer);
    if (!tok) return NULL;
    
    // Check for pointer (*) or array ([])
    int is_pointer = 0;
    int is_array = 0;
    int array_size = 0;
    VarType actual_type = base_type;
    
    if (tok->type == TOKEN_MULTIPLY) {
        // Pointer type: numeric*
        is_pointer = 1;
        actual_type = VAR_POINTER;
        token_free(tok);
        tok = lexer_next_token(lexer);  // consume '*'
        if (!tok) return NULL;
    } else if (tok->type == TOKEN_LBRACKET) {
        // Array type: numeric[10] or numeric[]
        is_array = 1;
        actual_type = VAR_ARRAY;
        token_free(tok);
        tok = lexer_next_token(lexer);  // consume '['
        if (!tok) return NULL;
        
        // Check for array size
        if (tok->type == TOKEN_NUMBER) {
            array_size = atoi(tok->value);
            token_free(tok);
            tok = lexer_next_token(lexer);  // consume size
            if (!tok) return NULL;
        }
        
        if (tok->type != TOKEN_RBRACKET) {
            fprintf(stderr, "Error: Expected ']' in array declaration\n");
            token_free(tok);
            return NULL;
        }
        token_free(tok);
        tok = lexer_next_token(lexer);  // consume ']'
        if (!tok) return NULL;
    }
    
    // Expect identifier
    if (tok->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected identifier after type\n");
        token_free(tok);
        return NULL;
    }
    
    VariableDeclaration* decl = malloc(sizeof(VariableDeclaration));
    decl->name = strdup(tok->value);
    decl->type = actual_type;
    decl->base_type = base_type;
    decl->is_pointer = is_pointer;
    decl->is_array = is_array;
    decl->array_size = array_size;
    decl->value = NULL;
    decl->init_expr = NULL;
    decl->internal_num_type = INTERNAL_INT64;
    decl->internal_str_type = INTERNAL_RODATA;
    decl->storage = STORAGE_BSS;
    decl->has_decimal_point = 0;
    
    // Phase 2: Initialize TTO fields
    decl->tto_info = NULL;
    decl->tto_analyzed = false;
    decl->needs_overflow_check = false;
    
    token_free(tok);  // consume identifier
    tok = lexer_next_token(lexer);  // read next
    if (!tok) return decl;  // EOF, no initializer
    
    // Check for optional '=' initializer
    if (tok->type != TOKEN_ASSIGN) {
        // No initializer - just declaration
        // Push back the token we read so caller can use it
        lexer_unget_token(lexer, tok);
        return decl;
    }
    
    token_free(tok);  // consume '='
    tok = lexer_next_token(lexer);  // read value
    if (!tok) {
        fprintf(stderr, "Error: Expected value after '='\n");
        variable_declaration_free(decl);
        return NULL;
    }
    
    // ✅ Parse init expression using arithmetic parser
    if (tok->type == TOKEN_IDENTIFIER ||
        tok->type == TOKEN_NUMBER ||
        tok->type == TOKEN_STRING ||
        tok->type == TOKEN_LPAREN) {
        
        // Use stateless arithmetic parser for expression parsing
        // tok is OWNED by us and will be consumed by arithmetic parser
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok);
        token_free(tok);  // arithmetic parser borrowed it, we still own it
        
        if (expr) {
            // Check if it's a simple literal
            if (expr->is_literal && !expr->left && !expr->right) {
                // Simple literal: numeric x = 5
                decl->value = strdup(expr->value);
                decl->storage = STORAGE_DATA;
                arithmetic_expr_free(expr);
            } else {
                // Complex expression: numeric x = a + b
                decl->init_expr = expr;  // ✅ Store ArithmeticExpr*
                decl->storage = STORAGE_BSS;  // Runtime initialization
                decl->tto_analyzed = false;
            }
        }
        
        // DON'T read next token - caller will do it
        return decl;
    } else if (tok->type == TOKEN_STRING) {
        // String literal
        decl->value = strdup(tok->value);
        
        // Phase 2: TTO analysis for string literals
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        *tto = codegen_tto_infer_string_type(decl->value, true);  // true = is constant
        decl->tto_info = tto;
        decl->tto_analyzed = true;
        
        // Update internal type based on TTO (backward compatibility)
        if (tto->type == INTERNAL_TYPE_SSO_STRING) {
            decl->internal_str_type = INTERNAL_SSO;
        } else if (tto->type == INTERNAL_TYPE_HEAP_STRING) {
            decl->internal_str_type = INTERNAL_HEAP;
        } else if (tto->type == INTERNAL_TYPE_RODATA_STRING) {
            decl->internal_str_type = INTERNAL_RODATA;
        }
        
        token_free(tok);
        return decl;
    } else if (tok->type == TOKEN_TRUE) {
        decl->value = strdup("true");
        token_free(tok);
        return decl;
    } else if (tok->type == TOKEN_FALSE) {
        decl->value = strdup("false");
        token_free(tok);
        return decl;
    } else if (tok->type == TOKEN_LBRACKET) {
        // Array literal: [1, 2, 3]
        // For now, store the literal string representation
        // Full parsing happens in array module
        char array_literal[1024] = "[";
        int pos = 1;
        
        token_free(tok);  // consume '['
        tok = lexer_next_token(lexer);
        
        while (tok && 
               tok->type != TOKEN_RBRACKET &&
               tok->type != TOKEN_EOF) {
            
            // Append current token value
            if (tok->type == TOKEN_NUMBER ||
                tok->type == TOKEN_STRING ||
                tok->type == TOKEN_IDENTIFIER) {
                int len = strlen(tok->value);
                if (pos + len + 5 < 1024) {
                    if (tok->type == TOKEN_STRING) {
                        array_literal[pos++] = '"';
                    }
                    strcpy(array_literal + pos, tok->value);
                    pos += len;
                    if (tok->type == TOKEN_STRING) {
                        array_literal[pos++] = '"';
                    }
                }
            }
            
            token_free(tok);
            tok = lexer_next_token(lexer);
            
            // Check for comma
            if (tok && tok->type == TOKEN_COMMA) {
                if (pos + 2 < 1024) {
                    array_literal[pos++] = ',';
                    array_literal[pos++] = ' ';
                }
                token_free(tok);  // consume ','
                tok = lexer_next_token(lexer);
            }
        }
        
        if (tok && tok->type == TOKEN_RBRACKET) {
            if (pos + 2 < 1024) {
                array_literal[pos++] = ']';
                array_literal[pos] = '\0';
            }
            token_free(tok);  // consume ']'
            
            decl->value = strdup(array_literal);
            return decl;
        } else {
            fprintf(stderr, "Error: Expected ']' in array literal\n");
            if (tok) token_free(tok);
            variable_declaration_free(decl);
            return NULL;
        }
    } else {
        fprintf(stderr, "Error: Expected value after '='\n");
        token_free(tok);
        variable_declaration_free(decl);
        return NULL;
    }
}

// Parse variable assignment: x = 20
// identifier_token is BORROWED from caller - don't free it!
VariableAssignment* variable_parse_assignment(Lexer* lexer, Token* identifier_token) {
    if (!lexer || !identifier_token) {
        return NULL;
    }
    
    // Check for identifier
    if (identifier_token->type != TOKEN_IDENTIFIER) {
        return NULL;
    }
    
    VariableAssignment* assign = malloc(sizeof(VariableAssignment));
    assign->name = strdup(identifier_token->value);
    assign->value_expr = NULL;
    
    // Phase 2: Initialize TTO fields
    assign->tto_info = NULL;
    assign->tto_analyzed = false;
    assign->needs_type_promotion = false;
    
    // identifier_token is BORROWED, read next token (OWNED)
    Token* tok = lexer_next_token(lexer);
    if (!tok) {
        fprintf(stderr, "Error: Expected '=' in assignment\n");
        free(assign->name);
        free(assign);
        return NULL;
    }
    
    // Expect '='
    if (tok->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' in assignment\n");
        token_free(tok);
        free(assign->name);
        free(assign);
        return NULL;
    }
    
    token_free(tok);  // consume '='
    tok = lexer_next_token(lexer);  // read expression
    
    if (!tok) {
        fprintf(stderr, "Error: Expected expression after '='\n");
        free(assign->name);
        free(assign);
        return NULL;
    }
    
    // Parse expression using stateless arithmetic API
    // tok is OWNED and will be consumed
    ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok);
    token_free(tok);  // We own it, arithmetic parser borrowed it
    
    if (expr) {
        assign->value_expr = expr;
    } else {
        fprintf(stderr, "Error: Failed to parse assignment expression\n");
        free(assign->name);
        free(assign);
        return NULL;
    }
    
    return assign;
}

// Free declaration
void variable_declaration_free(VariableDeclaration* decl) {
    if (!decl) return;
    
    if (decl->name) free(decl->name);
    if (decl->value) free(decl->value);
    // init_expr would be freed here if used
    
    // Phase 2: Free TTO info
    if (decl->tto_info) free(decl->tto_info);
    
    free(decl);
}

// Free assignment
void variable_assignment_free(VariableAssignment* assign) {
    if (!assign) return;
    
    if (assign->name) free(assign->name);
    // value_expr would be freed here if used
    
    // Phase 2: Free TTO info
    if (assign->tto_info) free(assign->tto_info);
    
    free(assign);
}
