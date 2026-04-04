# Return — MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
return expression    -- return a value
return               -- return from void function
```

`return` terminates the current function and optionally passes a value back to the caller.

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Return Numeric
```mlp
numeric function add(numeric a; numeric b)
    return a + b
end function

numeric result = add(3; 4)
print(result)
-- Output: 7
```

### Example 2 — Conditional Return (Early Exit)
```mlp
numeric function abs_val(numeric x)
    if x < 0 then
        return 0 - x
    end if
    return x
end function

print(abs_val(-5))
-- Output: 5
```

### Example 3 — Return from Void Function
```mlp
function greet(numeric x)
    if x == 0 then
        return
    end if
    print(x)
end function
```

### Example 4 — Return Inside Loop
```mlp
numeric function first_even(numeric limit)
    loop i = 0 to limit
        if i % 2 == 0 then
            return i
        end if
    end loop
    return -1
end function

print(first_even(10))
-- Output: 0
```

---

## ⚠️ CRITICAL RULES

1. Every non-void function **must** return a value on all code paths.
2. Return type is declared **before** the `function` keyword: `numeric function foo()`
3. `return` without a value is only valid in void functions.
4. Multiple `return` statements are allowed (early returns).
5. `return` exits the **immediately enclosing function** only.
6. `function main()` must NOT have `return 0` — no return statement.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: -> syntax for return type
function add(numeric a; numeric b) -> numeric
    return a + b
end function
```

```mlp
-- ✅ CORRECT: return type before function keyword
numeric function add(numeric a; numeric b)
    return a + b
end function
```

---

## 🔗 RELATED CARDS

- [FUNCTION_DEF.md](FUNCTION_DEF.md) — function definitions
- [IF_STATEMENT.md](IF_STATEMENT.md) — conditionals for early return
- [ERROR_HANDLING.md](ERROR_HANDLING.md) — error propagation with return
