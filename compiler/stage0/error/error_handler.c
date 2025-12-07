#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

void error_unexpected_token(Token* token, const char* expected) {
    fprintf(stderr, "\n❌ SYNTAX ERROR at line %d:\n", token->line);
    fprintf(stderr, "   Unexpected token: '%s' (type: %d)\n", 
            token->value ? token->value : "<unknown>", token->type);
    if (expected) {
        fprintf(stderr, "   Expected: %s\n", expected);
    }
    exit(1);
}

void error_unexpected_operator(Token* token) {
    fprintf(stderr, "\n❌ SYNTAX ERROR at line %d:\n", token->line);
    fprintf(stderr, "   Unexpected operator: '%s'\n", token->value);
    fprintf(stderr, "   Operators cannot appear alone at top level\n");
    fprintf(stderr, "   Hint: Use operators only in expressions\n");
    exit(1);
}

void error_unexpected_literal(Token* token, const char* type) {
    fprintf(stderr, "\n❌ SYNTAX ERROR at line %d:\n", token->line);
    fprintf(stderr, "   Unexpected %s literal: %s\n", type, 
            token->value ? token->value : "<value>");
    fprintf(stderr, "   Literals cannot appear alone at top level\n");
    fprintf(stderr, "   Hint: Use in assignment or function call\n");
    exit(1);
}

void error_invalid_statement(Token* token) {
    fprintf(stderr, "\n❌ SYNTAX ERROR at line %d:\n", token->line);
    fprintf(stderr, "   Invalid statement starting with: '%s'\n", 
            token->value ? token->value : "<token>");
    fprintf(stderr, "   Valid statements: print(...), numeric x = ..., -- comment\n");
    exit(1);
}
