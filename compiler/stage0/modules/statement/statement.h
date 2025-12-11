#ifndef STATEMENT_H
#define STATEMENT_H

#include "../expression/expression.h"

// Statement types
typedef enum {
    STMT_EXPRESSION,
    STMT_VARIABLE_DECL,
    STMT_ASSIGNMENT,
    STMT_ARRAY_ASSIGNMENT,  // YZ_15: arr[i] = value
    STMT_IF,
    STMT_WHILE,
    STMT_FOR,
    STMT_RETURN,
    STMT_PRINT,
    STMT_FUNCTION,
    STMT_IMPORT,            // YZ_35: import module_name
    STMT_BLOCK,
    STMT_EXIT,              // YZ_28: exit, exit for, exit while, exit if, exit function
    STMT_EXIT_FOR,          // YZ_28: exit for
    STMT_EXIT_WHILE,        // YZ_28: exit while
    STMT_EXIT_IF,           // YZ_28: exit if
    STMT_EXIT_FUNCTION      // YZ_28: exit function
} StatementType;

// Statement structure (simplified)
typedef struct Statement {
    StatementType type;
    void* data;  // Points to specific statement data (IfStatement, WhileStatement, etc.)
    struct Statement* next;  // Linked list for statement sequences (bodies, blocks)
} Statement;

// Statement management
Statement* statement_create(StatementType type);
void statement_free(Statement* stmt);

#endif
