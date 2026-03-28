# COMPOSITION - Özellik Kartı

**Özellik:** `>>` / `<<` — Fonksiyon kompozisyon operatörleri  
**Durum:** ⚠️ STUB#1 (Stage 1 — parse edilir, codegen implement edilmedi)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** mlp1

---

## 🎯 ÖZELLİK

`f >> g` → `x ↦ g(f(x))` (soldan sağa kompozisyon). `f << g` → `x ↦ f(g(x))` (sağdan sola). Token'lar parse edilir ama **codegen stub'dır** — `codegen_vars.cpp` L245'te `left` döndürülür.

---

## 📖 SÖZDİZİMİ

```mlp
-- >> soldan sağa: f >> g → g ∘ f
numeric function addOne(numeric x)
    return x + 1
end function

numeric function double(numeric x)
    return x * 2
end function

-- Hedef kullanım (STUB çözüldükten sonra):
function add_then_double = addOne >> double
-- add_then_double(3) → double(addOne(3)) → double(4) → 8

-- << sağdan sola: f << g → f ∘ g
function double_then_add = addOne << double
-- double_then_add(3) → addOne(double(3)) → addOne(6) → 7

-- |> ile fark:
-- |> veri akışı:  x |> f → f(x)      (değer üzerinde)
-- >> fonksiyon:   f >> g              (fonksiyon üzerinde, yeni fonksiyon üretir)
```

**Token Listesi:**
| Token | Açıklama |
|-------|----------|
| `>>` | `TOKEN_COMPOSE_RIGHT` — soldan sağa kompozisyon |
| `<<` | `TOKEN_COMPOSE_LEFT` — sağdan sola kompozisyon |

**⚠️ STUB#1 — KRİTİK UYARI:**
```mlp
-- codegen_vars.cpp L245:
-- std::cerr << "UYARI: >> kompozisyon henüz implement edilmedi\n";
-- return left;   ← sağ taraf YOKSAYILIYOR

-- YZ DİKKAT: >> ve << kullanan kod şu an yanlış çalışır
-- |> kullan veya STUB#1 çözülene kadar bekle
```

---

## 🧪 GOLDEN TEST

```mlp
-- STUB#1 çözüldükten sonra:
numeric function addOne(numeric x)
    return x + 1
end function

numeric function double(numeric x)
    return x * 2
end function

function main()
    -- Şu an ÇALIŞMIYOR (STUB#1):
    -- function composed = addOne >> double
    -- numeric r = composed(3)  -- beklenen: 8
    
    -- Geçici workaround:
    numeric r = double(addOne(3))
    if r == 8 then return 1 end if
end function
```

---

**Bağımlılıklar:** fonksiyon birinci sınıf vatandaş, tip sistemi (fonksiyon tipi)  
**STUB#1 Lokasyon:** `codegen_vars.cpp` L245  
**Hafta:** STUB#1 Fix = öncelikli backlog  
**Referans Diller:** Haskell `.`, F# `>>` / `<<`, Elm `>>` / `<<`  
**Kanonik Kaynak:** `mlp2_syntax.md` — Kompozisyon bölümü
