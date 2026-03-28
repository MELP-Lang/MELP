# Boolean Operations — MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Logic — boolean operators `and`, `or`, `not`

---

## 📖 BASIC SYNTAX

```mlp
condition1 and condition2   -- logical AND
condition1 or condition2    -- logical OR
not condition               -- logical NOT
```

Boolean operators are used in `if`, `loop`, and `else if` conditions.

---

## ✅ COMPLETE EXAMPLES

### Logical AND
```mlp
numeric x = 5
numeric y = 10

if x > 0 and y > 0 then
    print(1)
else
    print(0)
end if
-- Prints: 1
```

### Logical OR
```mlp
numeric a = 0
numeric b = 5

if a > 0 or b > 0 then
    print(1)
else
    print(0)
end if
-- Prints: 1
```

### Logical NOT
```mlp
numeric flag = 0

if not flag == 1 then
    print(2)
else
    print(3)
end if
-- Prints: 2
```

### Combined
```mlp
numeric age = 25
numeric score = 80

if age >= 18 and score >= 60 then
    print(1)
end if
-- Prints: 1
```

### Short-circuit in loop
```mlp
numeric i = 0
numeric limit = 5

loop i < limit and i < 3
    i = i + 1
end loop
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
- [ELSE_IF.md](ELSE_IF.md) — else if chains
- [COMPARISON.md](COMPARISON.md) — comparison operators
- [LOOP_STATEMENT.md](LOOP_STATEMENT.md) — loop conditions
