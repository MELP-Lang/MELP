# Exception Handling Module

Try-catch-finally exception handling for MELP.

## Overview

This module implements imperative exception handling as an **alternative** to the Result pattern (YZ_206). Both approaches can coexist - developers can choose based on preference.

**Result Pattern (YZ_206):**
- Zero-cost (compile-time)
- Explicit error handling
- Functional style
- Recommended for performance-critical code

**Try-Catch (YZ_207):**
- Runtime overhead (stack unwinding)
- Imperative style (familiar to Java/C++ developers)
- Automatic propagation
- Finally block for cleanup

## Module Structure

```
compiler/stage0/modules/exception/
├── exception.h              # Core exception structures
├── exception.c              # Exception management
├── exception_types.h        # Exception type registry
├── exception_types.c        # Type system & inheritance
├── exception_parser.h       # Custom exception definitions
├── exception_parser.c       # Exception definition parser
├── try_catch_parser.h       # Try-catch-finally parser
├── try_catch_parser.c       # Parser implementation
├── throw_parser.h           # Throw statement parser
├── throw_parser.c           # Throw implementation
├── exception_codegen.h      # LLVM codegen
└── exception_codegen.c      # LLVM exception handling
```

## Features

### 1. Try-Catch-Finally Syntax
```pmpl
try
    -- Code that may throw
    result = divide(10, 0)
catch e as DivisionByZeroException
    print("Division error")
catch e as Exception
    print("Generic error")
finally
    cleanup()  -- Always runs
end_try
```

### 2. Exception Types
- **Base:** Exception
- **Built-in:**
  - DivisionByZeroException
  - NullPointerException
  - IndexOutOfBoundsException
  - TypeMismatchException
  - FileNotFoundException
- **Custom:** User-defined with inheritance

### 3. Throw Statement
```pmpl
function divide(numeric a, numeric b) returns numeric
    if b == 0 then
        throw DivisionByZeroException("Cannot divide by zero")
    end_if
    return a / b
end_function
```

### 4. Custom Exceptions
```pmpl
exception NetworkException extends Exception
    numeric error_code
    string host
end_exception

throw NetworkException("Connection failed", 404, "localhost")
```

### 5. Exception Propagation
- Automatic propagation through call stack
- First matching catch clause handles exception
- Finally blocks execute even on rethrow

## LLVM Implementation

### Exception Runtime Representation
```c
struct Exception {
    i8 variant;     // Exception type ID
    i8* message;    // Error message
    i8* stack_trace; // Stack trace (optional)
}
```

### LLVM IR Instructions
- **invoke:** Call with exception handling
- **landingpad:** Catch handler entry point
- **resume:** Rethrow exception
- **Personality Function:** `__melp_personality_v0`

### Example LLVM IR
```llvm
define i32 @test_try_catch() personality i8* @__melp_personality_v0 {
entry:
  %result = invoke i32 @divide(i32 10, i32 0)
    to label %normal unwind label %exception

normal:
  ret i32 %result

exception:
  %exc = landingpad { i8*, i32 }
    catch i8* @_ZTI14DivisionException
  %exc_ptr = extractvalue { i8*, i32 } %exc, 0
  ; Handle exception
  ret i32 -1
}
```

## Runtime Functions

Located in `runtime/sto/exception_runtime.{h,c}`:

```c
// Throw an exception
void __melp_throw(void* exception_ptr);

// Begin catching an exception
void* __melp_begin_catch(void* exception_ptr);

// End catching (cleanup)
void __melp_end_catch(void);

// LLVM personality function
int __melp_personality_v0(int version, int actions, ...);
```

## Type System

### Exception Registry
- Maintains exception type hierarchy
- Supports inheritance (extends keyword)
- Type compatibility checking

### API
```c
// Initialize exception types
void exception_types_init(void);

// Register custom exception
void register_exception_type(const char* name, ExceptionType type, 
                             const char* parent_name);

// Query exception type
ExceptionType get_exception_type_by_name(const char* name);

// Check subtype relationship
int is_subtype_of(const char* child, const char* parent);
```

## Integration

### 1. Lexer Tokens (lexer.h)
```c
TOKEN_TRY, TOKEN_CATCH, TOKEN_FINALLY, TOKEN_THROW,
TOKEN_EXCEPTION, TOKEN_END_TRY, TOKEN_END_EXCEPTION
```

### 2. Parser Integration (statement parser)
```c
if (tokens[pos]->type == TOKEN_TRY) {
    TryBlock* try_block = parse_try_block(tokens, &pos);
    // Add to statement AST
}

if (tokens[pos]->type == TOKEN_THROW) {
    ThrowStatement* throw_stmt = parse_throw_statement(tokens, &pos);
    // Add to statement AST
}
```

### 3. Codegen Integration (statement codegen)
```c
case STMT_TRY:
    return llvm_gen_try_block(stmt->data.try_block, module, builder, function);

case STMT_THROW:
    return llvm_gen_throw_statement(stmt->data.throw_stmt, module, builder, function);
```

### 4. Makefile
```makefile
EXCEPTION_OBJS = exception/exception.o \
                 exception/exception_types.o \
                 exception/exception_parser.o \
                 exception/try_catch_parser.o \
                 exception/throw_parser.o \
                 exception/exception_codegen.o
```

## Testing

Test suite: `tests/llvm/10_exception/`
- 8 comprehensive test files
- Covers all exception features
- See `tests/llvm/10_exception/README.md`

## Performance Considerations

### Runtime Overhead
- **Happy path:** Minimal (invoke ≈ call)
- **Exception thrown:** Significant (stack unwinding)
- **Finally block:** Always runs (overhead)

### Recommendations
- Use Result pattern for hot paths
- Use try-catch for readability/familiarity
- Avoid exceptions in tight loops
- Cache personality function

## Comparison: Result vs Try-Catch

| Feature | Result Pattern | Try-Catch |
|---------|---------------|-----------|
| Cost | Zero (compile-time) | Runtime overhead |
| Style | Functional | Imperative |
| Explicitness | Explicit (forced) | Can be forgotten |
| Propagation | Manual (? operator) | Automatic |
| Cleanup | Destructors | Finally block |
| Performance | Excellent | Good |

## Future Enhancements

- [ ] Stack trace generation
- [ ] Exception chaining (cause)
- [ ] Source location tracking
- [ ] Debugger integration
- [ ] Performance optimizations

## References

- LLVM Exception Handling: https://llvm.org/docs/ExceptionHandling.html
- Itanium C++ ABI: https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
- Zero-cost exceptions: https://www.iar.com/knowledge/learn/programming/advanced-c-exception-handling/

## Status

**YZ_207:** ✅ Core implementation complete  
**Integration:** ⏳ Pending (Makefile + statement parser)  
**Testing:** ⏳ Pending (integration tests)

**Next Steps:**
1. Add to Makefile
2. Integrate with statement parser
3. Test with LLVM backend
4. Performance benchmarking
