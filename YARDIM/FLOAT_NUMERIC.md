# MELP Sayısal Tip Davranışı (Float/Numeric) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ Belgelenmiş Davranış — Tasarım kararı bekliyor

---

## İçindekiler

1. [Temel Bilgi](#temel-bilgi)
2. [Mevcut Davranış](#mevcut-davranış)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Bilinen Kısıtlamalar](#bilinen-kısıtlamalar)
5. [Durum Tablosu](#durum-tablosu)

---

## Temel Bilgi

MELP Stage 0'da tek sayısal tip vardır: **`numeric`**

```
numeric  ←→  LLVM i64 (64-bit işaretli tamsayı)
```

**Float/double desteği YOKTUR** — Stage 0 kapsamı dışıdır.

---

## Mevcut Davranış

```mlp
-- Tamsayı değerler:
numeric x = 42       -- ✅ 64-bit signed int
numeric y = -17      -- ✅ negatif
numeric z = 0        -- ✅ sıfır

-- Float literal:
numeric f = 3        -- ⚠️ 3.14 yazarsanız sessizce 3'e truncate edilir

-- Bölme:
numeric result = 10 / 3    -- 3 (tamsayı bölme — virgülsüz)

-- Kalan:
numeric mod_result = 10 mod 3    -- 1

-- Aralık:
-- numeric: -9,223,372,036,854,775,808 .. 9,223,372,036,854,775,807
```

---

## Kullanım Örnekleri

### Örnek 1: Temel Aritmetik
```mlp
function main()
    numeric a = 10
    numeric b = 3

    print(a + b)     -- 13
    print(a - b)     -- 7
    print(a * b)     -- 30
    print(a / b)     -- 3  (tamsayı bölme)
    print(a mod b)   -- 1  (kalan)
end function
```

### Örnek 2: Büyük Sayılar
```mlp
function main()
    numeric buyuk = 1000000 * 1000000
    print(buyuk)    -- 1000000000000
end function
```

### Örnek 3: Negatif Sayılar
```mlp
function main()
    numeric negatif = -42
    numeric mutlak = negatif * -1
    print(mutlak)   -- 42
end function
```

---

## Bilinen Kısıtlamalar

| Kısıtlama | Durum | Neden |
|-----------|-------|-------|
| Float/double yok | Stage 0 tasarım kararı | `numeric = i64` |
| `3,14` yazılsa `3` okunur | Sessiz truncation | Lexer float literal tanımaz |
| `10 / 3 = 3` | Tamsayı bölme | i64 aritmetiği |
| `sqrt()` → double → i64 | Truncation | FFI dönüşümü |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |
| `3.14` (float literal) | `3` (Stage 0'da float yok) |
| `10 / 3 == 3.33` beklentisi | `10 / 3 == 3` (tamsayı bölme) |

---

## İlgili Özellikler

- **BASIC_TYPES** — Temel tipler genel bakış
- **OPERATORS** — Aritmetik operatörler

**Ayrıca bakınız:**
- [BASIC_TYPES.md](BASIC_TYPES.md)
- [OPERATORS.md](OPERATORS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `numeric` (i64) | ✅ | ✅ | ✅ |
| Float (`f64`) | ❌ | ❌ | ✅ Planlı |
| Float literal `3,14` | ❌ | ❌ | ✅ |
| Tamsayı bölme | ✅ | ✅ | ✅ |
| Float bölme | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FLOAT_NUMERIC.md](../belgeler/language/feature_cards/FLOAT_NUMERIC.md)
- **İlgili Bug:** `TODO_BUG_FIXES/TODO_BUG_FIXES.md#G-14`
