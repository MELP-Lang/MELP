# BLOCK_TERMINATORS — Ozellik Karti

**Ozellik:** `end_X` — Blok kapatma token'lari  
**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp0

---

## 🎯 OZELLIK

MELP'te `{}` parantez yoktur. Her blok tipi kendi `end X` token'iyla kapatilir. **Istisna yoktur** — her acilan blok kapatilmalidir. Kural: `else if` iki kelimeden olusur, aralarinda bosluk vardir.

---

## 📖 SOZDIZIMI — TAM end_X LISTESI

```mlp
if <kosul> then
    ...
else if <kosul> then      -- else if: iki kelime, boslukla yazilir
    ...
else
    ...
end if

loop <kosul>
    ...
end loop

loop each eleman in koleksiyon
    ...
end loop

numeric function ad()
    ...
end function

struct Ad
    ...
end struct

enum Ad
    ...
end enum

match deger
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

implement ArayuzAdi for TipAdi
    ...
end implement

switch deger
    case X: ...
    default: ...
end switch
```

---

## ⚠️ KRITIK KURALLAR

```mlp
-- ✅ DOGRU: else if (iki kelime, bosluklu)
if x > 10 then
    ...
else
    if x > 5 then
        ...
    end if
end if

-- ❌ YANLIS: elseif (bitisik — gecersiz)
elseif x > 5 then

-- ✅ DOGRU: end loop (bosluklu)
loop kosul
    ...
end loop

-- ❌ YANLIS: end_loop (alt cizgili — gecersiz)
end_loop
```

---

## 🔗 ILGILI KARTLAR

- [IF_STATEMENT.md](IF_STATEMENT.md)
- [FOR_LOOP.md](FOR_LOOP.md)
- [LOOP_STATEMENT.md](LOOP_STATEMENT.md)
