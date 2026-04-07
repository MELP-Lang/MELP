# MELP Aralık (Range) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de çalışıyor

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
-- Dahil aralık (her iki uç dahil):
loop each i in 1..10
    -- i: 1, 2, 3, ..., 10
end loop

-- Hariç aralık (son hariç):
loop each i in 0..<10
    -- i: 0, 1, 2, ..., 9
end loop
```

Range ifadesi bir sayı aralığı üzerinde döngü kurmak için kullanılır.

---

## Kullanım Örnekleri

### Örnek 1: Basit 1'den 5'e
```mlp
loop each i in 1..5
    print(i)
end loop
-- Çıktı: 1 2 3 4 5
```

### Örnek 2: 0'dan n-1'e (hariç aralık)
```mlp
numeric n = 5
loop each i in 0..<n
    print(i)
end loop
-- Çıktı: 0 1 2 3 4
```

### Örnek 3: Toplam hesaplama
```mlp
numeric toplam = 0
loop each i in 1..100
    toplam = toplam + i
end loop
print(toplam)
-- Çıktı: 5050
```

### Örnek 4: for_each ile karşılaştırma

Stage1 öncesinde aynı sonuç for döngüsü ile:
```mlp
-- for döngüsü ile (Stage 0):
loop i = 1 to 5
    print(i)
end loop

-- range ile (Stage 1):
loop each i in 1..5
    print(i)
end loop
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `start..end` — dahil aralık
- `start..<end` — hariç aralık (son değer dahil değil)
- `for each` ile birlikte kullanılır
- Değişken sınır: `1..n`

### ❌ Yasaklar
- Geri aralık `10..1` — azalan için `for i = N downto M` kullan
- Float aralık — yalnızca tamsayı

---

## İlgili Özellikler

- **FOR_EACH** — Koleksiyon döngüsü
- **FOR_LOOP** — Sayaç tabanlı for döngüsü

**Ayrıca bakınız:**
- [FOR_EACH.md](FOR_EACH.md)
- [FOR_LOOP.md](FOR_LOOP.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `start..end` dahil | ❌ | ✅ | ✅ |
| `start..<end` hariç | ❌ | ✅ | ✅ |
| Geri aralık | ❌ | ❌ | ⏳ |
| Step range | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_for` | `end for` |
| `for i = 1 to 5` (Stage0 stili) | `for each i in 1..5` |
| `10..1` (geri aralık) | `for i = N downto M` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/RANGE.md](../belgeler/language/feature_cards/RANGE.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/range_for/](../GOLDEN_TEST_SUITE/range_for/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
