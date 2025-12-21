#ifndef PACKAGE_PARSER_H
#define PACKAGE_PARSER_H

#include "package_types.h"
#include "../lexer/lexer.h"

/**
 * Parse a package manifest file (package.mlp)
 * 
 * @param filepath Path to package.mlp file
 * @return PackageParseResult containing parsed package or error
 */
PackageParseResult package_parse_file(const char* filepath);

/**
 * Parse a package manifest from a token stream
 * 
 * @param tokens Array of tokens from lexer
 * @param token_count Number of tokens
 * @return PackageParseResult containing parsed package or error
 */
PackageParseResult package_parse_tokens(Token* tokens, int token_count);

/**
 * Parse package block: package { ... }
 * 
 * @param tokens Token array
 * @param token_count Total token count
 * @param current Current token index (will be updated)
 * @return Parsed Package or NULL on error
 */
Package* package_parse_block(Token* tokens, int token_count, int* current);

/**
 * Parse a single field inside package block
 * Example: name = "my_project"
 * 
 * @param tokens Token array
 * @param token_count Total token count
 * @param current Current token index (will be updated)
 * @param package Package structure to populate
 * @return true on success, false on error
 */
bool package_parse_field(Token* tokens, int token_count, int* current, Package* package);

/**
 * Parse dependencies array: dependencies = ["stdlib:1.0", "math:2.0"]
 * 
 * @param tokens Token array
 * @param token_count Total token count
 * @param current Current token index (will be updated)
 * @return Array of dependencies or NULL on error
 */
Dependency** package_parse_dependencies(Token* tokens, int token_count, int* current, int* dep_count);

/**
 * Parse semantic version string "1.2.3" into SemanticVersion struct
 * 
 * @param version_str Version string (e.g., "1.0.0")
 * @param out_version Output SemanticVersion structure
 * @return true on success, false on invalid format
 */
bool package_parse_version(const char* version_str, SemanticVersion* out_version);

/**
 * Validate package manifest (check required fields, etc.)
 * 
 * @param package Package to validate
 * @return true if valid, false otherwise
 */
bool package_validate(Package* package);

/**
 * Create a new empty Package structure
 * 
 * @return Newly allocated Package (caller must free)
 */
Package* package_create(void);

/**
 * Free a Package structure and all its contents
 * 
 * @param package Package to free
 */
void package_free(Package* package);

/**
 * Free a Dependency structure
 * 
 * @param dep Dependency to free
 */
void dependency_free(Dependency* dep);

/**
 * Create error result
 * 
 * @param error_msg Error message
 * @param line Line number where error occurred
 * @return PackageParseResult with error
 */
PackageParseResult package_error(const char* error_msg, int line);

/**
 * Create success result
 * 
 * @param package Successfully parsed package
 * @return PackageParseResult with package
 */
PackageParseResult package_success(Package* package);

#endif // PACKAGE_PARSER_H
