# MELP str() Builtin Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de tam olarak çalışıyor (Phase 1 — P-02)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Anti-Pattern'ler](#anti-patternler)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
str(ifade)   -- numeric veya boolean → string
```

`str()` fonksiyonu herhangi bir `numeric` veya `boolean` değeri `string`'e çevirir.

---

## Kullanım Örnekleri

### Örnek 1: Numeric literal
```mlp
print("Değer: " + str(42))
-- Çıktı: Değer: 42
```

### Örnek 2: Numeric değişken
```mlp
numeric sayi = 99
print("Sayı: " + str(sayi))
-- Çıktı: Sayı: 99
```

### Örnek 3: Boolean true
```mlp
boolean durum = true
print("Durum: " + str(durum))
-- Çıktı: Durum: true
```

### Örnek 4: Boolean false
```mlp
boolean bayrak = false
print("Bayrak: " + str(bayrak))
-- Çıktı: Bayrak: false
```

### Örnek 5: Zincirleme birleştirme
```mlp
numeric a = 1
numeric b = 2
print("a=" + str(a) + " b=" + str(b))
-- Çıktı: a=1 b=2
```

### Örnek 6: Negatif sayı
```mlp
numeric neg = -7
print(str(neg))
-- Çıktı: -7
```

---

## Anti-Pattern'ler

| Yanlış ❌ | Doğru ✅ | Açıklama |
|-----------|----------|---------|
| `to_string(42)` | `str(42)` | Eski Stage 0 adı — kaldırıldı |
| `string(42)` | `str(42)` | `string` bir tip adı, fonksiyon değil |
| `str(42; 10)` | `str(42)` | `str()` tek argüman alır |
| `numeric x = str(42)` | `string s = str(42)` | Dönüş tipi `string`'e atanmalı |

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `str(numeric_değer)` → string
- `str(boolean_değer)` → `"true"` veya `"false"`
- String birleştirme ile kullanım: `"önek" + str(x)`
- Değişken veya literal argüman

### ❌ Yasaklar
- `str()` birden fazla argüman almaz
- Dönüş değeri `numeric` değişkene atlanamaz
- `to_string()` artık geçerli değil — `str()` kullan

---

## İlgili Özellikler

- **STRING_CONCAT** — String birleştirme
- **PRINT** — print built-in
- **BASIC_TYPES** — Temel tipler
- **VARIABLE** — Değişken bildirimleri

**Ayrıca bakınız:**
- [STRING_CONCAT.md](STRING_CONCAT.md)
- [PRINT.md](PRINT.md)
- [BASIC_TYPES.md](BASIC_TYPES.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `str(numeric)` | ❌ | ✅ | ✅ |
| `str(boolean)` | ❌ | ✅ | ✅ |
| `str(float)` | ❌ | ✅ | ✅ |
| `to_string()` (eski) | ⚠️ | ❌ | ❌ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `to_string(42)` | `str(42)` |
| `string(42)` | `str(42)` |
| `print("x=" + n)` (sayıyı direkt birleştirme) | `print("x=" + str(n))` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STR_BUILTIN.md](../belgeler/language/feature_cards/STR_BUILTIN.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/str_builtin/](../GOLDEN_TEST_SUITE/str_builtin/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
