# Boolean Operations — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Logic — boolean operators `and`, `or`, `not`

---

## 📖 BASIC SYNTAX

```melp
condition1 and condition2   -- logical AND
condition1 or condition2    -- logical OR
not condition               -- logical NOT
```

Boolean operators are used in `if`, `while`, and `else_if` conditions.

---

## ✅ COMPLETE EXAMPLES

### Logical AND
```melp
numeric x = 5
numeric y = 10

if x > 0 and y > 0
    print(1)
else
    print(0)
end_if
-- Prints: 1
```

### Logical OR
```melp
numeric a = 0
numeric b = 5

if a > 0 or b > 0
    print(1)
else
    print(0)
end_if
-- Prints: 1
```

### Logical NOT
```melp
numeric flag = 0

if not flag == 1
    print(2)
else
    print(3)
end_if
-- Prints: 2
```

### Combined
```melp
numeric age = 25
numeric score = 80

if age >= 18 and score >= 60
    print(1)
end_if
-- Prints: 1
```

### Short-circuit in while
```melp
numeric i = 0
numeric limit = 5

while i < limit and i < 3
    i = i + 1
end_while
print(i)
-- Prints: 3
```

---

## ⚠️ CRITICAL RULES

- Boolean keywords are `and`, `or`, `not` — NOT `&&`, `||`, `!`.
- Operator precedence (highest to lowest): `not` > `and` > `or`.
- Use parentheses to override default precedence: `(a or b) and c`.
- Short-circuit evaluation: `and` stops at the first `false`; `or` stops at the first `true`.
- MELP has no `bool` type — conditions evaluate to numeric `0` (false) or non-zero (true).

---

## 🔗 RELATED CARDS

- [IF_STATEMENT.md](IF_STATEMENT.md) — conditional branching
- [ELSE_IF.md](ELSE_IF.md) — else-if chains
- [COMPARISON.md](COMPARISON.md) — comparison operators
- [WHILE_LOOP.md](WHILE_LOOP.md) — while conditions
