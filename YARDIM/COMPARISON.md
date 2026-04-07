# MELP Karşılaştırma Operatörleri (Comparison) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Operatör Tablosu](#operatör-tablosu)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [Yasaklar](#yasaklar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
a == b   -- eşit mi
a != b   -- eşit değil mi
a < b    -- küçük mü
a > b    -- büyük mü
a <= b   -- küçük eşit mi
a >= b   -- büyük eşit mi
```

Tüm karşılaştırma operatörleri `if`, `loop` ve `else if` koşullarında kullanılabilir.

---

## Operatör Tablosu

| Operatör | Anlamı | Örnek |
|----------|--------|-------|
| `==` | Eşit | `x == 5` |
| `!=` | Eşit değil | `x != 0` |
| `<` | Küçük | `a < b` |
| `>` | Büyük | `a > b` |
| `<=` | Küçük eşit | `count <= 10` |
| `>=` | Büyük eşit | `score >= 90` |

---

## Kullanım Örnekleri

### Örnek 1: Eşitlik kontrolü
```mlp
numeric x = 5
if x == 5 then
    print(1)
end if
-- Çıktı: 1
```

### Örnek 2: Eşitsizlik
```mlp
numeric y = 3
if y != 5 then
    print(1)
end if
-- Çıktı: 1
```

### Örnek 3: Küçük / büyük karşılaştırma
```mlp
numeric a = 2
numeric b = 7
if a < b then
    print(1)
end if
-- Çıktı: 1
```

### Örnek 4: Küçük eşit
```mlp
numeric count = 5
if count <= 5 then
    print(1)
end if
-- Çıktı: 1
```

### Örnek 5: Not sınıflandırma
```mlp
numeric score = 90

if score >= 90 then
    print(4)
else
    if score >= 80 then
        print(3)
    else
        print(2)
    end if
end if
-- Çıktı: 4
```

### Örnek 6: loop döngüsünde karşılaştırma
```mlp
numeric i = 0
loop i != 3
    i = i + 1
end loop
print(i)
-- Çıktı: 3
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `numeric` değerleri arasında tüm operatörler çalışır
- Koşul ifadelerinde tek veya birleşik karşılaştırma
- `boolean` operatörlerle birleştirme: `x > 0 and x < 10`

### ❌ Yasaklar
- `=` atama operatörüdür, eşitlik kontrolü değil — `==` kullan
- Zincirleme karşılaştırma geçersiz: `0 < x < 10` → `x > 0 and x < 10` kullan
- String eşitlik karşılaştırması (`==`) güvenilir değil (pointer karşılaştırır) — Stage1'de düzeltilecek

---

## İlgili Özellikler

- **BOOLEAN_OPS** — `and`, `or`, `not`
- **IF_STATEMENT** — Koşullu dallama
- **ELSE_IF** — else if zinciri
- **WHILE_LOOP** — Döngü koşulları

**Ayrıca bakınız:**
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)
- [IF_STATEMENT.md](IF_STATEMENT.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Numeric karşılaştırma | ✅ | ✅ | ✅ |
| String eşitlik (`==`) | ⚠️ | ✅ | ✅ |
| Zincirleme (`0 < x < 10`) | ❌ | ❌ | ⏳ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/COMPARISON.md](../belgeler/language/feature_cards/COMPARISON.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
