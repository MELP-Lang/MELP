# CLOSURE - Özellik Kartı

**Özellik:** Closure Mechanism  
**Durum:** 🔴 TODO (Phase 1.2)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Temel Closure (Tek Değişken Capture)
```mlp
function makeAdder(numeric x)
  return lambda(y) -> x + y
end_function

numeric add5 = makeAdder(5)
numeric result = add5(10)  -- 15
```

### Closure (Mutable Capture)
```mlp
function makeCounter()
  numeric count = 0
  return lambda()
    count = count + 1
    return count
  end_lambda
end_function

numeric counter = makeCounter()
print(counter())  -- 1
print(counter())  -- 2
print(counter())  -- 3
```

### Closure (Çoklu Capture)
```mlp
function makeCalculator(numeric a, numeric b)
  return lambda(op)
    if op == "+" then
      return a + b
    else if op == "*" then
      return a * b
    end_if
  end_lambda
end_function

numeric calc = makeCalculator(10, 5)
print(calc("+"))  -- 15
print(calc("*"))  -- 50
```

### İç İçe Closure
```mlp
function outer(numeric x)
  return lambda(y)
    return lambda(z) -> x + y + z
  end_lambda
end_function

numeric f = outer(1)(2)
print(f(3))  -- 6
```

---

## 📝 GRAMER KURALLARI

```ebnf
closure ::= lambda_expr with captured variables

capture ::= variable_reference from outer scope

lambda_expr ::= "lambda" "(" param_list? ")" "->" expr
              | "lambda" "(" param_list? ")" stmt_list "end_lambda"

capture_list ::= identifier ("," identifier)*
```

---

## ✅ TEST SENARYOLARI (Zorunlu)

Minimum 5 POC test:
1. Mutlu yol (happy path)
2. Sınır değeri
3. Hatalı girdi
4. İç içe kullanım
5. Entegrasyon senaryosu

---

## 🔧 UYGULAMA DOSYALARI

- `compiler/stage1/closure/` modülü
- `tests/` altında birim testleri
- `GOLDEN_TEST_SUITE/closure/` POC testleri

---

**Not:** Bu kart yalnızca sözdizimi referansı içerir.
