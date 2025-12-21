#include "throw_parser.h"
#include "../expression/expression.h"
#include <stdio.h>
#include <stdlib.h>

// Forward declaration
extern Expression* parse_expression(Token** tokens, int* pos);

// ============================================================================
// Throw Statement Parser
// ============================================================================

ThrowStatement* parse_throw_statement(Token** tokens, int* pos) {
    // Expect: throw
    if (tokens[*pos]->type != TOKEN_THROW) {
        fprintf(stderr, "Error: Expected 'throw' keyword at line %d\n", 
                tokens[*pos]->line);
        return NULL;
    }
    (*pos)++; // Skip 'throw'
    
    // Parse exception expression
    Expression* exception_expr = parse_expression(tokens, pos);
    if (!exception_expr) {
        fprintf(stderr, "Error: Expected exception expression after 'throw'\n");
        return NULL;
    }
    
    // Create throw statement
    ThrowStatement* throw_stmt = throw_statement_create(exception_expr);
    if (!throw_stmt) {
        // Note: Expression freed by expression manager
        return NULL;
    }
    
    return throw_stmt;
}
