#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_types.h"

int main(int argc, char** argv) {
    // Usage: mlp-errors <code> <file> <line> <col> [placeholders...]
    if (argc < 5) {
        fprintf(stderr, "Usage: mlp-errors <code> <file> <line> <col> [placeholders...]\n");
        fprintf(stderr, "\nExamples:\n");
        fprintf(stderr, "  mlp-errors E0001 test.mlp 5 10 myVar\n");
        fprintf(stderr, "  mlp-errors E0002 main.mlp 12 5 int string\n");
        fprintf(stderr, "  mlp-errors W0001 utils.mlp 8 5 unused\n");
        fprintf(stderr, "\nEnvironment:\n");
        fprintf(stderr, "  MLP_LANG - Language (tr/en), default: tr\n");
        return 1;
    }

    // Parse arguments
    ErrorContext ctx;
    ctx.error_code = argv[1];
    ctx.file = argv[2];
    ctx.line = atoi(argv[3]);
    ctx.column = atoi(argv[4]);
    
    // Language detection (default: Turkish)
    char* lang = getenv("MLP_LANG");
    if (!lang || strlen(lang) == 0) {
        lang = "tr";
    }

    // Collect placeholders
    ctx.placeholder_count = argc - 5;
    if (ctx.placeholder_count > MAX_PLACEHOLDERS) {
        ctx.placeholder_count = MAX_PLACEHOLDERS;
    }
    
    for (int i = 0; i < ctx.placeholder_count; i++) {
        ctx.placeholders[i] = argv[5 + i];
    }

    // Load error database
    // Try relative path first, then absolute
    const char* db_paths[] = {
        "data/errors.json",
        "../data/errors.json",
        "../../data/errors.json",
        "/home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/data/errors.json"
    };

    int loaded = 0;
    for (int i = 0; i < sizeof(db_paths) / sizeof(db_paths[0]); i++) {
        if (load_error_database(db_paths[i])) {
            loaded = 1;
            break;
        }
    }

    if (!loaded) {
        fprintf(stderr, "Error: Could not load error database\n");
        fprintf(stderr, "Tried paths:\n");
        for (int i = 0; i < sizeof(db_paths) / sizeof(db_paths[0]); i++) {
            fprintf(stderr, "  - %s\n", db_paths[i]);
        }
        return 1;
    }

    // Format and print error
    char* output = format_error(&ctx, lang);
    if (output) {
        printf("%s\n", output);
        cleanup_error_database();
        return 0;
    }

    cleanup_error_database();
    return 1;
}
