# MELP if let / while let Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 📅 PLANNED — Week 9.2 (Ağustos 2026)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Geçici Alternatif](#geçici-alternatif)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Hedef syntax (Week 9.2 sonrası):
if let Some(değişken) = ifade then
    -- değişken burada bağlı
end if

loop let Some(değişken) = ifade
    -- değişken her iterasyonda bağlı
end loop
```

`if let`, `match` bloğunun tek `case` olan kısa formudur. `Option{T}` veya `Result{T;E}` değerlerini tek satırda güvenli çözmek için kullanılır.

> **⚠️ UYARI:** Bu özellik henüz uygulanmamıştır (Week 9.2 planlanmıştır).  
> Mevcut durumda `match` kullanın — aşağıdaki [Geçici Alternatif](#geçici-alternatif) bölümüne bakınız.

---

## Kullanım Örnekleri

> Aşağıdaki örnekler hedef syntax'ı göstermektedir.

### Örnek 1: Option unwrap — if let
```mlp
-- Hedef (Week 9.2 sonrası):
if let Some(user) = find_user("Ali") then
    print("Bulundu: " + user.name)
else
    print("Bulunamadı")
end if
```

### Örnek 2: Result unwrap — if let
```mlp
-- Hedef (Week 9.2 sonrası):
if let Ok(value) = parse_number("42") then
    print("Sayı: " + value)
else
    print("Geçersiz sayı")
end if
```

### Örnek 3: while let (iterasyon)
```mlp
-- Hedef (Week 9.2 sonrası):
loop let Some(line) = read_next_line(reader)
    process(line)
end loop
```

---

## Geçici Alternatif

Week 9.2'ye kadar `match` kullanın:

```mlp
-- Şu an çalışan form (match ile):
Option{string} user = find_user("Ali")
match user
    case Some(u):
        print("Bulundu: " + u)
    case None:
        print("Bulunamadı")
end match

-- Result için:
Result{numeric; string} sonuc = parse_number("42")
match sonuc
    case Ok(v):
        print("Sayı: " + v)
    case Err(e):
        print("Hata: " + e)
end match
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler (Week 9.2 sonrası)
- `if let Some(v) = expr then ... end if`
- `if let Ok(v) = expr then ... end if`
- `while let Some(v) = expr`
- İç içe `if let` blokları

### ❌ Yasaklar
- Boş `if let` bloğu
- `if let` olmadan değişken bağlama
- Farklı pattern türleri tek `if let`'te birleştirme

### Normalizer Dönüşümü
```mlp
-- if let Some(v) = expr then ... else ... end if
-- →
match expr
    case Some(v):
        ...
    else
        ...
end match
```

---

## İlgili Özellikler

- **RESULT_OPTION** — `Option{T}` ve `Result{T;E}` tipleri
- **PATTERN_MATCHING** — `match/case/end match`
- **IF_STATEMENT** — Standart if koşulu

**Ayrıca bakınız:**
- [RESULT_OPTION.md](RESULT_OPTION.md)
- [PATTERN_MATCHING.md](PATTERN_MATCHING.md)
- [IF_STATEMENT.md](IF_STATEMENT.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while let ...` (while anahtar kelimesi) | `loop let ...` |
| `end_if` | `end if` |
| `end_match` | `end match` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `if let Some(v)` | ❌ | ❌ | ✅ Week 9.2 |
| `while let Some(v)` | ❌ | ❌ | ✅ Week 9.2 |
| `match` (alternatif) | ✅ | ✅ | ✅ |
| Nestted if let | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/IF_LET.md](../belgeler/language/feature_cards/IF_LET.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/if_let/](../GOLDEN_TEST_SUITE/if_let/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
