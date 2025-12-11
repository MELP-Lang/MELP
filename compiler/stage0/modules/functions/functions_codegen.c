#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions_codegen.h"
#include "functions.h"
#include "../arithmetic/arithmetic_parser.h"
#include "../arithmetic/arithmetic_codegen.h"
#include "../statement/statement_codegen.h"  // ✅ Statement codegen import!
#include "../variable/variable.h"  // ✅ VariableDeclaration type
#include "../control_flow/control_flow.h"  // ✅ IfStatement, WhileStatement, ForStatement
#include "../for_loop/for_loop.h"  // YZ_28: ForLoop type

// Now we can use real expression codegen AND statement codegen!
// arithmetic_parse_expression() and arithmetic_generate_code()
// statement_generate_code() for function bodies

// Temporary wrapper for expression codegen
// In a full compiler, this would be handled by expression module
static void expression_generate_code(FILE* output, void* expr, void* context) {
    // For now, assume expr is ArithmeticExpr*
    if (expr) {
        ArithmeticExpr* arith_expr = (ArithmeticExpr*)expr;
        arithmetic_generate_code(output, arith_expr, context);
    } else {
        fprintf(output, "    # No expression to evaluate\n");
        fprintf(output, "    xor %%rax, %%rax  # Return 0\n");
    }
}

// Generate function prologue
void function_generate_prologue(FILE* output, FunctionDeclaration* func) {
    fprintf(output, "\n# Function: %s\n", func->name);
    fprintf(output, ".global %s\n", func->name);
    fprintf(output, "%s:\n", func->name);
    
    // Save frame pointer
    fprintf(output, "    pushq %%rbp\n");
    fprintf(output, "    movq %%rsp, %%rbp\n");
    
    // Allocate space for local variables (if any)
    if (func->local_var_count > 0) {
        int stack_space = func->local_var_count * 8;  // 8 bytes per variable
        fprintf(output, "    subq $%d, %%rsp\n", stack_space);
    }
    
    // Parameters are passed via registers (x86-64 calling convention):
    // rdi, rsi, rdx, rcx, r8, r9 for first 6 parameters
    // Additional parameters on stack
    
    // Save register parameters to their stack locations
    int param_index = 0;
    FunctionParam* param = func->params;
    const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    while (param && param_index < 6) {
        if (param->name) {
            int offset = function_get_var_offset(func, param->name);
            fprintf(output, "    # Parameter: %s at %d(%%rbp)\n", param->name, offset);
            fprintf(output, "    movq %s, %d(%%rbp)\n", param_regs[param_index], offset);
        }
        param = param->next;
        param_index++;
    }
}

// Generate function epilogue
void function_generate_epilogue(FILE* output, FunctionDeclaration* func) {
    fprintf(output, "\n.L%s_return:\n", func->name);
    
    // Restore stack pointer
    fprintf(output, "    movq %%rbp, %%rsp\n");
    fprintf(output, "    popq %%rbp\n");
    fprintf(output, "    ret\n");
}

// Forward declaration for recursive scan
static void scan_statement_for_variables(FunctionDeclaration* func, Statement* stmt);

// Recursively scan statement tree for variable declarations
static void scan_statement_for_variables(FunctionDeclaration* func, Statement* stmt) {
    if (!stmt) return;
    
    // Register variable declaration
    if (stmt->type == STMT_VARIABLE_DECL) {
        VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
        if (decl && decl->name) {
            // TTO: Register with type flag (1=numeric, 0=string)
            int is_numeric = (decl->type != VAR_STRING) ? 1 : 0;
            function_register_local_var_with_type(func, decl->name, is_numeric);
        }
    }
    
    // Recursively scan nested blocks (if/else/while bodies)
    if (stmt->type == STMT_IF) {
        IfStatement* if_stmt = (IfStatement*)stmt->data;
        if (if_stmt) {
            // Scan then body
            Statement* nested = if_stmt->then_body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
            // Scan else body
            nested = if_stmt->else_body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
        }
    }
    else if (stmt->type == STMT_WHILE) {
        WhileStatement* while_stmt = (WhileStatement*)stmt->data;
        if (while_stmt) {
            Statement* nested = while_stmt->body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
        }
    }
    else if (stmt->type == STMT_FOR) {
        // ForLoop from for_loop module (not ForStatement)
        ForLoop* for_loop = (ForLoop*)stmt->data;
        if (for_loop) {
            Statement* nested = for_loop->body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
        }
    }
}

