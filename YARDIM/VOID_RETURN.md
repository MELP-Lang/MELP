# MELP Void Return Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ KISMİ — `return` argümansız zaman zaman parser hatası verebiliyor (G-04)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [Bilinen Kısıtlamalar](#bilinen-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Void fonksiyon (dönüş tipi belirtilmez):
function fonksiyon(parametreler)
    -- gövde
end function

-- Erken dönüş:
function fonksiyon(numeric x)
    if x < 0 then
        return    -- argümansız return
    end if
    print(x)
end function
```

---

## Kullanım Örnekleri

### Örnek 1: Temel void fonksiyon
```mlp
function selamlama(string isim)
    print("Merhaba " + isim)
end function

selamlama("Ali")
-- Çıktı: Merhaba Ali
```

### Örnek 2: Guard pattern (erken çıkış)
```mlp
function pozitif_yazdir(numeric x)
    if x <= 0 then
        return
    end if
    print(x)
end function

pozitif_yazdir(5)    -- yazdırır: 5
pozitif_yazdir(-3)   -- yazdırmaz
pozitif_yazdir(10)   -- yazdırır: 10
```

### Örnek 3: Döngüde erken çıkış
```mlp
function sinire_kadar_yazdir(numeric limit)
    numeric i = 1
    loop i <= 10
        if i > limit then
            return
        end if
        print(i)
        i = i + 1
    end loop
end function

sinire_kadar_yazdir(3)
-- Çıktı: 1 2 3
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Void fonksiyonlar dönüş tipi belirtilmeden tanımlanır: `function f()`
- Void fonksiyonda argümansız `return` (erken çıkış)
- Fonksiyon sonu otomatik çıkış (explicit `return` gerekmiyor)

### ❌ Yasaklar (ve çalışan alternatifler)
- Void fonksiyonda değer döndürme: `return 5` — tip hatası

---

## Bilinen Kısıtlamalar

| Durum | Açıklama |
|-------|---------|
| `return` argümansız | ⚠️ Zaman zaman parser hatası verebilir (G-04) |
| `return expr` (void içinde) | ❌ Tip hatası — void fonksiyonda değer döndürülemez |
| `function f()` (dönüş tipi yok) | ✅ Doğru void imzası |

**Not:** `function f() as void` sözdizimi **yanlıştır** — void fonksiyonlarda dönüş tipi hiç yazılmaz.

**`return 0` anti-pattern:**
```mlp
-- ❌ YANLIŞ: Void main'de return 0 kullanma
function main()
    print("hello")
    return 0    -- YANLIŞ!
end function

-- ✅ DOĞRU:
function main()
    print("hello")
end function
```

---

## İlgili Özellikler

- **RETURN** — Return deyimi
- **FUNCTION_DEF** — Fonksiyon tanımları
- **IF_STATEMENT** — Guard pattern için

**Ayrıca bakınız:**
- [RETURN.md](RETURN.md)
- [FUNCTION_DEF.md](FUNCTION_DEF.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `as void` dönüş tipi (YANLIŞ SYNTAX) | ❌ | ❌ | ❌ |
| Argümansız `return` | ⚠️ | ✅ | ✅ |
| Otomatik void çıkış | ✅ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as void` | `function f()` (dönüş tipi yok) |
| `return 0` (void/main fonksiyonda) | kaldır |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `end_loop` | `end loop` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/VOID_RETURN.md](../belgeler/language/feature_cards/VOID_RETURN.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/void_return/](../GOLDEN_TEST_SUITE/void_return/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
