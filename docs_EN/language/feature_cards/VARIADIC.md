# VARIADIC - Özellik Kartı

**Özellik:** `numeric... args` — Değişken sayıda argüman  
**Durum:** ⚠️ PARTIAL (Stage 1 — syntax parse edilir, spread sınırlı)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** mlp1

---

## 🎯 ÖZELLİK

`...` ile son parametreye değişken sayıda argüman alınır. C tarzı `va_args` değil — tip güvenli dizi olarak alınır. `for each` ile iç erişim yapılır.

---

## 📖 SÖZDİZİMİ

```mlp
-- Variadic parametre: son parametre olmalı
numeric function sum(numeric... args)
    numeric total = 0
    loop each n in args
        total = total + n
    end loop
    return total
end function

-- Kullanım: 1, 2 veya daha fazla argüman
numeric s1 = sum(1; 2; 3)           -- 6
numeric s2 = sum(10; 20; 30; 40)    -- 100
numeric s3 = sum(5)                  -- 5

-- Sabit + variadic karışık
void function log(string prefix; string... messages)
    loop each msg in messages
        print(prefix + ": " + msg)
    end loop
end function

log("INFO"; "Başlatıldı"; "Hazır"; "Bekliyor")

-- Variadic ile generic
void function print_all{T}(T... items)
    loop each item in items
        print(item)
    end loop
end function
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `...` | `TOKEN_VARIADIC` — son parametre içinde kullanılır |

**Spread Operatörü (Planlı):**
```mlp
-- Mevcut array'i variadic'e geçirme (planlı)
-- numeric[] nums = [1; 2; 3]
-- numeric s = sum(...nums)    -- ⚠️ HENÜZ ÇALIŞMIYOR
-- Şimdilik: sum(nums[0]; nums[1]; nums[2])
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function max_of(numeric first; numeric... rest)
    numeric m = first
    loop each n in rest
        if n > m then m = n end if
    end loop
    return m
end function

function main()
    numeric result = max_of(3; 1; 7; 2; 5)
    if result == 7 then return 1 end if
end function
```

---

**Bağımlılıklar:** Array (variadic internal olarak array), for-each  
**Hafta:** Stage 1  
**Referans Diller:** C `va_args`, Python `*args`, Go `...T`, TypeScript `...args: T[]`  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Fonksiyon parametreleri
