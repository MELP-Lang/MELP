// LLVM IR Code Generator for Functions
// Wrapper around llvm_backend module for functions compiler
// YZ_58 - Phase 13.5 Part 3-5

#include "functions_codegen_llvm.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../variable/variable.h"
#include "../statement/statement.h"  // YZ_58: Statement types
#include "../comparison/comparison.h"  // YZ_58: Comparison expressions
#include "../control_flow/control_flow.h"  // YZ_58: Control flow structures
#include "../for_loop/for_loop.h"  // YZ_60: For loop structures
#include "../print/print.h"  // YZ_61: Print statement
#include <stdlib.h>
#include <string.h>

// Initialize LLVM codegen context
FunctionLLVMContext* function_llvm_context_create(FILE* output) {
    FunctionLLVMContext* ctx = malloc(sizeof(FunctionLLVMContext));
    ctx->llvm_ctx = llvm_context_create(output);
    ctx->current_func = NULL;
    ctx->globals_emitted = 0;  // YZ_61: Initialize flag
    return ctx;
}

// Free LLVM codegen context
void function_llvm_context_free(FunctionLLVMContext* ctx) {
    if (ctx) {
        llvm_context_free(ctx->llvm_ctx);
        free(ctx);
    }
}

// Generate LLVM IR module header
void function_generate_module_header_llvm(FILE* output) {
    LLVMContext* ctx = llvm_context_create(output);
    llvm_emit_module_header(ctx);
    llvm_context_free(ctx);
}

// Generate LLVM IR module footer
void function_generate_module_footer_llvm(FILE* output) {
    // This function is called with a new context, can't access globals
    // So we don't emit anything here
    // Globals are emitted at end of last function
    (void)output;  // Unused
}

// Forward declaration for statement generation
static LLVMValue* generate_statement_llvm(FunctionLLVMContext* ctx, Statement* stmt);
static LLVMValue* generate_expression_llvm(FunctionLLVMContext* ctx, void* expr);
static LLVMValue* generate_comparison_llvm(FunctionLLVMContext* ctx, ComparisonExpr* cmp);

// Generate LLVM IR for expression
static LLVMValue* generate_expression_llvm(FunctionLLVMContext* ctx, void* expr) {
    if (!expr) {
        return llvm_const_i64(0);
    }
    
    ArithmeticExpr* arith = (ArithmeticExpr*)expr;
    
    // Handle literals
    if (arith->is_literal) {
        // Handle boolean literals
        if (strcmp(arith->value, "true") == 0) {
            return llvm_const_i64(1);
        }
        if (strcmp(arith->value, "false") == 0) {
            return llvm_const_i64(0);
        }
        
        // Parse numeric literal
        long value = atol(arith->value);
        return llvm_const_i64(value);
    }
    
    // Handle variables
    if (!arith->is_literal && !arith->is_function_call && arith->value) {
        // Check if this is a parameter (no need to load, already in register)
        FunctionParam* param = ctx->current_func->params;
        int is_param = 0;
        while (param) {
            if (strcmp(param->name, arith->value) == 0) {
                is_param = 1;
                break;
            }
            param = param->next;
        }
        
        if (is_param) {
            // Parameter - already a value, just return register reference
            return llvm_reg(arith->value);
        } else {
            // Local variable - need to load from stack
            LLVMValue* var_ptr = llvm_reg(arith->value);
            LLVMValue* loaded = llvm_emit_load(ctx->llvm_ctx, var_ptr);
            llvm_value_free(var_ptr);
            return loaded;
        }
    }
    
    // Handle function calls
    if (arith->is_function_call && arith->func_call) {
        FunctionCallExpr* call = arith->func_call;
        
        // Special case: println
        if (strcmp(call->function_name, "println") == 0 && call->arg_count == 1) {
            LLVMValue* arg = generate_expression_llvm(ctx, call->arguments[0]);
            LLVMValue* result = llvm_emit_println(ctx->llvm_ctx, arg);
            // Don't free arg - println returns it
            return result;
        }
        
        // General function call
        LLVMValue** args = malloc(sizeof(LLVMValue*) * call->arg_count);
        for (int i = 0; i < call->arg_count; i++) {
            args[i] = generate_expression_llvm(ctx, call->arguments[i]);
        }
        
        LLVMValue* result = llvm_emit_call(ctx->llvm_ctx, call->function_name, args, call->arg_count);
        
        // Free args
        for (int i = 0; i < call->arg_count; i++) {
            llvm_value_free(args[i]);
        }
        free(args);
        
        return result;
    }
    
    // Handle binary operations
    if (arith->left && arith->right) {
        LLVMValue* left = generate_expression_llvm(ctx, arith->left);
        LLVMValue* right = generate_expression_llvm(ctx, arith->right);
        
        LLVMValue* result = NULL;
        
        switch (arith->op) {
            case ARITH_ADD:
                result = llvm_emit_add(ctx->llvm_ctx, left, right);
                break;
            case ARITH_SUB:
                result = llvm_emit_sub(ctx->llvm_ctx, left, right);
                break;
            case ARITH_MUL:
                result = llvm_emit_mul(ctx->llvm_ctx, left, right);
                break;
            case ARITH_DIV:
                result = llvm_emit_div(ctx->llvm_ctx, left, right);
                break;
            case ARITH_AND:
                result = llvm_emit_and(ctx->llvm_ctx, left, right);
                break;
            case ARITH_OR:
                result = llvm_emit_or(ctx->llvm_ctx, left, right);
                break;
            default:
                result = llvm_const_i64(0);
        }
        
        llvm_value_free(left);
        llvm_value_free(right);
        
        return result;
    }
    
    // Default: return 0
    return llvm_const_i64(0);
}

