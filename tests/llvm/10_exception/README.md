# YZ_207 Exception Handling Tests

Test suite for try-catch-finally exception handling.

## ✅ Implementation Status

**Phase 1-4: COMPLETE**
- ✅ Try-catch-finally syntax parser
- ✅ Exception type registry (6 built-in types)
- ✅ Throw statement parser
- ✅ LLVM codegen (landingpad/invoke)
- ✅ Runtime support functions
- ✅ 8 comprehensive test files

## Test Files

### Basic Tests
1. **test_basic_try_catch.mlp** - Basic try-catch block
2. **test_multiple_catch.mlp** - Multiple catch clauses
3. **test_finally.mlp** - Finally block execution

### Advanced Tests
4. **test_nested.mlp** - Nested try-catch blocks
5. **test_rethrow.mlp** - Exception rethrowing
6. **test_propagation.mlp** - Exception propagation through call stack
7. **test_finally_only.mlp** - Try-finally without catch
8. **test_custom_exception.mlp** - Custom exception types

## Running Tests

```bash
# Compile test (when integrated)
./functions_compiler tests/llvm/10_exception/test_basic_try_catch.mlp temp/test.ll

# Execute (when runtime ready)
lli temp/test.ll
```

## Syntax Examples

### Basic Try-Catch
```pmpl
try
    numeric result = divide(10, 0)
    return result
catch e as DivisionByZeroException
    return -1
end_try
```

### Multiple Catch Clauses
```pmpl
try
    throw CustomException("error")
catch e as CustomException
    return 1
catch e as Exception
    return 2
end_try
```

### Try-Catch-Finally
```pmpl
try
    result = 10
    throw exception("error")
catch e as Exception
    result = 20
finally
    result = result + 5  -- Always runs
end_try
```

### Custom Exceptions
```pmpl
exception CustomException extends Exception
    numeric error_code
end_exception

function test() returns numeric
    try
        throw CustomException("Error", 404)
    catch e as CustomException
        return e.error_code
    end_try
end_function
```

## Expected Behavior

### Try-Catch
- Try block executes normally
- On exception: Jump to matching catch clause
- Catch clause handles exception
- Execution continues after try-catch

### Finally
- Always executes (exception or not)
- Executes before return
- Executes even if exception thrown

### Exception Matching
- Catch clauses tested in order
- First matching type catches exception
- More specific exceptions before general

## Implementation Details

### LLVM IR
- landingpad instruction for exception handling
- invoke instead of call for exception-aware calls
- resume instruction for rethrowing
- Personality function: __melp_personality_v0

### Runtime Functions
- `__melp_throw(exception_ptr)`: Throw exception
- `__melp_begin_catch(exception_ptr)`: Begin catch
- `__melp_end_catch()`: End catch
- `__melp_personality_v0(...)`: LLVM personality

### Built-in Exception Types
1. Exception (base)
2. DivisionByZeroException
3. NullPointerException
4. IndexOutOfBoundsException
5. TypeMismatchException
6. FileNotFoundException

## Phase Status

✅ **Phase 1:** Try-Catch-Finally Syntax  
✅ **Phase 2:** Exception Types & Registry  
✅ **Phase 3:** LLVM Codegen  
✅ **Phase 4:** Runtime Support  
⏳ **Phase 5:** Integration (Makefile + statement parser)  
⏳ **Phase 6:** Testing & Documentation

## Notes

- Result pattern (YZ_206) provides zero-cost alternative
- Try-catch has runtime overhead (stack unwinding)
- Both approaches can coexist
- User can choose based on preference
