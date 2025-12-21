#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "../statement/statement.h"
#include "../expression/expression.h"

// Forward declarations
typedef struct TryBlock TryBlock;
typedef struct CatchClause CatchClause;
typedef struct FinallyBlock FinallyBlock;
typedef struct ThrowStatement ThrowStatement;

// Exception type enumeration
typedef enum {
    EXCEPTION_BASE,
    EXCEPTION_DIVISION_BY_ZERO,
    EXCEPTION_NULL_POINTER,
    EXCEPTION_INDEX_OUT_OF_BOUNDS,
    EXCEPTION_TYPE_MISMATCH,
    EXCEPTION_FILE_NOT_FOUND,
    EXCEPTION_CUSTOM
} ExceptionType;

// Exception struct (runtime representation)
typedef struct Exception {
    ExceptionType type;
    char* message;
    char* stack_trace;
    struct Exception* cause;  // Nested exception
} Exception;

// Catch clause structure
struct CatchClause {
    char* exception_var;      // Variable name (e.g., "e")
    char* exception_type;     // Type name (e.g., "DivisionException")
    Statement** body;         // Statements in catch block
    int body_count;
    struct CatchClause* next; // Linked list
};

// Finally block structure
struct FinallyBlock {
    Statement** body;         // Statements in finally block
    int body_count;
};

// Try block structure
struct TryBlock {
    Statement** body;         // Statements in try block
    int body_count;
    CatchClause* catch_clauses; // Linked list of catch clauses
    FinallyBlock* finally_block; // Optional finally block
};

// Throw statement structure
struct ThrowStatement {
    Expression* exception_expr; // Exception expression to throw
};

// Memory management
TryBlock* try_block_create(void);
void try_block_free(TryBlock* try_block);

CatchClause* catch_clause_create(const char* var, const char* type);
void catch_clause_free(CatchClause* clause);

FinallyBlock* finally_block_create(void);
void finally_block_free(FinallyBlock* finally_block);

ThrowStatement* throw_statement_create(Expression* expr);
void throw_statement_free(ThrowStatement* throw_stmt);

// Exception utilities
Exception* exception_create(ExceptionType type, const char* message);
void exception_free(Exception* exception);
const char* exception_type_to_string(ExceptionType type);

#endif // EXCEPTION_H
