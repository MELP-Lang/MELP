#ifndef ARRAY_H
#define ARRAY_H

#include "../variable/variable.h"

// Collection types
typedef enum {
    COLL_ARRAY,      // [] - Homogeneous, mutable, stack/heap
    COLL_LIST,       // () - Heterogeneous, mutable, heap
    COLL_TUPLE,      // <> - Heterogeneous, immutable, stack
    COLL_MAP         // {} - Key-value pairs, mutable, heap (YZ_201)
} CollectionType;

// Array: Fixed/dynamic size, homogeneous elements
typedef struct {
    VarType element_type;    // Type of elements (all same type)
    int capacity;            // Allocated capacity
    int length;              // Current number of elements
    void** elements;         // Array of element pointers
    int is_dynamic;          // 0 = fixed size (stack), 1 = dynamic (heap)
} Array;

// List: Dynamic size, heterogeneous elements (always heap)
typedef struct {
    int capacity;            // Allocated capacity
    int length;              // Current number of elements
    void** elements;         // Array of element pointers (mixed types)
    VarType* element_types;  // Type of each element
} List;

// Tuple: Fixed size, heterogeneous, immutable (stack only)
typedef struct {
    int length;              // Number of elements (fixed at creation)
    void** elements;         // Array of element pointers
    VarType* element_types;  // Type of each element
} Tuple;

// Map: Key-value pairs, dynamic size (heap only) - YZ_201
typedef struct {
    int capacity;            // Allocated capacity
    int length;              // Current number of key-value pairs
    char** keys;             // Array of string keys
    void** values;           // Array of value pointers (mixed types)
    VarType* value_types;    // Type of each value
} Map;

// Generic collection wrapper
typedef struct {
    CollectionType type;
    union {
        Array array;
        List list;
        Tuple tuple;
        Map map;             // YZ_201: Map literal
    } data;
} Collection;

// Index access: arr[i] or list(i) or tuple[i]
typedef struct {
    char* collection_name;   // Name of array/list/tuple variable
    int index_type;          // 0 = constant, 1 = variable, 2 = expression
    union {
        int const_index;     // Constant index value
        char* var_index;     // Variable name for index
        void* expr_index;    // Expression for index (cast to Expression*)
    } index;
    int is_list_access;      // 0 = array [], 1 = list ()
    int is_tuple_access;     // YZ_21: 1 if tuple[i]
} IndexAccess;

#endif
