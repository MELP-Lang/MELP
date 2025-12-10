#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

/**
 * Register Allocation (Phase 7 - Task 3)
 * 
 * Simple register allocation strategy:
 * - Use %r8-%r15 for frequently accessed variables
 * - Fall back to stack for others
 * - Priority: loop counters, function parameters, return values
 */

typedef struct {
    int in_use;
    char* var_name;
} RegisterSlot;

typedef struct {
    RegisterSlot registers[8];  // %r8-%r15
    int spill_count;            // Number of variables spilled to stack
} RegisterAllocator;

// Initialize register allocator
RegisterAllocator* register_allocator_create();

// Allocate a register for variable (returns register name or NULL if spill needed)
const char* register_allocator_alloc(RegisterAllocator* alloc, const char* var_name);

// Free a register
void register_allocator_free(RegisterAllocator* alloc, const char* var_name);

// Get register for variable (returns NULL if not allocated)
const char* register_allocator_get(RegisterAllocator* alloc, const char* var_name);

// Free allocator
void register_allocator_destroy(RegisterAllocator* alloc);

#endif