// Generate LLVM IR for comparison expression
static LLVMValue* generate_comparison_llvm(FunctionLLVMContext* ctx, ComparisonExpr* cmp) {
    if (!cmp) {
        return llvm_const_i64(0);
    }
    
    LLVMValue* left = NULL;
    LLVMValue* right = NULL;
    
    // Generate left operand
    if (cmp->left) {
        left = generate_expression_llvm(ctx, cmp->left);
    } else if (cmp->left_value) {
        if (cmp->left_is_literal) {
            long val = atol(cmp->left_value);
            left = llvm_const_i64(val);
        } else {
            // Variable reference
            FunctionParam* param = ctx->current_func->params;
            int is_param = 0;
            while (param) {
                if (strcmp(param->name, cmp->left_value) == 0) {
                    is_param = 1;
                    break;
                }
                param = param->next;
            }
            
            if (is_param) {
                left = llvm_reg(cmp->left_value);
            } else {
                LLVMValue* var_ptr = llvm_reg(cmp->left_value);
                left = llvm_emit_load(ctx->llvm_ctx, var_ptr);
                llvm_value_free(var_ptr);
            }
        }
    } else {
        left = llvm_const_i64(0);
    }
    
    // Generate right operand
    if (cmp->right) {
        right = generate_expression_llvm(ctx, cmp->right);
    } else if (cmp->right_value) {
        if (cmp->right_is_literal) {
            long val = atol(cmp->right_value);
            right = llvm_const_i64(val);
        } else {
            // Variable reference
            FunctionParam* param = ctx->current_func->params;
            int is_param = 0;
            while (param) {
                if (strcmp(param->name, cmp->right_value) == 0) {
                    is_param = 1;
                    break;
                }
                param = param->next;
            }
            
            if (is_param) {
                right = llvm_reg(cmp->right_value);
            } else {
                LLVMValue* var_ptr = llvm_reg(cmp->right_value);
                right = llvm_emit_load(ctx->llvm_ctx, var_ptr);
                llvm_value_free(var_ptr);
            }
        }
    } else {
        right = llvm_const_i64(0);
    }
    
    // Map comparison operator to LLVM icmp predicate
    const char* llvm_op = NULL;
    switch (cmp->op) {
        case CMP_EQUAL:         llvm_op = "eq";  break;
        case CMP_NOT_EQUAL:     llvm_op = "ne";  break;
        case CMP_LESS:          llvm_op = "slt"; break;
        case CMP_LESS_EQUAL:    llvm_op = "sle"; break;
        case CMP_GREATER:       llvm_op = "sgt"; break;
        case CMP_GREATER_EQUAL: llvm_op = "sge"; break;
        default:                llvm_op = "eq";  break;
    }
    
    // Emit comparison instruction
    LLVMValue* result = llvm_emit_icmp(ctx->llvm_ctx, llvm_op, left, right);
    
    llvm_value_free(left);
    llvm_value_free(right);
    
    // TODO: Handle logical chaining (AND/OR) if cmp->next is set
    
    return result;
}

