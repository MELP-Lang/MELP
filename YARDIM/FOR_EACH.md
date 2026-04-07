# MELP For Each Döngüsü Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Phase 2'de tam olarak çalışıyor (L-06)

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
loop each eleman in koleksiyon
    -- gövde
end loop
```

**Açıklama:** Bir koleksiyonun her elemanı üzerinde döngü kurar.

---

## Kullanım Örnekleri

### Örnek 1: Numeric dizi
```mlp
numeric[] sayilar = [1; 2; 3; 4; 5]
loop each n in sayilar
    print(n)
end loop
-- Çıktı: 1 2 3 4 5
```

### Örnek 2: String listesi
```mlp
string[] isimler = ["Ali"; "Veli"; "Ayşe"]
loop each isim in isimler
    print("Merhaba " + isim)
end loop
-- Çıktı: Merhaba Ali / Merhaba Veli / Merhaba Ayşe
```

### Örnek 3: Toplam hesaplama
```mlp
numeric function topla_liste(numeric[] dizi)
    numeric toplam = 0
    loop each n in dizi
        toplam = toplam + n
    end loop
    return toplam
end function

numeric[] data = [10; 20; 30; 40]
print(topla_liste(data))
-- Çıktı: 100
```

### Örnek 4: İç içe for each
```mlp
numeric[][] matris = [[1; 2]; [3; 4]]
loop each satir in matris
    loop each hucre in satir
        print(hucre)
    end loop
end loop
-- Çıktı: 1 2 3 4
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Array ve List koleksiyonları üzerinde çalışır
- `end for` ile kapatılır (for döngüsü ile aynı kapatıcı)
- İç içe kullanım desteklenir
- Eleman tipi otomatik çıkarılır

### ❌ Yasaklar
- `for i = 0; i < n; i++` tarzı C for döngüsü yoktur
- `for item: collection` (iki nokta yerine `in` kullan)
- `each` ve `in` arasındaki değişken zorunludur

**Sayma döngüsü için alternatif:**
```mlp
-- ❌ C-style for yoktur
-- ✅ Bunun yerine loop kullan:
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
```

---

## İlgili Özellikler

- **FOR_LOOP** — `for i = 0 to N` sayaç döngüsü
- **RANGE** — Aralık üzerinde döngü
- **ARRAY** — Dizi tipi
- **COLLECTIONS** — Koleksiyon tipleri

**Ayrıca bakınız:**
- [FOR_LOOP.md](FOR_LOOP.md)
- [RANGE.md](RANGE.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_for` | `end for` |
| `while i < 10` | `loop i < 10` |
| `end_while` | `end loop` |
| `function f() as numeric` | `numeric function f()` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Array üzerinde for each | ✅ | ✅ | ✅ |
| List üzerinde for each | ✅ | ✅ | ✅ |
| Range ile for each | ❌ | ✅ | ✅ |
| Channel üzerinde for each | ❌ | ⏳ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FOR_EACH.md](../belgeler/language/feature_cards/FOR_EACH.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/for_each/](../GOLDEN_TEST_SUITE/for_each/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
