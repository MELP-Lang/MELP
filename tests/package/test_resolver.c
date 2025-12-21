/**
 * Test driver for dependency resolver
 * 
 * Usage: ./test_resolver <package.mlp>
 */

#include "../../compiler/stage0/modules/package/package_parser.h"
#include "../../compiler/stage0/modules/package/dependency_resolver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// MOCK PACKAGE LOADER
// ============================================================================

/**
 * Mock package loader for testing
 * Loads packages from tests/package/ directory
 */
Package* mock_package_loader(const char* package_name, const char* version) {
    char filepath[512];
    
    // Try different naming patterns
    const char* patterns[] = {
        "tests/package/test_%s.mlp",          // test_stdlib.mlp
        "tests/package/%s.mlp",                // stdlib.mlp
        "tests/package/test_circular_%s.mlp",  // test_circular_b.mlp (from package_b)
        "tests/package/test_deep_%s.mlp",      // test_deep_a.mlp
        NULL
    };
    
    // Special handling for circular packages: package_a -> circular_a, package_b -> circular_b
    char adjusted_name[256];
    if (strncmp(package_name, "package_", 8) == 0) {
        // package_a -> circular_a
        snprintf(adjusted_name, sizeof(adjusted_name), "circular_%s", package_name + 8);
    } else {
        strncpy(adjusted_name, package_name, sizeof(adjusted_name));
    }
    
    for (int i = 0; patterns[i] != NULL; i++) {
        snprintf(filepath, sizeof(filepath), patterns[i], adjusted_name);
        
        // Try to parse the file
        PackageParseResult result = package_parse_file(filepath);
        if (result.success) {
            printf("  [Loader] Loaded package '%s' from %s\n", package_name, filepath);
            return result.package;
        }
    }
    
    printf("  [Loader] ⚠️  Package '%s' not found (version: %s)\n", package_name, version);
    return NULL;
}

// ============================================================================
// TEST CASES
// ============================================================================

void test_basic_resolution(const char* package_file) {
    printf("\n=== TEST: Basic Dependency Resolution ===\n");
    printf("Package: %s\n", package_file);
    
    DependencyResolutionResult result = dependency_resolve_file(package_file, mock_package_loader);
    dependency_result_print(&result);
    dependency_result_free(&result);
}

void test_circular_dependency(void) {
    printf("\n=== TEST: Circular Dependency Detection ===\n");
    printf("Package: tests/package/test_circular_a.mlp\n");
    printf("Expected: Circular dependency error (A -> B -> A)\n");
    
    DependencyResolutionResult result = dependency_resolve_file(
        "tests/package/test_circular_a.mlp", 
        mock_package_loader
    );
    
    dependency_result_print(&result);
    
    if (!result.success && result.cycle) {
        printf("✅ Circular dependency correctly detected!\n");
    } else {
        printf("❌ Failed to detect circular dependency!\n");
    }
    
    dependency_result_free(&result);
}

void test_deep_dependencies(void) {
    printf("\n=== TEST: Deep Dependency Chain ===\n");
    printf("Package: tests/package/test_deep_root.mlp\n");
    printf("Expected: root -> deep_a -> deep_b -> deep_c\n");
    
    DependencyResolutionResult result = dependency_resolve_file(
        "tests/package/test_deep_root.mlp", 
        mock_package_loader
    );
    
    dependency_result_print(&result);
    
    if (result.success && result.package_count == 4) {
        printf("✅ Deep dependency chain resolved correctly!\n");
    } else {
        printf("❌ Deep dependency resolution failed!\n");
    }
    
    dependency_result_free(&result);
}

void test_no_dependencies(void) {
    printf("\n=== TEST: No Dependencies ===\n");
    printf("Package: tests/package/test_minimal.mlp\n");
    
    DependencyResolutionResult result = dependency_resolve_file(
        "tests/package/test_minimal.mlp", 
        mock_package_loader
    );
    
    dependency_result_print(&result);
    
    if (result.success && result.package_count == 1) {
        printf("✅ Single package (no deps) resolved correctly!\n");
    } else {
        printf("❌ Single package resolution failed!\n");
    }
    
    dependency_result_free(&result);
}

void test_with_dependencies(void) {
    printf("\n=== TEST: Package with Dependencies ===\n");
    printf("Package: tests/package/test_dependencies.mlp\n");
    
    DependencyResolutionResult result = dependency_resolve_file(
        "tests/package/test_dependencies.mlp", 
        mock_package_loader
    );
    
    dependency_result_print(&result);
    dependency_result_free(&result);
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    printf("==============================================\n");
    printf("   Dependency Resolver Test Suite\n");
    printf("==============================================\n");
    
    if (argc >= 2) {
        // Test specific file
        test_basic_resolution(argv[1]);
    } else {
        // Run all tests
        printf("\nRunning all test cases...\n");
        
        test_no_dependencies();
        test_with_dependencies();
        test_deep_dependencies();
        test_circular_dependency();
        
        printf("\n==============================================\n");
        printf("   All Tests Complete\n");
        printf("==============================================\n");
    }
    
    return 0;
}
