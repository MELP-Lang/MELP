#include "control_flow.h"
#include <stdlib.h>

static void body_statement_free(BodyStatement* stmt) {
    while (stmt) {
        BodyStatement* next = stmt->next;
        if (stmt->code) free(stmt->code);
        free(stmt);
        stmt = next;
    }
}

void if_statement_free(IfStatement* stmt) {
    if (!stmt) return;
    body_statement_free(stmt->then_body);
    body_statement_free(stmt->else_body);
    free(stmt);
}

void while_statement_free(WhileStatement* stmt) {
    if (!stmt) return;
    body_statement_free(stmt->body);
    free(stmt);
}

void for_statement_free(ForStatement* stmt) {
    if (!stmt) return;
    if (stmt->iterator) free(stmt->iterator);
    body_statement_free(stmt->body);
    free(stmt);
}
