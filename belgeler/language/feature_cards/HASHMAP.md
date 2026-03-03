# HashMap — MELP Syntax

**Durum:** ✅ COMPLETE (Phase 3 — COLL-1: hashmap<K;V> codegen + hashmap_set/get/has/delete/size, 25 Şubat 2026)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Collections — key-value hash maps

---

## 📖 BASIC SYNTAX

```melp
hashmap<KeyType, ValueType> name = {}
name[key] = value
numeric val = name[key]
```

---

## ✅ COMPLETE EXAMPLES

### Create and populate
```melp
hashmap<string, numeric> scores = {}
scores["alice"] = 95
scores["bob"] = 87
print(scores["alice"])
-- Prints: 95
```

### Check membership
```melp
hashmap<string, numeric> map = {}
map["x"] = 1
if map.has("x")
    print(1)
else
    print(0)
end_if
-- Prints: 1
```

### Delete a key
```melp
hashmap<string, numeric> h = {}
h["a"] = 10
h.delete("a")
print(h.length)
-- Prints: 0
```

### Iterate over keys
```melp
hashmap<string, numeric> data = {}
data["one"] = 1
data["two"] = 2
for_each key in data.keys()
    print(data[key])
end_for_each
```

---

## ⚠️ CRITICAL RULES

- Both key and value types must be specified: `hashmap<K, V>`.
- Accessing a missing key returns the zero value for the value type (`0` for numeric, `""` for string).
- Use `has(key)` to test existence before access.
- `length` returns the number of key-value pairs.
- Key type is typically `string` or `numeric`.

---

## 🔗 RELATED CARDS

- [COLLECTIONS.md](COLLECTIONS.md) — collection type overview
- [ARRAY.md](ARRAY.md) — ordered collections
- [FOR_EACH.md](FOR_EACH.md) — iterate over collections
- [GENERICS.md](GENERICS.md) — generic type parameters
