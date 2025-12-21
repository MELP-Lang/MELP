#include "statement_parser.h"
#include "../control_flow/control_flow_parser.h"  // ✅ While/If parsing
#include "../for_loop/for_loop_parser.h"           // ✅ For loop parsing
#include "../switch/switch_parser.h"               // ✅ YZ_89: Switch parsing
#include "../print/print_parser.h"                 // ✅ Print parsing
#include "../print/print.h"                        // ✅ YZ_61: PrintStatement struct
#include "../variable/variable_parser.h"           // ✅ Variable declarations
#include "../array/arithmetic/arithmetic_parser.h"       // ✅ Expressions
#include "../array/arithmetic/arithmetic.h"              // ✅ ArithmeticExpr
#include "../functions/functions.h"                // ✅ ReturnStatement
#include "../functions/functions_parser.h"         // ✅ YZ_26: Function parsing
#include "../lexer/lexer.h"                        // ✅ Token operations
#include "../error/error.h"                        // ✅ Error handling system
#include "../array/array.h"                        // ✅ YZ_15: IndexAccess, ArrayAssignment
#include "../array/array_parser.h"                 // ✅ YZ_15: array_parse_index_access
#include "../import/import_parser.h"               // ✅ YZ_35: Import statement parsing
#include "../struct/struct.h"                      // ✅ YZ_81: Struct definitions
#include "../struct/struct_parser.h"               // ✅ YZ_81: Struct parsing
#include "../enum/enum.h"                          // ✅ YZ_96: Enum definitions
#include "../enum/enum_parser.h"                   // ✅ YZ_96: Enum parsing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
static Statement* parse_else_chain(Parser* parser);
static Statement* parse_statement_list(Parser* parser);

// Helper: Parse a list of statements until block terminator
static Statement* parse_statement_list(Parser* parser) {
    Statement* head = NULL;
    Statement* tail = NULL;
    
    while (1) {
        Statement* stmt = statement_parse(parser);
        if (!stmt) break;
        
        if (!head) {
            head = stmt;
            tail = stmt;
        } else {
            tail->next = stmt;
            tail = stmt;
        }
    }
    
    return head;
}

