# STR_BUILTIN — Özellik Kartı

**Özellik:** `str(x)` builtin — numeric / boolean → string dönüşümü  
**Durum:** ✅ COMPLETE (Phase 1 — P-02, 25 Şubat 2026)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  
**Kanonik Kaynak:** `TODO_PRODUCT_READY/TODO_PRODUCT_READY.md` → P-02  
**Golden Test Dizini:** `GOLDEN_TEST_SUITE/str_builtin/` (8 test, 8/8 geçiyor)

> ✅ **Doğrulandı (25 Şubat 2026 ÜA):** `str(x)` değişken, `str(true/false)`, `"..." + str(x)` üç kalıp da çalışıyor.

---

## 🎯 ÖZELLİK

`str(x)` fonksiyonu herhangi bir `numeric` veya `boolean` değeri `string`'e çevirir.  
String birleştirme (`+`) ile birlikte kullanılır.

---

## 📖 SÖZDİZİMİ

```mlp
-- numeric → string
str(42)          -- "42"
str(0)           -- "0"
str(-7)          -- "-7"
str(3)           -- "3"  (tamsayı kısmı)

-- boolean → string
str(true)        -- "true"
str(false)       -- "false"

-- string birleştirme ile kullanım (en sık kalıp)
print("Değer: " + str(x))
print("Sonuç: " + str(hesapla()))

-- değişken ile
numeric sayi = 42;
print("Sayı: " + str(sayi))

-- boolean ile
boolean durum = true;
print("Durum: " + str(durum))
```

---

## 🚫 ANTI-PATTERN'LER

| Yanlış ❌ | Doğru ✅ | Açıklama |
|---|---|---|
| `to_string(42)` | `str(42)` | Eski Stage 0 adı — kaldırıldı |
| `string(42)` | `str(42)` | `string` bir tip adı, fonksiyon değil |
| `str(42; 10)` | `str(42)` | `str()` tek argüman alır |
| `numeric x = str(42)` | `string s = str(42)` | Dönüş tipi `string` |
| `print(str(42))` | `print("" + str(42))` | `print()` zaten string istiyor — birleştirmeyle kullan |

---

## 🔧 ETKİLENEN DOSYALAR

| Dosya | Değişiklik |
|---|---|
| `compiler/stage1/modules/orchestrator/src/codegen_callable.cpp` | `handleFunctionCall` içine `str` handler |

**Implementasyon notu:**
- `str(boolean)` → LLVM `select` talimatı ile `"true"` / `"false"` global string pointer
- `str(numeric)` → `alloca [32 x i8]` + `snprintf` çağrısı
- `snprintf` modülde yoksa `ExternalLinkage` ile declare et

---

## 🧪 GOLDEN TESTLER

**Dizin:** `GOLDEN_TEST_SUITE/str_builtin/`  
**Minimum:** 5 test

| Dosya | Test | Beklenen Çıktı |
|---|---|---|
| `test_01_numeric_literal.mlp` | `str(42)` literal | `Değer: 42` |
| `test_02_numeric_var.mlp` | `str(değişken)` | `Sayı: 99` |
| `test_03_bool_true.mlp` | `str(true)` | `Durum: true` |
| `test_04_bool_false.mlp` | `str(false)` | `Bayrak: false` |
| `test_05_concat_chain.mlp` | `str` + concat birden fazla | `a=1 b=2` |

### Test Formatı

```mlp
-- test_01_numeric_literal.mlp
-- FEATURE: STR_BUILTIN
-- PHASE: Phase1-P02
-- EXPECTED: Değer: 42

function main() as numeric
  print("Değer: " + str(42))
  return 0
end_function
```

```mlp
-- test_05_concat_chain.mlp
-- FEATURE: STR_BUILTIN
-- PHASE: Phase1-P02
-- EXPECTED: a=1 b=2

function main() as numeric
  numeric a = 1;
  numeric b = 2;
  print("a=" + str(a) + " b=" + str(b))
  return 0
end_function
```

### Test Koşu Protokolü

```bash
COMPILER=./build/compiler/stage1/modules/orchestrator/melp_compiler

for test in GOLDEN_TEST_SUITE/str_builtin/test_*.mlp; do
    base=$(basename "$test" .mlp)

    # 1. Derle
    "$COMPILER" "$test"
    if [ $? -ne 0 ]; then
        echo "❌ COMPILE FAIL: $base"
        continue
    fi

    # 2. Çalıştır
    actual=$(lli output.ll 2>&1)
    expected=$(grep "^-- EXPECTED:" "$test" | cut -d: -f2- | xargs)

    # 3. Karşılaştır
    if [ "$actual" = "$expected" ]; then
        echo "✅ PASS: $base"
    else
        echo "❌ FAIL: $base | beklenen: '$expected' | alınan: '$actual'"
    fi
done
```
