#include "cli_parser.h"
#include <stdio.h>
#include <string.h>

void cli_print_help(const char* program_name) {
    fprintf(stderr, "Usage: %s <input.mlp> <output.s>\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "MELP Compiler - Stage 0\n");
    fprintf(stderr, "Compile MELP source code to x86-64 assembly\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  --help     Show this help message\n");
    fprintf(stderr, "  --version  Show version information\n");
}

void cli_print_version(void) {
    printf("MELP Compiler v0.3.0-alpha (Stage 0)\n");
    printf("26 modules, STO-enabled\n");
    printf("Build date: 7 Aralık 2025\n");
}

CLIArgs cli_parse_args(int argc, char** argv) {
    CLIArgs args = {NULL, NULL, 0, 0};
    
    // Check for flags
    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            args.show_help = 1;
            return args;
        }
        if (strcmp(argv[1], "--version") == 0) {
            args.show_version = 1;
            return args;
        }
    }
    
    // Normal usage: program input.mlp output.s
    if (argc == 3) {
        args.input_file = argv[1];
        args.output_file = argv[2];
    }
    
    return args;
}
