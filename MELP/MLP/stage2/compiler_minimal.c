// MLP-GCC Generated C Code
// Target: GCC C99
// STO: Smart Type Optimization enabled

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// STO Runtime types
typedef struct { /* bigdecimal */ } mlp_bigdecimal_t;
typedef struct { char* data; size_t len; } mlp_string_t;

int64_t tokenize_helper(int64_t source_length, int64_t position, int64_t count) {
    if (position >= source_length) {
        return count;
    }
    int64_t next_pos;
    int64_t next_count;
    next_pos = position + 1;
    next_count = count + 1;
    return tokenize_helper (source_length, next_pos, next_count);
}

int64_t tokenize(int64_t source_length) {
    return tokenize_helper (source_length, 0, 0);
}

int64_t parse_helper(int64_t total, int64_t current) {
    if (current >= total) {
        return current;
    }
    int64_t next;
    next = current + 1;
    return parse_helper (total, next);
}

int64_t parse_tokens(int64_t token_count) {
    return parse_helper (token_count, 0);
}

int64_t codegen_helper(int64_t total, int64_t current) {
    if (current >= total) {
        return current;
    }
    int64_t next;
    next = current + 1;
    return codegen_helper (total, next);
}

int64_t generate_c_code(int64_t node_count) {
    return codegen_helper (node_count, 0);
}

int64_t main(void) {
    int64_t tokens;
    int64_t ast_nodes;
    int64_t c_lines;
    int64_t source_size;
    source_size = 100;
    tokens = tokenize ( source_size );
    ast_nodes = parse_tokens ( tokens );
    c_lines = generate_c_code ( ast_nodes );
    return c_lines;
}

