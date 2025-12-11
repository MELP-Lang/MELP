#include "statement.h"
#include "../import/import.h"  // YZ_35: Import statement
#include <stdlib.h>

Statement* statement_create(StatementType type) {
    Statement* stmt = malloc(sizeof(Statement));
    stmt->type = type;
    stmt->data = NULL;
    stmt->next = NULL;  // Initialize linked list pointer
    return stmt;
}

void statement_free(Statement* stmt) {
    if (!stmt) return;
    
    // Free import statement data if needed
    if (stmt->type == STMT_IMPORT && stmt->data) {
        import_statement_free((ImportStatement*)stmt->data);
    }
    
    // Data freed by specific statement modules
    free(stmt);
}
