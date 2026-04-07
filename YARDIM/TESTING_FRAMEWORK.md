# MELP Test Çerçevesi (Testing Framework) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.5

---

## Genel Bilgi

MELP built-in test çerçevesi, `test`/`end test` ve `assert` fonksiyonları ile test yazmanızı sağlar.  
Phase 4.5'te implement edilecektir.

> **⚠️ UYARI:** Yerleşik test çerçevesi henüz mevcut değildir.  
> Test amacıyla `TEST_BLOCK` ve `return` bazlı golden test yaklaşımını kullanın.

---

## Planlanan Syntax

```mlp
-- Test tanımlama:
test "toplama çalışıyor"
    numeric sonuc = topla(2; 3)
    assert(sonuc == 5; "2 + 3 = 5 olmalı")
end test

-- Test suite:
describe("Matematik Testleri"
    test "toplama"
        assert(topla(1; 1) == 2)
    end test
    
    test "çıkarma"
        assert(cikart(5; 3) == 2)
    end test
end describe)

-- Assert fonksiyonları:
assert(kosul; mesaj)
assert_eq(gercek; beklenen)
assert_ne(gercek; beklenen)
assert_true(kosul)
assert_false(kosul)
```

```bash
-- Test çalıştırma:
$ mlp test
Running 5 tests...
✓ toplama çalışıyor
✓ çıkarma çalışıyor
✗ modulo çalışıyor
  Expected: 2, Got: 3
```

---

## Şu Anki Alternatif: Golden Test Yaklaşımı

```mlp
-- Şu an çalışan test yaklaşımı:
-- return 0 = başarısız, return 1 = başarılı

function main()
    -- Test 1: Toplama
    if topla(2; 3) != 5 then
    end if
    
    -- Test 2: Çıkarma
    if cikart(5; 3) != 2 then
    end if
    
end function
```

---

## İlgili Özellikler

- **TEST_BLOCK** — Mevcut test bloğu sözdizimi
- **DEBUG_BLOCK** — Debug amaçlı kullanım

**Ayrıca bakınız:**
- [TEST_BLOCK.md](TEST_BLOCK.md)

---

## Durum Tablosu

| Özellik | Phase 4.5 | Şu An |
|---------|-----------|-------|
| `test`/`end test` | ✅ Planlı | ❌ |
| `assert()` | ✅ Planlı | ❌ |
| `describe()` | ✅ Planlı | ❌ |
| `mlp test` komutu | ✅ Planlı | ❌ |
| Golden test (return) | N/A | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_test` | `end test` |
| `end_describe` | `end describe` |
| `end_function` | `end function` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/TESTING_FRAMEWORK.md](../belgeler/language/feature_cards/TESTING_FRAMEWORK.md)
- **Mevcut:** [TEST_BLOCK.md](TEST_BLOCK.md)
