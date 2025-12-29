#include "register_allocator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Available registers for allocation (caller-saved)
static const char* available_regs[] = {
    "%r8", "%r9", "%r10", "%r11", 
    "%r12", "%r13", "%r14", "%r15"
};

RegisterAllocator* register_allocator_create() {
    RegisterAllocator* alloc = malloc(sizeof(RegisterAllocator));
    for (int i = 0; i < 8; i++) {
        alloc->registers[i].in_use = 0;
        alloc->registers[i].var_name = NULL;
    }
    alloc->spill_count = 0;
    return alloc;
}

const char* register_allocator_alloc(RegisterAllocator* alloc, const char* var_name) {
    // Check if already allocated
    const char* existing = register_allocator_get(alloc, var_name);
    if (existing) return existing;
    
    // Find free register
    for (int i = 0; i < 8; i++) {
        if (!alloc->registers[i].in_use) {
            alloc->registers[i].in_use = 1;
            alloc->registers[i].var_name = strdup(var_name);
            return available_regs[i];
        }
    }
    
    // No free registers - need to spill to stack
    alloc->spill_count++;
    return NULL;
}

void register_allocator_free(RegisterAllocator* alloc, const char* var_name) {
    for (int i = 0; i < 8; i++) {
        if (alloc->registers[i].in_use && 
            strcmp(alloc->registers[i].var_name, var_name) == 0) {
            alloc->registers[i].in_use = 0;
            free(alloc->registers[i].var_name);
            alloc->registers[i].var_name = NULL;
            return;
        }
    }
}

const char* register_allocator_get(RegisterAllocator* alloc, const char* var_name) {
    for (int i = 0; i < 8; i++) {
        if (alloc->registers[i].in_use && 
            strcmp(alloc->registers[i].var_name, var_name) == 0) {
            return available_regs[i];
        }
    }
    return NULL;
}

void register_allocator_destroy(RegisterAllocator* alloc) {
    for (int i = 0; i < 8; i++) {
        if (alloc->registers[i].var_name) {
            free(alloc->registers[i].var_name);
        }
    }
    free(alloc);
}
