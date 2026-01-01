#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create AST node
ASTNode* make_ast_node(ASTNodeType type, const char* value, int line) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->line = line;
    node->left = NULL;
    node->right = NULL;
    node->first_child = NULL;
    node->next_sibling = NULL;
    node->data_type = NULL;
    return node;
}

// Add child to parent node (linked list)
void ast_add_child(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    
    if (parent->first_child == NULL) {
        parent->first_child = child;
    } else {
        ASTNode* last = parent->first_child;
        while (last->next_sibling != NULL) {
            last = last->next_sibling;
        }
        last->next_sibling = child;
    }
}

// Free AST recursively
void ast_free(ASTNode* node) {
    if (!node) return;
    
    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->first_child);
    ast_free(node->next_sibling);
    
    if (node->value) free(node->value);
    if (node->data_type) free(node->data_type);
    free(node);
}

// Create parser state
ParserState* parser_create(Token** tokens, int count) {
    ParserState* parser = malloc(sizeof(ParserState));
    parser->tokens = tokens;
    parser->token_count = count;
    parser->current = 0;
    parser->has_error = 0;
    parser->error_message = NULL;
    return parser;
}

void parser_free(ParserState* parser) {
    if (parser->error_message) free(parser->error_message);
    free(parser);
}

// Helper: Get current token
static Token* current_token(ParserState* parser) {
    if (parser->current >= parser->token_count) return NULL;
    return parser->tokens[parser->current];
}

// Helper: Advance to next token
static void advance(ParserState* parser) {
    if (parser->current < parser->token_count) {
        parser->current++;
    }
}

// Helper: Check if current token matches type
static int match(ParserState* parser, TokenType type) {
    Token* token = current_token(parser);
    return token && token->type == type;
}

// Helper: Consume token if matches, error otherwise
static int expect(ParserState* parser, TokenType type) {
    if (match(parser, type)) {
        advance(parser);
        return 1;
    }
    parser->has_error = 1;
    return 0;
}

// Forward declarations
static ASTNode* parse_statement(ParserState* parser);
static ASTNode* parse_expression(ParserState* parser);

// Parse function definition
static ASTNode* parse_function(ParserState* parser) {
    Token* func_token = current_token(parser);
    advance(parser); // Skip 'function'
    
    Token* name_token = current_token(parser);
    if (!match(parser, TOKEN_IDENTIFIER)) {
        parser->has_error = 1;
        return NULL;
    }
    advance(parser);
    
    ASTNode* func_node = make_ast_node(AST_FUNCTION, name_token->value, func_token->line);
    
    // Parameters: function_name(param1, param2)
    if (match(parser, TOKEN_LPAREN)) {
        advance(parser);
        
        while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
            Token* param = current_token(parser);
            if (match(parser, TOKEN_IDENTIFIER)) {
                ASTNode* param_node = make_ast_node(AST_IDENTIFIER, param->value, param->line);
                ast_add_child(func_node, param_node);
                advance(parser);
                
                if (match(parser, TOKEN_COMMA)) advance(parser);
            } else {
                break;
            }
        }
        
        if (match(parser, TOKEN_RPAREN)) advance(parser);
    }
    
    // Function body
    ASTNode* body = make_ast_node(AST_BLOCK, NULL, func_token->line);
    
    while (!match(parser, TOKEN_END_FUNCTION) && !match(parser, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt) ast_add_child(body, stmt);
    }
    
    if (match(parser, TOKEN_END_FUNCTION)) advance(parser);
    
    ast_add_child(func_node, body);
    return func_node;
}

// Parse struct definition
static ASTNode* parse_struct(ParserState* parser) {
    Token* struct_token = current_token(parser);
    advance(parser); // Skip 'struct'
    
    Token* name_token = current_token(parser);
    if (!match(parser, TOKEN_IDENTIFIER)) {
        parser->has_error = 1;
        return NULL;
    }
    advance(parser);
    
    if (match(parser, TOKEN_THEN)) advance(parser);
    
    ASTNode* struct_node = make_ast_node(AST_STRUCT, name_token->value, struct_token->line);
    
    // Parse fields
    while (!match(parser, TOKEN_END_STRUCT) && !match(parser, TOKEN_EOF)) {
        // Field: TypeName field_name
        Token* type_token = current_token(parser);
        if (match(parser, TOKEN_NUMERIC) || match(parser, TOKEN_STRING_TYPE) || 
            match(parser, TOKEN_BOOLEAN) || match(parser, TOKEN_LIST) || 
            match(parser, TOKEN_IDENTIFIER)) {
            advance(parser);
            
            Token* field_token = current_token(parser);
            if (match(parser, TOKEN_IDENTIFIER)) {
                ASTNode* field = make_ast_node(AST_VARIABLE_DECL, field_token->value, field_token->line);
                field->data_type = strdup(type_token->value);
                ast_add_child(struct_node, field);
                advance(parser);
            }
        } else {
            break;
        }
    }
    
    if (match(parser, TOKEN_END_STRUCT)) advance(parser);
    return struct_node;
}

