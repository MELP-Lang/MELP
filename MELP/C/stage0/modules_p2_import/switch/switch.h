#ifndef SWITCH_H
#define SWITCH_H

// Forward declaration for Statement (to avoid circular dependency)
typedef struct Statement Statement;

// Switch case structure
typedef struct SwitchCase {
    void* value;                // Expression for case value (NULL for default)
    int is_default;             // 1 if this is default case, 0 otherwise
    Statement* body;            // Statements to execute for this case
    struct SwitchCase* next;    // Next case in the list
} SwitchCase;

// Switch statement structure
typedef struct SwitchStatement {
    void* expression;           // Expression to switch on (must evaluate to numeric)
    SwitchCase* cases;          // Linked list of cases
    SwitchCase* default_case;   // Pointer to default case (NULL if none)
} SwitchStatement;

// Case management
SwitchCase* switch_case_create(void* value, int is_default);
void switch_case_free(SwitchCase* case_item);

// Switch statement management
SwitchStatement* switch_statement_create(void* expression);
void switch_statement_free(SwitchStatement* stmt);
void switch_statement_add_case(SwitchStatement* stmt, SwitchCase* case_item);

#endif // SWITCH_H
