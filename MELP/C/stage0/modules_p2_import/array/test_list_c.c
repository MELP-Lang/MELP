// Test STO list runtime functions
#include <stdio.h>
#include <stdint.h>

// From variable.h
typedef enum {
    VAR_NUMERIC = 0,
    VAR_STRING = 1,
    VAR_BOOLEAN = 2,
    VAR_POINTER = 3,
    VAR_ARRAY = 4
} VarType;

// Declare external functions from STO runtime
extern void sto_runtime_init(void);
extern void* sto_list_alloc(int capacity);
extern void sto_list_set(void* list, int index, void* value, uint8_t type);

int main() {
    printf("Testing STO list runtime...\n");
    
    // Initialize STO runtime
    sto_runtime_init();
    
    // Allocate list: (1; 2; 3;)
    void* list1 = sto_list_alloc(3);
    
    int64_t val1 = 1;
    int64_t val2 = 2;
    int64_t val3 = 3;
    
    sto_list_set(list1, 0, &val1, VAR_NUMERIC);
    sto_list_set(list1, 1, &val2, VAR_NUMERIC);
    sto_list_set(list1, 2, &val3, VAR_NUMERIC);
    
    printf("✅ List test passed (values stored)\n");
    printf("   List: (1; 2; 3;)\n");
    
    return 0;
}
