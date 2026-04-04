# MELP Derleme Zamanı Hesaplama (Comptime) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔮 PLANLANMADI — AOT mimari değerlendirmesi bekliyor

---

## Genel Bilgi

`comptime` derleme zamanında çalışan fonksiyon ve ifadelerdir. Zig'in `comptime` modeline benzer.  
**MELP'in AOT mimarisiyle uyumu henüz değerlendirilmedi** — bu nedenle planlanmış bir özellik DEĞİLDİR.

> **⚠️ UYARI:** `comptime` kullanmayın. Sözdizimi ve tasarım belirsizdir.

---

## Neden Planlanmamış

1. MELP AOT derleyici → runtime yok → `comptime`/runtime ayrımı farklı etki
2. LLVM IR constant folding zaten bazı derleme zamanı optimizasyonları yapıyor
3. Generics + where clause yeterli mi? → Önce bu soruyu cevapla
4. Makro sistemi (`MACRO.md`) ile örtüşüyor → Önce makro tasarlanmalı

---

## Mevcut Alternatifler

```mlp
-- 1. LLVM constant folding (örtük):
numeric MAX = 1000    -- derleyici sabit olarak optimize eder

-- 2. Generics ile tip bazlı seçim:
function process{T: Numeric}(T val) as T
    -- tip bazlı özelleşme

-- 3. Normal fonksiyon (açık çağrı):
numeric function factorial(numeric n)
    if n <= 1 then return 1 end if
    return n * factorial(n - 1)
end function

numeric FACT_10 = factorial(10)   -- runtime'da çalışır
```

---

## İlgili Özellikler

- **MACRO** — Derleme zamanı kod üretimi (Research aşamasında)
- **GENERICS** — Tip parametreleri

**Ayrıca bakınız:**
- [MACRO.md](MACRO.md)
- [GENERICS.md](GENERICS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 | Sonrası |
|---------|--------|--------|--------|---------|
| `comptime` | ❌ | ❌ | ❌ | 🔮 Değerlendirme |
| LLVM constant fold | ✅ | ✅ | ✅ | ✅ |
| Generics | ❌ | ⚠️ | ✅ | ✅ |
| Macro | ❌ | ❌ | 🔮 | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/COMPTIME.md](../belgeler/language/feature_cards/COMPTIME.md)
- **Referans:** Zig `comptime`, C++ `constexpr`, D `static if`