// Parse variable declaration
static ASTNode* parse_variable_decl(ParserState* parser) {
    Token* type_token = current_token(parser);
    advance(parser); // Skip type
    
    Token* name_token = current_token(parser);
    if (!match(parser, TOKEN_IDENTIFIER)) {
        parser->has_error = 1;
        return NULL;
    }
    advance(parser);
    
    ASTNode* var_node = make_ast_node(AST_VARIABLE_DECL, name_token->value, type_token->line);
    var_node->data_type = strdup(type_token->value);
    
    // Optional initialization: Numeric x = 10
    if (match(parser, TOKEN_ASSIGN)) {
        advance(parser);
        ASTNode* expr = parse_expression(parser);
        if (expr) var_node->right = expr;
    }
    
    return var_node;
}

// Parse assignment
static ASTNode* parse_assignment(ParserState* parser) {
    Token* name_token = current_token(parser);
    ASTNode* left = make_ast_node(AST_IDENTIFIER, name_token->value, name_token->line);
    advance(parser);
    
    // Handle member access: obj.field
    while (match(parser, TOKEN_DOT)) {
        advance(parser);
        Token* field_token = current_token(parser);
        if (match(parser, TOKEN_IDENTIFIER)) {
            ASTNode* member = make_ast_node(AST_MEMBER_ACCESS, field_token->value, field_token->line);
            member->left = left;
            left = member;
            advance(parser);
        }
    }
    
    if (!match(parser, TOKEN_ASSIGN)) {
        // Not an assignment, might be function call
        if (match(parser, TOKEN_LPAREN)) {
            ASTNode* call = make_ast_node(AST_CALL, left->value, left->line);
            advance(parser); // Skip (
            
            while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
                ASTNode* arg = parse_expression(parser);
                if (arg) ast_add_child(call, arg);
                if (match(parser, TOKEN_COMMA)) advance(parser);
            }
            
            if (match(parser, TOKEN_RPAREN)) advance(parser);
            ast_free(left);
            return call;
        }
        return left;
    }
    
    advance(parser); // Skip =
    
    ASTNode* assign = make_ast_node(AST_ASSIGN, NULL, name_token->line);
    assign->left = left;
    assign->right = parse_expression(parser);
    
    return assign;
}

// Parse if statement
static ASTNode* parse_if(ParserState* parser) {
    Token* if_token = current_token(parser);
    advance(parser); // Skip 'if'
    
    ASTNode* if_node = make_ast_node(AST_IF, NULL, if_token->line);
    
    // Condition
    ASTNode* condition = parse_expression(parser);
    if_node->left = condition;
    
    if (match(parser, TOKEN_THEN)) advance(parser);
    
    // Then block
    ASTNode* then_block = make_ast_node(AST_BLOCK, NULL, if_token->line);
    while (!match(parser, TOKEN_ELSE) && !match(parser, TOKEN_END_IF) && !match(parser, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt) ast_add_child(then_block, stmt);
    }
    if_node->right = then_block;
    
    // Else block (optional)
    if (match(parser, TOKEN_ELSE)) {
        advance(parser);
        ASTNode* else_block = make_ast_node(AST_BLOCK, NULL, if_token->line);
        while (!match(parser, TOKEN_END_IF) && !match(parser, TOKEN_EOF)) {
            ASTNode* stmt = parse_statement(parser);
            if (stmt) ast_add_child(else_block, stmt);
        }
        ast_add_child(if_node, else_block);
    }
    
    if (match(parser, TOKEN_END_IF)) advance(parser);
    return if_node;
}

// Parse while statement
static ASTNode* parse_while(ParserState* parser) {
    Token* while_token = current_token(parser);
    advance(parser); // Skip 'while'
    
    ASTNode* while_node = make_ast_node(AST_WHILE, NULL, while_token->line);
    
    // Condition
    ASTNode* condition = parse_expression(parser);
    while_node->left = condition;
    
    if (match(parser, TOKEN_THEN)) advance(parser);
    
    // Body
    ASTNode* body = make_ast_node(AST_BLOCK, NULL, while_token->line);
    while (!match(parser, TOKEN_END_WHILE) && !match(parser, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt) ast_add_child(body, stmt);
    }
    while_node->right = body;
    
    if (match(parser, TOKEN_END_WHILE)) advance(parser);
    return while_node;
}

// Parse return statement
static ASTNode* parse_return(ParserState* parser) {
    Token* return_token = current_token(parser);
    advance(parser); // Skip 'return'
    
    ASTNode* return_node = make_ast_node(AST_RETURN, NULL, return_token->line);
    
    // Optional return value
    if (!match(parser, TOKEN_END_FUNCTION) && !match(parser, TOKEN_EOF)) {
        return_node->left = parse_expression(parser);
    }
    
    return return_node;
}

