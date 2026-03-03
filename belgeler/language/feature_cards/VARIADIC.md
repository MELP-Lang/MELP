# VARIADIC - Özellik Kartı

**Özellik:** `numeric... args` — Değişken sayıda argüman  
**Durum:** ⚠️ PARTIAL (Stage 1 — syntax parse edilir, spread sınırlı)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

`...` ile son parametreye değişken sayıda argüman alınır. C tarzı `va_args` değil — tip güvenli dizi olarak alınır. `for each` ile iç erişim yapılır.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Variadic parametre: son parametre olmalı
function sum(numeric... args) as numeric
    numeric total = 0
    for each n in args
        total = total + n
    end_for
    return total
end_function

-- Kullanım: 1, 2 veya daha fazla argüman
numeric s1 = sum(1; 2; 3)           -- 6
numeric s2 = sum(10; 20; 30; 40)    -- 100
numeric s3 = sum(5)                  -- 5

-- Sabit + variadic karışık
function log(string prefix; string... messages) as void
    for each msg in messages
        print(prefix + ": " + msg)
    end_for
end_function

log("INFO"; "Başlatıldı"; "Hazır"; "Bekliyor")

-- Variadic ile generic
function print_all{T}(T... items) as void
    for each item in items
        print(item)
    end_for
end_function
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `...` | `TOKEN_VARIADIC` — son parametre içinde kullanılır |

**Spread Operatörü (Planlı):**
```pmlp
-- Mevcut array'i variadic'e geçirme (planlı)
-- numeric[] nums = [1; 2; 3]
-- numeric s = sum(...nums)    -- ⚠️ HENÜZ ÇALIŞMIYOR
-- Şimdilik: sum(nums[0]; nums[1]; nums[2])
```

---

## 🧪 GOLDEN TEST

```pmlp
function max_of(numeric first; numeric... rest) as numeric
    numeric m = first
    for each n in rest
        if n > m then m = n end_if
    end_for
    return m
end_function

function main() as numeric
    numeric result = max_of(3; 1; 7; 2; 5)
    if result == 7 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Array (variadic internal olarak array), for-each  
**Hafta:** Stage 1  
**Referans Diller:** C `va_args`, Python `*args`, Go `...T`, TypeScript `...args: T[]`  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Fonksiyon parametreleri
