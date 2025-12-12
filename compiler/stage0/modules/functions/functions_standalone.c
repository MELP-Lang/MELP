// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // YZ_45: For smart linking mtime checks
#include <unistd.h>    // YZ_56: For readlink
#include <libgen.h>    // YZ_56: For dirname
#include "../lexer/lexer.h"
#include "../error/error.h"
#include "../import/import.h"          // YZ_35: Import statement
#include "../import/import_parser.h"   // YZ_35: Import parser
#include "../import/import_cache.h"    // YZ_42: Module caching
#include "../import/import_cache_persist.h"  // YZ_43: Persistent cache
#include "functions.h"
#include "functions_parser.h"
#include "functions_codegen.h"

// YZ_56: Get the MELP compiler base directory (where /runtime, /compiler are)
static const char* get_compiler_base_dir(void) {
    static char base_dir[1024] = {0};
    
    if (base_dir[0] != '\0') {
        return base_dir;  // Cached
    }
    
    // Get executable path
    char exe_path[1024];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        
        // Go up from /compiler/stage0/modules/functions/functions_compiler
        // to /home/pardus/projeler/MLP/MLP
        // Find last 5 slashes and cut there (was 4, now 5 to reach MLP root)
        int slashes = 0;
        for (int i = len - 1; i >= 0 && slashes < 5; i--) {
            if (exe_path[i] == '/') {
                slashes++;
                if (slashes == 5) {
                    exe_path[i] = '\0';
                    break;
                }
            }
        }
        snprintf(base_dir, sizeof(base_dir), "%s", exe_path);
    } else {
        // Fallback: assume we're in modules/functions
        snprintf(base_dir, sizeof(base_dir), "../../../..");
    }
    
    return base_dir;
}

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

// Error Recovery: Skip tokens until we find a synchronization point
// Returns 1 if sync point found, 0 if EOF reached
static int skip_to_sync_point(Lexer* lexer) {
    Token* tok;
    int depth = 0;  // Track nesting depth
    
    while ((tok = lexer_next_token(lexer)) != NULL) {
        TokenType type = tok->type;
        token_free(tok);
        
        if (type == TOKEN_EOF) {
            return 0;  // No more input
        }
        
        // Track function nesting
        if (type == TOKEN_FUNCTION) {
            if (depth == 0) {
                // Found start of new function - unget and return
                // We can't really "unget" so we'll handle this differently
                return 1;
            }
            depth++;
        } else if (type == TOKEN_END) {
            // Peek next token to see if it's "function"
            Token* next = lexer_next_token(lexer);
            if (next && next->type == TOKEN_FUNCTION) {
                token_free(next);
                if (depth > 0) depth--;
                if (depth == 0) {
                    // We've finished a function block, next should be clean
                    return 1;
                }
            } else if (next) {
                // Not "end function", just some other "end"
                token_free(next);
            }
        }
    }
    
    return 0;  // EOF reached
}

// YZ_44 Part 5.1: Track module assembly files for linking
#define MAX_MODULE_ASSEMBLIES 64
static char* module_assembly_files[MAX_MODULE_ASSEMBLIES];
static int module_assembly_count = 0;

static void add_module_assembly(const char* path) {
    if (module_assembly_count < MAX_MODULE_ASSEMBLIES) {
        module_assembly_files[module_assembly_count++] = strdup(path);
    }
}

static void cleanup_module_assemblies(void) {
    for (int i = 0; i < module_assembly_count; i++) {
        free(module_assembly_files[i]);
    }
    module_assembly_count = 0;
}

