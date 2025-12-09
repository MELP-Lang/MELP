#include "for_loop_codegen.h"
#include "../statement/statement_codegen.h"
#include "../functions/functions.h"
#include <string.h>

static int for_label_counter = 0;

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
    FunctionDeclaration* func = (FunctionDeclaration*)context;
    int loop_id = for_label_counter++;
    
    fprintf(output, "\n    ; For loop (desugared to while)\n");
    
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
    fprintf(output, "    jz .for_end_%d\n", loop_id);
    
    // 4. Body statements
    Statement* stmt = loop->body;
    while (stmt) {
        statement_generate_code(output, stmt, context);
        stmt = stmt->next;
    }
    
    // 5. Increment/Decrement: i = i + 1 (TO) or i = i - 1 (DOWNTO)
    fprintf(output, "    movq %d(%%rbp), %%r8  # Load %s\n", var_offset, loop->var_name);
    if (loop->direction == FOR_TO) {
        fprintf(output, "    addq $1, %%r8  # i++\n");
    } else {
        fprintf(output, "    subq $1, %%r8  # i--\n");
    }
    fprintf(output, "    movq %%r8, %d(%%rbp)  # Store %s\n", var_offset, loop->var_name);
    
    // 6. Jump back to start
    fprintf(output, "    jmp .for_start_%d\n", loop_id);
    fprintf(output, ".for_end_%d:\n", loop_id);
}
