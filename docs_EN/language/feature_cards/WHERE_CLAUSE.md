# WHERE_CLAUSE - Özellik Kartı

**Özellik:** `where T: Trait` — Generic kısıtlama sözdizimi  
**Durum:** 🔮 PLANNED (Stage 1+ — inline `{T: Trait}` mevcut)  
**Hafta:** Stage 1+  
**Sözdizimi Sürümü:** mlp2

---

## 🎯 ÖZELLİK

Generic kısıtlamaları için ayrı `where` cümlesi. Şu an inline `{T: Interface}` sözdizimi prefix olarak mevcut. `where` cümlesi daha okunabilir imza için planlanmıştır.

---

## 📖 SÖZDİZİMİ

```mlp
-- Mevcut: inline kısıtlama (ŞU AN ÇALIŞAN)
T function max{T: Comparable}(T a; T b)
    if a > b then return a end if
    return b
end function

-- Hedef: where cümlesi (PLANLI — Stage 1+)
T function max{T}(T a; T b) where T: Comparable
    if a > b then return a end if
    return b
end function

-- Çoklu kısıtlama (inline)
string function serialize{T: Printable; T: Serializable}(T val)
    return val.to_string()
end function

-- Çoklu kısıtlama (where — planlı)
string function serialize{T}(T val)
    where T: Printable
    where T: Serializable
    return val.to_string()
end function

-- Struct generic kısıtlama
struct Container{T: Comparable}
    T value
    T min_val
end struct
```

**Inline vs Where Karşılaştırması:**
```mlp
-- Inline (şu an):
T[] function sort{T: Comparable; T: Cloneable}(T[] arr)

-- Where (sonra):
T[] function sort{T}(T[] arr)
    where T: Comparable
    where T: Cloneable
```

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ where cümlesi henüz parse edilmiyor
-- Şimdilik: {T: Interface} inline kullan
void function process{T: Printable}(T item)
    print(item.to_string())
end function
```

---

## 🧪 GOLDEN TEST

```mlp
-- Şu an çalışan inline form:
T function identity{T: Comparable}(T x)
    return x
end function

function main()
    numeric n = identity{numeric}(42)
    if n == 42 then return 1 end if
end function
```

---

**Bağımlılıklar:** Generics, Interface/Trait sistemi  
**Hafta:** Stage 1+ (inline mevcut, `where` planlı)  
**Referans Diller:** Rust `where T: Trait`, Swift `where T: Protocol`, C# `where T : class`  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Generic bölümü; `mlp2_syntax.md` — Trait bölümü
