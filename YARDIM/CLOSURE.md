# MELP Closure Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 1.2 (henüz implemente edilmedi)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [İlgili Özellikler](#ilgili-özellikler)
5. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

Closure, dış kapsamdaki değişkenleri yakalayan bir lambda'dır.

> **⚠️ UYARI:** Closure Phase 1.2'de implement edilecek. Şu an çalışmıyor.
> Mevcut alternatif için [LAMBDA.md](LAMBDA.md) dosyasına bak.

---

## Kullanım Örnekleri

### Örnek 1: Mevcut alternatif — fonksiyon parametresiyle aynı işlev
```mlp
numeric function ekle(numeric x; numeric y)
  return x + y
end function

function main()
  numeric sonuc = ekle(5; 10)
  print(sonuc)
end function
-- Çıktı: 15
```

### Örnek 2: Sayaç deseni (mevcut yöntem)
```mlp
numeric function arttir(numeric sayac)
  return sayac + 1
end function

function main()
  numeric c = 0
  c = arttir(c)
  c = arttir(c)
  c = arttir(c)
  print(c)
end function
-- Çıktı: 3
```

### Örnek 3: Çoklu parametre deseni (closure yerine)
```mlp
numeric function hesapla(numeric a; numeric b; string op)
  if op == "+" then
    return a + b
  else
      if op == "*" then
    return a * b
      else
    return 0
      end if
  end if
end function

function main()
  print(hesapla(10; 5; "+"))
  print(hesapla(10; 5; "*"))
end function
-- Çıktı: 15
-- Çıktı: 50
```

---

## Sık Yapılan Hatalar

| ❌ Yanlış | ✅ Doğru |
|-----------|---------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `else_if` | `else if` |
| `function f() as numeric` | `numeric function f()` |

---

## Kurallar ve Kısıtlamalar

### ✅ Planlanan (Phase 1.2)
- Dış kapsamdan otomatik değişken yakalama
- Mutable capture (sayaç deseni)
- Lambda başka lambda döndürebilir (iç içe closure)

### ❌ Henüz yok (Stage 0/1 başlangıcı)
- Closure semantiği implement edilmemiş
- `lambda` keyword parse edilir ama closure semantiği yok
- Şu an Lambda için [LAMBDA.md](LAMBDA.md) kullan

---

## İlgili Özellikler

- **LAMBDA** — Temel lambda (şu an çalışan alternatif)
- **HIGHER_ORDER_FUNCTIONS** — HOF desteği
- **FUNCREF** — Fonksiyon referansları

**Ayrıca bakınız:**
- [LAMBDA.md](LAMBDA.md)
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel lambda | ✅ | ✅ | ✅ |
| Closure (dış kapsam yakalama) | ❌ | ⏳ | ✅ |
| Mutable capture | ❌ | ❌ | ✅ |
| İç içe closure | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/CLOSURE.md](../belgeler/language/feature_cards/CLOSURE.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/closure/](../GOLDEN_TEST_SUITE/closure/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
