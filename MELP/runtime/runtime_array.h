#ifndef RUNTIME_ARRAY_H
#define RUNTIME_ARRAY_H
#include <stdint.h>

typedef struct {
    void** data;
    int size;
    int capacity;
    int element_size;
} List;

List* list_create(void);
void list_free(List* list);
void list_add(List* list, void* element);
void* list_get(List* list, int index);
void list_remove(List* list, int index);
int list_size(List* list);
void list_clear(List* list);

// MLP_List type for compatibility
typedef struct {
    void* data;
    int64_t length;
    int64_t capacity;
    int64_t element_size;
} MLP_List;

MLP_List* mlp_list_create(int64_t element_size);
void mlp_list_add(MLP_List* list, void* element);
void* mlp_list_get(MLP_List* list, int64_t index);
int64_t mlp_list_length(MLP_List* list);
void mlp_list_free(MLP_List* list);
MLP_List* mlp_get_args_list(void);

#endif
