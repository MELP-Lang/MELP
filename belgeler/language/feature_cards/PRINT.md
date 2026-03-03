# Print — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Built-in I/O — standard output

---

## 📖 BASIC SYNTAX

```melp
print(expression)
println(expression)   -- same as print but explicit newline
print_str(string_var) -- deprecated alias; prefer print()
```

`print` is a built-in, not a user-defined function. It writes to stdout followed by a newline.

---

## ✅ COMPLETE EXAMPLES

### Numeric value
```melp
print(42)
-- Prints: 42
```

### Variable
```melp
numeric x = 7
print(x)
-- Prints: 7
```

### String variable
```melp
string msg = "Hello"
print(msg)
-- Prints: Hello
```

### String literal
```melp
print("World")
-- Prints: World
```

### Expression result
```melp
numeric a = 3
numeric b = 4
print(a + b)
-- Prints: 7
```

### String concat result
```melp
string s1 = "foo"
string s2 = "bar"
print(s1 + s2)
-- Prints: foobar
```

---

## ⚠️ CRITICAL RULES

- `print(x)` always appends a newline (`\n`) to output.
- Numeric values are printed as decimal integers via `printf("%lld\n")`.
- String values are printed via `puts()` (no extra newline added by puts; the `\n` from puts is used).
- Passing a string expression directly to `print()` is supported: `print(a + b)`.
- `print_str` is a legacy alias — prefer `print` for all types.
- There is no format-string support; use string concatenation to compose output.

---

## 🔗 RELATED CARDS

- [STRING_CONCAT.md](STRING_CONCAT.md) — building strings
- [VARIABLE.md](VARIABLE.md) — variables
- [FUNCTION_DEF.md](FUNCTION_DEF.md) — user-defined functions
