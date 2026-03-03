# CODE_INTELLIGENCE - Özellik Kartı

**Özellik:** Code Intelligence (analysis, refactoring)  
**Durum:** 🔴 TODO (Phase 4.2)  
**Sözdizimi Sürümü:** N/A (IDE feature)

---

## 🎯 ÖZELLIKLER

### Rename Symbol
```mlp
function oldName() -> numeric  -- Rename to newName
  return 42
end_function

-- All references updated automatically
```

### Extract Function
```mlp
-- Before:
function main() -> numeric
  numeric x = 10
  numeric y = 20
  return x + y
end_function

-- After:
function add(numeric a, numeric b) -> numeric
  return a + b
end_function

function main() -> numeric
  return add(10, 20)
end_function
```

### Code Lens
- Show references count
- Show test coverage
- Show performance hints

---

**Phase:** 4.2 (Month 7)
