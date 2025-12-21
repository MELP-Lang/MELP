// YZ_213: Stub for melp_runtime_error (temporary)
#include <stdio.h>
#include <stdlib.h>

void melp_runtime_error(const char* message) {
    fprintf(stderr, "Runtime Error: %s\n", message);
    exit(1);
}
