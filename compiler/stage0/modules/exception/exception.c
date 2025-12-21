#include "exception.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// TryBlock Management
// ============================================================================

TryBlock* try_block_create(void) {
    TryBlock* try_block = (TryBlock*)malloc(sizeof(TryBlock));
    if (!try_block) {
        fprintf(stderr, "Error: Failed to allocate TryBlock\n");
        return NULL;
    }
    
    try_block->body = NULL;
    try_block->body_count = 0;
    try_block->catch_clauses = NULL;
    try_block->finally_block = NULL;
    
    return try_block;
}

void try_block_free(TryBlock* try_block) {
    if (!try_block) return;
    
    // Free try body statements
    if (try_block->body) {
        for (int i = 0; i < try_block->body_count; i++) {
            // Note: Statements freed by statement manager
        }
        free(try_block->body);
    }
    
    // Free catch clauses
    CatchClause* clause = try_block->catch_clauses;
    while (clause) {
        CatchClause* next = clause->next;
        catch_clause_free(clause);
        clause = next;
    }
    
    // Free finally block
    if (try_block->finally_block) {
        finally_block_free(try_block->finally_block);
    }
    
    free(try_block);
}

// ============================================================================
// CatchClause Management
// ============================================================================

CatchClause* catch_clause_create(const char* var, const char* type) {
    CatchClause* clause = (CatchClause*)malloc(sizeof(CatchClause));
    if (!clause) {
        fprintf(stderr, "Error: Failed to allocate CatchClause\n");
        return NULL;
    }
    
    clause->exception_var = var ? strdup(var) : NULL;
    clause->exception_type = type ? strdup(type) : NULL;
    clause->body = NULL;
    clause->body_count = 0;
    clause->next = NULL;
    
    return clause;
}

void catch_clause_free(CatchClause* clause) {
    if (!clause) return;
    
    if (clause->exception_var) free(clause->exception_var);
    if (clause->exception_type) free(clause->exception_type);
    
    if (clause->body) {
        for (int i = 0; i < clause->body_count; i++) {
            // Note: Statements freed by statement manager
        }
        free(clause->body);
    }
    
    // Don't free next - caller handles linked list
    free(clause);
}

// ============================================================================
// FinallyBlock Management
// ============================================================================

FinallyBlock* finally_block_create(void) {
    FinallyBlock* finally_block = (FinallyBlock*)malloc(sizeof(FinallyBlock));
    if (!finally_block) {
        fprintf(stderr, "Error: Failed to allocate FinallyBlock\n");
        return NULL;
    }
    
    finally_block->body = NULL;
    finally_block->body_count = 0;
    
    return finally_block;
}

void finally_block_free(FinallyBlock* finally_block) {
    if (!finally_block) return;
    
    if (finally_block->body) {
        for (int i = 0; i < finally_block->body_count; i++) {
            // Note: Statements freed by statement manager
        }
        free(finally_block->body);
    }
    
    free(finally_block);
}

// ============================================================================
// ThrowStatement Management
// ============================================================================

ThrowStatement* throw_statement_create(Expression* expr) {
    ThrowStatement* throw_stmt = (ThrowStatement*)malloc(sizeof(ThrowStatement));
    if (!throw_stmt) {
        fprintf(stderr, "Error: Failed to allocate ThrowStatement\n");
        return NULL;
    }
    
    throw_stmt->exception_expr = expr;
    
    return throw_stmt;
}

void throw_statement_free(ThrowStatement* throw_stmt) {
    if (!throw_stmt) return;
    
    // Note: Expression freed by expression manager
    
    free(throw_stmt);
}

// ============================================================================
// Exception Runtime Support
// ============================================================================

Exception* exception_create(ExceptionType type, const char* message) {
    Exception* exception = (Exception*)malloc(sizeof(Exception));
    if (!exception) {
        fprintf(stderr, "Error: Failed to allocate Exception\n");
        return NULL;
    }
    
    exception->type = type;
    exception->message = message ? strdup(message) : NULL;
    exception->stack_trace = NULL;  // Filled by runtime
    exception->cause = NULL;
    
    return exception;
}

void exception_free(Exception* exception) {
    if (!exception) return;
    
    if (exception->message) free(exception->message);
    if (exception->stack_trace) free(exception->stack_trace);
    if (exception->cause) exception_free(exception->cause);
    
    free(exception);
}

const char* exception_type_to_string(ExceptionType type) {
    switch (type) {
        case EXCEPTION_BASE:
            return "Exception";
        case EXCEPTION_DIVISION_BY_ZERO:
            return "DivisionByZeroException";
        case EXCEPTION_NULL_POINTER:
            return "NullPointerException";
        case EXCEPTION_INDEX_OUT_OF_BOUNDS:
            return "IndexOutOfBoundsException";
        case EXCEPTION_TYPE_MISMATCH:
            return "TypeMismatchException";
        case EXCEPTION_FILE_NOT_FOUND:
            return "FileNotFoundException";
        case EXCEPTION_CUSTOM:
            return "CustomException";
        default:
            return "UnknownException";
    }
}