// Parse primary expression
static ASTNode* parse_primary(ParserState* parser) {
    Token* token = current_token(parser);
    
    if (match(parser, TOKEN_NUMBER)) {
        advance(parser);
        return make_ast_node(AST_NUMBER, token->value, token->line);
    }
    
    if (match(parser, TOKEN_STRING)) {
        advance(parser);
        return make_ast_node(AST_STRING, token->value, token->line);
    }
    
    if (match(parser, TOKEN_IDENTIFIER)) {
        advance(parser);
        ASTNode* node = make_ast_node(AST_IDENTIFIER, token->value, token->line);
        
        // Function call
        if (match(parser, TOKEN_LPAREN)) {
            ASTNode* call = make_ast_node(AST_CALL, token->value, token->line);
            advance(parser);
            
            while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
                ASTNode* arg = parse_expression(parser);
                if (arg) ast_add_child(call, arg);
                if (match(parser, TOKEN_COMMA)) advance(parser);
            }
            
            if (match(parser, TOKEN_RPAREN)) advance(parser);
            ast_free(node);
            return call;
        }
        
        // Member access
        while (match(parser, TOKEN_DOT)) {
            advance(parser);
            Token* member_token = current_token(parser);
            if (match(parser, TOKEN_IDENTIFIER)) {
                ASTNode* member = make_ast_node(AST_MEMBER_ACCESS, member_token->value, member_token->line);
                member->left = node;
                node = member;
                advance(parser);
                
                // Method call: obj.method()
                if (match(parser, TOKEN_LPAREN)) {
                    ASTNode* call = make_ast_node(AST_CALL, member_token->value, member_token->line);
                    call->left = node->left; // obj part
                    advance(parser);
                    
                    while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
                        ASTNode* arg = parse_expression(parser);
                        if (arg) ast_add_child(call, arg);
                        if (match(parser, TOKEN_COMMA)) advance(parser);
                    }
                    
                    if (match(parser, TOKEN_RPAREN)) advance(parser);
                    return call;
                }
            }
        }
        
        return node;
    }
    
    // list() constructor
    if (match(parser, TOKEN_LIST)) {
        Token* list_token = token;
        advance(parser);
        if (match(parser, TOKEN_LPAREN)) {
            advance(parser);
            if (match(parser, TOKEN_RPAREN)) {
                advance(parser);
                return make_ast_node(AST_LIST_INIT, NULL, list_token->line);
            }
        }
    }
    
    // Parenthesized expression
    if (match(parser, TOKEN_LPAREN)) {
        advance(parser);
        ASTNode* expr = parse_expression(parser);
        if (match(parser, TOKEN_RPAREN)) advance(parser);
        return expr;
    }
    
    return NULL;
}

// Parse binary expression (simple left-to-right, no precedence)
static ASTNode* parse_expression(ParserState* parser) {
    ASTNode* left = parse_primary(parser);
    if (!left) return NULL;
    
    while (1) {
        Token* op_token = current_token(parser);
        TokenType op_type = op_token ? op_token->type : TOKEN_EOF;
        
        if (op_type == TOKEN_PLUS || op_type == TOKEN_MINUS || 
            op_type == TOKEN_STAR || op_type == TOKEN_SLASH ||
            op_type == TOKEN_EQ || op_type == TOKEN_NEQ ||
            op_type == TOKEN_LT || op_type == TOKEN_GT ||
            op_type == TOKEN_LTE || op_type == TOKEN_GTE) {
            
            advance(parser);
            ASTNode* right = parse_primary(parser);
            if (!right) break;
            
            ASTNode* binary = make_ast_node(AST_BINARY_OP, op_token->value, op_token->line);
            binary->left = left;
            binary->right = right;
            left = binary;
        } else {
            break;
        }
    }
    
    return left;
}

// Parse statement
static ASTNode* parse_statement(ParserState* parser) {
    Token* token = current_token(parser);
    if (!token) return NULL;
    
    // Function definition
    if (match(parser, TOKEN_FUNCTION)) {
        return parse_function(parser);
    }
    
    // Struct definition
    if (match(parser, TOKEN_STRUCT)) {
        return parse_struct(parser);
    }
    
    // Variable declaration
    if (match(parser, TOKEN_NUMERIC) || match(parser, TOKEN_STRING_TYPE) || 
        match(parser, TOKEN_BOOLEAN) || match(parser, TOKEN_LIST)) {
        return parse_variable_decl(parser);
    }
    
    // If statement
    if (match(parser, TOKEN_IF)) {
        return parse_if(parser);
    }
    
    // While loop
    if (match(parser, TOKEN_WHILE)) {
        return parse_while(parser);
    }
    
    // Return statement
    if (match(parser, TOKEN_RETURN)) {
        return parse_return(parser);
    }
    
    // Import (skip)
    if (match(parser, TOKEN_IMPORT)) {
        advance(parser);
        if (match(parser, TOKEN_STRING)) advance(parser);
        return NULL; // Skip imports
    }
    
    // Assignment or expression statement
    if (match(parser, TOKEN_IDENTIFIER)) {
        return parse_assignment(parser);
    }
    
    // Unknown, skip
    advance(parser);
    return NULL;
}

// Parse program (entry point)
ASTNode* parse_program(ParserState* parser) {
    ASTNode* program = make_ast_node(AST_PROGRAM, NULL, 1);
    
    while (!match(parser, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt) {
            ast_add_child(program, stmt);
        }
    }
    
    return program;
}