// Helper: Parse else/else_if chain recursively (YZ_36 - Recursive else_if)
// Returns: Statement* representing else body (may contain nested if for else_if)
static Statement* parse_else_chain(Parser* parser) {
    // Check if current_token has ELSE (set by parent)
    Token* check = parser->current_token;
    if (!check || check->type != TOKEN_ELSE) {
        return NULL;  // No else/else_if
    }
    
    // Consume ELSE token
    parser->current_token = NULL;
    token_free(check);
    
    // Peek: is it else_if?
    Token* peek = lexer_next_token(parser->lexer);
    
    if (peek && peek->type == TOKEN_IF) {
        // This is else_if! Create nested if
        IfStatement* nested_if = control_flow_parse_if(parser->lexer, peek);
        token_free(peek);
        
        if (!nested_if) {
            return NULL;
        }
        
        // Parse then body of nested if
        nested_if->then_body = parse_statement_list(parser);
        
        // RECURSIVE: Check for more else/else_if in the chain
        Statement* nested_else = parse_else_chain(parser);
        if (nested_else) {
            nested_if->has_else = 1;
            nested_if->else_body = nested_else;
        }
        
        // Wrap nested if as statement
        Statement* stmt = statement_create(STMT_IF);
        stmt->data = nested_if;
        stmt->next = NULL;
        return stmt;
        
    } else {
        // Normal else - no if following
        if (peek) {
            lexer_unget_token(parser->lexer, peek);
        }
        
        // Parse else body
        return parse_statement_list(parser);
    }
}

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
    
    // ⭐ RF_YZ_3: PMPL block terminators (single tokens, no pattern matching!)
    switch (tok->type) {
        case TOKEN_END_IF:
            // Check if there's an "else" or "else_if" after "end_if"
            {
                Token* after = lexer_next_token(parser->lexer);
                if (after) {
                    parser->current_token = after;  // Save for parent parser
                }
                token_free(tok);
                return NULL;
            }
        
        case TOKEN_END:  // YZ_30: Generic 'end' keyword (Python-style function terminator)
            {
                // YZ_30: Check for 'end X' (two-word terminators)
                // Supports: end function, end if, end while, end for, end struct
                Token* after = lexer_next_token(parser->lexer);
                if (after && (after->type == TOKEN_FUNCTION ||
                              after->type == TOKEN_IF ||
                              after->type == TOKEN_WHILE ||
                              after->type == TOKEN_FOR ||
                              after->type == TOKEN_STRUCT ||
                              after->type == TOKEN_SWITCH)) {
                    // 'end X' - consume both tokens
                    token_free(after);
                }  else if (after) {
                    // Not a block keyword - put it back
                    lexer_unget_token(parser->lexer, after);
                }
                token_free(tok);
                return NULL;  // End of block
            }
        
        case TOKEN_END_WHILE:
        case TOKEN_END_FOR:
        case TOKEN_END_FUNCTION:
        case TOKEN_END_STRUCT:
        case TOKEN_END_SWITCH:
        case TOKEN_END_MATCH:
        case TOKEN_END_OPERATOR:
        case TOKEN_END_TRY:
            token_free(tok);
            return NULL;  // End of block
        
        // ✅ YZ_90: Explicit exit tokens (PMPL underscore style)
        case TOKEN_EXIT_FOR:
            token_free(tok);
            stmt = statement_create(STMT_EXIT_FOR);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        case TOKEN_EXIT_WHILE:
            token_free(tok);
            stmt = statement_create(STMT_EXIT_WHILE);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        case TOKEN_EXIT_IF:
            token_free(tok);
            stmt = statement_create(STMT_EXIT_IF);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        case TOKEN_EXIT_FUNCTION:
            token_free(tok);
            stmt = statement_create(STMT_EXIT_FUNCTION);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        case TOKEN_EXIT_SWITCH:
            token_free(tok);
            stmt = statement_create(STMT_EXIT_SWITCH);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        // ✅ YZ_90: Explicit continue tokens (PMPL underscore style)
        case TOKEN_CONTINUE_FOR:
            token_free(tok);
            stmt = statement_create(STMT_CONTINUE_FOR);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        case TOKEN_CONTINUE_WHILE:
            token_free(tok);
            stmt = statement_create(STMT_CONTINUE_WHILE);
            stmt->data = NULL;
            stmt->next = NULL;
            return stmt;
        
        default:
            // Not a block terminator, continue parsing
            break;
    }
    
    // ✅ ELSE keyword - not a statement, return NULL and keep token for parent if parser
    if (tok->type == TOKEN_ELSE) {
        parser->current_token = tok;  // Keep token for parent parser
        return NULL;
    }
    
    // YZ_36: ELSE_IF handling - convert to ELSE + IF for parent
    // TOKEN_ELSE_IF is a single token, but we need to handle it as ELSE + IF
    if (tok->type == TOKEN_ELSE_IF) {
        // This else_if is at the same level as the parent if
        // We need to:
        // 1. Free the TOKEN_ELSE_IF
        // 2. Create synthetic TOKEN_ELSE for current_token
        // 3. Create synthetic TOKEN_IF and push to lexer
        
        token_free(tok);
        
        // Create synthetic IF token and push to lexer
        Token* if_tok = malloc(sizeof(Token));
        if_tok->type = TOKEN_IF;
        if_tok->value = strdup("if");
        if_tok->line = parser->lexer->line;
        if_tok->has_leading_whitespace = 0;
        lexer_unget_token(parser->lexer, if_tok);
        
        // Create synthetic ELSE token for parent
        Token* else_tok = malloc(sizeof(Token));
        else_tok->type = TOKEN_ELSE;
        else_tok->value = strdup("else");
        else_tok->line = parser->lexer->line;
        else_tok->has_leading_whitespace = 0;
        parser->current_token = else_tok;
        
        return NULL;  // Parent will check current_token
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
    
    // ✅ YZ_81: STRUCT definition - use struct module
    if (tok->type == TOKEN_STRUCT) {
        StructDef* struct_data = parse_struct_definition(parser);  // YZ_88: Pass full parser
        
        // We own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (struct_data) {
            stmt = statement_create(STMT_STRUCT);
            stmt->data = struct_data;
            stmt->next = NULL;
        }
        
        return stmt;
    }
    
    // ✅ YZ_96: ENUM definition - use enum module
    if (tok->type == TOKEN_ENUM) {
        EnumDefinition* enum_data = enum_parse(parser->lexer, tok);
        
        // We own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (enum_data) {
            stmt = statement_create(STMT_ENUM);
            stmt->data = enum_data;
            stmt->next = NULL;
        }
        
        return stmt;
    }
    
    // ✅ YZ_26: FUNCTION definition - use functions module
    if (tok->type == TOKEN_FUNCTION) {
        // Put token back - parse_function_declaration expects to read it
        lexer_unget_token(parser->lexer, tok);
        
        FunctionDeclaration* func_data = parse_function_declaration(parser->lexer);
        
        if (func_data) {
            stmt = statement_create(STMT_FUNCTION);
            stmt->data = func_data;
            stmt->next = NULL;
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
    
    // ✅ IF statement - NEW STATELESS PATTERN! (YZ_36: Recursive else_if)
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
            
            // Parse then body using helper
            if_data->then_body = parse_statement_list(parser);
            
            // ✅ YZ_36: Check for ELSE/ELSE_IF chain using recursive helper!
            // After "end_if", statement_parse may have set current_token to ELSE
            Statement* else_body = parse_else_chain(parser);
            if (else_body) {
                if_data->has_else = 1;
                if_data->else_body = else_body;
            }
        }
        
        return stmt;  // ✅ Clean and simple!
    }
    
    // ✅ YZ_89: SWITCH statement
    if (tok->type == TOKEN_SWITCH) {
        SwitchStatement* switch_data = switch_parse(parser, tok);
        
        // We own tok - free it!
        token_free(tok);
        tok = NULL;
        
        if (switch_data) {
            stmt = statement_create(STMT_SWITCH);
            stmt->data = switch_data;
            stmt->next = NULL;
        }
        
        return stmt;
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
    
    // ✅ YZ_90: Standalone EXIT - generic loop exit (PMPL spec allows this)
    // YZ_30: Also handle 'exit while', 'exit for', 'exit if' (two-word syntax)
    if (tok->type == TOKEN_EXIT) {
        // Check for 'exit X' pattern
        Token* after = lexer_next_token(parser->lexer);
        StatementType exit_type = STMT_EXIT;  // Default: generic exit
        
        if (after) {
            if (after->type == TOKEN_WHILE) {
                exit_type = STMT_EXIT_WHILE;
                token_free(after);
            } else if (after->type == TOKEN_FOR) {
                exit_type = STMT_EXIT_FOR;
                token_free(after);
            } else if (after->type == TOKEN_IF) {
                exit_type = STMT_EXIT_IF;
                token_free(after);
            } else if (after->type == TOKEN_FUNCTION) {
                exit_type = STMT_EXIT_FUNCTION;
                token_free(after);
            } else if (after->type == TOKEN_SWITCH) {
                exit_type = STMT_EXIT_SWITCH;
                token_free(after);
            } else {
                // Not a known block type - put it back
                lexer_unget_token(parser->lexer, after);
            }
        }
        
        token_free(tok);
        stmt = statement_create(exit_type);
        stmt->data = NULL;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ YZ_90: Standalone CONTINUE - generic loop continue (PMPL spec allows this)
    if (tok->type == TOKEN_CONTINUE) {
        token_free(tok);
        stmt = statement_create(STMT_CONTINUE);
        stmt->data = NULL;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ YZ_61: PRINT statement - use print module parser
    if (tok->type == TOKEN_PRINT) {
        // Delegate to print_parser module (stateless pattern - pass token)
        PrintStatement* print_stmt = parse_print_statement(parser->lexer, tok);
        token_free(tok);
        tok = NULL;
        
        if (!print_stmt) {
            return NULL;
        }
        
        stmt = statement_create(STMT_PRINT);
        stmt->data = print_stmt;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ YZ_47: PRINTLN builtin - Reserved keyword (parser)
    // ✅ YZ_48: FunctionCallExpr creation fixed
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
        
        ArithmeticExpr* arg_expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
        token_free(expr_tok);
        
        if (!arg_expr) {
            return NULL;
        }
        
        Token* rparen = lexer_next_token(parser->lexer);
        if (!rparen || rparen->type != TOKEN_RPAREN) {
            error_parser(0, "Expected ')' after println expression");
            if (rparen) token_free(rparen);
            arithmetic_expr_free(arg_expr);
            return NULL;
        }
        token_free(rparen);
        
        // Create FunctionCallExpr for println
        FunctionCallExpr* func_call = malloc(sizeof(FunctionCallExpr));
        func_call->function_name = strdup("println");
        func_call->arg_count = 1;
        func_call->arguments = malloc(sizeof(ArithmeticExpr*));
        func_call->arguments[0] = arg_expr;
        
        // Wrap in ArithmeticExpr
        ArithmeticExpr* expr = malloc(sizeof(ArithmeticExpr));
        memset(expr, 0, sizeof(ArithmeticExpr));
        expr->is_function_call = 1;
        expr->func_call = func_call;
        
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
        
        ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
        token_free(expr_tok);
        
        // Create return statement with expression
        ReturnStatement* ret_stmt = return_statement_create((void*)expr);
        
        stmt = statement_create(STMT_RETURN);
        stmt->data = ret_stmt;
        stmt->next = NULL;
        return stmt;
    }
    
    // ✅ Variable declaration - use variable module (STATELESS)
    // YZ_CONST: Also handle const declarations
    if (tok->type == TOKEN_CONST || tok->type == TOKEN_NUMERIC || tok->type == TOKEN_STRING_TYPE || tok->type == TOKEN_BOOLEAN ||
        tok->type == TOKEN_ARRAY || tok->type == TOKEN_LIST || tok->type == TOKEN_MAP || tok->type == TOKEN_TUPLE) {
        // Call stateless version - tok is borrowed by variable_parse_declaration
        VariableDeclaration* decl = variable_parse_declaration(parser->lexer, tok);
        
        // We own tok, so we must free it
        token_free(tok);
        
        if (decl) {
            // ✅ RF_YZ_3: Consume optional trailing semicolon
            Token* maybe_semicolon = lexer_next_token(parser->lexer);
            if (maybe_semicolon && maybe_semicolon->type == TOKEN_SEMICOLON) {
                token_free(maybe_semicolon);  // Consume semicolon
            } else if (maybe_semicolon) {
                // Not a semicolon - put it back for next statement
                lexer_unget_token(parser->lexer, maybe_semicolon);
            }
            
            stmt = statement_create(STMT_VARIABLE_DECL);
            stmt->data = decl;
            stmt->next = NULL;
            return stmt;
        }
        
        // If parse failed, token already freed by parser
        return NULL;
    }
    
    // ✅ Variable assignment or function call - check if identifier followed by '=', '[', or '('
    if (tok->type == TOKEN_IDENTIFIER) {
        // YZ_82: Check if this is a struct instance declaration (Point p)
        StructDef* struct_def = struct_try_parse_instance_decl(parser->lexer, tok);
        if (struct_def) {
            // This is a struct type! Next token should be instance variable name
            Token* inst_name_tok = lexer_next_token(parser->lexer);
            
            if (!inst_name_tok || inst_name_tok->type != TOKEN_IDENTIFIER) {
                token_free(tok);
                if (inst_name_tok) token_free(inst_name_tok);
                error_parser(0, "Expected variable name after struct type '%s'", tok->value);
                return NULL;
            }
            
            // Create struct instance
            StructInstance* instance = struct_create_instance(struct_def, inst_name_tok->value);
            
            token_free(tok);
            token_free(inst_name_tok);
            
            stmt = statement_create(STMT_STRUCT_INSTANCE);
            stmt->data = instance;
            stmt->next = NULL;
            return stmt;
        }
        
        // YZ_101: Check if this is an enum variable declaration (Color c = Color.Red)
        if (enum_is_type(tok->value)) {
            // This is an enum type!
            char* enum_type = strdup(tok->value);
            token_free(tok);
            
            // Next token should be variable name
            Token* var_name_tok = lexer_next_token(parser->lexer);
            if (!var_name_tok || var_name_tok->type != TOKEN_IDENTIFIER) {
                free(enum_type);
                if (var_name_tok) token_free(var_name_tok);
                error_parser(0, "Expected variable name after enum type '%s'", enum_type);
                return NULL;
            }
            
            char* var_name = strdup(var_name_tok->value);
            token_free(var_name_tok);
            
            // Check for optional initializer: = EnumType.Value
            Token* eq_tok = lexer_next_token(parser->lexer);
            int64_t init_value = 0;
            int has_initializer = 0;
            
            if (eq_tok && eq_tok->type == TOKEN_ASSIGN) {
                token_free(eq_tok);
                has_initializer = 1;
                
                // Expect: EnumType.ValueName
                Token* type_tok = lexer_next_token(parser->lexer);
                if (!type_tok || type_tok->type != TOKEN_IDENTIFIER) {
                    free(enum_type);
                    free(var_name);
                    if (type_tok) token_free(type_tok);
                    error_parser(0, "Expected enum type in initializer");
                    return NULL;
                }
                
                Token* dot_tok = lexer_next_token(parser->lexer);
                if (!dot_tok || dot_tok->type != TOKEN_DOT) {
                    free(enum_type);
                    free(var_name);
                    token_free(type_tok);
                    if (dot_tok) token_free(dot_tok);
                    error_parser(0, "Expected '.' in enum value");
                    return NULL;
                }
                token_free(dot_tok);
                
                Token* value_tok = lexer_next_token(parser->lexer);
                if (!value_tok || value_tok->type != TOKEN_IDENTIFIER) {
                    free(enum_type);
                    free(var_name);
                    token_free(type_tok);
                    if (value_tok) token_free(value_tok);
                    error_parser(0, "Expected enum value name");
                    return NULL;
                }
                
                // Lookup enum value
                init_value = enum_lookup_value(type_tok->value, value_tok->value);
                if (init_value == -1) {
                    error_parser(0, "Unknown enum value: %s.%s", type_tok->value, value_tok->value);
                    free(enum_type);
                    free(var_name);
                    token_free(type_tok);
                    token_free(value_tok);
                    return NULL;
                }
                
                token_free(type_tok);
                token_free(value_tok);
            } else if (eq_tok) {
                // Not '=' - put it back
                lexer_unget_token(parser->lexer, eq_tok);
            }
            
            // Create enum variable
            EnumVariable* enum_var = enum_variable_create(enum_type, var_name, init_value, has_initializer);
            free(enum_type);
            free(var_name);
            
            stmt = statement_create(STMT_ENUM_VARIABLE);
            stmt->data = enum_var;
            stmt->next = NULL;
            return stmt;
        }
        
        // Need to look ahead for '=', '[', '(', or '.'
        Token* next_tok = lexer_next_token(parser->lexer);
        
        // YZ_82/YZ_83: Check for member assignment: p.x = value or p.addr.zip = value
        if (next_tok && next_tok->type == TOKEN_DOT) {
            // Struct member assignment (possibly nested)
            char* inst_name = strdup(tok->value);
            token_free(tok);
            token_free(next_tok);
            
            // YZ_83: Parse member chain (could be multiple levels)
            char** member_chain = NULL;
            int chain_length = 0;
            int chain_capacity = 4;
            member_chain = malloc(sizeof(char*) * chain_capacity);
            
            // Loop to collect all chained members until '='
            while (1) {
                // Expect member name
                Token* member_tok = lexer_next_token(parser->lexer);
                if (!member_tok || member_tok->type != TOKEN_IDENTIFIER) {
                    free(inst_name);
                    for (int i = 0; i < chain_length; i++) {
                        free(member_chain[i]);
                    }
                    free(member_chain);
                    if (member_tok) token_free(member_tok);
                    error_parser(0, "Expected member name after '.'");
                    return NULL;
                }
                
                // Expand array if needed
                if (chain_length >= chain_capacity) {
                    chain_capacity *= 2;
                    member_chain = realloc(member_chain, sizeof(char*) * chain_capacity);
                }
                
                member_chain[chain_length++] = strdup(member_tok->value);
                token_free(member_tok);
                
                // Check what comes next: '.' or '='
                Token* next = lexer_next_token(parser->lexer);
                if (!next) {
                    free(inst_name);
                    for (int i = 0; i < chain_length; i++) {
                        free(member_chain[i]);
                    }
                    free(member_chain);
                    error_parser(0, "Unexpected end after member");
                    return NULL;
                }
                
                if (next->type == TOKEN_ASSIGN) {
                    // Done with chain, move to value expression
                    token_free(next);
                    break;
                } else if (next->type == TOKEN_DOT) {
                    // Continue chain
                    token_free(next);
                    continue;
                } else {
                    free(inst_name);
                    for (int i = 0; i < chain_length; i++) {
                        free(member_chain[i]);
                    }
                    free(member_chain);
                    token_free(next);
                    error_parser(0, "Expected '=' or '.' after member name");
                    return NULL;
                }
            }
            
            // Parse value expression
            Token* expr_tok = lexer_next_token(parser->lexer);
            if (!expr_tok) {
                free(inst_name);
                for (int i = 0; i < chain_length; i++) {
                    free(member_chain[i]);
                }
                free(member_chain);
                error_parser(0, "Expected expression after '='");
                return NULL;
            }
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
            token_free(expr_tok);
            
            if (!expr) {
                free(inst_name);
                for (int i = 0; i < chain_length; i++) {
                    free(member_chain[i]);
                }
                free(member_chain);
                return NULL;
            }
            
            // Create member assignment structure
            MemberAssignment* mem_assign = malloc(sizeof(MemberAssignment));
            mem_assign->instance_name = inst_name;
            mem_assign->member_name = (chain_length > 0) ? strdup(member_chain[0]) : NULL;  // Keep for compat
            mem_assign->value_expr = expr;
            mem_assign->member_chain = member_chain;
            mem_assign->chain_length = chain_length;
            
            stmt = statement_create(STMT_MEMBER_ASSIGNMENT);
            stmt->data = mem_assign;
            stmt->next = NULL;
            return stmt;
        }
        
        // YZ_65: Check for function call: func(args)
        if (next_tok && next_tok->type == TOKEN_LPAREN) {
            // Function call as statement (return value ignored)
            // Put the '(' back, arithmetic parser will consume it
            lexer_unget_token(parser->lexer, next_tok);
            
            // Parse function call using arithmetic parser
            ArithmeticExpr* call_expr = arithmetic_parse_expression_stateless(parser->lexer, tok, NULL);
            
            if (!call_expr) {
                token_free(tok);
                error_parser(0, "Failed to parse function call");
                return NULL;
            }
            
            // tok is now owned by call_expr, don't free it
            
            // Create expression statement
            stmt = statement_create(STMT_EXPRESSION);
            stmt->data = call_expr;
            stmt->next = NULL;
            return stmt;
        }
        
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
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
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
            arr_assign->sto_info = NULL;
            arr_assign->sto_analyzed = false;
            
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
            
            ArithmeticExpr* expr = arithmetic_parse_expression_stateless(parser->lexer, expr_tok, NULL);
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
                assign->sto_info = NULL;
                assign->sto_analyzed = false;
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
