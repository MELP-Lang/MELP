#ifndef MLP_RUNTIME_H
#define MLP_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>

// MLP list implementation (dynamic array)
typedef struct {
    void** data;
    int size;
    int capacity;
} mlp_list_t;

// List operations
mlp_list_t* mlp_list_create();
void mlp_list_free(mlp_list_t* list);
void mlp_list_append(mlp_list_t* list, void* item);
int mlp_list_length(mlp_list_t* list);
void* mlp_list_get(mlp_list_t* list, int index);

#endif // MLP_RUNTIME_H
