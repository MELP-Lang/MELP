#include <stdio.h>
#include <stdlib.h>
#include "normalize/normalize.h"

int main() {
    const char* input = "function main() returns numeric\n    numeric x = 5;\n    if x > 0 then\n        print(5)\n    end if\n    return 0\nend function\n";
    
    char* output = normalize_to_pmpl(input);
    printf("INPUT:\n%s\n", input);
    printf("OUTPUT:\n%s\n", output);
    
    free(output);
    return 0;
}
