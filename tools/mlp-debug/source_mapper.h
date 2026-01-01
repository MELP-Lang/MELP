#ifndef SOURCE_MAPPER_H
#define SOURCE_MAPPER_H

#include <stdio.h>
#include <stdbool.h>

// Source mapping entry (MLP line <-> C line)
typedef struct {
    int mlp_line;      // Line in MLP source
    int c_line;        // Line in generated C code
    char* scope;       // Scope name (function, global, etc.)
    int column;        // Column number (for future use)
} SourceMapEntry;

// Source map structure
typedef struct {
    char* mlp_file;              // MLP source file path
    char* c_file;                // Generated C file path
    SourceMapEntry* entries;     // Array of mapping entries
    int count;                   // Number of entries
    int capacity;                // Allocated capacity
} SourceMap;

// Create a new source map
SourceMap* source_map_create(const char* mlp_file, const char* c_file);

// Add a mapping entry
void source_map_add_entry(SourceMap* map, int mlp_line, int c_line, const char* scope);

// Find C line from MLP line
int source_map_mlp_to_c(SourceMap* map, int mlp_line);

// Find MLP line from C line
int source_map_c_to_mlp(SourceMap* map, int c_line);

// Load source map from generated C file (parse SOURCEMAP comments)
SourceMap* source_map_load_from_file(const char* c_file);

// Save source map to JSON file
bool source_map_save_json(SourceMap* map, const char* output_file);

// Load source map from JSON file
SourceMap* source_map_load_json(const char* json_file);

// Free source map
void source_map_free(SourceMap* map);

// Print source map (for debugging)
void source_map_print(SourceMap* map);

#endif // SOURCE_MAPPER_H
