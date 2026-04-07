# MELP Arayüz (Interfaces) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ PARTIAL — Stage 1 (parse edilir, dispatch eksik)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [İlgili Özellikler](#ilgili-özellikler)
5. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Arayüz tanımlama:
interface ArayüzAdı
    donus_tipi function metod_adi()
    donus_tipi function baska_metod(tip parametre)
end interface

-- Struct'ta implement:
struct StrüktürAdı implements ArayüzAdı
    -- alanlar
end struct

-- Metodun implementasyonu:
donus_tipi function StrüktürAdı.metod_adi()
    -- gövde
end function
```

> **⚠️ KISMİ DURUM:** Interface syntax parse edilir ama virtual dispatch (çok biçimlilik) Stage 1'de eksiktir.

---

## Kullanım Örnekleri

### Örnek 1: Basit interface
```mlp
interface Çizilebilir
    numeric function ciz()
    numeric function alan_al()
end interface

struct Daire implements Çizilebilir
    numeric yaricap
end struct

numeric function Daire.ciz()
    print("Daire çiziliyor")
    return 0
end function

numeric function Daire.alan_al()
    return 3 * this.yaricap * this.yaricap   -- yaklaşık
end function
```

### Örnek 2: Çoklu interface
```mlp
interface Yazdırılabilir
    string function string_yap()   -- tip ÖNCE
end interface

interface Karşılaştırılabilir
    numeric function karşılaştır(Karşılaştırılabilir diger)
end interface

struct Nokta implements Yazdırılabilir; Karşılaştırılabilir
    numeric x
    numeric y
end struct

string function Nokta.string_yap()
    return "(" + x + "; " + y + ")"
end function
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Interface tanımlama
- `implements` ile struct'a bağlama
- `StrüktürAdı.metod_adi()` implementasyonu

### ⚠️ Dikkat Edilmesi Gerekenler
- Virtual dispatch (interface tipinden çağırma) Stage 1'de çalışmıyor
- `interface Foo` tipinde değişken tutup çok biçimli çağırma yapılamıyor

### ❌ Henüz Çalışmayanlar
- Arayüz tipinde değişken: `Çizilebilir nesne = daire_nesnesi`
- Dinamik dispatch

---

## İlgili Özellikler

- **STRUCT_CONSTRUCTOR** — Struct tanımlama
- **METHODS** — Struct metodları
- **GENERICS** — Tip parametreleri

**Ayrıca bakınız:**
- [STRUCT_CONSTRUCTOR.md](STRUCT_CONSTRUCTOR.md)
- [METHODS.md](METHODS.md)
- [GENERICS.md](GENERICS.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as string` | `string function f()` |
| `end_interface` | `end interface` |
| `end_function` | `end function` |
| `baska_metod(param as tip)` | `baska_metod(tip param)` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `interface` tanımlama | ❌ | ⚠️ Parse | ✅ |
| `implements` keyword | ❌ | ⚠️ Parse | ✅ |
| Metod implementasyonu | ❌ | ⚠️ Partial | ✅ |
| Virtual dispatch | ❌ | ❌ | ✅ |
| Interface tipinde değişken | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/INTERFACES.md](../belgeler/language/feature_cards/INTERFACES.md)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