int main(int argc, char** argv) {
    // Initialize error handling system
    error_init();
    
    // YZ_42: Initialize module cache
    cache_init();
    
    // YZ_43: Initialize persistent cache directory
    cache_persist_init();
    
    // YZ_38: Parse command-line arguments
    int compile_only = 0;  // Flag for -c or --compile-only
    const char* input_file = NULL;
    const char* output_file = NULL;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compile-only") == 0) {
            compile_only = 1;
        } else if (!input_file) {
            input_file = argv[i];
        } else if (!output_file) {
            output_file = argv[i];
        }
    }
    
    // Check required arguments
    if (!input_file || !output_file) {
        error_io("Usage: %s [-c|--compile-only] <input.mlp> <output.s>", argv[0]);
        error_io("  -c, --compile-only    Generate assembly only (no linking)");
        return 1;
    }
    
    // Read input file
    char* source = read_file(input_file);
    if (!source) {
        perror("Error reading input file");
        return 1;
    }
    
    // Phase 6: Set source code for enhanced error messages
    error_set_source(source, input_file);
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        error_fatal("Failed to create lexer");
        free(source);
        return 1;
    }
    
    // YZ_40: FIRST PASS - Register all function names (forward reference support)
    // This allows functions to call other functions defined later in the file
    printf("🔍 First pass: Scanning function names...\n");
    int func_count = 0;
    Token* prev_tok = NULL;
    
    while (1) {
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // Look for 'function' keyword
        if (tok->type == TOKEN_FUNCTION) {
            // Skip 'end function' pattern (prev token was 'end')
            if (prev_tok && prev_tok->type == TOKEN_END) {
                // This is 'end function', not a new function declaration
                if (prev_tok) token_free(prev_tok);
                prev_tok = tok;
                continue;
            }
            
            func_count++;
            // Next token should be function name
            Token* name_tok = lexer_next_token(lexer);
            if (name_tok && name_tok->type == TOKEN_IDENTIFIER) {
                function_register_name(name_tok->value);
                printf("   📝 Registered: %s()\n", name_tok->value);
            } else if (name_tok) {
                printf("   ⚠️  TOKEN_FUNCTION but next token is type=%d\n", name_tok->type);
            }
            if (name_tok) token_free(name_tok);
        }
        
        if (prev_tok) token_free(prev_tok);
        prev_tok = tok;
    }
    
    if (prev_tok) token_free(prev_tok);
    
    printf("   Total function declarations found: %d\n", func_count);
    
    // Free first lexer and create new one for second pass
    lexer_free(lexer);
    lexer = lexer_create(source);
    if (!lexer) {
        error_fatal("Failed to create lexer for second pass");
        free(source);
        return 1;
    }
    
    printf("✅ First pass complete. Starting second pass...\n\n");
    
    // Parse all functions with ERROR RECOVERY (SECOND PASS)
    // YZ_35: Also handle import statements at top-level
    FunctionDeclaration* functions = NULL;
    FunctionDeclaration* last_func = NULL;
    ImportStatement* imports = NULL;    // YZ_35: Store imports
    ImportStatement* last_import = NULL;
    
    while (1) {
        // Check if we should stop due to too many errors
        if (error_should_stop()) {
            error_hint("Too many errors, stopping compilation");
            break;
        }
        
        // Peek at next token to see if it's import or function
        Token* tok = lexer_next_token(lexer);
        if (!tok || tok->type == TOKEN_EOF) {
            if (tok) token_free(tok);
            break;
        }
        
        // YZ_35: Handle import statement
        if (tok->type == TOKEN_IMPORT) {
            ImportStatement* import_stmt = import_parse(lexer, tok);
            token_free(tok);
            
            if (import_stmt) {
                // YZ_36: Add to linked list
                if (!imports) {
                    imports = import_stmt;
                    last_import = import_stmt;
                } else {
                    last_import->next = import_stmt;
                    last_import = import_stmt;
                }
                
                // Log the import
                if (import_stmt->is_resolved) {
                    printf("📦 Import: %s (resolved to %s)\n", 
                           import_stmt->module_name, import_stmt->module_path);
                    
                    // YZ_36: Load module functions
                    import_stmt->functions = import_load_module(import_stmt->module_path);
                    if (import_stmt->functions) {
                        // Count imported functions and register them
                        int func_count = 0;
                        FunctionDeclaration* f = import_stmt->functions;
                        while (f) {
                            func_count++;
                            function_register_name(f->name);  // YZ_36: Register for arithmetic parser
                            f = f->next;
                        }
                        printf("   ✅ Loaded %d function(s) from %s\n", func_count, import_stmt->module_name);
                        
                        // YZ_44 Part 5.1: Track module assembly file
                        char* asm_path = cache_get_assembly_path(import_stmt->module_path);
                        if (asm_path) {
                            add_module_assembly(asm_path);
                            free(asm_path);
                        }
                    } else {
                        error_warning(0, "No functions found in module '%s'", import_stmt->module_name);
                    }
                } else {
                    error_warning(0, "Module '%s' not found (compilation continues)", 
                                  import_stmt->module_name);
                }
            }
            continue;
        }
        
        // Not an import, put token back for function parser
        lexer_unget_token(lexer, tok);
        
        FunctionDeclaration* func = parse_function_declaration(lexer);
        
        if (!func) {
            // Parse failed - check if we should try to recover
            if (error_should_recover() && !error_should_stop()) {
                // Enter recovery mode
                error_enter_recovery();
                
                // Skip to next synchronization point (next 'function' or EOF)
                if (skip_to_sync_point(lexer)) {
                    // Found sync point, mark recovery and continue
                    error_mark_recovered();
                    error_note("Attempting to continue parsing after error...");
                    continue;
                }
            }
            // No recovery possible or EOF
            break;
        }
        
        // Successfully parsed a function
        if (error_in_recovery_mode()) {
            error_exit_recovery();
        }
        
        if (!functions) {
            functions = func;
            last_func = func;
        } else {
            last_func->next = func;
            last_func = func;
        }
    }
    
    // Close lexer
    lexer_free(lexer);
    
    // YZ_36: Register all function names before merging
    // This allows arithmetic parser to recognize user-defined functions
    FunctionDeclaration* func_temp = functions;
    while (func_temp) {
        function_register_name(func_temp->name);
        func_temp = func_temp->next;
    }
    
    // YZ_44 Part 5.1: Count main file functions (before merge)
    int main_function_count = 0;
    func_temp = functions;
    while (func_temp) {
        main_function_count++;
        func_temp = func_temp->next;
    }
    
    // YZ_42: Merge imported functions (with duplicate check for cache)
    // Build a list of unique function names already in main list
    if (imports) {
        // Find last function in main list
        last_func = functions;
        if (last_func) {
            while (last_func->next) {
                last_func = last_func->next;
            }
        }
        
        ImportStatement* imp = imports;
        while (imp) {
            if (imp->functions) {
                FunctionDeclaration* imported_func = imp->functions;
                while (imported_func) {
                    // Check if function already exists
                    int exists = 0;
                    FunctionDeclaration* check = functions;
                    while (check) {
                        if (strcmp(check->name, imported_func->name) == 0) {
                            exists = 1;
                            break;
                        }
                        check = check->next;
                    }
                    
                    if (!exists) {
                        // Append to main list
                        FunctionDeclaration* next_import = imported_func->next;  // Save next before modifying
                        imported_func->next = NULL;  // Break the link
                        
                        if (!functions) {
                            functions = imported_func;
                            last_func = imported_func;
                        } else {
                            last_func->next = imported_func;
                            last_func = imported_func;
                        }
                        
                        imported_func = next_import;  // Continue with saved next
                    } else {
                        imported_func = imported_func->next;
                    }
                }
            }
            imp = imp->next;
        }
    }
    
    // YZ_36: Free imports
    if (imports) {
        ImportStatement* imp = imports;
        while (imp) {
            ImportStatement* next = imp->next;
            // Functions are now in main list or cached, don't free them
            imp->functions = NULL;
            free(imp->module_name);
            free(imp->module_path);
            free(imp);
            imp = next;
        }
    }
    
    // Check if we have any errors
    if (error_has_errors()) {
        error_print_summary();
        free(source);
        return 1;
    }
    
    // Check if we parsed any functions
    if (!functions) {
        error_warning(0, "No functions found in input file");
        free(source);
        return 0;  // Not an error, just nothing to do
    }
    
    // YZ_39: For compile-only mode, generate assembly now
    if (compile_only) {
        // Open output file
        FILE* output = fopen(output_file, "w");
        if (!output) {
            perror("Error opening output file");
            return 1;
        }
        
        // Generate assembly header
        // Phase 6: Add .rodata section for runtime error messages
        fprintf(output, ".section .rodata\n");
        fprintf(output, ".div_zero_msg:\n");
        fprintf(output, "    .string \"Division by zero is not allowed!\"\n");
        fprintf(output, "\n");
        
        // Generate AT&T syntax assembly (default for GCC)
        fprintf(output, ".text\n\n");
        
        // Generate code for each function
        FunctionDeclaration* func = functions;
        while (func) {
            function_generate_declaration(output, func);
            func = func->next;
        }
        
        // Close output
        fclose(output);
    }
    
    // Free source code
    free(source);
    
    // YZ_39: Generate object files and link if needed
    if (compile_only) {
        // YZ_39: Generate object file (.s -> .o)
        char obj_file[512];
        snprintf(obj_file, sizeof(obj_file), "%s", output_file);
        // Replace .s with .o
        char* dot = strrchr(obj_file, '.');
        if (dot && strcmp(dot, ".s") == 0) {
            strcpy(dot, ".o");
        } else {
            strcat(obj_file, ".o");
        }
        
        // Assemble: gcc -c output.s -o output.o
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s 2>&1", output_file, obj_file);
        
        printf("🔧 Assembling: %s -> %s\n", output_file, obj_file);
        FILE* gcc = popen(cmd, "r");
        if (gcc) {
            char line[512];
            int has_error = 0;
            while (fgets(line, sizeof(line), gcc)) {
                fprintf(stderr, "   gcc: %s", line);
                has_error = 1;
            }
            int status = pclose(gcc);
            if (status != 0 || has_error) {
                error_fatal("Assembly failed: %s", obj_file);
                return 1;
            }
        }
        
        printf("✅ Compiled (object file) %s -> %s\n", input_file, obj_file);
        printf("   Next: Link with: gcc -o program %s ...\n", obj_file);
    } else {
        // YZ_39: Full compilation with linking
        // Step 1: Generate temporary assembly and object files
        char asm_file[512];
        char obj_file[512];
        snprintf(asm_file, sizeof(asm_file), "%s.s", output_file);
        snprintf(obj_file, sizeof(obj_file), "%s.o", output_file);
        
        // Re-open assembly file for this step
        FILE* asm_output = fopen(asm_file, "w");
        if (!asm_output) {
            error_fatal("Cannot create assembly file: %s", asm_file);
            return 1;
        }
        
        // Generate assembly header
        fprintf(asm_output, "# Main program: %s\n", input_file);
        fprintf(asm_output, "# Generated by MELP Compiler (YZ_44 Part 5.1)\n\n");
        fprintf(asm_output, ".section .rodata\n");
        fprintf(asm_output, ".div_zero_msg:\n");
        fprintf(asm_output, "    .string \"Division by zero is not allowed!\"\n");
        fprintf(asm_output, "\n");
        fprintf(asm_output, ".text\n\n");
        
        // YZ_44 Part 5.1: Generate code ONLY for main file functions
        // (imported modules already have their own assembly files)
        FunctionDeclaration* func = functions;
        int func_index = 0;
        while (func && func_index < main_function_count) {
            function_generate_declaration(asm_output, func);
            func = func->next;
            func_index++;
        }
        
        fclose(asm_output);
        
        // YZ_45 Part 5.3: SMART LINKING SYSTEM
        // Only assemble modules if .o is older than .s
        char* module_obj_files[MAX_MODULE_ASSEMBLIES];
        int module_obj_count = 0;
        
        for (int i = 0; i < module_assembly_count; i++) {
            // YZ_44 Part 5.2: Generate proper object file name
            // math.s -> math.o (not math.s.o)
            char mod_obj[512];
            const char* asm_file = module_assembly_files[i];
            const char* ext = strrchr(asm_file, '.');
            if (ext && strcmp(ext, ".s") == 0) {
                size_t len = ext - asm_file;
                strncpy(mod_obj, asm_file, len);
                mod_obj[len] = '\0';
                strcat(mod_obj, ".o");
            } else {
                snprintf(mod_obj, sizeof(mod_obj), "%s.o", asm_file);
            }
            
            module_obj_files[i] = strdup(mod_obj);
            module_obj_count++;
            
            // YZ_45 Part 5.3: CHECK IF OBJECT FILE IS UP-TO-DATE
            struct stat asm_st, obj_st;
            int need_assemble = 1;  // Default: assemble
            
            if (stat(asm_file, &asm_st) == 0 && stat(mod_obj, &obj_st) == 0) {
                // Both files exist - check timestamps
                if (obj_st.st_mtime >= asm_st.st_mtime) {
                    // Object is newer or same age as assembly - skip!
                    printf("⚡ SKIP: Object up-to-date: %s\n", mod_obj);
                    need_assemble = 0;
                }
            }
            
            if (!need_assemble) {
                continue;  // YZ_45: Skip assembly for this module!
            }
            
            // Assemble module (only if needed)
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s 2>&1", module_assembly_files[i], mod_obj);
            
            printf("🔧 Assembling module: %s -> %s\n", module_assembly_files[i], mod_obj);
            FILE* gcc = popen(cmd, "r");
            if (gcc) {
                char line[512];
                int has_error = 0;
                while (fgets(line, sizeof(line), gcc)) {
                    fprintf(stderr, "   gcc: %s", line);
                    has_error = 1;
                }
                int status = pclose(gcc);
                if (status != 0 || has_error) {
                    error_fatal("Module assembly failed: %s", mod_obj);
                    return 1;
                }
            }
        }
        
        // Step 2: Assemble main: gcc -c output.s -o output.o
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s 2>&1", asm_file, obj_file);
        
        printf("🔧 Assembling main: %s -> %s\n", asm_file, obj_file);
        FILE* gcc = popen(cmd, "r");
        if (gcc) {
            char line[512];
            int has_error = 0;
            while (fgets(line, sizeof(line), gcc)) {
                fprintf(stderr, "   gcc: %s", line);
                has_error = 1;
            }
            int status = pclose(gcc);
            if (status != 0 || has_error) {
                error_fatal("Assembly failed: %s", obj_file);
                return 1;
            }
        }
        
        // Step 3: Link: gcc -o output main.o module1.o module2.o ... -L... -lmlp_stdlib -lsto_runtime -lm
        // YZ_44 Part 5.1: Build command with all object files
        char link_cmd[4096];
        int offset = snprintf(link_cmd, sizeof(link_cmd), "gcc -o %s %s", output_file, obj_file);
        
        // Add all module object files
        for (int i = 0; i < module_obj_count; i++) {
            offset += snprintf(link_cmd + offset, sizeof(link_cmd) - offset, " %s", module_obj_files[i]);
        }
        
        // Add libraries
        // YZ_56: Use absolute paths for libraries (works from any directory)
        const char* compiler_dir = get_compiler_base_dir();
        char stdlib_path[1024], sto_path[1024];
        snprintf(stdlib_path, sizeof(stdlib_path), "%s/runtime/stdlib", compiler_dir);
        snprintf(sto_path, sizeof(sto_path), "%s/runtime/sto", compiler_dir);
        
        offset += snprintf(link_cmd + offset, sizeof(link_cmd) - offset,
                          " -L%s -lmlp_stdlib -L%s -lsto_runtime -lm 2>&1",
                          stdlib_path, sto_path);
        
        // YZ_56 Debug: Print link command
        // fprintf(stderr, "DEBUG: Link command: %s\n", link_cmd);
        
        printf("🔗 Linking: %s", output_file);
        if (module_obj_count > 0) {
            printf(" (with %d module(s))", module_obj_count);
        }
        printf("\n");
        
        FILE* gcc_link = popen(link_cmd, "r");
        if (gcc_link) {
            char line[512];
            int has_error = 0;
            while (fgets(line, sizeof(line), gcc_link)) {
                // YZ_56: Only show errors, not warnings
                if (strstr(line, "error:") || strstr(line, "undefined reference") || 
                    strstr(line, "collect2: error")) {
                    fprintf(stderr, "   gcc: %s", line);
                    has_error = 1;
                }
            }
            pclose(gcc_link);
            // YZ_56: Only fail if we detected actual errors (not just warnings)
            if (has_error) {
                error_fatal("Linking failed: %s", output_file);
                return 1;
            }
        }
        
        // Step 4: Clean up temporary files (main program only)
        // YZ_48: Temporarily keep assembly for debugging
        // remove(asm_file);
        remove(obj_file);
        
        // YZ_44 Part 5.2: Keep module object files persistent (don't delete)
        // They will be reused in incremental compilation
        for (int i = 0; i < module_obj_count; i++) {
            // Just free the path string, keep the .o file on disk
            free(module_obj_files[i]);
        }
        
        printf("✅ Compiled and linked %s -> %s\n", input_file, output_file);
        printf("   Run with: LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/sto ./%s\n", 
               output_file);
    }
    
    // YZ_44 Part 5.1: Cleanup module assembly tracking
    cleanup_module_assemblies();
    
    // YZ_42: Print cache stats (for debugging)
    if (getenv("MELP_CACHE_STATS")) {
        cache_print_stats();
    }
    
    // YZ_42: Cleanup cache
    cache_cleanup();
    
    return 0;
}
