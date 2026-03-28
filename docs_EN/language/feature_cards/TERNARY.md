# TERNARY - Özellik Kartı

**Özellik:** `x ? a : b` — Üçlü koşul operatörü  
**Durum:** 🔴 TODO (Phase 2 — `?` ve `:` ternary token'ları yok)  
**Hafta:** Stage1 (spec) / Phase 2 (impl)  
**Sözdizimi Sürümü:** mlp0

> ⚠️ **Gerçek durum (25 Şubat 2026):** `TOKEN_TERNARY_QUESTION` lexer'da tanımlı değil.
> `melp_compiler /tmp/test.mlp` → `Parser error: Expected expression`
> TODO_COMPLETE_FINAL Phase 2'ye eklenmeli.

---

## 🎯 ÖZELLİK

C tarzı üçlü operatör. Kısa koşul ifadesi için kullanılır. Tip güvenli: `a` ve `b` aynı tipte olmalıdır. Yuvalama mümkündür ama okunabilirlik için kaçınılmalıdır.

---

## 📖 SÖZDİZİMİ

```mlp
-- Temel form
numeric abs_val = x >= 0 ? x : -x

-- String seçimi
string label = is_admin ? "Yönetici" : "Kullanıcı"

-- Yuvalamalı (kaçının — okunabilirlik düşer)
string grade = score >= 90 ? "A" : score >= 70 ? "B" : "C"

-- Fonksiyon argümanı olarak
print(n > 0 ? "pozitif" : "negatif veya sıfır")

-- Koşul bloğu ile karşılaştırma
-- Ternary:       numeric v = flag ? 1 : 0
-- if/end if:     if flag then numeric v = 1 else numeric v = 0 end if
```

**Token Listesi:**
| Token | Açıklama |
|-------|----------|
| `?` | `TOKEN_TERNARY_QUESTION` — koşul sonu |
| `:` | `TOKEN_TERNARY_COLON` — true/false ayırıcı |

**DİKKAT — `?` çakışması:**
```mlp
-- Nullable ? ile TERNARY ? farklı context'te:
numeric? n = null       -- Nullable tipi
numeric v = n != null ? n : 0   -- Ternary operatör
-- Parser context'ten ayırt eder
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function abs(numeric x)
    return x >= 0 ? x : -x
end function

function main()
    numeric a = abs(-5)
    numeric b = abs(3)
    if a == 5 then
        if b == 3 then return 1 end if
    end if
end function
```

---

**Bağımlılıklar:** Boolean ifadeleri  
**Hafta:** Stage1  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` — Operatörler bölümü
