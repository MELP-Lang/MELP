#include "arithmetic_parser.h"
#include "arithmetic_optimize.h"  // YZ_32: Constant folding
#include "../codegen_context/codegen_context.h"
#include "../array/array_parser.h"  // YZ_14: For array index access
#include "../functions/functions.h"  // YZ_36: For function_is_known()
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
    
    // Boolean literal (true/false)
    if (parser->current_token->type == TOKEN_TRUE || parser->current_token->type == TOKEN_FALSE) {
        expr->is_literal = 1;
        expr->value = strdup(parser->current_token->value);  // "true" or "false"
        expr->is_float = 0;
        expr->is_string = 0;
        expr->is_boolean = 1;
        
        // TTO analysis for boolean literal
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_INT64;  // Boolean stored as int (0/1)
        tto->is_constant = true;  // Boolean literals are constant
        tto->needs_promotion = false;
        tto->mem_location = MEM_REGISTER;  // Small value, keep in register
        expr->tto_info = tto;
        expr->tto_analyzed = true;
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
        
        // TTO analysis for string literal
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_SSO_STRING;  // Small String Optimization
        tto->is_constant = true;  // String literals are constant
        tto->needs_promotion = false;
        tto->mem_location = MEM_RODATA;  // String literals in .rodata
        expr->tto_info = tto;
        expr->tto_analyzed = true;
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
        
        // Phase 2.3: TTO analysis for numeric literal
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        *tto = codegen_tto_infer_numeric_type(expr->value);
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = (tto->type == INTERNAL_TYPE_INT64);
        
        advance(parser);
        return expr;
    }
    
    // Variable or Function Call
    if (parser->current_token->type == TOKEN_IDENTIFIER) {
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
                
                // Parse remaining arguments (comma-separated)
                while (parser->current_token && parser->current_token->type == TOKEN_COMMA) {
                    advance(parser);  // consume ','
                    
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
            
            // TTO info: assume INT64 return value
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_INT64;
            tto->is_constant = false;
            tto->needs_promotion = true;
            tto->mem_location = MEM_REGISTER;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
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
        binary->is_string = 0;
        binary->is_boolean = 0;
        
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
        binary->is_boolean = 0;
        
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
        binary->is_boolean = 0;
        
        // String type propagation: If either operand is string, result is string
        // This enables: text + text (concat), text + numeric (would need runtime conversion)
        binary->is_string = (left->is_string || right->is_string);
        
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
        binary->is_boolean = 0;
        
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
    
    // YZ_18: Handle NOT operator (unary boolean NOT)
    if ((*current)->type == TOKEN_NOT) {
        advance_stateless(lexer, current);  // consume 'not'
        
        // Parse the operand
        ArithmeticExpr* operand = parse_primary_stateless(lexer, current);
        if (!operand) return NULL;
        
        // Create XOR with 1: not x = x xor 1
        ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
        expr->op = ARITH_XOR;
        expr->left = operand;
        expr->right = malloc(sizeof(ArithmeticExpr));
        expr->right->is_literal = 1;
        expr->right->value = strdup("1");
        expr->right->is_float = 0;
        expr->right->is_string = 0;
        expr->right->is_boolean = 0;  // Treat as numeric 1, not boolean
        expr->right->left = NULL;
        expr->right->right = NULL;
        expr->right->tto_info = NULL;
        expr->right->tto_analyzed = false;
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
        expr->tto_info = NULL;
        expr->tto_analyzed = false;
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
        expr->op = ARITH_SUB;
        expr->left = malloc(sizeof(ArithmeticExpr));
        expr->left->is_literal = 1;
        expr->left->value = strdup("0");
        expr->left->is_float = 0;
        expr->left->is_string = 0;
        expr->left->is_boolean = 0;
        expr->left->left = NULL;
        expr->left->right = NULL;
        expr->left->tto_info = NULL;
        expr->left->tto_analyzed = false;
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
        expr->tto_info = NULL;
        expr->tto_analyzed = false;
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
    expr->left = NULL;
    expr->right = NULL;
    expr->value = NULL;
    expr->tto_info = NULL;
    expr->tto_analyzed = false;
    expr->needs_overflow_check = false;
    expr->is_function_call = 0;
    expr->func_call = NULL;
    expr->is_array_access = 0;
    expr->array_access = NULL;
    expr->is_collection = 0;
    expr->collection = NULL;
    
    // Number literal
    if ((*current)->type == TOKEN_NUMBER) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = (strchr(expr->value, '.') != NULL);
        expr->is_string = 0;  // YZ_10: Not a string
        expr->is_boolean = 0;
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        *tto = codegen_tto_infer_numeric_type(expr->value);
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = (tto->type == INTERNAL_TYPE_INT64);
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // YZ_10: String literal
    if ((*current)->type == TOKEN_STRING) {
        expr->is_literal = 1;
        expr->value = strdup((*current)->value);
        expr->is_float = 0;
        expr->is_string = 1;  // YZ_10: This is a string!
        expr->is_boolean = 0;
        
        // TTO analysis for string literal
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_SSO_STRING;  // Small String Optimization
        tto->is_constant = true;  // String literals are constant
        tto->needs_promotion = false;
        tto->mem_location = MEM_RODATA;  // String literals in .rodata
        expr->tto_info = tto;
        expr->tto_analyzed = true;
        expr->needs_overflow_check = false;
        
        advance_stateless(lexer, current);
        return expr;
    }
    
    // Variable or Function Call or Collection Access
    if ((*current)->type == TOKEN_IDENTIFIER) {
        char* identifier = strdup((*current)->value);
        advance_stateless(lexer, current);
        
        // YZ_23: PRIORITY ORDER (most specific first):
        // 1. Collection access: arr[i], list(i), tuple<i>
        // 2. Function call: func(args...)
        // 3. Variable: x
        
        // YZ_29: For TOKEN_LPAREN, we need to distinguish between:
        //   - Function call: toUpperCase(msg), println(x), add(10, 20)
        //   - List access: mylist(0)
        // YZ_36: Solution: Check if identifier is a known function (builtin or user-defined)
        int is_known_func = function_is_known(identifier);
        
        // YZ_23: Check for array/list/tuple access FIRST (but NOT for known functions!)
        if (*current && ((*current)->type == TOKEN_LBRACKET || 
                        ((*current)->type == TOKEN_LPAREN && !is_known_func) ||
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
            
            // TTO info: result is INT64 (collection element)
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_INT64;
            tto->is_constant = false;
            tto->needs_promotion = true;
            tto->mem_location = MEM_REGISTER;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
            expr->needs_overflow_check = true;
            
            // array_parse_index_access consumed tokens, update current
            advance_stateless(lexer, current);
            
            free(identifier);  // IndexAccess has its own copy
            return expr;
        }
        
        // Phase 3.5: Check for function call
        // YZ_29: Now this code is reachable for builtin functions!
        // YZ_36: And also for user-defined functions!
        if (*current && (*current)->type == TOKEN_LPAREN && is_known_func) {
            // It's a function call: identifier(args...)
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
                
                // Parse remaining arguments (comma-separated)
                while (*current && (*current)->type == TOKEN_COMMA) {
                    advance_stateless(lexer, current);  // consume ','
                    
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
            
            // TTO info: assume INT64 return value
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_INT64;
            tto->is_constant = false;
            tto->needs_promotion = true;
            tto->mem_location = MEM_REGISTER;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
            expr->needs_overflow_check = true;
            
            return expr;
        }
        
        // It's a variable
        expr->is_literal = 0;
        expr->value = identifier;
        expr->is_float = 0;
        expr->is_string = 0;  // YZ_10: TODO - Infer from variable type in symbol table
        expr->is_boolean = 0;
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_INT64;
        tto->is_constant = false;
        tto->needs_promotion = true;
        tto->mem_location = MEM_REGISTER;
        expr->tto_info = tto;
        expr->tto_analyzed = true;
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
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_ARRAY;
        tto->is_constant = false;
        tto->needs_promotion = false;
        tto->mem_location = MEM_HEAP;
        expr->tto_info = tto;
        expr->tto_analyzed = true;
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
            
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_LIST;
            tto->is_constant = false;
            tto->needs_promotion = false;
            tto->mem_location = MEM_HEAP;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
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
            
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_LIST;
            tto->is_constant = false;
            tto->needs_promotion = false;
            tto->mem_location = MEM_HEAP;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
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
            
            TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
            tto->type = INTERNAL_TYPE_TUPLE;
            tto->is_constant = false;
            tto->needs_promotion = false;
            tto->mem_location = MEM_HEAP;
            expr->tto_info = tto;
            expr->tto_analyzed = true;
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
        
        // Parse remaining elements
        while (*current && (*current)->type == TOKEN_COMMA) {
            advance_stateless(lexer, current);  // Skip ','
            
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
        
        TTOTypeInfo* tto = malloc(sizeof(TTOTypeInfo));
        tto->type = INTERNAL_TYPE_TUPLE;
        tto->is_constant = false;
        tto->needs_promotion = false;
        tto->mem_location = MEM_HEAP;
        expr->tto_info = tto;
        expr->tto_analyzed = true;
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
        binary->op = ARITH_POW;
        binary->left = left;
        binary->right = right;
        binary->is_literal = 0;
        binary->value = NULL;
        binary->is_float = 0;
        binary->is_boolean = 0;
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
        binary->is_boolean = 0;
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
        binary->is_string = (left->is_string || right->is_string);  // YZ_10: String propagation
        binary->is_boolean = 0;
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
        binary->is_boolean = 0;
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
