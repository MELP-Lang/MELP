# RESULT_OPTION - Özellik Kartı

**Özellik:** Result<T,E> and Option<T> Types  
**Durum:** ✅ COMPLETE (Week 9.1 — 18/18 golden test)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Option<T> Kullanımı
```mlp
enum Option<T>
  Some(T)
  None
end_enum

function find(numeric[] arr, numeric target) -> Option<numeric>
  for numeric i = 0 to arr.length - 1
    if arr[i] == target then
      return Option.Some(i)
    end_if
  end_for
  return Option.None
end_function
```

### Result<T, E> Kullanımı
```mlp
enum Result<T, E>
  Ok(T)
  Err(E)
end_enum

function divide(numeric a, numeric b) -> Result<numeric, character[]>
  if b == 0 then
    return Result.Err("division by zero")
  end_if
  return Result.Ok(a / b)
end_function
```

### Unwrap ve Map
```mlp
numeric opt = Option.Some(42)
numeric value = opt.unwrap()  -- Returns 42 or panics

numeric result = opt.map(lambda(x) -> x * 2)  -- Option.Some(84)
```

---

## 📝 GRAMER KURALLARI

```ebnf
option_type ::= "Option" "<" type ">"

result_type ::= "Result" "<" type "," type ">"

option_init ::= "Option" "." ("Some" "(" expr ")" | "None")

result_init ::= "Result" "." ("Ok" "(" expr ")" | "Err" "(" expr ")")

unwrap_call ::= expr "." "unwrap" "(" ")"

map_call ::= expr "." "map" "(" lambda_expr ")"
```

---

**Phase:** 3.2 (Month 3)
