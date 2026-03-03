# 📘 PMLP Feature Card: Error Handling (Try-Catch + Result)

**Durum:** ⚠️ PARTIAL (Stage 1 — try/catch parse edilir, mesaj formatı eksik)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  

**Stage:** Stage 1 Phase 1.5  
**Complexity:** ⭐⭐⭐ High  
**Estimated Time:** 4-5 days

---

## 🎯 Feature Summary

**What:** Structured error handling with try-catch and Result type.

**Why:** Handle errors gracefully without crashing.

**How:** `try-catch` blocks + `Result<T, E>` type for explicit error handling.

---

## 📖 Syntax Reference

### Try-Catch Block

```pmlp
try
    numeric result = risky_operation()
    print(result)
catch error
    print("Error: ")
    print(error.message)
end_try
```

**MELP Format:** `catch error` (error is the variable name).

### Result Type

```pmlp
struct Result<T; E>
    boolean is_success
    T value
    E error
end_struct

function divide(numeric a; numeric b) as Result<numeric; string>
    Result<numeric; string> result
    
    if b == 0 then
        result.is_success = false
        result.error = "Division by zero"
    else
        result.is_success = true
        result.value = a / b
    end_if
    
    return result
end_function
```

### Error Propagation

```pmlp
function process() as Result<numeric; string>
    Result<numeric; string> r1 = divide(10; 2)
    if not r1.is_success then
        return r1  -- Propagate error
    end_if
    
    Result<numeric; string> r2 = divide(r1.value; 5)
    return r2
end_function
```

---

## 🔍 Detailed Specification

### 1. Try-Catch Syntax

**Full Form:**
```pmlp
try
    -- code that may fail
catch error_var
    -- error handling
end_try
```

**Example:**
```pmlp
try
    numeric x = parse_number("abc")
catch err
    print("Parse failed: ")
    print(err.message)
end_try
```

### 2. Error Type

**Built-in Error Struct:**
```pmlp
struct Error
    string message
    numeric code
    string source
end_struct
```

**Custom Errors:**
```pmlp
struct ValidationError
    string message
    string field_name
end_struct

function validate_age(numeric age) as Result<numeric; ValidationError>
    Result<numeric; ValidationError> result
    
    if age < 0 then
        result.is_success = false
        result.error.message = "Age cannot be negative"
        result.error.field_name = "age"
    else
        result.is_success = true
        result.value = age
    end_if
    
    return result
end_function
```

### 3. Result Type Pattern

**Generic Result:**
```pmlp
struct Result<T; E>
    boolean is_success
    T value
    E error
end_struct
```

**Usage Pattern:**
```pmlp
Result<numeric; string> r = risky_function()

if r.is_success then
    print("Success: ")
    print(r.value)
else
    print("Error: ")
    print(r.error)
end_if
```

### 4. Finally Block (Optional)

```pmlp
try
    open_file("data.txt")
catch err
    print("Error opening file")
finally
    close_file()
end_try
```

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/error_handling_tests/`

### Test 1: Basic Try-Catch
```pmlp
function main() as numeric
    try
        numeric x = 10 / 0
        print(x)
    catch err
        print("Division error")
    end_try
    return 0
end_function
```

### Test 2: Result Type
```pmlp
function divide(numeric a; numeric b) as Result<numeric; string>
    Result<numeric; string> r
    
    if b == 0 then
        r.is_success = false
        r.error = "Cannot divide by zero"
    else
        r.is_success = true
        r.value = a / b
    end_if
    
    return r
end_function

function main() as numeric
    Result<numeric; string> result = divide(10; 0)
    
    if result.is_success then
        print(result.value)
    else
        print(result.error)
    end_if
    
    return 0
end_function
```

---

## ⚙️ Implementation Notes

**Try-Catch Compilation:**
- Use LLVM `invoke` instruction
- Generate landing pads for catch blocks
- Exception unwinding via libunwind

**Result Type:**
- Pure value-based (no exceptions)
- Zero runtime overhead
- Explicit error handling

**Error Propagation:**
```pmlp
-- Manual (Stage 1)
Result<T; E> r = function()
if not r.is_success then
    return r
end_if

-- Future: ? operator (Stage 2+)
T value = function()?
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```pmlp
try
    risky_call()
catch error
    print(error.message)
end_try
```

**❌ Wrong (Java style):**
```java
try {
    riskyCall();
} catch (Exception e) {
    System.out.println(e.getMessage());
}
```

**❌ Wrong (Rust style):**
```rust
match risky_call() {
    Ok(v) => println!("{}", v),
    Err(e) => println!("{}", e),
}
```

**MELP way:**
- `try-catch` keywords
- `catch variable_name` (no type annotation)
- `end_try` terminator
- Result type is explicit struct

---

## 📋 Success Criteria

- ✅ Try-catch blocks work
- ✅ Error objects created
- ✅ Result type compiles
- ✅ Error propagation works
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/error_handling_tests/
