# MELP Print (Yazdırma) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [Yasaklar](#yasaklar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
print(ifade)
```

`print` built-in bir fonksiyondur. Standart çıktıya yazar ve satır sonu (`\n`) ekler.

---

## Kullanım Örnekleri

### Örnek 1: Sayı yazdırma
```mlp
print(42)
-- Çıktı: 42
```

### Örnek 2: Değişken yazdırma
```mlp
numeric x = 7
print(x)
-- Çıktı: 7
```

### Örnek 3: String değişken
```mlp
string msg = "Merhaba"
print(msg)
-- Çıktı: Merhaba
```

### Örnek 4: String literal
```mlp
print("Dünya")
-- Çıktı: Dünya
```

### Örnek 5: İfade sonucunu yazdırma
```mlp
numeric a = 3
numeric b = 4
print(a + b)
-- Çıktı: 7
```

### Örnek 6: String birleştirme sonucu
```mlp
string s1 = "foo"
string s2 = "bar"
print(s1 + s2)
-- Çıktı: foobar
```

### Örnek 7: str() ile sayıyı string olarak yazdırma
```mlp
numeric n = 99
print("Sayı: " + str(n))
-- Çıktı: Sayı: 99
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `print(x)` her zaman satır sonu (`\n`) ekler
- Numeric değerler ondalık tamsayı formatında yazdırılır
- String değerler, string birleştirme ifadeleri
- `str()` ile numeric'i string'e çevirerek birleştir

### ❌ Yasaklar
- Çoklu argüman: `print(a, b)` — HATA; birleştir: `print(str(a) + " " + str(b))`
- Format string: `print("%d", x)` — HATA; `print(str(x))` kullan
- `print_str()` eskimiş alias — `print()` kullan

---

## İlgili Özellikler

- **STRING_CONCAT** — String birleştirme
- **STR_BUILTIN** — `str()` ile tip dönüşümü
- **VARIABLE** — Değişkenler
- **FUNCTION_DEF** — Kullanıcı tanımlı fonksiyonlar

**Ayrıca bakınız:**
- [STRING_CONCAT.md](STRING_CONCAT.md)
- [STR_BUILTIN.md](STR_BUILTIN.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `print(numeric)` | ✅ | ✅ | ✅ |
| `print(string)` | ✅ | ✅ | ✅ |
| `print(ifade)` | ✅ | ✅ | ✅ |
| Çoklu argüman | ❌ | ⏳ | ✅ |
| Format string | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `print(a, b)` (virgülle çoklu arg) | `print(str(a) + " " + str(b))` |
| `print("%d", x)` (format string) | `print(str(x))` |
| `print("x=" + n)` (sayıyı direkt birleştirme) | `print("x=" + str(n))` |
| `println(x)` (yanlış adlandırma) | `print(x)` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/PRINT.md](../belgeler/language/feature_cards/PRINT.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
