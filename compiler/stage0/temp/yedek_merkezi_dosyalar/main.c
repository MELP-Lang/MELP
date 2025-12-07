// MLP Compiler - Entry Point (Stage 0)
#include <stdio.h>
#include "cli/cli_parser.h"
#include "pipeline/pipeline.h"

int main(int argc, char** argv) {
    // Parse command-line arguments
    CLIArgs args = cli_parse_args(argc, argv);
    
    // Handle special commands
    if (args.show_help) {
        cli_print_help(argv[0]);
        return 0;
    }
    
    if (args.show_version) {
        cli_print_version();
        return 0;
    }
    
    // Validate arguments
    if (!args.input_file || !args.output_file) {
        fprintf(stderr, "Error: Input and output files required\n");
        cli_print_help(argv[0]);
        return 1;
    }
    
    // Run compilation pipeline
    CompileResult result = pipeline_compile(args.input_file, args.output_file);
    
    return result.exit_code;
}
