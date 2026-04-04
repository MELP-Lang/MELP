# LAMBDA — MELP Hızlı Referans

**Özellik:** Anonim fonksiyon (inline expression lambda) ve closure  
**Durum:** ✅ Inline lambda tam çalışıyor | ❌ `end lambda` blok syntax yok  
**Feature Kartı:** `belgeler/language/feature_cards/LAMBDA.md`

---

## Temel Sözdizimi

```mlp
-- Tek parametreli inline lambda
numeric ikile = lambda(x) -> x * 2
println(ikile(7))   -- 14

-- Çift parametreli
numeric topla = lambda(x; y) -> x + y
println(topla(10; 20))   -- 30
```

**Parametre ayırıcısı:** `;` (virgül değil!)  
**Atama tipi:** Her zaman `numeric` (string lambda yok)  
**Çağrı sözdizimi:** `isim(arg)` veya `isim(arg1; arg2)`

---

## Closure (Dış Kapsam Yakalama)

```mlp
function main()
    numeric taban = 5

    -- taban değişkenini yakalar (by-value)
    numeric taban_ekle = lambda(x) -> x + taban

    println(taban_ekle(3))   -- 8
    println(taban_ekle(10))  -- 15
end function
```

Closure, **derleme anında** dış scope değerini kopyalar.

---

## Daha Karmaşık İfadeler

```mlp
function main()
    -- Üs alma lambdası
    numeric us = lambda(x; n) -> x ** n
    println(us(2; 8))   -- 256

    -- Zincirleme
    numeric f = lambda(x) -> x * 2
    numeric g = lambda(x) -> x + 1
    println(f(g(3)))    -- 8  (g(3)=4, f(4)=8)
end function
```

---

## Test Çıktıları (Doğrulanmış)

| Test | Kod | Beklenen | Sonuç |
|------|-----|----------|-------|
| Tek param | `lambda(x) -> x * 2` → `ikile(7)` | 14 | ✅ |
| Çift param | `lambda(x; y) -> x + y` → `topla(10; 20)` | 30 | ✅ |
| Closure | `lambda(x) -> x + taban` (taban=5) → `(3)` | 8 | ✅ |
| Closure | aynı lambda → `(10)` | 15 | ✅ |
| Üs alma | `lambda(x; n) -> x ** n` → `(2; 8)` | 256 | ✅ |

---

## Sınırlamalar

| Özellik | Durum |
|---------|-------|
| `lambda(x) -> expr` (inline) | ✅ Tam çalışıyor |
| `lambda() ... end lambda` (blok) | ❌ Implement edilmemiş |
| Fonksiyon parametresi olarak lambda | ❌ Implement edilmemiş |
| `string` tipine lambda ataması | ❌ Sadece `numeric` destekleniyor |

---

## Lambda Olmadan Benzer Davranış

```mlp
-- Lambda yoksa iç fonksiyon kullan
numeric function ikile(numeric x)
    return x * 2
end function

function main()
    println(ikile(7))   -- 14
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `lambda(x; y) as void ...` (blok) | `lambda(x; y) -> ifade` (inline) |
| `lambda(x, y) -> x + y` (virgül) | `lambda(x; y) -> x + y` |

---

## İlgili YARDIM Dosyaları

- [FUNCTION.md](FUNCTION.md) — Normal fonksiyon tanımı
- [OPERATORS.md](OPERATORS.md) — Lambda içinde kullanılabilir operatörler
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi
