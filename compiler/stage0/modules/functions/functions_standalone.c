// FUNCTIONS MODULE - STANDALONE TEST COMPILER
// Uses modular architecture with chained imports
// ~100 lines - thin wrapper around parser + codegen modules

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../error/error.h"
#include "../import/import.h"          // YZ_35: Import statement
#include "../import/import_parser.h"   // YZ_35: Import parser
#include "functions.h"
#include "functions_parser.h"
#include "functions_codegen.h"

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

int main(int argc, char** argv) {
    // Initialize error handling system
    error_init();
    
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
    
    // YZ_36: Merge imported functions into main function list
    if (imports) {
        ImportStatement* imp = imports;
        while (imp) {
            if (imp->functions) {
                // Add imported functions to the end of main function list
                if (!functions) {
                    functions = imp->functions;
                    last_func = imp->functions;
                    // Find the last function in imported module
                    while (last_func->next) {
                        last_func = last_func->next;
                    }
                } else {
                    // Append imported functions
                    FunctionDeclaration* imported_func = imp->functions;
                    while (imported_func) {
                        last_func->next = imported_func;
                        last_func = imported_func;
                        imported_func = imported_func->next;
                    }
                }
            }
            imp = imp->next;
        }
    }
    
    // YZ_36: Free imports (but not the functions, they're now in the main list)
    if (imports) {
        ImportStatement* imp = imports;
        while (imp) {
            ImportStatement* next = imp->next;
            // Don't free functions, they're now owned by main list
            imp->functions = NULL;
            import_statement_free(imp);
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
        fprintf(asm_output, ".section .rodata\n");
        fprintf(asm_output, ".div_zero_msg:\n");
        fprintf(asm_output, "    .string \"Division by zero is not allowed!\"\n");
        fprintf(asm_output, "\n");
        fprintf(asm_output, ".text\n\n");
        
        // Generate code for each function
        FunctionDeclaration* func = functions;
        while (func) {
            function_generate_declaration(asm_output, func);
            func = func->next;
        }
        
        fclose(asm_output);
        
        // Step 2: Assemble: gcc -c output.s -o output.o
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s 2>&1", asm_file, obj_file);
        
        printf("🔧 Assembling: %s -> %s\n", asm_file, obj_file);
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
        
        // Step 3: Link: gcc -o output output.o -L... -lmlp_stdlib -ltto_runtime -lm
        snprintf(cmd, sizeof(cmd), 
                 "gcc -o %s %s "
                 "-L../../../../runtime/stdlib -lmlp_stdlib "
                 "-L../../../../runtime/tto -ltto_runtime -lm 2>&1",
                 output_file, obj_file);
        
        printf("🔗 Linking: %s\n", output_file);
        gcc = popen(cmd, "r");
        if (gcc) {
            char line[512];
            int has_error = 0;
            while (fgets(line, sizeof(line), gcc)) {
                // Only report actual errors, not warnings
                if (strstr(line, "error:") || strstr(line, "undefined reference")) {
                    fprintf(stderr, "   gcc: %s", line);
                    has_error = 1;
                }
            }
            int status = pclose(gcc);
            if (status != 0 && has_error) {
                error_fatal("Linking failed: %s", output_file);
                return 1;
            }
        }
        
        // Step 4: Clean up temporary files
        remove(asm_file);
        remove(obj_file);
        
        printf("✅ Compiled and linked %s -> %s\n", input_file, output_file);
        printf("   Run with: LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/tto ./%s\n", 
               output_file);
    }
    return 0;
}
