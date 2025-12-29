#include "functions_generic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create generic registry
GenericRegistry* generic_registry_create() {
    GenericRegistry* registry = malloc(sizeof(GenericRegistry));
    registry->templates = NULL;
    registry->template_count = 0;
    return registry;
}

// Destroy generic registry
void generic_registry_destroy(GenericRegistry* registry) {
    if (!registry) return;
    
    GenericTemplate* template = registry->templates;
    while (template) {
        GenericTemplate* next = template->next;
        
        // Free instances
        GenericInstance* instance = template->instances;
        while (instance) {
            GenericInstance* next_inst = instance->next;
            free(instance->mangled_name);
            if (instance->specialized_func) {
                function_free(instance->specialized_func);
            }
            for (int i = 0; i < instance->type_count; i++) {
                free(instance->concrete_types[i]);
            }
            free(instance->concrete_types);
            free(instance);
            instance = next_inst;
        }
        
        // Free template
        free(template->name);
        if (template->template_func) {
            function_free(template->template_func);
        }
        free(template);
        template = next;
    }
    
    free(registry);
}

// Register a generic template
void generic_register_template(GenericRegistry* registry, FunctionDeclaration* func) {
    if (!registry || !func || !func->is_generic_template) return;
    
    GenericTemplate* template = malloc(sizeof(GenericTemplate));
    template->name = strdup(func->name);
    template->template_func = func;  // Take ownership
    template->instances = NULL;
    template->next = registry->templates;
    
    registry->templates = template;
    registry->template_count++;
    
    printf("📋 Registered generic template: %s<%d type params>\n", 
           func->name, func->type_param_count);
}

// Find a template by name
GenericTemplate* generic_find_template(GenericRegistry* registry, const char* name) {
    if (!registry || !name) return NULL;
    
    GenericTemplate* template = registry->templates;
    while (template) {
        if (strcmp(template->name, name) == 0) {
            return template;
        }
        template = template->next;
    }
    return NULL;
}

// Check if an instance with these types already exists
GenericInstance* generic_find_instance(GenericTemplate* template,
                                        char** concrete_types,
                                        int type_count) {
    if (!template || !concrete_types) return NULL;
    
    GenericInstance* instance = template->instances;
    while (instance) {
        // Check if type counts match
        if (instance->type_count == type_count) {
            // Check if all types match
            int match = 1;
            for (int i = 0; i < type_count; i++) {
                if (strcmp(instance->concrete_types[i], concrete_types[i]) != 0) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                return instance;
            }
        }
        instance = instance->next;
    }
    return NULL;
}

// Instantiate a generic template (monomorphization)
GenericInstance* generic_instantiate(GenericRegistry* registry,
                                      const char* template_name,
                                      char** concrete_types,
                                      int type_count) {
    if (!registry || !template_name || !concrete_types) return NULL;
    
    // Find template
    GenericTemplate* template = generic_find_template(registry, template_name);
    if (!template) {
        fprintf(stderr, "Error: Generic template '%s' not found\n", template_name);
        return NULL;
    }
    
    // Check if instance already exists
    GenericInstance* existing = generic_find_instance(template, concrete_types, type_count);
    if (existing) {
        printf("♻️  Reusing instance: %s\n", existing->mangled_name);
        return existing;
    }
    
    // Specialize the function
    FunctionDeclaration* specialized = generic_specialize_function(
        template->template_func, concrete_types, type_count);
    
    if (!specialized) {
        return NULL;
    }
    
    // Create instance
    GenericInstance* instance = malloc(sizeof(GenericInstance));
    instance->mangled_name = function_mangle_name(template_name, concrete_types, type_count);
    instance->specialized_func = specialized;
    instance->concrete_types = malloc(sizeof(char*) * type_count);
    for (int i = 0; i < type_count; i++) {
        instance->concrete_types[i] = strdup(concrete_types[i]);
    }
    instance->type_count = type_count;
    instance->is_emitted = 0;
    instance->next = template->instances;
    
    template->instances = instance;
    
    printf("⚙️  Instantiated: %s → %s\n", template_name, instance->mangled_name);
    
    return instance;
}

// Specialize a function by substituting type parameters
FunctionDeclaration* generic_specialize_function(FunctionDeclaration* template_func,
                                                  char** concrete_types,
                                                  int type_count) {
    if (!template_func || !concrete_types) return NULL;
    
    // Validate type count
    if (type_count != template_func->type_param_count) {
        fprintf(stderr, "Error: Type count mismatch (expected %d, got %d)\n",
                template_func->type_param_count, type_count);
        return NULL;
    }
    
    // Create mangled name
    char* mangled_name = function_mangle_name(template_func->name, concrete_types, type_count);
    
    // Clone function with new name
    FunctionDeclaration* specialized = function_create(mangled_name, template_func->return_type);
    free(mangled_name);
    
    // Copy return type (with substitution if generic)
    if (template_func->return_struct_type) {
        // Check if return type is a type parameter
        int type_param_index = -1;
        for (int i = 0; i < template_func->type_param_count; i++) {
            if (strcmp(template_func->return_struct_type, template_func->type_params[i]) == 0) {
                type_param_index = i;
                break;
            }
        }
        
        if (type_param_index >= 0) {
            // Substitute with concrete type
            specialized->return_struct_type = strdup(concrete_types[type_param_index]);
        } else {
            // Not a type parameter, copy as-is
            specialized->return_struct_type = strdup(template_func->return_struct_type);
        }
    }
    
    // Clone parameters with type substitution
    FunctionParam* param = template_func->params;
    while (param) {
        FunctionParamType param_type = param->type;
        char* param_struct_type = NULL;
        
        // If parameter has struct type, check if it's a type parameter
        if (param->struct_type_name) {
            int type_param_index = -1;
            for (int i = 0; i < template_func->type_param_count; i++) {
                if (strcmp(param->struct_type_name, template_func->type_params[i]) == 0) {
                    type_param_index = i;
                    break;
                }
            }
            
            if (type_param_index >= 0) {
                // Substitute with concrete type
                param_struct_type = strdup(concrete_types[type_param_index]);
            } else {
                // Not a type parameter, copy as-is
                param_struct_type = strdup(param->struct_type_name);
            }
        }
        
        // Add parameter
        if (param_struct_type) {
            function_add_struct_param(specialized, param->name, param_struct_type);
            free(param_struct_type);
        } else {
            function_add_param(specialized, param->name, param_type);
        }
        
        param = param->next;
    }
    
    // TODO: Clone and substitute function body (statements)
    // For now, just copy the body pointer (shallow copy)
    specialized->body = template_func->body;
    
    return specialized;
}
