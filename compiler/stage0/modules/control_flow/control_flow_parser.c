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

// ============================================================================
// OLD STATEFUL PARSER API (DEPRECATED)
// ============================================================================

static void advance(ControlFlowParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    parser->current_token = lexer_next_token(parser->lexer);
}

ControlFlowParser* control_flow_parser_create(Lexer* lexer) {
    ControlFlowParser* parser = malloc(sizeof(ControlFlowParser));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
    return parser;
}

void control_flow_parser_free(ControlFlowParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    free(parser);
}

// OLD stateful parse_if - DEPRECATED
IfStatement* control_flow_parse_if_old(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_IF) {
        return NULL;
    }
    advance(parser);
    
    IfStatement* stmt = malloc(sizeof(IfStatement));
    stmt->condition = NULL;
    stmt->has_else = 0;
    
    // Parse condition using MANUAL comparison parser creation
    ComparisonParser* cmp = malloc(sizeof(ComparisonParser));
    cmp->lexer = parser->lexer;
    cmp->current_token = parser->current_token;  // Transfer token
    parser->current_token = NULL;  // Don't double-free
    
    stmt->condition = comparison_parse_expression(cmp);
    
    // Sync tokens
    if (parser->current_token) token_free(parser->current_token);
    parser->current_token = cmp->current_token;
    cmp->current_token = NULL;
    comparison_parser_free(cmp);
    
    if (!stmt->condition) {
        fprintf(stderr, "Error: parse_if - condition is NULL\n");
        if_statement_free(stmt);
        return NULL;
    }
    
    // Expect 'then'
    if (!parser->current_token || parser->current_token->type != TOKEN_THEN) {
        fprintf(stderr, "Error: parse_if - Expected 'then' but got '%s' type=%d\n",
                parser->current_token ? parser->current_token->value : "NULL",
                parser->current_token ? parser->current_token->type : -1);
        if_statement_free(stmt);
        return NULL;
    }
    advance(parser);
    
    // ✅ DON'T skip body - let statement_parser handle it!
    // Just return the if statement with condition parsed
    // Statement parser will recursively parse then/else bodies
    
    return stmt;
}

// OLD stateful parse_while - DEPRECATED
WhileStatement* control_flow_parse_while_old(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_WHILE) {
        return NULL;
    }
    advance(parser);
    
    WhileStatement* stmt = malloc(sizeof(WhileStatement));
    stmt->condition = NULL;
    
    // Parse condition using MANUAL comparison parser creation
    ComparisonParser* cmp = malloc(sizeof(ComparisonParser));
    cmp->lexer = parser->lexer;
    cmp->current_token = parser->current_token;  // Transfer token
    parser->current_token = NULL;  // Don't double-free
    
    stmt->condition = comparison_parse_expression(cmp);
    
    if (parser->current_token) token_free(parser->current_token);
    parser->current_token = cmp->current_token;
    cmp->current_token = NULL;
    comparison_parser_free(cmp);
    
    // ✅ DON'T skip body - let statement_parser handle it!
    // Just return the while statement with condition parsed
    // Statement parser will recursively parse body
    
    return stmt;
}

// OLD stateful parse_for - DEPRECATED
ForStatement* control_flow_parse_for_old(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_FOR) {
        return NULL;
    }
    advance(parser);
    
    ForStatement* stmt = malloc(sizeof(ForStatement));
    stmt->iterator = NULL;
    stmt->start = NULL;
    stmt->end = NULL;
    stmt->label = NULL;  // Initialize label
    
    // Check for label: outer: for i in ...
    if (parser->current_token && parser->current_token->type == TOKEN_IDENTIFIER) {
        Token* peek = lexer_next_token(parser->lexer);
        if (peek && peek->type == TOKEN_COLON) {
            // This is a label
            stmt->label = strdup(parser->current_token->value);
            advance(parser);  // Consume label
            advance(parser);  // Consume colon
            
            // Now expect 'for' keyword again
            if (parser->current_token && parser->current_token->type == TOKEN_FOR) {
                advance(parser);
            }
        }
        if (peek) token_free(peek);
    }
    
    // Get iterator
    if (parser->current_token && parser->current_token->type == TOKEN_IDENTIFIER) {
        stmt->iterator = strdup(parser->current_token->value);
        advance(parser);
    }
    
    // Skip to 'end for'
    while (parser->current_token && parser->current_token->type != TOKEN_EOF) {
        if (parser->current_token->type == TOKEN_END) {
            advance(parser);
            if (parser->current_token && parser->current_token->type == TOKEN_FOR) {
                advance(parser);
            }
            break;
        }
        advance(parser);
    }
    
    return stmt;
}
