# Comparison Operators — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Operators — relational comparisons

---

## 📖 BASIC SYNTAX

| Operator | Meaning |
|----------|---------|
| `==` | Equal to |
| `!=` | Not equal to |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal to |
| `>=` | Greater than or equal to |

All comparison operators return a truth value usable in `if`, `while`, and `else_if` conditions.

---

## ✅ COMPLETE EXAMPLES

### Equal
```melp
numeric x = 5
if x == 5
    print(1)
end_if
-- Prints: 1
```

### Not equal
```melp
numeric y = 3
if y != 5
    print(1)
end_if
-- Prints: 1
```

### Less than / greater than
```melp
numeric a = 2
numeric b = 7
if a < b
    print(1)
end_if
-- Prints: 1
```

### Less than or equal
```melp
numeric count = 5
if count <= 5
    print(1)
end_if
-- Prints: 1
```

### Greater than or equal
```melp
numeric score = 90
if score >= 90
    print(4)
else_if score >= 80
    print(3)
else
    print(2)
end_if
-- Prints: 4
```

### In while condition
```melp
numeric i = 0
while i != 3
    i = i + 1
end_while
print(i)
-- Prints: 3
```

---

## ⚠️ CRITICAL RULES

- Comparisons currently operate on `numeric` (integer) values.
- String equality via `==` is not yet supported (compares pointer addresses, not content).
- Chained comparisons like `0 < x < 10` are **not** valid; use `x > 0 and x < 10`.
- Result is truthy (non-zero) or falsy (0) — there is no dedicated `bool` type.
- `=` is **assignment**; `==` is **equality test**.

---

## 🔗 RELATED CARDS

- [IF_STATEMENT.md](IF_STATEMENT.md) — if/else
- [ELSE_IF.md](ELSE_IF.md) — else-if chains
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md) — and/or/not
- [OPERATORS.md](OPERATORS.md) — arithmetic operators
- [WHILE_LOOP.md](WHILE_LOOP.md) — while loop conditions
