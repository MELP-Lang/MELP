#include "package_builder.h"
#include "package_parser.h"
#include "dependency_resolver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

// ============================================================================
// BUILD CONFIGURATION
// ============================================================================

/**
 * Create default build configuration
 */
BuildConfig* build_config_create(void) {
    BuildConfig* config = (BuildConfig*)calloc(1, sizeof(BuildConfig));
    if (!config) return NULL;
    
    config->build_dir = strdup("build/");
    config->dist_dir = strdup("dist/");
    config->obj_dir = strdup("build/obj/");
    config->bin_dir = strdup("build/bin/");
    config->clean_build = false;
    config->verbose = false;
    config->debug_mode = false;
    
    return config;
}

/**
 * Free build configuration
 */
void build_config_free(BuildConfig* config) {
    if (!config) return;
    
    free(config->build_dir);
    free(config->dist_dir);
    free(config->obj_dir);
    free(config->bin_dir);
    free(config);
}

/**
 * Load build configuration from package
 */
BuildConfig* build_config_from_package(Package* package) {
    BuildConfig* config = build_config_create();
    if (!config) return NULL;
    
    if (package->build_dir) {
        free(config->build_dir);
        config->build_dir = strdup(package->build_dir);
    }
    
    if (package->dist_dir) {
        free(config->dist_dir);
        config->dist_dir = strdup(package->dist_dir);
    }
    
    return config;
}

/**
 * Print build configuration
 */
void build_config_print(BuildConfig* config) {
    if (!config) return;
    
    printf("=== Build Configuration ===\n");
    printf("Build dir: %s\n", config->build_dir);
    printf("Dist dir: %s\n", config->dist_dir);
    printf("Obj dir: %s\n", config->obj_dir);
    printf("Bin dir: %s\n", config->bin_dir);
    printf("Clean build: %s\n", config->clean_build ? "yes" : "no");
    printf("Verbose: %s\n", config->verbose ? "yes" : "no");
    printf("Debug mode: %s\n", config->debug_mode ? "yes" : "no");
    printf("===========================\n");
}

// ============================================================================
// DIRECTORY MANAGEMENT
// ============================================================================

/**
 * Check if directory exists
 */
bool build_directory_exists(const char* path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

/**
 * Create directory (mkdir -p equivalent)
 */
bool build_create_directory(const char* path) {
    char tmp[512];
    char* p = NULL;
    size_t len;
    
    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }
    
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
                return false;
            }
            *p = '/';
        }
    }
    
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
        return false;
    }
    
    return true;
}

/**
 * Create build directory structure
 */
bool build_create_directories(BuildConfig* config) {
    if (!config) return false;
    
    bool success = true;
    
    success &= build_create_directory(config->build_dir);
    success &= build_create_directory(config->dist_dir);
    success &= build_create_directory(config->obj_dir);
    success &= build_create_directory(config->bin_dir);
    
    return success;
}

/**
 * Clean build directories
 */
bool build_clean_directories(BuildConfig* config) {
    if (!config) return false;
    
    // Simple implementation: just create fresh directories
    // In production, would use system("rm -rf build/") or similar
    
    printf("Cleaning build directories...\n");
    return build_create_directories(config);
}

// ============================================================================
// BUILD RESULT
// ============================================================================

/**
 * Create success result
 */
BuildResult build_result_success(const char* output_path, int files_compiled, double build_time) {
    BuildResult result;
    result.success = true;
    result.error_message = NULL;
    result.output_path = output_path ? strdup(output_path) : NULL;
    result.files_compiled = files_compiled;
    result.build_time = build_time;
    return result;
}

/**
 * Create error result
 */
BuildResult build_result_error(const char* error_message) {
    BuildResult result;
    result.success = false;
    result.error_message = strdup(error_message);
    result.output_path = NULL;
    result.files_compiled = 0;
    result.build_time = 0.0;
    return result;
}

/**
 * Free build result
 */
void build_result_free(BuildResult* result) {
    if (!result) return;
    
    free(result->error_message);
    free(result->output_path);
}

/**
 * Print build result
 */
void build_result_print(BuildResult* result) {
    if (!result) return;
    
    printf("\n=== Build Result ===\n");
    
    if (result->success) {
        printf("Status: ✅ SUCCESS\n");
        if (result->output_path) {
            printf("Output: %s\n", result->output_path);
        }
        printf("Files compiled: %d\n", result->files_compiled);
        printf("Build time: %.2f seconds\n", result->build_time);
    } else {
        printf("Status: ❌ FAILED\n");
        if (result->error_message) {
            printf("Error: %s\n", result->error_message);
        }
    }
    
    printf("====================\n\n");
}

// ============================================================================
// UTILITIES
// ============================================================================

/**
 * Get output path for package
 */
char* build_get_output_path(Package* package, BuildConfig* config) {
    if (!package || !config) return NULL;
    
    char* path = (char*)malloc(512);
    if (!path) return NULL;
    
    snprintf(path, 512, "%s%s", config->bin_dir, package->name);
    return path;
}

/**
 * Get object file path for source file
 */
