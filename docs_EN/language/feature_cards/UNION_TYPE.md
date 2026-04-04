# UNION_TYPE - Özellik Kartı

**Özellik:** `numeric|string` — Union tipler  
**Durum:** ⚠️ PARTIAL (Stage 1 — temel parse var; tip kontrolü eksik)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** mlp1

---

## 🎯 ÖZELLİK

`|` ile birden fazla tipi birleştiren union. TypeScript tarzı. Değer iki tipten birini taşıyabilir. Tip darlama (narrowing) `match` veya `typeof` benzeri kontrol ile yapılır.

---

## 📖 SÖZDİZİMİ

```mlp
-- Union tip bildirimi
numeric|string value = 42
numeric|string other = "merhaba"

-- Fonksiyon parametresi
string function process(numeric|string input)
    match typeof(input)
        case "numeric":
            return "Sayı: " + input
        case "string":
            return "Metin: " + input
    end match
end function

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
```mlp
-- Union: herhangi iki tip
numeric|string x = 42    -- numeric veya string

-- Option{T}: değer var/yok
Option{numeric} n = Some(42)   -- numeric veya None (tercih edilir)
```

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ Tip darlama (narrowing) henüz tam çalışmıyor
-- Alternatif: Option{T} veya enum kullan
enum Value
    NumVal(numeric)
    StrVal(string)
end enum
```

---

## 🧪 GOLDEN TEST

```mlp
string function stringify(numeric|string x)
    match typeof(x)
        case "numeric": return "n:" + x
        case "string": return "s:" + x
    end match
end function

function main()
    string a = stringify(99)
    string b = stringify("ok")
    if a == "n:99" then
        if b == "s:ok" then return 1 end if
    end if
end function
```

---

**Bağımlılıklar:** match, typeof, enum  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Tip sistemi; `melp_syntax.md` — TOKEN_UNION_PIPE
