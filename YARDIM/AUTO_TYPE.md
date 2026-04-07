# MELP Auto Tip Çıkarımı (Auto Type) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ KISMİ — `auto numeric` çalışıyor, `auto string` hatalı (EKS-018)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Mevcut Durum](#mevcut-durum)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [Yasaklar](#yasaklar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
auto değişken = ifade
```

`auto` anahtar kelimesi değişken tipi çıkarımı sağlar — değişkenin tipini açıkça yazmak gerekmez.

---

## Mevcut Durum

| Kullanım | Durum | Not |
|----------|-------|-----|
| `auto x = 42` | ✅ Çalışıyor | Numeric doğru çıkarılıyor |
| `auto s = "metin"` | ❌ Hatalı | Pointer adresi yazdırıyor (EKS-018) |
| `auto b = true` | ⚠️ Test gerekli | Boolean çıkarımı güvenilir değil |

**Güvenli alternatif:** `auto` yerine her zaman açık tip yaz:
```mlp
numeric x = 42         -- auto yerine
string s = "metin"     -- auto yerine
boolean b = true       -- auto yerine
```

---

## Kullanım Örnekleri

### Örnek 1: Numeric (çalışıyor)
```mlp
auto x = 42
print(x)
-- Çıktı: 42
```

### Örnek 2: Ondalık sayı (çalışıyor)
```mlp
auto pi = 3,14
print(pi)
-- Çıktı: 3.14
```

### Örnek 3: String (⚠️ buggy — bunun yerine açık tip kullan)
```mlp
-- ❌ Hatalı çıktı:
auto s = "merhaba"
print(s)
-- Çıktı: 93901662875652 (pointer adresi — YaaNLIŞ!)

-- ✅ Doğru:
string s = "merhaba"
print(s)
-- Çıktı: merhaba
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `auto x = 42` → numeric
- `auto f = 3,14` → numeric (ondalık)
- Açık tip bildirimi her zaman `auto`'ya tercih edilir

### ❌ Yasaklar
- `auto` string literal ile güvenilir değil — `string s = ...` kullan
- `var`, `let` yoktur — yalnızca `auto` veya açık tip
- Bildirimsiz değişken kullanımı

---

## İlgili Özellikler

- **BASIC_TYPES** — Açık tip kullanımı
- **ASSIGNMENT** — Değişken atama
- **VARIABLE** — Değişken bildirimleri

**Ayrıca bakınız:**
- [BASIC_TYPES.md](BASIC_TYPES.md)
- [VARIABLE.md](VARIABLE.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `auto numeric` | ✅ | ✅ | ✅ |
| `auto string` | ❌ | ✅ | ✅ |
| `auto boolean` | ⚠️ | ✅ | ✅ |
| Tüm tipler | ❌ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/AUTO_TYPE.md](../belgeler/language/feature_cards/AUTO_TYPE.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/auto_type_tests/](../GOLDEN_TEST_SUITE/auto_type_tests/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