// Generate function declaration
void function_generate_declaration(FILE* output, FunctionDeclaration* func) {
    if (!func) return;
    
    // ✅ FIRST: Register parameters as local variables
    // Parameters occupy first N stack slots: -8, -16, -24...
    FunctionParam* param = func->params;
    while (param) {
        if (param->name) {
            function_register_local_var(func, param->name);
        }
        param = param->next;
    }
    
    // ✅ SECOND: Recursively scan ALL statements (including nested blocks)
    Statement* stmt = func->body;
    while (stmt) {
        scan_statement_for_variables(func, stmt);
        stmt = stmt->next;
    }
    
    // NOW generate prologue with correct count
    function_generate_prologue(output, func);
    
    // ✅ Function body - use statement codegen (modular!)
    stmt = func->body;
    while (stmt) {
        statement_generate_code(output, stmt, func);
        stmt = stmt->next;
    }
    
    function_generate_epilogue(output, func);
}

// Generate function call
void function_generate_call(FILE* output, FunctionCall* call) {
    if (!call) return;
    
    fprintf(output, "    # Call function: %s\n", call->function_name);
    
    // Check if this is a builtin stdlib function
    int is_builtin = function_is_builtin(call->function_name);
    
    if (is_builtin) {
        // Handle builtin stdlib functions
        
        // println(value) - determine type and call appropriate function
        if (strcmp(call->function_name, "println") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: println requires exactly 1 argument\n");
                return;
            }
            
            // Determine argument type
            // If argument is a variable, check its type
            void* arg = call->arguments[0];
            ArithmeticExpr* expr = (ArithmeticExpr*)arg;
            
            // Check if this is a variable reference (simple heuristic)
            int is_string_var = 0;
            if (expr && !expr->left && !expr->right && expr->value && !expr->is_literal) {
                // This is a variable reference
                // Check if variable name suggests string type (temporary hack)
                const char* var_name = expr->value;
                if (strstr(var_name, "message") || strstr(var_name, "str") || 
                    strstr(var_name, "text") || strstr(var_name, "name") ||
                    strstr(var_name, "welcome") || strstr(var_name, "greeting") ||
                    strstr(var_name, "title") || strstr(var_name, "label")) {
                    is_string_var = 1;
                }
            }
            
            // Evaluate argument expression
            fprintf(output, "    # Evaluate println argument\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                
                if (is_string_var) {
                    // String argument - call mlp_println_string
                    fprintf(output, "    movq %%r8, %%rdi    # arg1: string pointer\n");
                    fprintf(output, "    call mlp_println_string\n");
                } else {
                    // Numeric argument - TTO-aware call
                    fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
                    fprintf(output, "    movq %%r8, (%%rsp)   # Store value\n");
                    fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
                    fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
                    fprintf(output, "    call mlp_println_numeric\n");
                    fprintf(output, "    addq $16, %%rsp      # Clean up temp space\n");
                }
            }
            return;
        }
        
        // print(value) - print without newline
        if (strcmp(call->function_name, "print") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: print requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # Evaluate print argument\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
                fprintf(output, "    movq %%r8, (%%rsp)   # Store value\n");
                fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
                fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
                fprintf(output, "    call mlp_print_numeric\n");
                fprintf(output, "    addq $16, %%rsp      # Clean up\n");
            }
            return;
        }
        
        // toString(value) - convert to string
        if (strcmp(call->function_name, "toString") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: toString requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # Evaluate toString argument\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    subq $16, %%rsp      # Allocate temp space\n");
                fprintf(output, "    movq %%r8, (%%rsp)   # Store value\n");
                fprintf(output, "    movq %%rsp, %%rdi    # arg1: pointer to value\n");
                fprintf(output, "    movq $0, %%rsi       # arg2: TTO_TYPE_INT64\n");
                fprintf(output, "    call mlp_toString_numeric\n");
                fprintf(output, "    addq $16, %%rsp      # Clean up\n");
                // Return value (string pointer) is in %rax
            }
            return;
        }
        
        // YZ_29: Phase 5 - String case conversion & trimming
        
        // toUpperCase(string) - convert to uppercase
        if (strcmp(call->function_name, "toUpperCase") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: toUpperCase requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # toUpperCase: convert string to uppercase\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: string pointer\n");
                fprintf(output, "    call mlp_string_toUpperCase\n");
                fprintf(output, "    movq %%rax, %%r8     # Result in %%r8\n");
            }
            return;
        }
        
        // toLowerCase(string) - convert to lowercase
        if (strcmp(call->function_name, "toLowerCase") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: toLowerCase requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # toLowerCase: convert string to lowercase\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: string pointer\n");
                fprintf(output, "    call mlp_string_toLowerCase\n");
                fprintf(output, "    movq %%rax, %%r8     # Result in %%r8\n");
            }
            return;
        }
        
        // trim(string) - remove leading/trailing whitespace
        if (strcmp(call->function_name, "trim") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: trim requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # trim: remove whitespace from both ends\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: string pointer\n");
                fprintf(output, "    call mlp_string_trim\n");
                fprintf(output, "    movq %%rax, %%r8     # Result in %%r8\n");
            }
            return;
        }
        
        // trimStart(string) - remove leading whitespace
        if (strcmp(call->function_name, "trimStart") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: trimStart requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # trimStart: remove leading whitespace\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: string pointer\n");
                fprintf(output, "    call mlp_string_trimStart\n");
                fprintf(output, "    movq %%rax, %%r8     # Result in %%r8\n");
            }
            return;
        }
        
        // trimEnd(string) - remove trailing whitespace
        if (strcmp(call->function_name, "trimEnd") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: trimEnd requires exactly 1 argument\n");
                return;
            }
            
            fprintf(output, "    # trimEnd: remove trailing whitespace\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: string pointer\n");
                fprintf(output, "    call mlp_string_trimEnd\n");
                fprintf(output, "    movq %%rax, %%r8     # Result in %%r8\n");
            }
            return;
        }
        
        // YZ_31: Input functions
        
        // input() - read string from stdin (no arguments)
        if (strcmp(call->function_name, "input") == 0) {
            if (call->arg_count == 0) {
                // No prompt - just call mlp_input()
                fprintf(output, "    # input: read string from stdin\n");
                fprintf(output, "    call mlp_input\n");
                fprintf(output, "    movq %%rax, %%r8     # Result (string pointer) in %%r8\n");
            } else if (call->arg_count == 1) {
                // With prompt - call mlp_input_prompt(prompt)
                fprintf(output, "    # input: read string with prompt\n");
                if (call->arguments && call->arguments[0]) {
                    expression_generate_code(output, call->arguments[0], NULL);
                    fprintf(output, "    movq %%r8, %%rdi     # arg1: prompt string\n");
                    fprintf(output, "    call mlp_input_prompt\n");
                    fprintf(output, "    movq %%rax, %%r8     # Result (string pointer) in %%r8\n");
                }
            } else {
                fprintf(output, "    # ERROR: input() accepts 0 or 1 arguments\n");
            }
            return;
        }
        
        // input_numeric() - read numeric from stdin
        if (strcmp(call->function_name, "input_numeric") == 0) {
            if (call->arg_count == 0) {
                // No prompt - just call mlp_input_numeric()
                fprintf(output, "    # input_numeric: read number from stdin\n");
                fprintf(output, "    call mlp_input_numeric\n");
                fprintf(output, "    movq %%rax, %%r8     # Result (int64) in %%r8\n");
            } else if (call->arg_count == 1) {
                // With prompt - call mlp_input_numeric_prompt(prompt)
                fprintf(output, "    # input_numeric: read number with prompt\n");
                if (call->arguments && call->arguments[0]) {
                    expression_generate_code(output, call->arguments[0], NULL);
                    fprintf(output, "    movq %%r8, %%rdi     # arg1: prompt string\n");
                    fprintf(output, "    call mlp_input_numeric_prompt\n");
                    fprintf(output, "    movq %%rax, %%r8     # Result (int64) in %%r8\n");
                }
            } else {
                fprintf(output, "    # ERROR: input_numeric() accepts 0 or 1 arguments\n");
            }
            return;
        }
        
        // YZ_33: Phase 9 - File I/O functions
        
        // read_file(filename) - read entire file as string
        if (strcmp(call->function_name, "read_file") == 0) {
            if (call->arg_count != 1) {
                fprintf(output, "    # ERROR: read_file requires exactly 1 argument (filename)\n");
                return;
            }
            
            fprintf(output, "    # read_file: read file content as string\n");
            if (call->arguments && call->arguments[0]) {
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    movq %%r8, %%rdi     # arg1: filename string\n");
                fprintf(output, "    call mlp_read_file\n");
                fprintf(output, "    movq %%rax, %%r8     # Result (file content string) in %%r8\n");
            }
            return;
        }
        
        // write_file(filename, content) - write content to file (overwrite)
        if (strcmp(call->function_name, "write_file") == 0) {
            if (call->arg_count != 2) {
                fprintf(output, "    # ERROR: write_file requires exactly 2 arguments (filename, content)\n");
                return;
            }
            
            fprintf(output, "    # write_file: write content to file\n");
            if (call->arguments && call->arguments[0] && call->arguments[1]) {
                // First argument: filename
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    pushq %%r8           # Save filename\n");
                
                // Second argument: content
                expression_generate_code(output, call->arguments[1], NULL);
                fprintf(output, "    movq %%r8, %%rsi     # arg2: content string\n");
                fprintf(output, "    popq %%rdi           # arg1: filename\n");
                fprintf(output, "    call mlp_write_file\n");
                fprintf(output, "    movq %%rax, %%r8     # Result (1=success, 0=fail) in %%r8\n");
            }
            return;
        }
        
        // append_file(filename, content) - append content to file
        if (strcmp(call->function_name, "append_file") == 0) {
            if (call->arg_count != 2) {
                fprintf(output, "    # ERROR: append_file requires exactly 2 arguments (filename, content)\n");
                return;
            }
            
            fprintf(output, "    # append_file: append content to file\n");
            if (call->arguments && call->arguments[0] && call->arguments[1]) {
                // First argument: filename
                expression_generate_code(output, call->arguments[0], NULL);
                fprintf(output, "    pushq %%r8           # Save filename\n");
                
                // Second argument: content
                expression_generate_code(output, call->arguments[1], NULL);
                fprintf(output, "    movq %%r8, %%rsi     # arg2: content string\n");
                fprintf(output, "    popq %%rdi           # arg1: filename\n");
                fprintf(output, "    call mlp_append_file\n");
                fprintf(output, "    movq %%rax, %%r8     # Result (1=success, 0=fail) in %%r8\n");
            }
            return;
        }
    }
    
    // Standard user-defined function call
    // Evaluate arguments and place in registers/stack
    // x86-64 calling convention: rdi, rsi, rdx, rcx, r8, r9
    const char* arg_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    for (int i = 0; i < call->arg_count && i < 6; i++) {
        fprintf(output, "    # Argument %d\n", i);
        // Evaluate argument expression and result will be in %rax
        if (call->arguments && call->arguments[i]) {
            expression_generate_code(output, call->arguments[i], NULL);  // Arguments don't have local context
            // Move result from %rax to appropriate argument register
            fprintf(output, "    movq %%rax, %s\n", arg_regs[i]);
        } else {
            fprintf(output, "    movq $0, %s  # Default: null argument\n", arg_regs[i]);
        }
    }
    
    // Handle arguments beyond 6 (push to stack in reverse order)
    if (call->arg_count > 6) {
        for (int i = call->arg_count - 1; i >= 6; i--) {
            fprintf(output, "    # Stack argument %d\n", i);
            // Evaluate argument expression
            if (call->arguments && call->arguments[i]) {
                expression_generate_code(output, call->arguments[i], NULL);  // Arguments don't have local context
                fprintf(output, "    pushq %%rax\n");
            } else {
                fprintf(output, "    pushq $0  # Default: null argument\n");
            }
        }
    }
    
    // Align stack to 16 bytes if needed
    fprintf(output, "    # Align stack\n");
    fprintf(output, "    andq $-16, %%rsp\n");
    
    // Call the function
    fprintf(output, "    call %s\n", call->function_name);
    
    // Clean up stack arguments if any
    if (call->arg_count > 6) {
        int stack_cleanup = (call->arg_count - 6) * 8;
        fprintf(output, "    addq $%d, %%rsp\n", stack_cleanup);
    }
    
    // Return value is in %rax
    fprintf(output, "    # Return value in %%rax\n");
}

// Generate return statement
void function_generate_return(FILE* output, ReturnStatement* ret, void* context) {
    if (!ret) return;
    
    if (ret->return_value) {
        fprintf(output, "    # Evaluate return value\n");
        // Evaluate return expression and result will be in %rax
        expression_generate_code(output, ret->return_value, context);
    }
    
    // Jump to function epilogue (no label needed, fall through to epilogue)
    fprintf(output, "    # Return from function\n");
}
