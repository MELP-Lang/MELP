#ifndef STAGE1_PARSER_H
#define STAGE1_PARSER_H

#include "lexer.h"

// AST Node types
typedef enum {
    AST_PROGRAM = 1,
    AST_FUNCTION = 2,
    AST_STRUCT = 3,
    AST_VARIABLE_DECL = 4,
    AST_ASSIGN = 5,
    AST_IF = 6,
    AST_WHILE = 7,
    AST_RETURN = 8,
    AST_CALL = 9,
    AST_BINARY_OP = 10,
    AST_IDENTIFIER = 11,
    AST_NUMBER = 12,
    AST_STRING = 13,
    AST_MEMBER_ACCESS = 14,
    AST_IMPORT = 15,
    AST_LIST_INIT = 16,
    AST_BLOCK = 17
} ASTNodeType;

// AST Node structure (using linked list for children)
typedef struct ASTNode {
    ASTNodeType type;
    char* value;
    int line;
    
    // For binary ops
    struct ASTNode* left;
    struct ASTNode* right;
    
    // For blocks/functions (linked list of children)
    struct ASTNode* first_child;
    struct ASTNode* next_sibling;
    
    // For typed nodes
    char* data_type;
} ASTNode;

// Parser state
typedef struct {
    Token** tokens;
    int token_count;
    int current;
    int has_error;
    char* error_message;
} ParserState;

// Function prototypes
ParserState* parser_create(Token** tokens, int count);
void parser_free(ParserState* parser);
ASTNode* parse_program(ParserState* parser);
ASTNode* make_ast_node(ASTNodeType type, const char* value, int line);
void ast_free(ASTNode* node);
void ast_add_child(ASTNode* parent, ASTNode* child);

#endif // STAGE1_PARSER_H
