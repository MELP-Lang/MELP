# BLOCK_TERMINATORS - Özellik Kartı

**Özellik:** `end_X` — Blok kapatma token'ları  
**Durum:** ✅ COMPLETE (Stage 0)  
**Hafta:** Stage 0  
**Sözdizimi Sürümü:** pmlp0

---

## 🎯 ÖZELLİK

MELP'te `{}` parantez yok. Her blok tipi kendi `end_X` token'ıyla kapatılır. Bu kuralın **istisnası yoktur** — her açılan blok kapatılmalıdır. Tek-token kural: `else_if` (tek token, boşluk olmadan).

---

## 📖 SÖZDİZİMİ

```pmlp
-- Tam end_X listesi:

if <koşul> then
    ...
else_if <koşul> then      -- TEK TOKEN: else_if (boşluk yok)
    ...
else
    ...
end_if

while <koşul>
    ...
end_while

for each item in collection
    ...
end_for

function name() as type
    ...
end_function

struct Name
    ...
end_struct

enum Name
    ...
end_enum

match value
    case X: ...
end_match

try
    ...
catch e
    ...
end_try

lambda(...) as type
    ...
end_lambda

debug
    ...
end_debug

implement InterfaceName for TypeName
    ...
end_implement

switch value
    case X: ...
    default: ...
end_switch
```

**Kritik Kurallar:**
```pmlp
-- ✅ DOĞRU: else_if tek token
if x > 10 then
    ...
else_if x > 5 then
    ...
end_if

-- ❌ YANLIŞ: else if iki ayrı token
else if x > 5 then   -- HATA

-- ✅ DOĞRU: İç içe — her blok kendi end_X
if a then
    for each item in list
        if b then
            ...
        end_if
    end_for
end_if
```

---

## 🧪 GOLDEN TEST

```pmlp
function test_terminators(numeric x) as numeric
    if x > 10 then
        while x > 10
            x = x - 1
        end_while
        return 1
    else_if x > 5 then
        return 2
    else
        return 3
    end_if
end_function

function main() as numeric
    if test_terminators(15) == 1 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Yok (dilbilgisi kuralı)  
**Hafta:** Stage 0  
**Kanonik Kaynak:** `pmlp0_sozdizimi.md` — Blok yapıları; `melp_syntax.md` — Token tablosu
