#include "control_flow_codegen.h"
#include "../comparison/comparison.h"
#include "../comparison/comparison_codegen.h"
#include "../statement/statement.h"  // ✅ Full Statement definition for ->next
#include "../statement/statement_codegen.h"  // YZ_28: For loop_push/loop_pop
#include <stdio.h>

static int label_counter = 0;

void control_flow_generate_if(FILE* output, IfStatement* stmt, void* context) {
    if (!output || !stmt) return;
    
    // TIER 1: Handle expression-based if
    if (stmt->type == CTRL_IF_EXPR) {
        fprintf(output, "\n    # TIER 1: Expression-based if (result = if cond then val1 else val2)\n");
        fprintf(output, "    # TODO: Implement expression-based if codegen\n");
        return;
    }
    
    int label_else = label_counter++;
    int label_end = label_counter++;
    
    fprintf(output, "\n    # If statement\n");
    
    // ✅ Phase 3.2: Use chained comparison generator for logical ops support
    ComparisonExpr* cond = (ComparisonExpr*)stmt->condition;
    comparison_generate_code_with_chain(output, cond, context);
    
    // Test result
    fprintf(output, "    test %%rax, %%rax\n");
    if (stmt->has_else) {
        fprintf(output, "    jz .if_else_%d\n", label_else);
    } else {
        fprintf(output, "    jz .if_end_%d\n", label_end);
    }
    
    // Then body - recursively generate statements
    fprintf(output, "    # Then body\n");
    Statement* then_stmt = stmt->then_body;
    while (then_stmt) {
        statement_generate_code(output, then_stmt, context);
        then_stmt = then_stmt->next;
    }
    
    if (stmt->has_else) {
        fprintf(output, "    jmp .if_end_%d\n", label_end);
        fprintf(output, ".if_else_%d:\n", label_else);
        fprintf(output, "    # Else body\n");
        
        // Else body - recursively generate statements
        Statement* else_stmt = stmt->else_body;
        while (else_stmt) {
            statement_generate_code(output, else_stmt, context);
            else_stmt = else_stmt->next;
        }
    }
    
    fprintf(output, ".if_end_%d:\n", label_end);
}

void control_flow_generate_while(FILE* output, WhileStatement* stmt, void* context) {
    if (!output || !stmt) return;
    
    int loop_id = label_counter++;
    
    // YZ_28: Push loop context for exit while
    loop_push(loop_id);
    
    fprintf(output, "\n    # While loop\n");
    fprintf(output, ".while_start_%d:\n", loop_id);
    
    // ✅ Phase 3.2: Use chained comparison generator for logical ops support
    ComparisonExpr* cond = (ComparisonExpr*)stmt->condition;
    comparison_generate_code_with_chain(output, cond, context);
    
    fprintf(output, "    test %%rax, %%rax\n");
    fprintf(output, "    jz .loop_end_%d\n", loop_id);
    
    // ✅ Body - recursively generate statements
    fprintf(output, "    # Loop body\n");
    Statement* body_stmt = stmt->body;
    while (body_stmt) {
        statement_generate_code(output, body_stmt, context);
        body_stmt = body_stmt->next;
    }
    
    fprintf(output, "    jmp .while_start_%d\n", loop_id);
    fprintf(output, ".loop_end_%d:\n", loop_id);
    
    // YZ_28: Pop loop context
    loop_pop();
}

void control_flow_generate_for(FILE* output, ForStatement* stmt, void* context) {
    if (!output || !stmt) return;
    
    fprintf(output, "\n    # For loop\n");
    
    // TIER 1: Handle labeled loops
    if (stmt->label) {
        fprintf(output, "    # TIER 1: Loop label '%s'\n", stmt->label);
        fprintf(output, ".loop_label_%s:\n", stmt->label);
    }
    
    fprintf(output, "    # Iterator: %s\n", stmt->iterator ? stmt->iterator : "none");
}
