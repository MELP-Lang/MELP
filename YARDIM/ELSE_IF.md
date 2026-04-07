# MELP Else-If Zinciri (Else-If Chain) Referansı

**Son Güncelleme:** 15 Mart 2026
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
if koşul1 then
    -- birinci dal
else
    if koşul2 then
        -- ikinci dal
    else
        if koşul3 then
            -- üçüncü dal
        else
            -- varsayılan dal
        end if
    end if
end if
```

**Kritik:** `else if` iki ayrı token'dır — aralarında **boşluk vardır**.

---

## Kullanım Örnekleri

### Örnek 1: Basit else if
```mlp
numeric x = 5

if x > 10 then
    print(1)
else
    if x > 3 then
        print(2)
    else
        print(3)
    end if
end if
-- Çıktı: 2
```

### Örnek 2: Dört yönlü zincir (not sınıflandırma)
```mlp
numeric grade = 75

if grade >= 90 then
    print("A")
else
    if grade >= 80 then
        print("B")
    else
        if grade >= 70 then
            print("C")
        else
            if grade >= 60 then
                print("D")
            else
                print("F")
            end if
        end if
    end if
end if
-- Çıktı: C
```

### Örnek 3: else olmadan
```mlp
numeric score = 15

if score > 20 then
    print(1)
else
    if score > 10 then
        print(2)
    end if
end if
-- Çıktı: 2
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `else if` iki ayrı kelime — `else if` yazılır
- Zincir uzunluğu sınırsızdır
- Son `else` isteğe bağlıdır
- Tüm dallar tek `end if` ile kapatılır
- İlk eşleşen koşul çalışır, sonrakiler atlanır

### ❌ Yasaklar
- `elif` (Python stili) — HATA
- `elseif` (boşluksuz) — HATA
- Her dalı ayrı `end if` ile kapatmak — YANLIŞ

**Kanonik yazım:**
```mlp
else if x > 5 then
```

---

## İlgili Özellikler

- **If Statement** — Temel if/else
- **BOOLEAN_OPS** — Koşullarda and/or/not
- **COMPARISON** — Karşılaştırma operatörleri
- **BLOCK_TERMINATORS** — end if ve diğer blok kapatıcılar

**Ayrıca bakınız:**
- [IF_STATEMENT.md](IF_STATEMENT.md)
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)
- [COMPARISON.md](COMPARISON.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `else if` zinciri | ✅ | ✅ | ✅ |
| Sınırsız zincir uzunluğu | ✅ | ✅ | ✅ |
| İsteğe bağlı `else` | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [../../docs_EN/language/feature_cards/IF_STATEMENT.md](../../docs_EN/language/feature_cards/IF_STATEMENT.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/if_tests/](../GOLDEN_TEST_SUITE/if_tests/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
