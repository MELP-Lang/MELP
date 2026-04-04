# VALIDATION_TESTING - Özellik Kartı

**Özellik:** Validation & Testing Pipeline  
**Durum:** 🔴 TODO (Phase 1.4)  
**Sözdizimi Sürümü:** mlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Test Tanımı ve Beklenen Değer
```mlp
test "simple lambda"
  numeric add = lambda(x; y) -> x + y
  expect(add(10; 20)).toBe(30)
end test
```

---

## 📝 GRAMER KURALLARI

```ebnf
test_decl    ::= "test" string_literal stmt_list "end_test"
expect_call  ::= "expect" "(" expr ")" ".toBe" "(" expr ")"
```

---

## ✅ TEST SENARYOLARI (Zorunlu)

Minimum 5 POC test:
1. Lambda için pozitif senaryo
2. Closure için regresyon senaryosu
3. Higher-order davranış doğrulaması
4. Hata durumunda doğru failure çıktısı
5. Benchmark/smoke sonucu doğrulaması

---

## 🔧 UYGULAMA DOSYALARI

- `tests/lambda/` doğrulama testleri
- `GOLDEN_TEST_SUITE/validation_testing/` POC testleri
- `compiler/stage1/lambda/` hedef modüller

---

**Not:** Bu kart phase 1.4 için test ve doğrulama referansıdır.
