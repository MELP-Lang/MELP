# COLLECTIONS - Özellik Kartı

**Özellik:** Array `[]`, List `()`, Tuple `<>` — Koleksiyon tipleri  
**Durum:** ✅ COMPLETE (Stage 0 — Array ve List; Tuple partial)  
**Hafta:** Stage 0  
**Sözdizimi Sürümü:** pmlp0/pmlp1

---

## 🎯 ÖZELLİK

MELP'te üç yerleşik koleksiyon tipi vardır. Ayırıcı **daima `;`** — asla `,` kullanma. HashMap Stage 1+ olarak planlanmıştır.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Array: sabit tip, indeksli erişim
numeric[] arr = [1; 2; 3; 4; 5]
numeric first = arr[0]
numeric last = arr[4]
numeric len = length(arr)

-- Boş array
string[] names = []

-- 2D array
numeric[][] matrix = [[1; 2]; [3; 4]]

-- List: dinamik, sıralı
List{string} words = ("merhaba"; "dünya")
words.add("!")
string head = words[0]

-- Tuple: heterojen, sabit uzunluk
Tuple{numeric; string} pair = <42; "Ali">
numeric id = pair.0
string name = pair.1

-- Tuple ile çok dönüş değeri
function get_coords() as Tuple{numeric; numeric}
    return <10; 20>
end_function

Tuple{numeric; numeric} pos = get_coords()
print(pos.0)   -- 10
print(pos.1)   -- 20
```

**Ayırıcı Kuralı (KRİTİK):**
```pmlp
-- ✅ DOĞRU: Noktalı virgül
[1; 2; 3]
("a"; "b"; "c")
<1; "Ali">

-- ❌ YANLIŞ: Virgül KULLANMA
[1, 2, 3]     -- HATA
("a", "b")    -- HATA
```

**HashMap (Planlı — Stage 1+):**
```pmlp
-- ⚠️ Henüz yok — tahmini sözdizimi:
-- HashMap{string; numeric} scores = {"Ali": 95; "Veli": 87}
```

---

## 🧪 GOLDEN TEST

```pmlp
function sum(numeric[] arr) as numeric
    numeric total = 0
    for each n in arr
        total = total + n
    end_for
    return total
end_function

function main() as numeric
    numeric[] nums = [10; 20; 30]
    numeric s = sum(nums)
    if s == 60 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** for-each, length(), generics  
**Hafta:** Stage 0 (Array/List), Stage 1+ (HashMap)  
**Kanonik Kaynak:** `pmlp0_sozdizimi.md` — Koleksiyonlar; `pmlp1_sozdizimi.md` — Tuple
