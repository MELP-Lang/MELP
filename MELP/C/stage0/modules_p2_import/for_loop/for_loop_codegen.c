#include "for_loop_codegen.h"
#include "../statement/statement_codegen.h"
#include "../functions/functions.h"
#include <string.h>

static int for_label_counter = 0;

// YZ_28: Generate code for for-each loop
// Desugar: for each item in arr  =>  for __idx = 0 to length-1
//              body                       item = arr[__idx]
//          end for                        body
//                                     end for
static void for_each_generate_code(FILE* output, ForLoop* loop, void* context) {
    FunctionDeclaration* func = (FunctionDeclaration*)context;
    int loop_id = for_label_counter++;
    
    fprintf(output, "\n    # For-each loop: for each %s in %s\n", 
            loop->var_name, loop->collection_name);
    
    // YZ_28/YZ_90: Push loop context for exit/continue for
    loop_push(loop_id, loop_id);  // Same ID for start/end labels
    
    // Register loop variable and internal index
    function_register_local_var(func, loop->var_name);
    
    // Get collection info (array, list, or tuple)
    int coll_offset = function_get_var_offset(func, loop->collection_name);
    int coll_length = 0;
    int is_tuple = function_is_tuple(func, loop->collection_name);
    int is_list = function_is_list(func, loop->collection_name);
    
    if (is_tuple) {
        coll_length = function_get_tuple_length(func, loop->collection_name);
    } else if (is_list) {
        coll_length = function_get_list_length(func, loop->collection_name);
    } else {
        // Array
        coll_length = function_get_array_length(func, loop->collection_name);
    }
    
    // Create internal index variable __for_idx_N
    char idx_name[64];
    snprintf(idx_name, sizeof(idx_name), "__for_idx_%d", loop_id);
    function_register_local_var(func, idx_name);
    int idx_offset = function_get_var_offset(func, idx_name);
    int item_offset = function_get_var_offset(func, loop->var_name);
    
    // 1. Initialize index: __idx = 0
    fprintf(output, "    movq $0, %%r8  # Init index\n");
    fprintf(output, "    movq %%r8, %d(%%rbp)  # __for_idx_%d = 0\n", idx_offset, loop_id);
    
    // 2. Loop start
    fprintf(output, ".foreach_start_%d:\n", loop_id);
    
    // 3. Check: __idx < length
    fprintf(output, "    movq %d(%%rbp), %%r8  # Load index\n", idx_offset);
    fprintf(output, "    movq $%d, %%r9  # Collection length\n", coll_length);
    fprintf(output, "    cmpq %%r9, %%r8\n");
    fprintf(output, "    jge .loop_end_%d  # Exit if index >= length\n", loop_id);
    
    // 4. Get item: item = collection[__idx]
    if (is_tuple) {
        // Tuple access: tpl<idx>
        fprintf(output, "    # Tuple element access\n");
        fprintf(output, "    movq %d(%%rbp), %%rdi  # Tuple pointer\n", coll_offset);
        fprintf(output, "    movq %d(%%rbp), %%rsi  # Index\n", idx_offset);
        fprintf(output, "    call sto_tuple_get\n");
        fprintf(output, "    movq %%rax, %d(%%rbp)  # Store in %s\n", item_offset, loop->var_name);
    } else if (is_list) {
        // List access: lst(idx)
        fprintf(output, "    # List element access\n");
        fprintf(output, "    movq %d(%%rbp), %%rdi  # List pointer\n", coll_offset);
        fprintf(output, "    movq %d(%%rbp), %%rsi  # Index\n", idx_offset);
        fprintf(output, "    call sto_list_get\n");
        fprintf(output, "    movq %%rax, %d(%%rbp)  # Store in %s\n", item_offset, loop->var_name);
    } else {
        // Array access: arr[idx] - direct memory access
        fprintf(output, "    # Array element access\n");
        fprintf(output, "    movq %d(%%rbp), %%r9  # Array base pointer\n", coll_offset);
        fprintf(output, "    movq %d(%%rbp), %%r8  # Index\n", idx_offset);
        fprintf(output, "    movq (%%r9, %%r8, 8), %%r10  # Load element\n");
        fprintf(output, "    movq %%r10, %d(%%rbp)  # Store in %s\n", item_offset, loop->var_name);
    }
    
    // 5. Body statements
    Statement* stmt = loop->body;
    while (stmt) {
        statement_generate_code(output, stmt, context);
        stmt = stmt->next;
    }
    
    // 6. Increment: __idx++
    fprintf(output, "    movq %d(%%rbp), %%r8  # Load index\n", idx_offset);
    fprintf(output, "    addq $1, %%r8  # index++\n");
    fprintf(output, "    movq %%r8, %d(%%rbp)  # Store index\n", idx_offset);
    
    // 7. Jump back
    fprintf(output, "    jmp .foreach_start_%d\n", loop_id);
    fprintf(output, ".loop_end_%d:\n", loop_id);
    
    // YZ_28: Pop loop context
    loop_pop();
}

