# MELP Where Clause Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ KISMİ — Inline `{T: Interface}` şu an çalışır; `where` koşulu Stage 1+ planındadır

---

## Genel Bilgi

Where clause, generic fonksiyon ve yapıların hangi tip kısıtlamalarına uyması gerektiğini belirtir.

> **Şu an çalışan:** Inline `{T: ArayüzAdı}` sözdizimi  
> **Planned:** `where T: ArayüzAdı` ayrı blok sözdizimi

---

## Inline Kısıtlama (Şu An Çalışır)

Tip parametresi doğrudan fonksiyon imzasında kısıtlanabilir:

```mlp
-- {T: Comparable} inline kısıtlama
T function maksimum{T: Comparable}(T a; T b)
    if a > b then
        return a
    else
        return b
    end if
end function

function main()
    numeric sonuc = maksimum{numeric}(10; 20)
    print(sonuc)
end function
-- Çıktı: 20
```

---

## Planlanan `where` Sözdizimi (Stage 1+)

```mlp
-- Ayrı where bloğu (planlanan)
T function sirala{T}(T a; T b)
    where T: Comparable
    if a < b then
        return a
    else
        return b
    end if
end function
```

---

## Birden Fazla Kısıtlama

```mlp
-- Çoklu kısıtlama (inline, planlanan)
function isle{T: Comparable & Printable}(T deger)
    print(deger)
end function
```

---

## Yaygın Kısıtlamalar

| Kısıtlama | Anlam |
|-----------|-------|
| `{T: Comparable}` | `<`, `>`, `==` operatörleri desteklemeli |
| `{T: Printable}` | `print()` desteklemeli |
| `{T: Numeric}` | Aritmetik operasyonlar desteklemeli |

---

## Kısıt İhlali Hatası (Planlanan)

```mlp
-- Comparable olmayan tipi geçirmek hata vermeli
struct Nokta
    numeric x
    numeric y
end struct

-- Nokta Comparable değil, bu hata vermeli:
-- maksimum{Nokta}(n1; n2)   -- HATA: Nokta Comparable değil
```

---

## İlgili Özellikler

- **GENERICS** — Generic tipler
- **INTERFACES** — Arayüz tanımlama

**Ayrıca bakınız:**
- [GENERICS.md](GENERICS.md)
- [INTERFACES.md](INTERFACES.md)
- [belgeler/language/feature_cards/WHERE_CLAUSE.md](../belgeler/language/feature_cards/WHERE_CLAUSE.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 1+ |
|---------|---------|---------|---------|
| Inline `{T: I}` | ⚠️ Parse | ⚠️ Parse | ✅ Planlı |
| `where T:` sözdizimi | ❌ | ❌ | ✅ Planlı |
| Kısıt doğrulama | ❌ | ❌ | ✅ Planlı |
| Hata mesajı | ❌ | ❌ | ✅ Planlı |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f{T}() as T` | `T function f{T}()` |
| `function f() as void` | `function f()` |
| `end_function` | `end function` |
| `end_struct` | `end struct` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/WHERE_CLAUSE.md](../belgeler/language/feature_cards/WHERE_CLAUSE.md)
