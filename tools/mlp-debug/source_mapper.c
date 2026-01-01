#include "source_mapper.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Create a new source map
SourceMap* source_map_create(const char* mlp_file, const char* c_file) {
    SourceMap* map = (SourceMap*)malloc(sizeof(SourceMap));
    if (!map) return NULL;
    
    map->mlp_file = strdup(mlp_file);
    map->c_file = strdup(c_file);
    map->entries = NULL;
    map->count = 0;
    map->capacity = 0;
    
    return map;
}

// Add a mapping entry
void source_map_add_entry(SourceMap* map, int mlp_line, int c_line, const char* scope) {
    if (!map) return;
    
    // Grow array if needed
    if (map->count >= map->capacity) {
        int new_capacity = map->capacity == 0 ? 16 : map->capacity * 2;
        SourceMapEntry* new_entries = (SourceMapEntry*)realloc(
            map->entries, new_capacity * sizeof(SourceMapEntry));
        if (!new_entries) return;
        
        map->entries = new_entries;
        map->capacity = new_capacity;
    }
    
    // Add entry
    SourceMapEntry* entry = &map->entries[map->count++];
    entry->mlp_line = mlp_line;
    entry->c_line = c_line;
    entry->scope = scope ? strdup(scope) : NULL;
    entry->column = 0;
}

// Find C line from MLP line
int source_map_mlp_to_c(SourceMap* map, int mlp_line) {
    if (!map || !map->entries) return -1;
    
    // Find closest mapping
    int best_c_line = -1;
    int best_diff = 999999;
    
    for (int i = 0; i < map->count; i++) {
        int diff = abs(map->entries[i].mlp_line - mlp_line);
        if (diff < best_diff) {
            best_diff = diff;
            best_c_line = map->entries[i].c_line;
        }
    }
    
    return best_c_line;
}

// Find MLP line from C line
int source_map_c_to_mlp(SourceMap* map, int c_line) {
    if (!map || !map->entries) return -1;
    
    // Find closest mapping
    int best_mlp_line = -1;
    int best_diff = 999999;
    
    for (int i = 0; i < map->count; i++) {
        int diff = abs(map->entries[i].c_line - c_line);
        if (diff < best_diff) {
            best_diff = diff;
            best_mlp_line = map->entries[i].mlp_line;
        }
    }
    
    return best_mlp_line;
}

// Load source map from generated C file (parse SOURCEMAP comments)
SourceMap* source_map_load_from_file(const char* c_file) {
    FILE* f = fopen(c_file, "r");
    if (!f) return NULL;
    
    // Try to find MLP file name from first comment
    char line[1024];
    char mlp_file[512] = "unknown.mlp";
    
    // Look for debug info comment
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Debug info enabled for:")) {
            char* start = strchr(line, ':');
            if (start) {
                start += 2; // Skip ": "
                char* end = strchr(start, '\n');
                if (end) *end = '\0';
                strncpy(mlp_file, start, sizeof(mlp_file) - 1);
                break;
            }
        }
    }
    
    rewind(f);
    
    SourceMap* map = source_map_create(mlp_file, c_file);
    if (!map) {
        fclose(f);
        return NULL;
    }
    
    // Parse SOURCEMAP comments
    int c_line_num = 0;
    while (fgets(line, sizeof(line), f)) {
        c_line_num++;
        
        // Look for SOURCEMAP comments
        if (strstr(line, "/* SOURCEMAP:")) {
            int mlp_line, c_line;
            if (sscanf(line, "/* SOURCEMAP: mlp:%d -> c:%d */", &mlp_line, &c_line) == 2) {
                source_map_add_entry(map, mlp_line, c_line, "global");
            }
        }
        
        // Also parse #line directives
        if (strncmp(line, "#line", 5) == 0) {
            int mlp_line;
            char file[512];
            if (sscanf(line, "#line %d \"%[^\"]\"", &mlp_line, file) == 2) {
                source_map_add_entry(map, mlp_line, c_line_num + 1, "global");
            }
        }
    }
    
    fclose(f);
    return map;
}

