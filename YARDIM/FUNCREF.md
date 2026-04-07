# MELP Fonksiyon Referansı (FuncRef) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⏳ Stage 1 (kısmi destek)

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
-- Fonksiyon referansı (fonksiyon adını değer olarak kullan)
funcref<(NumTip; NumTip) -> DönüşTip> ref = fonksiyon_adı

-- Çağrı
ref(arg1; arg2)
```

Bir fonksiyona referans almak onu değer olarak saklayıp geçirmeye yarar.

---

## Kullanım Örnekleri

### Örnek 1: Basit fonksiyon referansı
```mlp
numeric function topla(numeric a; numeric b)
    return a + b
end function

funcref<(numeric; numeric) -> numeric> f = topla
numeric sonuc = f(3; 4)
print(sonuc)
-- Çıktı: 7
```

### Örnek 2: Higher-order fonksiyona geçirme
```mlp
numeric function uygula(funcref<(numeric) -> numeric> fn; numeric x)
    return fn(x)
end function

numeric function kare(numeric n)
    return n * n
end function

print(uygula(kare; 5))
-- Çıktı: 25
```

### Örnek 3: Callback pattern
```mlp
function islem_yap(numeric[] arr; funcref<(numeric) -> void> callback)
    loop each n in arr
        callback(n)
    end loop
end function

function yazdir(numeric x)
    print(x)
end function

numeric[] data = [1; 2; 3]
islem_yap(data; yazdir)
-- Çıktı: 1 2 3
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Fonksiyon adı referans olarak değişkene atanabilir
- Higher-order fonksiyonlara argüman olarak geçirilebilir
- Tip imzası açık olarak belirtilmelidir

### ❌ Yasaklar
- Closure semantiği yok (dış değişken yakalamaz)
- Anonim fonksiyon bu sözdizimi ile oluşturulamaz (lambda kullan)

---

## İlgili Özellikler

- **LAMBDA** — Anonim fonksiyonlar
- **HIGHER_ORDER_FUNCTIONS** — HOF desteği
- **CLOSURE** — Closure semantiği (planlı)

**Ayrıca bakınız:**
- [LAMBDA.md](LAMBDA.md)
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `function f() as void` | `function f()` |
| `end_function` | `end function` |
| `end_for` | `end for` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Fonksiyon referansı | ❌ | ✅ | ✅ |
| HOF'a geçirme | ❌ | ✅ | ✅ |
| Döndürülen funcref | ❌ | ⏳ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FUNCREF.md](../belgeler/language/feature_cards/FUNCREF.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/funcref_tests/](../GOLDEN_TEST_SUITE/funcref_tests/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