// Generate LLVM IR for statement
static LLVMValue* generate_statement_llvm(FunctionLLVMContext* ctx, Statement* stmt) {
    if (!stmt) {
        return NULL;
    }
    
    switch (stmt->type) {
        case STMT_VARIABLE_DECL: {
            VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
            
            // Allocate variable
            LLVMValue* var_ptr = llvm_emit_alloca(ctx->llvm_ctx, decl->name);
            
            // Generate initializer expression
            if (decl->init_expr) {
                LLVMValue* init_val = generate_expression_llvm(ctx, decl->init_expr);
                llvm_emit_store(ctx->llvm_ctx, init_val, var_ptr);
                llvm_value_free(init_val);
            } else if (decl->value) {
                // Fallback: use string value
                long val;
                if (strcmp(decl->value, "true") == 0) {
                    val = 1;
                } else if (strcmp(decl->value, "false") == 0) {
                    val = 0;
                } else {
                    val = atol(decl->value);
                }
                LLVMValue* init_val = llvm_const_i64(val);
                llvm_emit_store(ctx->llvm_ctx, init_val, var_ptr);
                llvm_value_free(init_val);
            }
            
            llvm_value_free(var_ptr);
            return NULL;
        }
        
        case STMT_PRINT: {
            // YZ_61: Phase 15 & 17 - Print statement with stdlib integration
            PrintStatement* print_stmt = (PrintStatement*)stmt->data;
            
            if (print_stmt->type == PRINT_STRING_LITERAL) {
                // YZ_61: Phase 17 - String literal support
                // Create global string constant
                char* global_name = llvm_emit_string_global(ctx->llvm_ctx, print_stmt->value);
                
                // Get pointer to first character
                char* str_ptr = llvm_new_temp(ctx->llvm_ctx);
                size_t str_len = strlen(print_stmt->value) + 1;
                fprintf(ctx->llvm_ctx->output, "  %s = getelementptr inbounds [%zu x i8], [%zu x i8]* %s, i64 0, i64 0\n",
                        str_ptr, str_len, str_len, global_name);
                
                // Call mlp_println_string
                fprintf(ctx->llvm_ctx->output, "  call void @mlp_println_string(i8* %s)\n", str_ptr);
                
                free(global_name);
                free(str_ptr);
                return NULL;
            }
            
            // Get the value to print (for variables)
            LLVMValue* value = NULL;
            if (print_stmt->type == PRINT_VARIABLE) {
                // Get variable pointer and load value
                LLVMValue* var_ptr = llvm_reg(print_stmt->value);
                value = llvm_emit_load(ctx->llvm_ctx, var_ptr);
                llvm_value_free(var_ptr);
            }
            
            if (!value) {
                return NULL;
            }
            
            // Call mlp_println_numeric(void* value, uint8_t sto_type)
            // For now, we pass the i64 value directly as a pointer
            // and type = INTERNAL_TYPE_INT64 (1)
            
            // Allocate space for the value on stack
            char* temp_name = llvm_new_temp(ctx->llvm_ctx);
            fprintf(ctx->llvm_ctx->output, "  %s = alloca i64\n", temp_name);
            
            // Store the value
            fprintf(ctx->llvm_ctx->output, "  store i64 %s, i64* %s\n", 
                    value->name, temp_name);
            
            // Cast to void* (i8*)
            char* void_ptr_name = llvm_new_temp(ctx->llvm_ctx);
            fprintf(ctx->llvm_ctx->output, "  %s = bitcast i64* %s to i8*\n",
                    void_ptr_name, temp_name);
            
            // Call mlp_println_numeric(i8* value, i8 type)
            // type = 1 (INTERNAL_TYPE_INT64)
            fprintf(ctx->llvm_ctx->output, "  call void @mlp_println_numeric(i8* %s, i8 1)\n",
                    void_ptr_name);
            
            llvm_value_free(value);
            free(temp_name);
            free(void_ptr_name);
            
            return NULL;
        }
        
        case STMT_RETURN: {
            ReturnStatement* ret = (ReturnStatement*)stmt->data;
            
            if (ret->return_value) {
                LLVMValue* ret_val = generate_expression_llvm(ctx, ret->return_value);
                llvm_emit_return(ctx->llvm_ctx, ret_val);
                llvm_value_free(ret_val);
            } else {
                LLVMValue* zero = llvm_const_i64(0);
                llvm_emit_return(ctx->llvm_ctx, zero);
                llvm_value_free(zero);
            }
            return NULL;
        }
        
        case STMT_EXPRESSION: {
            // Expression statement (e.g., function call for side effects)
            void* expr = stmt->data;
            LLVMValue* result = generate_expression_llvm(ctx, expr);
            if (result) {
                llvm_value_free(result);
            }
            return NULL;
        }
        
        case STMT_IF: {
            IfStatement* if_stmt = (IfStatement*)stmt->data;
            
            // Generate unique labels
            char* then_label = llvm_new_label(ctx->llvm_ctx);
            char* else_label = if_stmt->has_else ? llvm_new_label(ctx->llvm_ctx) : NULL;
            char* after_label = llvm_new_label(ctx->llvm_ctx);
            
            // Generate condition
            LLVMValue* cond = generate_comparison_llvm(ctx, (ComparisonExpr*)if_stmt->condition);
            
            // Emit conditional branch
            if (if_stmt->has_else) {
                llvm_emit_br_cond(ctx->llvm_ctx, cond, then_label, else_label);
            } else {
                llvm_emit_br_cond(ctx->llvm_ctx, cond, then_label, after_label);
            }
            llvm_value_free(cond);
            
            // Then block
            llvm_emit_label(ctx->llvm_ctx, then_label);
            Statement* then_stmt = if_stmt->then_body;
            while (then_stmt) {
                generate_statement_llvm(ctx, then_stmt);
                then_stmt = then_stmt->next;
            }
            llvm_emit_br(ctx->llvm_ctx, after_label);
            
            // Else block (if exists)
            if (if_stmt->has_else) {
                llvm_emit_label(ctx->llvm_ctx, else_label);
                Statement* else_stmt = if_stmt->else_body;
                while (else_stmt) {
                    generate_statement_llvm(ctx, else_stmt);
                    else_stmt = else_stmt->next;
                }
                llvm_emit_br(ctx->llvm_ctx, after_label);
                free(else_label);
            }
            
            // After if
            llvm_emit_label(ctx->llvm_ctx, after_label);
            
            free(then_label);
            free(after_label);
            return NULL;
        }
        
        case STMT_WHILE: {
            WhileStatement* while_stmt = (WhileStatement*)stmt->data;
            
            // Generate unique labels
            char* cond_label = llvm_new_label(ctx->llvm_ctx);
            char* body_label = llvm_new_label(ctx->llvm_ctx);
            char* end_label = llvm_new_label(ctx->llvm_ctx);
            
            // Jump to condition check
            llvm_emit_br(ctx->llvm_ctx, cond_label);
            
            // Condition block
            llvm_emit_label(ctx->llvm_ctx, cond_label);
            LLVMValue* cond = generate_comparison_llvm(ctx, (ComparisonExpr*)while_stmt->condition);
            llvm_emit_br_cond(ctx->llvm_ctx, cond, body_label, end_label);
            llvm_value_free(cond);
            
            // Body block
            llvm_emit_label(ctx->llvm_ctx, body_label);
            Statement* body_stmt = while_stmt->body;
            while (body_stmt) {
                generate_statement_llvm(ctx, body_stmt);
                body_stmt = body_stmt->next;
            }
            llvm_emit_br(ctx->llvm_ctx, cond_label);  // Loop back
            
            // End block
            llvm_emit_label(ctx->llvm_ctx, end_label);
            
            free(cond_label);
            free(body_label);
            free(end_label);
            return NULL;
        }
        
        case STMT_FOR: {
            // Note: Parser returns ForLoop, but we need to convert it
            ForLoop* for_loop = (ForLoop*)stmt->data;
            
            // For loop only works with range loops currently
            if (for_loop->loop_type != FOR_TYPE_RANGE) {
                fprintf(stderr, "Error: For-each loops not yet supported in LLVM backend\n");
                return NULL;
            }
            
            // Allocate iterator variable
            LLVMValue* iter_ptr = llvm_emit_alloca(ctx->llvm_ctx, for_loop->var_name);
            
            // Initialize iterator with start value
            LLVMValue* start_val = llvm_const_i64(for_loop->start_value);
            llvm_emit_store(ctx->llvm_ctx, start_val, iter_ptr);
            llvm_value_free(start_val);
            
            // Create constant for end value with unique name
            char end_var_name[64];
            snprintf(end_var_name, sizeof(end_var_name), "__for_end_%d", ctx->llvm_ctx->temp_counter++);
            LLVMValue* end_val = llvm_const_i64(for_loop->end_value);
            LLVMValue* end_ptr = llvm_emit_alloca(ctx->llvm_ctx, end_var_name);
            llvm_emit_store(ctx->llvm_ctx, end_val, end_ptr);
            llvm_value_free(end_val);
            
            // Generate unique labels
            char* cond_label = llvm_new_label(ctx->llvm_ctx);
            char* body_label = llvm_new_label(ctx->llvm_ctx);
            char* inc_label = llvm_new_label(ctx->llvm_ctx);
            char* end_label = llvm_new_label(ctx->llvm_ctx);
            
            // Jump to condition
            llvm_emit_br(ctx->llvm_ctx, cond_label);
            
            // Condition: i <= end
            llvm_emit_label(ctx->llvm_ctx, cond_label);
            LLVMValue* iter_val = llvm_emit_load(ctx->llvm_ctx, iter_ptr);
            LLVMValue* end_val_loaded = llvm_emit_load(ctx->llvm_ctx, end_ptr);
            LLVMValue* cond = llvm_emit_icmp(ctx->llvm_ctx, "sle", iter_val, end_val_loaded);
            llvm_emit_br_cond(ctx->llvm_ctx, cond, body_label, end_label);
            llvm_value_free(iter_val);
            llvm_value_free(end_val_loaded);
            llvm_value_free(cond);
            
            // Body block
            llvm_emit_label(ctx->llvm_ctx, body_label);
            Statement* body_stmt = for_loop->body;
            while (body_stmt) {
                generate_statement_llvm(ctx, body_stmt);
                body_stmt = body_stmt->next;
            }
            llvm_emit_br(ctx->llvm_ctx, inc_label);
            
            // Increment: i = i + 1 (or decrement for downto)
            llvm_emit_label(ctx->llvm_ctx, inc_label);
            LLVMValue* iter_val_inc = llvm_emit_load(ctx->llvm_ctx, iter_ptr);
            LLVMValue* one = llvm_const_i64(1);
            LLVMValue* inc_result;
            if (for_loop->direction == FOR_TO) {
                inc_result = llvm_emit_add(ctx->llvm_ctx, iter_val_inc, one);
            } else {
                inc_result = llvm_emit_sub(ctx->llvm_ctx, iter_val_inc, one);
            }
            llvm_emit_store(ctx->llvm_ctx, inc_result, iter_ptr);
            llvm_value_free(iter_val_inc);
            llvm_value_free(one);
            llvm_value_free(inc_result);
            llvm_emit_br(ctx->llvm_ctx, cond_label);  // Loop back
            
            // End block
            llvm_emit_label(ctx->llvm_ctx, end_label);
            
            llvm_value_free(iter_ptr);
            llvm_value_free(end_ptr);
            free(cond_label);
            free(body_label);
            free(inc_label);
            free(end_label);
            return NULL;
        }
        
        case STMT_ASSIGNMENT: {
            VariableAssignment* assign = (VariableAssignment*)stmt->data;
            
            // Generate value expression
            LLVMValue* value = generate_expression_llvm(ctx, assign->value_expr);
            
            // Get variable pointer (already allocated)
            LLVMValue* var_ptr = llvm_reg(assign->name);
            
            // Store value
            llvm_emit_store(ctx->llvm_ctx, value, var_ptr);
            
            llvm_value_free(value);
            llvm_value_free(var_ptr);
            return NULL;
        }
        
        default:
            // TODO: Handle other statement types
            return NULL;
    }
}

// Generate LLVM IR for function declaration
void function_generate_declaration_llvm(FunctionLLVMContext* ctx, FunctionDeclaration* func) {
    if (!func) return;
    
    ctx->current_func = func;
    
    // Collect parameter names
    const char** param_names = NULL;
    int param_count = 0;
    
    FunctionParam* param = func->params;
    while (param) {
        param_count++;
        param = param->next;
    }
    
    if (param_count > 0) {
        param_names = malloc(sizeof(char*) * param_count);
        param = func->params;
        int i = 0;
        while (param) {
            param_names[i++] = param->name;
            param = param->next;
        }
    }
    
    // Emit function start
    llvm_emit_function_start(ctx->llvm_ctx, func->name, param_names, param_count);
    llvm_emit_function_entry(ctx->llvm_ctx);
    
    // Generate body statements
    Statement* stmt = func->body;
    while (stmt) {
        generate_statement_llvm(ctx, stmt);
        stmt = stmt->next;
    }
    
    // Emit function end
    llvm_emit_function_end(ctx->llvm_ctx);
    
    // Cleanup
    if (param_names) {
        free((void*)param_names);
    }
}
