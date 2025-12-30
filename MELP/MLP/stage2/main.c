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

int64_t compile_mlp_file(int64_t filename) {
    int64_t tokens;
    int64_t token_count;
    int64_t ast_nodes;
    int64_t node_count;
    token_count = lex_source ( filename );
    if (token_count == 0) {
        return 1;
    }
    node_count = parse_tokens ( tokens ; token_count );
    if (node_count == 0) {
        return 2;
    }
    return 0;
}

int64_t main(void) {
    int64_t result;
    int64_t test_mode;
    test_mode = 1;
    if (test_mode == 1) {
        result = compile_mlp_file ( 1 );
        return result;
    }
    return 0;
}

