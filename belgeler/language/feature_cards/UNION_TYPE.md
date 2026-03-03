# UNION_TYPE - Özellik Kartı

**Özellik:** `numeric|string` — Union tipler  
**Durum:** ⚠️ PARTIAL (Stage 1 — temel parse var; tip kontrolü eksik)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

`|` ile birden fazla tipi birleştiren union. TypeScript tarzı. Değer iki tipten birini taşıyabilir. Tip darlama (narrowing) `match` veya `typeof` benzeri kontrol ile yapılır.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Union tip bildirimi
numeric|string value = 42
numeric|string other = "merhaba"

-- Fonksiyon parametresi
function process(numeric|string input) as string
    match typeof(input)
        case "numeric":
            return "Sayı: " + input
        case "string":
            return "Metin: " + input
    end_match
end_function

-- Üç tipli union
numeric|string|boolean mixed = true

-- Nullable union (kombinasyon)
numeric|string|null opt = null
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `\|` union bağlamında | `TOKEN_UNION_PIPE` — iki tip arası |

**Option{T} ile Fark:**
```pmlp
-- Union: herhangi iki tip
numeric|string x = 42    -- numeric veya string

-- Option{T}: değer var/yok
Option{numeric} n = Some(42)   -- numeric veya None (tercih edilir)
```

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ Tip darlama (narrowing) henüz tam çalışmıyor
-- Alternatif: Option{T} veya enum kullan
enum Value
    NumVal(numeric)
    StrVal(string)
end_enum
```

---

## 🧪 GOLDEN TEST

```pmlp
function stringify(numeric|string x) as string
    match typeof(x)
        case "numeric": return "n:" + x
        case "string": return "s:" + x
    end_match
end_function

function main() as numeric
    string a = stringify(99)
    string b = stringify("ok")
    if a == "n:99" then
        if b == "s:ok" then return 1 end_if
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** match, typeof, enum  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Tip sistemi; `melp_syntax.md` — TOKEN_UNION_PIPE
