# Nullable Types - MeLP Syntax

**Status:** ⚠️ PARTIAL (Stage1 — basic nullable works; `??` and `?.` not yet implemented)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

Add `?` to any type to make it nullable:

```mlp
numeric? n = null
string? name = null
boolean? flag = true
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Nullable Declaration and Null Check
```mlp
function main()
    numeric? n = null
    if n != null then
        print(n)
    else
        print("is null")
    end if
end function
-- Output: is null
```

### Example 2 — Assigning Value
```mlp
function main()
    numeric? n = null
    n = 42
    if n != null then
        print(n)
    end if
end function
-- Output: 42
```

### Example 3 — Nullable Function Return
```mlp
numeric? function find_age(string name)
    if name == "Ali" then
        return 25
    end if
    return null
end function

function main()
    numeric? age = find_age("Ali")
    if age != null then
        print(age)
    else
        print("not found")
    end if

    numeric? age2 = find_age("Bob")
    if age2 != null then
        print(age2)
    else
        print("not found")
    end if
end function
-- Output: 25
-- Output: not found
```

### Example 4 — Null Fallback (Manual)
```mlp
function main()
    numeric? n = null
    numeric val = 0
    if n != null then
        val = n
    end if
    print(val)
end function
-- Output: 0
```

---

## ⚠️ CRITICAL RULES

1. Nullable type: `type?` — add `?` suffix to any type.
2. Null check: `if x != null then`
3. `??` (null coalescing) and `?.` (safe navigation) are **not yet implemented**.
4. `Option{T}` is preferred for type-safe null handling.
5. Return type before `function` keyword: `numeric? function find_age(...)`

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: -> syntax for return type
function find_age(string name) -> numeric?
    return null
end function
```

```mlp
-- ✅ CORRECT: return type before function keyword
numeric? function find_age(string name)
    return null
end function
```

---

## 🔗 RELATED CARDS

- [RESULT_OPTION.md](RESULT_OPTION.md) — Option{T} preferred alternative
- [IF_STATEMENT.md](IF_STATEMENT.md) — null checks
