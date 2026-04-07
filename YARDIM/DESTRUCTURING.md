# MELP Yapı Söküm (Destructuring) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 📅 PLANNED — Week 6.4 (Nisan 2026)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Geçici Alternatif](#geçici-alternatif)
4. [İlgili Özellikler](#ilgili-özellikler)
5. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Hedef syntax (Week 6.4 sonrası):
numeric x; numeric y; numeric z = get_coords()

-- _ ile istenmeyen değerleri atla:
numeric _; numeric hi = min_max(arr)
```

Tuple veya çoklu dönüş değerlerini tek atama ifadesiyle ayrı değişkenlere bağlar.

> **⚠️ UYARI:** Bu özellik henüz implement edilmemiştir (Week 6.4 planlanmıştır).  
> Mevcut durumda index tabanlı erişim kullanın — aşağıdaki [Geçici Alternatif](#geçici-alternatif) bölümüne bakınız.

---

## Kullanım Örnekleri

> Aşağıdaki örnekler hedef syntax'ı göstermektedir.

### Örnek 1: Tuple destructuring
```mlp
-- Hedef (Week 6.4 sonrası):
tuple function koordinat_al()
    return <10; 20; 30>
end function

numeric x; numeric y; numeric z = koordinat_al()
-- x=10, y=20, z=30
```

### Örnek 2: Çok dönüş değeri
```mlp
-- Hedef (Week 6.4 sonrası):
tuple function min_max(numeric[] arr)
    return <arr[0]; arr[arr.length - 1]>
end function

numeric ust; numeric alt = min_max([1; 5; 3; 8; 2])
-- ust=1, alt=8
```

### Örnek 3: Swap
```mlp
-- Hedef (Week 6.4 sonrası):
tuple function swap(numeric a; numeric b)
    return <b; a>
end function

numeric x; numeric y = swap(1; 2)
-- x=2, y=1
```

---

## Geçici Alternatif

Week 6.4'e kadar index tabanlı erişim kullanın:

```mlp
-- Şu an çalışan form:
Tuple{numeric; numeric; numeric} coords = koordinat_al()
numeric x = coords.0
numeric y = coords.1
numeric z = coords.2
```

---

## İlgili Özellikler

- **COLLECTIONS** — Tuple tipi
- **PATTERN_MATCHING** — Gelişmiş pattern ile destructuring

**Ayrıca bakınız:**
- [COLLECTIONS.md](COLLECTIONS.md)
- [PATTERN_MATCHING.md](PATTERN_MATCHING.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `a; b = tuple` | ❌ | ❌ | ✅ Week 6.4 |
| `_ ile atla` | ❌ | ❌ | ✅ |
| Struct destructuring | ❌ | ❌ | ⏳ |
| Index tabanlı erişim | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/DESTRUCTURING.md](../belgeler/language/feature_cards/DESTRUCTURING.md)
- **Referans:** Python `a, b = fn()`, Rust `let (x, y) = tuple`
