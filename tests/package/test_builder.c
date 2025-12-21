/**
 * Test driver for package builder and validator
 * 
 * Usage: ./test_builder <package.mlp>
 */

#include "../../compiler/stage0/modules/package/package_parser.h"
#include "../../compiler/stage0/modules/package/package_validator.h"
#include "../../compiler/stage0/modules/package/package_builder.h"
#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// TEST CASES
// ============================================================================

void test_validation(const char* package_file) {
    printf("\n=== TEST: Package Validation ===\n");
    printf("Package: %s\n", package_file);
    
    // Parse package
    PackageParseResult parse_result = package_parse_file(package_file);
    if (!parse_result.success) {
        printf("❌ Failed to parse package: %s\n", parse_result.error_message);
        free(parse_result.error_message);
        return;
    }
    
    printf("✅ Package parsed successfully\n");
    
    // Validate package
    ValidationResult val_result = package_validate_full(parse_result.package);
    validation_result_print(&val_result);
    
    validation_result_free(&val_result);
    package_free(parse_result.package);
}

void test_build(const char* package_file) {
    printf("\n=== TEST: Package Build ===\n");
    printf("Package: %s\n", package_file);
    
    // Create build configuration
    BuildConfig* config = build_config_create();
    config->verbose = true;
    
    printf("\n");
    build_config_print(config);
    
    // Build package
    BuildResult result = build_package_with_deps(package_file, config);
    build_result_print(&result);
    
    build_result_free(&result);
    build_config_free(config);
}

void test_directory_creation(void) {
    printf("\n=== TEST: Directory Creation ===\n");
    
    BuildConfig* config = build_config_create();
    
    printf("Creating directories...\n");
    printf("  - %s\n", config->build_dir);
    printf("  - %s\n", config->obj_dir);
    printf("  - %s\n", config->bin_dir);
    printf("  - %s\n", config->dist_dir);
    
    bool success = build_create_directories(config);
    
    if (success) {
        printf("✅ Directories created successfully\n");
    } else {
        printf("❌ Failed to create directories\n");
    }
    
    build_config_free(config);
}

void run_all_validation_tests(void) {
    printf("\n============================================\n");
    printf("   Validation Test Suite\n");
    printf("============================================\n");
    
    test_validation("tests/package/test_basic_package.mlp");
    test_validation("tests/package/test_dependencies.mlp");
    test_validation("tests/package/test_minimal.mlp");
    test_validation("tests/package/test_complete.mlp");
    test_validation("tests/package/test_invalid.mlp");
}

void run_all_build_tests(void) {
    printf("\n============================================\n");
    printf("   Build Test Suite\n");
    printf("============================================\n");
    
    test_directory_creation();
    test_build("tests/package/test_minimal.mlp");
    test_build("tests/package/test_basic_package.mlp");
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    printf("==============================================\n");
    printf("   Package Builder & Validator Test Suite\n");
    printf("==============================================\n");
    
    if (argc >= 2) {
        // Test specific file
        const char* package_file = argv[1];
        
        test_validation(package_file);
        test_build(package_file);
    } else {
        // Run all tests
        printf("\nRunning all test cases...\n");
        
        run_all_validation_tests();
        run_all_build_tests();
        
        printf("\n==============================================\n");
        printf("   All Tests Complete\n");
        printf("==============================================\n");
    }
    
    return 0;
}
