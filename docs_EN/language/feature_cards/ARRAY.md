# Array — MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
numeric[] name = [elem1; elem2; elem3]
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Numeric Array
```mlp
numeric[] nums = [1; 2; 3; 4; 5]
print(nums[0])
-- Output: 1
```

### Example 2 — Array Length
```mlp
numeric[] data = [10; 20; 30]
numeric len = data.length
print(len)
-- Output: 3
```

### Example 3 — Mutating an Element
```mlp
numeric[] vals = [1; 2; 3]
vals[1] = 99
print(vals[1])
-- Output: 99
```

### Example 4 — Iterating with For
```mlp
numeric[] items = [5; 10; 15]
loop i = 0 to 2
    print(items[i])
end loop
-- Output: 5
-- Output: 10
-- Output: 15
```

### Example 5 — In a Function
```mlp
function main()
    numeric[] scores = [85; 90; 78]
    numeric sum = 0
    loop i = 0 to 2
        sum = sum + scores[i]
    end loop
    print(sum)
end function
-- Output: 253
```

---

## ⚠️ CRITICAL RULES

1. Indices are **0-based**.
2. Out-of-bounds access is a runtime error.
3. Array element separator: `;` (semicolon, not comma).
4. `length` is a read-only property, not a method call.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: comma separator in array literal
numeric[] nums = [1, 2, 3]
```

```mlp
-- ✅ CORRECT: semicolon separator
numeric[] nums = [1; 2; 3]
```

---

## 🔗 RELATED CARDS

- [FOR_LOOP.md](FOR_LOOP.md) — iteration
- [FOR_EACH.md](FOR_EACH.md) — iterate over elements directly
- [COLLECTIONS.md](COLLECTIONS.md) — overview of collection types
