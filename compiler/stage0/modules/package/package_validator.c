#include "package_validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// ============================================================================
// VALIDATION RESULT MANAGEMENT
// ============================================================================

/**
 * Create validation result
 */
ValidationResult validation_result_create(void) {
    ValidationResult result;
    result.valid = true;
    result.errors = NULL;
    result.error_count = 0;
    result.warnings = NULL;
    result.warning_count = 0;
    return result;
}

/**
 * Add error to validation result
 */
void validation_add_error(ValidationResult* result, const char* error) {
    if (!result || !error) return;
    
    result->valid = false;
    
    result->errors = (char**)realloc(result->errors, (result->error_count + 1) * sizeof(char*));
    result->errors[result->error_count++] = strdup(error);
}

/**
 * Add warning to validation result
 */
void validation_add_warning(ValidationResult* result, const char* warning) {
    if (!result || !warning) return;
    
    result->warnings = (char**)realloc(result->warnings, (result->warning_count + 1) * sizeof(char*));
    result->warnings[result->warning_count++] = strdup(warning);
}

/**
 * Free validation result
 */
void validation_result_free(ValidationResult* result) {
    if (!result) return;
    
    for (int i = 0; i < result->error_count; i++) {
        free(result->errors[i]);
    }
    free(result->errors);
    
    for (int i = 0; i < result->warning_count; i++) {
        free(result->warnings[i]);
    }
    free(result->warnings);
}

/**
 * Print validation result
 */
void validation_result_print(ValidationResult* result) {
    if (!result) return;
    
    printf("\n=== Validation Result ===\n");
    
    if (result->valid) {
        printf("Status: ✅ VALID\n");
    } else {
        printf("Status: ❌ INVALID\n");
    }
    
    if (result->error_count > 0) {
        printf("\nErrors (%d):\n", result->error_count);
        for (int i = 0; i < result->error_count; i++) {
            printf("  ❌ %s\n", result->errors[i]);
        }
    }
    
    if (result->warning_count > 0) {
        printf("\nWarnings (%d):\n", result->warning_count);
        for (int i = 0; i < result->warning_count; i++) {
            printf("  ⚠️  %s\n", result->warnings[i]);
        }
    }
    
    if (result->valid && result->error_count == 0 && result->warning_count == 0) {
        printf("No issues found.\n");
    }
    
    printf("=========================\n\n");
}

// ============================================================================
// UTILITY VALIDATORS
// ============================================================================

/**
 * Check if string is valid identifier
 */
bool is_valid_identifier(const char* str) {
    if (!str || strlen(str) == 0) return false;
    
    // First character must be letter or underscore
    if (!isalpha(str[0]) && str[0] != '_') {
        return false;
    }
    
    // Rest can be alphanumeric, underscore, or hyphen
    for (size_t i = 1; i < strlen(str); i++) {
        if (!isalnum(str[i]) && str[i] != '_' && str[i] != '-') {
            return false;
        }
    }
    
    return true;
}

/**
 * Check if string is valid version format
 */
bool is_valid_version(const char* version) {
    if (!version) return false;
    
    int major, minor, patch;
    int parsed = sscanf(version, "%d.%d.%d", &major, &minor, &patch);
    
    // At least major version required
    return (parsed >= 1 && major >= 0);
}

/**
 * Check if file exists
 */
