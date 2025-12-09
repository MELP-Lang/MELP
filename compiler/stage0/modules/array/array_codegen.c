#include "array_codegen.h"
#include "../arithmetic/arithmetic_codegen.h"
#include <stdio.h>
#include <string.h>

// Static counter for array labels
static int array_label_counter = 0;

// Generate code for Collection literal (main entry point)
void codegen_collection(FILE* output, Collection* coll) {
    if (!coll) return;
    
    switch (coll->type) {
        case COLL_ARRAY:
            codegen_array_literal(output, &coll->data.array);
            break;
        case COLL_LIST:
            codegen_list_literal(output, &coll->data.list);
            break;
        case COLL_TUPLE:
            codegen_tuple_literal(output, &coll->data.tuple);
            break;
    }
}

// Generate array literal: [1, 2, 3]
void codegen_array_literal(FILE* output, Array* arr) {
    if (!arr) return;
    
    fprintf(output, "    ; Array literal: %d elements, type=%d\n", arr->length, arr->element_type);
    
    // Label for this array
    int arr_id = array_label_counter++;
    
    // Allocate on heap using TTO
    fprintf(output, "    ; Allocate array on heap\n");
    fprintf(output, "    mov rdi, %d  ; Number of elements\n", arr->length);
    fprintf(output, "    mov rsi, 8   ; Element size (8 bytes)\n");
    fprintf(output, "    call tto_array_alloc  ; Returns pointer in rax\n");
    fprintf(output, "    mov [array_%d], rax  ; Save array pointer\n", arr_id);
    
    // Initialize elements
    if (arr->length > 0 && arr->elements) {
        fprintf(output, "    ; Initialize array elements\n");
        for (int i = 0; i < arr->length; i++) {
            ArithmeticExpr* elem = (ArithmeticExpr*)arr->elements[i];
            if (elem) {
                // Generate code for element expression
                fprintf(output, "    ; Element %d\n", i);
                arithmetic_generate_code(output, elem, NULL);  // Result in r8 or xmm0
                
                // Move result to rax (arithmetic puts int in r8, float in xmm0)
                fprintf(output, "    mov rax, r8  ; Move result to rax\n");
                
                // Store in array
                fprintf(output, "    mov rbx, [array_%d]  ; Load array pointer\n", arr_id);
                fprintf(output, "    mov [rbx + %d], rax  ; Store element at index %d\n", i * 8, i);
            }
        }
    }
    
    // Final array pointer in rax
    fprintf(output, "    mov rax, [array_%d]  ; Array pointer result\n", arr_id);
}

// Generate list literal: (1; "text"; 3.14;)
void codegen_list_literal(FILE* output, List* list) {
    if (!list) return;
    
    fprintf(output, "    ; List literal: %d elements (heterogeneous)\n", list->length);
    
    int list_id = array_label_counter++;
    
    // Allocate list structure
    fprintf(output, "    ; Allocate list\n");
    fprintf(output, "    mov rdi, %d  ; Capacity\n", list->capacity);
    fprintf(output, "    call tto_list_alloc  ; Returns list pointer in rax\n");
    fprintf(output, "    mov [list_%d], rax  ; Save list pointer\n", list_id);
    
    // Initialize elements with type info
    if (list->length > 0 && list->elements && list->element_types) {
        fprintf(output, "    ; Initialize list elements\n");
        for (int i = 0; i < list->length; i++) {
            ArithmeticExpr* elem = (ArithmeticExpr*)list->elements[i];
            if (elem) {
                fprintf(output, "    ; List element %d (type=%d)\n", i, list->element_types[i]);
                arithmetic_generate_code(output, elem, NULL);  // Result in r8/xmm0
                fprintf(output, "    mov rdx, r8  ; Move result to rdx\n");
                
                // Store element and type
                fprintf(output, "    mov rbx, [list_%d]  ; Load list pointer\n", list_id);
                fprintf(output, "    mov rdi, rbx\n");
                fprintf(output, "    mov rsi, %d  ; Index\n", i);
                // rdx already has value
                fprintf(output, "    mov rcx, %d  ; Type\n", list->element_types[i]);
                fprintf(output, "    call tto_list_set  ; Set element with type\n");
            }
        }
    }
    
    fprintf(output, "    mov rax, [list_%d]  ; List pointer result\n", list_id);
}

