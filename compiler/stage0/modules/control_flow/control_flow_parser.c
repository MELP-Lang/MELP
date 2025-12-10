#include "control_flow_parser.h"
#include "../comparison/comparison_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// NEW STATELESS TEMPLATE FUNCTIONS
// ============================================================================

// Parse if statement (stateless template)
IfStatement* control_flow_parse_if(Lexer* lexer, Token* if_token) {
    // Caller owns if_token - we don't free it!
    if (!lexer || !if_token || if_token->type != TOKEN_IF) {
        return NULL;
    }
    
    IfStatement* stmt = malloc(sizeof(IfStatement));
    stmt->condition = NULL;
    stmt->then_body = NULL;
    stmt->else_body = NULL;
    stmt->has_else = 0;
    
    // Read next token for condition
    Token* tok = lexer_next_token(lexer);
    
    // Parse condition - comparison also uses borrowing pattern
    // We pass lexer and current token, comparison reads what it needs
    stmt->condition = comparison_parse_expression_stateless(lexer, tok);
    
    // comparison_parse_expression consumed tok and any other tokens it needed
    // We don't need to free tok - comparison module handled it
    
    if (!stmt->condition) {
        free(stmt);
        return NULL;
    }
    
    // Read 'then' keyword
    tok = lexer_next_token(lexer);
    if (!tok || tok->type != TOKEN_THEN) {
        fprintf(stderr, "Error: Expected 'then' after if condition\n");
        // Free what we own
        comparison_expr_free(stmt->condition);
        free(stmt);
        if (tok) token_free(tok);
        return NULL;
    }
    token_free(tok);  // We read it, we free it
    
    // Don't parse body - statement_parser will handle recursively
    return stmt;
}

// Parse while statement (stateless template)
WhileStatement* control_flow_parse_while(Lexer* lexer, Token* while_token) {
    // Caller owns while_token - we don't free it!
    if (!lexer || !while_token || while_token->type != TOKEN_WHILE) {
        return NULL;
    }
    
    WhileStatement* stmt = malloc(sizeof(WhileStatement));
    stmt->condition = NULL;
    stmt->body = NULL;
    
    // Read next token for condition
    Token* tok = lexer_next_token(lexer);
    
    // Parse condition - borrowing pattern
    stmt->condition = comparison_parse_expression_stateless(lexer, tok);
    
    if (!stmt->condition) {
        free(stmt);
        return NULL;
    }
    
    // ✅ Expect 'do' keyword after condition
    tok = lexer_next_token(lexer);
    if (!tok || tok->type != TOKEN_DO) {
        fprintf(stderr, "[Parser] error: Expected 'do' after while condition\n");
        free(stmt->condition);
        free(stmt);
        if (tok) token_free(tok);
        return NULL;
    }
    token_free(tok);
    
    // Don't parse body - statement_parser will handle recursively
    return stmt;
}

// Parse expression-based if (stateless template)
IfStatement* control_flow_parse_if_expr(Lexer* lexer, Token* current_token) {
    // Parse: result = if condition then value1 else value2
    Token* tok = current_token;
    
    IfStatement* stmt = malloc(sizeof(IfStatement));
    stmt->condition = comparison_parse_expression_stateless(lexer, tok);
    stmt->has_else = 1;  // Expression-based if requires else
    stmt->type = CTRL_IF_EXPR;
    
    return stmt;
}

// Parse for statement (stateless template - future)
ForStatement* control_flow_parse_for(Lexer* lexer, Token* for_token) {
    // TODO: Implement for loop parsing
    (void)lexer;
    (void)for_token;
    return NULL;
}
