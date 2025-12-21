#include "package_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * Create a new empty Package structure
 */
Package* package_create(void) {
    Package* pkg = (Package*)calloc(1, sizeof(Package));
    if (!pkg) return NULL;
    
    // Set defaults
    pkg->name = NULL;
    pkg->version_str = NULL;
    pkg->author = NULL;
    pkg->entry = NULL;
    pkg->description = NULL;
    pkg->dependencies = NULL;
    pkg->dependency_count = 0;
    pkg->build_dir = strdup("build/");
    pkg->dist_dir = strdup("dist/");
    pkg->manifest_path = NULL;
    pkg->is_valid = false;
    
    return pkg;
}

/**
 * Free a Dependency structure
 */
void dependency_free(Dependency* dep) {
    if (!dep) return;
    
    free(dep->name);
    free(dep->version_required);
    free(dep);
}

/**
 * Free a Package structure and all its contents
 */
void package_free(Package* package) {
    if (!package) return;
    
    free(package->name);
    free(package->version_str);
    free(package->author);
    free(package->entry);
    free(package->description);
    free(package->build_dir);
    free(package->dist_dir);
    free(package->manifest_path);
    
    // Free dependencies
    if (package->dependencies) {
        for (int i = 0; i < package->dependency_count; i++) {
            dependency_free(package->dependencies[i]);
        }
        free(package->dependencies);
    }
    
    free(package);
}

/**
 * Create error result
 */
PackageParseResult package_error(const char* error_msg, int line) {
    PackageParseResult result;
    result.package = NULL;
    result.success = false;
    result.error_message = strdup(error_msg);
    result.error_line = line;
    return result;
}

/**
 * Create success result
 */
PackageParseResult package_success(Package* package) {
    PackageParseResult result;
    result.package = package;
    result.success = true;
    result.error_message = NULL;
    result.error_line = -1;
    return result;
}

// ============================================================================
// VERSION PARSING
// ============================================================================

/**
 * Parse semantic version string "1.2.3" into SemanticVersion struct
 */
bool package_parse_version(const char* version_str, SemanticVersion* out_version) {
    if (!version_str || !out_version) return false;
    
    int major = 0, minor = 0, patch = 0;
    int parsed = sscanf(version_str, "%d.%d.%d", &major, &minor, &patch);
    
    if (parsed < 1) return false;  // At least major version required
    
    out_version->major = major;
    out_version->minor = (parsed >= 2) ? minor : 0;
    out_version->patch = (parsed >= 3) ? patch : 0;
    
    return true;
}

// ============================================================================
// FIELD PARSING
// ============================================================================

/**
 * Extract string value from STRING token
 * Removes quotes from "value" -> value
 */
static char* extract_string_value(const char* token_value) {
    if (!token_value) return NULL;
    
    size_t len = strlen(token_value);
    if (len < 2) return strdup(token_value);
    
    // Remove surrounding quotes
    if (token_value[0] == '"' && token_value[len-1] == '"') {
        char* result = (char*)malloc(len - 1);
        if (!result) return NULL;
        strncpy(result, token_value + 1, len - 2);
        result[len - 2] = '\0';
        return result;
    }
    
    return strdup(token_value);
}

/**
 * Parse dependencies array: dependencies = ["stdlib:1.0", "math:2.0"]
 */
