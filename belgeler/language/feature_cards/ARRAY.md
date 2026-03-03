# Array — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Collections — fixed-size and dynamic arrays

---

## 📖 BASIC SYNTAX

```melp
array<ElementType> name = [elem1, elem2, ...]
```

---

## ✅ COMPLETE EXAMPLES

### Numeric array
```melp
array<numeric> nums = [1, 2, 3, 4, 5]
print(nums[0])
-- Prints: 1
```

### Array length
```melp
array<numeric> data = [10, 20, 30]
numeric len = data.length
print(len)
-- Prints: 3
```

### Mutating an element
```melp
array<numeric> vals = [1, 2, 3]
vals[1] = 99
print(vals[1])
-- Prints: 99
```

### Iterating with for
```melp
array<numeric> items = [5, 10, 15]
for i = 0 to 2
    print(items[i])
end_for
-- Prints: 5, 10, 15
```

### Push (dynamic array)
```melp
array<numeric> bag = []
bag.push(7)
bag.push(8)
print(bag.length)
-- Prints: 2
```

---

## ⚠️ CRITICAL RULES

- Indices are **0-based**.
- Out-of-bounds access is a runtime error.
- `array<T>` is a generic type; element type `T` must be specified.
- `length` is a read-only property, not a method call.
- `push` appends to the end; `pop` removes and returns the last element.

---

## 🔗 RELATED CARDS

- [COLLECTIONS.md](COLLECTIONS.md) — overview of collection types
- [FOR_LOOP.md](FOR_LOOP.md) — iteration
- [FOR_EACH.md](FOR_EACH.md) — iterate over elements directly
- [GENERICS.md](GENERICS.md) — generic type parameters
