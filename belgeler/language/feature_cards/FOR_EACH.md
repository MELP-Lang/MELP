# FOR_EACH - Özellik Kartı

**Özellik:** `for each item in collection` — Koleksiyon döngüsü  
**Durum:** ✅ COMPLETE (Phase 2 — L-06, 25 Şubat 2026)  
**Hafta:** Stage 0 (spec) / Phase 2 (impl)  
**Sözdizimi Sürümü:** pmlp0

> ✅ **Gerçek durum (25 Şubat 2026):** `each` ve `in` token’ları + parser + codegen tamamlandı.
> `for each x in nums ... end_for` → `mlp_array_length` + `mlp_array_get` ile iterate eder.
> Golden testler: `GOLDEN_TEST_SUITE/for_each/` 5/5 ✅

---

## 🎯 ÖZELLİK

MELP'in tek döngü formu `for each ... in ... end_for`'dur. `for i = 0; i < n; i++` tarzı klasik for yoktur. İndeks erişimi için `range` veya `enumerate` kullanılır (planlı).

---

## 📖 SÖZDİZİMİ

```pmlp
-- Array üzerinde
numeric[] nums = [1; 2; 3; 4; 5]
for each n in nums
    print(n)
end_for

-- String listesi
string[] names = ["Ali"; "Veli"; "Ayşe"]
for each name in names
    print("Merhaba " + name)
end_for

-- Range ile sayma (hedef — Range PLANNED)
-- for each i in 1..10 do ... end_for  ← planlı

-- Şimdilik sayma döngüsü için while:
numeric i = 0
while i < 10
    print(i)
    i = i + 1
end_while

-- List üzerinde
List{string} items = ("a"; "b"; "c")
for each item in items
    process(item)
end_for

-- Channel üzerinde (Stage 1.5)
-- for each msg in ch  ... end_for

-- İç içe for
numeric[][] matrix = [[1; 2]; [3; 4]]
for each row in matrix
    for each cell in row
        print(cell)
    end_for
end_for
```

**Klasik for Karşılaştırması:**
```pmlp
-- ❌ MELP'te YOKTUR:
-- for i = 0; i < 10; i++ do ... end_for

-- ✅ MELP'te kullan:
numeric i = 0
while i < 10
    print(i)
    i = i + 1
end_while

-- ✅ Koleksiyon için:
for each item in collection  ... end_for
```

---

## 🧪 GOLDEN TEST

```pmlp
function sum_list(numeric[] arr) as numeric
    numeric total = 0
    for each n in arr
        total = total + n
    end_for
    return total
end_function

function main() as numeric
    numeric[] data = [10; 20; 30; 40]
    numeric result = sum_list(data)
    if result == 100 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Array, List, Channel; Range (planlı)  
**Hafta:** Stage 0  
**Kanonik Kaynak:** `pmlp0_sozdizimi.md` — Döngüler bölümü
