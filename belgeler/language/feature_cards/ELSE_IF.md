# Else-If Chain — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Conditional branching with else_if chaining

---

## 📖 BASIC SYNTAX

```melp
if condition
    -- then body
else_if other_condition
    -- else-if body
else
    -- fallback body
end_if
```

---

## ✅ COMPLETE EXAMPLES

### Simple else_if
```melp
numeric x = 5

if x > 10
    print(1)
else_if x > 3
    print(2)
else
    print(3)
end_if
-- Prints: 2
```

### 4-way chain
```melp
numeric grade = 75

if grade >= 90
    print(4)
else_if grade >= 80
    print(3)
else_if grade >= 70
    print(2)
else_if grade >= 60
    print(1)
else
    print(0)
end_if
-- Prints: 2
```

### Without else
```melp
numeric score = 15

if score > 20
    print(1)
else_if score > 10
    print(2)
end_if
-- Prints: 2
```

---

## ⚠️ CRITICAL RULES

- Each `else_if` introduces an additional condition check.
- Only ONE branch body executes — the first matching condition wins.
- `else_if` chains can be arbitrarily long.
- The final `else` is optional.
- All branches share the same terminating `end_if`.
- `else_if` is a single keyword (not `else if` with a space).

---

## 🔗 RELATED CARDS

- [IF_STATEMENT.md](IF_STATEMENT.md) — basic if/else
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md) — and/or/not in conditions
- [COMPARISON.md](COMPARISON.md) — comparison operators