// Generate tuple literal: <42, "text">
void codegen_tuple_literal(FILE* output, Tuple* tuple) {
    if (!tuple) return;
    
    fprintf(output, "    ; Tuple literal: %d elements (immutable)\n", tuple->length);
    
    int tuple_id = array_label_counter++;
    
    // Tuples are stack-allocated (fixed size, immutable)
    fprintf(output, "    ; Allocate tuple on stack\n");
    int bytes = tuple->length * 8 + tuple->length * 4;  // Elements + types
    fprintf(output, "    sub rsp, %d  ; Allocate %d bytes\n", bytes, bytes);
    fprintf(output, "    mov [tuple_%d], rsp  ; Save tuple pointer\n", tuple_id);
    
    // Initialize tuple elements
    if (tuple->length > 0 && tuple->elements && tuple->element_types) {
        for (int i = 0; i < tuple->length; i++) {
            ArithmeticExpr* elem = (ArithmeticExpr*)tuple->elements[i];
            if (elem) {
                fprintf(output, "    ; Tuple element %d (type=%d)\n", i, tuple->element_types[i]);
                arithmetic_generate_code(output, elem, NULL);  // Result in r8/xmm0
                fprintf(output, "    mov rax, r8  ; Move result to rax\n");
                
                // Store in tuple (immutable after creation)
                fprintf(output, "    mov rbx, [tuple_%d]  ; Load tuple pointer\n", tuple_id);
                fprintf(output, "    mov [rbx + %d], rax  ; Store element\n", i * 8);
                
                // Store type info
                fprintf(output, "    mov dword [rbx + %d], %d  ; Store type\n", 
                       tuple->length * 8 + i * 4, tuple->element_types[i]);
            }
        }
    }
    
    fprintf(output, "    mov rax, [tuple_%d]  ; Tuple pointer result\n", tuple_id);
}

// Generate stack-based array (fixed size)
void codegen_array_stack(FILE* output, Array* arr) {
    fprintf(output, "    ; Stack array: %d elements\n", arr->length);
    
    // Allocate space on stack
    int bytes = arr->length * 8;  // 8 bytes per element (pointer or numeric)
    fprintf(output, "    sub rsp, %d\n", bytes);
    
    // TODO: Initialize elements
    fprintf(output, "    ; TODO: Initialize array elements\n");
}

// Generate heap-based array (dynamic size)
void codegen_array_heap(FILE* output, Array* arr) {
    fprintf(output, "    ; Heap array: capacity %d\n", arr->capacity);
    
    // Call malloc for array allocation
    int bytes = arr->capacity * 8;
    fprintf(output, "    mov rdi, %d  ; Size in bytes\n", bytes);
    fprintf(output, "    extern malloc\n");
    fprintf(output, "    call malloc\n");
    fprintf(output, "    ; Array pointer in rax\n");
    
    // TODO: Initialize elements and store pointer
}

// Generate list creation (always heap)
void codegen_list_create(FILE* output, List* list) {
    fprintf(output, "    ; List creation: capacity %d\n", list->capacity);
    
    // Lists need two allocations:
    // 1. Array of element pointers
    // 2. Array of element types
    
    fprintf(output, "    ; Allocate element pointer array\n");
    int bytes_elements = list->capacity * 8;
    fprintf(output, "    mov rdi, %d\n", bytes_elements);
    fprintf(output, "    extern malloc\n");
    fprintf(output, "    call malloc\n");
    fprintf(output, "    push rax  ; Save element array pointer\n");
    
    fprintf(output, "    ; Allocate type array\n");
    int bytes_types = list->capacity * 4;  // 4 bytes per VarType enum
    fprintf(output, "    mov rdi, %d\n", bytes_types);
    fprintf(output, "    call malloc\n");
    fprintf(output, "    ; Type array in rax, element array on stack\n");
    
    // TODO: Store both pointers in List struct
}

