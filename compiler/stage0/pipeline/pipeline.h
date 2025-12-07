#ifndef PIPELINE_H
#define PIPELINE_H

// Compilation result
typedef struct {
    int exit_code;
} CompileResult;

// Main compilation pipeline
// Reads input_file, compiles to output_file
CompileResult pipeline_compile(const char* input_file, const char* output_file);

#endif
