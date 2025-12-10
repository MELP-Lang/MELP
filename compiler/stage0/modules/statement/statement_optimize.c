#include "statement_optimize.h"
#include "../control_flow/control_flow.h"
#include "../comparison/comparison.h"
#include "../arithmetic/arithmetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if comparison condition is constant
int is_constant_comparison(ComparisonExpr* cmp, int* is_true) {
    if (!cmp) return 0;
    
    // Check if both sides are constant numeric literals
    if (cmp->left_is_literal && cmp->right_is_literal &&
        cmp->left_value && cmp->right_value) {
        
        double left_val = atof(cmp->left_value);
        double right_val = atof(cmp->right_value);
        
        switch (cmp->op) {
            case CMP_EQUAL:  *is_true = (left_val == right_val); return 1;
            case CMP_NOT_EQUAL:  *is_true = (left_val != right_val); return 1;
            case CMP_LESS:  *is_true = (left_val < right_val); return 1;
            case CMP_LESS_EQUAL:  *is_true = (left_val <= right_val); return 1;
            case CMP_GREATER:  *is_true = (left_val > right_val); return 1;
            case CMP_GREATER_EQUAL:  *is_true = (left_val >= right_val); return 1;
            default: return 0;
        }
    }
    
    return 0;
}

// Check if arithmetic expression is constant boolean
int is_constant_boolean(ArithmeticExpr* expr, int* is_true) {
    if (!expr) return 0;
    
    // Check for boolean literals
    if (expr->is_boolean && expr->is_literal) {
        if (strcmp(expr->value, "true") == 0) {
            *is_true = 1;
            return 1;
        }
        if (strcmp(expr->value, "false") == 0) {
            *is_true = 0;
            return 1;
        }
    }
    
    return 0;
}

// Check if condition is constant
int statement_is_constant_condition(void* condition, int* is_true) {
    if (!condition) return 0;
    
    // Try as comparison
    ComparisonExpr* cmp = (ComparisonExpr*)condition;
    if (is_constant_comparison(cmp, is_true)) {
        return 1;
    }
    
    // Try as arithmetic (boolean)
    ArithmeticExpr* arith = (ArithmeticExpr*)condition;
    if (is_constant_boolean(arith, is_true)) {
        return 1;
    }
    
    return 0;
}

// Optimize single statement
Statement* statement_optimize_dead_code_single(Statement* stmt) {
    if (!stmt) return NULL;
    
    // Optimize if statement
    if (stmt->type == STMT_IF) {
        IfStatement* if_stmt = (IfStatement*)stmt->data;
        int is_true = 0;
        
        if (statement_is_constant_condition(if_stmt->condition, &is_true)) {
            if (is_true) {
                // if true then X else Y → X (remove else)
                if (if_stmt->has_else && if_stmt->else_body) {
                    statement_free(if_stmt->else_body);
                    if_stmt->else_body = NULL;
                    if_stmt->has_else = 0;
                }
                // Keep then body
                return stmt;
            } else {
                // if false then X else Y → Y (or remove entirely)
                if (if_stmt->has_else && if_stmt->else_body) {
                    // Keep else body, discard then body
                    statement_free(if_stmt->then_body);
                    Statement* else_body = if_stmt->else_body;
                    if_stmt->else_body = NULL;
                    statement_free(stmt);
                    return else_body;
                } else {
                    // No else, remove entire if statement
                    statement_free(stmt);
                    return NULL;
                }
            }
        }
    }
    
    // Optimize while statement
    if (stmt->type == STMT_WHILE) {
        WhileStatement* while_stmt = (WhileStatement*)stmt->data;
        int is_true = 0;
        
        if (statement_is_constant_condition(while_stmt->condition, &is_true)) {
            if (!is_true) {
                // while false → remove entire loop
                statement_free(stmt);
                return NULL;
            }
            // while true → infinite loop, keep as is (might have exit inside)
        }
    }
    
    return stmt;
}

// Optimize statement list (linked list)
Statement* statement_optimize_dead_code(Statement* stmt) {
    if (!stmt) return NULL;
    
    Statement* current = stmt;
    Statement* prev = NULL;
    Statement* head = stmt;
    int found_return = 0;
    
    while (current) {
        // Stop processing after return (all code after is dead)
        if (found_return) {
            statement_free(current);
            if (prev) {
                prev->next = NULL;
            }
            break;
        }
        
        // Optimize current statement
        Statement* optimized = statement_optimize_dead_code_single(current);
        Statement* next = current->next;
        
        if (optimized == NULL) {
            // Statement was removed
            if (prev) {
                prev->next = next;
            } else {
                head = next;
            }
            current = next;
            continue;
        }
        
        // Update links if statement changed
        if (optimized != current) {
            if (prev) {
                prev->next = optimized;
            } else {
                head = optimized;
            }
            optimized->next = next;
        }
        
        // Recursively optimize nested bodies
        if (optimized->type == STMT_IF) {
            IfStatement* if_stmt = (IfStatement*)optimized->data;
            if_stmt->then_body = statement_optimize_dead_code(if_stmt->then_body);
            if (if_stmt->else_body) {
                if_stmt->else_body = statement_optimize_dead_code(if_stmt->else_body);
            }
        } else if (optimized->type == STMT_WHILE) {
            WhileStatement* while_stmt = (WhileStatement*)optimized->data;
            while_stmt->body = statement_optimize_dead_code(while_stmt->body);
        } else if (optimized->type == STMT_FOR) {
            ForStatement* for_stmt = (ForStatement*)optimized->data;
            for_stmt->body = statement_optimize_dead_code(for_stmt->body);
        }
        
        // Check if this is a return statement
        if (optimized->type == STMT_RETURN) {
            found_return = 1;
        }
        
        prev = optimized;
        current = optimized->next;
    }
    
    return head;
}
