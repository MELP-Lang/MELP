# PRODUCT_POLISH - Özellik Kartı

**Özellik:** Product Polish & Release Readiness  
**Durum:** 🔴 TODO (Phase Stage3)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Release Smoke Senaryosu
```mlp
test "release smoke"
  run("mlp repl --version")
  run("mlp test --smoke")
  expect(exit_code()).toBe(0)
end_test
```

---

## 📝 GRAMER KURALLARI

```ebnf
smoke_test   ::= "test" string_literal stmt_list "end_test"
run_cmd      ::= "run" "(" string_literal ")"
```

---

## ✅ TEST SENARYOLARI (Zorunlu)

Minimum 5 POC test:
1. Release smoke akışı
2. Dokümantasyon link bütünlüğü
3. Örnek proje açılışı
4. IDE temel akış (LSP/DAP bağlanma)
5. Paket komutları smoke doğrulaması

---

## 🔧 UYGULAMA DOSYALARI

- `GOLDEN_TEST_SUITE/product_polish/` POC testleri
- `TODO_STAGE2_PRODUCT_READY/MONTH_06_07_ELECTRON_IDE/` release phase dokümanları
- CI/release scriptleri (varsa)

---

**Not:** Bu kart phase Stage3 polish için release readiness referansıdır.
