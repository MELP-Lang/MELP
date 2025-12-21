/**
 * Test driver for package parser
 * 
 * Usage: ./test_package <package.mlp>
 */

#include "../../compiler/stage0/modules/package/package_parser.h"
#include <stdio.h>
#include <stdlib.h>

void print_package(Package* pkg) {
    printf("\n=== Package Information ===\n");
    printf("Name: %s\n", pkg->name ? pkg->name : "(null)");
    printf("Version: %s (%d.%d.%d)\n", 
           pkg->version_str ? pkg->version_str : "(null)",
           pkg->version.major, pkg->version.minor, pkg->version.patch);
    
    if (pkg->author) {
        printf("Author: %s\n", pkg->author);
    }
    
    if (pkg->description) {
        printf("Description: %s\n", pkg->description);
    }
    
    if (pkg->entry) {
        printf("Entry: %s\n", pkg->entry);
    }
    
    printf("Build dir: %s\n", pkg->build_dir);
    printf("Dist dir: %s\n", pkg->dist_dir);
    
    if (pkg->dependency_count > 0) {
        printf("\nDependencies (%d):\n", pkg->dependency_count);
        for (int i = 0; i < pkg->dependency_count; i++) {
            Dependency* dep = pkg->dependencies[i];
            printf("  - %s : %s (%d.%d.%d)\n", 
                   dep->name, 
                   dep->version_required,
                   dep->version.major, 
                   dep->version.minor, 
                   dep->version.patch);
        }
    }
    
    printf("\nValid: %s\n", pkg->is_valid ? "YES" : "NO");
    printf("===========================\n\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <package.mlp>\n", argv[0]);
        printf("\nTest files available:\n");
        printf("  - tests/package/test_basic_package.mlp\n");
        printf("  - tests/package/test_dependencies.mlp\n");
        printf("  - tests/package/test_minimal.mlp\n");
        printf("  - tests/package/test_invalid.mlp\n");
        printf("  - tests/package/test_complete.mlp\n");
        return 1;
    }
    
    const char* filepath = argv[1];
    printf("Parsing package file: %s\n", filepath);
    
    PackageParseResult result = package_parse_file(filepath);
    
    if (result.success) {
        printf("✅ Parse successful!\n");
        print_package(result.package);
        package_free(result.package);
        return 0;
    } else {
        printf("❌ Parse failed!\n");
        printf("Error: %s\n", result.error_message);
        if (result.error_line >= 0) {
            printf("Line: %d\n", result.error_line);
        }
        free(result.error_message);
        return 1;
    }
}
