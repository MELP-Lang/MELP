# MELP Nullable Tipler Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⏳ Stage 1 (kısmi destek)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Null Kontrolü](#null-kontrolü)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
Tip? degisken = null   -- nullable tip
Tip? degisken = deger  -- değerli nullable
```

`?` suffix'i bir tipin `null` alabileceğini belirtir.

---

## Kullanım Örnekleri

### Örnek 1: Nullable numeric
```mlp
numeric? n = null
if n == null then
    print("değer yok")
else
    print(n)
end if
-- Çıktı: değer yok
```

### Örnek 2: Nullable string
```mlp
string? isim = null
isim = "Ali"
if isim != null then
    print(isim)
end if
-- Çıktı: Ali
```

### Örnek 3: Fonksiyon nullable döndürür
```mlp
numeric? function bul(numeric[] arr; numeric hedef)
    loop each n in arr
        if n == hedef then
            return hedef
        end if
    end loop
    return null
end function

numeric[] data = [1; 2; 3; 4; 5]
numeric? sonuc = bul(data; 3)
if sonuc != null then
    print("Bulundu: " + str(sonuc))
end if
-- Çıktı: Bulundu: 3
```

---

## Null Kontrolü

```mlp
-- Klasik null kontrolü:
if degisken != null then
    -- güvenle kullan
end if

-- if let ile (Stage1+):
if let deger = nullable_degisken then
    print(deger)
end if
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `Tip?` sözdizimi ile nullable değişken tanımlama
- `null` ile karşılaştırma: `== null`, `!= null`
- Nullable dönüş tipi: `numeric? function f()`
- `if let` ile güvenli açma (Stage 1)

### ❌ Yasaklar
- Null kontrolü yapmadan nullable değeri kullanmak riski taşır
- Non-nullable değişkene `null` atamak — tip hatası

---

## İlgili Özellikler

- **IF_LET** — Güvenli nullable açma
- **RESULT_OPTION** — Option<T> tipi
- **BASIC_TYPES** — Temel tipler

**Ayrıca bakınız:**
- [IF_LET.md](IF_LET.md)
- [RESULT_OPTION.md](RESULT_OPTION.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric?` | `numeric? function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `end_for` | `end for` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `Tip?` sözdizimi | ❌ | ✅ | ✅ |
| `null` atama | ❌ | ✅ | ✅ |
| `if let` açma | ❌ | ✅ | ✅ |
| Null safety guarantees | ❌ | ⏳ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/NULLABLE.md](../belgeler/language/feature_cards/NULLABLE.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/nullable/](../GOLDEN_TEST_SUITE/nullable/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
