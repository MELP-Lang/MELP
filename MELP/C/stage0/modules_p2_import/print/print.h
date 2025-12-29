#ifndef PRINT_H
#define PRINT_H

// Print module - Independent print statement handling

typedef struct Expression Expression;

typedef enum {
    PRINT_STRING_LITERAL,   // "Hello"
    PRINT_VARIABLE,         // x (variable name)
    PRINT_EXPRESSION        // x + 5 (future)
} PrintValueType;

typedef struct {
    PrintValueType type;
    char* value;            // String literal or variable name
} PrintStatement;

// Create and free print statements
PrintStatement* print_statement_create(Expression* value);
void print_statement_free(PrintStatement* stmt);

#endif
