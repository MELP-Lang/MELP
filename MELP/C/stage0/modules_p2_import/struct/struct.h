#ifndef STRUCT_H
#define STRUCT_H

#include <stddef.h>
#include "../lexer/lexer.h"

// Forward declarations
typedef struct Statement Statement;
typedef struct Expression Expression;

// YZ_86: Method parameter
typedef struct MethodParam {
    char* name;
    char* type;                    // numeric, string, boolean, or struct type
    struct MethodParam* next;
} MethodParam;

// YZ_86: Struct method definition
typedef struct StructMethod {
    char* name;                    // Method name
    char* return_type;             // Return type (numeric, string, boolean, struct, or "void")
    MethodParam* params;           // Parameter list (excluding implicit 'self')
    Statement* body;               // Method body statements
    struct StructMethod* next;     // Next method in list
} StructMethod;

// Yapı üyesi tanımı
typedef struct StructMember {
    char* name;                    // Üye adı
    char* type;                    // Üye tipi (numeric, string, boolean, başka struct)
    size_t offset;                 // Yapı içindeki offset (byte cinsinden)
    size_t size;                   // Üye boyutu
    struct StructDef* nested_def;  // YZ_83: Nested struct definition (NULL if primitive)
    struct StructMember* next;     // Sonraki üye (linked list)
} StructMember;

// Yapı tanımı
typedef struct StructDef {
    char* name;                    // Yapı adı
    StructMember* members;         // Üye listesi
    StructMethod* methods;         // YZ_86: Method listesi
    size_t total_size;             // Toplam yapı boyutu
    size_t alignment;              // Hizalama gereksinimi (8 byte)
    struct StructDef* next;        // YZ_82: Registry için linked list
} StructDef;

// Yapı örneği (instantiation)
typedef struct StructInstance {
    char* struct_name;             // Hangi yapıdan türetildi
    char* instance_name;           // Örnek değişken adı
    StructDef* definition;         // Yapı tanımına referans
} StructInstance;

// Üye erişimi
typedef struct MemberAccess {
    char* struct_instance;         // Yapı örneği adı
    char* member_name;             // Erişilen üye adı (deprecated - use member_chain)
    StructMember* member;          // Üye tanımına referans
    
    // YZ_83: Nested member access support (p.addr.zip)
    char** member_chain;           // Array of member names ["addr", "zip"]
    int chain_length;              // Number of members in chain
} MemberAccess;

// YZ_82/YZ_83: Member assignment (p.x = 10 or p.addr.zip = 12345)
typedef struct MemberAssignment {
    char* instance_name;           // Instance variable name
    char* member_name;             // Member field name (deprecated - use chain)
    void* value_expr;              // ArithmeticExpr* (value to assign)
    
    // YZ_83: Nested member assignment support
    char** member_chain;           // Array of member names ["addr", "zip"]
    int chain_length;              // Number of members in chain
} MemberAssignment;

// API fonksiyonları
StructDef* struct_create_definition(const char* name);
void struct_add_member(StructDef* def, const char* name, const char* type);
void struct_calculate_layout(StructDef* def);
StructInstance* struct_create_instance(StructDef* def, const char* instance_name);
MemberAccess* struct_create_member_access(const char* instance, const char* member);

void struct_def_free(StructDef* def);
void struct_instance_free(StructInstance* inst);
void struct_member_access_free(MemberAccess* access);

// YZ_82: Struct definition registry (for looking up types)
void struct_register_definition(StructDef* def);
StructDef* struct_lookup_definition(const char* name);
void struct_clear_definitions(void);

// YZ_82: Struct instance tracking (for codegen)
typedef struct StructInstanceInfo {
    char* instance_name;
    StructDef* definition;
    int stack_offset;              // Offset from rbp
    int is_pointer;                // YZ_84: 1 if this is a pointer (function parameter), 0 if value
    struct StructInstanceInfo* next;
} StructInstanceInfo;

void struct_register_instance(const char* name, StructDef* def, int offset);
void struct_register_instance_pointer(const char* name, StructDef* def, int offset);  // YZ_84
StructInstanceInfo* struct_lookup_instance(const char* name);
void struct_clear_instances(void);

// YZ_86: Method API
StructMethod* struct_method_create(const char* name, const char* return_type);
void struct_method_add_param(StructMethod* method, const char* name, const char* type);
void struct_add_method(StructDef* def, StructMethod* method);
StructMethod* struct_find_method(StructDef* def, const char* method_name);
void struct_method_free(StructMethod* method);

// YZ_86: Method call
typedef struct MethodCall {
    char* instance_name;           // Instance calling the method (receiver)
    char* method_name;             // Method name
    Expression** arguments;        // Arguments (excluding implicit self)
    int arg_count;
} MethodCall;

MethodCall* method_call_create(const char* instance, const char* method);
void method_call_add_arg(MethodCall* call, Expression* arg);
void method_call_free(MethodCall* call);

// YZ_85: STO Size Analysis for struct return optimization
size_t struct_get_size(StructDef* def);

// YZ_85: Return method selection based on STO thresholds
typedef enum {
    STRUCT_RETURN_REGISTER,    // ≤16 bytes: RAX + RDX
    STRUCT_RETURN_STACK_COPY,  // 17-64 bytes: memcpy to stack
    STRUCT_RETURN_HIDDEN_PTR   // >64 bytes: hidden pointer
} StructReturnMethod;

StructReturnMethod struct_get_return_method(StructDef* def);

#endif // STRUCT_H
