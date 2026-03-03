# Result<T, E> Usage Guide

## Quick Start

```mlp
-- Define Result type
enum Result<T, E>
  Ok(T)
  Err(E)
end_enum

-- Success case
Result success = Result.Ok(42)
match success
  case Ok(value) -> print(value)      -- Prints: 42
  case Err(error) -> print(0)
end_match

-- Error case
Result failure = Result.Err(404)
match failure
  case Ok(value) -> print(0)
  case Err(code) -> print(code)       -- Prints: 404
end_match
```

## Option<T> Pattern

```mlp
enum Option<T>
  Some(T)
  None
end_enum

Option maybe = Option.Some(100)
match maybe
  case Some(v) -> print(v)            -- Prints: 100
  case None -> print(0)
end_match

Option nothing = Option.None
match nothing
  case Some(v) -> print(999)
  case None -> print(0)               -- Prints: 0
end_match
```

## Pattern Matching

### Extract Values

```mlp
Result r = Result.Ok(5)

match r
  case Ok(value) -> 
    print(value)                      -- value = 5
  case Err(error) ->
    print(error)
end_match
```

### Multiple Results

```mlp
Result r1 = Result.Ok(10)
Result r2 = Result.Err(20)
Result r3 = Result.Ok(30)

match r1
  case Ok(v) -> print(v)              -- 10
  case Err(e) -> print(0)
end_match

match r2
  case Ok(v) -> print(0)
  case Err(e) -> print(e)            -- 20
end_match

match r3
  case Ok(v) -> print(v)              -- 30
  case Err(e) -> print(0)
end_match
```

## Working Patterns

### ✅ Supported

1. **Literal Values**
```mlp
Result r1 = Result.Ok(42)           -- Integer literals
Result r2 = Result.Err(7)           -- Error codes
```

2. **Variables**
```mlp
numeric x = 100
Result r = Result.Ok(x)             -- Variable pass-through
```

3. **Zero Values**
```mlp
Result r = Result.Ok(0)             -- Zero is valid
```

4. **Large Numbers**
```mlp
Result r = Result.Ok(999999)        -- Large integers
```

5. **Multiple Variants**
```mlp
Result success = Result.Ok(1)
Result failure = Result.Err(2)      -- Both in same scope
```

## Known Limitations

### ❌ NOT Working (Yet)

1. **Arithmetic Expressions** (prints tag instead of value)
```mlp
numeric x = 10
numeric y = 5
Result r = Result.Ok(x + y)         -- ❌ Prints 1 instead of 15

-- Workaround:
numeric sum = x + y
Result r = Result.Ok(sum)           -- ✅ Works
```

2. **Reassignment** (second value not stored)
```mlp
Result r = Result.Ok(42)            -- ✅ Works
match r
  case Ok(v) -> print(v)            -- Prints: 42
  case Err(e) -> print(0)
end_match

r = Result.Err(99)                  -- ⚠️ Tag updates, value doesn't
match r
  case Ok(v) -> print(0)
  case Err(e) -> print(e)           -- ❌ Prints 0 instead of 99
end_match
```

3. **Strings** (only numeric payloads)
```mlp
-- NOT IMPLEMENTED:
-- Result r = Result.Err("error message")
```

4. **Method Chaining** (combinators)
```mlp
-- NOT IMPLEMENTED:
-- result.map(fn).and_then(fn2)
```

## Best Practices

### 1. Use Pre-Computed Values

**Don't:**
```mlp
Result r = Result.Ok(compute())     -- Might not work
```

**Do:**
```mlp
numeric value = compute()
Result r = Result.Ok(value)         -- Always works
```

### 2. Avoid Reassignment

**Don't:**
```mlp
Result r = Result.Ok(1)
r = Result.Err(2)                   -- Value not updated
```

**Do:**
```mlp
Result r1 = Result.Ok(1)
Result r2 = Result.Err(2)           -- Use separate variables
```

### 3. Always Match Both Cases

**Don't:**
```mlp
match result
  case Ok(v) -> print(v)
  -- Missing Err case!
end_match
```

**Do:**
```mlp
match result
  case Ok(v) -> print(v)
  case Err(e) -> print(0)           -- Handle both
end_match
```

## Examples

### Error Code Handling

```mlp
enum Result<T, E>
  Ok(T)
  Err(E)
end_enum

Result file_status = Result.Err(404)

match file_status
  case Ok(size) -> print(size)
  case Err(code) -> 
    match code
      case 404 -> print(0)          -- Not found
      case 500 -> print(1)          -- Server error
    end_match
end_match
```

### Multiple Operations

```mlp
enum Result<T, E>
  Ok(T)
  Err(E)
end_enum

Result op1 = Result.Ok(10)
Result op2 = Result.Ok(20)
Result op3 = Result.Err(1)

numeric total = 0

match op1
  case Ok(v) -> total = total + v
  case Err(e) -> total = 0
end_match

match op2
  case Ok(v) -> total = total + v
  case Err(e) -> total = 0
end_match

print(total)  -- 30
```

## Testing

Run golden tests:
```bash
./run_result_tests.sh
```

Compile single file:
```bash
./build/compiler/stage1/modules/orchestrator/melp_compiler my_file.mlp
lli output.ll
```

## Reference

- Full report: [WEEK_9_1_RESULT_TYPE_COMPLETE.md](WEEK_9_1_RESULT_TYPE_COMPLETE.md)
- Tests: `GOLDEN_TEST_SUITE/result_type/`
- Parser: `compiler/stage0/parser/parser.cpp`
- Codegen: `compiler/stage1/modules/enum/enum_codegen.cpp`
