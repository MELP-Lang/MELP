# Comparison Operators — MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
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

All comparison operators return a truth value usable in `if`, `loop`, and `else if` conditions.

---

## ✅ COMPLETE EXAMPLES

### Equal
```mlp
numeric x = 5
if x == 5 then
    print(1)
end if
-- Prints: 1
```

### Not equal
```mlp
numeric y = 3
if y != 5 then
    print(1)
end if
-- Prints: 1
```

### Less than / greater than
```mlp
numeric a = 2
numeric b = 7
if a < b then
    print(1)
end if
-- Prints: 1
```

### Less than or equal
```mlp
numeric count = 5
if count <= 5 then
    print(1)
end if
-- Prints: 1
```

### Greater than or equal
```mlp
numeric score = 90
if score >= 90 then
    print(4)
else
    if score >= 80 then
        print(3)
    else
        print(2)
    end if
end if
-- Prints: 4
```

### In loop condition
```mlp
numeric i = 0
loop i != 3
    i = i + 1
end loop
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
- [ELSE_IF.md](ELSE_IF.md) — else if chains
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md) — and/or/not
- [OPERATORS.md](OPERATORS.md) — arithmetic operators
- [LOOP_STATEMENT.md](LOOP_STATEMENT.md) — loop conditions
