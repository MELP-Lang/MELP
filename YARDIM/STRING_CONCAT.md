# MELP String Birleştirme (String Concatenation) Referansı

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
string sonuc = strA + strB
```

`string` değişkenleri `+` operatörüyle birleştirilir. Sonuç yeni bir `string` değeridir.

---

## Kullanım Örnekleri

### Örnek 1: Basit birleştirme
```mlp
string a = "Merhaba"
string b = " Dünya"
string c = a + b
print(c)
-- Çıktı: Merhaba Dünya
```

### Örnek 2: Çok adımlı birleştirme
```mlp
string x = "foo"
string y = "bar"
string z = "baz"
string ab = x + y
string abc = ab + z
print(abc)
-- Çıktı: foobarbaz
```

### Örnek 3: Literal birleştirme
```mlp
string greeting = "Merhaba" + " orada"
print(greeting)
-- Çıktı: Merhaba orada
```

### Örnek 4: Numeric ile birleştirme (str() kullanarak)
```mlp
numeric sayi = 42
print("Değer: " + str(sayi))
-- Çıktı: Değer: 42
```

### Örnek 5: Zincirleme birleştirme
```mlp
string isim = "Ali"
numeric yas = 25
print("Ad: " + isim + ", Yaş: " + str(yas))
-- Çıktı: Ad: Ali, Yaş: 25
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- İki `string` değeri `+` ile birleştirilir
- `str()` ile numeric değerler stringe çevrilip birleştirilebilir
- Zincirleme: `a + b + c + d`
- Literal ve değişken karışımı: `"Merhaba " + isim`

### ❌ Yasaklar
- `string` + `numeric` doğrudan — tanımsız davranış; `str()` kullan
- `str()` kullanmadan `print(n)` ile yazdırılan bir sayıyı string ile birleştirme
- String çarpma veya tekrarlama: `"abc" * 3` — desteklenmez

---

## İlgili Özellikler

- **STR_BUILTIN** — `str()` ile tip dönüşümü
- **PRINT** — print built-in
- **STRING_INTERPOLATION** — `"{değişken}"` sözdizimi (⏳ Stage2)
- **VARIABLE** — Değişken bildirimleri

**Ayrıca bakınız:**
- [STR_BUILTIN.md](STR_BUILTIN.md)
- [PRINT.md](PRINT.md)
- [STRING_INTERPOLATION.md](STRING_INTERPOLATION.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `string + string` | ✅ | ✅ | ✅ |
| `str(numeric) + string` | ✅ | ✅ | ✅ |
| String interpolation `{x}` | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `print("x=" + n)` (sayıyı direkt birleştirme) | `print("x=" + str(n))` |
| `"abc" * 3` (string çarpma) | desteklenmez |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STRING_CONCAT.md](../belgeler/language/feature_cards/STRING_CONCAT.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/string_ops/](../GOLDEN_TEST_SUITE/string_ops/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
