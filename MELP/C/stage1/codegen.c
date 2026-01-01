#include "codegen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create codegen state
CodegenState* codegen_create() {
    CodegenState* state = malloc(sizeof(CodegenState));
    state->output_capacity = 65536; // 64KB initial
    state->output = malloc(state->output_capacity);
    state->output[0] = '\0';
    state->output_size = 0;
    state->indent_level = 0;
    return state;
}

void codegen_free(CodegenState* state) {
    if (state->output) free(state->output);
    free(state);
}

char* codegen_get_output(CodegenState* state) {
    return state->output;
}

// Emit code
static void emit(CodegenState* state, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Calculate needed size
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    // Resize if needed
    while (state->output_size + needed + 1 >= state->output_capacity) {
        state->output_capacity *= 2;
        state->output = realloc(state->output, state->output_capacity);
    }
    
    // Write
    int written = vsnprintf(state->output + state->output_size, 
                           state->output_capacity - state->output_size, 
                           format, args);
    state->output_size += written;
    
    va_end(args);
}

// Emit with indentation
static void emit_line(CodegenState* state, const char* format, ...) {
    // Indent
    for (int i = 0; i < state->indent_level; i++) {
        emit(state, "    ");
    }
    
    // Content
    va_list args;
    va_start(args, format);
    
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    while (state->output_size + needed + 2 >= state->output_capacity) {
        state->output_capacity *= 2;
        state->output = realloc(state->output, state->output_capacity);
    }
    
    int written = vsnprintf(state->output + state->output_size, 
                           state->output_capacity - state->output_size, 
                           format, args);
    state->output_size += written;
    emit(state, "\n");
    
    va_end(args);
}

// Map MLP type to C type
static const char* map_type(const char* mlp_type) {
    if (!mlp_type) return "void";
    if (strcmp(mlp_type, "Numeric") == 0) return "int64_t";
    if (strcmp(mlp_type, "String") == 0) return "char*";
    if (strcmp(mlp_type, "Boolean") == 0) return "bool";
    if (strcmp(mlp_type, "list") == 0) return "mlp_list_t*";
    return mlp_type; // Custom struct type
}

// Forward declarations
static void codegen_statement(CodegenState* state, ASTNode* node);
static void codegen_expression(CodegenState* state, ASTNode* node);

// Generate function
static void codegen_function(CodegenState* state, ASTNode* func) {
    emit_line(state, "");
    emit_line(state, "int64_t %s(", func->value);
    
    // Parameters (children before body)
    ASTNode* param = func->first_child;
    int param_count = 0;
    while (param && param->type != AST_BLOCK) {
        if (param_count > 0) emit(state, ", ");
        emit(state, "int64_t %s", param->value);
        param = param->next_sibling;
        param_count++;
    }
    
    if (param_count == 0) {
        emit(state, "void");
    }
    emit(state, ") {\n");
    
    // Body
    state->indent_level++;
    ASTNode* body = param ? param : func->first_child;
    if (body && body->type == AST_BLOCK) {
        ASTNode* stmt = body->first_child;
        while (stmt) {
            codegen_statement(state, stmt);
            stmt = stmt->next_sibling;
        }
    }
    state->indent_level--;
    
    emit_line(state, "}");
}

// Generate struct
static void codegen_struct(CodegenState* state, ASTNode* struct_node) {
    emit_line(state, "");
    emit_line(state, "typedef struct {");
    state->indent_level++;
    
    ASTNode* field = struct_node->first_child;
    while (field) {
        if (field->type == AST_VARIABLE_DECL) {
            const char* c_type = map_type(field->data_type);
            emit_line(state, "%s %s;", c_type, field->value);
        }
        field = field->next_sibling;
    }
    
    state->indent_level--;
    emit_line(state, "} %s;", struct_node->value);
}

// Generate variable declaration
static void codegen_variable_decl(CodegenState* state, ASTNode* var) {
    const char* c_type = map_type(var->data_type);
    
    if (var->right) {
        // With initialization
        emit_line(state, "%s %s = ", c_type, var->value);
        codegen_expression(state, var->right);
        emit(state, ";");
    } else {
        // Without initialization
        emit_line(state, "%s %s;", c_type, var->value);
    }
}

// Generate assignment
static void codegen_assignment(CodegenState* state, ASTNode* assign) {
    for (int i = 0; i < state->indent_level; i++) emit(state, "    ");
    
    // Left side (identifier or member access)
    if (assign->left->type == AST_MEMBER_ACCESS) {
        codegen_expression(state, assign->left);
    } else {
        emit(state, "%s", assign->left->value);
    }
    
    emit(state, " = ");
    codegen_expression(state, assign->right);
    emit(state, ";\n");
}

