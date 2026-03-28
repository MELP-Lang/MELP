# FOR_EACH - Özellik Kartı

**Özellik:** `for each item in collection` — Koleksiyon döngüsü  
**Durum:** ✅ COMPLETE (Phase 2 — L-06, 25 Şubat 2026)  
**Hafta:** Stage1 (spec) / Phase 2 (impl)  
**Sözdizimi Sürümü:** mlp0

> ✅ **Gerçek durum (25 Şubat 2026):** `each` ve `in` token’ları + parser + codegen tamamlandı.
> `for each x in nums ... end for` → `mlp_array_length` + `mlp_array_get` ile iterate eder.
> Golden testler: `GOLDEN_TEST_SUITE/for_each/` 5/5 ✅

---

## 🎯 ÖZELLİK

MELP'in tek döngü formu `for each ... in ... end for`'dur. `for i = 0; i < n; i++` tarzı klasik for yoktur. İndeks erişimi için `range` veya `enumerate` kullanılır (planlı).

---

## 📖 SÖZDİZİMİ

```mlp
-- Array üzerinde
numeric[] nums = [1; 2; 3; 4; 5]
loop each n in nums
    print(n)
end loop

-- String listesi
string[] names = ["Ali"; "Veli"; "Ayşe"]
loop each name in names
    print("Merhaba " + name)
end loop

-- Range ile sayma (hedef — Range PLANNED)
-- for each i in 1..10 do ... end for  ← planlı

-- Şimdilik sayma döngüsü için loop:
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop

-- List üzerinde
List{string} items = ("a"; "b"; "c")
loop each item in items
    process(item)
end loop

-- Channel üzerinde (Stage 1.5)
-- for each msg in ch  ... end for

-- İç içe for
numeric[][] matrix = [[1; 2]; [3; 4]]
loop each row in matrix
    loop each cell in row
        print(cell)
    end loop
end loop
```

**Klasik for Karşılaştırması:**
```mlp
-- ❌ MELP'te YOKTUR:
-- for i = 0; i < 10; i++ do ... end for

-- ✅ MELP'te kullan:
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop

-- ✅ Koleksiyon için:
loop each item in collection  ... end for
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function sum_list(numeric[] arr)
    numeric total = 0
    loop each n in arr
        total = total + n
    end loop
    return total
end function

function main()
    numeric[] data = [10; 20; 30; 40]
    numeric result = sum_list(data)
    print(result)
end function
-- Output: 100
```

---

**Bağımlılıklar:** Array, List, Channel; Range (planlı)  
**Hafta:** Stage1  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` — Döngüler bölümü
