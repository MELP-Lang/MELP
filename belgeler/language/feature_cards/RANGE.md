# RANGE - Özellik Kartı

**Özellik:** `1..10` / `1..=10` — Aralık literalleri  
**Durum:** 🔮 PLANNED (Stage 1+)  
**Hafta:** Stage 1+  
**Sözdizimi Sörümü:** pmlp2

---

## 🎯 ÖZELLİK

`..` (exclusive) ve `..=` (inclusive) aralık operatörleri. `for each` döngüsü, dizi dilimleme ve pattern matching'de kullanılır. Rust tarzı range semantiği.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Exclusive range: 1..10 → [1,2,...,9] (10 dahil değil)
for each i in 1..10
    print(i)        -- 1'den 9'a
end_for

-- Inclusive range: 1..=10 → [1,2,...,10]
for each i in 1..=10
    print(i)        -- 1'den 10'a
end_for

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
end_match
```

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ Stage 1+'a kadar ÇALIŞMIYOR
-- Sayma döngüsü için şimdilik while kullan:
numeric i = 1
while i <= 10
    print(i)
    i = i + 1
end_while
```

---

## 🧪 GOLDEN TEST

```pmlp
-- Stage 1+ sonrası:
function sum_range(numeric start; numeric end_val) as numeric
    numeric total = 0
    for each i in start..=end_val
        total = total + i
    end_for
    return total
end_function

function main() as numeric
    -- 1+2+3+4+5 = 15
    numeric s = sum_range(1; 5)
    if s == 15 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** for-each, pattern matching, dizi dilimleme  
**Hafta:** Stage 1+  
**Referans Diller:** Rust `..` / `..=`, Python `range()`, Swift `...` / `..<`  
**Kanonik Kaynak:** `pmlp2_syntax.md` — Koleksiyonlar ve aralıklar
