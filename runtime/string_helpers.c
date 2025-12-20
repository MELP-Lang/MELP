// MELP Runtime - String Helper Functions
// Minimal implementations for Stage 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String concatenation
char* mlp_string_concat(char* a, char* b) {
    if (!a) a = "";
    if (!b) b = "";
    
    size_t len = strlen(a) + strlen(b) + 1;
    char* result = malloc(len);
    strcpy(result, a);
    strcat(result, b);
    return result;
}

// Number to string
char* mlp_number_to_string(long long num) {
    char* result = malloc(32);
    snprintf(result, 32, "%lld", num);
    return result;
}

// String comparison
long long mlp_string_compare(char* a, char* b) {
    if (!a || !b) return 0;
    return strcmp(a, b) == 0 ? 1 : 0;
}

// Print string
void mlp_println(char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("\n");
    }
}
