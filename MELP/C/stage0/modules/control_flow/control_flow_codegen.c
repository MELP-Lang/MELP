#include "control_flow_codegen.h"
#include "../comparison/comparison.h"
#include "../comparison/comparison_codegen.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>

static int label_counter = 0;

void control_flow_generate_if(FILE* output, IfStatement* stmt) {
    if (!output || !stmt) return;
    
    emit_c("\n    // If statement\n");
    
    // Generate condition
    ComparisonExpr* cond = (ComparisonExpr*)stmt->condition;
    
    // Build condition expression
    const char* op_str = "";
    switch (cond->op) {
        case CMP_EQUAL: op_str = "=="; break;
        case CMP_NOT_EQUAL: op_str = "!="; break;
        case CMP_LESS: op_str = "<"; break;
        case CMP_LESS_EQUAL: op_str = "<="; break;
        case CMP_GREATER: op_str = ">"; break;
        case CMP_GREATER_EQUAL: op_str = ">="; break;
    }
    
    emit_c("    if (%s %s %s) {\n", 
           cond->left_value, op_str, cond->right_value);
    
    // Generate then body
    BodyStatement* body = stmt->then_body;
    while (body) {
        if (body->code) {
            emit_c("%s\n", body->code);
        }
        body = body->next;
    }
    
    if (stmt->has_else) {
        emit_c("    } else {\n");
        
        // Generate else body
        body = stmt->else_body;
        while (body) {
            if (body->code) {
                emit_c("%s\n", body->code);
            }
            body = body->next;
        }
    }
    
    emit_c("    }\n");
}

void control_flow_generate_while(FILE* output, WhileStatement* stmt) {
    if (!output || !stmt) return;
    
    emit_c("\n    // While loop\n");
    
    // Generate condition
    ComparisonExpr* cond = (ComparisonExpr*)stmt->condition;
    
    // Build condition expression
    const char* op_str = "";
    switch (cond->op) {
        case CMP_EQUAL: op_str = "=="; break;
        case CMP_NOT_EQUAL: op_str = "!="; break;
        case CMP_LESS: op_str = "<"; break;
        case CMP_LESS_EQUAL: op_str = "<="; break;
        case CMP_GREATER: op_str = ">"; break;
        case CMP_GREATER_EQUAL: op_str = ">="; break;
    }
    
    emit_c("    while (%s %s %s) {\n", 
           cond->left_value, op_str, cond->right_value);
    
    // Generate loop body
    BodyStatement* body = stmt->body;
    while (body) {
        if (body->code) {
            emit_c("%s\n", body->code);
        }
        body = body->next;
    }
    
    emit_c("    }\n");
}

void control_flow_generate_for(FILE* output, ForStatement* stmt) {
    if (!output || !stmt) return;
    
    emit_c("\n    // For loop\n");
    
    // Generate for loop: for (int64_t i = start; i <= end; i++)
    emit_c("    for (int64_t %s = %s; %s <= %s; %s++) {\n",
           stmt->iterator ? stmt->iterator : "i",
           stmt->start ? (char*)stmt->start : "0",
           stmt->iterator ? stmt->iterator : "i",
           stmt->end ? (char*)stmt->end : "10",
           stmt->iterator ? stmt->iterator : "i");
    
    // Generate loop body
    BodyStatement* body = stmt->body;
    while (body) {
        if (body->code) {
            emit_c("%s\n", body->code);
        }
        body = body->next;
    }
    
    emit_c("    }\n");
}
