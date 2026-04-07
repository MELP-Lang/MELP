# MELP Import Sistemi Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de çalışıyor

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
import modul_adi
```

Başka bir `.mlp` dosyasını içe aktar. Uzantı (`.mlp`) yazılmaz.

---

## Kullanım Örnekleri

### Örnek 1: Temel import

**Dosya: matematik.mlp**
```mlp
numeric function topla(numeric a; numeric b)
    return a + b
end function

numeric function carp(numeric a; numeric b)
    return a * b
end function
```

**Dosya: main.mlp**
```mlp
import matematik

function main()
    numeric sonuc = matematik.topla(5; 3)
    print(sonuc)    -- 8

    sonuc = matematik.carp(4; 7)
    print(sonuc)    -- 28
end function
```

### Örnek 2: Struct import

**Dosya: geometri.mlp**
```mlp
struct Nokta
    numeric x
    numeric y
end struct

numeric function mesafe(Nokta p)
    return p.x * p.x + p.y * p.y
end function
```

**Dosya: main.mlp**
```mlp
import geometri

function main()
    geometri.Nokta p
    p.x = 3
    p.y = 4
    print(geometri.mesafe(p))    -- 25
end function
```

### Örnek 3: Çoklu import

```mlp
import string_araclari
import matematik
import veri_yapilari

function main()
    string metin = string_araclari.birlestir("Merhaba"; " Dünya")
    numeric toplam = matematik.topla(10; 20)
    println(str(toplam))   -- 30
end function
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `import modul_adi` (uzantısız)
- Modül fonksiyonlarına `modul.fonksiyon()` ile erişim
- Modül struct'larına `modul.Struct` ile erişim
- Birden fazla import

### ❌ Yasaklar
- `import "modul.mlp"` (tırnak ve uzantı olmadan)
- Döngüsel import — hata oluşturur
- `from modul import fonksiyon` stili — yok, `modul.fonksiyon()` kullan
- `import *` — yok

---

## İlgili Özellikler

- **MODULES** — Modül sistemi
- **FUNCTION_DEF** — Dışa aktarılan fonksiyonlar
- **STRUCT** — Dışa aktarılan struct'lar

**Ayrıca bakınız:**
- [MODULES.md](MODULES.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `numeric function main()` | `function main()` |
| `return 0` (main içinde) | kaldır |
| `import "modul.mlp"` | `import modul` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel import | ❌ | ✅ | ✅ |
| Struct import | ❌ | ✅ | ✅ |
| Çoklu import | ❌ | ✅ | ✅ |
| Seçici import (`from`) | ❌ | ❌ | ⏳ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/IMPORT.md](../belgeler/language/feature_cards/IMPORT.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/import_tests/](../GOLDEN_TEST_SUITE/import_tests/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
