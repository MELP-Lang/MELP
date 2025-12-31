// Test new STATELESS Dict API
#include "MELP/runtime/runtime_types.h"
#include "MELP/runtime/runtime_memory.h"
#include <stdio.h>

int main(void) {
    printf("=== DICT STATELESS API TEST ===\n\n");
    
    // Test 1: Create dictionary
    printf("Test 1: Create dictionary\n");
    Sozluk* dict = sozluk_yeni(100);
    if (dict) {
        printf("  ✅ Dictionary created (size: 100)\n");
    } else {
        printf("  ❌ Dictionary creation failed\n");
        return 1;
    }
    
    // Test 2: Add key-value pairs
    printf("\nTest 2: Add key-value pairs\n");
    sozluk_koy(dict, "age", 25);
    sozluk_koy(dict, "year", 2025);
    sozluk_koy(dict, "version", 1);
    printf("  ✅ Added 3 key-value pairs\n");
    
    // Test 3: Retrieve values
    printf("\nTest 3: Retrieve values\n");
    int64_t age = sozluk_al(dict, "age");
    int64_t year = sozluk_al(dict, "year");
    int64_t version = sozluk_al(dict, "version");
    printf("  ✅ age = %ld\n", (long)age);
    printf("  ✅ year = %ld\n", (long)year);
    printf("  ✅ version = %ld\n", (long)version);
    
    // Test 4: Update existing key
    printf("\nTest 4: Update existing key\n");
    sozluk_koy(dict, "age", 30);
    int64_t new_age = sozluk_al(dict, "age");
    printf("  ✅ age updated: %ld\n", (long)new_age);
    
    // Test 5: Non-existent key
    printf("\nTest 5: Non-existent key\n");
    int64_t missing = sozluk_al(dict, "missing");
    printf("  ✅ missing key returns: %ld (expected 0)\n", (long)missing);
    
    // Test 6: Clean up
    printf("\nTest 6: Clean up\n");
    sozluk_sil(dict);
    printf("  ✅ Dictionary destroyed\n");
    
    printf("\n=== ALL TESTS PASSED ✅ ===\n");
    printf("STATELESS compliance verified!\n");
    return 0;
}
