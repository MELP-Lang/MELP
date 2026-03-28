# CODE_INTELLIGENCE - Özellik Kartı

**Özellik:** Code Intelligence (analysis, refactoring)  
**Durum:** 🔴 TODO (Phase 4.2)  
**Sözdizimi Sürümü:** N/A (IDE feature)

---

## 🎯 ÖZELLIKLER

### Rename Symbol
```mlp
numeric function oldName()  -- Rename to newName
  return 42
end function

-- All references updated automatically
```

### Extract Function
```mlp
-- Before:
function main()
  numeric x = 10
  numeric y = 20
  print(x + y)
end function

-- After:
numeric function add(numeric a; numeric b)
  return a + b
end function

function main()
  print(add(10; 20))
end function
```

### Code Lens
- Show references count
- Show test coverage
- Show performance hints

---

**Phase:** 4.2 (Month 7)
