# MELP Ürün Hazırlık (Product Polish) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Stage 3 (Release Readiness)

---

## Genel Bilgi

Product Polish, MELP'in son kullanıcıya sunulmadan önce geçmesi gereken kalite kapılarını tanımlar.  
Smoke test paketi, CI/CD pipeline ve dokümantasyon tamamlanması gerekir.

> **⚠️ Stage 3'e Kadar Gerekli Değil:** Product Polish kriterleri mevcut geliştirme aşamasında  
> öncelikli değildir. Stage 0 ve Stage 1 tamamlandıktan sonra ele alınacaktır.

---

## Ürün Hazırlık Kriterleri

| Kriter | Hedef | Durum |
|--------|-------|-------|
| Smoke test paketi | 5 temel senaryo | 🔴 TODO |
| CI/CD pipeline | Her commit'te test | 🔴 TODO |
| Hata mesajı kalitesi | Anlaşılır + dosya:satır | ⚠️ Kısmi |
| Kullanıcı dokümantasyonu | YARDIM dizini | 🔄 In Progress |
| İkili dosya kurulum | `mlp install` | 🔴 TODO |

---

## Planlanan Smoke Test Senaryoları

5 temel senaryo Phase 3 için zorunludur:

```mlp
-- Senaryo 1: Merhaba Dünya
function main()
    print("Merhaba Dünya")
end function
```

```mlp
-- Senaryo 2: Aritmetik
numeric function topla(numeric a; numeric b)
    return a + b
end function

function main()
    print(topla(3; 5))
end function
```

```mlp
-- Senaryo 3: Döngü
function main()
    numeric i = 0
    loop i < 5
        print(i)
        i = i + 1
    end loop
end function
```

```mlp
-- Senaryo 4: Struct
struct Kisi
    string isim
    numeric yas
end struct

function main()
    Kisi k
    k.isim = "Ahmet"
    k.yas = 30
    print(k.isim)
end function
```

```mlp
-- Senaryo 5: External FFI
external function printf(string fmt) as numeric

function main()
    printf("Merhaba C\n")
end function
```

---

## CI/CD Pipeline Planı

```yaml
# .github/workflows/melp-ci.yml (planlanan)
name: MELP CI
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Build
        run: cmake --build build/ --clean-first
      - name: Golden Tests
        run: scripts/run_all_golden_tests.sh
      - name: Smoke Tests
        run: scripts/smoke_tests.sh
```

---

## Hata Mesajı Kalite Hedefi

Mevcut durum (kısmi):
```
dosya.mlp:10:5: ❌ Hata: tanımsız değişken 'foo'
```

Hedef format:
```
dosya.mlp:10:5: ❌ Hata: tanımsız değişken 'foo'
  10 | print(foo)
     |       ^^^
Öneri: 'foo' değişkeni tanımlanmadı. 'numeric foo = 0' ile tanımlayın.
```

---

## İlgili Özellikler

- **VALIDATION_TESTING** — Test framework
- **ERROR_LOCATION** — Hata mesajı formatı

**Ayrıca bakınız:**
- [VALIDATION_TESTING.md](VALIDATION_TESTING.md)
- [ERROR_LOCATION.md](ERROR_LOCATION.md)
- [belgeler/language/feature_cards/PRODUCT_POLISH.md](../belgeler/language/feature_cards/PRODUCT_POLISH.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 2 | Stage 3 |
|---------|---------|---------|---------|---------|
| Temel derleme | ✅ | ✅ | ✅ | ✅ |
| Smoke testler | ❌ | ❌ | ❌ | ✅ Planlı |
| CI pipeline | ❌ | ❌ | ❌ | ✅ Planlı |
| Kurulum paketi | ❌ | ❌ | ❌ | ✅ Planlı |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `loop i < 5 then` | `loop i < 5` |
| `end_function` | `end function` |
| `end_loop` | `end loop` |
| `end_struct` | `end struct` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/PRODUCT_POLISH.md](../belgeler/language/feature_cards/PRODUCT_POLISH.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/](../GOLDEN_TEST_SUITE/)
