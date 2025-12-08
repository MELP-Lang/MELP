#include "statement_parser.h"
#include "../control_flow/control_flow_parser.h"  // ✅ While/If parsing
#include "../print/print_parser.h"                 // ✅ Print parsing
#include "../variable/variable_parser.h"           // ✅ Variable declarations
#include "../arithmetic/arithmetic_parser.h"       // ✅ Expressions
#include "../lexer/lexer.h"                        // ✅ Token operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Statement parsing with modular imports
// Each statement type delegates to its specific module

Statement* statement_parse(Parser* parser) {
    if (!parser || !parser->lexer) {
        return NULL;
    }
    
    Token* tok = lexer_next_token(parser->lexer);
    if (!tok) return NULL;
    
    Statement* stmt = NULL;
    
    // Check for end of block
    if (tok->type == TOKEN_END) {
        // Peek at next token to see what kind of end
        Token* next = lexer_next_token(parser->lexer);
        if (next && (next->type == TOKEN_WHILE || 
                     next->type == TOKEN_IF || 
                     next->type == TOKEN_FOR ||
                     next->type == TOKEN_FUNCTION)) {
            // End of block - don't consume these tokens
            // TODO: Need to put tokens back! For now, this is incomplete
            token_free(next);
            token_free(tok);
            return NULL;  // End of block
        }
        if (next) token_free(next);
        token_free(tok);
        return NULL;
    }
    
    // ✅ WHILE statement - use control_flow module
    if (tok->type == TOKEN_WHILE) {
        token_free(tok);
        
        // Create control flow parser
        ControlFlowParser* cfp = control_flow_parser_create(parser->lexer);
        WhileStatement* while_data = control_flow_parse_while(cfp);
        
        if (while_data) {
            stmt = statement_create(STMT_WHILE);
            stmt->data = while_data;
            stmt->next = NULL;
            
            // Parse body recursively
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Statement* body_stmt = statement_parse(parser);
                if (!body_stmt) break;
                
                if (!body_head) {
                    body_head = body_stmt;
                    body_tail = body_stmt;
                } else {
                    body_tail->next = body_stmt;
                    body_tail = body_stmt;
                }
            }
            
            // ✅ Store body in WhileStatement (now has body field!)
            while_data->body = body_head;
        }
        
        control_flow_parser_free(cfp);
        token_free(tok);
        return stmt;
    }
    
    // ✅ IF statement - use control_flow module
    if (tok->type == TOKEN_IF) {
        token_free(tok);
        
        ControlFlowParser* cfp = control_flow_parser_create(parser->lexer);
        IfStatement* if_data = control_flow_parse_if(cfp);
        
        if (if_data) {
            stmt = statement_create(STMT_IF);
            stmt->data = if_data;
            stmt->next = NULL;
            
            // Parse then body
            // TODO: Similar to while body parsing
        }
        
        control_flow_parser_free(cfp);
        return stmt;
    }
    
    // ✅ PRINT statement - use print module
    if (tok->type == TOKEN_PRINT) {
        token_free(tok);
        
        // TODO: Use print_parse() from print module
        stmt = statement_create(STMT_PRINT);
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ RETURN statement
    if (tok->type == TOKEN_RETURN) {
        token_free(tok);
        
        stmt = statement_create(STMT_RETURN);
        // TODO: Parse return expression using arithmetic module
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ Variable declaration - use variable module
    if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING || tok->type == TOKEN_BOOLEAN) {
        // TODO: Use variable_parse() from variable module
        token_free(tok);
        stmt = statement_create(STMT_VARIABLE_DECL);
        stmt->next = NULL;
        return stmt;
    }
    
    // Unknown statement
    token_free(tok);
    return NULL;
}
