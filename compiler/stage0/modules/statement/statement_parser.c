#include "statement_parser.h"
#include "../control_flow/control_flow_parser.h"  // ✅ While/If parsing
#include "../for_loop/for_loop_parser.h"           // ✅ For loop parsing
#include "../print/print_parser.h"                 // ✅ Print parsing
#include "../variable/variable_parser.h"           // ✅ Variable declarations
#include "../arithmetic/arithmetic_parser.h"       // ✅ Expressions
#include "../arithmetic/arithmetic.h"              // ✅ ArithmeticExpr
#include "../functions/functions.h"                // ✅ ReturnStatement
#include "../lexer/lexer.h"                        // ✅ Token operations
#include "../error/error.h"                        // ✅ Error handling system
#include "../array/array.h"                        // ✅ YZ_15: IndexAccess, ArrayAssignment
#include "../array/array_parser.h"                 // ✅ YZ_15: array_parse_index_access
#include "../import/import_parser.h"               // ✅ YZ_35: Import statement parsing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Statement parsing with modular imports
// Each statement type delegates to its specific module

Statement* statement_parse(Parser* parser) {
    if (!parser || !parser->lexer) {
        error_fatal("statement_parse - NULL parser or lexer");
        return NULL;
    }
    
    // Check if we have a lookahead token
    Token* tok = parser->current_token;
    if (tok) {
        // Use cached token and clear it
        parser->current_token = NULL;
    } else {
        // Read next token from lexer
        tok = lexer_next_token(parser->lexer);
    }
    
    if (!tok) return NULL;
    
    Statement* stmt = NULL;
    
    // Check for end of block
    if (tok->type == TOKEN_END) {
        // Peek at next token to see what kind of end
        Token* next = lexer_next_token(parser->lexer);
        if (next && (next->type == TOKEN_WHILE || 
                     next->type == TOKEN_FOR ||
                     next->type == TOKEN_FUNCTION)) {
            // End of block - don't consume these tokens
            token_free(next);
            token_free(tok);
            return NULL;  // End of block
        }
        
        // ✅ Special case: "end if" - consume "if" and check what's after
        if (next && next->type == TOKEN_IF) {
            token_free(next);  // Consume "if"
            // Check if there's an "else" after "end if"
            Token* after_if = lexer_next_token(parser->lexer);
            if (after_if) {
                parser->current_token = after_if;  // Save for parent parser
            }
            token_free(tok);
            return NULL;  // End of "end if"
        }
        
        // For other cases, put the next token back
        if (next) {
            parser->current_token = next;  // Save for parent parser
        }
        token_free(tok);
        return NULL;
    }
    
    // ✅ ELSE keyword - not a statement, return NULL and keep token for parent if parser
    if (tok->type == TOKEN_ELSE) {
        parser->current_token = tok;  // Keep token for parent parser
        return NULL;
    }
    
    // ✅ YZ_35: IMPORT statement - use import module
    if (tok->type == TOKEN_IMPORT) {
        ImportStatement* import_data = import_parse(parser->lexer, tok);
        
        // We own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (import_data) {
            stmt = statement_create(STMT_IMPORT);
            stmt->data = import_data;
            stmt->next = NULL;
            
            // Note: Import statements are typically at top of file
            // Module loading will be handled by compiler/linker
            // For now, we just parse and store the import
        }
        
        return stmt;
    }
    
    // ✅ WHILE statement - use control_flow module
    if (tok->type == TOKEN_WHILE) {
        // ✅ NEW STATELESS PATTERN - No malloc/free!
        // Call template function, passing token
        WhileStatement* while_data = control_flow_parse_while(parser->lexer, tok);
        
        // We still own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (while_data) {
            stmt = statement_create(STMT_WHILE);
            stmt->data = while_data;
            stmt->next = NULL;
            
            // Parse body recursively
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Statement* body_stmt = statement_parse(parser);
                if (!body_stmt) {
                    break;
                }
                
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
        
        return stmt;  // ✅ No more control_flow_parser_free!
    }
    
    // ✅ IF statement - NEW STATELESS PATTERN!
    if (tok->type == TOKEN_IF) {
        
        // ✅ NEW STATELESS PATTERN - No malloc/free!
        IfStatement* if_data = control_flow_parse_if(parser->lexer, tok);
        
        // We still own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (if_data) {
            stmt = statement_create(STMT_IF);
            stmt->data = if_data;
            stmt->next = NULL;
            
            // Parse then body recursively
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Statement* body_stmt = statement_parse(parser);
                if (!body_stmt) {
                    break;
                }
                
                if (!body_head) {
                    body_head = body_stmt;
                    body_tail = body_stmt;
                } else {
                    body_tail->next = body_stmt;
                    body_tail = body_stmt;
                }
            }
            
            // ✅ Store then body in IfStatement
            if_data->then_body = body_head;
            
            // ✅ Check for ELSE clause (this is the ONLY difference from while!)
            // After "end if", statement_parser will have put next token in current_token
            // This could be "else" or something else
            Token* check_tok = parser->current_token;
            if (check_tok && check_tok->type == TOKEN_ELSE) {
                parser->current_token = NULL;  // Consume "else"
                token_free(check_tok);
                if_data->has_else = 1;
                
                // Parse else body recursively (SAME AS WHILE/THEN BODY PARSING!)
                Statement* else_head = NULL;
                Statement* else_tail = NULL;
                
                while (1) {
                    Statement* body_stmt = statement_parse(parser);
                    if (!body_stmt) {
                        break;
                    }
                    
                    if (!else_head) {
                        else_head = body_stmt;
                        else_tail = body_stmt;
                    } else {
                        else_tail->next = body_stmt;
                        else_tail = body_stmt;
                    }
                }
                
                // ✅ Store else body in IfStatement
                if_data->else_body = else_head;
            }
            // If no else, just continue
        }
        
        return stmt;  // ✅ No more control_flow_parser_free!
    }
    
    // ✅ FOR statement - use for_loop module
    if (tok->type == TOKEN_FOR) {
        
        // ✅ NEW STATELESS PATTERN - No malloc/free!
        ForLoop* for_data = for_loop_parse(parser->lexer, tok);
        
        // We still own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (for_data) {
            stmt = statement_create(STMT_FOR);
            stmt->data = for_data;
            stmt->next = NULL;
            
            // Parse body recursively (like while loop)
            Statement* body_head = NULL;
            Statement* body_tail = NULL;
            
            while (1) {
                Statement* body_stmt = statement_parse(parser);
                if (!body_stmt) {
                    break;
                }
                
                if (!body_head) {
                    body_head = body_stmt;
                    body_tail = body_stmt;
                } else {
                    body_tail->next = body_stmt;
                    body_tail = body_stmt;
                }
            }
            
            // ✅ Store body in ForLoop
            for_data->body = body_head;
        }
        
        return stmt;
    }
    
    // ✅ YZ_28: EXIT statement - exit from block (VB.NET style)
    // Syntax: exit, exit for, exit while, exit if, exit function
    if (tok->type == TOKEN_EXIT) {
        token_free(tok);
        
        // Peek next token to see if it's "for", "while", "if", "function"
        Token* next = lexer_next_token(parser->lexer);
        
        if (next && next->type == TOKEN_FOR) {
            // exit for
            token_free(next);
            stmt = statement_create(STMT_EXIT_FOR);
        } else if (next && next->type == TOKEN_WHILE) {
            // exit while
            token_free(next);
            stmt = statement_create(STMT_EXIT_WHILE);
        } else if (next && next->type == TOKEN_IF) {
            // exit if
            token_free(next);
            stmt = statement_create(STMT_EXIT_IF);
        } else if (next && next->type == TOKEN_FUNCTION) {
            // exit function
            token_free(next);
            stmt = statement_create(STMT_EXIT_FUNCTION);
        } else {
            // Just "exit" - exit from nearest block
            if (next) {
                // Put token back for next parse
                parser->current_token = next;
            }
            stmt = statement_create(STMT_EXIT);
        }
        
        stmt->data = NULL;
        stmt->next = NULL;
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
    
    // ✅ YZ_48: PRINTLN builtin - Reserved keyword
    if (tok->type == TOKEN_PRINTLN) {
        token_free(tok);
        
        Token* lparen = lexer_next_token(parser->lexer);
        if (!lparen || lparen->type != TOKEN_LPAREN) {
            error_parser(0, "Expected '(' after 'println'");
            if (lparen) token_free(lparen);
            return NULL;
        }
        token_free(lparen);
        
        Token* expr_tok = lexer_next_token(parser->lexer);
        if (!expr_tok) {
            error_parser(0, "Expected expression in println()");
            return NULL;
        }
        
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok);
        token_free(expr_tok);
        
        if (!expr) {
            return NULL;
        }
        
        Token* rparen = lexer_next_token(parser->lexer);
        if (!rparen || rparen->type != TOKEN_RPAREN) {
            error_parser(0, "Expected ')' after println expression");
            if (rparen) token_free(rparen);
            arithmetic_expr_free(expr);
            return NULL;
        }
        token_free(rparen);
        
        stmt = statement_create(STMT_EXPRESSION);
        stmt->data = expr;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ RETURN statement
    if (tok->type == TOKEN_RETURN) {
        token_free(tok);
        
        // Parse return expression using stateless arithmetic API
        Token* expr_tok = lexer_next_token(parser->lexer);
        if (!expr_tok) {
            error_parser(0, "Expected expression after 'return'");
            return NULL;
        }
        
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok);
        token_free(expr_tok);
        
        // Create return statement with expression
        ReturnStatement* ret_stmt = return_statement_create((void*)expr);
        
        stmt = statement_create(STMT_RETURN);
        stmt->data = ret_stmt;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ Variable declaration - use variable module (STATELESS)
    if (tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || tok->type == TOKEN_BOOLEAN ||
        tok->type == TOKEN_LIST || tok->type == TOKEN_TUPLE) {
        // Call stateless version - tok is borrowed by variable_parse_declaration
        VariableDeclaration* decl = variable_parse_declaration(parser->lexer, tok);
        
        // We own tok, so we must free it
        token_free(tok);
        
        if (decl) {
            stmt = statement_create(STMT_VARIABLE_DECL);
            stmt->data = decl;
            stmt->next = NULL;
            return stmt;
        }
        
        // If parse failed, token already freed by parser
        return NULL;
    }
    
    // ✅ Variable assignment - check if identifier followed by '=' or '['
    if (tok->type == TOKEN_IDENTIFIER) {
        // Need to look ahead for '=' or '['
        Token* next_tok = lexer_next_token(parser->lexer);
        
        // YZ_15: Check for array assignment: arr[i] = value
        if (next_tok && next_tok->type == TOKEN_LBRACKET) {
            // Array element assignment
            char* arr_name = strdup(tok->value);
            token_free(tok);
            
            // Parse index access using existing array parser
            IndexAccess* access = array_parse_index_access(parser->lexer, arr_name, next_tok);
            token_free(next_tok);
            
            if (!access) {
                free(arr_name);
                error_parser(0, "Failed to parse array index");
                return NULL;
            }
            
            // Expect '=' after ']'
            Token* eq_tok = lexer_next_token(parser->lexer);
            if (!eq_tok || eq_tok->type != TOKEN_ASSIGN) {
                free(arr_name);
                if (access->collection_name) free(access->collection_name);
                if (access->index_type == 1 && access->index.var_index) free(access->index.var_index);
                free(access);
                if (eq_tok) token_free(eq_tok);
                error_parser(0, "Expected '=' after array index");
                return NULL;
            }
            token_free(eq_tok);
            
            // Parse value expression
            Token* expr_tok = lexer_next_token(parser->lexer);
            if (!expr_tok) {
                free(arr_name);
                if (access->collection_name) free(access->collection_name);
                if (access->index_type == 1 && access->index.var_index) free(access->index.var_index);
                free(access);
                error_parser(0, "Expected expression after '='");
                return NULL;
            }
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok);
            token_free(expr_tok);
            
            if (!expr) {
                free(arr_name);
                if (access->collection_name) free(access->collection_name);
                if (access->index_type == 1 && access->index.var_index) free(access->index.var_index);
                free(access);
                return NULL;
            }
            
            // Create array assignment
            ArrayAssignment* arr_assign = malloc(sizeof(ArrayAssignment));
            arr_assign->index_access = (void*)access;  // Store as void*
            arr_assign->value_expr = expr;
            arr_assign->tto_info = NULL;
            arr_assign->tto_analyzed = false;
            
            stmt = statement_create(STMT_ARRAY_ASSIGNMENT);
            stmt->data = arr_assign;
            stmt->next = NULL;
            return stmt;
        }
        
        // Variable assignment: var = value
        // YZ_25: If statement ends with ';', it's implicit declaration (type inferred later)
        // If no ';' and variable not declared, codegen will catch the error
        if (next_tok && next_tok->type == TOKEN_ASSIGN) {
            token_free(next_tok);  // We know it's '=', consume it
            
            // Parse expression after '=' using stateless API
            Token* expr_tok = lexer_next_token(parser->lexer);
            if (!expr_tok) {
                token_free(tok);
                error_parser(0, "Expected expression after '='");
                return NULL;
            }
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok);
            token_free(expr_tok);
            
            if (expr) {
                // YZ_25: Check for trailing semicolon (implicit declaration marker)
                Token* maybe_semicolon = lexer_next_token(parser->lexer);
                int is_implicit_declaration = 0;
                
                if (maybe_semicolon && maybe_semicolon->type == TOKEN_SEMICOLON) {
                    // Has semicolon - this is an implicit variable declaration
                    is_implicit_declaration = 1;
                    token_free(maybe_semicolon);
                } else {
                    // No semicolon - put token back for next statement
                    if (maybe_semicolon) {
                        lexer_unget_token(parser->lexer, maybe_semicolon);
                    }
                }
                
                VariableAssignment* assign = malloc(sizeof(VariableAssignment));
                assign->name = strdup(tok->value);
                assign->value_expr = expr;
                assign->tto_info = NULL;
                assign->tto_analyzed = false;
                assign->needs_type_promotion = false;
                assign->is_implicit_declaration = is_implicit_declaration;  // YZ_25: Mark if implicit
                
                token_free(tok);
                
                stmt = statement_create(STMT_ASSIGNMENT);
                stmt->data = assign;
                stmt->next = NULL;
                return stmt;
            }
            
            token_free(tok);
            return NULL;
        } else {
            // Not assignment, put token back and return NULL
            if (next_tok) {
                parser->current_token = next_tok;
            }
            token_free(tok);
            return NULL;
        }
    }
    
    // Unknown statement
    token_free(tok);
    return NULL;
}
