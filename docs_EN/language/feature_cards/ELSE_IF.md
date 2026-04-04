# Else-If Chain — MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Conditional branching with else if chaining

---

## 📖 BASIC SYNTAX

```mlp
if condition then
    -- then body
else
    if other_condition then
        -- else-if body
    else
        -- fallback body
    end if
end if
```

---

## ✅ COMPLETE EXAMPLES

### Simple else if
```mlp
numeric x = 5

if x > 10 then
    print(1)
else
    if x > 3 then
        print(2)
    else
        print(3)
    end if
end if
-- Prints: 2
```

### 4-way chain
```mlp
numeric grade = 75

if grade >= 90 then
    print(4)
else
    if grade >= 80 then
        print(3)
    else
        if grade >= 70 then
            print(2)
        else
            if grade >= 60 then
                print(1)
            else
                print(0)
            end if
        end if
    end if
end if
-- Prints: 2
```

### Without else
```mlp
numeric score = 15

if score > 20 then
    print(1)
else
    if score > 10 then
        print(2)
    end if
end if
-- Prints: 2
```

---

## ⚠️ CRITICAL RULES

- Each `else if` introduces an additional condition check.
- Only ONE branch body executes — the first matching condition wins.
- `else if` chains can be arbitrarily long.
- The final `else` is optional.
- All branches share the same terminating `end if`.
- `else if` is written with a space — NOT `else if` (underscore is wrong).

---

## 🔗 RELATED CARDS

- [IF_STATEMENT.md](IF_STATEMENT.md) — basic if/else
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md) — and/or/not in conditions
- [COMPARISON.md](COMPARISON.md) — comparison operators
