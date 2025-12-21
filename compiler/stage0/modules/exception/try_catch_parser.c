#include "try_catch_parser.h"
#include "../parser_core/parser.h"
#include "../statement/statement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern Statement* parse_statement(Token** tokens, int* pos);

// ============================================================================
// Try Block Parser
// ============================================================================

TryBlock* parse_try_block(Token** tokens, int* pos) {
    // Expect: try
    if (tokens[*pos]->type != TOKEN_TRY) {
        fprintf(stderr, "Error: Expected 'try' keyword at line %d\n", 
                tokens[*pos]->line);
        return NULL;
    }
    (*pos)++; // Skip 'try'
    
    TryBlock* try_block = try_block_create();
    if (!try_block) return NULL;
    
    // Parse try body (until catch or finally or end_try)
    int body_capacity = 10;
    try_block->body = (Statement**)malloc(sizeof(Statement*) * body_capacity);
    try_block->body_count = 0;
    
    while (tokens[*pos]->type != TOKEN_CATCH && 
           tokens[*pos]->type != TOKEN_FINALLY && 
           tokens[*pos]->type != TOKEN_END_TRY &&
           tokens[*pos]->type != TOKEN_EOF) {
        
        Statement* stmt = parse_statement(tokens, pos);
        if (!stmt) {
            try_block_free(try_block);
            return NULL;
        }
        
        // Expand array if needed
        if (try_block->body_count >= body_capacity) {
            body_capacity *= 2;
            try_block->body = (Statement**)realloc(try_block->body, 
                                                   sizeof(Statement*) * body_capacity);
        }
        
        try_block->body[try_block->body_count++] = stmt;
    }
    
    // Parse catch clauses (optional, can be multiple)
    CatchClause* last_catch = NULL;
    while (tokens[*pos]->type == TOKEN_CATCH) {
        CatchClause* clause = parse_catch_clause(tokens, pos);
        if (!clause) {
            try_block_free(try_block);
            return NULL;
        }
        
        if (!try_block->catch_clauses) {
            try_block->catch_clauses = clause;
            last_catch = clause;
        } else {
            last_catch->next = clause;
            last_catch = clause;
        }
    }
    
    // Parse finally block (optional)
    if (tokens[*pos]->type == TOKEN_FINALLY) {
        try_block->finally_block = parse_finally_block(tokens, pos);
        if (!try_block->finally_block) {
            try_block_free(try_block);
            return NULL;
        }
    }
    
    // Expect: end_try
    if (tokens[*pos]->type != TOKEN_END_TRY) {
        fprintf(stderr, "Error: Expected 'end_try' at line %d\n", 
                tokens[*pos]->line);
        try_block_free(try_block);
        return NULL;
    }
    (*pos)++; // Skip 'end_try'
    
    // Validate: Must have at least one catch or finally
    if (!try_block->catch_clauses && !try_block->finally_block) {
        fprintf(stderr, "Error: Try block must have at least one catch or finally clause\n");
        try_block_free(try_block);
        return NULL;
    }
    
    return try_block;
}

// ============================================================================
// Catch Clause Parser
// ============================================================================

CatchClause* parse_catch_clause(Token** tokens, int* pos) {
    // Expect: catch
    if (tokens[*pos]->type != TOKEN_CATCH) {
        fprintf(stderr, "Error: Expected 'catch' keyword\n");
        return NULL;
    }
    (*pos)++; // Skip 'catch'
    
    // Expect: identifier (exception variable)
    if (tokens[*pos]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected exception variable name after 'catch'\n");
        return NULL;
    }
    char* exception_var = strdup(tokens[*pos]->value);
    (*pos)++;
    
    // Expect: as
    if (tokens[*pos]->type != TOKEN_AS) {
        fprintf(stderr, "Error: Expected 'as' keyword in catch clause\n");
        free(exception_var);
        return NULL;
    }
    (*pos)++; // Skip 'as'
    
    // Expect: identifier (exception type)
    if (tokens[*pos]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected exception type name after 'as'\n");
        free(exception_var);
        return NULL;
    }
    char* exception_type = strdup(tokens[*pos]->value);
    (*pos)++;
    
    CatchClause* clause = catch_clause_create(exception_var, exception_type);
    free(exception_var);
    free(exception_type);
    
    if (!clause) return NULL;
    
    // Parse catch body (until catch or finally or end_try)
    int body_capacity = 10;
    clause->body = (Statement**)malloc(sizeof(Statement*) * body_capacity);
    clause->body_count = 0;
    
    while (tokens[*pos]->type != TOKEN_CATCH && 
           tokens[*pos]->type != TOKEN_FINALLY && 
           tokens[*pos]->type != TOKEN_END_TRY &&
           tokens[*pos]->type != TOKEN_EOF) {
        
        Statement* stmt = parse_statement(tokens, pos);
        if (!stmt) {
            catch_clause_free(clause);
            return NULL;
        }
        
        // Expand array if needed
        if (clause->body_count >= body_capacity) {
            body_capacity *= 2;
            clause->body = (Statement**)realloc(clause->body, 
                                               sizeof(Statement*) * body_capacity);
        }
        
        clause->body[clause->body_count++] = stmt;
    }
    
    return clause;
}

// ============================================================================
// Finally Block Parser
// ============================================================================

FinallyBlock* parse_finally_block(Token** tokens, int* pos) {
    // Expect: finally
    if (tokens[*pos]->type != TOKEN_FINALLY) {
        fprintf(stderr, "Error: Expected 'finally' keyword\n");
        return NULL;
    }
    (*pos)++; // Skip 'finally'
    
    FinallyBlock* finally_block = finally_block_create();
    if (!finally_block) return NULL;
    
    // Parse finally body (until end_try)
    int body_capacity = 10;
    finally_block->body = (Statement**)malloc(sizeof(Statement*) * body_capacity);
    finally_block->body_count = 0;
    
    while (tokens[*pos]->type != TOKEN_END_TRY &&
           tokens[*pos]->type != TOKEN_EOF) {
        
        Statement* stmt = parse_statement(tokens, pos);
        if (!stmt) {
            finally_block_free(finally_block);
            return NULL;
        }
        
        // Expand array if needed
        if (finally_block->body_count >= body_capacity) {
            body_capacity *= 2;
            finally_block->body = (Statement**)realloc(finally_block->body, 
                                                      sizeof(Statement*) * body_capacity);
        }
        
        finally_block->body[finally_block->body_count++] = stmt;
    }
    
    return finally_block;
}
