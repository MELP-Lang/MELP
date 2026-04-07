# MELP Fonksiyon Tanımlama (Function Definition) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ TAMAMLANDI — Stage 0

---

## İçindekiler

1. [Temel Sözdizimi](#temel-sözdizimi)
2. [Parametre Kuralları](#parametre-kuralları)
3. [Dönüş Tipi](#dönüş-tipi)
4. [Örnekler](#örnekler)
5. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
6. [İlgili Özellikler](#i̇lgili-özellikler)

---

## Temel Sözdizimi

```mlp
<dönüş_tipi> function <isim>(<tip> <parametre>; <tip> <parametre>)
    -- gövde
end function
```

---

## Parametre Kuralları

**Kritik:** Parametreler `,` değil `;` ile ayrılır!

```mlp
-- TEK PARAMETRE
function selamlama(string isim)
    print(isim)
end function

-- İKİ PARAMETRE (noktalı virgül!)
numeric function topla(numeric a; numeric b)
    return a + b
end function

-- ÜÇ PARAMETRE
string function birlestir(string ad; string soyad; numeric yas)
    return ad + " " + soyad
end function

-- PARAMETRESİZ
function merhaba()
    print("Merhaba!")
end function
```

---

## Dönüş Tipi

```mlp
-- void: değer döndürmez
function yaz(string x)
    print(x)
end function

-- numeric: tam sayı
numeric function kare(numeric n)
    return n * n
end function

-- string: metin
string function selamlama(string isim)
    return "Merhaba " + isim
end function

-- boolean: true/false
boolean function pozitif_mi(numeric n)
    return n > 0
end function
```

---

## Örnekler

### Basit Aritmetik

```mlp
numeric function faktoriyel(numeric n)
    if n <= 1 then
        return 1
    end if
    return n * faktoriyel(n - 1)
end function

function main()
    print(faktoriyel(5))
end function
-- Çıktı: 120
```

### Struct ile Çalışan Fonksiyon

```mlp
struct Dikdortgen
    numeric en
    numeric boy
end struct

numeric function alan(Dikdortgen d)
    return d.en * d.boy
end function

function main()
    Dikdortgen r
    r.en = 5
    r.boy = 3
    print(alan(r))
end function
-- Çıktı: 15
```

### Ana Fonksiyon

```mlp
-- main() her programda bulunur (giriş noktası)
function main()
    print("Merhaba Dünya")
end function
```

---

## Kurallar ve Kısıtlamalar

| Kural | Açıklama |
|-------|---------|
| Parametreler `;` ile ayrılır | `,` KULLANILMAZ |
| `tip function` | Dönüş tipi ÖNCE, sonra `function` |
| `end function` zorunlu | Blok terminatörü |
| `return` | Değer döndürmek için |
| `void` dönüş | Return ifadesi isteğe bağlı |
| Özyinelemeli çağrı | ✅ Desteklenir |
| İleri tanımlama | ❌ Şu an yok |

---

## Generic Fonksiyon

```mlp
-- {T} ile generic tanım (kısmi destek)
T function maksimum{T: Comparable}(T a; T b)
    if a > b then
        return a
    end if
    return b
end function
```

---

## External Fonksiyon (C FFI)

```mlp
-- C kütüphanesi fonksiyonu bildirimi
external function printf(string fmt) as numeric
external function malloc(numeric size) as string

function main()
    printf("C'den merhaba!\n")
end function
```

---

## İlgili Özellikler

- **LAMBDA** — Anonim fonksiyonlar
- **GENERICS** — Generic fonksiyonlar
- **EXTERNAL** — FFI fonksiyonları
- **WHERE_CLAUSE** — Tip kısıtlamaları

**Ayrıca bakınız:**
- [LAMBDA.md](LAMBDA.md)
- [GENERICS.md](GENERICS.md)
- [EXTERNAL.md](EXTERNAL.md)
- [belgeler/language/feature_cards/FUNCTION_DEF.md](../belgeler/language/feature_cards/FUNCTION_DEF.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `function f() as void` | `function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `return 0` (main içinde) | kaldır |

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 2 |
|---------|---------|---------|---------|
| Temel fonksiyon | ✅ | ✅ | ✅ |
| Parametreler | ✅ | ✅ | ✅ |
| Dönüş tipleri | ✅ | ✅ | ✅ |
| Özyinelemeli | ✅ | ✅ | ✅ |
| Generic `{T}` | ⚠️ Kısmi | ⚠️ Kısmi | ✅ Planlı |
| Function overload | ❌ | ❌ | ✅ Planlı |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FUNCTION_DEF.md](../belgeler/language/feature_cards/FUNCTION_DEF.md)
- **Tamamlanma:** Stage 0
