# ERROR_PROPAGATION - Özellik Kartı

**Özellik:** Error Propagation (`?` operator)  
**Durum:** 🔴 TODO (Phase 3.3)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Rust-like `?` Operator
```mlp
function readFile(character[] path) -> Result<character[], character[]>
  numeric file = openFile(path)?  -- Early return if Err
  numeric content = file.read()?
  file.close()
  return Result.Ok(content)
end_function
```

### Manuel Error Propagation
```mlp
function process() -> Result<numeric, character[]>
  numeric r1 = step1()
  match r1
    case Ok(x) then numeric val = x
    case Err(e) then return Result.Err(e)
  end_match
  
  numeric r2 = step2(val)
  match r2
    case Ok(y) then return Result.Ok(y)
    case Err(e) then return Result.Err(e)
  end_match
end_function
```

### Try-Catch Benzeri
```mlp
function main() -> numeric
  try
    numeric x = riskyOperation()
    return x
  catch e
    print("Error: " + e)
    return 0
  end_try
end_function
```

---

## 📝 GRAMER KURALLARI

```ebnf
try_expr ::= "try" expr ("?" | "!")

propagate_op ::= expr "?"

unwrap_op ::= expr "!"

try_block ::= "try" stmt_list "catch" identifier stmt_list "end_try"
```

---

**Phase:** 3.3 (Month 3)
