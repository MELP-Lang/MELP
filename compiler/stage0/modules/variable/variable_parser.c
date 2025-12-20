#include "variable_parser.h"
#include "../codegen_context/codegen_context.h"
#include "../arithmetic/arithmetic_parser.h"  // ✅ For expression parsing
#include "../arithmetic/arithmetic.h"         // ✅ ArithmeticExpr
#include "../array/array_parser.h"            // ✅ For array literal parsing
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// STATELESS PATTERN: No parser struct, no advance() helper
// Token ownership: type_token is BORROWED, other tokens are OWNED

// Parse variable declaration: numeric x = 10
// OR const declaration: const numeric x = 10
// type_token is BORROWED from caller - don't free it!
VariableDeclaration* variable_parse_declaration(Lexer* lexer, Token* type_token) {
    if (!lexer || !type_token) {
        return NULL;
    }
    
    // YZ_CONST: Check if this is a const declaration
    bool is_const = false;
    Token* actual_type_token = type_token;
    
    if (type_token->type == TOKEN_CONST) {
        is_const = true;
        // Read next token for the actual type
        actual_type_token = lexer_next_token(lexer);
        if (!actual_type_token) {
            fprintf(stderr, "Error: Expected type after 'const'\n");
            return NULL;
        }
    }
    
    // Check for type keyword (actual_type_token is OWNED if is_const, BORROWED otherwise)
    VarType base_type;
    if (actual_type_token->type == TOKEN_NUMERIC) {
        base_type = VAR_NUMERIC;
    } else if (actual_type_token->type == TOKEN_STRING_TYPE) {
        base_type = VAR_STRING;
    } else if (actual_type_token->type == TOKEN_BOOLEAN) {
        base_type = VAR_BOOLEAN;
    } else if (actual_type_token->type == TOKEN_ARRAY) {
        base_type = VAR_ARRAY;  // YZ_74: Array type keyword
    } else if (actual_type_token->type == TOKEN_LIST) {
        base_type = VAR_LIST;
    } else if (actual_type_token->type == TOKEN_TUPLE) {
        base_type = VAR_TUPLE;
    } else {
        if (is_const) token_free(actual_type_token);
        return NULL;  // Not a variable declaration
    }
    
    // actual_type_token consumed by reading, read next token (OWNED)
    if (is_const) token_free(actual_type_token);  // YZ_CONST: Free if we owned it
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
    decl->is_const = is_const;  // YZ_CONST: Set const flag
    decl->next = NULL;  // YZ_121: Initialize next pointer
    
    // Phase 2: Initialize STO fields
    decl->sto_info = NULL;
    decl->sto_analyzed = false;
    decl->needs_overflow_check = false;
    
    token_free(tok);  // consume identifier
    tok = lexer_next_token(lexer);  // read next
    if (!tok) return decl;  // EOF, no initializer
    
    // Check for optional '=' initializer
    if (tok->type != TOKEN_ASSIGN) {
        // YZ_CONST: Const variables must have initializer
        if (is_const) {
            fprintf(stderr, "Error: Const variable '%s' must have an initializer\n", decl->name);
            variable_declaration_free(decl);
            token_free(tok);
            return NULL;
        }
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
        tok->type == TOKEN_LPAREN ||  // YZ_19: list literal (1;2;) or parenthesized expression
        tok->type == TOKEN_LANGLE ||  // YZ_20: tuple literal <1,2>
        tok->type == TOKEN_LESS ||    // YZ_112: tuple literal with whitespace before < (e.g., "= <1;2>")
        tok->type == TOKEN_NOT ||     // YZ_18: Handle NOT operator
        tok->type == TOKEN_MINUS) {   // YZ_41: Handle unary minus (negative numbers)
        
        // Use stateless arithmetic parser for expression parsing
        // tok is OWNED by us and will be consumed by arithmetic parser
        // NULL func for global scope (const declarations at top level)
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok, NULL);
        token_free(tok);  // arithmetic parser borrowed it, we still own it
        
        if (expr) {
            // Check if it's a simple literal (but not a collection)
            if (expr->is_literal && !expr->left && !expr->right && !expr->is_collection) {
                // Simple literal: numeric x = 5
                decl->value = strdup(expr->value);
                decl->storage = STORAGE_DATA;
                arithmetic_expr_free(expr);
            } else {
                // Complex expression or collection: numeric x = a + b, list x = (1;2;), tuple x = <1,2>
                decl->init_expr = expr;  // ✅ Store ArithmeticExpr*
                decl->storage = STORAGE_BSS;  // Runtime initialization
                decl->sto_analyzed = false;
            }
        }
        
        // DON'T read next token - caller will do it
        return decl;
    } else if (tok->type == TOKEN_STRING) {
        // String literal
        decl->value = strdup(tok->value);
        
        // Phase 2: STO analysis for string literals
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        *sto_info = codegen_sto_infer_string_type(decl->value, true);  // true = is constant
        decl->sto_info = sto_info;
        decl->sto_analyzed = true;
        
        // Update internal type based on STO (backward compatibility)
        if (sto_info->type == INTERNAL_TYPE_SSO_STRING) {
            decl->internal_str_type = INTERNAL_SSO;
        } else if (sto_info->type == INTERNAL_TYPE_HEAP_STRING) {
            decl->internal_str_type = INTERNAL_HEAP;
        } else if (sto_info->type == INTERNAL_TYPE_RODATA_STRING) {
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
        // Use array module's parser to create Collection*
        Collection* coll = array_parse_literal(lexer, tok);
        
        if (!coll) {
            fprintf(stderr, "Error: Failed to parse array literal\n");
            token_free(tok);
            variable_declaration_free(decl);
            return NULL;
        }
        
        // Wrap Collection* in ArithmeticExpr*
        ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
        memset(expr, 0, sizeof(ArithmeticExpr));
        expr->is_collection = 1;
        expr->collection = coll;
        expr->is_literal = 0;
        
        // Store as init_expr (void* → ArithmeticExpr*)
        decl->init_expr = (void*)expr;
        decl->storage = STORAGE_BSS;  // Runtime initialization
        decl->sto_analyzed = false;
        
        // tok is borrowed by array_parse_literal, so don't free here
        return decl;
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
    
    // Phase 2: Initialize STO fields
    assign->sto_info = NULL;
    assign->sto_analyzed = false;
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
    // NULL func for global scope (assignments at top level)
    ArithmeticExpr* expr = arithmetic_parse_expression_stateless(lexer, tok, NULL);
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
    
    // Phase 2: Free STO info
    if (decl->sto_info) free(decl->sto_info);
    
    free(decl);
}

// Free assignment
void variable_assignment_free(VariableAssignment* assign) {
    if (!assign) return;
    
    if (assign->name) free(assign->name);
    // value_expr would be freed here if used
    
    // Phase 2: Free STO info
    if (assign->sto_info) free(assign->sto_info);
    
    free(assign);
}
