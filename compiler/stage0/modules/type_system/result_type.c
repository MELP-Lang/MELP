#include "result_type.h"
#include "../error/error.h"
#include "../expression/expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Result Type Implementation - YZ_206
 * 
 * Phase 1: Core result type structure and parsing
 */

// Create result type
Type* result_type_create(Type* value_type, Type* error_type) {
    if (!value_type || !error_type) {
        return NULL;
    }

    Type* type = type_create(TYPE_STRUCT);  // Result is a tagged union (struct)
    if (!type) {
        return NULL;
    }

    // Allocate ResultType metadata
    ResultType* rt = (ResultType*)malloc(sizeof(ResultType));
    if (!rt) {
        type_free(type);
        return NULL;
    }

    rt->value_type = value_type;
    rt->error_type = error_type;
    rt->is_generic = (value_type->kind == TYPE_GENERIC || error_type->kind == TYPE_GENERIC);

    // Store ResultType in the type's name field (we'll use a custom pointer)
    // Format: "result<value_type, error_type>"
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "result<%s, %s>",
             type_to_string(value_type),
             type_to_string(error_type));
    type->name = strdup(buffer);

    // Store metadata pointer (we'll use member_types[0] as a hack for now)
    type->member_count = 2;
    type->member_types = (Type**)malloc(sizeof(Type*) * 2);
    type->member_types[0] = value_type;  // T
    type->member_types[1] = error_type;  // E

    type->member_names = (char**)malloc(sizeof(char*) * 2);
    type->member_names[0] = strdup("variant");  // i8: 0=ok, 1=error
    type->member_names[1] = strdup("data");     // union of T and E

    return type;
}

// Check if type is Result<T, E>
bool is_result_type(Type* type) {
    if (!type || type->kind != TYPE_STRUCT) {
        return false;
    }
    // Check if name starts with "result<"
    return type->name && strncmp(type->name, "result<", 7) == 0;
}

// Get result value type T
Type* result_get_value_type(Type* result) {
    if (!is_result_type(result) || result->member_count < 2) {
        return NULL;
    }
    return result->member_types[0];
}

// Get result error type E
Type* result_get_error_type(Type* result) {
    if (!is_result_type(result) || result->member_count < 2) {
        return NULL;
    }
    return result->member_types[1];
}

// Parse result type declaration
// Example: result<numeric, string>
ResultType* parse_result_type(Token** tokens, int* index) {
    if (!tokens || !index) {
        return NULL;
    }

    Token* current = tokens[*index];
    
    // Expect: result
    if (current->type != TOKEN_IDENTIFIER || strcmp(current->value, "result") != 0) {
        error_parser(current->line, "Expected 'result' keyword");
        return NULL;
    }
    (*index)++;

    // Expect: <
    current = tokens[*index];
    if (current->type != TOKEN_LESS) {
        error_parser(current->line, "Expected '<' after 'result'");
        return NULL;
    }
    (*index)++;

    // Parse value type T
    // For now, we'll use simple type names (numeric, string, etc.)
    current = tokens[*index];
    if (current->type != TOKEN_IDENTIFIER) {
        error_parser(current->line, "Expected type name for result value type");
        return NULL;
    }
    
    Type* value_type = type_create(TYPE_UNKNOWN);
    value_type->name = strdup(current->value);
    
    // Map MELP types
    if (strcmp(current->value, "numeric") == 0) {
        value_type->kind = TYPE_INT;
    } else if (strcmp(current->value, "string") == 0) {
        value_type->kind = TYPE_STRING;
    } else if (strcmp(current->value, "boolean") == 0) {
        value_type->kind = TYPE_BOOL;
    } else {
        value_type->kind = TYPE_GENERIC;  // Assume generic for now
    }
    (*index)++;

    // Expect: ,
    current = tokens[*index];
    if (current->type != TOKEN_COMMA) {
        error_parser(current->line, "Expected ',' after result value type");
        type_free(value_type);
        return NULL;
    }
    (*index)++;

    // Parse error type E
    current = tokens[*index];
    if (current->type != TOKEN_IDENTIFIER) {
        error_parser(current->line, "Expected type name for result error type");
        type_free(value_type);
        return NULL;
    }
    
    Type* error_type = type_create(TYPE_UNKNOWN);
    error_type->name = strdup(current->value);
    
    // Map MELP types
    if (strcmp(current->value, "numeric") == 0) {
        error_type->kind = TYPE_INT;
    } else if (strcmp(current->value, "string") == 0) {
        error_type->kind = TYPE_STRING;
    } else if (strcmp(current->value, "boolean") == 0) {
        error_type->kind = TYPE_BOOL;
    } else {
        error_type->kind = TYPE_GENERIC;  // Assume generic for now
    }
    (*index)++;

    // Expect: >
    current = tokens[*index];
    if (current->type != TOKEN_GREATER) {
        error_parser(current->line, "Expected '>' after result error type");
        type_free(value_type);
        type_free(error_type);
        return NULL;
    }
    (*index)++;

    // Create ResultType
    ResultType* rt = (ResultType*)malloc(sizeof(ResultType));
    if (!rt) {
        type_free(value_type);
        type_free(error_type);
        return NULL;
    }

    rt->value_type = value_type;
    rt->error_type = error_type;
    rt->is_generic = (value_type->kind == TYPE_GENERIC || error_type->kind == TYPE_GENERIC);

    return rt;
}

