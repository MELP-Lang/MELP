#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>  // YZ_83: For debug fprintf

// Yapı tanımı oluştur
StructDef* struct_create_definition(const char* name) {
    StructDef* def = malloc(sizeof(StructDef));
    def->name = strdup(name);
    def->members = NULL;
    def->methods = NULL;  // YZ_86: Initialize methods
    def->total_size = 0;
    def->alignment = 8; // x86-64 default hizalama
    def->next = NULL;   // YZ_82: Initialize next pointer
    return def;
}

// Yapıya üye ekle
void struct_add_member(StructDef* def, const char* name, const char* type) {
    StructMember* member = malloc(sizeof(StructMember));
    member->name = strdup(name);
    member->type = strdup(type);
    member->next = NULL;
    member->nested_def = NULL;  // YZ_83: Initialize nested struct pointer
    
    // Tip boyutlarını belirle
    if (strcmp(type, "numeric") == 0) {
        member->size = 8; // INT64 veya DOUBLE
    } else if (strcmp(type, "string") == 0) {
        member->size = 24; // SSO struct boyutu
    } else if (strcmp(type, "boolean") == 0) {
        member->size = 1; // bool
    } else {
        // YZ_83: Check if this is a nested struct type
        StructDef* nested = struct_lookup_definition(type);
        if (nested) {
            member->nested_def = nested;
            member->size = nested->total_size;  // Use full struct size
        } else {
            member->size = 8; // Unknown type, assume pointer
        }
    }
    
    // Listeye ekle
    if (def->members == NULL) {
        def->members = member;
    } else {
        StructMember* current = def->members;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = member;
    }
}

// Yapı layout'unu hesapla (offsetler ve toplam boyut)
void struct_calculate_layout(StructDef* def) {
    size_t offset = 0;
    StructMember* current = def->members;
    
    while (current != NULL) {
        // Hizalama uygula
        size_t align = (current->size < 8) ? current->size : 8;
        if (offset % align != 0) {
            offset += align - (offset % align);
        }
        
        current->offset = offset;
        offset += current->size;
        current = current->next;
    }
    
    // Toplam boyutu hizala
    if (offset % def->alignment != 0) {
        offset += def->alignment - (offset % def->alignment);
    }
    
    def->total_size = offset;
}

// Yapı örneği oluştur
StructInstance* struct_create_instance(StructDef* def, const char* instance_name) {
    StructInstance* inst = malloc(sizeof(StructInstance));
    inst->struct_name = strdup(def->name);
    inst->instance_name = strdup(instance_name);
    inst->definition = def;
    return inst;
}

// Üye erişimi oluştur
MemberAccess* struct_create_member_access(const char* instance, const char* member) {
    MemberAccess* access = malloc(sizeof(MemberAccess));
    access->struct_instance = strdup(instance);
    access->member_name = strdup(member);
    access->member = NULL; // Parser tarafından doldurulacak
    
    // YZ_83: Initialize member chain with single member
    access->member_chain = malloc(sizeof(char*));
    access->member_chain[0] = strdup(member);
    access->chain_length = 1;
    
    return access;
}

// Bellek temizleme
void struct_def_free(StructDef* def) {
    if (!def) return;
    
    free(def->name);
    
    StructMember* current = def->members;
    while (current != NULL) {
        StructMember* next = current->next;
        free(current->name);
        free(current->type);
        free(current);
        current = next;
    }
    
    // YZ_86: Free methods
    StructMethod* method = def->methods;
    while (method != NULL) {
        StructMethod* next = method->next;
        struct_method_free(method);
        method = next;
    }
    
    free(def);
}

void struct_instance_free(StructInstance* inst) {
    if (!inst) return;
    free(inst->struct_name);
    free(inst->instance_name);
    free(inst);
}

void struct_member_access_free(MemberAccess* access) {
    if (!access) return;
    free(access->struct_instance);
    free(access->member_name);
    
    // YZ_83: Free member chain
    if (access->member_chain) {
        for (int i = 0; i < access->chain_length; i++) {
            free(access->member_chain[i]);
        }
        free(access->member_chain);
    }
    
    free(access);
}

// YZ_82: Struct definition registry (simple linked list)
// YZ_34 FIX: Use separate wrapper to avoid conflicting with main struct list
typedef struct StructRegistryEntry {
    StructDef* def;
    struct StructRegistryEntry* next;
} StructRegistryEntry;

static StructRegistryEntry* registered_structs = NULL;

void struct_register_definition(StructDef* def) {
    if (!def) return;
    
    // Check if already registered (avoid duplicates)
    StructRegistryEntry* current = registered_structs;
    while (current) {
        if (strcmp(current->def->name, def->name) == 0) {
            // Already registered, skip
            return;
        }
        current = current->next;
    }
    
    // Create new registry entry
    StructRegistryEntry* entry = malloc(sizeof(StructRegistryEntry));
    entry->def = def;
    entry->next = registered_structs;
    registered_structs = entry;
}

StructDef* struct_lookup_definition(const char* name) {
    StructRegistryEntry* current = registered_structs;
    while (current) {
        if (strcmp(current->def->name, name) == 0) {
            return current->def;
        }
        current = current->next;
    }
    return NULL;
}

