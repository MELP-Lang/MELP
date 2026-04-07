/**
 * std.fs Module - C++ Builtin Tests
 * Tests for file system operations
 * Stage 1 Week 8 - Day 1
 */

#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

using namespace melp_runtime;

// ANSI color codes
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void test_assert(bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << GREEN << "✓ " << test_name << RESET << std::endl;
        tests_passed++;
    } else {
        std::cout << RED << "✗ " << test_name << RESET << std::endl;
        tests_failed++;
    }
}

void test_file_existence_checks() {
    std::cout << "\n" << YELLOW << "=== Test Group 1: File Existence Checks ===" << RESET << std::endl;
    
    // Create test file
    const char* test_file = "/tmp/melp_test_file.txt";
    const char* test_dir = "/tmp/melp_test_dir";
    
    // Clean up any previous test artifacts
    std::remove(test_file);
    rmdir(test_dir);
    
    // Test 1: File doesn't exist initially
    test_assert(!fs_exists(test_file), "Non-existent file returns false");
    test_assert(!fs_is_file(test_file), "Non-existent file is not a file");
    test_assert(!fs_is_directory(test_file), "Non-existent file is not a directory");
    
    // Create test file
    FILE* f = fopen(test_file, "w");
    fprintf(f, "test content");
    fclose(f);
    
    // Test 2: File exists after creation
    test_assert(fs_exists(test_file), "Created file exists");
    test_assert(fs_is_file(test_file), "Created file is a file");
    test_assert(!fs_is_directory(test_file), "Created file is not a directory");
    
    // Test 3: File size
    int64_t size = fs_file_size(test_file);
    test_assert(size == 12, "File size is correct (12 bytes)");
    
    // Create test directory
    mkdir(test_dir, 0755);
    
    // Test 4: Directory checks
    test_assert(fs_exists(test_dir), "Created directory exists");
    test_assert(!fs_is_file(test_dir), "Directory is not a file");
    test_assert(fs_is_directory(test_dir), "Directory is a directory");
    
    // Clean up
    std::remove(test_file);
    rmdir(test_dir);
}

void test_file_read_write() {
    std::cout << "\n" << YELLOW << "=== Test Group 2: File Read/Write Operations ===" << RESET << std::endl;
    
    const char* test_file = "/tmp/melp_rw_test.txt";
    std::remove(test_file);
    
    // Test 1: Write string to file
    const char* content = "Hello, MLP!\n";
    int write_success = fs_write_string(test_file, content);
    test_assert(write_success == 1, "Write string succeeds");
    test_assert(fs_exists(test_file), "File exists after write");
    
    // Test 2: Read string from file
    int read_success;
    char* read_content = fs_read_to_string(test_file, &read_success);
    test_assert(read_success == 1, "Read string succeeds");
    test_assert(strcmp(read_content, content) == 0, "Read content matches written content");
    free(read_content);
    
    // Test 3: Append to file
    const char* append_content = "Second line\n";
    int append_success = fs_append_string(test_file, append_content);
    test_assert(append_success == 1, "Append string succeeds");
    
    // Test 4: Read appended content
    read_content = fs_read_to_string(test_file, &read_success);
    test_assert(read_success == 1, "Read appended file succeeds");
    std::string expected = std::string(content) + std::string(append_content);
    test_assert(strcmp(read_content, expected.c_str()) == 0, "Appended content is correct");
    free(read_content);
    
    // Test 5: Overwrite file
    const char* new_content = "Overwritten!";
    fs_write_string(test_file, new_content);
    read_content = fs_read_to_string(test_file, &read_success);
    test_assert(strcmp(read_content, new_content) == 0, "Overwrite replaces content");
    free(read_content);
    
    // Test 6: Read non-existent file
    std::remove(test_file);
    read_content = fs_read_to_string(test_file, &read_success);
    test_assert(read_success == 0, "Read non-existent file returns error");
    free(read_content);
    
    // Clean up
    std::remove(test_file);
}

void test_directory_operations() {
    std::cout << "\n" << YELLOW << "=== Test Group 3: Directory Operations ===" << RESET << std::endl;
    
    const char* test_dir = "/tmp/melp_dir_test";
    const char* nested_dir = "/tmp/melp_dir_test/nested/deep";
    
    // Clean up
    system("rm -rf /tmp/melp_dir_test");
    
    // Test 1: Create directory
    int create_success = fs_create_dir(test_dir);
    test_assert(create_success == 1, "Create directory succeeds");
    test_assert(fs_is_directory(test_dir), "Created directory exists");
    
    // Test 2: Create nested directories
    create_success = fs_create_dir_all(nested_dir);
    test_assert(create_success == 1, "Create nested directories succeeds");
    test_assert(fs_is_directory(nested_dir), "Nested directory exists");
    
    // Test 3: Remove empty directory
    int remove_success = fs_remove_dir(nested_dir);
    test_assert(remove_success == 1, "Remove empty directory succeeds");
    test_assert(!fs_exists(nested_dir), "Removed directory doesn't exist");
    
    // Test 4: Create files in directory
    const char* file1 = "/tmp/melp_dir_test/file1.txt";
    const char* file2 = "/tmp/melp_dir_test/file2.txt";
    fs_write_string(file1, "content1");
    fs_write_string(file2, "content2");
    
    // Test 5: Read directory
    int entry_count;
    char** entries = fs_read_dir(test_dir, &entry_count);
    test_assert(entry_count == 3, "Read directory returns correct count (2 files + 1 dir)");
    
    bool has_file1 = false, has_file2 = false, has_nested = false;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i], "file1.txt") == 0) has_file1 = true;
        if (strcmp(entries[i], "file2.txt") == 0) has_file2 = true;
        if (strcmp(entries[i], "nested") == 0) has_nested = true;
        free(entries[i]);
    }
    delete[] entries;
    
    test_assert(has_file1 && has_file2 && has_nested, "Read directory returns all entries");
    
    // Test 6: Remove directory recursively
    remove_success = fs_remove_dir_all(test_dir);
    test_assert(remove_success == 1, "Remove directory recursively succeeds");
    test_assert(!fs_exists(test_dir), "Recursively removed directory doesn't exist");
    
    // Clean up
    system("rm -rf /tmp/melp_dir_test");
}