// Generate tuple creation (stack allocation)
void codegen_tuple_create(FILE* output, Tuple* tuple) {
    fprintf(output, "    ; Tuple creation: %d elements (immutable)\n", tuple->length);
    
    // Allocate on stack (like struct)
    int bytes = tuple->length * 8 + tuple->length * 4;  // Elements + types
    fprintf(output, "    sub rsp, %d\n", bytes);
    
    // TODO: Initialize tuple elements
    fprintf(output, "    ; TODO: Initialize tuple elements (immutable)\n");
}

// Generate array index access
void codegen_array_index(FILE* output, const char* array_name, int index) {
    fprintf(output, "    # Array index access: %s[%d]\n", array_name, index);
    
    // Array is stored on stack, load the heap pointer first
    // Note: array_name should be stack offset like "-8(%rbp)" not symbol name
    fprintf(output, "    movq %s, %%rbx  # Load array pointer from stack\n", array_name);
    
    // Calculate offset: index * 8 (assuming 8-byte elements)
    int offset = index * 8;
    fprintf(output, "    movq %d(%%rbx), %%rax  # Get element at index %d\n", offset, index);
}

// Generate list index access (with runtime type info)
void codegen_list_index(FILE* output, const char* list_name, int index) {
    fprintf(output, "    # List index access: %s(%d)\n", list_name, index);
    
    // Load list structure (AT&T syntax)
    fprintf(output, "    movq %s(%%rip), %%rbx  # List element array\n", list_name);
    fprintf(output, "    movq 8+%s(%%rip), %%rcx  # List type array\n", list_name);
    
    // Get element
    int offset = index * 8;
    fprintf(output, "    movq %d(%%rbx), %%rax  # Element pointer\n", offset);
    
    // Get type info
    int type_offset = index * 4;
    fprintf(output, "    movl %d(%%rcx), %%edx  # Element type\n", type_offset);
}

// Generate index access with variable index (runtime calculation)
void codegen_array_index_var(FILE* output, const char* array_name, const char* index_var) {
    fprintf(output, "    # Array index access: %s[%s]\n", array_name, index_var);
    
    // Load array base address (AT&T syntax)
    fprintf(output, "    leaq %s(%%rip), %%rbx  # Array base address\n", array_name);
    
    // Load index from variable
    fprintf(output, "    movq %s(%%rip), %%rcx  # Load index value\n", index_var);
    
    // Calculate offset: index * 8 (assuming 8-byte elements)
    fprintf(output, "    shlq $3, %%rcx  # index * 8\n");
    
    // Get element at calculated offset
    fprintf(output, "    movq (%%rbx,%%rcx), %%rax  # Get element at index\n");
}

// Generate index access from IndexAccess struct
void codegen_index_access(FILE* output, IndexAccess* access) {
    if (!access || !access->collection_name) return;
    
    if (access->is_list_access) {
        // List access: collection(index)
        if (access->index_type == 0) {
            codegen_list_index(output, access->collection_name, access->index.const_index);
        } else {
            // Variable/expression index for list - similar to array
            fprintf(output, "    ; List index access with variable: %s(...)\n", access->collection_name);
            fprintf(output, "    ; TODO: Runtime list index access\n");
        }
    } else {
        // Array access: collection[index]
        if (access->index_type == 0) {
            // Constant index
            codegen_array_index(output, access->collection_name, access->index.const_index);
        } else if (access->index_type == 1) {
            // Variable index
            codegen_array_index_var(output, access->collection_name, access->index.var_index);
        } else {
            // Expression index - evaluate first then access
            fprintf(output, "    ; Array index access with expression\n");
            fprintf(output, "    ; TODO: Evaluate expression first\n");
        }
    }
}
