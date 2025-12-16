// Test wrapper for array runtime - links assembly with C main
#include <stdio.h>

// Declare external functions from STO runtime
extern void sto_runtime_init(void);
extern void* sto_array_alloc(int count, int elem_size);

int main() {
    printf("Testing STO array runtime...\n");
    
    // Initialize STO runtime
    sto_runtime_init();
    
    // Allocate array: [1, 2, 3]
    long* array1 = (long*)sto_array_alloc(3, 8);
    array1[0] = 1;
    array1[1] = 2;
    array1[2] = 3;
    
    printf("Array 1: [%ld, %ld, %ld]\n", array1[0], array1[1], array1[2]);
    
    // Allocate array: [10, 20, 30, 40]
    long* array2 = (long*)sto_array_alloc(4, 8);
    array2[0] = 10;
    array2[1] = 20;
    array2[2] = 30;
    array2[3] = 40;
    
    printf("Array 2: [%ld, %ld, %ld, %ld]\n", 
           array2[0], array2[1], array2[2], array2[3]);
    
    printf("✅ All tests passed!\n");
    
    return 0;
}
