#include <stdio.h>
#include <stdint.h>
#include "../../runtime/sto/sto_runtime.h"

int main() {
    printf("Allocating tuple with 2 elements\n");
    STOTuple* t = sto_tuple_alloc(2);
    if (!t) {
        printf("ERROR: sto_tuple_alloc returned NULL\n");
        return 1;
    }
    printf("Tuple allocated: %p\n", (void*)t);
    
    int64_t val1 = 42;
    printf("Setting element 0 to %ld\n", val1);
    sto_tuple_set(t, 0, &val1, 0);
    printf("Element 0 set\n");
    
    int64_t val2 = 99;
    printf("Setting element 1 to %ld\n", val2);
    sto_tuple_set(t, 1, &val2, 0);
    printf("Element 1 set\n");
    
    printf("Success!\n");
    sto_tuple_free(t);
    return 42;
}
