# LAMBDA - Özellik Kartı

**Özellik:** Lambda Expressions & Closures — Anonim fonksiyonlar, closure desteği
**Durum:** ✅ inline lambda çalışıyor | ❌ end_lambda block syntax implement edilmemiş
**Sözdizimi Sürümü:** mlp (güncel)

---

## ⚠️ ÖNEMLİ: DOĞRU SYNTAX

Bu card'daki `end_lambda` ve `function add = lambda(...) as numeric` syntax'ı **ÇALIŞMIYOR**.

### DOĞRU (çalışan) syntax:

```mlp
-- Lambda inline expression: lambda(params) -> expr
numeric add = lambda(x; y) -> x + y
numeric result = add(10; 20)  -- 30

-- Closure: dış değişkeni yakalar
numeric multiplier = 3
numeric mult = lambda(x) -> x * multiplier
print(mult(5))  -- 15
```

### YANLIŞ (implement edilmemiş) syntax:
```
-- ❌ ÇALIŞMIYOR:
function add = lambda(numeric a; numeric b) as numeric
    return a + b
end_lambda
```

**Kanıt:** `GOLDEN_TEST_SUITE/lambda_tests/test_01_basic_lambda.mlp` ve `test_06_closure.mlp` çalışıyor.

---

## 🎯 ÖZELLİK

Lambda, `numeric` değişkene atanan inline anonim ifadelerdir (`lambda(params) -> expr`).
Dış scope'taki değişkenleri by-value yakalayan closure'lar oluşturabilir.
Parametre ayırıcı: `;`

---

## 📖 SÖZDİZİMİ

### Basit Lambda
```mlp
function main() as numeric
    numeric add = lambda(x; y) -> x + y
    numeric result = add(10; 20)
    print(result)  -- 30
    return 0
end_function
```

### Closure (Dış Scope Yakalama)
```mlp
function main() as numeric
    numeric multiplier = 3
    numeric mult = lambda(x) -> x * multiplier
    print(mult(5))   -- 15
    print(mult(10))  -- 30
    return 0
end_function
```

### Tek Parametreli Lambda
```mlp
function main() as numeric
    numeric double = lambda(x) -> x * 2
    print(double(7))  -- 14
    return 0
end_function
```

### ❌ PLANLANMIŞ AMA ÇALIŞMIYOR
```
-- Block lambda (end_lambda) — implement edilmemiş:
function add = lambda(numeric a; numeric b) as numeric
    return a + b
end_lambda

-- Fonksiyon parametresi olarak lambda — implement edilmemiş:
function apply(numeric x; function(numeric) as numeric op) as numeric
    return op(x)
end_function
```

---

## 🧪 GOLDEN TEST

```mlp
-- test_lambda_basic.mlp
function main() as numeric
    -- 1. Basit lambda
    numeric double = lambda(x) -> x * 2

    -- 2. Closure
    numeric base = 5
    numeric add_base = lambda(x) -> x + base

    if double(5) == 10 then
        print(1)
    end_if
    return 0
end_function
```

**Beklenen çıktı:** `1`

---

**Impl:** `compiler/stage1/modules/lambda/`
**Çalışan test'ler:** `GOLDEN_TEST_SUITE/lambda_tests/test_0[1-6]*.mlp`