bool file_exists(const char* path) {
    if (!path) return false;
    
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

// ============================================================================
// FIELD VALIDATORS
// ============================================================================

/**
 * Validate required fields
 */
bool validate_required_fields(Package* package, ValidationResult* result) {
    bool valid = true;
    
    if (!package->name || strlen(package->name) == 0) {
        validation_add_error(result, "Missing required field: 'name'");
        valid = false;
    }
    
    if (!package->version_str || strlen(package->version_str) == 0) {
        validation_add_error(result, "Missing required field: 'version'");
        valid = false;
    }
    
    return valid;
}

/**
 * Validate package name format
 */
bool validate_package_name(const char* name, ValidationResult* result) {
    if (!name) return true;  // Already checked in required_fields
    
    if (!is_valid_identifier(name)) {
        char error[256];
        snprintf(error, sizeof(error), 
                "Invalid package name '%s': must be alphanumeric with underscores/hyphens", 
                name);
        validation_add_error(result, error);
        return false;
    }
    
    // Check length
    if (strlen(name) > 64) {
        validation_add_warning(result, "Package name is very long (>64 characters)");
    }
    
    return true;
}

/**
 * Validate version string format
 */
bool validate_version_format(const char* version, ValidationResult* result) {
    if (!version) return true;  // Already checked in required_fields
    
    if (!is_valid_version(version)) {
        char error[256];
        snprintf(error, sizeof(error), 
                "Invalid version format '%s': must be semantic version (e.g., 1.0.0)", 
                version);
        validation_add_error(result, error);
        return false;
    }
    
    return true;
}

/**
 * Validate dependencies
 */
bool validate_dependencies(Package* package, ValidationResult* result) {
    if (!package || package->dependency_count == 0) {
        return true;  // No dependencies to validate
    }
    
    bool valid = true;
    
    for (int i = 0; i < package->dependency_count; i++) {
        Dependency* dep = package->dependencies[i];
        
        // Check dependency name
        if (!dep->name || strlen(dep->name) == 0) {
            validation_add_error(result, "Dependency has empty name");
            valid = false;
            continue;
        }
        
        if (!is_valid_identifier(dep->name)) {
            char error[256];
            snprintf(error, sizeof(error), 
                    "Invalid dependency name '%s': must be alphanumeric with underscores/hyphens",
                    dep->name);
            validation_add_error(result, error);
            valid = false;
        }
        
        // Check version
        if (!dep->version_required || strlen(dep->version_required) == 0) {
            char warning[256];
            snprintf(warning, sizeof(warning), 
                    "Dependency '%s' has no version specified",
                    dep->name);
            validation_add_warning(result, warning);
        } else if (!is_valid_version(dep->version_required)) {
            char error[256];
            snprintf(error, sizeof(error), 
                    "Dependency '%s' has invalid version format '%s'",
                    dep->name, dep->version_required);
            validation_add_error(result, error);
            valid = false;
        }
    }
    
    // Check for duplicate dependencies
    for (int i = 0; i < package->dependency_count; i++) {
        for (int j = i + 1; j < package->dependency_count; j++) {
            if (strcmp(package->dependencies[i]->name, 
                      package->dependencies[j]->name) == 0) {
                char warning[256];
                snprintf(warning, sizeof(warning), 
                        "Duplicate dependency '%s'",
                        package->dependencies[i]->name);
                validation_add_warning(result, warning);
            }
        }
    }
    
    return valid;
}

/**
 * Validate entry point
 */
bool validate_entry_point(Package* package, ValidationResult* result) {
    if (!package || !package->entry) {
        return true;  // No entry point specified (optional)
    }
    
    // Check if entry file exists
    if (!file_exists(package->entry)) {
        char warning[512];
        snprintf(warning, sizeof(warning), 
                "Entry point file '%s' not found",
                package->entry);
        validation_add_warning(result, warning);
        // Not an error, file might not exist yet
    }
    
    // Check file extension
    const char* ext = strrchr(package->entry, '.');
    if (!ext || strcmp(ext, ".mlp") != 0) {
        char warning[256];
        snprintf(warning, sizeof(warning), 
                "Entry point '%s' doesn't have .mlp extension",
                package->entry);
        validation_add_warning(result, warning);
    }
    
    return true;
}

// ============================================================================
// MAIN VALIDATION
// ============================================================================

/**
 * Validate a package manifest
 */
ValidationResult package_validate_full(Package* package) {
    ValidationResult result = validation_result_create();
    
    if (!package) {
        validation_add_error(&result, "NULL package provided");
        return result;
    }
    
    // Validate required fields
    validate_required_fields(package, &result);
    
    // Validate package name
    validate_package_name(package->name, &result);
    
    // Validate version format
    validate_version_format(package->version_str, &result);
    
    // Validate dependencies
    validate_dependencies(package, &result);
    
    // Validate entry point
    validate_entry_point(package, &result);
    
    // Additional checks
    if (package->author && strlen(package->author) > 128) {
        validation_add_warning(&result, "Author field is very long (>128 characters)");
    }
    
    if (package->description && strlen(package->description) > 512) {
        validation_add_warning(&result, "Description field is very long (>512 characters)");
    }
    
    return result;
}
