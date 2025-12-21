#include "import.h"
#include "import_parser.h"
#include "import_cache.h"  // YZ_42: Module caching
#include "import_cache_persist.h"  // YZ_43: Persistent cache
#include "module_declaration.h"  // YZ_204: Module system
#include "export_tracker.h"      // YZ_204: Export tracking
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <libgen.h>  // YZ_28: For dirname()
#include <limits.h>  // YZ_28: For PATH_MAX

// YZ_36: Need to include lexer and parser for module loading
#include "../lexer/lexer.h"
#include "../functions/functions_parser.h"
#include "../functions/functions_codegen.h"  // YZ_44: Part 5.1 - Assembly generation
#include "../variable/variable.h"         // YZ_13: For const declarations
#include "../variable/variable_parser.h"  // YZ_13: For const parsing
#include "../error/error.h"

// YZ_28: Current source file being compiled (for relative import resolution)
static const char* g_current_source_file = NULL;

void import_set_current_source_file(const char* path) {
    g_current_source_file = path;
}

const char* import_get_current_source_file(void) {
    return g_current_source_file;
}

// ============================================================================
// Phase 11: Circular Import Detection (YZ_37)
// ============================================================================

#define MAX_IMPORT_DEPTH 64

// Import stack for circular dependency detection
static const char* import_stack[MAX_IMPORT_DEPTH];
static int import_stack_top = 0;

// Push module to import stack
static int import_push_stack(const char* module_path) {
    if (import_stack_top >= MAX_IMPORT_DEPTH) {
        error_fatal("Import depth too deep (max %d) - possible circular import?", MAX_IMPORT_DEPTH);
        return 0;
    }
    import_stack[import_stack_top++] = module_path;
    return 1;
}

// Pop module from import stack
static void import_pop_stack(void) {
    if (import_stack_top > 0) {
        import_stack_top--;
    }
}

// Check if module is already being imported (circular dependency)
static int import_check_circular(const char* module_path) {
    for (int i = 0; i < import_stack_top; i++) {
        if (strcmp(import_stack[i], module_path) == 0) {
            return 1;  // Circular dependency detected
        }
    }
    return 0;  // No circular dependency
}

// Print import chain for error message
static void import_print_chain(void) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Import chain:\n");
    for (int i = 0; i < import_stack_top; i++) {
        fprintf(stderr, "  %s\n", import_stack[i]);
    }
}

// ============================================================================

ImportStatement* import_statement_create(const char* module_name) {
    ImportStatement* stmt = malloc(sizeof(ImportStatement));
    stmt->module_name = strdup(module_name);
    stmt->module_path = NULL;
    stmt->is_resolved = 0;
    stmt->functions = NULL;   // YZ_36: No functions yet
    stmt->next = NULL;        // YZ_36: End of list
    return stmt;
}

void import_statement_free(ImportStatement* import_stmt) {
    if (!import_stmt) return;
    if (import_stmt->module_name) free(import_stmt->module_name);
    if (import_stmt->module_path) free(import_stmt->module_path);
    free(import_stmt);
}

// Helper: Check if file exists
static int file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

