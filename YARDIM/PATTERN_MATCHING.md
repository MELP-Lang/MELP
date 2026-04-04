# MELP Pattern Matching Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🚧 WIP — Stage 1 (pmlp1). Stage 0'da çalışmaz!

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
match değer
    case kalıp then
        -- gövde
    case kalıp then
        -- gövde
    else
        -- varsayılan
end match
```

---

## Kullanım Örnekleri

### Örnek 1: Basit sayı eşleştirme
```mlp
numeric x = 5
match x
    case 1 then print("bir")
    case 2 then print("iki")
    case 5 then print("beş")
    else print("diğer")
end match
-- Çıktı: beş
```

### Örnek 2: Match ifade olarak değer döndürür
```mlp
numeric x = 2
string msg = match x
    case 1 then "bir"
    case 2 then "iki"
    else "diğer"
end match
print(msg)
-- Çıktı: iki
```

### Örnek 3: Guard clause (if koşulu)
```mlp
numeric n = 15
match n
    case x if x < 10 then print("küçük")
    case x if x < 20 then print("orta")
    else print("büyük")
end match
-- Çıktı: orta
```

### Örnek 4: Fonksiyon içinde match
```mlp
string function siniflandir(numeric n)
    return match n
        case 0 then "sıfır"
        case x if x < 0 then "negatif"
        case x if x < 100 then "küçük"
        else "büyük"
    end match
end function

print(siniflandir(0))
-- Çıktı: sıfır
print(siniflandir(-5))
-- Çıktı: negatif
```

---

## Gelişmiş Özellikler

### Enum Destructuring
```mlp
Option{string} kullanici = bul_kullanici("Ali")
match kullanici
    case Some(u) then
        print("Bulundu: " + u)
    case None then
        print("Yok")
end match
```

### Struct Destructuring
```mlp
struct Nokta
    numeric x
    numeric y
end struct

Nokta p
p.x = 10
p.y = 0

match p
    case Nokta { x: 0; y: 0 } then print("orijin")
    case Nokta { x: _; y: 0 } then print("X ekseninde")
    case Nokta { x: 0; y: _ } then print("Y ekseninde")
    else print("başka")
end match
```

### Wildcard `_`
```mlp
match x
    case 1 then print("bir")
    case _ then print("diğer herhangi")
end match
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `match / case / end match` yapısı
- Guard clause: `case x if koşul then`
- Wildcard: `case _ then`
- `else` ile varsayılan dal
- Match ifadesi olarak değer döndürebilir
- Enum ve struct destructuring (Stage 1)

### ❌ Yasaklar
- Tüm olasılıklar kapsanmazsa derleyici hatası (exhaustive)
- `switch/case` stili C sözdizimi geçersiz
- Her case ayrı `end match` ile kapatılmaz — tek `end match` kullanılır

---

## İlgili Özellikler

- **IF_STATEMENT** — Basit koşul kontrolü
- **SWITCH_CASE** — switch yapısı
- **ENUM** — Enum destructuring
- **STRUCT** — Struct pattern

**Ayrıca bakınız:**
- [SWITCH_CASE.md](SWITCH_CASE.md)
- [IF_LET.md](IF_LET.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Basit sayı eşleştirme | ❌ | ✅ | ✅ |
| Guard clause | ❌ | ✅ | ✅ |
| Enum destructuring | ❌ | ✅ | ✅ |
| Struct pattern | ❌ | ⏳ | ✅ |
| Match expression (değer döndürme) | ❌ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_match` | `end match` |
| `switch x ... end switch` | `match x ... end match` |
| `function f() as string` | `string function f()` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/PATTERN_MATCHING.md](../belgeler/language/feature_cards/PATTERN_MATCHING.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/pattern_matching/](../GOLDEN_TEST_SUITE/pattern_matching/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
