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
    STMT_SWITCH,            // YZ_89: switch statement
    STMT_RETURN,
    STMT_PRINT,
    STMT_FUNCTION,
    STMT_IMPORT,            // YZ_35: import module_name
    STMT_STRUCT,            // YZ_81: struct definition
    STMT_STRUCT_INSTANCE,   // YZ_82: Point p (struct instance declaration)
    STMT_MEMBER_ASSIGNMENT, // YZ_82: p.x = 10 (struct member assignment)
    STMT_ENUM,              // YZ_96: enum definition
    STMT_ENUM_VARIABLE,     // YZ_101: Color c = Color.Red (enum variable declaration)
    STMT_BLOCK,
    STMT_EXIT,              // YZ_28: exit, exit for, exit while, exit if, exit function
    STMT_EXIT_FOR,          // YZ_28: exit for
    STMT_EXIT_WHILE,        // YZ_28: exit while
    STMT_EXIT_IF,           // YZ_28: exit if
    STMT_EXIT_FUNCTION,     // YZ_28: exit function
    STMT_EXIT_SWITCH,       // YZ_90: exit switch
    STMT_CONTINUE,          // YZ_90: continue (current loop)
    STMT_CONTINUE_FOR,      // YZ_90: continue for
    STMT_CONTINUE_WHILE     // YZ_90: continue while
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
