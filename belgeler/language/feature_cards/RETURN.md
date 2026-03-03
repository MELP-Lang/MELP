# Return — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Control flow — function return

---

## 📖 BASIC SYNTAX

```melp
return expression    -- return a value
return               -- return from void function
```

`return` terminates the current function and optionally passes a value back to the caller.

---

## ✅ COMPLETE EXAMPLES

### Return numeric
```melp
function add(a: numeric, b: numeric) -> numeric
    return a + b
end_function

numeric result = add(3, 4)
print(result)
-- Prints: 7
```

### Conditional return (early exit)
```melp
function abs_val(x: numeric) -> numeric
    if x < 0
        return 0 - x
    end_if
    return x
end_function

print(abs_val(-5))
-- Prints: 5
```

### Return from void function
```melp
function greet(x: numeric) -> void
    if x == 0
        return
    end_if
    print(x)
end_function
```

### Return inside loop
```melp
function first_even(limit: numeric) -> numeric
    for i = 0 to limit
        if i % 2 == 0
            return i
        end_if
    end_for
    return -1
end_function

print(first_even(10))
-- Prints: 0
```

---

## ⚠️ CRITICAL RULES

- Every non-`void` function **must** return a value on all code paths.
- The return type is declared after `->` in the function signature.
- `return` without a value is only valid in `-> void` functions.
- Multiple `return` statements are allowed (early returns).
- `return` exits the **immediately enclosing function** only (not loops or blocks).

---

## 🔗 RELATED CARDS

- [FUNCTION_DEF.md](FUNCTION_DEF.md) — function definitions
- [IF_STATEMENT.md](IF_STATEMENT.md) — conditionals for early return
- [ERROR_HANDLING.md](ERROR_HANDLING.md) — error propagation with return
