#include "control_flow_parser.h"
#include "../comparison/comparison_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void advance(ControlFlowParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    parser->current_token = lexer_next_token(parser->lexer);
}

// Helper: Parse body statements until end keyword
static BodyStatement* parse_body_until(ControlFlowParser* parser, TokenType end_token) {
    BodyStatement* head = NULL;
    BodyStatement* tail = NULL;
    
    while (parser->current_token && parser->current_token->type != TOKEN_EOF) {
        // Stop at end markers
        if (parser->current_token->type == end_token ||
            parser->current_token->type == TOKEN_END ||
            parser->current_token->type == TOKEN_ELSE) {
            break;
        }
        
        BodyStatement* stmt = malloc(sizeof(BodyStatement));
        stmt->next = NULL;
        stmt->code = NULL;
        
        // Parse statement based on token type
        if (parser->current_token->type == TOKEN_PRINT) {
            // Print statement: print("message")
            advance(parser);  // Skip 'print'
            
            if (parser->current_token && parser->current_token->type == TOKEN_LPAREN) {
                advance(parser);  // Skip '('
            }
            
            if (parser->current_token && parser->current_token->type == TOKEN_STRING) {
                char code[512];
                snprintf(code, sizeof(code), "        printf(\"%s\\n\");", parser->current_token->value);
                stmt->code = strdup(code);
                stmt->type = BODY_STMT_PRINT;
                advance(parser);  // Skip string
                
                if (parser->current_token && parser->current_token->type == TOKEN_RPAREN) {
                    advance(parser);  // Skip ')'
                }
            }
        }
        else if (parser->current_token->type == TOKEN_NUMERIC) {
            // Variable declaration: numeric x = 10
            advance(parser);  // Skip 'numeric'
            
            if (parser->current_token && parser->current_token->type == TOKEN_IDENTIFIER) {
                char var_name[128];
                strncpy(var_name, parser->current_token->value, sizeof(var_name) - 1);
                advance(parser);  // Skip var name
                
                if (parser->current_token && parser->current_token->type == TOKEN_ASSIGN) {
                    advance(parser);  // Skip '='
                    
                    if (parser->current_token && parser->current_token->type == TOKEN_NUMBER) {
                        char code[512];
                        snprintf(code, sizeof(code), "        int64_t %s = %s;", var_name, parser->current_token->value);
                        stmt->code = strdup(code);
                        stmt->type = BODY_STMT_VARIABLE;
                        advance(parser);  // Skip value
                    }
                }
            }
        }
        else if (parser->current_token->type == TOKEN_EXIT) {
            // Break statement
            stmt->code = strdup("        break;");
            stmt->type = BODY_STMT_BREAK;
            advance(parser);
        }
        else if (parser->current_token->type == TOKEN_CONTINUE) {
            // Continue statement
            stmt->code = strdup("        continue;");
            stmt->type = BODY_STMT_CONTINUE;
            advance(parser);
        }
        else {
            // Unknown statement - skip
            advance(parser);
            free(stmt);
            continue;
        }
        
        // Add to list
        if (stmt->code) {
            if (!head) {
                head = stmt;
                tail = stmt;
            } else {
                tail->next = stmt;
                tail = stmt;
            }
        } else {
            free(stmt);
        }
    }
    
    return head;
}

ControlFlowParser* control_flow_parser_create(Lexer* lexer, Token* initial_token) {
    ControlFlowParser* parser = malloc(sizeof(ControlFlowParser));
    parser->lexer = lexer;
    parser->current_token = initial_token;  // Use passed token instead of reading new one
    return parser;
}

void control_flow_parser_free(ControlFlowParser* parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    free(parser);
}

