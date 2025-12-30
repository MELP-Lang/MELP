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

int64_t simple_compile(int64_t size) {
    int64_t result;
    result = size + 10;
    return result;
}

int64_t main(void) {
    int64_t output;
    output = simple_compile ( 42 );
    return output;
}

