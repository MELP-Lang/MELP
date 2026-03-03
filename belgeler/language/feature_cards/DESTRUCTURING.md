# DESTRUCTURING - Özellik Kartı

**Özellik:** Destructuring Assignment — Tuple/struct değerlerini tek satırda açma  
**Durum:** 📅 PLANNED (Week 6.4, Nisan 2026)  
**Hafta:** Week 6.4  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

Tuple veya çoklu dönüş değerlerini tek atama ifadesiyle ayrı değişkenlere bağlar. Mevcut durumda `dims[0]`, `dims[1]` gibi index tabanlı erişim gerekiyor — bu hatalara açık. Destructuring bunu `x; y = get_coords()` ile çözer.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Tuple destructuring (temel)
function get_coords() as tuple
    return <10; 20; 30>
end_function

numeric x; numeric y; numeric z = get_coords()
-- x=10, y=20, z=30

-- Birden fazla dönüş değeri (aslında tuple)
function min_max(numeric[] arr) as tuple
    return <arr[0]; arr[arr.length - 1]>
end_function

numeric lo; numeric hi = min_max([1; 5; 3; 8; 2])
-- lo=1, hi=8 (sorted)

-- _ ile istenmeyen değerleri atla
numeric _; numeric hi2 = min_max([1; 5; 3])
-- sadece hi2 kullanılır

-- Struct alanlarını aç (planlandı)
-- struct Point { numeric x; numeric y }
-- numeric px; numeric py = point
```

**Mevcut Durum (Week 6.4'e kadar) — YZ DİKKAT:**
```pmlp
-- ⚠️ Henüz ÇALIŞMIYOR — index kullan:
tuple coords = get_coords()
numeric x = coords[0]
numeric y = coords[1]
numeric z = coords[2]
```

---

## 🧪 GOLDEN TEST

```pmlp
function swap(numeric a; numeric b) as tuple
    return <b; a>
end_function

function main() as numeric
    -- Week 6.4 sonrası:
    -- numeric x; numeric y = swap(1; 2)
    -- if x == 2 and y == 1 then return 1 end_if
    -- Şu an:
    tuple result = swap(1; 2)
    if result[0] == 2 and result[1] == 1 then
        return 1  -- PASS
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Tuple (Stage 0'da mevcut)  
**Hafta:** Week 6.4 (Nisan 2026)  
**Referans Diller:** Python `a, b = fn()`, Rust `let (x, y) = tuple`, JS `const {x, y} = obj`
