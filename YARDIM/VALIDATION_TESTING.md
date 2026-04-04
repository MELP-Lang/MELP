# MELP Doğrulama ve Test (Validation Testing) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 1.4

---

## Genel Bilgi

MELP'in yerleşik test ve doğrulama çerçevesi Phase 1.4'te planlanmaktadır.  
Şu an golden test pipeline ile manuel doğrulama yapılmaktadır.

> **⚠️ Henüz Mevcut Değil:** `test` ve `assert` anahtar kelimeleri  
> Phase 1.4'te eklenecektir.

---

## Planlanan Sözdizimi

```mlp
-- test bloğu (planlanan)
test "toplama doğru çalışıyor" do
    numeric sonuc = 3 + 5
    expect(sonuc).toBe(8)
end test

-- assert (planlanan)
test "değer kontrolü" do
    boolean aktif = true
    assert(aktif == true)
end test
```

---

## Planlanan Test API'si

| Metod | Açıklama |
|-------|---------|
| `expect(deger).toBe(beklenen)` | Eşitlik kontrolü |
| `expect(deger).toBeTrue()` | Boolean true kontrolü |
| `expect(deger).toBeFalse()` | Boolean false kontrolü |
| `expect(deger).toBeGreaterThan(n)` | Sayısal karşılaştırma |
| `assert(koşul)` | Koşul doğru değilse hata |

---

## Şu Anki Geçici Çözüm

Golden test pipeline ile doğrulama:

```bash
# 1. MELP programı derle ve çalıştır
./build/compiler/stage1/modules/orchestrator/melp_compiler test_program.mlp -o test_binary
./test_binary > cikti.txt

# 2. Beklenen çıktı ile karşılaştır
diff cikti.txt beklenen_cikti.txt
echo $?  # 0 = geçti, 1 = başarısız
```

---

## Mevcut Golden Test Yapısı

```bash
GOLDEN_TEST_SUITE/
├── hello_world/
│   ├── input.mlp
│   └── expected_output.txt
├── arithmetic/
│   ├── input.mlp
│   └── expected_output.txt
└── structs/
    ├── input.mlp
    └── expected_output.txt
```

Golden testleri çalıştırmak için:
```bash
scripts/run_all_golden_tests.sh
```

---

## Manuel Doğrulama Örneği

```mlp
-- Şu an mümkün olan: Çıktı bazlı doğrulama
function test_toplama()
    numeric sonuc = 3 + 5
    if sonuc == 8 then
        print("GEÇTI: toplama doğru")
    else
        print("BAŞARISIZ: toplama yanlış")
    end if
end function

function main()
    test_toplama()
end function
-- Çıktı: GEÇTI: toplama doğru
```

---

## İlgili Özellikler

- **TESTING_FRAMEWORK** — Test framework altyapısı
- **DEBUG_BLOCK** — Debug blokları
- **ERROR_LOCATION** — Hata raporlama

**Ayrıca bakınız:**
- [TESTING_FRAMEWORK.md](TESTING_FRAMEWORK.md)
- [DEBUG_BLOCK.md](DEBUG_BLOCK.md)
- [belgeler/language/feature_cards/VALIDATION_TESTING.md](../belgeler/language/feature_cards/VALIDATION_TESTING.md)

---

## Durum Tablosu

| Özellik | Phase 1.4 | Şu An |
|---------|-----------|-------|
| `test` bloğu | ✅ Planlı | ❌ |
| `expect().toBe()` | ✅ Planlı | ❌ |
| `assert()` | ✅ Planlı | ❌ |
| Golden test pipeline | N/A | ✅ Mevcut |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_test` | `end test` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `test "..." then` (yanlış anahtar kelime) | `test "..." do` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/VALIDATION_TESTING.md](../belgeler/language/feature_cards/VALIDATION_TESTING.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/](../GOLDEN_TEST_SUITE/)
