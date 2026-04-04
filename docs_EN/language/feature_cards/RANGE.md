# RANGE - Özellik Kartı

**Özellik:** `1..10` / `1..=10` — Aralık literalleri  
**Durum:** 🔮 PLANNED (Stage 1+)  
**Hafta:** Stage 1+  
**Sözdizimi Sörümü:** mlp2

---

## 🎯 ÖZELLİK

`..` (exclusive) ve `..=` (inclusive) aralık operatörleri. `for each` döngüsü, dizi dilimleme ve pattern matching'de kullanılır. Rust tarzı range semantiği.

---

## 📖 SÖZDİZİMİ

```mlp
-- Exclusive range: 1..10 → [1,2,...,9] (10 dahil değil)
loop each i in 1..10
    print(i)        -- 1'den 9'a
end loop

-- Inclusive range: 1..=10 → [1,2,...,10]
loop each i in 1..=10
    print(i)        -- 1'den 10'a
end loop

-- Range değişkeni
Range{numeric} r = 1..=5

-- Dizi dilimleme (planlı)
numeric[] arr = [10; 20; 30; 40; 50]
numeric[] slice = arr[1..3]    -- [20; 30]

-- Pattern matching ile range
match score
    case 90..=100: return "A"
    case 70..90:   return "B"
    case 0..70:    return "C"
end match
```

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ Stage 1+'a kadar ÇALIŞMIYOR
-- Sayma döngüsü için şimdilik loop kullan:
numeric i = 1
loop i <= 10
    print(i)
    i = i + 1
end loop
```

---

## 🧪 GOLDEN TEST

```mlp
-- Stage 1+ sonrası:
numeric function sum_range(numeric start; numeric end_val)
    numeric total = 0
    loop each i in start..=end_val
        total = total + i
    end loop
    return total
end function

function main()
    -- 1+2+3+4+5 = 15
    numeric s = sum_range(1; 5)
    print(s)
end function
-- Output: 15
```

---

**Bağımlılıklar:** for-each, pattern matching, dizi dilimleme  
**Hafta:** Stage 1+  
**Referans Diller:** Rust `..` / `..=`, Python `range()`, Swift `...` / `..<`  
**Kanonik Kaynak:** `mlp2_syntax.md` — Koleksiyonlar ve aralıklar
