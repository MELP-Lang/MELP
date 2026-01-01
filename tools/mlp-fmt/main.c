/* main.c - MLP Formatter Entry Point */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fmt_types.h"
#include "fmt_lexer.h"
#include "fmt_formatter.h"

/* Print usage */
void print_usage(const char* prog_name) {
    fprintf(stderr, "Usage: %s [options] <file>\n", prog_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "  -i, --in-place    Format file in-place\n");
    fprintf(stderr, "  --check           Check if file needs formatting (exit 1 if yes)\n");
    fprintf(stderr, "  --indent SIZE     Indentation size (default: 4)\n");
    fprintf(stderr, "  --width WIDTH     Max line width (default: 80)\n");
    fprintf(stderr, "  -h, --help        Show this help\n");
}

/* Read file contents */
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    return content;
}

/* Write file contents */
bool write_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot write file '%s'\n", filename);
        return false;
    }
    
    fprintf(file, "%s", content);
    fclose(file);
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Parse options
    FormatOptions options = {
        .indent_size = 4,
        .max_line_width = 80,
        .spaces_around_operators = true,
        .newline_before_colon = false,
        .check_only = false
    };
    
    bool in_place = false;
    const char* filename = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--in-place") == 0) {
            in_place = true;
        }
        else if (strcmp(argv[i], "--check") == 0) {
            options.check_only = true;
        }
        else if (strcmp(argv[i], "--indent") == 0 && i + 1 < argc) {
            options.indent_size = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
            options.max_line_width = atoi(argv[++i]);
        }
        else if (argv[i][0] != '-') {
            filename = argv[i];
        }
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    
    if (!filename) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Read source file
    char* source = read_file(filename);
    if (!source) {
        return 1;
    }
    
    // Tokenize
    Lexer* lexer = lexer_create(source);
    int token_count;
    Token** tokens = lexer_tokenize(lexer, &token_count);
    
    // Format
    Formatter* formatter = formatter_create(tokens, token_count, options);
    char* formatted = formatter_format(formatter);
    
    // Check mode: compare with original
    if (options.check_only) {
        if (strcmp(source, formatted) != 0) {
            fprintf(stderr, "File '%s' needs formatting\n", filename);
            
            // Cleanup
            free(source);
            lexer_free(lexer);
            for (int i = 0; i < token_count; i++) {
                token_free(tokens[i]);
            }
            free(tokens);
            formatter_free(formatter);
            
            return 1; // Exit 1 if formatting needed
        } else {
            fprintf(stderr, "File '%s' is properly formatted\n", filename);
            
            // Cleanup
            free(source);
            lexer_free(lexer);
            for (int i = 0; i < token_count; i++) {
                token_free(tokens[i]);
            }
            free(tokens);
            formatter_free(formatter);
            
            return 0;
        }
    }
    
    // In-place mode: write to file
    if (in_place) {
        if (!write_file(filename, formatted)) {
            free(source);
            lexer_free(lexer);
            for (int i = 0; i < token_count; i++) {
                token_free(tokens[i]);
            }
            free(tokens);
            formatter_free(formatter);
            return 1;
        }
        fprintf(stderr, "Formatted '%s'\n", filename);
    }
    // Normal mode: print to stdout
    else {
        printf("%s", formatted);
    }
    
    // Cleanup
    free(source);
    lexer_free(lexer);
    for (int i = 0; i < token_count; i++) {
        token_free(tokens[i]);
    }
    free(tokens);
    formatter_free(formatter);
    
    return 0;
}
