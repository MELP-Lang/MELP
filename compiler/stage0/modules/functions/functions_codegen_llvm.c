// LLVM IR Code Generator for Functions
// Wrapper around llvm_backend module for functions compiler
// YZ_58 - Phase 13.5 Part 3-5
// YZ_203.5 - Generic Type Inference

#include "functions_codegen_llvm.h"
#include "../type_system/type_inference.h"
#include "functions_generic.h"  // YZ_203: Generic template support
#include "../array/arithmetic/arithmetic_parser.h"
#include "../variable/variable.h"
#include "../statement/statement.h"  // YZ_58: Statement types
#include "../comparison/comparison.h"  // YZ_58: Comparison expressions
#include "../control_flow/control_flow.h"  // YZ_58: Control flow structures
#include "../for_loop/for_loop.h"  // YZ_60: For loop structures
#include "../print/print.h"  // YZ_61: Print statement
#include <stdlib.h>
#include <string.h>

// YZ_200: Variable type tracking helpers
static void register_variable_type(FunctionLLVMContext* ctx, const char* name, int is_pointer) {
    // Grow array if needed
    if (ctx->var_type_count >= ctx->var_type_capacity) {
        ctx->var_type_capacity = ctx->var_type_capacity == 0 ? 16 : ctx->var_type_capacity * 2;
        ctx->var_types = realloc(ctx->var_types, sizeof(VarTypeEntry) * ctx->var_type_capacity);
    }
    
    // Add entry
    ctx->var_types[ctx->var_type_count].name = strdup(name);
    ctx->var_types[ctx->var_type_count].is_pointer = is_pointer;
    ctx->var_type_count++;
}

static int lookup_variable_type(FunctionLLVMContext* ctx, const char* name) {
    for (int i = 0; i < ctx->var_type_count; i++) {
        if (strcmp(ctx->var_types[i].name, name) == 0) {
            return ctx->var_types[i].is_pointer;
        }
    }
    return 0;  // Default: i64 (not pointer)
}

static void clear_variable_types(FunctionLLVMContext* ctx) {
    for (int i = 0; i < ctx->var_type_count; i++) {
        free(ctx->var_types[i].name);
    }
    ctx->var_type_count = 0;
}

// Initialize LLVM codegen context
FunctionLLVMContext* function_llvm_context_create(FILE* output, GenericRegistry* registry) {
    FunctionLLVMContext* ctx = malloc(sizeof(FunctionLLVMContext));
    ctx->llvm_ctx = llvm_context_create(output);
    ctx->current_func = NULL;
    ctx->globals_emitted = 0;  // YZ_61: Initialize flag
    
    // YZ_200: Initialize variable type tracking
    ctx->var_types = NULL;
    ctx->var_type_count = 0;
    ctx->var_type_capacity = 0;
    
    // YZ_203: Store generic registry reference
    ctx->generic_registry = registry;
    
    return ctx;
}

