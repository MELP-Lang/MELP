# PIPE_OPERATOR - Özellik Kartı

**Özellik:** `|>` — Pipe operatörü (soldan sağa fonksiyon zinciri)  
**Durum:** ⚠️ PARTIAL (Stage 1 Week 2 — parse edilir, test edilmedi)  
**Hafta:** Week 2  
**Sözdizimi Sürümü:** mlp1

---

## 🎯 ÖZELLİK

`x |> f(a)` → `f(x; a)` şeklinde dönüştürülür. Sol taraftaki değer fonksiyonun ilk argümanı olur. Zincirleme ile okunabilir veri dönüşüm pipeline'ları yazılır. F# / Elixir tarzı.

---

## 📖 SÖZDİZİMİ

```mlp
-- Temel kullanım: x |> f → f(x)
string result = "  merhaba  " |> trim()

-- Ek argümanla: x |> f(a) → f(x; a)
string padded = "ali" |> pad_left(10)

-- Zincirleme
string processed = raw_input
    |> trim()
    |> to_lower()
    |> replace("_"; "-")
    |> capitalize()

-- Koleksiyon pipeline
numeric total = [1; 2; 3; 4; 5]
    |> filter(is_even)
    |> map(square)
    |> reduce(0; add)

-- Lambda ile pipe
numeric result = 42
    |> (lambda(numeric x) numeric x * 2 end lambda)
    |> (lambda(numeric x) numeric x + 1 end lambda)
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `\|>` | `TOKEN_PIPE` — sol değeri sağ fonksiyona ilet |

**Dönüşüm Kuralı:**
```mlp
-- x |> f()       →  f(x)
-- x |> f(a)      →  f(x; a)      -- x birinci argüman
-- x |> f(a; b)   →  f(x; a; b)   -- x birinci argüman
```

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ Token parse edilir ama codegen test edilmedi
-- Çalıştığından emin olmak için önce test et
-- Alternatif (güvenli): doğrudan zincirli fonksiyon çağrısı
string r = capitalize(replace(to_lower(trim(raw_input)); "_"; "-"))
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function double(numeric x)
    return x * 2
end function

numeric function inc(numeric x)
    return x + 1
end function

function main()
    numeric result = 5 |> double() |> inc()
    -- double(5) = 10, inc(10) = 11
    if result == 11 then return 1 end if
end function
```

---

**Bağımlılıklar:** fonksiyon çağrısı, lambda  
**Hafta:** Week 2  
**Referans Diller:** F# `|>`, Elixir `|>`, Hack pipe `|>`  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Operatörler; `mlp2_syntax.md` — Pipeline bölümü
