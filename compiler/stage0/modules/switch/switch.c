#include "switch.h"
#include <stdlib.h>

// Create a new switch case
SwitchCase* switch_case_create(void* value, int is_default) {
    SwitchCase* case_item = (SwitchCase*)malloc(sizeof(SwitchCase));
    case_item->value = value;
    case_item->is_default = is_default;
    case_item->body = NULL;
    case_item->next = NULL;
    return case_item;
}

// Free a switch case
void switch_case_free(SwitchCase* case_item) {
    if (!case_item) return;
    
    // Free the case body statements
    // Note: body is freed by statement_free when the whole statement tree is freed
    
    // Free next cases recursively
    if (case_item->next) {
        switch_case_free(case_item->next);
    }
    
    free(case_item);
}

// Create a new switch statement
SwitchStatement* switch_statement_create(void* expression) {
    SwitchStatement* stmt = (SwitchStatement*)malloc(sizeof(SwitchStatement));
    stmt->expression = expression;
    stmt->cases = NULL;
    stmt->default_case = NULL;
    return stmt;
}

// Free a switch statement
void switch_statement_free(SwitchStatement* stmt) {
    if (!stmt) return;
    
    // Free expression
    // Note: expression is freed by expression_free when needed
    
    // Free all cases
    if (stmt->cases) {
        switch_case_free(stmt->cases);
    }
    
    free(stmt);
}

// Add a case to switch statement
void switch_statement_add_case(SwitchStatement* stmt, SwitchCase* case_item) {
    if (!stmt || !case_item) return;
    
    // If this is a default case, store reference
    if (case_item->is_default) {
        stmt->default_case = case_item;
    }
    
    // Add to linked list
    if (!stmt->cases) {
        stmt->cases = case_item;
    } else {
        // Append to end
        SwitchCase* current = stmt->cases;
        while (current->next) {
            current = current->next;
        }
        current->next = case_item;
    }
}
