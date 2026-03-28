# NATIVE_BINARY — Özellik Kartı

**Özellik:** Native ELF binary üretimi — `melp_compiler dosya.mlp -o a.out`  
**Durum:** ✅ COMPLETE (Phase 1 — P-01, 25 Şubat 2026)  
**Sözdizimi Sürümü:** CLI flag — dil sözdizimi değil  
**Kanonik Kaynak:** `TODO_PRODUCT_READY/TODO_PRODUCT_READY.md` → P-01  
**Golden Test Dizini:** `GOLDEN_TEST_SUITE/native_binary/`

> ✅ **Doğrulandı (25 Şubat 2026 ÜA):** `-o a.out` ile ELF 64-bit binary üretiyor, `lli` olmadan çalışıyor.
> ❌ **P-01e eksik:** `build/melp` wrapper script henüz oluşturulmadı.

---

## 🎯 ÖZELLİK

`melp_compiler` şu an `.ll` (LLVM IR) üretir; binary için `lli` gerekir.  
Bu özellik `-o <dosya>` veya `--emit-binary` flag'i ile doğrudan ELF binary üretir.

---

## 📖 KULLANIM (CLI)

```bash
# Eski davranış — BOZULMAMALI
melp_compiler dosya.mlp                  # → output.ll (değişmez)

# Yeni davranış — opt-in
melp_compiler dosya.mlp -o a.out         # → a.out (ELF binary)
melp_compiler dosya.mlp --emit-binary    # → a.out (varsayılan isim)
melp_compiler dosya.mlp -o /tmp/prog     # → /tmp/prog (ELF binary)
```

**Pipeline:**
```
dosya.mlp → melp_compiler → output.ll → llc-14 → dosya.o → cc → a.out
                                         [YENİ]            [YENİ]
```

---

## 🚫 ANTI-PATTERN'LER

| Yanlış ❌ | Doğru ✅ | Açıklama |
|---|---|---|
| `output.ll` üretimini kaldırmak | `output.ll` her zaman üretilir | Golden testler `output.ll → lli` zincirini kullanır |
| `--emit-binary` varsayılan yapmak | Opt-in zorunlu | 341 test yavaşlar / CI kırılır |
| `-o` argümanını `output.ll` dosya adı olarak kullanmak | `-o` sadece binary ismini belirler | İki farklı argüman, iki farklı işlev |
| `.o` geçici dosyasını bırakmak | `std::remove(objFile)` ile sil | Disk kirliliği |

---

## 🔧 ETKİLENEN DOSYALAR

| Dosya | Değişiklik |
|---|---|
| `compiler/stage1/modules/orchestrator/src/orchestrator.cpp` | `-o` / `--emit-binary` argüman parse + `llc-14` + `cc` çağrısı |

**Bağımlılıklar (sistemde mevcut):**
- `llc-14` → `/usr/bin/llc-14`
- `cc` → `/usr/bin/cc`
- `-lm` → standart math

---

## 🧪 GOLDEN TESTLER

**Dizin:** `GOLDEN_TEST_SUITE/native_binary/`  
**Minimum:** 5 test, her biri `%100 gerçek çalıştırma` (lli YOK)

### Test Listesi

| Dosya | Test | Beklenen Çıktı |
|---|---|---|
| `test_01_hello.mlp` | Temel print + binary çalışma | `Merhaba, Dunya!` |
| `test_02_return_code.mlp` | `return 0` exit code | exit 0 |
| `test_03_numeric_output.mlp` | Numeric hesaplama çıktısı | `42` |
| `test_04_string_concat.mlp` | String birleştirme binary'de | `ab` |
| `test_05_no_lli.mlp` | `lli` olmadan çalıştır — `lli` PATH'te olmasa bile çalışır | `OK` |

### Test Formatı

```mlp
-- test_01_hello.mlp
-- FEATURE: NATIVE_BINARY
-- PHASE: Phase1-P01
-- EXPECTED: Merhaba, Dunya!
-- RUN: melp_compiler {file} -o {binary} && {binary}
-- NO_LLI: true  <-- bu satır zorunlu; test koşucusu lli kullanmaz

function main()
  print("Merhaba, Dunya!")
end function
```

### Test Koşu Protokolü

```bash
# Test koşucusu şu adımları izler:
COMPILER=./build/compiler/stage1/modules/orchestrator/melp_compiler

for test in GOLDEN_TEST_SUITE/native_binary/test_*.mlp; do
    base=$(basename "$test" .mlp)
    binary=/tmp/nb_${base}

    # 1. Binary üret
    "$COMPILER" "$test" -o "$binary"

    # 2. lli OLMADAN çalıştır
    actual=$("$binary" 2>&1)
    expected=$(grep "^-- EXPECTED:" "$test" | cut -d: -f2- | xargs)

    # 3. Karşılaştır
    if [ "$actual" = "$expected" ]; then
        echo "✅ PASS: $base"
    else
        echo "❌ FAIL: $base | beklenen: '$expected' | alınan: '$actual'"
    fi

    # 4. Temizle
    rm -f "$binary"
done
```