// Type checking
bool result_type_check_ok(Type* result_type, Type* value_type) {
    if (!is_result_type(result_type)) {
        return false;
    }
    Type* expected = result_get_value_type(result_type);
    return type_equals(expected, value_type);
}

bool result_type_check_error(Type* result_type, Type* error_type) {
    if (!is_result_type(result_type)) {
        return false;
    }
    Type* expected = result_get_error_type(result_type);
    return type_equals(expected, error_type);
}

// Utility
char* result_type_to_string(ResultType* rt) {
    if (!rt) {
        return strdup("result<?, ?>");
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "result<%s, %s>",
             type_to_string(rt->value_type),
             type_to_string(rt->error_type));
    return strdup(buffer);
}

void result_type_free(ResultType* rt) {
    if (!rt) {
        return;
    }
    // Note: Don't free value_type/error_type here, they're managed by type system
    free(rt);
}

void result_match_free(ResultMatch* match) {
    if (!match) {
        return;
    }
    
    // Free result expression
    if (match->result_expr) {
        expression_free((Expression*)match->result_expr);
    }
    
    // Free binding names
    if (match->has_ok_case && match->ok_case.binding_name) {
        free(match->ok_case.binding_name);
    }
    if (match->has_error_case && match->error_case.binding_name) {
        free(match->error_case.binding_name);
    }
    
    // Note: Don't free body statements here - they're managed by statement system
    
    free(match);
}

// Parse ok() constructor
// Example: return ok(42)
void* parse_ok_constructor(Token** tokens, int* index, Type* result_type) {
    if (!tokens || !index || !result_type) {
        return NULL;
    }

    Token* current = tokens[*index];

    // Expect: ok
    if (current->type != TOKEN_IDENTIFIER || strcmp(current->value, "ok") != 0) {
        error_parser(current->line, "Expected 'ok' keyword");
        return NULL;
    }
    (*index)++;

    // Expect: (
    current = tokens[*index];
    if (current->type != TOKEN_LPAREN) {
        error_parser(current->line, "Expected '(' after 'ok'");
        return NULL;
    }
    (*index)++;

    // Parse value expression
    // For now, we'll just parse a simple literal or variable
    // TODO: Full expression parsing integration
    current = tokens[*index];
    
    Expression* value_expr = NULL;
    
    if (current->type == TOKEN_NUMBER) {
        value_expr = expression_create(EXPR_NUMBER);
        value_expr->data.number_value = atol(current->value);
        (*index)++;
    } else if (current->type == TOKEN_STRING) {
        value_expr = expression_create(EXPR_STRING);
        value_expr->data.string_value = strdup(current->value);
        (*index)++;
    } else if (current->type == TOKEN_IDENTIFIER) {
        value_expr = expression_create(EXPR_VARIABLE);
        value_expr->data.var_name = strdup(current->value);
        (*index)++;
    } else {
        error_parser(current->line, "Expected value expression in ok()");
        return NULL;
    }

    // Expect: )
    current = tokens[*index];
    if (current->type != TOKEN_RPAREN) {
        error_parser(current->line, "Expected ')' after ok value");
        expression_free(value_expr);
        return NULL;
    }
    (*index)++;

    // Create ok constructor expression
    Expression* ok_expr = expression_create(EXPR_RESULT_OK);
    ok_expr->data.result_constructor.value = value_expr;
    ok_expr->data.result_constructor.result_type = result_type;

    return ok_expr;
}

void* parse_error_constructor(Token** tokens, int* index, Type* result_type) {
    if (!tokens || !index || !result_type) {
        return NULL;
    }

    Token* current = tokens[*index];

    // Expect: error
    if (current->type != TOKEN_IDENTIFIER || strcmp(current->value, "error") != 0) {
        error_parser(current->line, "Expected 'error' keyword");
        return NULL;
    }
    (*index)++;

    // Expect: (
    current = tokens[*index];
    if (current->type != TOKEN_LPAREN) {
        error_parser(current->line, "Expected '(' after 'error'");
        return NULL;
    }
    (*index)++;

    // Parse error expression (usually string message)
    current = tokens[*index];
    
    Expression* error_expr = NULL;
    
    if (current->type == TOKEN_NUMBER) {
        error_expr = expression_create(EXPR_NUMBER);
        error_expr->data.number_value = atol(current->value);
        (*index)++;
    } else if (current->type == TOKEN_STRING) {
        error_expr = expression_create(EXPR_STRING);
        error_expr->data.string_value = strdup(current->value);
        (*index)++;
    } else if (current->type == TOKEN_IDENTIFIER) {
        error_expr = expression_create(EXPR_VARIABLE);
        error_expr->data.var_name = strdup(current->value);
        (*index)++;
    } else {
        error_parser(current->line, "Expected error expression in error()");
        return NULL;
    }

    // Expect: )
    current = tokens[*index];
    if (current->type != TOKEN_RPAREN) {
        error_parser(current->line, "Expected ')' after error value");
        expression_free(error_expr);
        return NULL;
    }
    (*index)++;

    // Create error constructor expression
    Expression* err_expr = expression_create(EXPR_RESULT_ERROR);
    err_expr->data.result_constructor.value = error_expr;
    err_expr->data.result_constructor.result_type = result_type;

    return err_expr;
}

