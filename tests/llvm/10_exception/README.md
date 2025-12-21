# YZ_207 Exception Handling Tests

Test suite for try-catch-finally exception handling.

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
# Compile test
./functions_compiler tests/llvm/10_exception/test_basic_try_catch.mlp temp/test.ll

# Execute (when runtime ready)
lli temp/test.ll
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

## Phase 1 Status

✅ Parser implementation complete
- Try-catch-finally syntax parsing
- Multiple catch clauses
- Exception type matching
- AST structures defined

⏳ Pending:
- LLVM codegen
- Runtime exception handling
- Stack unwinding
