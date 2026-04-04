# MELP Yüksek Dereceli Fonksiyonlar (Higher-Order Functions) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⏳ Stage 1 (kısmi destek)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [map / filter / reduce](#map--filter--reduce)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Fonksiyon parametre olarak alır:
numeric function uygula(funcref<(numeric) -> numeric> fn; numeric x)
    return fn(x)
end function

-- Fonksiyon döndürür:
funcref<(numeric) -> numeric> function make_adder(numeric x)
    return lambda(numeric y) -> y + x
end function
```

---

## Kullanım Örnekleri

### Örnek 1: Fonksiyon argüman olarak geçirme
```mlp
numeric function kare(numeric n)
    return n * n
end function

numeric function uygula(funcref<(numeric) -> numeric> fn; numeric x)
    return fn(x)
end function

print(uygula(kare; 5))
-- Çıktı: 25
```

### Örnek 2: Lambda ile HOF
```mlp
function uygula_tum(numeric[] arr; funcref<(numeric) -> void> fn)
    loop each n in arr
        fn(n)
    end loop
end function

numeric[] data = [1; 2; 3]
uygula_tum(data; lambda(numeric x) -> print(x * 2))
-- Çıktı: 2 4 6
```

### Örnek 3: Pipeline (birleşik uygulama)
```mlp
boolean function cift_mi(numeric n)
    return n % 2 == 0
end function

numeric[] function filtrele(numeric[] arr; funcref<(numeric) -> boolean> test)
    -- Stage1'de implemente edilecek
    return arr
end function
```

---

## map / filter / reduce

```mlp
-- map: her elemana fonksiyon uygula
-- arr.map(fn)  → Stage2

-- filter: koşula göre ele
-- arr.filter(fn)  → Stage2

-- reduce: birikimli hesaplama
-- arr.reduce(fn; initial)  → Stage2
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler (Stage 1)
- Fonksiyon başka bir fonksiyona argüman olarak geçirilebilir
- Lambda ile HOF kombinasyonu
- `funcref<>` tip imzası

### ❌ Yasaklar
- Yerleşik `map`/`filter`/`reduce` — Stage 2
- Partial application — Stage 2

---

## İlgili Özellikler

- **LAMBDA** — Anonim fonksiyonlar
- **FUNCREF** — Fonksiyon referansları
- **CLOSURE** — Closure semantiği

**Ayrıca bakınız:**
- [LAMBDA.md](LAMBDA.md)
- [FUNCREF.md](FUNCREF.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `function f() as void` | `function f()` |
| `end_function` | `end function` |
| `end_for` | `end for` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| HOF (argüman olarak fn) | ❌ | ✅ | ✅ |
| HOF (döndürülen fn) | ❌ | ✅ | ✅ |
| `map` / `filter` / `reduce` | ❌ | ❌ | ✅ |
| Partial application | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/HIGHER_ORDER_FUNCTIONS.md](../belgeler/language/feature_cards/HIGHER_ORDER_FUNCTIONS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/higher_order/](../GOLDEN_TEST_SUITE/higher_order/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