// Generate if statement
static void codegen_if(CodegenState* state, ASTNode* if_node) {
    emit_line(state, "if (");
    codegen_expression(state, if_node->left);
    emit(state, ") {");
    
    // Then block
    state->indent_level++;
    if (if_node->right && if_node->right->type == AST_BLOCK) {
        ASTNode* stmt = if_node->right->first_child;
        while (stmt) {
            codegen_statement(state, stmt);
            stmt = stmt->next_sibling;
        }
    }
    state->indent_level--;
    
    // Else block
    ASTNode* else_block = if_node->right ? if_node->right->next_sibling : NULL;
    if (else_block && else_block->type == AST_BLOCK) {
        emit_line(state, "} else {");
        state->indent_level++;
        ASTNode* stmt = else_block->first_child;
        while (stmt) {
            codegen_statement(state, stmt);
            stmt = stmt->next_sibling;
        }
        state->indent_level--;
    }
    
    emit_line(state, "}");
}

// Generate while loop
static void codegen_while(CodegenState* state, ASTNode* while_node) {
    emit_line(state, "while (");
    codegen_expression(state, while_node->left);
    emit(state, ") {");
    
    state->indent_level++;
    if (while_node->right && while_node->right->type == AST_BLOCK) {
        ASTNode* stmt = while_node->right->first_child;
        while (stmt) {
            codegen_statement(state, stmt);
            stmt = stmt->next_sibling;
        }
    }
    state->indent_level--;
    
    emit_line(state, "}");
}

// Generate return
static void codegen_return(CodegenState* state, ASTNode* ret) {
    emit_line(state, "return");
    if (ret->left) {
        emit(state, " ");
        codegen_expression(state, ret->left);
    }
    emit(state, ";");
}

// Generate expression
static void codegen_expression(CodegenState* state, ASTNode* expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case AST_NUMBER:
            emit(state, "%s", expr->value);
            break;
            
        case AST_STRING:
            emit(state, "\"%s\"", expr->value);
            break;
            
        case AST_IDENTIFIER:
            emit(state, "%s", expr->value);
            break;
            
        case AST_BINARY_OP:
            emit(state, "(");
            codegen_expression(state, expr->left);
            emit(state, " %s ", expr->value);
            codegen_expression(state, expr->right);
            emit(state, ")");
            break;
            
        case AST_CALL:
            // Check for list methods
            if (expr->left && strcmp(expr->value, "append") == 0) {
                emit(state, "mlp_list_append(");
                codegen_expression(state, expr->left);
                emit(state, ", ");
                ASTNode* arg = expr->first_child;
                if (arg) codegen_expression(state, arg);
                emit(state, ")");
            } else if (expr->left && strcmp(expr->value, "length") == 0) {
                emit(state, "mlp_list_length(");
                codegen_expression(state, expr->left);
                emit(state, ")");
            } else {
                // Regular function call
                emit(state, "%s(", expr->value);
                ASTNode* arg = expr->first_child;
                int arg_count = 0;
                while (arg) {
                    if (arg_count > 0) emit(state, ", ");
                    codegen_expression(state, arg);
                    arg = arg->next_sibling;
                    arg_count++;
                }
                emit(state, ")");
            }
            break;
            
        case AST_MEMBER_ACCESS:
            codegen_expression(state, expr->left);
            emit(state, ".%s", expr->value);
            break;
            
        case AST_LIST_INIT:
            emit(state, "mlp_list_create()");
            break;
            
        default:
            break;
    }
}

// Generate statement
static void codegen_statement(CodegenState* state, ASTNode* stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case AST_FUNCTION:
            codegen_function(state, stmt);
            break;
            
        case AST_STRUCT:
            codegen_struct(state, stmt);
            break;
            
        case AST_VARIABLE_DECL:
            codegen_variable_decl(state, stmt);
            break;
            
        case AST_ASSIGN:
            codegen_assignment(state, stmt);
            break;
            
        case AST_IF:
            codegen_if(state, stmt);
            break;
            
        case AST_WHILE:
            codegen_while(state, stmt);
            break;
            
        case AST_RETURN:
            codegen_return(state, stmt);
            break;
            
        case AST_CALL:
            // Expression statement (function call)
            for (int i = 0; i < state->indent_level; i++) emit(state, "    ");
            codegen_expression(state, stmt);
            emit(state, ";\n");
            break;
            
        default:
            break;
    }
}

// Generate program
void codegen_program(CodegenState* state, ASTNode* program) {
    // Header
    emit(state, "// Generated by MLP Stage1 Compiler\n");
    emit(state, "#include <stdint.h>\n");
    emit(state, "#include <stdbool.h>\n");
    emit(state, "#include <stdio.h>\n");
    emit(state, "#include <stdlib.h>\n");
    emit(state, "#include <string.h>\n");
    emit(state, "#include \"mlp_runtime.h\"\n");
    emit(state, "\n");
    
    // Generate all statements
    ASTNode* stmt = program->first_child;
    while (stmt) {
        codegen_statement(state, stmt);
        stmt = stmt->next_sibling;
    }
}
