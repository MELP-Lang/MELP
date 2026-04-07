# MELP String Interpolasyonu (String Interpolation) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⏳ PLANLANMIŞ — Stage 2 (pmlp2). Şu an `+` birleştirme kullan!

---

## İçindekiler

1. [Mevcut Durum](#mevcut-durum)
2. [Planlanan Syntax](#planlanan-syntax)
3. [Mevcut Alternatif](#mevcut-alternatif)
4. [Normalizer Dönüşümü](#normalizer-dönüşümü)
5. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Mevcut Durum

> ⚠️ **DİKKAT:** String interpolasyon (yani `"{değişken}"` sözdizimi) henüz **çalışmıyor**!
>
> Şu an yalnızca `+` ile string birleştirme kullan.

---

## Planlanan Syntax

Stage 2 (pmlp2) hedef sözdizimi:

```mlp
string isim = "Ali"
numeric yas = 25
string msg = "Merhaba {isim}, yaşın {yas}!"
-- → "Merhaba Ali, yaşın 25!"
```

İfade interpolasyonu:
```mlp
numeric x = 10
string result = "x'in karesi: {x * x}"
-- → "x'in karesi: 100"
```

---

## Mevcut Alternatif

Stage 0 ve Stage 1'de string birleştirme kullan:

```mlp
-- ⚠️ İnterpolasyon ÇALIŞMIYOR — bunun yerine:
string isim = "Ali"
numeric yas = 25
string msg = "Merhaba " + isim + ", yaşın " + str(yas) + "!"
print(msg)
-- Çıktı: Merhaba Ali, yaşın 25!
```

---

## Normalizer Dönüşümü

Stage 2'de derleyici `{expr}` gösterilen ifadeleri otomatik olarak `+` birleştirmeye dönüştürecek:

```
Kaynak: "Merhaba {isim}, yaşın {yas}!"

Normalizer çıktısı: "Merhaba " + isim + ", yaşın " + str(yas) + "!"
```

---

## Kurallar ve Kısıtlamalar

### ✅ Stage 2'de planlanan
- `"metin {ifade} metin"` sözdizimi
- İfade içinde aritmetik: `{x * 2}`
- İfade içinde fonksiyon çağrısı: `{fonksiyon()}`

### ❌ Stage 0/1'de yok
- `{değişken}` hiçbir interpolasyon çalışmaz
- YZ tuzağı: YZ'ler çalışmayan string interpolasyon kodu yazabilir — sadece `+` kullan!

---

## İlgili Özellikler

- **STRING_CONCAT** — Mevcut birleştirme yöntemi (şu an kullan!)
- **STR_BUILTIN** — `str()` ile numeric'i stringe çevir
- **PRINT** — print built-in

**Ayrıca bakınız:**
- [STRING_CONCAT.md](STRING_CONCAT.md)
- [STR_BUILTIN.md](STR_BUILTIN.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `"metin {değişken}"` | ❌ | ❌ | ✅ |
| `"metin {ifade}"` | ❌ | ❌ | ✅ |
| `+` ile birleştirme (alternatif) | ✅ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `"Merhaba {isim}"` (Stage1'de çalışmaz) | `"Merhaba " + isim` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STRING_INTERPOLATION.md](../belgeler/language/feature_cards/STRING_INTERPOLATION.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/string_interp/](../GOLDEN_TEST_SUITE/string_interp/)
- **pmlp Syntax:** [belgeler/pmlp2_sozdizimi.md](../belgeler/pmlp2_sozdizimi.md)