char* build_get_object_path(const char* source_file, BuildConfig* config) {
    if (!source_file || !config) return NULL;
    
    // Extract filename from path
    const char* filename = strrchr(source_file, '/');
    if (!filename) {
        filename = source_file;
    } else {
        filename++;  // Skip '/'
    }
    
    // Replace .mlp with .o
    char* path = (char*)malloc(512);
    if (!path) return NULL;
    
    char base_name[256];
    strncpy(base_name, filename, sizeof(base_name));
    
    // Remove extension
    char* dot = strrchr(base_name, '.');
    if (dot) *dot = '\0';
    
    snprintf(path, 512, "%s%s.o", config->obj_dir, base_name);
    return path;
}

/**
 * Check if rebuild is needed
 */
bool build_needs_rebuild(const char* source_file, const char* object_file) {
    struct stat source_stat, object_stat;
    
    // If object doesn't exist, rebuild needed
    if (stat(object_file, &object_stat) != 0) {
        return true;
    }
    
    // If source doesn't exist, something is wrong
    if (stat(source_file, &source_stat) != 0) {
        return true;
    }
    
    // Rebuild if source is newer than object
    return source_stat.st_mtime > object_stat.st_mtime;
}

// ============================================================================
// COMPILATION
// ============================================================================

/**
 * Compile a single source file (stub implementation)
 */
bool build_compile_file(const char* source_file, const char* output_file, BuildConfig* config) {
    if (!source_file || !output_file || !config) return false;
    
    if (config->verbose) {
        printf("Compiling: %s -> %s\n", source_file, output_file);
    }
    
    // Stub implementation: In real system, would invoke compiler here
    // For now, just create an empty output file to simulate compilation
    FILE* f = fopen(output_file, "w");
    if (!f) {
        if (config->verbose) {
            printf("Failed to create output file: %s\n", output_file);
        }
        return false;
    }
    fprintf(f, "# Object file for %s\n", source_file);
    fclose(f);
    
    return true;
}

// ============================================================================
// PACKAGE BUILDING
// ============================================================================

/**
 * Build a package
 */
BuildResult build_package(Package* package, BuildConfig* config) {
    if (!package || !config) {
        return build_result_error("NULL package or config");
    }
    
    clock_t start_time = clock();
    
    // Create build directories
    if (!build_create_directories(config)) {
        return build_result_error("Failed to create build directories");
    }
    
    // Get entry point
    const char* entry = package->entry ? package->entry : "main.mlp";
    
    if (config->verbose) {
        printf("\n=== Building Package: %s ===\n", package->name);
        printf("Entry point: %s\n", entry);
    }
    
    // Compile entry file
    char* obj_path = build_get_object_path(entry, config);
    if (!obj_path) {
        return build_result_error("Failed to get object path");
    }
    
    bool compile_success = build_compile_file(entry, obj_path, config);
    free(obj_path);
    
    if (!compile_success) {
        return build_result_error("Compilation failed");
    }
    
    // Get output path
    char* output_path = build_get_output_path(package, config);
    if (!output_path) {
        return build_result_error("Failed to get output path");
    }
    
    // Link (stub: just touch the output file)
    FILE* f = fopen(output_path, "w");
    if (!f) {
        free(output_path);
        return build_result_error("Failed to create output binary");
    }
    fprintf(f, "# Binary for %s\n", package->name);
    fclose(f);
    
    double build_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    
    BuildResult result = build_result_success(output_path, 1, build_time);
    free(output_path);
    
    return result;
}

/**
 * Build with dependency resolution
 */
BuildResult build_package_with_deps(const char* package_file, BuildConfig* config) {
    if (!package_file || !config) {
        return build_result_error("NULL package file or config");
    }
    
    // Parse package
    PackageParseResult parse_result = package_parse_file(package_file);
    if (!parse_result.success) {
        BuildResult result = build_result_error(parse_result.error_message);
        free(parse_result.error_message);
        return result;
    }
    
    // Resolve dependencies (with NULL loader for now)
    DependencyResolutionResult dep_result = dependency_resolve(parse_result.package, NULL);
    if (!dep_result.success) {
        BuildResult result = build_result_error(dep_result.error_message);
        dependency_result_free(&dep_result);
        package_free(parse_result.package);
        return result;
    }
    
    if (config->verbose) {
        printf("\n=== Build Order ===\n");
        for (int i = 0; i < dep_result.package_count; i++) {
            printf("%d. %s (v%s)\n", 
                   i + 1,
                   dep_result.load_order[i]->name,
                   dep_result.load_order[i]->version_str);
        }
        printf("===================\n");
    }
    
    // Build packages in dependency order
    int total_files = 0;
    clock_t start_time = clock();
    
    for (int i = 0; i < dep_result.package_count; i++) {
        Package* pkg = dep_result.load_order[i];
        
        // Skip placeholder packages
        if (!pkg->is_valid) {
            if (config->verbose) {
                printf("Skipping placeholder: %s\n", pkg->name);
            }
            continue;
        }
        
        BuildResult pkg_result = build_package(pkg, config);
        
        if (!pkg_result.success) {
            dependency_result_free(&dep_result);
            return pkg_result;
        }
        
        total_files += pkg_result.files_compiled;
        build_result_free(&pkg_result);
    }
    
    double build_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    
    // Get final output path
    char* output_path = build_get_output_path(parse_result.package, config);
    
    BuildResult result = build_result_success(output_path, total_files, build_time);
    free(output_path);
    
    dependency_result_free(&dep_result);
    
    return result;
}
