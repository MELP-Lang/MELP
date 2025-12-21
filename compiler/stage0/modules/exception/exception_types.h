#ifndef EXCEPTION_TYPES_H
#define EXCEPTION_TYPES_H

#include "exception.h"

// Built-in exception type names
#define EXCEPTION_TYPE_BASE                 "Exception"
#define EXCEPTION_TYPE_DIVISION_BY_ZERO     "DivisionByZeroException"
#define EXCEPTION_TYPE_NULL_POINTER         "NullPointerException"
#define EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS  "IndexOutOfBoundsException"
#define EXCEPTION_TYPE_TYPE_MISMATCH        "TypeMismatchException"
#define EXCEPTION_TYPE_FILE_NOT_FOUND       "FileNotFoundException"

// Exception type registry
typedef struct ExceptionTypeInfo {
    char* name;
    ExceptionType type;
    struct ExceptionTypeInfo* parent;  // For inheritance
} ExceptionTypeInfo;

// Exception type registry functions
void exception_types_init(void);
void exception_types_cleanup(void);

// Register exception type
void register_exception_type(const char* name, ExceptionType type, const char* parent_name);

// Query exception types
ExceptionType get_exception_type_by_name(const char* name);
const char* get_exception_name_by_type(ExceptionType type);
int is_exception_type_compatible(ExceptionType thrown, ExceptionType caught);

// Check if exception type is subtype of another
int is_subtype_of(const char* child_type, const char* parent_type);

#endif // EXCEPTION_TYPES_H
