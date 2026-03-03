# HIGHER_ORDER_FUNCTIONS - Özellik Kartı

**Özellik:** Higher-Order Functions (map, filter, reduce)  
**Durum:** ✅ COMPLETE (Phase 3 — COLL-2: array_map/filter/reduce + isimli fnptr, 25 Şubat 2026)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Map
```mlp
numeric[] numbers = [1, 2, 3, 4, 5]
numeric[] doubled = numbers.map(lambda(x) -> x * 2)
-- [2, 4, 6, 8, 10]
```

### Filter
```mlp
numeric[] numbers = [1, 2, 3, 4, 5, 6]
numeric[] evens = numbers.filter(lambda(x) -> x % 2 == 0)
-- [2, 4, 6]
```

### Reduce
```mlp
numeric[] numbers = [1, 2, 3, 4, 5]
numeric sum = numbers.reduce(lambda(acc, x) -> acc + x, 0)
-- 15
```

### Chaining
```mlp
numeric[] numbers = [1, 2, 3, 4, 5]
numeric result = numbers
  .filter(lambda(x) -> x % 2 == 0)
  .map(lambda(x) -> x * x)
  .reduce(lambda(acc, x) -> acc + x, 0)
-- 4 + 16 = 20
```

---

## 📝 GRAMER KURALLARI

```ebnf
map_call ::= expr "." "map" "(" lambda_expr ")"

filter_call ::= expr "." "filter" "(" lambda_expr ")"

reduce_call ::= expr "." "reduce" "(" lambda_expr "," expr ")"

method_chain ::= expr ("." method_call)+

higher_order_fn ::= map_call | filter_call | reduce_call
```

---

**Phase:** 1.4 (Month 1)