ResultMatch* parse_result_match(Token** tokens, int* index) {
    if (!tokens || !index) {
        return NULL;
    }

    Token* current = tokens[*index];

    // Expect: match
    if (current->type != TOKEN_IDENTIFIER || strcmp(current->value, "match") != 0) {
        error_parser(current->line, "Expected 'match' keyword");
        return NULL;
    }
    (*index)++;

    // Parse result expression (variable or function call that returns result)
    current = tokens[*index];
    void* result_expr = NULL;
    
    if (current->type == TOKEN_IDENTIFIER) {
        // For now, just create a variable expression
        Expression* var_expr = expression_create(EXPR_VARIABLE);
        var_expr->data.var_name = strdup(current->value);
        result_expr = var_expr;
        (*index)++;
    } else {
        error_parser(current->line, "Expected result expression in match");
        return NULL;
    }

    // Create match structure
    ResultMatch* match = (ResultMatch*)malloc(sizeof(ResultMatch));
    if (!match) {
        return NULL;
    }
    memset(match, 0, sizeof(ResultMatch));
    match->result_expr = result_expr;

    // Parse cases (ok and error)
    // We expect at least one case
    bool parsing_cases = true;
    
    while (parsing_cases) {
        current = tokens[*index];

        // Check for end_match
        if (current->type == TOKEN_IDENTIFIER && strcmp(current->value, "end_match") == 0) {
            (*index)++;
            parsing_cases = false;
            break;
        }

        // Expect: case
        if (current->type != TOKEN_IDENTIFIER || strcmp(current->value, "case") != 0) {
            error_parser(current->line, "Expected 'case' or 'end_match'");
            free(match);
            return NULL;
        }
        (*index)++;

        // Expect: ok or error
        current = tokens[*index];
        if (current->type != TOKEN_IDENTIFIER) {
            error_parser(current->line, "Expected 'ok' or 'error' after 'case'");
            free(match);
            return NULL;
        }

        bool is_ok_case = (strcmp(current->value, "ok") == 0);
        bool is_error_case = (strcmp(current->value, "error") == 0);

        if (!is_ok_case && !is_error_case) {
            error_parser(current->line, "Expected 'ok' or 'error' in match case");
            free(match);
            return NULL;
        }
        (*index)++;

        // Expect: (
        current = tokens[*index];
        if (current->type != TOKEN_LPAREN) {
            error_parser(current->line, "Expected '(' after ok/error");
            free(match);
            return NULL;
        }
        (*index)++;

        // Parse binding name (e.g., "value" in "ok(value)")
        current = tokens[*index];
        if (current->type != TOKEN_IDENTIFIER) {
            error_parser(current->line, "Expected binding variable name");
            free(match);
            return NULL;
        }
        char* binding_name = strdup(current->value);
        (*index)++;

        // Expect: )
        current = tokens[*index];
        if (current->type != TOKEN_RPAREN) {
            error_parser(current->line, "Expected ')' after binding name");
            free(binding_name);
            free(match);
            return NULL;
        }
        (*index)++;

        // Expect: : (colon)
        current = tokens[*index];
        if (current->type != TOKEN_COLON) {
            error_parser(current->line, "Expected ':' after case pattern");
            free(binding_name);
            free(match);
            return NULL;
        }
        (*index)++;

        // Parse case body (single statement for now)
        // TODO: Parse full statement block
        // For now, just skip until we hit "case" or "end_match"
        // This is a placeholder - we need proper statement parsing integration
        
        if (is_ok_case) {
            match->has_ok_case = true;
            match->ok_case.variant = RESULT_OK;
            match->ok_case.binding_name = binding_name;
            match->ok_case.body = NULL;  // TODO: Parse statement
        } else {
            match->has_error_case = true;
            match->error_case.variant = RESULT_ERROR;
            match->error_case.binding_name = binding_name;
            match->error_case.body = NULL;  // TODO: Parse statement
        }
    }

    // Validate: must have at least one case
    if (!match->has_ok_case && !match->has_error_case) {
        error_parser(tokens[*index - 1]->line, "Match must have at least one case");
        free(match);
        return NULL;
    }

    return match;
}

void* parse_result_propagation(Token** tokens, int* index) {
    // TODO: Phase 4 - ? operator
    error_parser(tokens[*index]->line, "? operator not yet implemented");
    return NULL;
}