// Desugar for loop to while loop pattern:
//
// for i = 0 to 10        =>    i = 0
//     body                     while i <= 10
// end                              body
//                                  i = i + 1
//                              end
//
// for i = 10 downto 0    =>    i = 10
//     body                     while i >= 0
// end                              body
//                                  i = i - 1
//                              end

void for_loop_generate_code(FILE* output, ForLoop* loop, void* context) {
    // YZ_28: Check for for-each loop
    if (loop->loop_type == FOR_TYPE_EACH) {
        for_each_generate_code(output, loop, context);
        return;
    }
    
    // Range-based for loop (existing code)
    FunctionDeclaration* func = (FunctionDeclaration*)context;
    int loop_id = for_label_counter++;
    
    fprintf(output, "\n    # For loop (desugared to while)\n");
    
    // YZ_28/YZ_90: Push loop context for exit/continue for
    loop_push(loop_id, loop_id);  // Same ID for start/end labels
    
    // 0. Auto-register loop variable if not already registered
    function_register_local_var(func, loop->var_name);
    
    // 1. Initialize loop variable: i = start_value
    int var_offset = function_get_var_offset(func, loop->var_name);
    fprintf(output, "    movq $%d, %%r8  # Init loop var %s\n", 
            loop->start_value, loop->var_name);
    fprintf(output, "    movq %%r8, %d(%%rbp)\n", var_offset);
    
    // 2. While loop structure
    fprintf(output, ".for_start_%d:\n", loop_id);
    
    // 3. Condition check: i <= end_value (for TO) or i >= end_value (for DOWNTO)
    fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", var_offset, loop->var_name);
    fprintf(output, "    movq $%d, %%r9  # End value\n", loop->end_value);
    fprintf(output, "    cmp %%r8, %%r9\n");
    fprintf(output, "    movq $0, %%rax\n");
    
    if (loop->direction == FOR_TO) {
        // For TO: continue while i <= end (i.e., end >= i)
        fprintf(output, "    setge %%al  # end >= i\n");
    } else {
        // For DOWNTO: continue while i >= end (i.e., end <= i)
        fprintf(output, "    setle %%al  # end <= i\n");
    }
    
    fprintf(output, "    test %%rax, %%rax\n");
    fprintf(output, "    jz .loop_end_%d\n", loop_id);
    
    // 4. Body statements
    Statement* stmt = loop->body;
    while (stmt) {
        statement_generate_code(output, stmt, context);
        stmt = stmt->next;
    }
    
    // 5. Increment/Decrement (continue jumps here!)
    fprintf(output, ".for_continue_%d:\n", loop_id);  // YZ_90: Continue target
    fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", var_offset, loop->var_name);
    if (loop->direction == FOR_TO) {
        fprintf(output, "    addq $1, %%r8  # i++\n");
    } else {
        fprintf(output, "    subq $1, %%r8  # i--\n");
    }
    fprintf(output, "    movq %%r8, %d(%%rbp)  # Store %s\n", var_offset, loop->var_name);
    
    // 6. Jump back to start
    fprintf(output, "    jmp .for_start_%d\n", loop_id);
    fprintf(output, ".loop_end_%d:\n", loop_id);
    
    // YZ_28: Pop loop context
    loop_pop();
}
