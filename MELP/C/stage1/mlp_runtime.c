#include "mlp_runtime.h"
#include <stdlib.h>
#include <string.h>

// Create new list
mlp_list_t* mlp_list_create() {
    mlp_list_t* list = malloc(sizeof(mlp_list_t));
    list->capacity = 16;
    list->size = 0;
    list->data = malloc(sizeof(void*) * list->capacity);
    return list;
}

// Free list
void mlp_list_free(mlp_list_t* list) {
    if (list) {
        if (list->data) free(list->data);
        free(list);
    }
}

// Append item to list
void mlp_list_append(mlp_list_t* list, void* item) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(void*) * list->capacity);
    }
    list->data[list->size++] = item;
}

// Get list length
int mlp_list_length(mlp_list_t* list) {
    return list->size;
}

// Get item at index
void* mlp_list_get(mlp_list_t* list, int index) {
    if (index < 0 || index >= list->size) return NULL;
    return list->data[index];
}
