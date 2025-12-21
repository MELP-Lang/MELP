// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../import/import.h"
#include "../import/import_parser.h"
#include "../struct/struct.h"
#include "../struct/struct_parser.h"
#include "../struct/struct_codegen.h"
#include "../enum/enum.h"
#include "../enum/enum_parser.h"
#include "../enum/enum_codegen.h"
#include "../variable/variable.h"
#include "../variable/variable_parser.h"
#include "functions.h"
#include "functions_parser.h"
#include "functions_codegen.h"
#include "functions_codegen_llvm.h"
#include "functions_generic.h"  // YZ_203: Generic type system

// Backend type selection
typedef enum {
    BACKEND_ASSEMBLY,
    BACKEND_LLVM
} BackendType;

// Global backend selection (YZ_07: expose for array_codegen.c)
BackendType backend = BACKEND_LLVM;

// Read entire file into string
static char* read_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    
    return content;
}

int main(int argc, char** argv) {
    const char* input_file = NULL;
    const char* output_file = NULL;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--backend=", 10) == 0) {
            char* backend_name = argv[i] + 10;
            if (strcmp(backend_name, "llvm") == 0) {
                backend = BACKEND_LLVM;
            } else if (strcmp(backend_name, "assembly") == 0) {
                backend = BACKEND_ASSEMBLY;
            } else {
                fprintf(stderr, "Unknown backend: %s\n", backend_name);
                fprintf(stderr, "Available backends: assembly, llvm\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [options] <input.mlp> <output>\n", argv[0]);
            printf("Options:\n");
            printf("  --backend=llvm        Generate LLVM IR (default)\n");
            printf("  --backend=assembly    Generate x86-64 assembly (legacy)\n");
            printf("  --help                Show this help\n");
            return 0;
        } else if (!input_file) {
            input_file = argv[i];
        } else if (!output_file) {
            output_file = argv[i];
        }
    }
    
    if (!input_file || !output_file) {
        fprintf(stderr, "Usage: %s [options] <input.mlp> <output>\n", argv[0]);
        fprintf(stderr, "Use --help for more information\n");
        return 1;
    }
    
    // YZ_30: Set current source file for relative import resolution
    import_set_current_source_file(input_file);
    
    // Read input file
    char* source = read_file(input_file);
    if (!source) {
        perror("Error reading input file");
        return 1;
    }
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Failed to create lexer\n");
        free(source);
        return 1;
    }
    
    // Parse all top-level declarations (imports, structs, enums, functions)
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    StructDef* structs = NULL;
    StructDef* last_struct = NULL;
    EnumDefinition* enums = NULL;
    EnumDefinition* last_enum = NULL;
    VariableDeclaration* consts = NULL;  // YZ_121: Global const list
    VariableDeclaration* last_const = NULL;
    
    // YZ_203: Generic template registry
    GenericRegistry* generic_registry = generic_registry_create();
    
    while (1) {
        // Peek next token to check type
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // YZ_30: Handle import statements at top level
        if (tok->type == TOKEN_IMPORT) {
            ImportStatement* import_stmt = import_parse(lexer, tok);
            token_free(tok);
            
            if (import_stmt && import_stmt->is_resolved && import_stmt->module_path) {
                printf("📦 Import: %s\n", import_stmt->module_name);
                
                // Load and parse the imported module
                FunctionDeclaration* imported_funcs = import_load_module(import_stmt->module_path);
                if (imported_funcs) {
                    // Add imported functions to our list
                    if (!functions) {
                        functions = imported_funcs;
                        last_func = imported_funcs;
                    } else {
                        last_func->next = imported_funcs;
                    }
                    // Find new last function
                    while (last_func->next) last_func = last_func->next;
                }
            }
            
            if (import_stmt) import_statement_free(import_stmt);
            continue;
        }
        
        // YZ_32: Handle struct definitions at top level
        if (tok->type == TOKEN_STRUCT) {
            // Create temporary parser for struct parsing
            Parser temp_parser;
            temp_parser.lexer = lexer;
            temp_parser.current_token = NULL;
            
            StructDef* struct_def = parse_struct_definition(&temp_parser);
            token_free(tok);
            
            if (struct_def) {
                printf("📦 Struct: %s\n", struct_def->name);
                
                // YZ_34: Register struct for instance declaration (Point p1)
                struct_register_definition(struct_def);
                
                if (!structs) {
                    structs = struct_def;
                } else {
                    last_struct->next = struct_def;
                }
                last_struct = struct_def;
            }
            continue;
        }
        
        // YZ_32: Handle enum definitions at top level
        if (tok->type == TOKEN_ENUM) {
            EnumDefinition* enum_def = enum_parse(lexer, tok);
            token_free(tok);
            
            if (enum_def) {
                printf("📦 Enum: %s\n", enum_def->name);
                
                // YZ_35: Register enum so it can be looked up for value access
                enum_register(enum_def);
                
                if (!enums) {
                    enums = enum_def;
                } else {
                    last_enum->next = enum_def;
                }
                last_enum = enum_def;
            }
            continue;
        }
        
        // YZ_104: Handle const declarations at top level
        if (tok->type == TOKEN_CONST) {
            VariableDeclaration* const_decl = variable_parse_declaration(lexer, tok);
            token_free(tok);
            
            if (const_decl) {
                printf("📦 Const: %s\n", const_decl->name);
                // YZ_121: Add to const list
                if (!consts) {
                    consts = const_decl;
                } else {
                    last_const->next = const_decl;
                }
                last_const = const_decl;
            }
            continue;
        }
        
        // Put token back for function parser
        lexer_unget_token(lexer, tok);
        
        // Parse function declaration
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) break;
        
        // YZ_203: Skip generic templates - they're instantiated on demand
        if (func->is_generic_template) {
            printf("📋 Generic template: %s<%d type params>\n", 
                   func->name, func->type_param_count);
            // Store in template registry
            generic_register_template(generic_registry, func);
            continue;  // Don't add to functions list, don't free (registry owns it)
        }
        
        if (!functions) {
            functions = func;
        } else {
            last_func->next = func;
        }
        last_func = func;
    }
    
    // Close lexer
    lexer_free(lexer);
    free(source);
    
    // Open output file
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening output file");
        return 1;
    }
    
    // Generate code based on backend selection
    if (backend == BACKEND_LLVM) {
        // LLVM IR generation
        FunctionLLVMContext* llvm_ctx = function_llvm_context_create(output);
        if (!llvm_ctx) {
            fprintf(stderr, "Error: Failed to create LLVM context\n");
            fclose(output);
            return 1;
        }
        
        // YZ_121: Set global consts for codegen
        function_codegen_set_global_consts(consts);
        
        // Generate module header (using llvm_ctx's output)
        llvm_emit_module_header(llvm_ctx->llvm_ctx);
        
        // Generate all functions
        FunctionDeclaration* func = functions;
        while (func) {
            function_generate_declaration_llvm(llvm_ctx, func);
            func = func->next;
        }
        
        // Generate module footer (includes string globals)
        llvm_emit_module_footer(llvm_ctx->llvm_ctx);
        
        // Cleanup
        function_llvm_context_free(llvm_ctx);
        
    } else {
        // Assembly generation
        // Generate assembly header (AT&T syntax - matches register format with % prefix)
        fprintf(output, ".att_syntax\n");
        fprintf(output, ".data\n\n");
        
        // YZ_32: Generate struct metadata (for runtime type info)
        StructDef* s = structs;
        while (s) {
            // Count fields
            int field_count = 0;
            for (StructMember* m = s->members; m; m = m->next) field_count++;
            
            fprintf(output, "# Struct: %s (%d fields, size: %zu bytes)\n", 
                    s->name, field_count, s->total_size);
            s = s->next;
        }
        
        // YZ_32: Generate enum constants
        EnumDefinition* e = enums;
        while (e) {
            fprintf(output, "# Enum: %s\n", e->name);
            int i = 0;
            for (EnumValue* v = e->values; v; v = v->next, i++) {
                fprintf(output, ".equ %s_%s, %ld\n", e->name, v->name, (long)v->value);
            }
            fprintf(output, "\n");
            e = e->next;
        }
        
        fprintf(output, "\n.text\n\n");
        
        // YZ_121: Set global consts for codegen
        function_codegen_set_global_consts(consts);
        
        // Generate code for each function
        FunctionDeclaration* func = functions;
        while (func) {
            function_generate_declaration(output, func);
            func = func->next;
        }
    }
    
    // Close output
    fclose(output);
    
    // Cleanup generic registry
    generic_registry_destroy(generic_registry);
    
    // Report summary
    int func_count = 0, struct_count = 0, enum_count = 0;
    for (FunctionDeclaration* f = functions; f; f = f->next) func_count++;
    for (StructDef* st = structs; st; st = st->next) struct_count++;
    for (EnumDefinition* en = enums; en; en = en->next) enum_count++;
    
    printf("✅ Compiled %s -> %s\n", input_file, output_file);
    printf("   📊 %d functions, %d structs, %d enums\n", func_count, struct_count, enum_count);
    return 0;
}
