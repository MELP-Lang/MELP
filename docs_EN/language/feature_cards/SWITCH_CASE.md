# SWITCH_CASE - Özellik Kartı

**Özellik:** `switch/case/default/end switch` — Çoklu değer dallanması  
**Durum:** 🔴 TODO (Phase 2 — `switch`/`end switch` token'ları yok)  
**Hafta:** Stage1 (spec) / Phase 2 (impl)  
**Sözdizimi Sürümü:** mlp0

> ⚠️ **Gerçek durum (25 Şubat 2026):** `switch`/`end switch`/`default` lexer'da tanımlı değil.
> Lexer'da olan: `match`/`case`/`end match` (farklı özellik — PATTERN_MATCHING).
> TODO_COMPLETE_FINAL Phase 2'ye eklenmeli.

---

## 🎯 ÖZELLİK

Sabit değer karşılaştırması için `switch`. Enum ve string karşılaştırmasında `match`'e tercih edilir. Fall-through yoktur — her `case` otomatik `break` davranışı gösterir.

---

## 📖 SÖZDİZİMİ

```mlp
-- Temel switch
switch x
    case 1: print("bir")
    case 2: print("iki")
    case 3: print("üç")
    default: print("diğer")
end switch

-- String switch
switch day
    case "Pazartesi": work()
    case "Cumartesi": rest()
    case "Pazar": rest()
    default: work()
end switch

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
end switch

-- switch vs match farkı
-- switch: sabit değer karşılaştırması
-- match: pattern (enum variant, struct destructure, guard clause)
```

**Fall-Through Yok:**
```mlp
-- ✅ MELP — case otomatik duruyor
switch x
    case 1: print("bir")   -- sadece "bir" yazdırır
    case 2: print("iki")
end switch

-- ❌ C tarzı fall-through YOKTUR
```

**match ile Karşılaştırma:**
```mlp
-- Basit değer → switch
switch color
    case "red": handle_red()
    default: handle_other()
end switch

-- Pattern / enum → match
match opt_val
    case Some(v): use(v)
    case None: skip()
end match
```

---

## 🧪 GOLDEN TEST

```mlp
string function day_type(string day)
    switch day
        case "Cumartesi": return "hafta sonu"
        case "Pazar": return "hafta sonu"
        default: return "hafta içi"
    end switch
end function

function main()
    if day_type("Cumartesi") == "hafta sonu" then
        if day_type("Pazartesi") == "hafta içi" then
        end if
    end if
end function
```

---

**Bağımlılıklar:** string, numeric, enum değerleri  
**Hafta:** Stage1  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` — Kontrol akışı
