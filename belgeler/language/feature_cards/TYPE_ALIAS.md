# Type Alias — MELP Syntax

**Durum:** 🔴 TODO (Phase 3 — sözdizimi belirsiz)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Type system — type aliasing

---

## 📖 BASIC SYNTAX

```melp
type AliasName = ExistingType
```

A type alias creates an alternative name for an existing type. It does not create a new type — `AliasName` and `ExistingType` are interchangeable.

---

## ✅ COMPLETE EXAMPLES

### Numeric alias
```melp
type Score = numeric
type Count = numeric

Score points = 100
Count items = 5
print(points)
-- Prints: 100
```

### Alias in function signature
```melp
type Meters = numeric

function distance(a: Meters, b: Meters) -> Meters
    return a + b
end_function

numeric d = distance(3, 4)
print(d)
-- Prints: 7
```

### Alias clarity
```melp
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

## 🔗 RELATED CARDS

- [BASIC_TYPES.md](BASIC_TYPES.md) — built-in types
- [VARIABLE.md](VARIABLE.md) — variable declarations
- [ENUM.md](ENUM.md) — enum types
