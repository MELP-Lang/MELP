#include <stdio.h>
#include <stdlib.h>

void mlp_panic_array_bounds(long index, long length, const char* array_name) {
    fprintf(stderr, "\n");
    fprintf(stderr, "==========================================================\n");
    fprintf(stderr, "RUNTIME ERROR: Array Index Out of Bounds\n");
    fprintf(stderr, "==========================================================\n");
    fprintf(stderr, "Array: %s\n", array_name ? array_name : "(unknown)");
    fprintf(stderr, "Index: %ld\n", index);
    fprintf(stderr, "Valid range: 0 to %ld\n", length - 1);
    fprintf(stderr, "==========================================================\n");
    fprintf(stderr, "\n");
    exit(42);  // Exit code 42 = bounds check failure
}
