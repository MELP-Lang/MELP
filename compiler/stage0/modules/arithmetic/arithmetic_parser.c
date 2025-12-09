#include "arithmetic_parser.h"
#include "../codegen_context/codegen_context.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ========================================
// ❌ DEPRECATED STATEFUL API (Kept for test compatibility - will be removed)
// ========================================

// Helper: Advance to next token
static void advance(ArithmeticParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    parser->current_token = lexer_next_token(parser->lexer);
}

// Ensure token is loaded (lazy loading)
static void ensure_token(ArithmeticParser* parser) {
    if (!parser->current_token) {
        parser->current_token = lexer_next_token(parser->lexer);
    }
}

// Create parser
ArithmeticParser* arithmetic_parser_create(Lexer* lexer) {
    ArithmeticParser* parser = malloc(sizeof(ArithmeticParser));
    parser->lexer = lexer;
    parser->current_token = NULL;  // Lazy loading - first token read on demand
    return parser;
}

// Free parser
void arithmetic_parser_free(ArithmeticParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    free(parser);
}

// Parse primary expression (numbers, variables, parentheses)
ArithmeticExpr* arithmetic_parse_primary(ArithmeticParser* parser) {
    if (!parser) {
        return NULL;
    }
    
    // Lazy load first token if needed
    ensure_token(parser);
    
    if (!parser->current_token) {
        return NULL;
    }
    
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    
    // Phase 2.3: Initialize TTO fields
    expr->tto_info = NULL;
    expr->tto_analyzed = false;
    expr->needs_overflow_check = false;
    
    // Number literal
    if (parser->current_token->type == TOKEN_NUMBER) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);
        expr->is_float = (strchr(expr->value, '.') != NULL);
        
        // Phase 2.3: TTO analysis for numeric literal
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        *tto = tto_infer_numeric_type(expr->value);
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = (tto->type == INTERNAL_TYPE_INT64);
        
        advance(parser);
        return expr;
    }
    
    // Variable
    if (parser->current_token->type == TOKEN_IDENTIFIER) {
        expr->is_literal = 0;
        expr->value = strdup(parser->current_token->value);
        expr->is_float = 0;
        
        // Phase 2.3: TTO info for variables (unknown at parse time)
        // We assume INT64 initially, will be refined at runtime
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_INT64;
        tto->is_constant = false;  // Variables can change
        tto->needs_promotion = true;  // May need overflow check
        tto->mem_location = MEM_REGISTER;
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = true;
        
        advance(parser);
        return expr;
    }
    
    // Parentheses
    if (parser->current_token->type == TOKEN_LPAREN) {
        advance(parser);  // consume '('
        free(expr);  // We don't need this expr
        expr = arithmetic_parse_expression(parser);
        
        if (parser->current_token->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            arithmetic_expr_free(expr);
            return NULL;
        }
        advance(parser);  // consume ')'
        return expr;
    }
    
    fprintf(stderr, "Error: Unexpected token in arithmetic expression\n");
    free(expr);
    return NULL;
}

