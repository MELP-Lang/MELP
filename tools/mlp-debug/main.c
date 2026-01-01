// mlp-debug - MLP Debugger (GDB Wrapper)
// Provides GDB-style debugging for MLP source code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source_mapper.h"
#include "gdb_wrapper.h"

void print_usage(const char* prog_name) {
    printf("MLP Debugger - GDB Wrapper for MLP Language\n\n");
    printf("Usage: %s <binary> <mlp-source>\n\n", prog_name);
    printf("Examples:\n");
    printf("  %s ./test test.mlp          # Debug test binary with test.mlp source\n", prog_name);
    printf("  %s ./myprogram prog.mlp     # Debug myprogram with prog.mlp\n\n", prog_name);
    printf("Commands in debugger:\n");
    printf("  break <line>  - Set breakpoint at MLP line number\n");
    printf("  run           - Start program execution\n");
    printf("  step          - Execute one MLP line\n");
    printf("  continue      - Continue until next breakpoint\n");
    printf("  print <var>   - Print variable value\n");
    printf("  quit          - Exit debugger\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Check for help flag
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong number of arguments\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    const char* binary_path = argv[1];
    const char* mlp_source = argv[2];
    
    // Check if files exist
    FILE* f = fopen(binary_path, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open binary: %s\n", binary_path);
        return 1;
    }
    fclose(f);
    
    f = fopen(mlp_source, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open MLP source: %s\n", mlp_source);
        return 1;
    }
    fclose(f);
    
    printf("=== MLP Debugger ===\n");
    printf("Binary: %s\n", binary_path);
    printf("Source: %s\n\n", mlp_source);
    
    // Create GDB session
    GDBSession* session = gdb_session_create(binary_path, mlp_source);
    if (!session) {
        fprintf(stderr, "Error: Failed to create GDB session\n");
        return 1;
    }
    
    // Start GDB
    if (!gdb_start(session)) {
        fprintf(stderr, "Error: Failed to start GDB\n");
        fprintf(stderr, "Make sure GDB is installed: sudo apt-get install gdb\n");
        gdb_session_free(session);
        return 1;
    }
    
    // Print source map info
    if (session->source_map && session->source_map->count > 0) {
        printf("Loaded source map with %d entries\n\n", session->source_map->count);
    } else {
        printf("Warning: No source map found (compile with -g for better debugging)\n\n");
    }
    
    // Enter interactive loop
    gdb_interactive_loop(session);
    
    // Cleanup
    gdb_session_free(session);
    
    return 0;
}
