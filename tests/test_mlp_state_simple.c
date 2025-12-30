/**
 * Simple Test: mlp_state Module
 * Basic validation of state management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../MELP/runtime/mlp_state.h"

int main(void) {
    printf("=== MLP State Simple Test ===\n\n");
    
    // Test 1: Basic set/get
    printf("Test 1: Basic set/get\n");
    mlp_state_init();
    mlp_state_set("name", "Alice");
    char* name = mlp_state_get("name");
    assert(name != NULL && strcmp(name, "Alice") == 0);
    printf("  ✅ Basic operations working\n");
    free(name);
    mlp_state_close();
    
    // Test 2: Nonexistent key returns empty string
    printf("Test 2: Nonexistent key\n");
    mlp_state_init();
    char* result = mlp_state_get("nonexistent");
    assert(result != NULL && strlen(result) == 0);
    printf("  ✅ Nonexistent key returns empty string\n");
    free(result);
    mlp_state_close();
    
    // Test 3: Delete
    printf("Test 3: Delete key\n");
    mlp_state_init();
    mlp_state_set("temp", "value");
    mlp_state_delete("temp");
    char* deleted = mlp_state_get("temp");
    assert(deleted != NULL && strlen(deleted) == 0);
    printf("  ✅ Delete working\n");
    free(deleted);
    mlp_state_close();
    
    printf("\n=== All Tests Passed ✅ ===\n");
    return 0;
}
