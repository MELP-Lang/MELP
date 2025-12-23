# 🤖 AI Başlangıç Noktası - MELP Compiler

## 📍 Şu Anki Durum
**Aktif Dal:** modern_YZ_05  
**Son Tamamlanan Görev:** Phase 1, Task 1.1 - String Type & Concatenation ✅  
**Tarih:** 13 Aralık 2025  
**Test Sonuçları:** 8/8 tests passed

## ✅ Task 1.1 - String Support (TAMAMLANDI)
- String değişken tanımlama: `string x = "hello"` ✅
- String birleştirme: `"a" + "b" + "c"` ✅
- String karşılaştırma: ==, !=, <, >, <=, >= ✅
- Test suite: 8/8 passing ✅
- Detaylar: `MODERN_LANGUAGE_PROCESS/modern_YZ/modern_YZ_05_TAMAMLANDI.md`

## 🎯 SONRAKİ GÖREV: Task 1.2 - For Loops

### Görev Tanımı (TODO_MODERN_FINAL.md'den)
```
Task 1.2: For Loops
- Numeric range: for i = 1 to 10
- Reverse range: for i = 10 to 1 step -1  
- List iteration: for item in list
```

### Başlangıç Adımları
1. **Yeni dal oluştur:**
   ```bash
   git checkout modern_YZ_05
   git checkout -b modern_YZ_06
   ```

2. **İncele:**
   - `compiler/stage0/modules/control_flow/control_flow_parser.s` - mevcut for loop desteği var mı?
   - `compiler/stage0/modules/functions/functions_codegen_llvm.c` - STMT_FOR handling
   - Runtime: list iteration için gerekli fonksiyonlar

3. **Test suite oluştur:**
   - `tests/phase1/test_for_numeric.mlp`
   - `tests/phase1/test_for_reverse.mlp`
   - `tests/phase1/test_for_list.mlp`

4. **Implement:**
   - Parser: for...to...step sözdizimi
   - Codegen: LLVM IR loop generation
   - List iteration: melp_list_get kullanarak

### Önemli Bilgiler
- While loops zaten çalışıyor (YZ_301'de tamamlandı)
- List API: `melp_list_length()`, `melp_list_get()` mevcut
- LLVM loop pattern: cond_label → body_label → inc_label → cond_label

## 📂 Önemli Dosyalar
- **TODO:** `TODO_MODERN_FINAL.md` (tüm modern features)
- **Kurallar:** `TODO_kurallari.md` (görev formatı ve kurallar)
- **Test klasörü:** `/tests/phase1/`
- **Compiler:** `/compiler/stage0/`

## 🔍 Hızlı Referans

### Compiler Modülleri
- **Parser:** `/compiler/stage0/modules/*/`
- **Codegen:** `/compiler/stage0/modules/functions/functions_codegen_llvm.c`
- **LLVM Backend:** `/compiler/stage0/modules/llvm_backend/`
- **Runtime:** `/runtime/stdlib/`

### Build & Test
```bash
# Compiler rebuild
cd /compiler/stage0 && make modules

# Test specific file
./compiler/stage0/modules/functions/functions_compiler test.mlp test.ll
clang -o test test.ll ./runtime/stdlib/libmlp_stdlib.a
./test

# Test suite
./tests/phase1/run_tests.sh
```

### Git Workflow
```bash
# Yeni görev başlat
git checkout modern_YZ_05
git checkout -b modern_YZ_06

# Tamamlandığında
git add .
git commit -m "modern_YZ_06: For Loops complete"
git push origin modern_YZ_06

# Completion report yaz
# MODERN_LANGUAGE_PROCESS/modern_YZ/modern_YZ_06_TAMAMLANDI.md
```

## 📖 İlgili Dokümanlar
- **MELP Language:** `MELP_REFERENCE.md`
- **Architecture:** `ARCHITECTURE.md`
- **Phase 3 Summary:** `docs/PHASE3_SUMMARY.md`

## 🚨 Önemli Notlar
1. **modern_YZ branch'i kullan** (selfhosting değil!)
2. **Stage 0 compiler'ı düzenle** (C dili ile yazılmış)
3. **Her zaman test suite oluştur** (run_tests.sh pattern'i takip et)
4. **TAMAMLANDI.md dosyası yaz** (task bitince)
5. **TODO_kurallari.md'ye uy** (görev formatı önemli)

---
**Hazır mısın?** "Task 1.2'yi başlatalım!" de ve for loops implementasyonuna başla! 🚀
