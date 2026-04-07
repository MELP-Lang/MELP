# MELP Variadic Parametreler Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ KISMİ — Stage 1'de syntax parse edilir, spread operatörü sınırlı

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
-- Variadic parametre (son parametre olmalı):
donustipi function fonksiyon(Tip... args)
    -- args dizi olarak erişilir
end function
```

`...` ile işaretlenen son parametre değişken sayıda argüman alır.

---

## Kullanım Örnekleri

### Örnek 1: Toplama
```mlp
numeric function topla(numeric... args)
    numeric total = 0
    loop each n in args
        total = total + n
    end loop
    return total
end function

print(topla(1; 2; 3))            -- 6
print(topla(10; 20; 30; 40))     -- 100
print(topla(5))                   -- 5
```

### Örnek 2: Sabit + variadic karışımı
```mlp
function log(string prefix; string... messages)
    loop each msg in messages
        print(prefix + ": " + msg)
    end loop
end function

log("INFO"; "Başlatıldı"; "Hazır"; "Bekliyor")
-- Çıktı:
-- INFO: Başlatıldı
-- INFO: Hazır
-- INFO: Bekliyor
```

### Örnek 3: en büyük değer
```mlp
numeric function maksimum(numeric ilk; numeric... kalanlar)
    numeric m = ilk
    loop each n in kalanlar
        if n > m then
            m = n
        end if
    end loop
    return m
end function

print(maksimum(3; 1; 7; 2; 5))
-- Çıktı: 7
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `Tip...` ile son parametre variadic yapılır
- Variadic parametre dizi gibi `for each` ile gezilir
- Sabit parametreler öne, variadic sona gelir
- `string...`, `numeric...` gibi tip belirtmek zorunlu

### ❌ Yasaklar
- Variadic parametre tek değil (sadece son parametre)
- Spread operatörü `...dizi` henüz desteklenmez — bireysel argüman geçir
- C tarzı `va_args` yok — tip güvenli dizi kullanılır

---

## İlgili Özellikler

- **FOR_EACH** — Variadic argümanlar dizi olarak erişilir
- **FUNCTION_DEF** — Fonksiyon tanımları
- **ARRAY** — Dizi tipi (variadic internally array)

**Ayrıca bakınız:**
- [FOR_EACH.md](FOR_EACH.md)
- [FUNCTION_DEF.md](FUNCTION_DEF.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `Tip...` variadic | ❌ | ✅ | ✅ |
| `for each` ile gezmek | ❌ | ✅ | ✅ |
| Spread `...dizi` | ❌ | ❌ | ✅ |
| Generic variadic `T...` | ❌ | ⏳ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f(numeric... args) as numeric` | `numeric function f(numeric... args)` |
| `end_function` | `end function` |
| `end_for` | `end for` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/VARIADIC.md](../belgeler/language/feature_cards/VARIADIC.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/variadic_spread/](../GOLDEN_TEST_SUITE/variadic_spread/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