// Parse power (highest precedence: **)
ArithmeticExpr* arithmetic_parse_power(ArithmeticParser* parser) {
    ArithmeticExpr* left = arithmetic_parse_primary(parser);
    if (!left) return NULL;
    
    while (parser->current_token && parser->current_token->type == TOKEN_POWER) {
        advance(parser);  // consume **
        
        ArithmeticExpr* right = arithmetic_parse_primary(parser);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        // Create binary operation node
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = ARITH_POW;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        
        // Phase 2.3: TTO type propagation for binary operation
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        // If both operands have TTO info, propagate types
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, ARITH_POW);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse term (multiplication, division, modulo)
ArithmeticExpr* arithmetic_parse_term(ArithmeticParser* parser) {
    ArithmeticExpr* left = arithmetic_parse_power(parser);  // Changed from primary to power
    if (!left) return NULL;
    
    while (parser->current_token) {
        ArithmeticOp op;
        
        if (parser->current_token->type == TOKEN_MULTIPLY) {
            op = ARITH_MUL;
        } else if (parser->current_token->type == TOKEN_DIVIDE) {
            op = ARITH_DIV;
        } else if (parser->current_token->type == TOKEN_MOD) {
            op = ARITH_MOD;
        } else {
            break;  // Not a term-level operator
        }
        
        advance(parser);  // consume operator
        
        ArithmeticExpr* right = arithmetic_parse_power(parser);  // Changed from primary to power
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        // Create binary operation node
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        
        // Phase 2.3: TTO type propagation
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse factor (addition, subtraction)
ArithmeticExpr* arithmetic_parse_factor(ArithmeticParser* parser) {
    ArithmeticExpr* left = arithmetic_parse_term(parser);
    if (!left) return NULL;
    
    while (parser->current_token) {
        ArithmeticOp op;
        
        if (parser->current_token->type == TOKEN_PLUS) {
            op = ARITH_ADD;
        } else if (parser->current_token->type == TOKEN_MINUS) {
            op = ARITH_SUB;
        } else {
            break;  // Not a factor-level operator
        }
        
        advance(parser);  // consume operator
        
        ArithmeticExpr* right = arithmetic_parse_term(parser);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        // Create binary operation node
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        
        // Phase 2.3: TTO type propagation
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse bitwise (lowest precedence: &, |, ^)
ArithmeticExpr* arithmetic_parse_bitwise(ArithmeticParser* parser) {
    ArithmeticExpr* left = arithmetic_parse_factor(parser);
    if (!left) return NULL;
    
    while (parser->current_token) {
        ArithmeticOp op;
        
        if (parser->current_token->type == TOKEN_AND) {
            op = ARITH_AND;
        } else if (parser->current_token->type == TOKEN_OR) {
            op = ARITH_OR;
        } else if (parser->current_token->type == TOKEN_XOR) {
            op = ARITH_XOR;
        } else {
            break;  // Not a bitwise operator
        }
        
        advance(parser);  // consume operator
        
        ArithmeticExpr* right = arithmetic_parse_factor(parser);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        // Create binary operation node
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        
        // Phase 2.3: TTO type propagation
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse full expression (entry point)
ArithmeticExpr* arithmetic_parse_expression(ArithmeticParser* parser) {
    return arithmetic_parse_bitwise(parser);  // Changed from factor to bitwise
}

// Parse assignment: varname = expression
// Returns variable name, fills expr pointer
char* arithmetic_parse_assignment(ArithmeticParser* parser, ArithmeticExpr** expr) {
    if (!parser || !parser->current_token || !expr) {
        return NULL;
    }
    
    // We expect: IDENTIFIER = EXPRESSION
    if (parser->current_token->type != TOKEN_IDENTIFIER) {
        return NULL;  // Not an assignment
    }
    
    char* var_name = strdup(parser->current_token->value);
    advance(parser);
    
    // Check for '='
    if (!parser->current_token || parser->current_token->type != TOKEN_EQUAL) {
        free(var_name);
        return NULL;  // Not an assignment
    }
    
    advance(parser);  // consume '='
    
    // Parse expression
    *expr = arithmetic_parse_expression(parser);
    
    if (!*expr) {
        free(var_name);
        return NULL;
    }
    
    return var_name;
}

// ========================================
// ✅ STATELESS API IMPLEMENTATION (New)
// ========================================

// Forward declarations for stateless recursive descent
static ArithmeticExpr* parse_primary_stateless(Lexer* lexer, Token** current);
static ArithmeticExpr* parse_power_stateless(Lexer* lexer, Token** current);
static ArithmeticExpr* parse_term_stateless(Lexer* lexer, Token** current);
static ArithmeticExpr* parse_factor_stateless(Lexer* lexer, Token** current);
static ArithmeticExpr* parse_bitwise_stateless(Lexer* lexer, Token** current);

// Helper: Advance to next token (stateless)
static void advance_stateless(Lexer* lexer, Token** current) {
    if (*current) {
        token_free(*current);
    }
    *current = lexer_next_token(lexer);
}

// Parse primary (stateless)
static ArithmeticExpr* parse_primary_stateless(Lexer* lexer, Token** current) {
    if (!current || !*current) return NULL;
    
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    expr->tto_info = NULL;
    expr->tto_analyzed = false;
    expr->needs_overflow_check = false;
    
    // Number literal
    if ((*current)->type == TOKEN_NUMBER) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = (strchr(expr->value, '.') != NULL);
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        *tto = tto_infer_numeric_type(expr->value);
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = (tto->type == INTERNAL_TYPE_INT64);
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // Variable
    if ((*current)->type == TOKEN_IDENTIFIER) {
        expr->is_literal = 0;
        expr->value = strdup((*current)->value);
        expr->is_float = 0;
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_INT64;
        tto->is_constant = false;
        tto->needs_promotion = true;
        tto->mem_location = MEM_REGISTER;
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = true;
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // Parentheses
    if ((*current)->type == TOKEN_LPAREN) {
        advance_stateless(lexer, current);
        free(expr);
        expr = parse_bitwise_stateless(lexer, current);
        
        if (!*current || (*current)->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            arithmetic_expr_free(expr);
            return NULL;
        }
        advance_stateless(lexer, current);
        return expr;
    }
    
    fprintf(stderr, "Error: Unexpected token in arithmetic expression\n");
    free(expr);
    return NULL;
}

// Parse power (stateless)
static ArithmeticExpr* parse_power_stateless(Lexer* lexer, Token** current) {
    ArithmeticExpr* left = parse_primary_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current && (*current)->type == TOKEN_POWER) {
        advance_stateless(lexer, current);
        
        ArithmeticExpr* right = parse_primary_stateless(lexer, current);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = ARITH_POW;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, ARITH_POW);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse term (stateless)
static ArithmeticExpr* parse_term_stateless(Lexer* lexer, Token** current) {
    ArithmeticExpr* left = parse_power_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current) {
        ArithmeticOp op;
        
        if ((*current)->type == TOKEN_MULTIPLY) {
            op = ARITH_MUL;
        } else if ((*current)->type == TOKEN_DIVIDE) {
            op = ARITH_DIV;
        } else if ((*current)->type == TOKEN_MOD) {
            op = ARITH_MOD;
        } else {
            break;
        }
        
        advance_stateless(lexer, current);
        
        ArithmeticExpr* right = parse_power_stateless(lexer, current);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse factor (stateless)
static ArithmeticExpr* parse_factor_stateless(Lexer* lexer, Token** current) {
    ArithmeticExpr* left = parse_term_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current) {
        ArithmeticOp op;
        
        if ((*current)->type == TOKEN_PLUS) {
            op = ARITH_ADD;
        } else if ((*current)->type == TOKEN_MINUS) {
            op = ARITH_SUB;
        } else {
            break;
        }
        
        advance_stateless(lexer, current);
        
        ArithmeticExpr* right = parse_term_stateless(lexer, current);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// Parse bitwise (stateless)
static ArithmeticExpr* parse_bitwise_stateless(Lexer* lexer, Token** current) {
    ArithmeticExpr* left = parse_factor_stateless(lexer, current);
    if (!left) return NULL;
    
    while (*current) {
        ArithmeticOp op;
        
        if ((*current)->type == TOKEN_AND) {
            op = ARITH_AND;
        } else if ((*current)->type == TOKEN_OR) {
            op = ARITH_OR;
        } else if ((*current)->type == TOKEN_XOR) {
            op = ARITH_XOR;
        } else {
            break;
        }
        
        advance_stateless(lexer, current);
        
        ArithmeticExpr* right = parse_factor_stateless(lexer, current);
        if (!right) {
            arithmetic_expr_free(left);
            return NULL;
        }
        
        ArithmeticExpr* binary = malloc(sizeof(ArithmeticExpr));
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->tto_info = NULL;
        binary->tto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->tto_analyzed && right->tto_analyzed && left->tto_info && right->tto_info) {
            TTOTypeInfo* propagated = malloc(sizeof(TTOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->tto_info, right->tto_info, op);
            binary->tto_info = propagated;
            binary->tto_analyzed = true;
            binary->needs_overflow_check = (propagated->type == INTERNAL_TYPE_INT64);
        }
        
        left = binary;
    }
    
    return left;
}

// PUBLIC: Parse expression (stateless) - entry point
ArithmeticExpr* arithmetic_parse_expression_stateless(Lexer* lexer, Token* first_token) {
    if (!lexer || !first_token) return NULL;
    
    // We borrow first_token but advance consumes tokens
    // So we need to copy first_token to avoid freeing caller's token
    Token* current = malloc(sizeof(Token));
    current->type = first_token->type;
    current->value = first_token->value ? strdup(first_token->value) : NULL;
    current->line = first_token->line;
    
    ArithmeticExpr* result = parse_bitwise_stateless(lexer, &current);
    
    // ✅ FIX: Don't free remaining token, push it back to lexer
    // This token is the first NON-expression token (e.g. "if", newline, etc)
    if (current) {
        lexer_unget_token(lexer, current);  // ✅ Give it back!
    }
    
    return result;
}

// PUBLIC: Parse assignment (stateless)
char* arithmetic_parse_assignment_stateless(Lexer* lexer, Token* first_token, ArithmeticExpr** expr) {
    if (!lexer || !first_token || !expr) return NULL;
    
    if (first_token->type != TOKEN_IDENTIFIER) return NULL;
    
    char* var_name = strdup(first_token->value);
    
    // Read '='
    Token* eq_tok = lexer_next_token(lexer);
    if (!eq_tok || eq_tok->type != TOKEN_EQUAL) {
        token_free(eq_tok);
        free(var_name);
        return NULL;
    }
    token_free(eq_tok);
    
    // Read first expression token
    Token* expr_tok = lexer_next_token(lexer);
    if (!expr_tok) {
        free(var_name);
        return NULL;
    }
    
    // Parse expression
    *expr = arithmetic_parse_expression_stateless(lexer, expr_tok);
    token_free(expr_tok);
    
    if (!*expr) {
        free(var_name);
        return NULL;
    }
    
    return var_name;
}
