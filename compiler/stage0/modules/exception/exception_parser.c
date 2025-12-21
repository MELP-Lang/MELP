#include "exception_parser.h"
#include "exception_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Exception Definition Parser
// ============================================================================

int parse_exception_definition(Token** tokens, int* pos) {
    // Expect: exception
    if (tokens[*pos]->type != TOKEN_EXCEPTION) {
        fprintf(stderr, "Error: Expected 'exception' keyword\n");
        return 0;
    }
    (*pos)++; // Skip 'exception'
    
    // Expect: identifier (exception name)
    if (tokens[*pos]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected exception name after 'exception'\n");
        return 0;
    }
    char* exception_name = strdup(tokens[*pos]->value);
    (*pos)++;
    
    // Optional: extends parent_type
    char* parent_type = NULL;
    if (tokens[*pos]->type == TOKEN_IDENTIFIER && 
        strcmp(tokens[*pos]->value, "extends") == 0) {
        (*pos)++; // Skip 'extends'
        
        // Expect: identifier (parent exception type)
        if (tokens[*pos]->type != TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Expected parent exception type after 'extends'\n");
            free(exception_name);
            return 0;
        }
        parent_type = strdup(tokens[*pos]->value);
        (*pos)++;
    } else {
        // Default parent: Exception
        parent_type = strdup(EXCEPTION_TYPE_BASE);
    }
    
    // Parse exception body (fields) - simplified for now
    // TODO: Parse struct-like fields
    while (tokens[*pos]->type != TOKEN_END_EXCEPTION &&
           tokens[*pos]->type != TOKEN_EOF) {
        // Skip field definitions for now
        // In full implementation, parse: string field_name, etc.
        (*pos)++;
    }
    
    // Expect: end_exception
    if (tokens[*pos]->type != TOKEN_END_EXCEPTION) {
        fprintf(stderr, "Error: Expected 'end_exception' at line %d\n", 
                tokens[*pos]->line);
        free(exception_name);
        free(parent_type);
        return 0;
    }
    (*pos)++; // Skip 'end_exception'
    
    // Register custom exception type
    register_exception_type(exception_name, EXCEPTION_CUSTOM, parent_type);
    
    printf("Registered custom exception: %s extends %s\n", 
           exception_name, parent_type);
    
    free(exception_name);
    free(parent_type);
    
    return 1;  // Success
}
