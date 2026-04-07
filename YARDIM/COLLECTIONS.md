# MELP Koleksiyon Tipleri (Collections) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Array ve List tamamlandı; Tuple kısmi

---

## İçindekiler

1. [Genel Bilgi](#genel-bilgi)
2. [Array](#array)
3. [List](#list)
4. [Tuple](#tuple)
5. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Genel Bilgi

MELP'te üç yerleşik koleksiyon tipi vardır. **Tüm koleksiyonlarda ayırıcı `;` (noktalı virgül)** — asla `,` kullanma.

| Tip | Açıklama | Sözdizimi |
|-----|----------|-----------|
| `numeric[]` / `string[]` | Array: sabit tip, indeksli | `[1; 2; 3]` |
| `List{T}` | List: dinamik, sıralı | `("a"; "b")` |
| `Tuple{T; U}` | Tuple: heterojen, sabit uzunluk | `<42; "Ali">` |

---

## Array

```mlp
-- Tanımlama
numeric[] sayilar = [1; 2; 3; 4; 5]
string[] isimler = ["Ali"; "Veli"; "Mehmet"]

-- İndeksli erişim (0 tabanlı)
numeric ilk = sayilar[0]
numeric son = sayilar[4]

-- Uzunluk
numeric len = length(sayilar)

-- Boş array
string[] bos = []

-- 2D array
numeric[][] matris = [[1; 2]; [3; 4]]
numeric eleman = matris[0][1]   -- 2

-- For each ile
loop each s in sayilar
    print(s)
end loop
```

---

## List

```mlp
-- Tanımlama
List{string} kelimeler = ("merhaba"; "dünya")

-- Eleman ekleme
kelimeler.add("!")

-- İndeksli erişim
string bas = kelimeler[0]

-- Uzunluk
numeric len = kelimeler.length
```

---

## Tuple

```mlp
-- Tanımlama (heterojen — farklı tipler)
Tuple{numeric; string} cift = <42; "Ali">

-- Alan erişimi (0-tabanlı)
numeric id = cift.0
string isim = cift.1

-- Fonksiyondan çok dönüş değeri
Tuple{numeric; numeric} function koordinat_al()
    return <10; 20>
end function

Tuple{numeric; numeric} konum = koordinat_al()
print(konum.0)   -- 10
print(konum.1)   -- 20
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Ayırıcı olarak `;` kullan
- Array ve List: aynı tip elemanlar
- Tuple: farklı tipler (`Tuple{numeric; string; boolean}`)

### ❌ Yasaklar
- **Virgül kullanma:** `[1, 2, 3]` → HATA
- Float array indexleme
- Array boyut aşımı (tanımsız davranış)

### ⚠️ HashMap
```mlp
-- HashMap Stage 1+ planlandı — şu an mevcut değil
-- HashMap{string; numeric} skorlar = {"Ali": 95; "Veli": 87}
```

---

## İlgili Özellikler

- **FOR_EACH** — Koleksiyon döngüsü
- **ARRAY** — Detaylı array referansı
- **HASHMAP** — HashMap (Stage 1+)

**Ayrıca bakınız:**
- [ARRAY.md](ARRAY.md)
- [FOR_EACH.md](FOR_EACH.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `numeric[]` / `string[]` Array | ✅ | ✅ | ✅ |
| `List{T}` | ✅ | ✅ | ✅ |
| `Tuple{T; U}` | ⚠️ Kısmi | ✅ | ✅ |
| `HashMap{K; V}` | ❌ | ✅ | ✅ |
| 2D Array | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/COLLECTIONS.md](../belgeler/language/feature_cards/COLLECTIONS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/](../GOLDEN_TEST_SUITE/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
