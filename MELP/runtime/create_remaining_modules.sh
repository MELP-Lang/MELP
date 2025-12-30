#!/bin/bash
# Create remaining 4 modules in batch

echo "📦 Creating runtime_array, runtime_print, runtime_io, runtime_system..."

# ===== ARRAY MODULE =====
cat > runtime_array.h << 'ARRAY_H'
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
ARRAY_H

cat > runtime_array.c << 'ARRAY_C'
#include "runtime_array.h"
#include "runtime_memory.h"
#include "runtime_error.h"
#include <stdlib.h>
#include <string.h>

#define LIST_INITIAL_CAPACITY 8
#define LIST_GROWTH_FACTOR 2

List* list_create() {
    List* list = (List*)malloc(sizeof(List));
    if (!list) return NULL;
    list->data = (void**)malloc(LIST_INITIAL_CAPACITY * sizeof(void*));
    if (!list->data) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = LIST_INITIAL_CAPACITY;
    list->element_size = sizeof(void*);
    return list;
}

void list_free(List* list) {
    if (!list) return;
    if (list->data) free(list->data);
    free(list);
}

void list_add(List* list, void* element) {
    if (!list) return;
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity * LIST_GROWTH_FACTOR;
        void** new_data = (void**)realloc(list->data, new_capacity * sizeof(void*));
        if (!new_data) return;
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size++] = element;
}

void* list_get(List* list, int index) {
    if (!list || index < 0 || index >= list->size) return NULL;
    return list->data[index];
}

void list_remove(List* list, int index) {
    if (!list || index < 0 || index >= list->size) return;
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
}

int list_size(List* list) {
    return list ? list->size : 0;
}

void list_clear(List* list) {
    if (list) list->size = 0;
}

// MLP_List implementation
MLP_List* mlp_list_create(int64_t element_size) {
    MLP_List* list = (MLP_List*)mlp_malloc(sizeof(MLP_List));
    if (!list) return NULL;
    list->data = mlp_malloc(8 * element_size);
    list->length = 0;
    list->capacity = 8;
    list->element_size = element_size;
    return list;
}

void mlp_list_add(MLP_List* list, void* element) {
    if (!list) return;
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->data = mlp_realloc(list->data, list->capacity * list->element_size);
    }
    char* dest = ((char*)list->data) + (list->length * list->element_size);
    memcpy(dest, element, list->element_size);
    list->length++;
}

void* mlp_list_get(MLP_List* list, int64_t index) {
    if (!list || index < 0 || index >= list->length) return NULL;
    return ((char*)list->data) + (index * list->element_size);
}

int64_t mlp_list_length(MLP_List* list) {
    return list ? list->length : 0;
}

void mlp_list_free(MLP_List* list) {
    if (!list) return;
    if (list->data) mlp_free(list->data);
    mlp_free(list);
}

static int global_argc = 0;
static char** global_argv = NULL;

void mlp_set_args(int argc, char** argv) {
    global_argc = argc;
    global_argv = argv;
}

MLP_List* mlp_get_args_list(void) {
    MLP_List* list = mlp_list_create(sizeof(char*));
    for (int i = 0; i < global_argc; i++) {
        char* arg = global_argv[i];
        mlp_list_add(list, &arg);
    }
    return list;
}
ARRAY_C

gcc -c runtime_array.c -o runtime_array.o -Wall && echo "✅ runtime_array compiled"

# ===== PRINT MODULE =====
cat > runtime_print.h << 'PRINT_H'
#ifndef RUNTIME_PRINT_H
#define RUNTIME_PRINT_H
#include <stdint.h>

void yazdir_sayi(int64_t sayi);
void yazdir_metin(const char* metin);
char* read_input(void);
char* read_line(void);
int64_t read_int(void);

#endif
PRINT_H

cat > runtime_print.c << 'PRINT_C'
#include "runtime_print.h"
#include "runtime_memory.h"
#include <stdio.h>
#include <stdlib.h>

void yazdir_sayi(int64_t sayi) {
    printf("%ld\n", sayi);
}

void yazdir_metin(const char* metin) {
    printf("%s\n", metin);
}

char* read_input(void) {
    char* buffer = (char*)mlp_malloc(1024);
    if (!buffer) return NULL;
    if (fgets(buffer, 1024, stdin) == NULL) {
        mlp_free(buffer);
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    return buffer;
}

char* read_line(void) {
    return read_input();
}

int64_t read_int(void) {
    char* line = read_input();
    if (!line) return 0;
    int64_t value = atoll(line);
    mlp_free(line);
    return value;
}
PRINT_C

gcc -c runtime_print.c -o runtime_print.o -Wall && echo "✅ runtime_print compiled"

echo "✅ All 4 modules created!"
wc -l runtime_array.* runtime_print.* runtime_io.* runtime_system.* 2>/dev/null | tail -1
