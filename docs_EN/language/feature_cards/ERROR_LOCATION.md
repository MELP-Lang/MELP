# ERROR_LOCATION — Özellik Kartı

**Özellik:** Hata mesajlarında dosya:satır:sütun formatı  
**Durum:** ✅ COMPLETE (Phase 1 — P-03, 25 Şubat 2026)  
**Sözdizimi Sürümü:** Derleyici davranışı — dil sözdizimi değil  
**Kanonik Kaynak:** `TODO_PRODUCT_READY/TODO_PRODUCT_READY.md` → P-03  
**Golden Test Dizini:** `GOLDEN_TEST_SUITE/error_location/`

> ✅ **Doğrulandı (25 Şubat 2026 ÜA):** exit 1, `dosya.mlp:0:0: ❌ mesaj` formatı. Satır 0 — col fallback kabul edildi (ASTNode'da alan yok).

---

## 🎯 ÖZELLİK

Derleyici hata mesajları `dosya.mlp:satır:sütun: ❌ mesaj` formatına taşınır.

---

## 📖 HATA MESAJI FORMATI

```
# Önceki (Stage 1)
❌ Undefined variable: x

# Sonraki (P-03)
hata.mlp:5:3: ❌ Undefined variable: x
```

**Format:**
```
<dosya_yolu>:<satır>:<sütun>: ❌ <mesaj>
```

**Örnekler:**
```
program.mlp:3:1: ❌ Undefined variable: sayi
hesap.mlp:10:5: ❌ Undefined function: topla
main.mlp:7:3: ❌ Type mismatch: expected numeric, got string
```

**Sütun bilinmiyorsa:**
```
program.mlp:3:0: ❌ Undefined variable: sayi
```
(sütun `0` kabul edilir — bilgi yokkenki fallback)

---

## 🚫 ANTI-PATTERN'LER

| Yanlış ❌ | Doğru ✅ | Açıklama |
|---|---|---|
| `❌ Undefined variable: x` | `dosya.mlp:5:3: ❌ Undefined variable: x` | Dosya/satır bilgisi eksik |
| `ast.h`'yi değiştirmek | `col` yoksa `0` kullan | `ast.h` FROZEN |
| Tüm `std::cerr` satırlarını değiştirmek | Sadece kritik 5 nokta | Kapsam kayması — debug mesajları hariç |
| Dosya yolunu tam path vermek | Sadece input dosyası adı | `/home/...` uzun yol yerine `program.mlp` |

---

## 🔧 ETKİLENEN DOSYALAR

| Dosya | Değişiklik |
|---|---|
| `compiler/stage1/modules/orchestrator/src/codegen_context.h` | `currentFile` alanı + `MLP_CODEGEN_ERROR` makrosu |
| `compiler/stage1/modules/orchestrator/src/orchestrator.cpp` | `cg.currentFile = inputPath` atama |
| Codegen modülleri (min 5 kritik nokta) | `std::cerr` → `MLP_CODEGEN_ERROR` |

**Kritik hata noktaları (öncelik sırası):**
1. Undefined variable
2. Undefined function
3. Type mismatch
4. Missing return
5. Invalid operation

---

## 🧪 GOLDEN TESTLER

**Dizin:** `GOLDEN_TEST_SUITE/error_location/`  
**Minimum:** 5 test  
**Not:** Bu testler derleyicinin HATA vermesini test eder — başarılı derleme değil.

| Dosya | Test | Beklenen Çıktı (stderr) |
|---|---|---|
| `test_01_undef_var.mlp` | Tanımsız değişken | `test_01_undef_var.mlp:3:` içeren hata |
| `test_02_undef_func.mlp` | Tanımsız fonksiyon | `test_02_undef_func.mlp:3:` içeren hata |
| `test_03_line_number.mlp` | Hata satır numarası doğru mu | çıktıda `:4:` var |
| `test_04_filename.mlp` | Dosya adı mesajda var mı | çıktıda `test_04_filename.mlp` var |
| `test_05_type_mismatch.mlp` | Tip hatası | `test_05_type_mismatch.mlp:` içeren hata |

### Test Formatı

```mlp
-- test_01_undef_var.mlp
-- FEATURE: ERROR_LOCATION
-- PHASE: Phase1-P03
-- EXPECT_COMPILE_FAIL: true
-- EXPECTED_STDERR_CONTAINS: test_01_undef_var.mlp:3:

function main()
  print(tanimsiz)
end function
```

### Test Koşu Protokolü

```bash
COMPILER=./build/compiler/stage1/modules/orchestrator/melp_compiler

loop test in GOLDEN_TEST_SUITE/error_location/test_*.mlp; do
    base=$(basename "$test" .mlp)
    expect_fail=$(grep "^-- EXPECT_COMPILE_FAIL:" "$test" | grep -c "true")
    expected_fragment=$(grep "^-- EXPECTED_STDERR_CONTAINS:" "$test" | cut -d: -f2- | xargs)

    # Derle — stderr yakala
    stderr_out=$("$COMPILER" "$test" 2>&1 >/dev/null)
    exit_code=$?

    if [ "$expect_fail" -gt 0 ]; then
        # Hata bekleniyor
        if [ $exit_code -ne 0 ] && echo "$stderr_out" | grep -q "$expected_fragment"; then
            echo "✅ PASS: $base (hata mesajı '$expected_fragment' içeriyor)"
        else
            echo "❌ FAIL: $base | stderr: '$stderr_out' | beklenen fragment: '$expected_fragment'"
        fi
    else
        # Başarılı derleme bekleniyor
        if [ $exit_code -eq 0 ]; then
            echo "✅ PASS: $base"
        else
            echo "❌ FAIL: $base (derleme başarısız olmamalıydı)"
        fi
    fi
done
```