// Free LLVM codegen context
void function_llvm_context_free(FunctionLLVMContext* ctx) {
    if (ctx) {
        // YZ_200: Free variable type tracking
        clear_variable_types(ctx);
        if (ctx->var_types) {
            free(ctx->var_types);
        }
        
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
static void scan_statement_for_variables(FunctionDeclaration* func, Statement* stmt);

// YZ_65: Scan statements to populate local_vars registry
static void scan_statement_for_variables(FunctionDeclaration* func, Statement* stmt) {
    if (!stmt) return;
    
    // Register variable declaration
    if (stmt->type == STMT_VARIABLE_DECL) {
        VariableDeclaration* decl = (VariableDeclaration*)stmt->data;
        if (decl && decl->name) {
            // YZ_200: Register with type flag (1=numeric, 0=string/list)
            // Also set is_list flag for list variables
            int is_numeric = (decl->type != VAR_STRING && decl->type != VAR_LIST) ? 1 : 0;
            function_register_local_var_with_type(func, decl->name, is_numeric);
            
            // YZ_200: Mark list variables
            if (decl->type == VAR_LIST) {
                LocalVariable* var = func->local_vars;
                while (var) {
                    if (strcmp(var->name, decl->name) == 0) {
                        var->is_list = 1;
                        break;
                    }
                    var = var->next;
                }
            }
            
            // YZ_202: Mark nullable variables
            if (decl->is_nullable) {
                LocalVariable* var = func->local_vars;
                while (var) {
                    if (strcmp(var->name, decl->name) == 0) {
                        var->is_nullable = 1;
                        break;
                    }
                    var = var->next;
                }
            }
        }
    }
    
    // Recursively scan nested blocks
    if (stmt->type == STMT_IF) {
        IfStatement* if_stmt = (IfStatement*)stmt->data;
        if (if_stmt) {
            Statement* nested = if_stmt->then_body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
            if (if_stmt->has_else) {
                nested = if_stmt->else_body;
                while (nested) {
                    scan_statement_for_variables(func, nested);
                    nested = nested->next;
                }
            }
        }
    }
    
    if (stmt->type == STMT_WHILE) {
        WhileStatement* while_stmt = (WhileStatement*)stmt->data;
        if (while_stmt) {
            Statement* nested = while_stmt->body;
            while (nested) {
                scan_statement_for_variables(func, nested);
                nested = nested->next;
            }
        }
    }
    
    // Scan next statement
    if (stmt->next) {
        scan_statement_for_variables(func, stmt->next);
    }
}

// Generate LLVM IR for expression
static LLVMValue* generate_expression_llvm(FunctionLLVMContext* ctx, void* expr) {
    if (!expr) {
        return llvm_const_i64(0);
    }
    
    ArithmeticExpr* arith = (ArithmeticExpr*)expr;
    
    // YZ_200: Handle collection literals (list, array, tuple)
    if (arith->is_collection && arith->collection) {
        Collection* coll = arith->collection;
        
        if (coll->type == COLL_LIST) {
            // Generate list allocation and initialization
            // 1. Call melp_list_create(element_size)
            LLVMValue* element_size = llvm_const_i64(sizeof(int64_t));  // For now, numeric only
            LLVMValue* list_ptr = llvm_emit_call(ctx->llvm_ctx, "melp_list_create", &element_size, 1);
            list_ptr->type = LLVM_TYPE_I8_PTR;  // Fix: melp_list_create returns i8*
            
            // 2. For each element, call melp_list_append(list, &element)
            List* list = &coll->data.list;
            for (int i = 0; i < list->length; i++) {
                // Generate element value
                ArithmeticExpr* elem_expr = (ArithmeticExpr*)list->elements[i];
                LLVMValue* elem_val = generate_expression_llvm(ctx, elem_expr);
                
                // YZ_200: Allocate memory for element (runtime expects void**)
                // malloc(8) for i64 element
                LLVMValue* size_arg = llvm_const_i64(8);
                LLVMValue* elem_mem = llvm_emit_call(ctx->llvm_ctx, "malloc", &size_arg, 1);
                elem_mem->type = LLVM_TYPE_I8_PTR;
                
                // Bitcast i8* to i64* and store value
                char* cast_temp = llvm_new_temp(ctx->llvm_ctx);
                fprintf(ctx->llvm_ctx->output, "    %s = bitcast i8* %s to i64*\n",
                        cast_temp, elem_mem->name);
                
                fprintf(ctx->llvm_ctx->output, "    store i64 ");
                if (elem_val->is_constant) {
                    fprintf(ctx->llvm_ctx->output, "%ld", elem_val->const_value);
                } else {
                    fprintf(ctx->llvm_ctx->output, "%s", elem_val->name);
                }
                fprintf(ctx->llvm_ctx->output, ", i64* %s\n", cast_temp);
                
                // Call melp_list_append(list_ptr, elem_mem) with i8* pointer
                LLVMValue* args[2] = {list_ptr, elem_mem};
                llvm_emit_call(ctx->llvm_ctx, "melp_list_append", args, 2);
                
                llvm_value_free(elem_val);
            }
            
            // 3. Return list pointer
            return list_ptr;
        }
        
        if (coll->type == COLL_MAP) {
            // YZ_201: Generate map allocation and initialization
            // 1. Call melp_map_create(value_size)
            LLVMValue* value_size = llvm_const_i64(sizeof(int64_t));  // For now, numeric values only
            LLVMValue* map_ptr = llvm_emit_call(ctx->llvm_ctx, "melp_map_create", &value_size, 1);
            map_ptr->type = LLVM_TYPE_I8_PTR;  // melp_map_create returns i8*
            
            // 2. For each key-value pair, call melp_map_insert(map, key, &value)
            Map* map = &coll->data.map;
            for (int i = 0; i < map->length; i++) {
                // Generate key string pointer
                char* key_str = map->keys[i];
                char* key_global = llvm_emit_string_global(ctx->llvm_ctx, key_str);
                size_t key_len = strlen(key_str) + 1;
                LLVMValue* key_ptr = llvm_emit_string_ptr(ctx->llvm_ctx, key_global, key_len);
                free(key_global);
                
                // Generate value expression
                ArithmeticExpr* val_expr = (ArithmeticExpr*)map->values[i];
                LLVMValue* val = generate_expression_llvm(ctx, val_expr);
                
                // Allocate memory for value (runtime expects void*)
                LLVMValue* size_arg = llvm_const_i64(8);  // i64 = 8 bytes
                LLVMValue* val_mem = llvm_emit_call(ctx->llvm_ctx, "malloc", &size_arg, 1);
                val_mem->type = LLVM_TYPE_I8_PTR;
                
                // Bitcast i8* to i64* and store value
                char* cast_temp = llvm_new_temp(ctx->llvm_ctx);
                fprintf(ctx->llvm_ctx->output, "    %s = bitcast i8* %s to i64*\n",
                        cast_temp, val_mem->name);
                
                fprintf(ctx->llvm_ctx->output, "    store i64 ");
                if (val->is_constant) {
                    fprintf(ctx->llvm_ctx->output, "%ld", val->const_value);
                } else {
                    fprintf(ctx->llvm_ctx->output, "%s", val->name);
                }
                fprintf(ctx->llvm_ctx->output, ", i64* %s\n", cast_temp);
                
                // Call melp_map_insert(map_ptr, key_ptr, val_mem)
                LLVMValue* args[3] = {map_ptr, key_ptr, val_mem};
                llvm_emit_call(ctx->llvm_ctx, "melp_map_insert", args, 3);
                
                llvm_value_free(val);
                llvm_value_free(key_ptr);
            }
            
            // 3. Return map pointer
            return map_ptr;
        }
        
        // Other collection types: return 0 for now
        return llvm_const_i64(0);
    }
    
    // Handle literals
    if (arith->is_literal) {
        // YZ_07: If value is NULL, return 0 (likely collection or uninitialized)
        if (!arith->value) {
            return llvm_const_i64(0);
        }
        
        // YZ_64: Handle string literals
        if (arith->is_string) {
            // Register string global and get global name
            char* global_name = llvm_emit_string_global(ctx->llvm_ctx, arith->value);
            
            // Emit getelementptr to get i8* pointer
            size_t str_len = strlen(arith->value) + 1;  // +1 for null terminator
            LLVMValue* str_ptr = llvm_emit_string_ptr(ctx->llvm_ctx, global_name, str_len);
            
            free(global_name);
            return str_ptr;
        }
        
        // Handle boolean literals
        if (strcmp(arith->value, "true") == 0) {
            return llvm_const_i64(1);
        }
        if (strcmp(arith->value, "false") == 0) {
            return llvm_const_i64(0);
        }
        
        // YZ_202: Handle null literal
        if (strcmp(arith->value, "null") == 0) {
            return "null";  // Will be converted to appropriate type (i8* null, MelpOptional* null)
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
        FunctionParamType param_type = FUNC_PARAM_NUMERIC;  // Default
        
        while (param) {
            if (strcmp(param->name, arith->value) == 0) {
                is_param = 1;
                param_type = param->type;
                break;
            }
            param = param->next;
        }
        
        if (is_param) {
            // Parameter - already a value, just return register reference
            LLVMValue* val = llvm_reg(arith->value);
            // YZ_64: Set type based on parameter type
            // YZ_21: List parameters are pointers (i8*)
            val->type = (param_type == FUNC_PARAM_TEXT || param_type == FUNC_PARAM_LIST) ? LLVM_TYPE_I8_PTR : LLVM_TYPE_I64;
            return val;
        } else {
            // YZ_200: Local variable - use type table for correct load
            int is_pointer = lookup_variable_type(ctx, arith->value);
            
            if (is_pointer) {
                // Pointer variable (string/list): load i8* from i8**
                LLVMValue* loaded = malloc(sizeof(LLVMValue));
                loaded->name = llvm_new_temp(ctx->llvm_ctx);
                loaded->is_constant = 0;
                loaded->type = LLVM_TYPE_I8_PTR;
                
                fprintf(ctx->llvm_ctx->output, "    %s = load i8*, i8** %%%s, align 8\n",
                        loaded->name, arith->value);
                
                return loaded;
            } else {
                // Numeric variable - load i64 from i64*
                LLVMValue* var_ptr = llvm_reg(arith->value);
                LLVMValue* loaded = llvm_emit_load(ctx->llvm_ctx, var_ptr);
                llvm_value_free(var_ptr);
                return loaded;
            }
        }
    }
    
    // YZ_200: Handle array/list indexing: arr[i], list(i)
    if (arith->is_array_access && arith->array_access) {
        IndexAccess* access = arith->array_access;
        
        // Load collection pointer from variable
        // Check if it's a list (i8*) or array
        int is_pointer = lookup_variable_type(ctx, access->collection_name);
        
        if (is_pointer) {
            // List indexing: call melp_list_get(list, index)
            // 1. Load list pointer
            LLVMValue* list_ptr = malloc(sizeof(LLVMValue));
            list_ptr->name = llvm_new_temp(ctx->llvm_ctx);
            list_ptr->is_constant = 0;
            list_ptr->type = LLVM_TYPE_I8_PTR;
            fprintf(ctx->llvm_ctx->output, "    %s = load i8*, i8** %%%s, align 8\n",
                    list_ptr->name, access->collection_name);
            
            // 2. Generate index expression (handle different index types)
            LLVMValue* index_val = NULL;
            if (access->index_type == 0) {
                // Constant index
                index_val = llvm_const_i64(access->index.const_index);
            } else if (access->index_type == 1) {
                // Variable index
                LLVMValue* var_ptr = llvm_reg(access->index.var_index);
                index_val = llvm_emit_load(ctx->llvm_ctx, var_ptr);
                llvm_value_free(var_ptr);
            } else {
                // Expression index
                index_val = generate_expression_llvm(ctx, (ArithmeticExpr*)access->index.expr_index);
            }
            
            // 3. Call melp_list_get(list, index) -> returns i8* (pointer to data)
            LLVMValue* args[2] = {list_ptr, index_val};
            LLVMValue* elem_ptr = llvm_emit_call(ctx->llvm_ctx, "melp_list_get", args, 2);
            elem_ptr->type = LLVM_TYPE_I8_PTR;
            
            // 4. Bitcast i8* to i64* and load value
            char* cast_temp = llvm_new_temp(ctx->llvm_ctx);
            fprintf(ctx->llvm_ctx->output, "    %s = bitcast i8* %s to i64*\n",
                    cast_temp, elem_ptr->name);
            
            // 5. Load i64 value
            LLVMValue* result = malloc(sizeof(LLVMValue));
            result->name = llvm_new_temp(ctx->llvm_ctx);
            result->is_constant = 0;
            result->type = LLVM_TYPE_I64;
            fprintf(ctx->llvm_ctx->output, "    %s = load i64, i64* %s\n",
                    result->name, cast_temp);
            
            llvm_value_free(list_ptr);
            llvm_value_free(index_val);
            llvm_value_free(elem_ptr);
            return result;
        } else {
            // Array indexing: legacy assembly code (not implemented in LLVM yet)
            // TODO: Implement array indexing in LLVM
            return llvm_const_i64(0);
        }
    }
    
    // Handle function calls
    if (arith->is_function_call && arith->func_call) {
        FunctionCallExpr* call = arith->func_call;
        
        // YZ_203.5: Handle generic function calls WITHOUT explicit type arguments
        // Check if this might be a generic function call (no type args provided)
        if (call->type_arg_count == 0 && ctx->generic_registry) {
            // Try to find a generic template with this name
            GenericTemplate* template = generic_find_template(
                ctx->generic_registry,
                call->function_name
            );
            
            if (template) {
                // This is a generic call without explicit types → infer them!
                printf("🔍 Inferring types for generic call: %s(...)\n", call->function_name);
                
                // Perform type inference directly from ArithmeticExpr arguments
                int inferred_count = 0;
                char** inferred_types = type_inference_infer_from_args(
                    template,
                    call->arguments,
                    call->arg_count,
                    &inferred_count
                );
                
                if (inferred_types && inferred_count > 0) {
                    // Success! Use inferred types for instantiation
                    call->type_arguments = inferred_types;
                    call->type_arg_count = inferred_count;
                    // Continue to YZ_203's instantiation code below
                } else {
                    fprintf(stderr, "Error: Type inference failed for '%s'\n", call->function_name);
                    return llvm_const_i64(0);
                }
            }
        }
        
        // YZ_203: Handle generic function instantiation
        const char* actual_function_name = call->function_name;
        if (call->type_arg_count > 0 && ctx->generic_registry) {
            // This is a generic function call with explicit type arguments
            // 1. Find template in registry
            GenericTemplate* template = generic_find_template(
                ctx->generic_registry, 
                call->function_name
            );
            
            if (!template) {
                fprintf(stderr, "Error: Generic template '%s' not found\n", 
                        call->function_name);
                return llvm_const_i64(0);
            }
            
            // 2. Check if this specialization already exists
            GenericInstance* instance = generic_find_instance(
                template, 
                call->type_arguments, 
                call->type_arg_count
            );
            
            if (!instance) {
                // 3. Create new specialization (monomorphization)
                instance = generic_instantiate(
                    ctx->generic_registry,
                    call->function_name,
                    call->type_arguments, 
                    call->type_arg_count
                );
                
                if (!instance) {
                    fprintf(stderr, "Error: Failed to instantiate generic function '%s'\n",
                            call->function_name);
                    return llvm_const_i64(0);
                }
                
                // 4. Generate LLVM IR for specialized function
                FunctionDeclaration* saved_func = ctx->current_func;
                ctx->current_func = instance->specialized_func;
                function_generate_declaration_llvm(ctx, instance->specialized_func);
                ctx->current_func = saved_func;
                
                // Mark as emitted
                instance->is_emitted = 1;
            } else if (!instance->is_emitted) {
                // Instance exists but not yet emitted - emit it now
                FunctionDeclaration* saved_func = ctx->current_func;
                ctx->current_func = instance->specialized_func;
                function_generate_declaration_llvm(ctx, instance->specialized_func);
                ctx->current_func = saved_func;
                instance->is_emitted = 1;
            }
            
            // 5. Use mangled name for the call
            actual_function_name = instance->mangled_name;
        }
        
        // YZ_200: Check if this is actually a list indexing operation
        // If function name matches a list variable, treat as list access
        int is_pointer = lookup_variable_type(ctx, actual_function_name);
        if (is_pointer && call->arg_count == 1) {
            // This is list indexing: list_var(index)
            // 1. Load list pointer
            LLVMValue* list_ptr = malloc(sizeof(LLVMValue));
            list_ptr->name = llvm_new_temp(ctx->llvm_ctx);
            list_ptr->is_constant = 0;
            list_ptr->type = LLVM_TYPE_I8_PTR;
            fprintf(ctx->llvm_ctx->output, "    %s = load i8*, i8** %%%s, align 8\n",
                    list_ptr->name, call->function_name);
            
            // 2. Generate index expression
            LLVMValue* index_val = generate_expression_llvm(ctx, call->arguments[0]);
            
            // 3. Call melp_list_get(list, index) -> returns i8* (void*)
            LLVMValue* args[2] = {list_ptr, index_val};
            LLVMValue* elem_ptr = llvm_emit_call(ctx->llvm_ctx, "melp_list_get", args, 2);
            elem_ptr->type = LLVM_TYPE_I8_PTR;
            
            // 4. Bitcast i8* to i64* and load value
            char* cast_temp = llvm_new_temp(ctx->llvm_ctx);
            fprintf(ctx->llvm_ctx->output, "    %s = bitcast i8* %s to i64*\n",
                    cast_temp, elem_ptr->name);
            
            // 5. Load i64 value from pointer
            LLVMValue* result = malloc(sizeof(LLVMValue));
            result->name = llvm_new_temp(ctx->llvm_ctx);
            result->is_constant = 0;
            result->type = LLVM_TYPE_I64;
            fprintf(ctx->llvm_ctx->output, "    %s = load i64, i64* %s\n",
                    result->name, cast_temp);
            
            llvm_value_free(list_ptr);
            llvm_value_free(index_val);
            llvm_value_free(elem_ptr);
            return result;
        }
        
        // Special case: println
        if (strcmp(actual_function_name, "println") == 0 && call->arg_count == 1) {
            LLVMValue* arg = generate_expression_llvm(ctx, call->arguments[0]);
            LLVMValue* result = llvm_emit_println(ctx->llvm_ctx, arg);
            // Don't free arg - println returns it
            return result;
        }
        
        // YZ_200: Map built-in list functions to runtime functions
        // YZ_201: Map built-in map functions to runtime functions
        const char* runtime_name = actual_function_name;
        int is_list_append = 0;
        int is_map_insert = 0;
        
        if (strcmp(actual_function_name, "append") == 0) {
            runtime_name = "melp_list_append";
            is_list_append = 1;
        } else if (strcmp(actual_function_name, "prepend") == 0) {
            runtime_name = "melp_list_prepend";
            is_list_append = 1;  // prepend also needs i64->i8* conversion
        } else if (strcmp(actual_function_name, "length") == 0) {
            // Check if first arg is map or list (both have length())
            // For now, assume melp_map_length if name suggests map
            // Better: check variable type from var_types
            runtime_name = "melp_list_length";  // Default to list, will handle map below
        } else if (strcmp(actual_function_name, "clear") == 0) {
            runtime_name = "melp_list_clear";
        } else if (strcmp(actual_function_name, "insert") == 0) {
            runtime_name = "melp_map_insert";
            is_map_insert = 1;
        } else if (strcmp(actual_function_name, "get") == 0) {
            runtime_name = "melp_map_get";
        } else if (strcmp(actual_function_name, "remove") == 0) {
            runtime_name = "melp_map_remove";
        } else if (strcmp(actual_function_name, "has_key") == 0) {
            runtime_name = "melp_map_has_key";
        }
        
        // Generate arguments
        LLVMValue** args = malloc(sizeof(LLVMValue*) * call->arg_count);
        for (int i = 0; i < call->arg_count; i++) {
            args[i] = generate_expression_llvm(ctx, call->arguments[i]);
            
            // YZ_200: For append/prepend, convert second argument (element) to pointer
            // Runtime expects void* pointing to element data
            if (is_list_append && i == 1) {
                if (args[i]->type == LLVM_TYPE_I64) {
                    // Allocate stack space for element with unique name
                    char* elem_var = llvm_new_temp(ctx->llvm_ctx);
                    fprintf(ctx->llvm_ctx->output, "    %s = alloca i64, align 8\n", elem_var);
                    // Store value
                    fprintf(ctx->llvm_ctx->output, "    store i64 ");
                    if (args[i]->is_constant) {
                        fprintf(ctx->llvm_ctx->output, "%ld", args[i]->const_value);
                    } else {
                        fprintf(ctx->llvm_ctx->output, "%s", args[i]->name);
                    }
                    fprintf(ctx->llvm_ctx->output, ", i64* %s, align 8\n", elem_var);
                    
                    // Cast to i8* for runtime call
                    LLVMValue* ptr_val = malloc(sizeof(LLVMValue));
                    ptr_val->name = llvm_new_temp(ctx->llvm_ctx);
                    ptr_val->is_constant = 0;
                    ptr_val->type = LLVM_TYPE_I8_PTR;
                    fprintf(ctx->llvm_ctx->output, "    %s = bitcast i64* %s to i8*\n",
                            ptr_val->name, elem_var);
                    llvm_value_free(args[i]);
                    args[i] = ptr_val;
                }
            }
            
            // YZ_201: For map insert, convert third argument (value) to pointer
            // Runtime expects void* pointing to value data
            if (is_map_insert && i == 2) {
                if (args[i]->type == LLVM_TYPE_I64) {
                    // Allocate stack space for value
                    char* val_var = llvm_new_temp(ctx->llvm_ctx);
                    fprintf(ctx->llvm_ctx->output, "    %s = alloca i64, align 8\n", val_var);
                    // Store value
                    fprintf(ctx->llvm_ctx->output, "    store i64 ");
                    if (args[i]->is_constant) {
                        fprintf(ctx->llvm_ctx->output, "%ld", args[i]->const_value);
                    } else {
                        fprintf(ctx->llvm_ctx->output, "%s", args[i]->name);
                    }
                    fprintf(ctx->llvm_ctx->output, ", i64* %s, align 8\n", val_var);
                    
                    // Cast to i8* for runtime call
                    LLVMValue* ptr_val = malloc(sizeof(LLVMValue));
                    ptr_val->name = llvm_new_temp(ctx->llvm_ctx);
                    ptr_val->is_constant = 0;
                    ptr_val->type = LLVM_TYPE_I8_PTR;
                    fprintf(ctx->llvm_ctx->output, "    %s = bitcast i64* %s to i8*\n",
                            ptr_val->name, val_var);
                    llvm_value_free(args[i]);
                    args[i] = ptr_val;
                }
            }
        }
        
        LLVMValue* result = llvm_emit_call(ctx->llvm_ctx, runtime_name, args, call->arg_count);
        
        // Free args
        for (int i = 0; i < call->arg_count; i++) {
            llvm_value_free(args[i]);
        }
        free(args);
        
        return result;
    }
    
    // Handle binary operations
    if (arith->left && arith->right) {
        // YZ_202: Special handling for ?? (null coalesce)
        if (arith->value && strcmp(arith->value, "??") == 0) {
            // Null coalescing: left ?? right
            // Generate: left_value ? left_value : right_value
            LLVMValue* left = generate_expression_llvm(ctx, arith->left);
            
            // Check if left is null (compare with 0 for now)
            LLVMValue* is_null = llvm_emit_icmp(ctx->llvm_ctx, "eq", left, llvm_const_i64(0));
            
            // If null, evaluate right; otherwise use left
            LLVMValue* right = generate_expression_llvm(ctx, arith->right);
            LLVMValue* result = llvm_emit_select(ctx->llvm_ctx, is_null, right, left);
            
            llvm_value_free(left);
            llvm_value_free(right);
            llvm_value_free(is_null);
            
            return result;
        }
        
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
            
            // YZ_200: Handle list variables
            if (decl->type == VAR_LIST) {
                // Register variable type
                register_variable_type(ctx, decl->name, 1);  // 1 = pointer (i8*)
                
                // List variable: list numbers = (1; 2; 3;)
                // Allocate i8* pointer on stack for list pointer
                char var_ptr_name[256];
                snprintf(var_ptr_name, sizeof(var_ptr_name), "%%%s", decl->name);
                fprintf(ctx->llvm_ctx->output, "  %s = alloca i8*, align 8\n", var_ptr_name);
                
                // Generate initializer expression (list literal)
                if (decl->init_expr) {
                    LLVMValue* list_ptr = generate_expression_llvm(ctx, decl->init_expr);
                    
                    // Store list pointer to variable
                    fprintf(ctx->llvm_ctx->output, "  store i8* ");
                    if (list_ptr->is_constant) {
                        fprintf(ctx->llvm_ctx->output, "null");  // NULL list
                    } else {
                        fprintf(ctx->llvm_ctx->output, "%s", list_ptr->name);
                    }
                    fprintf(ctx->llvm_ctx->output, ", i8** %s, align 8\n", var_ptr_name);
                    
                    llvm_value_free(list_ptr);
                } else {
                    // No initializer - store NULL
                    fprintf(ctx->llvm_ctx->output, "  store i8* null, i8** %s, align 8\n", var_ptr_name);
                }
                
                return NULL;
            }
            
            // YZ_201: Handle map variables
            if (decl->type == VAR_MAP) {
                // Register variable type
                register_variable_type(ctx, decl->name, 1);  // 1 = pointer (i8*)
                
                // Map variable: map person = {"name": "Alice"; "age": "30";}
                // Allocate i8* pointer on stack for map pointer
                char var_ptr_name[256];
                snprintf(var_ptr_name, sizeof(var_ptr_name), "%%%s", decl->name);
                fprintf(ctx->llvm_ctx->output, "  %s = alloca i8*, align 8\n", var_ptr_name);
                
                // Generate initializer expression (map literal)
                if (decl->init_expr) {
                    LLVMValue* map_ptr = generate_expression_llvm(ctx, decl->init_expr);
                    
                    // Store map pointer to variable
                    fprintf(ctx->llvm_ctx->output, "  store i8* ");
                    if (map_ptr->is_constant) {
                        fprintf(ctx->llvm_ctx->output, "null");  // NULL map
                    } else {
                        fprintf(ctx->llvm_ctx->output, "%s", map_ptr->name);
                    }
                    fprintf(ctx->llvm_ctx->output, ", i8** %s, align 8\n", var_ptr_name);
                    
                    llvm_value_free(map_ptr);
                } else {
                    // No initializer - store NULL
                    fprintf(ctx->llvm_ctx->output, "  store i8* null, i8** %s, align 8\n", var_ptr_name);
                }
                
                return NULL;
            }
            
            // YZ_62: Phase 17 - Handle string variables
            if (decl->type == VAR_STRING && decl->value) {
                // Register variable type
                register_variable_type(ctx, decl->name, 1);  // 1 = pointer (i8*)
                
                // String variable: string x = "hello"
                // Allocate i8* pointer on stack using variable name + _ptr suffix
                char var_ptr_name[256];
                snprintf(var_ptr_name, sizeof(var_ptr_name), "%%%s_ptr", decl->name);
                fprintf(ctx->llvm_ctx->output, "  %s = alloca i8*, align 8\n", var_ptr_name);
                
                // Create global string constant
                char* global_name = llvm_emit_string_global(ctx->llvm_ctx, decl->value);
                
                // Get pointer to first character
                char* str_ptr = llvm_new_temp(ctx->llvm_ctx);
                size_t str_len = strlen(decl->value) + 1;
                fprintf(ctx->llvm_ctx->output, "  %s = getelementptr inbounds [%zu x i8], [%zu x i8]* %s, i64 0, i64 0\n",
                        str_ptr, str_len, str_len, global_name);
                
                // Store string pointer to variable
                fprintf(ctx->llvm_ctx->output, "  store i8* %s, i8** %s, align 8\n", str_ptr, var_ptr_name);
                
                free(global_name);
                free(str_ptr);
                return NULL;
            }
            
            // Numeric/boolean variables
            // Register variable type
            register_variable_type(ctx, decl->name, 0);  // 0 = not pointer (i64)
            
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
            
            // YZ_62: Phase 17 - Handle string variable printing
            if (print_stmt->type == PRINT_VARIABLE) {
                // Need to determine if variable is string or numeric
                // For now, we'll check if it's a known string variable
                // TODO: Add proper symbol table with type info
                
                // YZ_63: Check if it's a string variable or parameter
                Statement* stmt_iter = ctx->current_func->body;
                int is_string_var = 0;
                int is_string_param = 0;
                
                // Check function parameters first
                FunctionParam* param = ctx->current_func->params;
                while (param) {
                    if (strcmp(param->name, print_stmt->value) == 0 && param->type == FUNC_PARAM_TEXT) {
                        is_string_param = 1;
                        break;
                    }
                    param = param->next;
                }
                
                // If not a parameter, check local variables
                if (!is_string_param) {
                    while (stmt_iter) {
                        if (stmt_iter->type == STMT_VARIABLE_DECL) {
                            VariableDeclaration* decl = (VariableDeclaration*)stmt_iter->data;
                            if (strcmp(decl->name, print_stmt->value) == 0 && decl->type == VAR_STRING) {
                                is_string_var = 1;
                                break;
                            }
                        }
                        stmt_iter = stmt_iter->next;
                    }
                }
                
                if (is_string_param) {
                    // String parameter: already i8*, just use it directly
                    fprintf(ctx->llvm_ctx->output, "  call void @mlp_println_string(i8* %%%s)\n", print_stmt->value);
                    return NULL;
                }
                
                if (is_string_var) {
                    // String variable: load i8* and call mlp_println_string
                    char* var_ptr_name = llvm_new_temp(ctx->llvm_ctx);
                    char* loaded_str = llvm_new_temp(ctx->llvm_ctx);
                    
                    // Load string pointer from variable
                    // We need to find the alloca'd pointer
                    fprintf(ctx->llvm_ctx->output, "  %s = load i8*, i8** %%%s_ptr, align 8\n", 
                            loaded_str, print_stmt->value);
                    
                    // Call mlp_println_string
                    fprintf(ctx->llvm_ctx->output, "  call void @mlp_println_string(i8* %s)\n", loaded_str);
                    
                    free(var_ptr_name);
                    free(loaded_str);
                    return NULL;
                }
            }
            
            // Get the value to print (for numeric variables)
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
            
            // YZ_200: Get return type from function declaration (TEXT/LIST -> i8*, others -> i64)
            int return_type = (ctx->current_func->return_type == FUNC_RETURN_TEXT || 
                              ctx->current_func->return_type == FUNC_RETURN_LIST) ? 1 : 0;
            
            if (ret->return_value) {
                LLVMValue* ret_val = generate_expression_llvm(ctx, ret->return_value);
                llvm_emit_return(ctx->llvm_ctx, ret_val, return_type);
                llvm_value_free(ret_val);
            } else {
                LLVMValue* zero = llvm_const_i64(0);
                llvm_emit_return(ctx->llvm_ctx, zero, return_type);
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
    
    // YZ_65: First pass - scan for local variables to populate registry
    Statement* scan_stmt = func->body;
    while (scan_stmt) {
        scan_statement_for_variables(func, scan_stmt);
        scan_stmt = scan_stmt->next;
    }
    
    // Collect parameter names and types
    const char** param_names = NULL;
    int* param_types = NULL;
    int param_count = 0;
    
    FunctionParam* param = func->params;
    while (param) {
        param_count++;
        param = param->next;
    }
    
    if (param_count > 0) {
        param_names = malloc(sizeof(char*) * param_count);
        param_types = malloc(sizeof(int) * param_count);
        param = func->params;
        int i = 0;
        while (param) {
            param_names[i] = param->name;
            // YZ_63: FUNC_PARAM_TEXT (1) -> string type (i8*)
            // YZ_21: FUNC_PARAM_LIST -> also i8* (pointer)
            param_types[i] = (param->type == FUNC_PARAM_TEXT || param->type == FUNC_PARAM_LIST) ? 1 : 0;
            i++;
            param = param->next;
        }
    }
    
    // Emit function start
    // YZ_200: Map return type (TEXT/LIST -> i8*, others -> i64)
    int return_type = (func->return_type == FUNC_RETURN_TEXT || func->return_type == FUNC_RETURN_LIST) ? 1 : 0;
    llvm_emit_function_start(ctx->llvm_ctx, func->name, param_names, param_types, param_count, return_type);
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
    if (param_types) {
        free(param_types);
    }
}
