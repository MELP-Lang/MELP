# MELP Generics (Tip Parametreleri) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ PARTIAL — Stage 1 (`{T}` parse edilir, tip kontrolü eksik)

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
-- Tip parametresi {T} ile generic fonksiyon:
T function identity{T}(T deger)
    return deger
end function

-- Generic struct:
struct Kutu{T}
    T deger
    boolean dolu
end struct

-- Çağırma (tip belirtilerek):
numeric x = identity{numeric}(42)
string s = identity{string}("merhaba")
```

**Not:** MELP generic syntax `{T}` kullanır (köşeli parantez `<T>` değil).

> **⚠️ KISMİ DURUM:** `{T}` token'ları parse edilir ama tam tip kontrolü Stage 1'de eksiktir.

---

## Kullanım Örnekleri

### Örnek 1: Identity fonksiyon
```mlp
T function identity{T}(T deger)
    return deger
end function

function main()
    numeric n = identity{numeric}(42)
    string s = identity{string}("merhaba")
    print(n)   -- 42
    print(s)   -- merhaba
end function
```

### Örnek 2: Generic struct
```mlp
struct Kutu{T}
    T deger
    boolean dolu
end struct

function main()
    Kutu{numeric} sayi_kutu
    sayi_kutu.deger = 100
    sayi_kutu.dolu = true
    
    Kutu{string} metin_kutu
    metin_kutu.deger = "merhaba"
    metin_kutu.dolu = true
    
    print(sayi_kutu.deger)    -- 100
    print(metin_kutu.deger)   -- merhaba
end function
```

### Örnek 3: Çoklu tip parametresi
```mlp
Tuple{T; U} function cift{T; U}(T birinci; U ikinci)
    return <birinci; ikinci>
end function
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler (Stage 1 partial)
- `{T}` tek tip parametresi
- Generic fonksiyon bildirimi
- Generic struct bildirimi

### ⚠️ Dikkat Edilmesi Gerekenler
- Tip kontrolü eksik — hatalı kullanım derleme zamanı hatası vermeyebilir
- Monomorphization (tek tip için özelleşme) tam değil

### ❌ Henüz Çalışmayanlar
- `where` clause kısıtlamaları
- Çoklu tip parametresi tam destek

---

## İlgili Özellikler

- **INTERFACES** — Interface kısıtlamaları (where clause)
- **COLLECTIONS** — `Option{T}`, `List{T}` gibi built-in generics

**Ayrıca bakınız:**
- [INTERFACES.md](INTERFACES.md)
- [COLLECTIONS.md](COLLECTIONS.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f{T}() as T` | `T function f{T}()` |
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `{T}` parse | ❌ | ✅ | ✅ |
| Generic fonksiyon | ❌ | ⚠️ Partial | ✅ |
| Generic struct | ❌ | ⚠️ Partial | ✅ |
| `where` clause | ❌ | ❌ | ✅ |
| Tip kontrolü | ❌ | ⚠️ Eksik | ✅ |
| Monomorphization | ❌ | ⚠️ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/GENERICS.md](../belgeler/language/feature_cards/GENERICS.md)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
