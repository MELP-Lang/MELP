# MELP If Deyimi (If Statement) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

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
if koşul then
    -- gövde
end if
```

### else ile:
```mlp
if koşul then
    -- doğru dal
else
    -- yanlış dal
end if
```

### else if ile:
```mlp
if koşul1 then
    -- birinci dal
else
    if koşul2 then
        -- ikinci dal
    else
        -- varsayılan dal
    end if
end if
```

---

## Kullanım Örnekleri

### Örnek 1: Basit if
```mlp
numeric x = 10

if x > 5 then
    print("büyük")
end if
-- Çıktı: büyük
```

### Örnek 2: if-else
```mlp
numeric n = 3

if n > 5 then
    print("büyük")
else
    print("küçük")
end if
-- Çıktı: küçük
```

### Örnek 3: else if zinciri
```mlp
numeric score = 85

if score >= 90 then
    print("A")
else
    if score >= 80 then
        print("B")
    else
        if score >= 70 then
            print("C")
        else
            print("F")
        end if
    end if
end if
-- Çıktı: B
```

### Örnek 4: İç içe if
```mlp
numeric a = 5
numeric b = 10

if a > 0 then
    if b > 5 then
        print("her ikisi de doğru")
    end if
end if
-- Çıktı: her ikisi de doğru
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `if` her zaman `then` ile bitmek zorundadır
- `end if` (tek token) ile blok kapatılır
- `else if` dalları isteğe bağlıdır
- Son `else` isteğe bağlıdır
- Koşul için parantez kullanmak isteğe bağlıdır: `if (x > 5) then` da çalışır

### ❌ Yasaklar
- `then` kelimesi atlanamaz: `if x > 5` — HATA; `if x > 5 then` olmalı
- Python stili `:` — HATA: `if x > 5:` geçersiz
- C stili `{}` — HATA: `if (x > 5) { }` geçersiz
- `elif` (Python stili) — HATA

```mlp
-- ❌ YANLIŞ:
if x > 5
    print("hata")
end if

-- ✅ DOĞRU:
if x > 5 then
    print("doğru")
end if
```

---

## İlgili Özellikler

- **Else If** — else if dalı
- **BOOLEAN_OPS** — Koşullarda and/or/not
- **COMPARISON** — Karşılaştırma operatörleri
- **BLOCK_TERMINATORS** — end if kuralları

**Ayrıca bakınız:**
- [Else If](ELSE_IF.md)
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)
- [COMPARISON.md](COMPARISON.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `if x > 5` (then yok) | `if x > 5 then` |
| `elif` | `else if` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel `if/else` | ✅ | ✅ | ✅ |
| `else if` zinciri | ✅ | ✅ | ✅ |
| Koşul ifadesi | ✅ | ✅ | ✅ |
| `if let` (pattern) | ❌ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/IF_STATEMENT.md](../belgeler/language/feature_cards/IF_STATEMENT.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/if_tests/](../GOLDEN_TEST_SUITE/if_tests/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