Dependency** package_parse_dependencies(Token* tokens, int token_count, int* current, int* dep_count) {
    *dep_count = 0;
    
    // Expect '['
    if (*current >= token_count || strcmp(tokens[*current].value, "[") != 0) {
        return NULL;
    }
    (*current)++;  // Skip '['
    
    // Count dependencies first
    int temp_idx = *current;
    int count = 0;
    while (temp_idx < token_count && strcmp(tokens[temp_idx].value, "]") != 0) {
        if (tokens[temp_idx].type == TOKEN_STRING) {
            count++;
        }
        temp_idx++;
    }
    
    if (count == 0) {
        // Empty array: dependencies = []
        (*current)++;  // Skip ']'
        return NULL;
    }
    
    // Allocate dependency array
    Dependency** deps = (Dependency**)calloc(count, sizeof(Dependency*));
    if (!deps) return NULL;
    
    int dep_idx = 0;
    while (*current < token_count && strcmp(tokens[*current].value, "]") != 0) {
        if (tokens[*current].type == TOKEN_STRING) {
            char* dep_str = extract_string_value(tokens[*current].value);
            
            // Parse "package:version" format
            char* colon = strchr(dep_str, ':');
            if (colon) {
                *colon = '\0';  // Split at colon
                char* pkg_name = dep_str;
                char* version = colon + 1;
                
                Dependency* dep = (Dependency*)calloc(1, sizeof(Dependency));
                dep->name = strdup(pkg_name);
                dep->version_required = strdup(version);
                package_parse_version(version, &dep->version);
                dep->resolved_package = NULL;
                
                deps[dep_idx++] = dep;
            }
            
            free(dep_str);
        }
        (*current)++;
    }
    
    // Skip ']'
    if (*current < token_count && strcmp(tokens[*current].value, "]") == 0) {
        (*current)++;
    }
    
    *dep_count = dep_idx;
    return deps;
}

/**
 * Parse a single field inside package block
 * Example: name = "my_project"
 */
bool package_parse_field(Token* tokens, int token_count, int* current, Package* package) {
    if (*current >= token_count) return false;
    
    Token* field_name_token = &tokens[*current];
    if (field_name_token->type != TOKEN_IDENTIFIER) {
        return false;
    }
    
    char* field_name = field_name_token->value;
    (*current)++;
    
    // Expect '='
    if (*current >= token_count || strcmp(tokens[*current].value, "=") != 0) {
        return false;
    }
    (*current)++;
    
    // Get field value
    if (*current >= token_count) return false;
    Token* value_token = &tokens[*current];
    
    // Parse based on field name
    if (strcmp(field_name, "name") == 0) {
        if (value_token->type != TOKEN_STRING) return false;
        package->name = extract_string_value(value_token->value);
        (*current)++;
        
    } else if (strcmp(field_name, "version") == 0) {
        if (value_token->type != TOKEN_STRING) return false;
        package->version_str = extract_string_value(value_token->value);
        package_parse_version(package->version_str, &package->version);
        (*current)++;
        
    } else if (strcmp(field_name, "author") == 0) {
        if (value_token->type != TOKEN_STRING) return false;
        package->author = extract_string_value(value_token->value);
        (*current)++;
        
    } else if (strcmp(field_name, "entry") == 0) {
        if (value_token->type != TOKEN_STRING) return false;
        package->entry = extract_string_value(value_token->value);
        (*current)++;
        
    } else if (strcmp(field_name, "description") == 0) {
        if (value_token->type != TOKEN_STRING) return false;
        package->description = extract_string_value(value_token->value);
        (*current)++;
        
    } else if (strcmp(field_name, "dependencies") == 0) {
        package->dependencies = package_parse_dependencies(
            tokens, token_count, current, &package->dependency_count
        );
        
    } else {
        // Unknown field - skip it
        (*current)++;
    }
    
    // Skip optional comma
    if (*current < token_count && strcmp(tokens[*current].value, ",") == 0) {
        (*current)++;
    }
    
    return true;
}

// ============================================================================
// PACKAGE BLOCK PARSING
// ============================================================================

/**
 * Parse package block: package { ... }
 */
Package* package_parse_block(Token* tokens, int token_count, int* current) {
    Package* package = package_create();
    if (!package) return NULL;
    
    // Expect 'package' keyword
    if (*current >= token_count || 
        tokens[*current].type != TOKEN_IDENTIFIER ||
        strcmp(tokens[*current].value, "package") != 0) {
        package_free(package);
        return NULL;
    }
    (*current)++;
    
    // Expect '{'
    if (*current >= token_count || strcmp(tokens[*current].value, "{") != 0) {
        package_free(package);
        return NULL;
    }
    (*current)++;
    
    // Parse fields until '}'
    while (*current < token_count && strcmp(tokens[*current].value, "}") != 0) {
        if (!package_parse_field(tokens, token_count, current, package)) {
            // Field parsing failed - continue to next token
            (*current)++;
        }
    }
    
    // Expect '}'
    if (*current >= token_count || strcmp(tokens[*current].value, "}") != 0) {
        package_free(package);
        return NULL;
    }
    (*current)++;
    
    return package;
}

