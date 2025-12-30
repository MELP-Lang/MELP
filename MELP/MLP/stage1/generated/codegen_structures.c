// MLP-GCC Generated C Code
// Target: GCC C99
// STO: Smart Type Optimization enabled

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// STO Runtime types
typedef struct { /* bigdecimal */ } mlp_bigdecimal_t;
typedef struct { char* data; size_t len; } mlp_string_t;

typedef struct {
    const char* field_name;
    const char* field_type;
    int64_t offset;
    Boolean is_method;
    const char* method_label;
} StructFieldMetadata;

typedef struct {
    const char* struct_name;
    void* fields;  /* list */
    int64_t field_count;
    int64_t total_size;
} StructMetadata;

void register_struct_metadata() {
    /* function body */
}

void find_struct_metadata() {
    /* function body */
}

void find_field_in_struct() {
    /* function body */
}

void visit_list_literal() {
    /* function body */
}

void visit_array_access() {
    /* function body */
}

void visit_hashmap_get() {
    /* function body */
}

void visit_struct_instantiation() {
    /* function body */
}

void visit_member_access() {
    /* function body */
}

