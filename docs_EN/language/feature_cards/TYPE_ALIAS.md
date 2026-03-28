# Type Alias — MELP Syntax

**Durum:** 🔴 TODO (Phase 3 — sözdizimi belirsiz)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Type system — type aliasing

---

## 📖 BASIC SYNTAX

```mlp
type AliasName = ExistingType
```

A type alias creates an alternative name for an existing type. It does not create a new type — `AliasName` and `ExistingType` are interchangeable.

---

## ✅ COMPLETE EXAMPLES

### Numeric alias
```mlp
type Score = numeric
type Count = numeric

Score points = 100
Count items = 5
print(points)
-- Prints: 100
```

### Alias in function signature
```mlp
type Meters = numeric

Meters function distance(Meters a; Meters b)
    return a + b
end function

function main()
    numeric d = distance(3; 4)
    print(d)
    -- Output: 7
end function
```

### Alias clarity
```mlp
type UserId = numeric
type ProductId = numeric

UserId user = 42
ProductId prod = 99
```

---

## ⚠️ CRITICAL RULES

- `type` is a keyword; alias names must be `PascalCase` by convention.
- Aliases are resolved at compile time — no runtime overhead.
- Recursive or circular aliases are not permitted.
- Currently supported base types: `numeric`, `string`, named `enum` types.

---

---

## 🧪 GOLDEN TEST

```mlp
type Score = numeric
type Label = string

Score function scale(Score s)
    return s * 2
end function

function main()
    Score s = 21
    numeric result = scale(s)
    if result == 42 then
        print("pass")
        -- Output: pass
    end if
end function
```

---

**Bağımlılıklar:** BASIC_TYPES, VARIABLE  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Tip sistemi

## 🔗 RELATED CARDS

- [BASIC_TYPES.md](BASIC_TYPES.md) — built-in types
- [VARIABLE.md](VARIABLE.md) — variable declarations
- [ENUM.md](ENUM.md) — enum types
