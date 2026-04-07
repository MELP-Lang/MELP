# MELP Operatör Aşıma (Operator Overloading) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ PARTIAL — Stage 1 Phase 1.5

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Desteklenen Operatörler](#desteklenen-operatörler)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Struct için operatör tanımla:
StrüktürAdı function StrüktürAdı.operatorOP(StrüktürAdı diger)
    -- gövde
end function

-- Karşılaştırma operatörü:
boolean function StrüktürAdı.operator==(StrüktürAdı diger)
    -- gövde
end function
```

---

## Kullanım Örnekleri

### Örnek 1: Vektör toplama (`operator+`)
```mlp
struct Vektor
    numeric x
    numeric y
end struct

Vektor function Vektor.operator+(Vektor diger)
    Vektor sonuc
    sonuc.x = this.x + diger.x
    sonuc.y = this.y + diger.y
    return sonuc
end function

function main()
    Vektor v1
    v1.x = 10
    v1.y = 20
    
    Vektor v2
    v2.x = 5
    v2.y = 15
    
    Vektor v3 = v1 + v2    -- operator+ çağrılır
    print(v3.x)   -- 15
    print(v3.y)   -- 35
end function
-- Çıktı: 15
```

### Örnek 2: Eşitlik karşılaştırması (`operator==`)
```mlp
struct Nokta
    numeric x
    numeric y
end struct

boolean function Nokta.operator==(Nokta diger)
    return this.x == diger.x and this.y == diger.y
end function
```

---

## Desteklenen Operatörler

| Operatör | Metod Adı | Açıklama |
|----------|-----------|----------|
| `+` | `operator+` | Toplama |
| `-` | `operator-` | Çıkarma |
| `*` | `operator*` | Çarpma |
| `/` | `operator/` | Bölme |
| `==` | `operator==` | Eşitlik |
| `!=` | `operator!=` | Eşitsizlik |
| `<` | `operator<` | Küçüktür |
| `>` | `operator>` | Büyüktür |

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Struct metodları olarak tanımlama
- `this` ile kendi alanlarına erişim

### ⚠️ Dikkat Edilmesi Gerekenler
- Stage 1.5 - partial implementation; tam destek Stage 2'de

### ❌ Yasaklar
- Built-in tipler için aşım (`numeric + numeric` değiştirilemez)
- Yeni operatör sembolü tanımlama

---

## İlgili Özellikler

- **METHODS** — Struct metodları
- **STRUCT_CONSTRUCTOR** — Struct tanımlama

**Ayrıca bakınız:**
- [METHODS.md](METHODS.md)
- [STRUCT_CONSTRUCTOR.md](STRUCT_CONSTRUCTOR.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_struct` | `end struct` |
| `return 0` (main içinde) | kaldır |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `operator+` tanımlama | ❌ | ⚠️ | ✅ |
| `operator==` | ❌ | ⚠️ | ✅ |
| Karşılaştırıcılar | ❌ | ⚠️ | ✅ |
| Özel sembol tanımlama | ❌ | ❌ | ❌ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/OPERATOR_OVERLOAD.md](../belgeler/language/feature_cards/OPERATOR_OVERLOAD.md)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
