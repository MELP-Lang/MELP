# TEST / END_TEST — MELP Hızlı Referans

**Özellik:** Test bloğu — birim test çerçevesi  
**Durum:** ✅ Tam çalışıyor  
**Sözdizimi:** `test "isim" do ... end test`

---

## Temel Sözdizimi

```mlp
test "test açıklaması" do
    -- Test gövdesi
    assert(koşul)
end test
```

**`test` keyword'ü bir fonksiyon içinde kullanılmalıdır.**

---

## Çalışan Örnek

```mlp
function main()
    test "iki kere iki dört" do
        numeric x = 2 * 2
        assert(x == 4)
    end test

    test "toplama doğru" do
        numeric a = 10
        numeric b = 20
        assert(a + b == 30)
    end test

end function
```

**Çıktı (başarılı):**
```
  test: iki kere iki dört ... PASS
  test: toplama doğru ... PASS
```

---

## assert() — Koşul Doğrulama

```mlp
assert(koşul)         -- koşul false ise FAIL + exit(1)
assert(x == 4)        -- eşitlik kontrolü
assert(x > 0)         -- karşılaştırma
assert(not (x == 0))  -- mantıksal
```

**Başarısız assert davranışı:**
```
  test: dört beştir ... ASSERTION FAILED
```
Program `exit(1)` ile sonlanır. Sonraki testler çalışmaz.

---

## Birden Fazla assert

```mlp
function main()
    test "string operasyonları" do
        string s = "merhaba"
        assert(str(42) == "42")
        assert(s == "merhaba")
    end test
end function
```

---

## Test Çıktıları (Doğrulanmış)

| Test | Açıklama | Sonuç |
|------|----------|-------|
| `assert(2*2 == 4)` | Doğru koşul → PASS | ✅ |
| `assert(2*2 == 5)` | Yanlış koşul → ASSERTION FAILED + exit(1) | ✅ |
| Birden fazla test bloğu | Sırayla çalışır | ✅ |

---

## IR Karşılığı

| MLP | Codegen |
|-----|---------|
| `test "isim" do` | `mlp_test_begin(ptr_to_name)` çağrısı |
| `assert(x == 4)` | `mlp_assert(x == 4)` — false ise exit(1) |
| `end test` | `mlp_test_end()` çağrısı |

---

## Sınırlamalar

| Özellik | Durum |
|---------|-------|
| `test "isim" do` sözdizimi | ✅ Tam çalışıyor |
| `assert(koşul)` | ✅ Tam çalışıyor |
| `assert_eq(a; b)` | ❌ Yok — `assert(a == b)` kullan |
| `assert_msg(koşul; "mesaj")` | ❌ Yok |
| Başarısız test sonrası devam | ❌ exit(1) yapıyor — test izolasyonu yok |
| `test(isim; lambda() ... end lambda)` sözdizimi | ❌ Parser hatası |

---

## Anti-Pattern'ler

```mlp
-- ❌ YANLIŞ: test() fonksiyon sözdizimi (çalışmıyor)
test("test1"; lambda()
    assert(1 == 1)
end lambda)

-- ✅ DOĞRU: test ... do ... end test
test "test1" do
    assert(1 == 1)
end test

-- ❌ YANLIŞ: test fonksiyon dışında
test "main dışı" do
    assert(true)
end test

-- ✅ DOĞRU: test fonksiyon içinde
function main()
    test "ana test" do
        assert(true)
    end test
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_test` | `end test` |
| `end_function` | `end function` |
| `test("isim"; lambda() ...)` | `test "isim" do ... end test` |
| `assert_eq(a; b)` | `assert(a == b)` |

---

## İlgili YARDIM Dosyaları

- [DEBUG.md](DEBUG.md) — `debug`/`pause` zero-overhead debug bloğu
- [CONTROL_FLOW.md](CONTROL_FLOW.md) — `if/then/end if` koşul yapısı
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi
