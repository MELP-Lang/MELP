# BLOCK_TERMINATORS - Özellik Kartı

**Özellik:** `end_X` — Blok kapatma token'ları  
**Durum:** ✅ COMPLETE (Stage1)  
**Hafta:** Stage1  
**Sözdizimi Sürümü:** mlp0

---

## 🎯 ÖZELLİK

MELP'te `{}` parantez yok. Her blok tipi kendi `end_X` token'ıyla kapatılır. Bu kuralın **istisnası yoktur** — her açılan blok kapatılmalıdır. Kural: `else if` iki kelimeden oluşur, aralarında boşluk vardır.

---

## 📖 SÖZDİZİMİ

```mlp
-- Tam end_X listesi:

if <koşul> then
    ...
else if <koşul> then      -- else if: iki kelime, boşlukla yazılır
    ...
else
    ...
end if

loop <koşul>
    ...
end loop

loop each item in collection
    ...
end loop

numeric function name()
    ...
end function

struct Name
    ...
end struct

enum Name
    ...
end enum

match value
    case X: ...
end match

try
    ...
catch e
    ...
end try

lambda(...)
    ...
end lambda

debug
    ...
end debug

implement InterfaceName for TypeName
    ...
end implement

switch value
    case X: ...
    default: ...
end switch
```

**Kritik Kurallar:**
```mlp
-- ✅ DOĞRU: else if (iki kelime, boşlukla)
if x > 10 then
    ...
else
    if x > 5 then
        ...
    end if
end if

-- Bu kullanım her iki blokta doğrudur:
else if x > 5 then   -- DOĞRU

-- ✅ DOĞRU: İç içe — her blok kendi end_X
if a then
    loop each item in list
        if b then
            ...
        end if
    end loop
end if
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function test_terminators(numeric x)
    if x > 10 then
        loop x > 10
            x = x - 1
        end loop
        return 1
    else
        if x > 5 then
            return 2
        else
            return 3
        end if
    end if
end function

function main()
    if test_terminators(15) == 1 then return 1 end if
end function
```

---

**Bağımlılıklar:** Yok (dilbilgisi kuralı)  
**Hafta:** Stage1  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` — Blok yapıları; `melp_syntax.md` — Token tablosu