char* import_resolve_module_path(const char* module_name) {
    char path[512];
    
    // YZ_13: Enhancement - Support relative paths
    // YZ_28: Fix - Resolve relative paths based on source file location, not CWD
    if (strchr(module_name, '/') != NULL || strchr(module_name, '\\') != NULL) {
        // Direct path (relative or absolute)
        
        // YZ_28: If relative path and we know the source file, resolve relative to it
        if (module_name[0] != '/' && g_current_source_file) {
            // Get directory of current source file
            char source_dir[PATH_MAX];
            char* source_copy = strdup(g_current_source_file);
            char* dir = dirname(source_copy);
            snprintf(source_dir, sizeof(source_dir), "%s", dir);
            free(source_copy);
            
            // Build path relative to source file's directory
            snprintf(path, sizeof(path), "%s/%s", source_dir, module_name);
            
            // Normalize path (resolve .. and .)
            char resolved_path[PATH_MAX];
            if (realpath(path, resolved_path)) {
                if (file_exists(resolved_path)) {
                    return strdup(resolved_path);
                }
            }
            
            // Try adding .mlp extension if not present
            if (strstr(module_name, ".mlp") == NULL) {
                snprintf(path, sizeof(path), "%s/%s.mlp", source_dir, module_name);
                if (realpath(path, resolved_path)) {
                    if (file_exists(resolved_path)) {
                        return strdup(resolved_path);
                    }
                }
            }
        }
        
        // Fallback: Check if it exists as-is (absolute path or CWD-relative)
        if (file_exists(module_name)) {
            return strdup(module_name);
        }
        
        // Try adding .mlp extension if not present
        if (strstr(module_name, ".mlp") == NULL) {
            snprintf(path, sizeof(path), "%s.mlp", module_name);
            if (file_exists(path)) {
                return strdup(path);
            }
        }
        
        // Path not found
        return NULL;
    }
    
    // Original search order for simple module names:
    // YZ_28: First check in same directory as source file
    if (g_current_source_file) {
        char source_dir[PATH_MAX];
        char* source_copy = strdup(g_current_source_file);
        char* dir = dirname(source_copy);
        snprintf(source_dir, sizeof(source_dir), "%s", dir);
        free(source_copy);
        
        // Try in source file's directory first (with .mlp extension)
        snprintf(path, sizeof(path), "%s/%s.mlp", source_dir, module_name);
        if (file_exists(path)) {
            return strdup(path);
        }
        
        // Try without extension (in case module_name already has it)
        snprintf(path, sizeof(path), "%s/%s", source_dir, module_name);
        if (file_exists(path)) {
            return strdup(path);
        }
    }
    
    // 1. modules/core/module_name.mlp
    snprintf(path, sizeof(path), "modules/core/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 2. modules/advanced/module_name.mlp
    snprintf(path, sizeof(path), "modules/advanced/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 3. modules/experimental/module_name.mlp
    snprintf(path, sizeof(path), "modules/experimental/%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // 4. module_name.mlp (current directory)
    snprintf(path, sizeof(path), "%s.mlp", module_name);
    if (file_exists(path)) {
        return strdup(path);
    }
    
    // Not found
    return NULL;
}

// YZ_36: Load and parse a module file
// YZ_42: Enhanced with module caching for incremental compilation
// YZ_43: Enhanced with persistent cache and incremental object files
// YZ_45 Part 5.4: Incremental Skip Logic - Skip parsing if object file is up-to-date
FunctionDeclaration* import_load_module(const char* module_path) {
    // YZ_42: Check in-memory cache first (same compilation)
    FunctionDeclaration* cached_functions = cache_get(module_path);
    if (cached_functions) {
        // Cache hit! No need to parse again
        return cached_functions;
    }
    
    // YZ_45 Part 5.4: INCREMENTAL SKIP LOGIC
    // Check persistent cache AND object file validity
    // If both are valid, we can skip parsing entirely!
    CacheMetadata cached_meta = {0};
    int has_persistent_cache = cache_persist_load(module_path, &cached_meta);
    if (has_persistent_cache) {
        int is_valid = cache_persist_is_valid(&cached_meta);
        int obj_valid = (cached_meta.object_path && cache_object_is_valid(module_path, cached_meta.object_path));
        
        if (is_valid && obj_valid) {
            // YZ_45: Object file is up-to-date!
            // Create minimal function declarations from cache metadata
            // (Only function names, no body - sufficient for function registry)
            printf("   ⚡ SKIP: Module unchanged, using cached object: %s\n", 
                   cached_meta.object_path);
            
            FunctionDeclaration* funcs = NULL;
            FunctionDeclaration* last = NULL;
            
            for (int i = 0; i < cached_meta.function_count; i++) {
                FunctionDeclaration* func = malloc(sizeof(FunctionDeclaration));
                memset(func, 0, sizeof(FunctionDeclaration));  // YZ_45: Initialize all fields
                func->name = strdup(cached_meta.function_names[i]);
                func->return_type = FUNC_RETURN_NUMERIC;  // Default
                func->param_count = 0;
                func->local_var_count = 0;
                func->body = NULL;
                func->params = NULL;
                func->local_vars = NULL;
                func->next = NULL;
                
                if (!funcs) {
                    funcs = func;
                    last = func;
                } else {
                    last->next = func;
                    last = func;
                }
            }
            
            // Put in in-memory cache for this compilation
            // YZ_45: Don't pass dependencies (not parsed from cache yet, TODO: parse deps too)
            cache_put(module_path, funcs, NULL, 0);
            
            cache_metadata_cleanup(&cached_meta);
            return funcs;  // YZ_45: EARLY EXIT - No parsing needed!
        }
        cache_metadata_cleanup(&cached_meta);  // Cleanup fields only
    }
    
    // YZ_37: Check for circular import
    if (import_check_circular(module_path)) {
        fprintf(stderr, "\n");
        fprintf(stderr, "%sCircular import detected!%s\n", 
                "\033[1;31m", "\033[0m");
        fprintf(stderr, "Attempting to import: %s\n", module_path);
        import_print_chain();
        fprintf(stderr, "  -> %s (circular!)\n", module_path);
        error_fatal("Circular import dependency");
        return NULL;
    }
    
    // YZ_37: Push to import stack
    if (!import_push_stack(module_path)) {
        return NULL;
    }
    
    // YZ_42: Track dependencies for cache
    char** dependencies = NULL;
    int dependency_count = 0;
    int dependency_capacity = 0;
    
    // Read module file
    FILE* file = fopen(module_path, "r");
    if (!file) {
        import_pop_stack();
        error_fatal("Cannot open module file: %s", module_path);
        return NULL;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* source = malloc(size + 1);
    if (!source) {
        fclose(file);
        error_fatal("Memory allocation failed for module: %s", module_path);
        return NULL;
    }
    
    fread(source, 1, size, file);
    source[size] = '\0';
    fclose(file);
    
    // YZ_37: Save current error context before parsing module
    int saved_context = error_save_context();
    
    // YZ_28: Save current source file and set it to this module for nested imports
    const char* prev_source_file = g_current_source_file;
    g_current_source_file = module_path;
    
    // Set error context for this module
    error_set_source(source, module_path);
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        error_restore_context(saved_context);
        free(source);
        error_fatal("Failed to create lexer for module: %s", module_path);
        return NULL;
    }
    
    // Parse all functions in the module
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    
    // YZ_204: Track export state
    int expecting_export = 0;
    
    while (1) {
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // YZ_37: Handle import statements in modules
        if (tok->type == TOKEN_IMPORT) {
            // Parse import statement
            ImportStatement* import_stmt = import_parse(lexer, tok);
            token_free(tok);
            
            if (!import_stmt) {
                continue;  // Parse error already reported
            }
            
            // Load the imported module recursively
            if (import_stmt->is_resolved && import_stmt->module_path) {
                printf("   📦 Nested Import: %s (from %s)\n", 
                       import_stmt->module_name, module_path);
                
                // YZ_42: Track dependency for cache
                if (dependency_count >= dependency_capacity) {
                    dependency_capacity = dependency_capacity == 0 ? 4 : dependency_capacity * 2;
                    dependencies = realloc(dependencies, sizeof(char*) * dependency_capacity);
                }
                dependencies[dependency_count++] = strdup(import_stmt->module_path);
                
                FunctionDeclaration* imported_funcs = import_load_module(import_stmt->module_path);
                if (imported_funcs) {
                    // Add imported functions to our list
                    if (!functions) {
                        functions = imported_funcs;
                        // Find last function
                        last_func = imported_funcs;
                        while (last_func->next) last_func = last_func->next;
                    } else {
                        last_func->next = imported_funcs;
                        // Find new last function
                        while (last_func->next) last_func = last_func->next;
                    }
                }
            }
            
            import_statement_free(import_stmt);
            continue;
        }
        
        // YZ_13: Handle const declarations in modules (import context)
        // ✅ Stateless: Const'lar parse edilip LLVM IR'a yazılır
        // ✅ No global state modification!
        if (tok->type == TOKEN_CONST) {
            VariableDeclaration* const_decl = variable_parse_declaration(lexer, tok);
            token_free(tok);
            
            if (const_decl) {
                // ✅ Const parsed successfully
                // ✅ Will be emitted to LLVM IR during codegen
                // ✅ Linking will merge all const declarations
                // TODO: Free const_decl properly when we track all declarations
            }
            continue;
        }
        
        // YZ_204: Handle module declaration
        if (tok->type == TOKEN_MODULE) {
            // Parse and set module declaration for this import
            ModuleDeclaration* module_decl = module_declaration_parse(lexer, tok);
            token_free(tok);
            
            if (module_decl) {
                printf("   📦 Module in import: %s\n", module_decl->module_name);
                // Module context is now set for export tracking
            }
            continue;
        }
        
        // YZ_204: Handle export keyword
        if (tok->type == TOKEN_EXPORT) {
            // Export keyword in imported module
            int consumed = export_parse_keyword(lexer, tok);
            token_free(tok);
            
            if (consumed) {
                expecting_export = 1;  // Next declaration should be exported
            }
            continue;
        }
        
        // Put token back for function parser
        lexer_unget_token(lexer, tok);
        
        // Parse function
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) {
            // YZ_108: Parse hatası - bu fonksiyonu atla (Tree Shaking)
            fprintf(stderr, "⚠️ Warning: Skipping unparseable function in %s\n", module_path);
            
            // Sonraki fonksiyona atla
            Token* skip_tok;
            while ((skip_tok = lexer_next_token(lexer)) != NULL) {
                if (skip_tok->type == TOKEN_EOF || 
                    skip_tok->type == TOKEN_FUNCTION ||
                    skip_tok->type == TOKEN_CONST) {
                    lexer_unget_token(lexer, skip_tok);
                    break;
                }
                token_free(skip_tok);
            }
            expecting_export = 0;  // Reset export flag
            continue;  // ✅ Döngüye devam
        }
        
        // YZ_204: If export was expected, add to export registry
        if (expecting_export) {
            export_add_symbol(func->name, EXPORT_FUNCTION, func);
            expecting_export = 0;  // Reset flag
        }
        
        // Add to list
        if (!functions) {
            functions = func;
            last_func = func;
        } else {
            last_func->next = func;
            last_func = func;
        }
    }
    
    lexer_free(lexer);
    free(source);
    
    // YZ_37: Restore original error context
    error_restore_context(saved_context);
    
    // YZ_28: Restore previous source file
    g_current_source_file = prev_source_file;
    
    // YZ_37: Pop from import stack (successful parse)
    import_pop_stack();
    
    // YZ_42: Store in cache for next time
    cache_put(module_path, functions, dependencies, dependency_count);
    
    // YZ_44 Part 5.1: Generate assembly for this module
    import_generate_module_assembly(module_path, functions);
    
    // YZ_43: Save to persistent cache
    char* assembly_path = cache_get_assembly_path(module_path);
    char* object_path = cache_get_object_path(module_path);
    CacheMetadata* metadata = cache_metadata_create(
        module_path, functions, dependencies, dependency_count,
        object_path, assembly_path
    );
    if (metadata) {
        cache_persist_save(module_path, metadata);
        cache_metadata_free(metadata);
    }
    free(assembly_path);
    free(object_path);
    
    // YZ_42: Clean up dependency tracking
    for (int i = 0; i < dependency_count; i++) {
        free(dependencies[i]);
    }
    free(dependencies);
    
    return functions;
}

// ============================================================================
// YZ_44 Part 5.1: Per-Module Assembly Generation
// ============================================================================

// Generate assembly file for a module
// Returns 1 on success, 0 on failure
int import_generate_module_assembly(const char* module_path, FunctionDeclaration* functions) {
    if (!module_path || !functions) {
        return 0;
    }
    
    // Get assembly path
    char* assembly_path = cache_get_assembly_path(module_path);
    if (!assembly_path) {
        return 0;
    }
    
    // Open assembly file for writing
    FILE* asm_file = fopen(assembly_path, "w");
    if (!asm_file) {
        fprintf(stderr, "Warning: Cannot create assembly file: %s\n", assembly_path);
        free(assembly_path);
        return 0;
    }
    
    // Generate assembly header
    fprintf(asm_file, "# Module: %s\n", module_path);
    fprintf(asm_file, "# Generated by MELP Compiler (YZ_44 Part 5.1)\n\n");
    fprintf(asm_file, ".text\n\n");
    
    // Generate code for each function in this module
    FunctionDeclaration* func = functions;
    while (func) {
        function_generate_declaration(asm_file, func);
        func = func->next;
    }
    
    fclose(asm_file);
    printf("   📝 Generated assembly: %s\n", assembly_path);
    
    free(assembly_path);
    return 1;
}
