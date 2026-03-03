# WHERE_CLAUSE - Özellik Kartı

**Özellik:** `where T: Trait` — Generic kısıtlama sözdizimi  
**Durum:** 🔮 PLANNED (Stage 1+ — inline `{T: Trait}` mevcut)  
**Hafta:** Stage 1+  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

Generic kısıtlamaları için ayrı `where` cümlesi. Şu an inline `{T: Interface}` sözdizimi prefix olarak mevcut. `where` cümlesi daha okunabilir imza için planlanmıştır.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Mevcut: inline kısıtlama (ŞU AN ÇALIŞAN)
function max{T: Comparable}(T a; T b) as T
    if a > b then return a end_if
    return b
end_function

-- Hedef: where cümlesi (PLANLI — Stage 1+)
function max{T}(T a; T b) as T where T: Comparable
    if a > b then return a end_if
    return b
end_function

-- Çoklu kısıtlama (inline)
function serialize{T: Printable; T: Serializable}(T val) as string
    return val.to_string()
end_function

-- Çoklu kısıtlama (where — planlı)
function serialize{T}(T val) as string
    where T: Printable
    where T: Serializable
    return val.to_string()
end_function

-- Struct generic kısıtlama
struct Container{T: Comparable}
    T value
    T min_val
end_struct
```

**Inline vs Where Karşılaştırması:**
```pmlp
-- Inline (şu an):
function sort{T: Comparable; T: Cloneable}(T[] arr) as T[]

-- Where (sonra):
function sort{T}(T[] arr) as T[]
    where T: Comparable
    where T: Cloneable
```

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ where cümlesi henüz parse edilmiyor
-- Şimdilik: {T: Interface} inline kullan
function process{T: Printable}(T item) as void
    print(item.to_string())
end_function
```

---

## 🧪 GOLDEN TEST

```pmlp
-- Şu an çalışan inline form:
function identity{T: Comparable}(T x) as T
    return x
end_function

function main() as numeric
    numeric n = identity{numeric}(42)
    if n == 42 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Generics, Interface/Trait sistemi  
**Hafta:** Stage 1+ (inline mevcut, `where` planlı)  
**Referans Diller:** Rust `where T: Trait`, Swift `where T: Protocol`, C# `where T : class`  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Generic bölümü; `pmlp2_syntax.md` — Trait bölümü
