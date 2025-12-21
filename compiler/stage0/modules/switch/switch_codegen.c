#include "switch_codegen.h"
#include "../array/arithmetic/arithmetic.h"
#include "../array/arithmetic/arithmetic_codegen.h"
#include "../statement/statement.h"
#include "../statement/statement_codegen.h"
#include <stdio.h>

static int switch_label_counter = 0;

// Generate assembly for switch statement
// Strategy: Linear comparison for each case (simple and reliable)
// Future optimization: Jump table for dense integer ranges
void switch_codegen(FILE* output, SwitchStatement* stmt, void* func) {
    if (!stmt || !output) return;
    
    int switch_id = switch_label_counter++;
    
    fprintf(output, "\n    # Switch statement\n");
    
    // Evaluate switch expression into %rax
    ArithmeticExpr* expr = (ArithmeticExpr*)stmt->expression;
    arithmetic_generate_code(output, expr, func);
    
    // Save switch value in %r15 (preserved across comparisons)
    fprintf(output, "    movq %%r8, %%r15  # Save switch expression value\n");
    
    // Generate case checks
    SwitchCase* current_case = stmt->cases;
    int case_num = 0;
    
    while (current_case) {
        if (!current_case->is_default) {
            fprintf(output, "\n    # Case %d\n", case_num);
            
            // Evaluate case value
            ArithmeticExpr* case_expr = (ArithmeticExpr*)current_case->value;
            arithmetic_generate_code(output, case_expr, func);
            
            // Compare with switch value
            fprintf(output, "    cmpq %%r15, %%r8\n");
            fprintf(output, "    je .switch_%d_case_%d\n", switch_id, case_num);
        }
        
        current_case = current_case->next;
        case_num++;
    }
    
    // If no case matched, jump to default or end
    if (stmt->default_case) {
        fprintf(output, "    jmp .switch_%d_default\n", switch_id);
    } else {
        fprintf(output, "    jmp .switch_%d_end\n", switch_id);
    }
    
    // Generate case bodies
    current_case = stmt->cases;
    case_num = 0;
    
    while (current_case) {
        if (current_case->is_default) {
            fprintf(output, "\n.switch_%d_default:\n", switch_id);
            fprintf(output, "    # Default case\n");
        } else {
            fprintf(output, "\n.switch_%d_case_%d:\n", switch_id, case_num);
            fprintf(output, "    # Case %d body\n", case_num);
        }
        
        // Generate case body statements
        Statement* body_stmt = current_case->body;
        while (body_stmt) {
            statement_generate_code(output, body_stmt, func);
            body_stmt = body_stmt->next;
        }
        
        // Implicit break (no fall-through in PMPL)
        fprintf(output, "    jmp .switch_%d_end\n", switch_id);
        
        // Move to next case and increment counter if not default
        if (!current_case->is_default) {
            case_num++;
        }
        current_case = current_case->next;
    }
    
    fprintf(output, "\n.switch_%d_end:\n", switch_id);
    fprintf(output, "    # End of switch\n");
}
