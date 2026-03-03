# SWITCH_CASE - Özellik Kartı

**Özellik:** `switch/case/default/end_switch` — Çoklu değer dallanması  
**Durum:** 🔴 TODO (Phase 2 — `switch`/`end_switch` token'ları yok)  
**Hafta:** Stage 0 (spec) / Phase 2 (impl)  
**Sözdizimi Sürümü:** pmlp0

> ⚠️ **Gerçek durum (25 Şubat 2026):** `switch`/`end_switch`/`default` lexer'da tanımlı değil.
> Lexer'da olan: `match`/`case`/`end_match` (farklı özellik — PATTERN_MATCHING).
> TODO_COMPLETE_FINAL Phase 2'ye eklenmeli.

---

## 🎯 ÖZELLİK

Sabit değer karşılaştırması için `switch`. Enum ve string karşılaştırmasında `match`'e tercih edilir. Fall-through yoktur — her `case` otomatik `break` davranışı gösterir.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Temel switch
switch x
    case 1: print("bir")
    case 2: print("iki")
    case 3: print("üç")
    default: print("diğer")
end_switch

-- String switch
switch day
    case "Pazartesi": work()
    case "Cumartesi": rest()
    case "Pazar": rest()
    default: work()
end_switch

-- Çok satırlı case gövdesi
switch status
    case "ok":
        log("başarılı")
        count = count + 1
    case "err":
        log("hata")
        count = count - 1
    default:
        log("bilinmiyor")
end_switch

-- switch vs match farkı
-- switch: sabit değer karşılaştırması
-- match: pattern (enum variant, struct destructure, guard clause)
```

**Fall-Through Yok:**
```pmlp
-- ✅ MELP — case otomatik duruyor
switch x
    case 1: print("bir")   -- sadece "bir" yazdırır
    case 2: print("iki")
end_switch

-- ❌ C tarzı fall-through YOKTUR
```

**match ile Karşılaştırma:**
```pmlp
-- Basit değer → switch
switch color
    case "red": handle_red()
    default: handle_other()
end_switch

-- Pattern / enum → match
match opt_val
    case Some(v): use(v)
    case None: skip()
end_match
```

---

## 🧪 GOLDEN TEST

```pmlp
function day_type(string day) as string
    switch day
        case "Cumartesi": return "hafta sonu"
        case "Pazar": return "hafta sonu"
        default: return "hafta içi"
    end_switch
end_function

function main() as numeric
    if day_type("Cumartesi") == "hafta sonu" then
        if day_type("Pazartesi") == "hafta içi" then
            return 1
        end_if
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** string, numeric, enum değerleri  
**Hafta:** Stage 0  
**Kanonik Kaynak:** `pmlp0_sozdizimi.md` — Kontrol akışı
