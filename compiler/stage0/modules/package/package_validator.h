#ifndef PACKAGE_VALIDATOR_H
#define PACKAGE_VALIDATOR_H

#include "package_types.h"
#include <stdbool.h>

/**
 * Validation Result
 */
typedef struct {
    bool valid;                     // Overall validation status
    char** errors;                  // Array of error messages
    int error_count;                // Number of errors
    char** warnings;                // Array of warning messages
    int warning_count;              // Number of warnings
} ValidationResult;

// ============================================================================
// VALIDATION FUNCTIONS
// ============================================================================

/**
 * Validate a package manifest
 * Checks required fields, format, dependencies, etc.
 * 
 * @param package Package to validate
 * @return ValidationResult
 */
ValidationResult package_validate_full(Package* package);

/**
 * Validate required fields
 */
bool validate_required_fields(Package* package, ValidationResult* result);

/**
 * Validate package name format
 * Must be alphanumeric with underscores/hyphens
 */
bool validate_package_name(const char* name, ValidationResult* result);

/**
 * Validate version string format
 * Must be semantic version: X.Y.Z
 */
bool validate_version_format(const char* version, ValidationResult* result);

/**
 * Validate dependencies
 * Check format and version requirements
 */
bool validate_dependencies(Package* package, ValidationResult* result);

/**
 * Validate entry point
 * Check if file exists (if specified)
 */
bool validate_entry_point(Package* package, ValidationResult* result);

// ============================================================================
// VALIDATION RESULT MANAGEMENT
// ============================================================================

/**
 * Create validation result
 */
ValidationResult validation_result_create(void);

/**
 * Add error to validation result
 */
void validation_add_error(ValidationResult* result, const char* error);

/**
 * Add warning to validation result
 */
void validation_add_warning(ValidationResult* result, const char* warning);

/**
 * Free validation result
 */
void validation_result_free(ValidationResult* result);

/**
 * Print validation result
 */
void validation_result_print(ValidationResult* result);

// ============================================================================
// UTILITY VALIDATORS
// ============================================================================

/**
 * Check if string is valid identifier (alphanumeric + underscore)
 */
bool is_valid_identifier(const char* str);

/**
 * Check if string is valid version format (X.Y.Z)
 */
bool is_valid_version(const char* version);

/**
 * Check if file exists
 */
bool file_exists(const char* path);

#endif // PACKAGE_VALIDATOR_H
