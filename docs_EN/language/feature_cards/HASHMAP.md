# HashMap — MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
hashmap<KeyType; ValueType> name = {}
name[key] = value
ValueType val = name[key]
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Create and Populate
```mlp
hashmap<string; numeric> scores = {}
scores["alice"] = 95
scores["bob"] = 87
print(scores["alice"])
-- Output: 95
```

### Example 2 — Check Membership
```mlp
hashmap<string; numeric> map = {}
map["x"] = 1
if map.has("x") then
    print(1)
else
    print(0)
end if
-- Output: 1
```

### Example 3 — Delete a Key
```mlp
hashmap<string; numeric> h = {}
h["a"] = 10
h.delete("a")
print(h.length)
-- Output: 0
```

### Example 4 — In a Function
```mlp
function main()
    hashmap<string; numeric> counts = {}
    counts["hello"] = 1
    counts["world"] = 2
    print(counts["hello"] + counts["world"])
end function
-- Output: 3
```

---

## ⚠️ CRITICAL RULES

1. Both key and value types must be specified: `hashmap<K; V>` (semicolon separator).
2. Accessing a missing key returns zero value (`0` for numeric, `""` for string).
3. Use `has(key)` to test existence before access.
4. `length` returns the number of key-value pairs.
5. Key type is typically `string` or `numeric`.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: comma in type parameters
hashmap<string, numeric> scores = {}
```

```mlp
-- ✅ CORRECT: semicolon in type parameters
hashmap<string; numeric> scores = {}
```

---

## 🔗 RELATED CARDS

- [COLLECTIONS.md](COLLECTIONS.md) — collection type overview
- [ARRAY.md](ARRAY.md) — ordered collections
- [FOR_EACH.md](FOR_EACH.md) — iterate over collections
