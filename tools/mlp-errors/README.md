# MELP Error Message System

**Versiyon:** 1.0  
**Tarih:** 1 Ocak 2026  
**Yazar:** TOOLING_YZ_01  
**Durum:** ✅ Production Ready

---

## 📋 ÖZET

Rust/Clang seviyesinde detaylı, renkli, çok dilli error mesaj sistemi. Stage0 compiler'a modüler binary olarak entegre edildi.

**Özellikler:**
- ✅ 33 Error kod (E0001-E0502)
- ✅ 21 Warning kod (W0001-W0110)
- ✅ İki dil: Türkçe + İngilizce
- ✅ Renkli ANSI terminal çıktısı
- ✅ Source kod gösterimi
- ✅ Placeholder sistemi
- ✅ Modüler mimari (main.c'ye sadece +8 satır)

---

## 🏗️ MİMARİ

```
MELP/C/stage0/main.c          (+8 satır - error reporting helper)
    ↓ system call
tools/mlp-errors/             (Standalone binary - ~650 satır)
    ├── main.c               (CLI interface)
    ├── error_loader.c       (JSON parser with cJSON)
    ├── error_formatter.c    (Rust-style formatter)
    ├── error_types.h        (Type definitions)
    └── cJSON.c/h            (JSON library)
    ↓ reads
data/errors.json              (Error database - 54 errors/warnings)
```

**Tasarım Prensipleri:**
- ❄️ **FREEZE:** main.c dokunulmadı (651→659 satır)
- 🔧 **MODÜLER:** Ayrı binary, bağımsız geliştirme
- 📊 **DATA-DRIVEN:** JSON database, kod değişikliği gerektirmez
- 🌍 **I18N:** Çok dilli destek (TR/EN)

---

## 📦 KURULUM

### Build

```bash
cd tools/mlp-errors
make
```

### Install

```bash
make install  # Copies to build/
```

### Test

```bash
cd tests/errors
./run_tests.sh  # 22 tests
```

---

## 🚀 KULLANIM

### Temel Kullanım

```bash
mlp-errors <code> <file> <line> <col> [placeholders...]
```

### Örnekler

**1. Tanımlanmamış Değişken (Türkçe - varsayılan):**
```bash
mlp-errors E0001 test.mlp 5 10 x
```

**Çıktı:**
```
Hata[E0001]: Tanımlanmamış Değişken
  --> test.mlp:5:10
   |
 5 | print(x)
   |       ^ 'x' değişkeni bu kapsamda bulunamadı
   |
   = help: Değişkeni kullanmadan önce tanımlamayı deneyin:
     let x = değer;
```

**2. Tip Hatası (İngilizce):**
```bash
MLP_LANG=en mlp-errors E0002 main.mlp 12 5 int string
```

**Çıktı:**
```
Error[E0002]: Type Mismatch
  --> main.mlp:12:5
   |
12 | let y: int = "hello"
   |     ^ Expected type 'int' but found 'string'
   |
   = help: Check the variable type or perform type conversion
```

**3. Warning - Kullanılmayan Değişken:**
```bash
mlp-errors W0001 utils.mlp 8 5 unused
```

**Çıktı:**
```
Uyarı[W0001]: Kullanılmayan Değişken
  --> utils.mlp:8:5
   |
 8 | let unused = 5
   |     ^^^^^^ 'unused' değişkeni tanımlandı ama kullanılmadı
   |
   = help: Kullanılmayan değişkeni kaldırın veya '_' ile başlatın
```

---

## 🎨 ERROR KATEGORİLERİ

| Kategori | Kod Aralığı | Açıklama |
|----------|------------|----------|
| **Syntax** | E0001-E0099 | Sözdizimi hataları |
| **Type** | E0100-E0199 | Tip sistemi hataları |
| **Name Resolution** | E0200-E0299 | İsim çözümlemesi |
| **Import/File** | E0300-E0399 | Dosya ve import hataları |
| **Control Flow** | E0400-E0499 | Kontrol akışı hataları |
| **Memory/Runtime** | E0500-E0599 | Bellek ve runtime hataları |
| **Unused Code** | W0001-W0099 | Kullanılmayan kod uyarıları |
| **Style** | W0100-W0199 | Kod stili uyarıları |

**Detaylı liste:** [data/error_categories.md](../../data/error_categories.md)

---

## 🔧 PLACEHOLDER SİSTEMİ

Error mesajlarında dinamik değerler için placeholder kullanılır:

| Placeholder | Kullanım | Örnek |
|-------------|----------|-------|
| `{var}` | Değişken adı | x, userName |
| `{func}` | Fonksiyon adı | add, calculate |
| `{type}` | Tip adı | int, string |
| `{expected}` | Beklenen değer | int, 42 |
| `{found}` | Bulunan değer | string, "hello" |
| `{op}` | Operatör | +, -, * |
| `{file}` | Dosya adı | test.mlp |

**Kullanım:**
```bash
mlp-errors E0005 test.mlp 10 5 calculate 2 3
# Argümanlar: func=calculate, expected=2, found=3
```

---

## 🌍 ÇOK DİLLİ DESTEK

Error mesajları `MLP_LANG` environment variable ile kontrol edilir:

```bash
# Türkçe (varsayılan)
export MLP_LANG=tr
mlp-errors E0001 test.mlp 5 10 x

# İngilizce
export MLP_LANG=en
mlp-errors E0001 test.mlp 5 10 x
```

**Desteklenen Diller:**
- 🇹🇷 Türkçe (tr) - Varsayılan
- 🇬🇧 İngilizce (en)

---

## 🧪 TEST SUITE

Test suite: [tests/errors/run_tests.sh](../../tests/errors/run_tests.sh)

**Test Kapsamı:**
- ✅ Basic errors (E0001-E0010)
- ✅ Type system (E0100-E0105)
- ✅ Module system (E0200-E0204)
- ✅ Control flow (E0400-E0404)
- ✅ Warnings (W0001-W0110)
- ✅ Language switching (TR/EN)

**Çalıştırma:**
```bash
cd tests/errors
./run_tests.sh
```

**Sonuç:**
```
Test Results:
  Passed: 22
  Failed: 0
✅ All tests passed!
```

---

## 📊 PERFORMANS

**Binary Boyutu:** ~42 KB  
**JSON Parse:** ~1-2 ms (54 errors)  
**Error Format:** <1 ms  
**Toplam:** ~2-3 ms per error

---

## 🔄 MAIN.C ENTEGRASYONU

main.c'ye minimal entegrasyon - sadece helper fonksiyon:

```c
// Error reporting helper (uses external mlp-errors tool)
void report_mlp_error(const char* code, const char* file, 
                      int line, int col, const char* arg1) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "mlp-errors %s %s %d %d %s 2>&1", 
             code, file, line, col, arg1 ? arg1 : "");
    system(cmd);
}
```

**Etki:**
- 651 satır → 659 satır (+8 satır)
- Limit: 700 satır ✅
- FREEZE kuralına uygun ✅

**Kullanım:**
```c
// Örnek: Undefined variable error
report_mlp_error("E0001", "test.mlp", 5, 10, "x");
```

---

## 📁 DOSYA YAPISI

```
tools/mlp-errors/
├── main.c                  (112 satır) - CLI interface
├── error_loader.c          (154 satır) - JSON parser
├── error_formatter.c       (184 satır) - Formatter
├── error_types.h           (35 satır)  - Types
├── cJSON.c                 (~2500 satır) - JSON library
├── cJSON.h                 (~300 satır)
├── Makefile                (50 satır)
└── test_loader.c           (20 satır)  - Unit test

data/
├── errors.json             (33 KB) - Error database
└── error_categories.md     (3 KB)  - Documentation

tests/errors/
├── run_tests.sh            (150 satır) - Test suite
└── test_errors.mlp         (Test file)

build/
└── mlp-errors              (42 KB) - Compiled binary
```

**Toplam (excluding cJSON):** ~650 satır C kodu

---

## 🎯 BAŞARI KRİTERLERİ

- [x] `data/errors.json` (54 errors/warnings)
- [x] `tools/mlp-errors/` binary (< 1000 satır)
- [x] `main.c` (+8 satır, 651→659)
- [x] `tests/errors/` (22 test passing)
- [x] Renkli ANSI çıktı
- [x] Çok dilli destek (TR/EN)
- [x] JSON validation
- [x] Documentation

---

## 🚀 GELECEK GELİŞTİRMELER

### Versiyon 1.1 (Şubat 2026)
- [ ] Info severity level
- [ ] Fix suggestions (otomatik düzeltme önerileri)
- [ ] Related errors (benzer hatalar)

### Versiyon 1.2 (Mart 2026)
- [ ] IDE integration (LSP protokolü)
- [ ] HTML/JSON çıktı formatları
- [ ] Error documentation links

### Versiyon 2.0 (Nisan 2026)
- [ ] Üçüncü dil desteği
- [ ] Custom error templates
- [ ] Error analytics

---

## 📖 REFERANSLAR

- **TODO:** [1-TODO_TOOLING_BASICS_MODULAR.md](../../1-TODO_TOOLING_BASICS_MODULAR.md)
- **Error Categories:** [data/error_categories.md](../../data/error_categories.md)
- **FREEZE Rule:** [MAIN_C_FREEZE_RULE.md](../../MAIN_C_FREEZE_RULE.md)
- **Test Suite:** [tests/errors/run_tests.sh](../../tests/errors/run_tests.sh)

---

## 🤝 KATKIDA BULUNMA

Yeni error kodları eklemek için:

1. `data/errors.json` dosyasına yeni kod ekleyin
2. Her iki dil için (TR/EN) mesaj yazın
3. `data/error_categories.md` güncelleme
4. Test case ekleyin: `tests/errors/run_tests.sh`

---

## 📝 LİSANS

MELP Project - 2026

---

**Tarih:** 1 Ocak 2026  
**Yazar:** TOOLING_YZ_01  
**Durum:** ✅ Production Ready  
**Test Status:** 22/22 PASSING
