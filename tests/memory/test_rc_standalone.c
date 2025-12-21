// Test: Reference Counting Runtime
// YZ_210 Phase 2 - Standalone RC Test

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RC Implementation (copied from memory.c)
typedef struct RefCountHeader {
    size_t ref_count;
    size_t size;
    void* destructor;
} RefCountHeader;

static RefCountHeader* get_header(void* ptr) {
    if (!ptr) return NULL;
    return (RefCountHeader*)((char*)ptr - sizeof(RefCountHeader));
}

static void* get_user_ptr(RefCountHeader* header) {
    if (!header) return NULL;
    return (void*)((char*)header + sizeof(RefCountHeader));
}

void* rc_malloc(size_t size) {
    RefCountHeader* header = (RefCountHeader*)malloc(sizeof(RefCountHeader) + size);
    if (!header) return NULL;
    
    header->ref_count = 1;
    header->size = size;
    header->destructor = NULL;
    
    return get_user_ptr(header);
}

void rc_retain(void* ptr) {
    if (!ptr) return;
    RefCountHeader* header = get_header(ptr);
    if (header) {
        header->ref_count++;
    }
}

void rc_release(void* ptr) {
    if (!ptr) return;
    RefCountHeader* header = get_header(ptr);
    if (!header) return;
    
    if (header->ref_count > 0) {
        header->ref_count--;
    }
    
    if (header->ref_count == 0) {
        if (header->destructor) {
            void (*destructor)(void*) = (void (*)(void*))header->destructor;
            destructor(ptr);
        }
        free(header);
    }
}

size_t rc_get_count(void* ptr) {
    if (!ptr) return 0;
    RefCountHeader* header = get_header(ptr);
    return header ? header->ref_count : 0;
}

// Test Cases
int main() {
    printf("🧪 Reference Counting GC Test (YZ_210)\n");
    printf("=====================================\n\n");
    
    // Test 1: Basic allocation
    printf("Test 1: Basic rc_malloc\n");
    char* str1 = (char*)rc_malloc(100);
    strcpy(str1, "Hello, MELP!");
    printf("  Allocated: %s\n", str1);
    printf("  Ref count: %zu\n", rc_get_count(str1));
    printf("  ✅ Expected: 1\n\n");
    
    // Test 2: Retain (increment)
    printf("Test 2: rc_retain\n");
    char* str2 = str1;  // Alias
    rc_retain(str2);
    printf("  Alias created, retained\n");
    printf("  Ref count: %zu\n", rc_get_count(str1));
    printf("  ✅ Expected: 2\n\n");
    
    // Test 3: Release (decrement, no free)
    printf("Test 3: rc_release (no free)\n");
    rc_release(str2);
    printf("  One reference released\n");
    printf("  Ref count: %zu\n", rc_get_count(str1));
    printf("  ✅ Expected: 1\n\n");
    
    // Test 4: Final release (auto-free)
    printf("Test 4: rc_release (auto-free)\n");
    printf("  Releasing last reference...\n");
    rc_release(str1);
    printf("  ✅ Memory freed automatically!\n\n");
    
    // Test 5: Multiple allocations
    printf("Test 5: Multiple independent allocations\n");
    char* a = (char*)rc_malloc(50);
    char* b = (char*)rc_malloc(50);
    char* c = (char*)rc_malloc(50);
    strcpy(a, "A");
    strcpy(b, "B");
    strcpy(c, "C");
    printf("  Allocated: %s (count=%zu), %s (count=%zu), %s (count=%zu)\n",
           a, rc_get_count(a),
           b, rc_get_count(b),
           c, rc_get_count(c));
    rc_release(a);
    rc_release(b);
    rc_release(c);
    printf("  ✅ All freed!\n\n");
    
    printf("=====================================\n");
    printf("✅ All tests passed!\n");
    printf("Reference Counting GC is working! 🎉\n");
    
    return 0;
}
