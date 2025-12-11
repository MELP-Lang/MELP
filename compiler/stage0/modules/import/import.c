#include "import.h"
#include "import_parser.h"
#include "import_cache.h"  // YZ_42: Module caching
#include "import_cache_persist.h"  // YZ_43: Persistent cache
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

// YZ_36: Need to include lexer and parser for module loading
#include "../lexer/lexer.h"
#include "../functions/functions_parser.h"
#include "../error/error.h"

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
    
    // Search order:
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
FunctionDeclaration* import_load_module(const char* module_path) {
    // YZ_42: Check in-memory cache first (same compilation)
    FunctionDeclaration* cached_functions = cache_get(module_path);
    if (cached_functions) {
        // Cache hit! No need to parse again
        return cached_functions;
    }
    
    // YZ_43: Check persistent cache (for information only)
    // Note: We still need to parse to get actual function declarations
    // Persistent cache is mainly useful for object file tracking
    CacheMetadata cached_meta = {0};
    int has_persistent_cache = cache_persist_load(module_path, &cached_meta);
    if (has_persistent_cache) {
        if (cache_persist_is_valid(&cached_meta)) {
            // Check if object file is also valid
            if (cached_meta.object_path && cache_object_is_valid(module_path, cached_meta.object_path)) {
                printf("   ⚡ Valid object file found: %s (for future incremental compilation)\n", 
                       cached_meta.object_path);
            }
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
        
        // Put token back for function parser
        lexer_unget_token(lexer, tok);
        
        // Parse function
        FunctionDeclaration* func = parse_function_declaration(lexer);
        if (!func) {
            // Parse error - restore context and cleanup
            lexer_free(lexer);
            free(source);
            error_restore_context(saved_context);
            import_pop_stack();  // YZ_37: Pop from import stack
            error_fatal("Failed to parse module: %s", module_path);
            return NULL;
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
    
    // YZ_37: Pop from import stack (successful parse)
    import_pop_stack();
    
    // YZ_42: Store in cache for next time
    cache_put(module_path, functions, dependencies, dependency_count);
    
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
