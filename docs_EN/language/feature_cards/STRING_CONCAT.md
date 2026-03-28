# String Concatenation — MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** String variables and binary `+` concatenation

---

## 📖 BASIC SYNTAX

```mlp
string result = strA + strB
```

String variables are declared with the `string` type keyword. The `+` operator concatenates two string values and returns a new string.

---

## ✅ COMPLETE EXAMPLES

### Simple concat stored in variable
```mlp
string a = "Hello"
string b = " World"
string c = a + b
print(c)
-- Prints: Hello World
```

### Multi-step concat
```mlp
string x = "foo"
string y = "bar"
string z = "baz"
string ab = x + y
string abc = ab + z
print(abc)
-- Prints: foobarbaz
```

### Literal concat
```mlp
string greeting = "Hi" + " there"
print(greeting)
-- Prints: Hi there
```

### Print directly
```mlp
string name = "MELP"
print(name)
-- Prints: MELP
```

---

## ⚠️ CRITICAL RULES

- The type keyword is `string` (lowercase).
- String values must be enclosed in double quotes: `"text"`.
- `+` between two string operands performs concatenation (not numeric addition).
- If one operand is `numeric` and the other `string`, behaviour is undefined — keep types consistent.
- The resulting string is heap-allocated via libc `malloc`; memory is managed by the runtime.
- `print()` auto-detects string variables and uses `puts` internally.

---

## 🔗 RELATED CARDS

- [VARIABLE.md](VARIABLE.md) — variable declarations
- [PRINT.md](PRINT.md) — print built-in
- [OPERATORS.md](OPERATORS.md) — operator overview
