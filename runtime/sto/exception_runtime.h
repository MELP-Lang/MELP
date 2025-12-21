#ifndef EXCEPTION_RUNTIME_H
#define EXCEPTION_RUNTIME_H

// Runtime exception support for MELP
// These functions are called by LLVM-generated code

// Throw an exception
void __melp_throw(void* exception_ptr) __attribute__((noreturn));

// Begin catching an exception
void* __melp_begin_catch(void* exception_ptr);

// End catching (cleanup)
void __melp_end_catch(void);

// Personality function for LLVM exception handling
int __melp_personality_v0(int version, int actions, unsigned long long exception_class,
                          void* exception_object, void* context);

#endif // EXCEPTION_RUNTIME_H
