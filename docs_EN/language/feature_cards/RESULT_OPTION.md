# Result / Option Types - MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
-- Option<T>: value may or may not be present
Option{numeric} maybe_val = Option.Some(42)
Option{numeric} nothing = Option.None

-- Result<T;E>: operation succeeded or failed
Result{numeric; string} r = Result.Ok(10)
Result{numeric; string} err = Result.Err("oops")
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Option Usage
```mlp
Option{numeric} function find(numeric[] arr; numeric target)
    loop i = 0 to arr.length - 1
        if arr[i] == target then
            return Option.Some(i)
        end if
    end loop
    return Option.None
end function

function main()
    numeric[] data = [10; 20; 30]
    Option{numeric} idx = find(data; 20)
    match idx
        case Some(v) then print(v)
        case None then print("not found")
    end match
end function
-- Output: 1
```

### Example 2 — Result Usage
```mlp
Result{numeric; string} function divide(numeric a; numeric b)
    if b == 0 then
        return Result.Err("division by zero")
    end if
    return Result.Ok(a / b)
end function

function main()
    Result{numeric; string} r = divide(10; 2)
    match r
        case Ok(v) then print(v)
        case Err(e) then print(e)
    end match
end function
-- Output: 5
```

### Example 3 — Unwrap
```mlp
function main()
    Option{numeric} opt = Option.Some(42)
    numeric value = opt.unwrap()
    print(value)
end function
-- Output: 42
```

---

## ⚠️ CRITICAL RULES

1. Use `{T}` for generic type parameter (not `<T>`): `Option{numeric}`
2. Access variants: `Option.Some(val)`, `Option.None`, `Result.Ok(val)`, `Result.Err(e)`
3. Return type before `function` keyword: `Option{numeric} function find(...)`
4. Use `match` to safely unwrap variants.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: -> syntax for return type
function find(numeric[] arr; numeric target) -> Option<numeric>
```

```mlp
-- ✅ CORRECT: return type before function keyword
Option{numeric} function find(numeric[] arr; numeric target)
```

---

## 🔗 RELATED CARDS

- [PATTERN_MATCHING.md](PATTERN_MATCHING.md) — using match with Option/Result
- [ERROR_HANDLING.md](ERROR_HANDLING.md) — error handling
- [GENERICS.md](GENERICS.md) — generic types
