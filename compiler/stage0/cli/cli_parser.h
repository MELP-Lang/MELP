#ifndef CLI_PARSER_H
#define CLI_PARSER_H

// CLI argument structure
typedef struct {
    const char* input_file;
    const char* output_file;
    int show_help;
    int show_version;
} CLIArgs;

// Parse command line arguments
CLIArgs cli_parse_args(int argc, char** argv);

// Show usage information
void cli_print_help(const char* program_name);

// Show version information
void cli_print_version(void);

#endif
