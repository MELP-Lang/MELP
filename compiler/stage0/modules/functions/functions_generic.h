#ifndef FUNCTIONS_GENERIC_H
#define FUNCTIONS_GENERIC_H

#include "functions.h"

// YZ_203: Generic Type System

// Generic function instantiation (specialized version)
typedef struct GenericInstance {
    char* mangled_name;                    // "max_numeric", "identity_string"
    FunctionDeclaration* specialized_func; // Concrete function with substituted types
    char** concrete_types;                 // ["numeric"], ["string"], etc.
    int type_count;                        // Number of concrete types
    int is_emitted;                        // 1 if LLVM IR has been emitted
    struct GenericInstance* next;          // Linked list
} GenericInstance;

// Generic function template (not yet instantiated)
typedef struct GenericTemplate {
    char* name;                            // "max", "identity"
    FunctionDeclaration* template_func;    // Template with type parameters (T, U, etc.)
    GenericInstance* instances;            // List of instantiated versions
    struct GenericTemplate* next;          // Linked list
} GenericTemplate;

// Global template registry
typedef struct {
    GenericTemplate* templates;            // List of all generic templates
    int template_count;
} GenericRegistry;

// Registry management
GenericRegistry* generic_registry_create();
void generic_registry_destroy(GenericRegistry* registry);

// Template registration
void generic_register_template(GenericRegistry* registry, FunctionDeclaration* func);
GenericTemplate* generic_find_template(GenericRegistry* registry, const char* name);

// Instantiation (monomorphization)
GenericInstance* generic_instantiate(GenericRegistry* registry, 
                                      const char* template_name,
                                      char** concrete_types,
                                      int type_count);

// Check if an instance already exists
GenericInstance* generic_find_instance(GenericTemplate* template,
                                        char** concrete_types,
                                        int type_count);

// Helper: Clone function declaration and substitute types
FunctionDeclaration* generic_specialize_function(FunctionDeclaration* template_func,
                                                  char** concrete_types,
                                                  int type_count);

#endif
