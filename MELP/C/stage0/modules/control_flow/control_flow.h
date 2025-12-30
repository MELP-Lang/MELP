#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

// Control flow statement types
typedef enum {
    CTRL_IF,
    CTRL_IF_ELSE,
    CTRL_WHILE,
    CTRL_FOR
} ControlFlowType;

// Statement types for body
typedef enum {
    BODY_STMT_VARIABLE,    // Variable declaration
    BODY_STMT_PRINT,       // Print statement
    BODY_STMT_ASSIGNMENT,  // Variable assignment
    BODY_STMT_IF,          // Nested if
    BODY_STMT_WHILE,       // Nested while
    BODY_STMT_RETURN,      // Return statement
    BODY_STMT_BREAK,       // Break/exit
    BODY_STMT_CONTINUE     // Continue
} BodyStatementType;

// Simple statement in body
typedef struct BodyStatement {
    BodyStatementType type;
    char* code;  // Generated C code for this statement
    struct BodyStatement* next;
} BodyStatement;

// If statement structure
typedef struct IfStatement {
    void* condition;           // Comparison or logical expression
    int has_else;
    BodyStatement* then_body;  // Then block statements
    BodyStatement* else_body;  // Else block statements (if has_else)
} IfStatement;

// While statement structure
typedef struct WhileStatement {
    void* condition;
    BodyStatement* body;       // Loop body statements
} WhileStatement;

// For statement structure
typedef struct ForStatement {
    char* iterator;
    void* start;
    void* end;
    BodyStatement* body;       // Loop body statements
} ForStatement;

// Free functions
void if_statement_free(IfStatement* stmt);
void while_statement_free(WhileStatement* stmt);
void for_statement_free(ForStatement* stmt);

#endif // CONTROL_FLOW_H
