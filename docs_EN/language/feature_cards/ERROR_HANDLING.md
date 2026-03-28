# Error Handling - MeLP Syntax

**Status:** ⚠️ PARTIAL (Stage1 — try/catch parsed; message format incomplete)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
try
    -- code that may fail
catch error
    -- error handling
end try
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Try-Catch
```mlp
function main()
    try
        numeric x = 10 / 0
        print(x)
    catch err
        print("Division error")
    end try
end function
-- Output: Division error
```

### Example 2 — Result Type Pattern
```mlp
struct DivResult
    boolean is_success
    numeric value
    string error
end struct

DivResult function divide(numeric a; numeric b)
    DivResult r
    if b == 0 then
        r.is_success = false
        r.error = "Cannot divide by zero"
    else
        r.is_success = true
        r.value = a / b
    end if
    return r
end function

function main()
    DivResult result = divide(10; 0)
    if result.is_success then
        print(result.value)
    else
        print(result.error)
    end if
end function
-- Output: Cannot divide by zero
```

### Example 3 — Error Propagation
```mlp
struct CalcResult
    boolean is_success
    numeric value
    string error
end struct

CalcResult function safe_divide(numeric a; numeric b)
    CalcResult r
    if b == 0 then
        r.is_success = false
        r.error = "division by zero"
    else
        r.is_success = true
        r.value = a / b
    end if
    return r
end function

CalcResult function process(numeric x; numeric y; numeric z)
    CalcResult r1 = safe_divide(x; y)
    if not r1.is_success then
        return r1
    end if
    CalcResult r2 = safe_divide(r1.value; z)
    return r2
end function
```

---

## ⚠️ CRITICAL RULES

1. Try-catch block terminator: `end try` (two words, not `end try`).
2. `catch variable_name` — no type annotation on the caught variable.
3. For explicit error handling without exceptions, use a result struct.
4. Return type before `function` keyword: `DivResult function divide(...)`.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: end try (underscore)
try
    risky_call()
catch error
    print(error)
end try
```

```mlp
-- ✅ CORRECT: end try (space)
try
    risky_call()
catch error
    print(error)
end try
```

---

## 🔗 RELATED CARDS

- [RESULT_OPTION.md](RESULT_OPTION.md) — Result/Option types
- [ERROR_PROPAGATION.md](ERROR_PROPAGATION.md) — error propagation
- [ERROR_LOCATION.md](ERROR_LOCATION.md) — error location info
