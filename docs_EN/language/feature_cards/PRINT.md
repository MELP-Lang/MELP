# Print — MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
print(expression)
```

`print` is a built-in, not a user-defined function. It writes to stdout followed by a newline.

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Numeric Value
```mlp
print(42)
-- Output: 42
```

### Example 2 — Variable
```mlp
numeric x = 7
print(x)
-- Output: 7
```

### Example 3 — String Literal
```mlp
print("Hello, World!")
-- Output: Hello, World!
```

### Example 4 — Expression Result
```mlp
numeric a = 3
numeric b = 4
print(a + b)
-- Output: 7
```

### Example 5 — String Concat Result
```mlp
string s1 = "foo"
string s2 = "bar"
print(s1 + s2)
-- Output: foobar
```

### Example 6 — In a Function
```mlp
function main()
    string name = "Alice"
    print("Hello, " + name)
end function
-- Output: Hello, Alice
```

---

## ⚠️ CRITICAL RULES

1. `print(x)` always appends a newline to output.
2. Numeric values are printed as decimal integers.
3. Passing a string expression directly to `print()` is supported: `print(a + b)`.
4. There is no format-string support; use string concatenation to compose output.

---

## 🔗 RELATED CARDS

- [STRING_CONCAT.md](STRING_CONCAT.md) — building strings
- [VARIABLE.md](VARIABLE.md) — variables
- [FUNCTION_DEF.md](FUNCTION_DEF.md) — user-defined functions