// Save source map to JSON file
bool source_map_save_json(SourceMap* map, const char* output_file) {
    if (!map) return false;
    
    FILE* f = fopen(output_file, "w");
    if (!f) return false;
    
    fprintf(f, "{\n");
    fprintf(f, "  \"version\": \"1.0\",\n");
    fprintf(f, "  \"sourceFile\": \"%s\",\n", map->mlp_file);
    fprintf(f, "  \"generatedFile\": \"%s\",\n", map->c_file);
    fprintf(f, "  \"mappings\": [\n");
    
    for (int i = 0; i < map->count; i++) {
        SourceMapEntry* e = &map->entries[i];
        fprintf(f, "    {\"mlpLine\": %d, \"cLine\": %d, \"scope\": \"%s\"}",
                e->mlp_line, e->c_line, e->scope ? e->scope : "global");
        if (i < map->count - 1) fprintf(f, ",");
        fprintf(f, "\n");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    return true;
}

// Simple JSON parser for source map (minimal implementation)
SourceMap* source_map_load_json(const char* json_file) {
    FILE* f = fopen(json_file, "r");
    if (!f) return NULL;
    
    char mlp_file[512] = "";
    char c_file[512] = "";
    
    // Simple line-by-line parsing (not a full JSON parser)
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"sourceFile\"")) {
            char* start = strchr(line, '"');
            if (start) {
                start = strchr(start + 1, '"');
                if (start) {
                    start++;
                    char* end = strchr(start, '"');
                    if (end) {
                        *end = '\0';
                        strncpy(mlp_file, start, sizeof(mlp_file) - 1);
                    }
                }
            }
        }
        else if (strstr(line, "\"generatedFile\"")) {
            char* start = strchr(line, '"');
            if (start) {
                start = strchr(start + 1, '"');
                if (start) {
                    start++;
                    char* end = strchr(start, '"');
                    if (end) {
                        *end = '\0';
                        strncpy(c_file, start, sizeof(c_file) - 1);
                    }
                }
            }
        }
    }
    
    rewind(f);
    
    SourceMap* map = source_map_create(mlp_file, c_file);
    
    // Parse mappings
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"mlpLine\"")) {
            int mlp_line, c_line;
            char scope[128] = "global";
            
            // Simple parse: {"mlpLine": N, "cLine": M, "scope": "S"}
            char* p = line;
            while (*p) {
                if (strncmp(p, "\"mlpLine\"", 9) == 0) {
                    sscanf(p + 9, ": %d", &mlp_line);
                }
                else if (strncmp(p, "\"cLine\"", 7) == 0) {
                    sscanf(p + 7, ": %d", &c_line);
                }
                else if (strncmp(p, "\"scope\"", 7) == 0) {
                    char* start = strchr(p + 7, '"');
                    if (start) {
                        start++;
                        char* end = strchr(start, '"');
                        if (end) {
                            int len = end - start;
                            if (len < sizeof(scope)) {
                                strncpy(scope, start, len);
                                scope[len] = '\0';
                            }
                        }
                    }
                }
                p++;
            }
            
            source_map_add_entry(map, mlp_line, c_line, scope);
        }
    }
    
    fclose(f);
    return map;
}

// Free source map
void source_map_free(SourceMap* map) {
    if (!map) return;
    
    free(map->mlp_file);
    free(map->c_file);
    
    for (int i = 0; i < map->count; i++) {
        if (map->entries[i].scope) {
            free(map->entries[i].scope);
        }
    }
    
    free(map->entries);
    free(map);
}

// Print source map (for debugging)
void source_map_print(SourceMap* map) {
    if (!map) {
        printf("Source map is NULL\n");
        return;
    }
    
    printf("=== Source Map ===\n");
    printf("MLP File: %s\n", map->mlp_file);
    printf("C File: %s\n", map->c_file);
    printf("Mappings: %d\n\n", map->count);
    
    printf("%-10s %-10s %-15s\n", "MLP Line", "C Line", "Scope");
    printf("%-10s %-10s %-15s\n", "--------", "------", "-----");
    
    for (int i = 0; i < map->count; i++) {
        SourceMapEntry* e = &map->entries[i];
        printf("%-10d %-10d %-15s\n", 
               e->mlp_line, e->c_line, e->scope ? e->scope : "global");
    }
}
