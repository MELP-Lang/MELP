# MELP Üçlü Operatör (Ternary) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 2'de planlandı (`?` ve `:` token'ları henüz yok)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [Geçici Alternatif](#geçici-alternatif)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Hedef syntax (Phase 2 sonrası):
koşul ? doğru_değer : yanlış_değer
```

> **⚠️ UYARI:** `?` ve `:` (ternary) token'ları henüz lexer'da tanımlı değil.  
> `melp_compiler` ile çalıştırıldığında `Parser error: Expected expression` hatası alınır.  
> Geçici çözüm için [Geçici Alternatif](#geçici-alternatif) bölümüne bakınız.

---

## Kullanım Örnekleri

> Aşağıdaki örnekler hedef syntax'ı göstermektedir. Phase 2 sonrası çalışacaktır.

### Örnek 1: Mutlak değer
```mlp
-- Hedef (Phase 2 sonrası):
numeric abs_val = x >= 0 ? x : -x
```

### Örnek 2: String seçimi
```mlp
-- Hedef (Phase 2 sonrası):
string etiket = is_admin ? "Yönetici" : "Kullanıcı"
```

### Örnek 3: Fonksiyon argümanı olarak
```mlp
-- Hedef (Phase 2 sonrası):
print(n > 0 ? "pozitif" : "negatif veya sıfır")
```

### Örnek 4: Yuvalama (kaçının)
```mlp
-- Hedef (Phase 2 sonrası — okunabilirlik azalır):
string puan = score >= 90 ? "A" : score >= 70 ? "B" : "C"
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler (Phase 2 sonrası)
- Tip güvenli: `a` ve `b` aynı tipte olmalı
- Koşul `boolean` ifadesi olmalı
- Fonksiyon argümanı olarak kullanılabilir

### ❌ Yasaklar
- Tip uyuşmazlığı: `flag ? "metin" : 42` — geçersiz
- Fall-through: sadece tek doğru/yanlış kolu vardır
- Yan etkili ifadeler (okunabilirlik düşer)

### ⚠️ `?` Çakışması (Nullable)
```mlp
-- Nullable tip: numeric? (tip konumunda)
numeric? n = null

-- Ternary: n != null ? n : 0 (ifade konumunda)
-- Parser, context'ten ayırt eder
```

---

## Geçici Alternatif

Phase 2 tamamlanana kadar `if/else/end if` kullanın:

```mlp
-- Şu an çalışan form:
numeric abs_val
if x >= 0 then
    abs_val = x
else
    abs_val = -x
end if

-- Fonksiyon olarak:
numeric function abs(numeric x)
    if x >= 0 then
        return x
    end if
    return -x
end function
```

---

## İlgili Özellikler

- **IF_STATEMENT** — Uzun form koşul ifadesi
- **BOOLEAN_OPS** — Koşul ifadeleri
- **NULLABLE** — `?` nullable tip gösterimi (farklı)

**Ayrıca bakınız:**
- [IF_STATEMENT.md](IF_STATEMENT.md)
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `koşul ? a : b` | 🔴 TODO | ⏳ Phase 2 | ✅ |
| if/else alternatif | ✅ | ✅ | ✅ |
| Yuvalamalı ternary | ❌ | ❌ | ⚠️ önerilmez |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `koşul ? a : b` (Stage1'de çalışmaz) | `if koşul then a else b end if` |
| `end_function` | `end function` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/TERNARY.md](../belgeler/language/feature_cards/TERNARY.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/ternary/](../GOLDEN_TEST_SUITE/ternary/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
