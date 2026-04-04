# MELP For Döngüsü (For Loop) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Gelişmiş Özellikler](#gelişmiş-özellikler)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [Yasaklar](#yasaklar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Artan (to)
loop i = baslangic to bitis
    -- gövde
end loop

-- Azalan (downto)
loop i = baslangic downto bitis
    -- gövde
end loop
```

**Açıklama:** `for` döngüsü sayaç değişkeni ile belirlenen bir aralıkta çalışır. `to` ile artar, `downto` ile azalır.

---

## Kullanım Örnekleri

### Örnek 1: Basit sayma (0'dan 4'e)
```mlp
loop i = 0 to 4
    print(i)
end loop
-- Çıktı: 0 1 2 3 4
```

### Örnek 2: 1'den 10'a kadar toplam
```mlp
numeric sum = 0
loop i = 1 to 10
    sum = sum + i
end loop
print(sum)
-- Çıktı: 55
```

### Örnek 3: downto ile geri sayım
```mlp
loop count = 10 downto 1
    print(count)
end loop
print("Kalkış!")
-- Çıktı: 10 9 8 7 6 5 4 3 2 1 Kalkış!
```

### Örnek 4: Dizi elemanlarına erişim
```mlp
numeric[] numbers = [1; 2; 3; 4; 5]
loop i = 0 to 4
    print(numbers[i])
end loop
-- Çıktı: 1 2 3 4 5
```

### Örnek 5: İç içe döngü
```mlp
loop i = 1 to 3
    loop j = 1 to 3
        numeric prod = i * j
        print(prod)
    end loop
end loop
```

---

## Gelişmiş Özellikler

### Adımlı sayma (step ile — Stage1+)
```mlp
-- Stage1'de:
loop i = 0 to 10 step 2
    print(i)
end loop
-- Çıktı: 0 2 4 6 8 10
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `to` ile artan aralık
- `downto` ile azalan aralık
- Döngü değişkeni döngü içinde kullanılabilir
- İç içe döngüler desteklenir
- `end for` ile blok kapatılır

### ❌ Yasaklar
- `to` yerine virgül kullanmak: `for i = 0, 10` — HATA
- `end_for` (alt çizgili) — HATA; `end for` (boşluklu) olmalı
- Döngü sayacını döngü dışında tanımlamak zorunlu değil — döngü otomatik tanımlar
- Kayan noktalı sayaç kullanmak — Stage0'da desteklenmez

---

## İlgili Özellikler

- **FOR_EACH** — Koleksiyonlar üzerinde döngü
- **WHILE_LOOP** — Koşullu döngü
- **RANGE** — Aralık ifadeleri
- **BLOCK_TERMINATORS** — `end for` ve diğerleri

**Ayrıca bakınız:**
- [FOR_EACH.md](FOR_EACH.md)
- [WHILE_LOOP.md](WHILE_LOOP.md)
- [RANGE.md](RANGE.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_for` | `end for` |
| `for i = 0, 10` | `for i = 0 to 10` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `for i = 0 to N` | ✅ | ✅ | ✅ |
| `for i = N downto 0` | ✅ | ✅ | ✅ |
| `for i = 0 to N step 2` | ❌ | ✅ | ✅ |
| `for each item in list` | ❌ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FOR_LOOP.md](../belgeler/language/feature_cards/FOR_LOOP.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/for_downto/](../GOLDEN_TEST_SUITE/for_downto/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
