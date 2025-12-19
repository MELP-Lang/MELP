#include "switch_parser.h"
#include "../parser_core/parser_core.h"
#include "../statement/statement.h"
#include "../statement/statement_parser.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../error/error.h"
#include <stdio.h>
#include <stdlib.h>

// Parse switch statement
// Syntax:
//   switch <expression>
//       case <value>:
//           <statements>
//       case <value>:
//           <statements>
//       default:
//           <statements>
//   end_switch

SwitchStatement* switch_parse(Parser* parser, Token* switch_tok) {
    if (!parser || !switch_tok) {
        error_fatal("switch_parse - NULL parser or token");
        return NULL;
    }
    
    // Parse switch expression
    Token* expr_tok = lexer_next_token(parser->lexer);
    if (!expr_tok) {
        error_fatal("switch statement requires an expression");
        return NULL;
    }
    
    void* expression = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
    token_free(expr_tok);
    
    if (!expression) {
        error_fatal("switch statement requires an expression");
        return NULL;
    }
    
    // Create switch statement
    SwitchStatement* switch_stmt = switch_statement_create(expression);
    
    // Parse cases
    while (1) {
        Token* tok = lexer_next_token(parser->lexer);
        if (!tok) {
            error_fatal("Unexpected end of file in switch statement");
            switch_statement_free(switch_stmt);
            return NULL;
        }
        
        // Check for end_switch
        if (tok->type == TOKEN_END_SWITCH) {
            token_free(tok);
            break;
        }
        
        // Parse case or default
        if (tok->type == TOKEN_CASE) {
            // Parse case value
            Token* case_tok = lexer_next_token(parser->lexer);
            if (!case_tok) {
                error_fatal("case requires a value expression");
                token_free(tok);
                switch_statement_free(switch_stmt);
                return NULL;
            }
            
            void* case_value = arithmetic_parse_expression_stateless(parser->lexer, case_tok, NULL);
            token_free(case_tok);
            
            if (!case_value) {
                error_fatal("case requires a value expression");
                token_free(tok);
                switch_statement_free(switch_stmt);
                return NULL;
            }
            
            // Expect colon
            Token* colon = lexer_next_token(parser->lexer);
            if (!colon || colon->type != TOKEN_COLON) {
                error_fatal("Expected ':' after case value");
                if (colon) token_free(colon);
                token_free(tok);
                switch_statement_free(switch_stmt);
                return NULL;
            }
            token_free(colon);
            
            // Create case
            SwitchCase* case_item = switch_case_create(case_value, 0);
            
            // Parse case body (statements until next case/default/end_switch)
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Token* peek = lexer_next_token(parser->lexer);
                if (!peek) break;
                
                // Check for case terminator
                if (peek->type == TOKEN_CASE || 
                    peek->type == TOKEN_DEFAULT || 
                    peek->type == TOKEN_END_SWITCH) {
                    // Push back for next iteration
                    lexer_unget_token(parser->lexer, peek);
                    break;
                }
                
                // Push back and parse statement
                lexer_unget_token(parser->lexer, peek);
                Statement* stmt = statement_parse(parser);
                if (stmt) {
                    if (!body_head) {
                        body_head = stmt;
                        body_tail = stmt;
                    } else {
                        body_tail->next = stmt;
                    }
                    // Move tail to end of chain
                    while (body_tail && body_tail->next) {
                        body_tail = body_tail->next;
                    }
                }
            }
            
            case_item->body = body_head;
            switch_statement_add_case(switch_stmt, case_item);
            token_free(tok);
            
        } else if (tok->type == TOKEN_DEFAULT) {
            // Expect colon
            Token* colon = lexer_next_token(parser->lexer);
            if (!colon || colon->type != TOKEN_COLON) {
                error_fatal("Expected ':' after default");
                if (colon) token_free(colon);
                token_free(tok);
                switch_statement_free(switch_stmt);
                return NULL;
            }
            token_free(colon);
            
            // Create default case
            SwitchCase* default_case = switch_case_create(NULL, 1);
            
            // Parse default body
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Token* peek = lexer_next_token(parser->lexer);
                if (!peek) break;
                
                // Check for case terminator
                if (peek->type == TOKEN_CASE || 
                    peek->type == TOKEN_DEFAULT || 
                    peek->type == TOKEN_END_SWITCH) {
                    // Push back for next iteration
                    lexer_unget_token(parser->lexer, peek);
                    break;
                }
                
                // Push back and parse statement
                lexer_unget_token(parser->lexer, peek);
                Statement* stmt = statement_parse(parser);
                if (stmt) {
                    if (!body_head) {
                        body_head = stmt;
                        body_tail = stmt;
                    } else {
                        body_tail->next = stmt;
                    }
                    // Move tail to end of chain
                    while (body_tail && body_tail->next) {
                        body_tail = body_tail->next;
                    }
                }
            }
            
            default_case->body = body_head;
            switch_statement_add_case(switch_stmt, default_case);
            token_free(tok);
            
        } else {
            error_fatal("Expected 'case', 'default', or 'end_switch' in switch statement");
            token_free(tok);
            switch_statement_free(switch_stmt);
            return NULL;
        }
    }
    
    return switch_stmt;
}
