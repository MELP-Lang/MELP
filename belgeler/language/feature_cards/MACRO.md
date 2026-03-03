# MACRO - Özellik Kartı

**Özellik:** Makro sistemi — derleme zamanı kod üretimi  
**Durum:** 🔮 RESEARCH (Stage 2+ — içerik belirsiz)  
**Hafta:** Stage 2+  
**Sözdizimi Sürümü:** pmlp2+

---

## 🎯 ÖZELLİK

Derleme zamanında AST dönüşümü veya kod üretimi. Hangi makro modelinin benimseneceği henüz kesinleşmedi. Olası yaklaşımlar: Rust prosedürel makro, Lisp homoiconic makro, C preprocessor tarzı (tercih edilmiyor).

---

## 📖 SÖZDİZİMİ

```pmlp
-- ⚠️ TASLAK / ARAŞTIRMA AŞAMASINDA — kesin sözdizimi yok

-- Seçenek A: declarative makro (Rust macro_rules! benzeri)
macro assert(condition; message)
    if !(condition) then
        panic(message)
    end_if
end_macro

assert(x > 0; "x pozitif olmalı")

-- Seçenek B: prosedürel makro (AST dönüşümü)
-- @derive(Printable)
-- struct User
--     string name
--     numeric age
-- end_struct

-- Seçenek C: derleme zamanı fonksiyon (comptime + fn)
-- (COMPTIME kartına bakınız)
```

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ KESİNLİKLE MAKRO YAZMAYIN — mevcut değil
-- Stage 2+ tasarım kararına kadar belirsiz
-- Şimdilik: normal fonksiyonlar ve generics kullan
-- debug bloğu (DEBUG_BLOCK.md) compile-time elimination'a yakın
```

**Araştırma Soruları:**
- Hygienic mi, non-hygienic mi?
- AST bazlı mı, token bazlı mı?
- Hangi use-case'ler öncelikli? (derive, assert, dsl?)
- COMPTIME ile ilişkisi ne olacak?

---

## 🧪 GOLDEN TEST

```pmlp
-- Henüz test yazılamaz — sözdizimi belirlenmedi
-- Stage 2 tasarım toplantısı sonrası bu karta örnekler eklenir
```

---

**Bağımlılıklar:** AST altyapısı, COMPTIME, Trait derive mekanizması  
**Hafta:** Stage 2+ (2027+)  
**Referans Diller:** Rust macros, Lisp defmacro, Zig comptime, Julia @macro  
**Durum Notu:** Bu kart araştırma placeholder'ı — tasarım kararı gelince güncellenecek
