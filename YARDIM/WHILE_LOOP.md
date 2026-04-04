# MELP While Döngüsü (While Loop) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

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
loop koşul
    -- gövde
end loop
```

**Açıklama:** Koşul doğru olduğu sürece gövde çalıştırılır. Koşul yanlış olunca `end loop`'den sonraki koda geçilir.

---

## Kullanım Örnekleri

### Örnek 1: Temel sayaç
```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
-- Çıktı: 0 1 2 3 4 5 6 7 8 9
```

### Örnek 2: Koşul tabanlı döngü
```mlp
numeric n = 100
loop n > 1
    n = n / 2
end loop
print(n)
-- Çıktı: 0
```

### Örnek 3: Boolean koşul ile
```mlp
boolean devam = true
numeric sayac = 0

loop devam
    sayac = sayac + 1
    if sayac >= 5 then
        devam = false
    end if
end loop
print(sayac)
-- Çıktı: 5
```

### Örnek 4: İç içe while
```mlp
numeric i = 0
loop i < 3
    numeric j = 0
    loop j < 3
        print(i)
        j = j + 1
    end loop
    i = i + 1
end loop
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Koşul boolean ifadesi olmalı
- Koşul için parantez isteğe bağlıdır: `loop i < 10`
- `end loop` (iki token, boşluklu) ile kapatılır
- Döngü içinde değişken güncellenmeli — sonsuz döngüden kaçın

### ❌ Yasaklar
- `do` kelimesi — HATA: `while koşul do` geçersiz
- `end while` (iki token, boşluklu) — HATA; `end loop` olmalı
- `:` (Python stili) — HATA: `while koşul:` geçersiz
- `{}` (C stili) — HATA

**Yanlış → Doğru:**
```mlp
-- ❌ YANLIŞ:
loop i < 10 do
    print(i)
end loop

-- ✅ DOĞRU:
loop i < 10
    print(i)
    i = i + 1
end loop
```

---

## İlgili Özellikler

- **FOR_LOOP** — Sayaç tabanlı döngü
- **FOR_EACH** — Koleksiyon döngüsü
- **BOOLEAN_OPS** — Döngü koşulları için
- **BLOCK_TERMINATORS** — `end loop` kuralları

**Ayrıca bakınız:**
- [FOR_LOOP.md](FOR_LOOP.md)
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel `loop` | ✅ | ✅ | ✅ |
| Boolean koşul | ✅ | ✅ | ✅ |
| `break` / `continue` | ❌ | ⏳ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while koşul ... end while` | `loop koşul ... end loop` |
| `loop koşul do` | `loop koşul` (do yok) |
| `end_loop` | `end loop` |
| `end_function` | `end function` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/WHILE_LOOP.md](../belgeler/language/feature_cards/WHILE_LOOP.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/while_tests/](../GOLDEN_TEST_SUITE/while_tests/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
