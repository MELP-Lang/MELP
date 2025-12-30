#include "runtime_print.h"
#include "runtime_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yazdir_sayi(int64_t sayi) {
    printf("%ld\n", sayi);
}

void yazdir_metin(const char* metin) {
    printf("%s\n", metin);
}

char* read_input(void) {
    char* buffer = (char*)mlp_malloc(1024);
    if (!buffer) return NULL;
    if (fgets(buffer, 1024, stdin) == NULL) {
        mlp_free(buffer);
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    return buffer;
}

char* read_line(void) {
    return read_input();
}

int64_t read_int(void) {
    char* line = read_input();
    if (!line) return 0;
    int64_t value = atoll(line);
    mlp_free(line);
    return value;
}
