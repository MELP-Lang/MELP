# MELP Makro Sistemi Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔮 RESEARCH — Stage 2+ (içerik belirsiz)

---

## Genel Bilgi

`macro` derleme zamanında AST dönüşümü veya kod üretimi yapar.  
Hangi makro modelinin benimseneceği **henüz kesinleşmemiştir**.

> **⚠️ UYARI:** Makro sistemi mevcut değildir — Stage 2+ araştırma aşamasındadır.  
> Asla makro yazmaya çalışmayın. Normal fonksiyonlar ve generics kullanın.

---

## Olası Tasarım Seçenekleri

```mlp
-- ⚠️ TASLAK / ARAŞTIRMA — kesin sözdizimi yok

-- Seçenek A: Declarative makro (Rust macro_rules! benzeri)
macro assert(kosul; mesaj)
    if not kosul then
        print(mesaj)
    end if
end macro

assert(x > 0; "x pozitif olmalı")

-- Seçenek B: Prosedürel makro (AST dönüşümü)
-- @derive(Yazdırılabilir)
-- struct Kullanıcı
--     string isim
--     numeric yas
-- end struct

-- Seçenek C: Derleme zamanı fonksiyon (comptime + fn)
-- (COMPTIME kartına bakınız)
```

**Açık tasarım soruları:**
- Hygienic mi, non-hygienic mi?
- AST tabanlı mı, token tabanlı mı?
- Hangi use-case'ler öncelikli? (derive, assert, dsl?)
- `comptime` ile ilişkisi?

---

## Mevcut Alternatifler

```mlp
-- Makro yerine şimdilik kullanılabilenler:

-- 1. Normal fonksiyon:
function dogrula(boolean kosul; string mesaj)
    if kosul == false then
        print("HATA: " + mesaj)
    end if
end function

-- 2. Generics:
T function process{T}(T deger)
    -- tip bazlı özelleşme
    return deger
end function

-- 3. debug bloğu (compile-time elim yerine):
debug
    print("Debug mesajı")
end debug
```

---

## İlgili Özellikler

- **COMPTIME** — Derleme zamanı hesaplama (ayrı araştırma)
- **GENERICS** — Tip parametreleri (mevcut alternatif)
- **DEBUG_BLOCK** — Zero-overhead debug (mevcut)

**Ayrıca bakınız:**
- [COMPTIME.md](COMPTIME.md)
- [GENERICS.md](GENERICS.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_macro` | `end macro` |
| `end_if` | `end if` |
| `end_function` | `end function` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 | Sonrası |
|---------|--------|--------|--------|---------|
| Makro sistemi | ❌ | ❌ | 🔮 | ✅ |
| Normal fonksiyon | ✅ | ✅ | ✅ | ✅ |
| Generics | ❌ | ⚠️ | ✅ | ✅ |
| debug bloğu | ✅ | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/MACRO.md](../belgeler/language/feature_cards/MACRO.md)
- **Referans:** Rust `macro_rules!`, Lisp makrolar, Nim template