void test_path_operations() {
    std::cout << "\n" << YELLOW << "=== Test Group 4: Path Operations ===" << RESET << std::endl;
    
    // Test 1: Join paths
    char* joined = fs_join_path("/home/user", "documents/file.txt");
    test_assert(strcmp(joined, "/home/user/documents/file.txt") == 0, "Join paths works");
    free(joined);
    
    // Test 2: Join with trailing slash
    joined = fs_join_path("/home/user/", "file.txt");
    test_assert(strcmp(joined, "/home/user/file.txt") == 0, "Join paths handles trailing slash");
    free(joined);
    
    // Test 3: Join with leading slash
    joined = fs_join_path("/home/user", "/file.txt");
    test_assert(strcmp(joined, "/home/user/file.txt") == 0, "Join paths handles leading slash");
    free(joined);
    
    // Test 4: Basename
    char* base = fs_basename("/home/user/documents/file.txt");
    test_assert(strcmp(base, "file.txt") == 0, "Basename extracts filename");
    free(base);
    
    // Test 5: Basename of simple path
    base = fs_basename("file.txt");
    test_assert(strcmp(base, "file.txt") == 0, "Basename of simple path");
    free(base);
    
    // Test 6: Dirname
    char* dir = fs_dirname("/home/user/documents/file.txt");
    test_assert(strcmp(dir, "/home/user/documents") == 0, "Dirname extracts directory");
    free(dir);
    
    // Test 7: Dirname of root
    dir = fs_dirname("/file.txt");
    test_assert(strcmp(dir, "/") == 0, "Dirname of root is /");
    free(dir);
    
    // Test 8: Dirname of simple path
    dir = fs_dirname("file.txt");
    test_assert(strcmp(dir, ".") == 0, "Dirname of simple path is .");
    free(dir);
    
    // Test 9: Extension
    char* ext = fs_extension("file.txt");
    test_assert(ext != nullptr && strcmp(ext, "txt") == 0, "Extension extraction works");
    free(ext);
    
    // Test 10: Extension with multiple dots
    ext = fs_extension("archive.tar.gz");
    test_assert(ext != nullptr && strcmp(ext, "gz") == 0, "Extension with multiple dots");
    free(ext);
    
    // Test 11: No extension
    ext = fs_extension("README");
    test_assert(ext == nullptr, "File without extension returns nullptr");
    
    // Test 12: Hidden file (starts with .)
    ext = fs_extension(".gitignore");
    test_assert(ext == nullptr, "Hidden file without extension returns nullptr");
}

void test_metadata() {
    std::cout << "\n" << YELLOW << "=== Test Group 5: File Metadata ===" << RESET << std::endl;
    
    const char* test_file = "/tmp/melp_meta_test.txt";
    const char* test_dir = "/tmp/melp_meta_dir";
    
    // Clean up
    std::remove(test_file);
    rmdir(test_dir);
    
    // Test 1: File metadata
    fs_write_string(test_file, "Hello World!");
    int success;
    FileMetadata meta = fs_metadata(test_file, &success);
    
    test_assert(success == 1, "Get file metadata succeeds");
    test_assert(meta.size == 12, "File metadata size is correct");
    test_assert(meta.is_directory == 0, "File metadata is_directory is false");
    test_assert(meta.modified_time > 0, "File metadata has valid modified time");
    
    // Test 2: Directory metadata
    mkdir(test_dir, 0755);
    meta = fs_metadata(test_dir, &success);
    
    test_assert(success == 1, "Get directory metadata succeeds");
    test_assert(meta.is_directory == 1, "Directory metadata is_directory is true");
    test_assert(meta.modified_time > 0, "Directory metadata has valid modified time");
    
    // Test 3: Non-existent file metadata
    std::remove(test_file);
    meta = fs_metadata(test_file, &success);
    test_assert(success == 0, "Get non-existent file metadata fails");
    
    // Clean up
    rmdir(test_dir);
}

int main() {
    std::cout << YELLOW << "============================================" << RESET << std::endl;
    std::cout << YELLOW << "  std.fs Module - C++ Builtin Tests" << RESET << std::endl;
    std::cout << YELLOW << "  Stage 1 Week 8 - Day 1" << RESET << std::endl;
    std::cout << YELLOW << "============================================" << RESET << std::endl;
    
    test_file_existence_checks();
    test_file_read_write();
    test_directory_operations();
    test_path_operations();
    test_metadata();
    
    std::cout << "\n" << YELLOW << "============================================" << RESET << std::endl;
    std::cout << GREEN << "Tests Passed: " << tests_passed << RESET << std::endl;
    std::cout << RED << "Tests Failed: " << tests_failed << RESET << std::endl;
    std::cout << YELLOW << "============================================" << RESET << std::endl;
    
    return tests_failed > 0 ? 1 : 0;
}
