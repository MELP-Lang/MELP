# NULLABLE - Özellik Kartı

**Özellik:** `numeric?` — Nullable tipler ve null güvenliği  
**Durum:** ⚠️ PARTIAL (Stage 1 — temel nullable var; `??` ve `?.` eksik)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

Herhangi bir tipe `?` ekleyerek nullable yapılır: `numeric? x`. `null` değer atanabilir. Null coalescing `??` ve safe navigation `?.` henüz implement edilmedi; şimdilik `if x != null then` kullanın.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Nullable tip bildirimi
numeric? n = null
string? name = null
boolean? flag = true

-- Null kontrolü (ŞU AN geçerli yol)
if name != null then
    print(name)
end_if

-- Null atama
n = 42
n = null          -- tekrar nulllanabilir

-- Null coalescing (hedef sözdizimi — henüz eklenmedi)
-- numeric val = n ?? 0       -- ⚠️ HENÜZ ÇALIŞMIYOR

-- Safe navigation (hedef sözdizimi — henüz eklenmedi)
-- numeric len = name?.length  -- ⚠️ HENÜZ ÇALIŞMIYOR

-- Şimdilik kullan:
numeric val = 0
if n != null then val = n end_if

numeric len = 0
if name != null then len = string_length(name) end_if
```

**Option{T} ile İlişki:**
```pmlp
-- nullable T? ile Option{T} semantik olarak eşdeğer
-- T? → dil seviyesi null
-- Option{T} → stdlib enum (Some/None)  ← tercih edilen yol

-- Option{T} daha tip güvenli:
Option{numeric} safe_n = Some(42)
match safe_n
    case Some(v): print(v)
    case None: print("yok")
end_match
```

---

## 🧪 GOLDEN TEST

```pmlp
function find_age(string name) as numeric?
    if name == "Ali" then return 25 end_if
    return null
end_function

function main() as numeric
    numeric? age = find_age("Ali")
    if age != null then
        if age == 25 then return 1 end_if
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Option{T} (tercih edilir), null anahtar kelimesi  
**Eksik:** `??` null coalesce, `?.` safe navigation  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Tip sistemi; `melp_syntax.md` — TOKEN_QUESTION
