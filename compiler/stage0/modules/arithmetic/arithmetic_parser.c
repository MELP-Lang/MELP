#include "arithmetic_parser.h"
#include "arithmetic_optimize.h"  // YZ_32: Constant folding
#include "string_interpolation.h"  // YZ_90: String interpolation
#include "../codegen_context/codegen_context.h"
#include "../array/array_parser.h"  // YZ_14: For array index access
#include "../functions/functions.h"  // YZ_36: For function_is_known()
#include "../struct/struct.h"  // YZ_82: For member access
#include "../enum/enum.h"  // YZ_96: For enum value access
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
    memset(expr, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    
    // Phase 2.3: Initialize STO fields
    expr->sto_info = NULL;
    expr->sto_analyzed = false;
    expr->needs_overflow_check = false;
    
    // Boolean literal (true/false)
    if (parser->current_token->type == TOKEN_TRUE || parser->current_token->type == TOKEN_FALSE) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);  // "true" or "false"
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 1;
        
        // STO analysis for boolean literal
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_INT64;  // Boolean stored as int (0/1)
        sto_info->is_constant = true;  // Boolean literals are constant
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_REGISTER;  // Small value, keep in register
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance(parser);
        return expr;
    }
    
    // String literal
    if (parser->current_token->type == TOKEN_STRING) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);
        expr->is_float = 0;
        expr->is_string = 1;
        expr->is_boolean = 0;
        
        // STO analysis for string literal
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_SSO_STRING;  // Small String Optimization
        sto_info->is_constant = true;  // String literals are constant
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_RODATA;  // String literals in .rodata
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance(parser);
        return expr;
    }
    
    // Number literal
    if (parser->current_token->type == TOKEN_NUMBER) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);
        expr->is_float = (strchr(expr->value, '.') != NULL);
        expr->is_string = 0;
        expr->is_boolean = 0;
        
        // Phase 2.3: STO analysis for numeric literal
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        *sto_info = codegen_sto_infer_numeric_type(expr->value);
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = (sto_info->type == INTERNAL_TYPE_INT64);
        
        advance(parser);
        return expr;
    }
    
    // YZ_21: Boolean literals (true/false)
    if (parser->current_token->type == TOKEN_TRUE || parser->current_token->type == TOKEN_FALSE) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 1;
        
        // STO analysis: boolean = INTERNAL_TYPE_BOOLEAN
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_BOOLEAN;
        sto_info->is_constant = true;
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_REGISTER;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance(parser);
        return expr;
    }
    
    // Variable or Function Call
    // YZ_25: Also accept type keywords as function names (for type conversion: string(), numeric(), boolean())
    if (parser->current_token->type == TOKEN_IDENTIFIER ||
        parser->current_token->type == TOKEN_STRING_TYPE ||
        parser->current_token->type == TOKEN_NUMERIC ||
        parser->current_token->type == TOKEN_BOOLEAN ||
        parser->current_token->type == TOKEN_LIST) {
        char* identifier = strdup(parser->current_token->value);
        advance(parser);
        
        // Phase 3.5: Check for function call
        if (parser->current_token && parser->current_token->type == TOKEN_LPAREN) {
            // It's a function call: identifier(args...)
            advance(parser);  // consume '('
            
            // Parse arguments
            ArithmeticExpr** arguments = NULL;
            int arg_count = 0;
            int arg_capacity = 4;
            
            if (parser->current_token->type != TOKEN_RPAREN) {
                arguments = malloc(sizeof(ArithmeticExpr*) * arg_capacity);
                
                // Parse first argument
                ArithmeticExpr* arg = arithmetic_parse_expression(parser);
                if (!arg) {
                    free(identifier);
                    free(arguments);
                    free(expr);
                    return NULL;
                }
                arguments[arg_count++] = arg;
                
                // Parse remaining arguments (semicolon separated - PMPL standard)
                while (parser->current_token && 
                       parser->current_token->type == TOKEN_SEMICOLON) {
                    advance(parser);  // consume ';'
                    
                    // Resize array if needed
                    if (arg_count >= arg_capacity) {
                        arg_capacity *= 2;
                        arguments = realloc(arguments, sizeof(ArithmeticExpr*) * arg_capacity);
                    }
                    
                    arg = arithmetic_parse_expression(parser);
                    if (!arg) {
                        for (int i = 0; i < arg_count; i++) {
                            arithmetic_expr_free(arguments[i]);
                        }
                        free(identifier);
                        free(arguments);
                        free(expr);
                        return NULL;
                    }
                    arguments[arg_count++] = arg;
                }
            }
            
            if (!parser->current_token || parser->current_token->type != TOKEN_RPAREN) {
                fprintf(stderr, "Error: Expected ')' after function arguments\n");
                for (int i = 0; i < arg_count; i++) {
                    arithmetic_expr_free(arguments[i]);
                }
                free(identifier);
                free(arguments);
                free(expr);
                return NULL;
            }
            advance(parser);  // consume ')'
            
            // Create function call expression
            FunctionCallExpr* func_call = malloc(sizeof(FunctionCallExpr));
            func_call->function_name = identifier;
            func_call->arguments = arguments;
            func_call->arg_count = arg_count;
            
            expr->is_literal = 0;
            expr->value = NULL;
            expr->is_float = 0;
            expr->is_function_call = 1;
            expr->func_call = func_call;
            
            // STO info: assume INT64 return value
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_INT64;
            sto_info->is_constant = false;
            sto_info->needs_promotion = true;
            sto_info->mem_location = MEM_REGISTER;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = true;
            
            return expr;
        }
        
        // It's a variable
        expr->is_literal = 0;
        expr->value = identifier;
        expr->is_float = 0;
        expr->is_string = 0;  // TODO: Infer from variable type
        expr->is_boolean = 0;
        expr->is_function_call = 0;
        expr->func_call = NULL;
        
        // Phase 2.3: STO info for variables (unknown at parse time)
        // We assume INT64 initially, will be refined at runtime
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_INT64;
        sto_info->is_constant = false;  // Variables can change
        sto_info->needs_promotion = true;  // May need overflow check
        sto_info->mem_location = MEM_REGISTER;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = true;
        
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = ARITH_POW;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_string = 0;
        binary->is_boolean = 0;
        
        // Phase 2.3: STO type propagation for binary operation
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        // If both operands have STO info, propagate types
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, ARITH_POW);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        
        // Phase 2.3: STO type propagation
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        
        // String type propagation: If either operand is string, result is string
        // This enables: text + text (concat), text + numeric (would need runtime conversion)
        binary->is_string = (left->is_string || right->is_string);
        
        // Phase 2.3: STO type propagation
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        
        // Phase 2.3: STO type propagation
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
    
    // YZ_18: Handle NOT operator (unary boolean NOT)
    if ((*current)->type == TOKEN_NOT) {
        advance_stateless(lexer, current);  // consume 'not'
        
        // Parse the operand
        ArithmeticExpr* operand = parse_primary_stateless(lexer, current);
        if (!operand) return NULL;
        
        // Create XOR with 1: not x = x xor 1
        ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
        memset(expr, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        expr->op = ARITH_XOR;
        expr->left = operand;
        expr->right = malloc(sizeof(ArithmeticExpr));
        memset(expr->right, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        expr->right->is_literal = 1;
        expr->right->value = strdup("1");
        expr->right->is_float = 0;
        expr->right->is_string = 0;
        expr->right->is_boolean = 0;  // Treat as numeric 1, not boolean
        expr->right->left = NULL;
        expr->right->right = NULL;
        expr->right->sto_info = NULL;
        expr->right->sto_analyzed = false;
        expr->right->needs_overflow_check = false;
        expr->right->is_function_call = 0;
        expr->right->func_call = NULL;
        expr->right->is_array_access = 0;
        expr->right->array_access = NULL;
        expr->right->is_collection = 0;
        expr->right->collection = NULL;
        
        expr->is_literal = 0;
        expr->value = NULL;
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 0;  // Result is also treated as numeric
        expr->sto_info = NULL;
        expr->sto_analyzed = false;
        expr->needs_overflow_check = false;
        expr->is_function_call = 0;
        expr->func_call = NULL;
        expr->is_array_access = 0;
        expr->array_access = NULL;
        expr->is_collection = 0;
        expr->collection = NULL;
        
        return expr;
    }
    
    // YZ_41: Handle unary MINUS operator (negative numbers)
    if ((*current)->type == TOKEN_MINUS) {
        advance_stateless(lexer, current);  // consume '-'
        
        // Parse the operand (must be a primary expression)
        ArithmeticExpr* operand = parse_primary_stateless(lexer, current);
        if (!operand) return NULL;
        
        // Create subtraction: 0 - operand
        ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
        memset(expr, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        expr->op = ARITH_SUB;
        expr->left = malloc(sizeof(ArithmeticExpr));
        memset(expr->left, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        expr->left->is_literal = 1;
        expr->left->value = strdup("0");
        expr->left->is_float = 0;
        expr->left->is_string = 0;
        expr->left->is_boolean = 0;
        expr->left->left = NULL;
        expr->left->right = NULL;
        expr->left->sto_info = NULL;
        expr->left->sto_analyzed = false;
        expr->left->needs_overflow_check = false;
        expr->left->is_function_call = 0;
        expr->left->func_call = NULL;
        expr->left->is_array_access = 0;
        expr->left->array_access = NULL;
        expr->left->is_collection = 0;
        expr->left->collection = NULL;
        
        expr->right = operand;
        expr->is_literal = 0;
        expr->value = NULL;
        expr->is_float = operand->is_float;  // Preserve float type
        expr->is_string = 0;
        expr->is_boolean = 0;
        expr->sto_info = NULL;
        expr->sto_analyzed = false;
        expr->needs_overflow_check = false;
        expr->is_function_call = 0;
        expr->func_call = NULL;
        expr->is_array_access = 0;
        expr->array_access = NULL;
        expr->is_collection = 0;
        expr->collection = NULL;
        
        return expr;
    }
    
    ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
    memset(expr, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    expr->sto_info = NULL;
    expr->sto_analyzed = false;
    expr->needs_overflow_check = false;
    expr->is_function_call = 0;
    expr->func_call = NULL;
    expr->is_array_access = 0;
    expr->array_access = NULL;
    expr->is_collection = 0;
    expr->collection = NULL;
    expr->is_member_access = 0;  // YZ_82: Member access
    expr->member_access = NULL;
    
    // Number literal
    if ((*current)->type == TOKEN_NUMBER) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = (strchr(expr->value, '.') != NULL);
        expr->is_string = 0;  // YZ_10: Not a string
        expr->is_boolean = 0;
        
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        *sto_info = codegen_sto_infer_numeric_type(expr->value);
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = (sto_info->type == INTERNAL_TYPE_INT64);
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // YZ_21: Boolean literals (true/false) - stateless
    if ((*current)->type == TOKEN_TRUE || (*current)->type == TOKEN_FALSE) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 1;
        
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_BOOLEAN;
        sto_info->is_constant = true;
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_REGISTER;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // YZ_10: String literal
    if ((*current)->type == TOKEN_STRING) {
        // YZ_90: Check for string interpolation
        if (string_has_interpolation((*current)->value)) {
            // Parse interpolation: "Hello ${name}" -> "Hello " + name
            char* str_value = strdup((*current)->value);
            advance_stateless(lexer, current);
            ArithmeticExpr* interp_expr = parse_string_interpolation(lexer, str_value);
            free(str_value);
            free(expr);  // Free the allocated expr since we're returning a different one
            return interp_expr;
        }
        
        // Regular string literal (no interpolation)
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = 0;
        expr->is_string = 1;  // YZ_10: This is a string!
        expr->is_boolean = 0;
        
        // STO analysis for string literal
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_SSO_STRING;  // Small String Optimization
        sto_info->is_constant = true;  // String literals are constant
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_RODATA;  // String literals in .rodata
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // Variable or Function Call or Collection Access (YZ_86: or 'self')
    // YZ_25: Also accept type keywords as function names (for type conversion: string(), numeric(), boolean())
    if ((*current)->type == TOKEN_IDENTIFIER || 
        (*current)->type == TOKEN_SELF ||
        (*current)->type == TOKEN_STRING_TYPE ||
        (*current)->type == TOKEN_NUMERIC ||
        (*current)->type == TOKEN_BOOLEAN ||
        (*current)->type == TOKEN_LIST) {
        char* identifier = strdup((*current)->value);
        advance_stateless(lexer, current);
        
        // YZ_29: Check for unqualified enum value FIRST (e.g., T_FUNCTION)
        // This handles: numeric tok = T_FUNCTION
        int64_t unqualified_enum = enum_lookup_value_unqualified(identifier);
        if (unqualified_enum != -1) {
            // Found enum value! Replace identifier with literal
            char value_str[32];
            snprintf(value_str, sizeof(value_str), "%ld", unqualified_enum);
            
            free(expr->value);
            expr->value = strdup(value_str);
            expr->is_literal = 1;
            
            free(identifier);
            return expr;
        }
        
        // YZ_23: PRIORITY ORDER (most specific first):
        // 1. Collection access: arr[i], list(i), tuple<i>
        // 2. Function call: func(args...)
        // 3. Variable: x
        
        // YZ_29: For TOKEN_LPAREN, we need to distinguish between:
        //   - Function call: toUpperCase(msg), println(x), add(10, 20)
        //   - List access: mylist(0)
        // YZ_36: Solution: Check if identifier is a known function (builtin or user-defined)
        int is_known_func = function_is_known(identifier);
        
        // YZ_27: For TOKEN_LPAREN with unknown identifier, peek ahead to distinguish:
        //   - List access: mylist(0) - single expression, no comma
        //   - Function call: parse_literal(tokens, pos) - has comma (multiple params)
        //   - Function call: unknown_func() - empty parens
        // 
        // LIMITATION: Lexer only supports 1-token pushback, so we can't lookahead 2 tokens
        // SOLUTION: Use naming heuristic for common patterns
        // YZ_30: Enhanced heuristics - more function patterns
        int is_list_access_syntax = 0;
        if (*current && (*current)->type == TOKEN_LPAREN && !is_known_func) {
            // Not a known function - use heuristic to distinguish
            // Common function name prefixes/patterns
            int looks_like_function = 0;
            
            // Prefix-based patterns
            if (strncmp(identifier, "parse_", 6) == 0 ||
                strncmp(identifier, "get_", 4) == 0 ||
                strncmp(identifier, "create_", 7) == 0 ||
                strncmp(identifier, "is_", 3) == 0 ||
                strncmp(identifier, "check_", 6) == 0 ||
                strncmp(identifier, "has_", 4) == 0 ||
                strncmp(identifier, "to_", 3) == 0 ||
                strncmp(identifier, "make_", 5) == 0 ||
                strncmp(identifier, "build_", 6) == 0 ||
                strncmp(identifier, "add_", 4) == 0 ||
                strncmp(identifier, "remove_", 7) == 0 ||
                strncmp(identifier, "set_", 4) == 0 ||
                strncmp(identifier, "find_", 5) == 0 ||
                strncmp(identifier, "read_", 5) == 0 ||
                strncmp(identifier, "write_", 6) == 0 ||
                strncmp(identifier, "emit_", 5) == 0 ||
                strncmp(identifier, "generate_", 9) == 0 ||
                strncmp(identifier, "compile_", 8) == 0 ||
                strncmp(identifier, "process_", 8) == 0 ||
                strncmp(identifier, "handle_", 7) == 0 ||
                strncmp(identifier, "init_", 5) == 0 ||
                strncmp(identifier, "free_", 5) == 0 ||
                strncmp(identifier, "alloc_", 6) == 0 ||
                strncmp(identifier, "new_", 4) == 0 ||
                strncmp(identifier, "delete_", 7) == 0 ||
                strncmp(identifier, "load_", 5) == 0 ||
                strncmp(identifier, "save_", 5) == 0 ||
                strncmp(identifier, "open_", 5) == 0 ||
                strncmp(identifier, "close_", 6) == 0 ||
                strncmp(identifier, "print_", 6) == 0 ||
                strncmp(identifier, "scan_", 5) == 0 ||
                strncmp(identifier, "next_", 5) == 0 ||
                strncmp(identifier, "prev_", 5) == 0 ||
                strncmp(identifier, "first_", 6) == 0 ||
                strncmp(identifier, "last_", 5) == 0 ||
                strncmp(identifier, "push_", 5) == 0 ||
                strncmp(identifier, "pop_", 4) == 0 ||
                strncmp(identifier, "peek_", 5) == 0 ||
                strncmp(identifier, "clear_", 6) == 0 ||
                strncmp(identifier, "reset_", 6) == 0 ||
                strncmp(identifier, "update_", 7) == 0 ||
                strncmp(identifier, "append_", 7) == 0 ||
                strncmp(identifier, "insert_", 7) == 0 ||
                strncmp(identifier, "count_", 6) == 0 ||
                strncmp(identifier, "len_", 4) == 0 ||
                strncmp(identifier, "size_", 5) == 0 ||
                strncmp(identifier, "starts_with_", 12) == 0 ||   // YZ_30: string utilities
                strncmp(identifier, "ends_with_", 10) == 0 ||
                strncmp(identifier, "contains_", 9) == 0 ||
                strncmp(identifier, "skip_", 5) == 0 ||
                strncmp(identifier, "scan_", 5) == 0 ||
                strncmp(identifier, "tokenize_", 9) == 0 ||
                strncmp(identifier, "lex_", 4) == 0) {
                looks_like_function = 1;
            }
            
            // YZ_30: Exact match for common short function names
            // These are typical math/utility functions
            if (!looks_like_function) {
                if (strcmp(identifier, "add") == 0 ||
                    strcmp(identifier, "sub") == 0 ||
                    strcmp(identifier, "mul") == 0 ||
                    strcmp(identifier, "div") == 0 ||
                    strcmp(identifier, "mod") == 0 ||
                    strcmp(identifier, "pow") == 0 ||
                    strcmp(identifier, "sqrt") == 0 ||
                    strcmp(identifier, "abs") == 0 ||
                    strcmp(identifier, "min") == 0 ||
                    strcmp(identifier, "max") == 0 ||
                    strcmp(identifier, "sum") == 0 ||
                    strcmp(identifier, "avg") == 0 ||
                    strcmp(identifier, "len") == 0 ||
                    strcmp(identifier, "size") == 0 ||
                    strcmp(identifier, "count") == 0 ||
                    strcmp(identifier, "print") == 0 ||
                    strcmp(identifier, "println") == 0 ||
                    strcmp(identifier, "read") == 0 ||
                    strcmp(identifier, "write") == 0 ||
                    strcmp(identifier, "open") == 0 ||
                    strcmp(identifier, "close") == 0 ||
                    strcmp(identifier, "push") == 0 ||
                    strcmp(identifier, "pop") == 0 ||
                    strcmp(identifier, "peek") == 0 ||
                    strcmp(identifier, "append") == 0 ||
                    strcmp(identifier, "insert") == 0 ||
                    strcmp(identifier, "remove") == 0 ||
                    strcmp(identifier, "clear") == 0 ||
                    strcmp(identifier, "reset") == 0 ||
                    strcmp(identifier, "init") == 0 ||
                    strcmp(identifier, "free") == 0 ||
                    strcmp(identifier, "alloc") == 0 ||
                    strcmp(identifier, "malloc") == 0 ||
                    strcmp(identifier, "realloc") == 0 ||
                    strcmp(identifier, "calloc") == 0 ||
                    strcmp(identifier, "strcmp") == 0 ||
                    strcmp(identifier, "strlen") == 0 ||
                    strcmp(identifier, "strcpy") == 0 ||
                    strcmp(identifier, "strcat") == 0 ||
                    strcmp(identifier, "memcpy") == 0 ||
                    strcmp(identifier, "memset") == 0 ||
                    strcmp(identifier, "floor") == 0 ||
                    strcmp(identifier, "ceil") == 0 ||
                    strcmp(identifier, "round") == 0 ||
                    strcmp(identifier, "sin") == 0 ||
                    strcmp(identifier, "cos") == 0 ||
                    strcmp(identifier, "tan") == 0 ||
                    strcmp(identifier, "log") == 0 ||
                    strcmp(identifier, "exp") == 0 ||
                    strcmp(identifier, "rand") == 0 ||
                    strcmp(identifier, "srand") == 0 ||
                    strcmp(identifier, "exit") == 0 ||
                    strcmp(identifier, "assert") == 0 ||
                    strcmp(identifier, "error") == 0 ||
                    strcmp(identifier, "warn") == 0 ||
                    strcmp(identifier, "debug") == 0 ||
                    strcmp(identifier, "info") == 0 ||
                    strcmp(identifier, "trace") == 0 ||
                    strcmp(identifier, "call") == 0 ||
                    strcmp(identifier, "invoke") == 0 ||
                    strcmp(identifier, "run") == 0 ||
                    strcmp(identifier, "exec") == 0 ||
                    strcmp(identifier, "eval") == 0 ||
                    strcmp(identifier, "apply") == 0 ||
                    strcmp(identifier, "map") == 0 ||
                    strcmp(identifier, "filter") == 0 ||
                    strcmp(identifier, "reduce") == 0 ||
                    strcmp(identifier, "sort") == 0 ||
                    strcmp(identifier, "reverse") == 0 ||
                    strcmp(identifier, "find") == 0 ||
                    strcmp(identifier, "search") == 0 ||
                    strcmp(identifier, "index") == 0 ||
                    strcmp(identifier, "contains") == 0 ||
                    strcmp(identifier, "exists") == 0 ||
                    strcmp(identifier, "empty") == 0 ||
                    strcmp(identifier, "full") == 0 ||
                    strcmp(identifier, "copy") == 0 ||
                    strcmp(identifier, "clone") == 0 ||
                    strcmp(identifier, "dup") == 0 ||
                    strcmp(identifier, "swap") == 0 ||
                    strcmp(identifier, "move") == 0 ||
                    strcmp(identifier, "shift") == 0 ||
                    strcmp(identifier, "unshift") == 0 ||
                    strcmp(identifier, "slice") == 0 ||
                    strcmp(identifier, "splice") == 0 ||
                    strcmp(identifier, "concat") == 0 ||
                    strcmp(identifier, "join") == 0 ||
                    strcmp(identifier, "split") == 0 ||
                    strcmp(identifier, "trim") == 0 ||
                    strcmp(identifier, "upper") == 0 ||
                    strcmp(identifier, "lower") == 0 ||
                    strcmp(identifier, "format") == 0 ||
                    strcmp(identifier, "parse") == 0 ||
                    strcmp(identifier, "encode") == 0 ||
                    strcmp(identifier, "decode") == 0 ||
                    strcmp(identifier, "hash") == 0 ||
                    strcmp(identifier, "compare") == 0 ||
                    strcmp(identifier, "equals") == 0 ||
                    strcmp(identifier, "match") == 0 ||
                    strcmp(identifier, "test") == 0 ||
                    strcmp(identifier, "check") == 0 ||
                    strcmp(identifier, "verify") == 0 ||
                    strcmp(identifier, "validate") == 0) {
                    looks_like_function = 1;
                }
            }
            
            // YZ_31: Add test_ prefix pattern for test functions
            if (!looks_like_function) {
                if (strncmp(identifier, "test", 4) == 0) {
                    // test, test1, test2, test_something, etc.
                    looks_like_function = 1;
                }
            }
            
            // YZ_31: If still not recognized, peek ahead to check for comma
            // Multiple args (comma) = definitely a function call
            if (!looks_like_function) {
                // Peek into parentheses to check for comma
                // Save current position in lexer source
                Token* peek1 = lexer_next_token(lexer);  // consume '('
                if (peek1 && peek1->type == TOKEN_LPAREN) {
                    token_free(peek1);  // We already consumed '(' via *current
                }
                Token* peek2 = lexer_next_token(lexer);  // first arg or ')'
                if (peek2) {
                    if (peek2->type == TOKEN_RPAREN) {
                        // Empty parens: foo() - this IS a function call
                        looks_like_function = 1;
                        lexer_unget_token(lexer, peek2);
                    } else {
                        Token* peek3 = lexer_next_token(lexer);  // check for semicolon or ')'
                        if (peek3) {
                            // YZ_31: MELP uses semicolon (;) as parameter separator
                            // because comma is used for decimal numbers (123,45)
                            // Example: test(123,98; true; "deneme")
                            if (peek3->type == TOKEN_SEMICOLON) {
                                // Has semicolon: foo(a; b) - this IS a function call
                                looks_like_function = 1;
                            }
                            lexer_unget_token(lexer, peek3);
                        }
                        lexer_unget_token(lexer, peek2);
                    }
                }
            }
            
            // Decision: list access only if does NOT look like function
            if (!looks_like_function) {
                is_list_access_syntax = 1;
            }
            // Otherwise: treat as function call (forward reference)
        }
        
        // YZ_23/YZ_27: Check for array/list/tuple access
        if (*current && ((*current)->type == TOKEN_LBRACKET || 
                        is_list_access_syntax ||
                        (*current)->type == TOKEN_LANGLE)) {
            // Array: identifier[index]
            // List:  identifier(index)  
            // Tuple: identifier<index>
            IndexAccess* access = array_parse_index_access(lexer, identifier, *current);
            if (!access) {
                free(identifier);
                free(expr);
                return NULL;
            }
            
            // Create array access expression
            expr->is_literal = 0;
            expr->value = NULL;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            expr->is_function_call = 0;
            expr->func_call = NULL;
            expr->is_array_access = 1;
            expr->array_access = access;
            
            // STO info: result is INT64 (collection element)
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_INT64;
            sto_info->is_constant = false;
            sto_info->needs_promotion = true;
            sto_info->mem_location = MEM_REGISTER;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = true;
            
            // array_parse_index_access consumed tokens, update current
            advance_stateless(lexer, current);
            
            free(identifier);  // IndexAccess has its own copy
            return expr;
        }
        
        // YZ_82/YZ_83/YZ_86: Check for member access or method call (p.x or p.method())
        // YZ_96: Also check for enum value access (Status.ACTIVE)
        if (*current && (*current)->type == TOKEN_DOT) {
            advance_stateless(lexer, current);  // consume '.'
            
            if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Error: Expected member/method name after '.'\n");
                free(identifier);
                free(expr);
                return NULL;
            }
            
            char* member_or_method = strdup((*current)->value);
            advance_stateless(lexer, current);  // consume member/method name
            
            // YZ_96: Check if this is an enum value access (EnumName.VALUE)
            if (enum_is_type(identifier)) {
                int64_t enum_value = enum_lookup_value(identifier, member_or_method);
                if (enum_value >= 0) {
                    // Create literal expression for enum value
                    char value_str[32];
                    snprintf(value_str, sizeof(value_str), "%ld", enum_value);
                    
                    free(expr->value);
                    expr->value = strdup(value_str);
                    expr->is_literal = 1;
                    expr->is_member_access = 0;
                    
                    free(member_or_method);
                    free(identifier);
                    return expr;
                }
            }
            
            // YZ_86: Check if this is a method call (followed by '(')
            if (*current && (*current)->type == TOKEN_LPAREN) {
                advance_stateless(lexer, current);  // consume '('
                
                // Parse method arguments
                ArithmeticExpr** arguments = NULL;
                int arg_count = 0;
                int arg_capacity = 4;
                
                if ((*current)->type != TOKEN_RPAREN) {
                    arguments = malloc(sizeof(ArithmeticExpr*) * arg_capacity);
                    
                    while (1) {
                        ArithmeticExpr* arg = parse_bitwise_stateless(lexer, current);
                        if (!arg) {
                            for (int i = 0; i < arg_count; i++) {
                                arithmetic_expr_free(arguments[i]);
                            }
                            free(arguments);
                            free(member_or_method);
                            free(identifier);
                            free(expr);
                            return NULL;
                        }
                        
                        if (arg_count >= arg_capacity) {
                            arg_capacity *= 2;
                            arguments = realloc(arguments, sizeof(ArithmeticExpr*) * arg_capacity);
                        }
                        arguments[arg_count++] = arg;
                        
                        if (!*current) {
                            fprintf(stderr, "Error: Unexpected EOF in method call\n");
                            for (int i = 0; i < arg_count; i++) {
                                arithmetic_expr_free(arguments[i]);
                            }
                            free(arguments);
                            free(member_or_method);
                            free(identifier);
                            free(expr);
                            return NULL;
                        }
                        
                        if ((*current)->type == TOKEN_RPAREN) {
                            break;
                        } else if ((*current)->type == TOKEN_SEMICOLON) {
                            advance_stateless(lexer, current);  // consume ';'
                        } else {
                            fprintf(stderr, "Error: Expected ';' or ')' in method call\n");
                            for (int i = 0; i < arg_count; i++) {
                                arithmetic_expr_free(arguments[i]);
                            }
                            free(arguments);
                            free(member_or_method);
                            free(identifier);
                            free(expr);
                            return NULL;
                        }
                    }
                }
                
                advance_stateless(lexer, current);  // consume ')'
                
                // Create method call structure
                MethodCall* method_call = method_call_create(identifier, member_or_method);
                for (int i = 0; i < arg_count; i++) {
                    // Note: This is a bit hacky - we're storing ArithmeticExpr* as Expression*
                    // In a full implementation, we'd convert these properly
                    method_call_add_arg(method_call, (Expression*)arguments[i]);
                }
                free(arguments);
                free(member_or_method);
                free(identifier);
                
                // Create method call expression
                expr->is_literal = 0;
                expr->value = NULL;
                expr->is_float = 0;
                expr->is_string = 0;
                expr->is_boolean = 0;
                expr->is_function_call = 0;
                expr->func_call = NULL;
                expr->is_array_access = 0;
                expr->array_access = NULL;
                expr->is_member_access = 0;
                expr->member_access = NULL;
                expr->is_method_call = 1;  // YZ_86: New flag
                expr->method_call = (void*)method_call;
                
                // STO info: assume numeric return (INT64)
                STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
                sto_info->type = INTERNAL_TYPE_INT64;
                sto_info->is_constant = false;
                sto_info->needs_promotion = true;
                sto_info->mem_location = MEM_STACK;
                expr->sto_info = sto_info;
                expr->sto_analyzed = true;
                expr->needs_overflow_check = true;
                
                return expr;
            }
            
            // Parse member chain if not a method call (p.x or p.addr.zip)
            char** member_chain = NULL;
            int chain_length = 0;
            int chain_capacity = 4;
            member_chain = malloc(sizeof(char*) * chain_capacity);
            member_chain[chain_length++] = member_or_method;  // First member already parsed
            
            // Loop to collect remaining chained members
            while (*current && (*current)->type == TOKEN_DOT) {
                advance_stateless(lexer, current);  // consume '.'
                
                if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
                    fprintf(stderr, "Error: Expected member name after '.'\n");
                    for (int i = 0; i < chain_length; i++) {
                        free(member_chain[i]);
                    }
                    free(member_chain);
                    free(identifier);
                    free(expr);
                    return NULL;
                }
                
                // Expand array if needed
                if (chain_length >= chain_capacity) {
                    chain_capacity *= 2;
                    member_chain = realloc(member_chain, sizeof(char*) * chain_capacity);
                }
                
                member_chain[chain_length++] = strdup((*current)->value);
                advance_stateless(lexer, current);  // consume member name
            }
            
            // Create MemberAccess structure with first member
            MemberAccess* access = struct_create_member_access(identifier, member_chain[0]);
            free(identifier);
            
            if (!access) {
                for (int i = 0; i < chain_length; i++) {
                    free(member_chain[i]);
                }
                free(member_chain);
                free(expr);
                return NULL;
            }
            
            // YZ_83: Replace member_chain with full chain
            for (int i = 0; i < access->chain_length; i++) {
                free(access->member_chain[i]);
            }
            free(access->member_chain);
            access->member_chain = member_chain;
            access->chain_length = chain_length;
            
            // Create member access expression
            expr->is_literal = 0;
            expr->value = NULL;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            expr->is_function_call = 0;
            expr->func_call = NULL;
            expr->is_array_access = 0;
            expr->array_access = NULL;
            expr->is_member_access = 1;
            expr->member_access = (void*)access;
            
            // STO info: assume numeric member (INT64)
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_INT64;
            sto_info->is_constant = false;
            sto_info->needs_promotion = true;
            sto_info->mem_location = MEM_STACK;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = true;
            
            return expr;
        }
        
        // Phase 3.5: Check for function call
        // YZ_29: Now this code is reachable for builtin functions!
        // YZ_36: And also for user-defined functions!
        // YZ_27: And also for unknown functions (forward references)!
        if (*current && (*current)->type == TOKEN_LPAREN) {
            // It's a function call: identifier(args...)
            // Could be known function OR forward reference to unknown function
            advance_stateless(lexer, current);  // consume '('
            
            // Parse arguments
            ArithmeticExpr** arguments = NULL;
            int arg_count = 0;
            int arg_capacity = 4;
            
            if ((*current)->type != TOKEN_RPAREN) {
                arguments = malloc(sizeof(ArithmeticExpr*) * arg_capacity);
                
                // Parse first argument
                ArithmeticExpr* arg = parse_bitwise_stateless(lexer, current);
                if (!arg) {
                    free(identifier);
                    free(arguments);
                    free(expr);
                    return NULL;
                }
                arguments[arg_count++] = arg;
                
                // Parse remaining arguments (semicolon separated - PMPL standard)
                while (*current && 
                       (*current)->type == TOKEN_SEMICOLON) {
                    advance_stateless(lexer, current);  // consume ';'
                    
                    // Resize array if needed
                    if (arg_count >= arg_capacity) {
                        arg_capacity *= 2;
                        arguments = realloc(arguments, sizeof(ArithmeticExpr*) * arg_capacity);
                    }
                    
                    arg = parse_bitwise_stateless(lexer, current);
                    if (!arg) {
                        for (int i = 0; i < arg_count; i++) {
                            arithmetic_expr_free(arguments[i]);
                        }
                        free(identifier);
                        free(arguments);
                        free(expr);
                        return NULL;
                    }
                    arguments[arg_count++] = arg;
                }
            }
            
            if (!*current || (*current)->type != TOKEN_RPAREN) {
                fprintf(stderr, "Error: Expected ')' after function arguments\n");
                for (int i = 0; i < arg_count; i++) {
                    arithmetic_expr_free(arguments[i]);
                }
                free(identifier);
                free(arguments);
                free(expr);
                return NULL;
            }
            advance_stateless(lexer, current);  // consume ')'
            
            // Create function call expression
            FunctionCallExpr* func_call = malloc(sizeof(FunctionCallExpr));
            func_call->function_name = identifier;
            func_call->arguments = arguments;
            func_call->arg_count = arg_count;
            
            expr->is_literal = 0;
            expr->value = NULL;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            expr->is_function_call = 1;
            expr->func_call = func_call;
            
            // STO info: assume INT64 return value
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_INT64;
            sto_info->is_constant = false;
            sto_info->needs_promotion = true;
            sto_info->mem_location = MEM_REGISTER;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = true;
            
            return expr;
        }
        
        // YZ_29: Check for member access on variables (e.g., tokens.length, list.type)
        // This handles member access on ANY identifier, not just structs
        if (*current && (*current)->type == TOKEN_DOT) {
            advance_stateless(lexer, current);  // consume '.'
            
            if (!*current || (*current)->type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Error: Expected member name after '.'\n");
                free(identifier);
                free(expr);
                return NULL;
            }
            
            char* member_name = strdup((*current)->value);
            advance_stateless(lexer, current);  // consume member name
            
            // Create a generic member access expression
            // For now, we treat this as: identifier.member
            // The codegen will handle built-in members like .length, .type, etc.
            
            // Build the member access string: "identifier.member"
            size_t access_len = strlen(identifier) + strlen(member_name) + 2;  // +2 for '.' and '\0'
            char* access_str = malloc(access_len);
            snprintf(access_str, access_len, "%s.%s", identifier, member_name);
            
            free(identifier);
            free(member_name);
            
            expr->is_literal = 0;
            expr->value = access_str;  // Store "identifier.member" as value
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            expr->is_member_access = 1;  // Mark as member access
            expr->member_access = NULL;  // No MemberAccess struct for now
            
            // STO info: assume numeric result (most common for .length, etc.)
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_INT64;
            sto_info->is_constant = false;
            sto_info->needs_promotion = true;
            sto_info->mem_location = MEM_REGISTER;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = true;
            
            return expr;
        }
        
        // It's a variable
        expr->is_literal = 0;
        expr->value = identifier;
        expr->is_float = 0;
        expr->is_string = 0;  // YZ_10: TODO - Infer from variable type in symbol table
        expr->is_boolean = 0;
        
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_INT64;
        sto_info->is_constant = false;
        sto_info->needs_promotion = true;
        sto_info->mem_location = MEM_REGISTER;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = true;
        
        return expr;
    }
    
    // ========== YZ_17: Array Literal [1, 2, 3] ==========
    if ((*current)->type == TOKEN_LBRACKET) {
        Token* lbracket = *current;
        Collection* coll = array_parse_literal(lexer, lbracket);
        
        if (!coll) {
            free(expr);
            return NULL;
        }
        
        advance_stateless(lexer, current);  // Update current token
        expr->is_literal = 1;
        expr->is_collection = 1;
        expr->collection = coll;
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 0;
        
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_ARRAY;
        sto_info->is_constant = false;
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_HEAP;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
        return expr;
    }
    
    // Parentheses or List literal
    if ((*current)->type == TOKEN_LPAREN) {
        // Lookahead to distinguish:
        // - Parenthesized expression: (x + y)  -> NO ';' after first expr
        // - List literal: (1; 2; 3;)  -> has ';' after each element
        
        // Strategy: Peek ahead to see if we have ';' or ')' after first element
        // We need to look at tokens without consuming them
        
        // Save the lparen token
        Token* lparen = *current;
        advance_stateless(lexer, current);  // Skip '('
        
        // Check for empty list: ()
        if ((*current)->type == TOKEN_RPAREN) {
            // Empty list
            advance_stateless(lexer, current);  // Skip ')'
            
            Collection* coll = malloc(sizeof(Collection));
            coll->type = COLL_LIST;
            coll->data.list.capacity = 0;
            coll->data.list.length = 0;
            coll->data.list.elements = NULL;
            coll->data.list.element_types = NULL;
            
            expr->is_literal = 1;
            expr->is_collection = 1;
            expr->collection = coll;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_LIST;
            sto_info->is_constant = false;
            sto_info->needs_promotion = false;
            sto_info->mem_location = MEM_HEAP;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = false;
            
            return expr;
        }
        
        // Parse first element to see what follows
        ArithmeticExpr* first_elem = parse_bitwise_stateless(lexer, current);
        if (!first_elem) {
            fprintf(stderr, "Error: Failed to parse element after '('\n");
            free(expr);
            return NULL;
        }
        
        // Check what comes after first element
        if ((*current)->type == TOKEN_SEMICOLON) {
            // It's a list! (elem1; ...)
            // We need to backtrack and reparse with list parser
            // Problem: we already consumed tokens!
            
            // WORKAROUND: Build list manually here
            int capacity = 4;
            int length = 1;
            ArithmeticExpr** elements = malloc(sizeof(ArithmeticExpr*) * capacity);
            VarType* types = malloc(sizeof(VarType) * capacity);
            elements[0] = first_elem;
            types[0] = VAR_NUMERIC;  // Default
            
            // Parse remaining elements
            while ((*current)->type == TOKEN_SEMICOLON) {
                advance_stateless(lexer, current);  // Skip ';'
                
                // Check for end of list
                if ((*current)->type == TOKEN_RPAREN) {
                    break;
                }
                
                // Resize if needed
                if (length >= capacity) {
                    capacity *= 2;
                    elements = realloc(elements, sizeof(ArithmeticExpr*) * capacity);
                    types = realloc(types, sizeof(VarType) * capacity);
                }
                
                // Parse next element
                ArithmeticExpr* elem = parse_bitwise_stateless(lexer, current);
                if (!elem) {
                    fprintf(stderr, "Error: Failed to parse list element\n");
                    for (int i = 0; i < length; i++) {
                        arithmetic_expr_free(elements[i]);
                    }
                    free(elements);
                    free(types);
                    free(expr);
                    return NULL;
                }
                elements[length] = elem;
                types[length] = VAR_NUMERIC;
                length++;
            }
            
            // Expect ')'
            if ((*current)->type != TOKEN_RPAREN) {
                fprintf(stderr, "Error: Expected ')' to close list\n");
                for (int i = 0; i < length; i++) {
                    arithmetic_expr_free(elements[i]);
                }
                free(elements);
                free(types);
                free(expr);
                return NULL;
            }
            advance_stateless(lexer, current);  // Skip ')'
            
            // Build list collection
            Collection* coll = malloc(sizeof(Collection));
            coll->type = COLL_LIST;
            coll->data.list.capacity = capacity;
            coll->data.list.length = length;
            coll->data.list.elements = (void**)elements;
            coll->data.list.element_types = types;
            
            expr->is_literal = 1;
            expr->is_collection = 1;
            expr->collection = coll;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_LIST;
            sto_info->is_constant = false;
            sto_info->needs_promotion = false;
            sto_info->mem_location = MEM_HEAP;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = false;
            
            return expr;
        }
        
        // Not a list - it's a parenthesized expression
        if ((*current)->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')' after parenthesized expression\n");
            arithmetic_expr_free(first_elem);
            free(expr);
            return NULL;
        }
        advance_stateless(lexer, current);  // Skip ')'
        
        free(expr);
        return first_elem;
    }
    
    // Tuple literal: <x, y, z>
    if ((*current)->type == TOKEN_LANGLE) {
        advance_stateless(lexer, current);  // Skip '<'
        
        // Check for empty tuple: <>
        if ((*current)->type == TOKEN_GREATER) {
            // Empty tuple
            advance_stateless(lexer, current);  // Skip '>'
            
            Collection* coll = malloc(sizeof(Collection));
            coll->type = COLL_TUPLE;
            coll->data.tuple.length = 0;
            coll->data.tuple.elements = NULL;
            coll->data.tuple.element_types = NULL;
            
            expr->is_literal = 1;
            expr->is_collection = 1;
            expr->collection = coll;
            expr->is_float = 0;
            expr->is_string = 0;
            expr->is_boolean = 0;
            
            STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
            sto_info->type = INTERNAL_TYPE_TUPLE;
            sto_info->is_constant = false;
            sto_info->needs_promotion = false;
            sto_info->mem_location = MEM_HEAP;
            expr->sto_info = sto_info;
            expr->sto_analyzed = true;
            expr->needs_overflow_check = false;
            
            return expr;
        }
        
        // Parse elements (comma-separated)
        int capacity = 4;
        int length = 0;
        ArithmeticExpr** elements = malloc(sizeof(ArithmeticExpr*) * capacity);
        VarType* types = malloc(sizeof(VarType) * capacity);
        
        // Parse first element
        ArithmeticExpr* first_elem = parse_bitwise_stateless(lexer, current);
        if (!first_elem) {
            fprintf(stderr, "Error: Failed to parse tuple element\n");
            free(elements);
            free(types);
            free(expr);
            return NULL;
        }
        elements[0] = first_elem;
        types[0] = first_elem->is_string ? VAR_STRING : VAR_NUMERIC;
        length = 1;
        
        // Parse remaining elements (semicolon separated - PMPL standard)
        while (*current && 
               (*current)->type == TOKEN_SEMICOLON) {
            advance_stateless(lexer, current);  // Skip ';'
            
            // Check capacity
            if (length >= capacity) {
                capacity *= 2;
                elements = realloc(elements, sizeof(ArithmeticExpr*) * capacity);
                types = realloc(types, sizeof(VarType) * capacity);
            }
            
            // Parse next element
            ArithmeticExpr* elem = parse_bitwise_stateless(lexer, current);
            if (!elem) {
                fprintf(stderr, "Error: Failed to parse tuple element\n");
                for (int i = 0; i < length; i++) {
                    arithmetic_expr_free(elements[i]);
                }
                free(elements);
                free(types);
                free(expr);
                return NULL;
            }
            
            elements[length] = elem;
            types[length] = elem->is_string ? VAR_STRING : VAR_NUMERIC;
            length++;
        }
        
        // Expect '>'
        if (!*current || (*current)->type != TOKEN_GREATER) {
            fprintf(stderr, "Error: Expected '>' after tuple elements\n");
            for (int i = 0; i < length; i++) {
                arithmetic_expr_free(elements[i]);
            }
            free(elements);
            free(types);
            free(expr);
            return NULL;
        }
        advance_stateless(lexer, current);  // Skip '>'
        
        // Build tuple collection
        Collection* coll = malloc(sizeof(Collection));
        coll->type = COLL_TUPLE;
        coll->data.tuple.length = length;
        coll->data.tuple.elements = (void**)elements;
        coll->data.tuple.element_types = types;
        
        expr->is_literal = 1;
        expr->is_collection = 1;
        expr->collection = coll;
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 0;
        
        STOTypeInfo* sto_info = malloc(sizeof(STOTypeInfo));
        sto_info->type = INTERNAL_TYPE_TUPLE;
        sto_info->is_constant = false;
        sto_info->needs_promotion = false;
        sto_info->mem_location = MEM_HEAP;
        expr->sto_info = sto_info;
        expr->sto_analyzed = true;
        expr->needs_overflow_check = false;
        
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = ARITH_POW;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, ARITH_POW);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_string = (left->is_string || right->is_string);  // YZ_10: String propagation
        binary->is_boolean = 0;
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
        memset(binary, 0, sizeof(ArithmeticExpr));  // YZ_44: Initialize all fields to zero
        binary->op = op;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
        binary->sto_info = NULL;
        binary->sto_analyzed = false;
        binary->needs_overflow_check = false;
        
        if (left->sto_analyzed && right->sto_analyzed && left->sto_info && right->sto_info) {
            STOTypeInfo* propagated = malloc(sizeof(STOTypeInfo));
            *propagated = arithmetic_propagate_binary_types(left->sto_info, right->sto_info, op);
            binary->sto_info = propagated;
            binary->sto_analyzed = true;
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
    
    // ✅ YZ_32: Apply constant folding optimization
    if (result) {
        result = arithmetic_optimize_constant_fold(result);
    }
    
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