// Parse if statement
IfStatement* control_flow_parse_if(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_IF) {
        return NULL;
    }
    advance(parser);  // Skip 'if' - now lexer is at first operand position
    
    IfStatement* stmt = malloc(sizeof(IfStatement));
    stmt->condition = NULL;
    stmt->has_else = 0;
    stmt->then_body = NULL;
    stmt->else_body = NULL;
    
    // MANUAL COMPARISON PARSING (bypass comparison_parser to avoid token sync issues)
    ComparisonExpr* cond = malloc(sizeof(ComparisonExpr));
    cond->left = NULL;
    cond->right = NULL;
    cond->left_value = NULL;
    cond->right_value = NULL;
    cond->is_float = 0;
    cond->left_is_literal = 0;
    cond->right_is_literal = 0;
    
    // Get left operand (should be variable 'x')
    if (parser->current_token && parser->current_token->type == TOKEN_IDENTIFIER) {
        cond->left_value = strdup(parser->current_token->value);
        advance(parser);
    } else {
        free(cond);
        if_statement_free(stmt);
        return NULL;
    }
    
    // Get operator
    if (!parser->current_token) {
        free(cond->left_value);
        free(cond);
        if_statement_free(stmt);
        return NULL;
    }
    
    switch (parser->current_token->type) {
        case TOKEN_LESS: cond->op = CMP_LESS; break;
        case TOKEN_GREATER: cond->op = CMP_GREATER; break;
        case TOKEN_EQUAL: cond->op = CMP_EQUAL; break;
        case TOKEN_NOT_EQUAL: cond->op = CMP_NOT_EQUAL; break;
        case TOKEN_LESS_EQUAL: cond->op = CMP_LESS_EQUAL; break;
        case TOKEN_GREATER_EQUAL: cond->op = CMP_GREATER_EQUAL; break;
        default:
            fprintf(stderr, "Error: Expected comparison operator, got token type %d\\n", parser->current_token->type);
            free(cond->left_value);
            free(cond);
            if_statement_free(stmt);
            return NULL;
    }
    advance(parser);  // Skip operator
    
    // Get right operand (should be number '10')
    if (parser->current_token && parser->current_token->type == TOKEN_NUMBER) {
        cond->right_value = strdup(parser->current_token->value);
        cond->right_is_literal = 1;
        advance(parser);
    } else {
        free(cond->left_value);
        free(cond);
        if_statement_free(stmt);
        return NULL;
    }
    
    stmt->condition = cond;
    
    // Expect 'then'
    if (!parser->current_token || parser->current_token->type != TOKEN_THEN) {
        if_statement_free(stmt);
        return NULL;
    }
    advance(parser);
    
    // Parse then body (NEW!)
    stmt->then_body = parse_body_until(parser, TOKEN_ELSE);
    
    // Check for else
    if (parser->current_token && parser->current_token->type == TOKEN_ELSE) {
        stmt->has_else = 1;
        advance(parser);
        
        // Parse else body (NEW!)
        stmt->else_body = parse_body_until(parser, TOKEN_END);
    }
    
    // Skip 'end' keyword
    if (parser->current_token && parser->current_token->type == TOKEN_END) {
        advance(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_IF) {
            advance(parser);
        }
    }
    
    return stmt;
}

// Parse while statement
WhileStatement* control_flow_parse_while(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_WHILE) {
        return NULL;
    }
    advance(parser);
    
    WhileStatement* stmt = malloc(sizeof(WhileStatement));
    stmt->condition = NULL;
    stmt->body = NULL;
    
    // Parse condition
    ComparisonParser* cmp = comparison_parser_create(parser->lexer);
    stmt->condition = comparison_parse_expression(cmp);
    
    if (parser->current_token) token_free(parser->current_token);
    parser->current_token = cmp->current_token;
    cmp->current_token = NULL;
    comparison_parser_free(cmp);
    
    // Parse body (NEW!)
    stmt->body = parse_body_until(parser, TOKEN_END);
    
    // Skip 'end while'
    if (parser->current_token && parser->current_token->type == TOKEN_END) {
        advance(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_WHILE) {
            advance(parser);
        }
    }
    
    return stmt;
}

// Parse for statement
ForStatement* control_flow_parse_for(ControlFlowParser* parser) {
    if (!parser || !parser->current_token || parser->current_token->type != TOKEN_FOR) {
        return NULL;
    }
    advance(parser);
    
    ForStatement* stmt = malloc(sizeof(ForStatement));
    stmt->iterator = NULL;
    stmt->start = NULL;
    stmt->end = NULL;
    stmt->body = NULL;
    
    // Get iterator
    if (parser->current_token && parser->current_token->type == TOKEN_IDENTIFIER) {
        stmt->iterator = strdup(parser->current_token->value);
        advance(parser);
    }
    
    // Parse body (NEW!)
    stmt->body = parse_body_until(parser, TOKEN_END);
    
    // Skip 'end for'
    if (parser->current_token && parser->current_token->type == TOKEN_END) {
        advance(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_FOR) {
            advance(parser);
        }
    }
    
    return stmt;
}
