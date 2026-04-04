# MELP Method'lar Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de çalışıyor (`method` keyword ile)

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
donustipi function StructTip.metod_adi(parametreler)
    -- self'e method keyword ile erişilir
    -- gövde
end function
```

Veya `method` keyword ile:

```mlp
struct Sayac
    numeric deger
end struct

method artir(Sayac self)
    self.deger = self.deger + 1
end method
```

---

## Kullanım Örnekleri

### Örnek 1: Method tanımı ve çağrısı
```mlp
struct Dikdortgen
    numeric en
    numeric boy
end struct

numeric function alan(Dikdortgen self)
    return self.en * self.boy
end function

Dikdortgen r
r.en = 5
r.boy = 3
print(alan(r))
-- Çıktı: 15
```

### Örnek 2: method keyword ile
```mlp
struct Sayac
    numeric deger
end struct

Sayac method artir(Sayac self)
    self.deger = self.deger + 1
    return self
end method

Sayac s
s.deger = 0
s = artir(s)
print(s.deger)
-- Çıktı: 1
```

### Örnek 3: Zincirleme method (method chaining)
```mlp
struct Builder
    string sonuc
end struct

Builder method ekle(Builder self; string parca)
    self.sonuc = self.sonuc + parca
    return self
end method

Builder b
b.sonuc = ""
b = ekle(b; "Merhaba ")
b = ekle(b; "Dünya")
print(b.sonuc)
-- Çıktı: Merhaba Dünya
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `function Tip.metod(Tip self; ...)` sözdizimi
- `method metod(Tip self; ...)` keyword'ü
- `self` değişkeni ile alana erişim
- Method'lar değiştirilmiş `self` döndürebilir

### ❌ Yasaklar
- `this` — hem `this` hem `self` destekleniyor; `this.alan` tercih edilir
- OOP inheritance (kalıtım) — MELP'te yok
- Private/public erişim modifier — yoktur

---

## İlgili Özellikler

- **STRUCT** — Struct tanımları
- **FUNCTION_DEF** — Fonksiyon tanımları
- **INTERFACES** — Arayüzler (metod imzaları)

**Ayrıca bakınız:**
- [STRUCT.md](STRUCT.md)
- [INTERFACES.md](INTERFACES.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_method` | `end method` |
| `method f() as void` | `method f()` |
| `method f() as Tip` | `Tip method f()` |
| `end_function` | `end function` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Struct metodu (function) | ✅ | ✅ | ✅ |
| `method` keyword | ❌ | ✅ | ✅ |
| Method chaining | ❌ | ✅ | ✅ |
| Kalıtım | ❌ | ❌ | ❌ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/METHODS.md](../belgeler/language/feature_cards/METHODS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/methods_tests/](../GOLDEN_TEST_SUITE/methods_tests/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
