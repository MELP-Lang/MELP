#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>
#include "../lexer/lexer.h"
#include "../codegen_context/sto_types.h"

typedef enum {
    VAR_NUMERIC,
    VAR_STRING,
    VAR_BOOLEAN,
    VAR_POINTER,     // Pointer type (numeric*, string*)
    VAR_ARRAY,       // Array type (numeric[], string[])
    VAR_LIST,        // List type () - heterogeneous, mutable
    VAR_TUPLE        // Tuple type <> - heterogeneous, immutable
} VarType;

// Internal numeric representation (transparent to user)
typedef enum {
    INTERNAL_INT64,      // Small integers: -2^63 to 2^63-1
    INTERNAL_DOUBLE,     // Floating point: ~15 digits precision
    INTERNAL_BIGDECIMAL  // Arbitrary precision (future)
} InternalNumericType;

// Internal string representation (transparent to user)
typedef enum {
    INTERNAL_SSO,        // Small String Optimization: ≤23 bytes inline
    INTERNAL_HEAP,       // Heap allocated: >23 bytes
    INTERNAL_RODATA      // Read-only data section: string literals
} InternalStringType;

// Storage location for variables
typedef enum {
    STORAGE_REGISTER,    // Temporary values in registers
    STORAGE_STACK,       // Local variables on stack
    STORAGE_DATA,        // Global/state variables in .data
    STORAGE_BSS,         // Uninitialized globals in .bss
    STORAGE_HEAP         // Dynamic allocation via malloc
} StorageLocation;

// Variable declaration structure
typedef struct {
    char* name;                          // Variable name
    VarType type;                        // Variable type (numeric, string, boolean, pointer, array)
    char* value;                         // Initial value (string representation)
    InternalNumericType internal_num_type;  // For STO optimization (DEPRECATED - use sto_info)
    InternalStringType internal_str_type;   // For STO optimization (DEPRECATED - use sto_info)
    StorageLocation storage;             // Where variable is stored
    int has_decimal_point;               // Flag for numeric type detection
    void* init_expr;                     // Expression* for complex initialization
    
    // Pointer/Array specific fields
    VarType base_type;                   // Base type for pointer/array (e.g., VAR_NUMERIC for numeric*)
    int is_pointer;                      // 1 if pointer type
    int is_array;                        // 1 if array type
    int array_size;                      // Array size (0 = dynamic)
    
    // ========== YZ_CONST: Const Support ==========
    bool is_const;                       // 1 if constant (immutable)
    
    // ========== Phase 2: STO Integration ==========
    STOTypeInfo* sto_info;               // Full STO analysis result (heap allocated)
    bool sto_analyzed;                   // Has STO analysis been performed?
    bool needs_overflow_check;           // Runtime overflow detection needed?
    
    // YZ_121: For linked list of global consts
    struct VariableDeclaration* next;
} VariableDeclaration;

// Variable assignment structure
typedef struct {
    char* name;                          // Variable name
    void* value_expr;                    // Expression* for value
    
    // ========== Phase 2: STO Integration ==========
    STOTypeInfo* sto_info;               // STO analysis of assigned value (heap allocated)
    bool sto_analyzed;                   // Has STO analysis been performed?
    bool needs_type_promotion;           // Does this need type promotion (int64 -> bigdec)?
    
    // ========== YZ_25: Implicit Declaration ==========
    bool is_implicit_declaration;        // True if declared with `;` (type inference)
} VariableAssignment;

// ========== YZ_15: Array Element Assignment ==========
// Array element assignment: arr[i] = value
// Note: IndexAccess is defined in array/array.h
typedef struct {
    void* index_access;                  // IndexAccess* (forward declaration)
    void* value_expr;                    // Expression* for value to assign
    
    STOTypeInfo* sto_info;               // STO analysis
    bool sto_analyzed;                   // Has STO analysis been performed?
} ArrayAssignment;

#endif
