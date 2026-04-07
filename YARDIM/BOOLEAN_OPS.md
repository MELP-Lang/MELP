# MELP Boolean İşlemleri (Boolean Operations) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Operatör Tablosu](#operatör-tablosu)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Öncelik Sırası](#öncelik-sırası)
5. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
6. [Yasaklar](#yasaklar)
7. [İlgili Özellikler](#ilgili-özellikler)
8. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
koşul1 and koşul2   -- mantıksal VE
koşul1 or koşul2    -- mantıksal VEYA
not koşul           -- mantıksal DEĞİL
```

Boolean operatörler `if`, `loop`, `else if` koşullarında kullanılır.

---

## Operatör Tablosu

| Operatör | Anlamı | MELP | C/JS karşılığı |
|----------|--------|------|----------------|
| VE | Her ikisi de doğru | `and` | `&&` |
| VEYA | En az biri doğru | `or` | `\|\|` |
| DEĞİL | Tersini al | `not` | `!` |

---

## Kullanım Örnekleri

### Örnek 1: Mantıksal VE (and)
```mlp
numeric x = 5
numeric y = 10

if x > 0 and y > 0 then
    print(1)
else
    print(0)
end if
-- Çıktı: 1
```

### Örnek 2: Mantıksal VEYA (or)
```mlp
numeric a = 0
numeric b = 5

if a > 0 or b > 0 then
    print(1)
else
    print(0)
end if
-- Çıktı: 1
```

### Örnek 3: Mantıksal DEĞİL (not)
```mlp
boolean flag = false

if not flag then
    print("bayrak yok")
end if
-- Çıktı: bayrak yok
```

### Örnek 4: Birleşik koşul
```mlp
numeric age = 25
numeric score = 80

if age >= 18 and score >= 60 then
    print("geçti")
end if
-- Çıktı: geçti
```

### Örnek 5: loop ile boolean
```mlp
numeric i = 0
numeric limit = 5

loop i < limit and i < 3
    i = i + 1
end loop
print(i)
-- Çıktı: 3
```

---

## Öncelik Sırası

En yüksekten en düşüğe:
1. `not`
2. `and`
3. `or`

```mlp
-- not a and b → (not a) and b
-- a or b and c → a or (b and c)

-- Önceliği değiştirmek için parantez kullan:
if (a or b) and c then
    ...
end if
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `and`, `or`, `not` — İngilizce anahtar kelimeler (küçük harf)
- `not` tekli operatör olarak kullanılır
- `and`/`or` kısa devre değerlendirmesi yapar
- Parantez ile öncelik değiştirilebilir

### ❌ Yasaklar
- `&&`, `||`, `!` — C/JS stili operatörler YASAK
- `AND`, `OR`, `NOT` — Büyük harf versiyonlar YASAK
- `MELP'te ayrı bir `bool` tipi yoktur — koşullar `numeric` değerlendirir (0=false, 0≠true)

---

## İlgili Özellikler

- **IF_STATEMENT** — Koşullu dallama
- **ELSE_IF** — else if zinciri
- **COMPARISON** — Karşılaştırma operatörleri
- **WHILE_LOOP** — loop koşulları

**Ayrıca bakınız:**
- [IF_STATEMENT.md](IF_STATEMENT.md)
- [COMPARISON.md](COMPARISON.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `and` | ✅ | ✅ | ✅ |
| `or` | ✅ | ✅ | ✅ |
| `not` | ✅ | ✅ | ✅ |
| Kısa devre değerlendirme | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/BOOLEAN_OPS.md](../belgeler/language/feature_cards/BOOLEAN_OPS.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
