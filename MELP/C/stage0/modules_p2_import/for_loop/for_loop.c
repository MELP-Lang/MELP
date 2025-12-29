#include "for_loop.h"
#include "../statement/statement.h"
#include <stdlib.h>

void for_loop_free(ForLoop* loop) {
    if (!loop) return;
    
    if (loop->var_name) {
        free(loop->var_name);
    }
    
    // YZ_28: Free collection name for for-each loops
    if (loop->collection_name) {
        free(loop->collection_name);
    }
    
    // Free body statements
    if (loop->body) {
        Statement* current = loop->body;
        while (current) {
            Statement* next = current->next;
            statement_free(current);
            current = next;
        }
    }
    
    free(loop);
}