void struct_clear_definitions(void) {
    // Free registry entries but not the structs themselves
    // The structs are owned by the main compiler
    StructRegistryEntry* current = registered_structs;
    while (current) {
        StructRegistryEntry* next = current->next;
        free(current);
        current = next;
    }
    registered_structs = NULL;
}

// YZ_82: Struct instance tracking (for codegen)
static StructInstanceInfo* registered_instances = NULL;

void struct_register_instance(const char* name, StructDef* def, int offset) {
    StructInstanceInfo* info = malloc(sizeof(StructInstanceInfo));
    info->instance_name = strdup(name);
    info->definition = def;
    info->stack_offset = offset;
    info->is_pointer = 0;  // YZ_84: By default, value type
    info->next = registered_instances;
    registered_instances = info;
}

// YZ_84: Register struct parameter (passed as pointer)
void struct_register_instance_pointer(const char* name, StructDef* def, int offset) {
    StructInstanceInfo* info = malloc(sizeof(StructInstanceInfo));
    info->instance_name = strdup(name);
    info->definition = def;
    info->stack_offset = offset;
    info->is_pointer = 1;  // YZ_84: Pointer type (function parameter)
    info->next = registered_instances;
    registered_instances = info;
}

StructInstanceInfo* struct_lookup_instance(const char* name) {
    StructInstanceInfo* current = registered_instances;
    while (current) {
        if (strcmp(current->instance_name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void struct_clear_instances(void) {
    StructInstanceInfo* current = registered_instances;
    while (current) {
        StructInstanceInfo* next = current->next;
        free(current->instance_name);
        free(current);
        current = next;
    }
    registered_instances = NULL;
}

// YZ_85: Get total size of struct (for STO analysis)
size_t struct_get_size(StructDef* def) {
    if (!def) return 0;
    
    // Use pre-calculated total_size if available
    if (def->total_size > 0) {
        return def->total_size;
    }
    
    // Otherwise calculate from members
    size_t total = 0;
    StructMember* member = def->members;
    while (member) {
        total += member->size;
        member = member->next;
    }
    
    return total;
}

// YZ_85: STO-based return method selection
// Thresholds:
//   ≤16 bytes  → RAX + RDX (2 registers, fastest)
//   17-64 bytes → Stack copy (memcpy, balanced)
//   >64 bytes  → Hidden pointer (memory efficient)
StructReturnMethod struct_get_return_method(StructDef* def) {
    if (!def) return STRUCT_RETURN_HIDDEN_PTR;
    
    size_t size = struct_get_size(def);
    
    if (size <= 16) {
        // Small struct: fits in 2 registers (RAX + RDX)
        return STRUCT_RETURN_REGISTER;
    } else if (size <= 64) {
        // Medium struct: stack copy with memcpy
        return STRUCT_RETURN_STACK_COPY;
    } else {
        // Large struct: hidden pointer for memory efficiency
        return STRUCT_RETURN_HIDDEN_PTR;
    }
}

// YZ_86: Method implementation
StructMethod* struct_method_create(const char* name, const char* return_type) {
    StructMethod* method = malloc(sizeof(StructMethod));
    method->name = strdup(name);
    method->return_type = strdup(return_type);
    method->params = NULL;
    method->body = NULL;
    method->next = NULL;
    return method;
}

void struct_method_add_param(StructMethod* method, const char* name, const char* type) {
    MethodParam* param = malloc(sizeof(MethodParam));
    param->name = strdup(name);
    param->type = strdup(type);
    param->next = NULL;
    
    // Append to parameter list
    if (method->params == NULL) {
        method->params = param;
    } else {
        MethodParam* current = method->params;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = param;
    }
}

void struct_add_method(StructDef* def, StructMethod* method) {
    if (!def || !method) return;
    
    // Append to method list
    if (def->methods == NULL) {
        def->methods = method;
    } else {
        StructMethod* current = def->methods;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = method;
    }
}

StructMethod* struct_find_method(StructDef* def, const char* method_name) {
    if (!def || !method_name) return NULL;
    
    StructMethod* current = def->methods;
    while (current) {
        if (strcmp(current->name, method_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void struct_method_free(StructMethod* method) {
    if (!method) return;
    
    free(method->name);
    free(method->return_type);
    
    // Free parameters
    MethodParam* param = method->params;
    while (param) {
        MethodParam* next = param->next;
        free(param->name);
        free(param->type);
        free(param);
        param = next;
    }
    
    // Note: method->body (Statement*) is freed by statement_free
    free(method);
}

// YZ_86: Method call implementation
MethodCall* method_call_create(const char* instance, const char* method) {
    MethodCall* call = malloc(sizeof(MethodCall));
    call->instance_name = strdup(instance);
    call->method_name = strdup(method);
    call->arguments = NULL;
    call->arg_count = 0;
    return call;
}

void method_call_add_arg(MethodCall* call, Expression* arg) {
    if (!call || !arg) return;
    
    call->arguments = realloc(call->arguments, sizeof(Expression*) * (call->arg_count + 1));
    call->arguments[call->arg_count] = arg;
    call->arg_count++;
}

void method_call_free(MethodCall* call) {
    if (!call) return;
    
    free(call->instance_name);
    free(call->method_name);
    
    // Note: arguments (Expression*) are freed by expression_free
    if (call->arguments) {
        free(call->arguments);
    }
    
    free(call);
}