// ============================================================================
// VALIDATION
// ============================================================================

/**
 * Validate package manifest (check required fields)
 */
bool package_validate(Package* package) {
    if (!package) return false;
    
    // Required fields: name, version
    if (!package->name || strlen(package->name) == 0) {
        return false;
    }
    
    if (!package->version_str || strlen(package->version_str) == 0) {
        return false;
    }
    
    // Version must be valid semantic version
    if (package->version.major < 0) {
        return false;
    }
    
    package->is_valid = true;
    return true;
}

// ============================================================================
// MAIN PARSING FUNCTIONS
// ============================================================================

/**
 * Parse a package manifest from a token stream
 */
PackageParseResult package_parse_tokens(Token* tokens, int token_count) {
    if (!tokens || token_count == 0) {
        return package_error("Empty token stream", 0);
    }
    
    int current = 0;
    Package* package = package_parse_block(tokens, token_count, &current);
    
    if (!package) {
        return package_error("Failed to parse package block", 0);
    }
    
    // Validate package
    if (!package_validate(package)) {
        char error_buf[256];
        if (!package->name) {
            snprintf(error_buf, sizeof(error_buf), 
                    "Missing required field 'name' in package manifest");
        } else if (!package->version_str) {
            snprintf(error_buf, sizeof(error_buf), 
                    "Missing required field 'version' in package manifest");
        } else {
            snprintf(error_buf, sizeof(error_buf), 
                    "Invalid package manifest");
        }
        package_free(package);
        return package_error(error_buf, 0);
    }
    
    return package_success(package);
}

/**
 * Parse a package manifest file (package.mlp)
 */
PackageParseResult package_parse_file(const char* filepath) {
    if (!filepath) {
        return package_error("NULL filepath provided", 0);
    }
    
    // Read file
    FILE* file = fopen(filepath, "r");
    if (!file) {
        char error_buf[512];
        snprintf(error_buf, sizeof(error_buf), 
                "Failed to open package file: %s", filepath);
        return package_error(error_buf, 0);
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read content
    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return package_error("Memory allocation failed", 0);
    }
    
    size_t read_size = fread(content, 1, file_size, file);
    content[read_size] = '\0';
    fclose(file);
    
    // Create lexer and tokenize
    Lexer* lexer = lexer_create(content);
    if (!lexer) {
        free(content);
        return package_error("Failed to create lexer", 0);
    }
    
    // Collect all tokens
    Token** token_array = NULL;
    int token_count = 0;
    int token_capacity = 16;
    token_array = (Token**)malloc(token_capacity * sizeof(Token*));
    
    while (1) {
        Token* token = lexer_next_token(lexer);
        if (!token || token->type == TOKEN_EOF) {
            if (token) token_free(token);
            break;
        }
        
        // Expand array if needed
        if (token_count >= token_capacity) {
            token_capacity *= 2;
            token_array = (Token**)realloc(token_array, token_capacity * sizeof(Token*));
        }
        
        token_array[token_count++] = token;
    }
    
    lexer_free(lexer);
    free(content);
    
    if (token_count == 0) {
        free(token_array);
        return package_error("Failed to tokenize package file", 0);
    }
    
    // Convert Token** to Token* array for parsing
    Token* tokens = (Token*)malloc(token_count * sizeof(Token));
    for (int i = 0; i < token_count; i++) {
        tokens[i] = *token_array[i];
    }
    
    // Parse tokens
    PackageParseResult result = package_parse_tokens(tokens, token_count);
    
    // Set manifest path
    if (result.success && result.package) {
        result.package->manifest_path = strdup(filepath);
    }
    
    // Free tokens
    for (int i = 0; i < token_count; i++) {
        token_free(token_array[i]);
    }
    free(token_array);
    free(tokens);
    
    return result;
}
