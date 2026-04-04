# MELP Struct Constructor Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 3 (EKS-003). Şu an factory pattern kullan!

---

## İçindekiler

1. [Planlanan Syntax](#planlanan-syntax)
2. [Mevcut Alternatif](#mevcut-alternatif)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Planlanan Syntax

Stage 1 hedef sözdizimi (henüz çalışmıyor):

```mlp
-- Constructor parametreli struct tanımı:
struct Nokta(numeric x; numeric y)
    numeric x
    numeric y
end struct

-- Constructor çağrısı:
Nokta p = Nokta(10; 20)
print(p.x)   -- 10
print(p.y)   -- 20
```

---

## Mevcut Alternatif

**Factory Pattern** — şu an çalışan yöntem:

```mlp
struct Nokta
    numeric x
    numeric y
end struct

Nokta function nokta_olustur(numeric x; numeric y)
    Nokta p
    p.x = x
    p.y = y
    return p
end function

Nokta p = nokta_olustur(10; 20)
print(p.x)
-- Çıktı: 10
```

---

## Kullanım Örnekleri

### Örnek 1: Factory pattern (şu an kullan)
```mlp
struct Dikdortgen
    numeric en
    numeric boy
end struct

Dikdortgen function dikdortgen(numeric en; numeric boy)
    Dikdortgen r
    r.en = en
    r.boy = boy
    return r
end function

Dikdortgen r = dikdortgen(5; 3)
print(r.en)
-- Çıktı: 5
```

### Örnek 2: İç içe struct (mevcut)
```mlp
struct Adres
    string sokak
    numeric posta_kodu
end struct

struct Kisi
    string isim
    Adres adres
end struct

Kisi function kisi_olustur(string isim; Adres adr)
    Kisi k
    k.isim = isim
    k.adres = adr
    return k
end function
```

---

## Kurallar ve Kısıtlamalar

### ✅ Şu an çalışan
- Factory pattern ile struct başlatma
- Boş struct oluşturma: `Nokta p`
- Alan atama: `p.x = 10`

### ❌ Yasaklar
- `new Point(10, 20)` — `new` keyword yoktur
- `Point{x: 10; y: 20}` literal — henüz desteklenmiyor
- `Point(10; 20)` sözdizimi — Phase 3'te gelecek

---

## İlgili Özellikler

- **STRUCT** — Struct tanımları
- **METHODS** — Struct metodları
- **FUNCTION_DEF** — Factory fonksiyonları

**Ayrıca bakınız:**
- [STRUCT.md](STRUCT.md)
- [METHODS.md](METHODS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Boş struct oluşturma | ✅ | ✅ | ✅ |
| Factory pattern | ✅ | ✅ | ✅ |
| Constructor parametresi | ❌ | ❌ | ✅ |
| Struct literal `{x: v}` | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `new Point(10; 20)` | factory pattern veya constructor syntax |
| `end_function` | `end function` |
| `end_struct` | `end struct` |
| `function f() as Nokta` | `Nokta function f()` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STRUCT_CONSTRUCTOR.md](../belgeler/language/feature_cards/STRUCT_CONSTRUCTOR.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/struct_constructor/](../GOLDEN_TEST_SUITE/struct_constructor/)
- **pmlp Syntax:** [belgeler/pmlp_sozdizimi.md](../belgeler/pmlp_sozdizimi.md)
