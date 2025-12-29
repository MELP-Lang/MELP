// Find a function by name in a linked list of FunctionDeclaration
#include "functions.h"
#include <string.h>

FunctionDeclaration* find_function_by_name(FunctionDeclaration* head, const char* name) {
    for (FunctionDeclaration* f = head; f; f = f->next) {
        if (strcmp(f->name, name) == 0) return f;
    }
    return NULL;
}
