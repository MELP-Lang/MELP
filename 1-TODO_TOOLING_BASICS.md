# 1-TODO_TOOLING_BASICS.md

**Hedef:** Developer Experience İyileştirme  
**Süre:** 2-3 hafta (Hafta 7-9)  
**Öncelik:** Yüksek  
**Bağımlılık:** 0-TODO_SELFHOSTING.md (%100 tamamlanmış olmalı)

---

## 📋 GENEL BAKIŞ

TODO #0 tamamlandığında MLP-GCC çalışan bir derleyici olacak ama **developer experience** zayıf olacak:
- ❌ Error mesajları kısa ve belirsiz
- ❌ Debug yapmak zor (stack trace yok)
- ❌ IDE desteği yok (autocomplete, syntax check)
- ❌ Code formatting manuel

Bu TODO **temel tooling altyapısını** kuracak.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

**TODO tamamlandığında:**
- ✅ Error mesajları detaylı ve renkli (Rust/Clang seviyesi)
- ✅ GDB ile debug edilebilir (DWARF debug symbols)
- ✅ Basic LSP çalışıyor (autocomplete, go-to-definition)
- ✅ Stack traces panic'te gösteriliyor
- ✅ mlp-fmt code formatter çalışıyor

---

## 📊 TASK BREAKDOWN

### **Task 1: Enhanced Error Messages** (3-4 gün)

**Hedef:** Rust/Clang seviyesinde detaylı error mesajları

**Mevcut Durum:**
```
Error: Undefined variable 'x'
```

**Hedef Durum:**
```
Error: Cannot find value 'x' in this scope
  --> examples/test.mlp:5:10
   |
 5 |     yazdir(x)
   |            ^ not found in this scope
   |
help: Maybe you meant 'y'?
```

**Detay:**
- [ ] Error position tracking (satır, kolon)
- [ ] Code snippet gösterimi (^^ işaretleme)
- [ ] Renkli çıktı (ANSI colors)
- [ ] "Did you mean?" önerileri
- [ ] Türkçe/İngilizce error mesajları
- [ ] Error code'lar (E0001, E0002...)

**Test:**
```bash
# Hatalı kod derle, error mesajını kontrol et
./mlp-gcc examples/error_test.mlp 2>&1 | grep "help:"
```

---

### **Task 2: GDB Integration** (2-3 gün)

**Hedef:** GDB ile debug edilebilir binary'ler

**Gereksinimler:**
- [ ] DWARF debug symbols üret (`-g` flag)
- [ ] Source mapping (MLP → C → Binary)
- [ ] Variable inspection (gdb'de değişken göster)
- [ ] Breakpoint support
- [ ] Stack frame navigation

**Dosyalar:**
- `MELP/C/stage0/modules/codegen/debug_info.c` (~300 satır)
  - DWARF section generation
  - Line number mapping

**Test:**
```bash
# Debug symbols ile derle
./mlp-gcc -g examples/test.mlp -o test

# GDB ile çalıştır
gdb ./test
(gdb) break main
(gdb) run
(gdb) print x  # MLP değişkenini göster
```

---

### **Task 3: Basic LSP Server** (5-6 gün)

**Hedef:** IDE entegrasyonu için LSP (Language Server Protocol)

**Özellikler:**
- [ ] Autocomplete (fonksiyon, değişken)
- [ ] Go-to-definition (F12)
- [ ] Hover tooltips (tip bilgisi)
- [ ] Syntax check (real-time errors)
- [ ] Document symbols (outline view)

**Dosyalar:**
- `MELP/C/tools/mlp-lsp/` (yeni dizin)
  - `lsp_server.c` (~400 satır)
  - `json_rpc.c` (~300 satır)
  - `workspace.c` (~250 satır)

**LSP Methods (Minimum):**
```json
{
  "initialize": "✅",
  "textDocument/didOpen": "✅",
  "textDocument/didChange": "✅",
  "textDocument/completion": "✅",
  "textDocument/definition": "✅",
  "textDocument/hover": "✅"
}
```

**Test:**
```bash
# LSP server başlat
./mlp-lsp --stdio

# VS Code'da test et (sonraki TODO'da extension yazılacak)
# Şimdilik netcat ile test
echo '{"jsonrpc":"2.0","id":1,"method":"initialize"}' | nc localhost 8080
```

---

### **Task 4: Stack Traces** (2 gün)

**Hedef:** Panic/crash'te detaylı stack trace

**Mevcut Durum:**
```
Panic: Division by zero
Segmentation fault
```

**Hedef Durum:**
```
Panic: Division by zero
Stack trace:
  0: calculate (math_utils.mlp:42)
  1: process_data (main.mlp:18)
  2: main (main.mlp:5)
```

**Gereksinimler:**
- [ ] Runtime stack unwinding
- [ ] Function name resolution
- [ ] Source file mapping
- [ ] Renkli output

**Dosyalar:**
- `MELP/runtime/panic.c` (güncelle ~150 satır ekle)
- `MELP/runtime/stacktrace.c` (yeni ~250 satır)

**Test:**
```bash
# Panic oluşturan kod
echo 'function test() numeric x = 10 / 0 end_function' > panic_test.mlp
./mlp-gcc panic_test.mlp && ./a.out
# Stack trace görmeli
```

---

### **Task 6: Help Documentation System** (2-3 gün)

**Hedef:** Kod içi yardım sistemi (inline help)

**Özellikler:**
- [ ] `--help` flags tüm komutlarda
- [ ] `mlp help <topic>` command (e.g., `mlp help functions`)
- [ ] Inline documentation comments (`/** ... */` DocComments)
- [ ] Code hover tooltips (LSP ile entegre)
- [ ] Man pages (`man mlp`, `man mlp-fmt`)

**DocComment Syntax:**
```mlp
/**
 * Calculates sum of two numbers
 * @param a First number
 * @param b Second number  
 * @return Sum of a and b
 * @example
 *   result = add(2, 3)  # returns 5
 */
function add(numeric a, numeric b) as numeric
    return a + b
end_function
```

**Help Command:**
```bash
# General help
mlp help

# Topic-specific help
mlp help functions
mlp help types
mlp help stdlib

# Man pages
man mlp
man mlp-fmt
man mlp-lsp
```

**Dosyalar:**
```
MELP/C/tools/mlp-help/
├── help_system.c      (~300 satır)
├── help_topics.c      (~400 satır)
└── docstring_parser.c (~250 satır)

docs/help/
├── functions.md
├── types.md
├── stdlib.md
└── examples.md
```

**Test:**
```bash
mlp help functions  # Fonksiyon syntaxını göster
mlp help types      # Tip sistemini göster
```

---

### **Task 5: Code Formatter (mlp-fmt)** (3-4 gün)

**Hedef:** Otomatik code formatting (gofmt, rustfmt gibi)

**Özellikler:**
- [ ] Indentation düzeltme
- [ ] Whitespace normalizasyonu
- [ ] Consistent style (tek stil, tartışma yok)
- [ ] `--check` mode (CI için)
- [ ] `--diff` mode (değişiklikleri göster)

**Stil Kuralları:**
```mlp
-- İndent: 4 space
function example()
    numeric x = 42
    if x > 0 then
        yazdir("pozitif")
    end_if
end_function

-- Operators: space around
numeric result = x + y    -- ✅
numeric result = x+y      -- ❌ (düzeltilecek)

-- Function calls: no space before (
func(x, y)    -- ✅
func (x, y)   -- ❌
```

**Dosyalar:**
- `MELP/C/tools/mlp-fmt/` (yeni dizin)
  - `formatter.c` (~400 satır)
  - `style_rules.c` (~200 satır)

**Test:**
```bash
# Kötü formatlanmış kod
echo 'function test()numeric x=42 if x>0 then yazdir("test")end_if end_function' > ugly.mlp

# Format et
./mlp-fmt ugly.mlp

# Kontrol et
./mlp-fmt --check ugly.mlp
echo $?  # 0 = formatted, 1 = needs formatting
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100)
  - Compiler çalışıyor olmalı
  - Import sistemi hazır
  - Runtime modüler

### Sonraki TODO:
- **2-TODO_STDLIB_EXPAND.md**
  - LSP daha iyi stdlib bilgisi kullanabilecek
  - Formatter stdlib kodunu da formatlar

---

## 🧪 TEST STRATEJİSİ

### Unit Tests

```bash
# Her task için ayrı test
tests/tooling/test_error_messages.sh
tests/tooling/test_gdb_integration.sh
tests/tooling/test_lsp_basic.sh
tests/tooling/test_stacktrace.sh
tests/tooling/test_formatter.sh
```

### Integration Tests

```bash
# End-to-end senaryo
# 1. Hatalı kod yaz
echo 'function test() x = undeclared end_function' > test.mlp

# 2. Error mesajını kontrol et
./mlp-gcc test.mlp 2>&1 | grep "not found in this scope"

# 3. Debug symbols ile derle
./mlp-gcc -g examples/fib.mlp -o fib
gdb -batch -ex "break fib" -ex "run" ./fib

# 4. LSP test
./mlp-lsp --stdio < lsp_test_input.json

# 5. Format test
./mlp-fmt examples/*.mlp --check
```

### Performance Tests

```bash
# LSP response time (autocomplete <100ms)
time echo '{"method":"completion"}' | ./mlp-lsp --stdio

# Formatter hızı (1000 satır <1s)
time ./mlp-fmt large_file.mlp
```

---

## 📦 DELIVERABLES

### Yeni Araçlar

```
MELP/C/tools/
├── mlp-lsp/          ← LSP server
│   ├── lsp_server.c
│   ├── json_rpc.c
│   └── workspace.c
│
└── mlp-fmt/          ← Code formatter
    ├── formatter.c
    └── style_rules.c
```

### Güncellenmiş Modüller

```
MELP/C/stage0/modules/
├── codegen/
│   └── debug_info.c  ← DWARF symbols (yeni)
│
└── error/
    └── enhanced_errors.c  ← Detaylı errors (güncelle)

MELP/runtime/
├── panic.c           ← Stack trace (güncelle)
└── stacktrace.c      ← Stack unwinding (yeni)
```

### Dokümantasyon

```
docs/
├── TOOLING_GUIDE.md       ← Araç kullanım kılavuzu
├── ERROR_CODES.md         ← Error code referansı
├── LSP_SPEC.md            ← LSP implementasyonu
└── FORMATTER_RULES.md     ← Stil kuralları
```

---

## 🎯 BAŞARI KRİTERLERİ

### Minimum Viable Product (MVP)

- [ ] Error mesajları Rust/Clang seviyesinde
- [ ] GDB ile debug edilebilir
- [ ] LSP basic özellikler (autocomplete, goto-def)
- [ ] Stack trace panic'te gösteriliyor
- [ ] mlp-fmt çalışıyor ve consistent stil uyguluyor

### Nice to Have (Bonus)

- [ ] Error codes dokümantasyonu (web sitesi)
- [ ] LSP hover'da example code
- [ ] Formatter custom config (mlp-fmt.toml)
- [ ] Stack trace HTML çıktısı (CI için)
- [ ] Performance profiling (--profile flag)

### Regression Tests

```bash
# Tüm testler geçmeli
./run_tests.sh tooling
# Expected: 100% pass (0 failures)

# Performance benchmark
./benchmark_tooling.sh
# Expected: LSP <100ms, Formatter 1000 LOC/s
```

---

## 📅 TARİHSEL CONTEXT

### Neden Bu TODO Gerekli?

**P1 (mlp-original) Deneyimi:**
- Error mesajları berbattı → Debug imkansızdı
- IDE desteği yoktu → Kod yazmak zordu
- Tooling yoktu → Adoption düşüktü

**P3'te Fark:**
- ✅ İlk günden tooling planı
- ✅ Developer experience öncelik
- ✅ Modern standartlarda (LSP, formatters)

### Benchmark Hedefleri

| Özellik | Rust | Go | MLP-GCC Hedef |
|---------|------|-----|---------------|
| Error quality | 10/10 | 7/10 | 9/10 |
| LSP response | <50ms | <30ms | <100ms |
| Formatter speed | 5K LOC/s | 10K LOC/s | 1K LOC/s |
| Debug symbols | ✅ | ✅ | ✅ |

---

## 🚀 BAŞLANGIÇ KOMUTU

```bash
# TODO dizini oluştur
cd /home/pardus/projeler/MLP/MLP-GCC
./TODO_OLUSTUR/setup_todo.sh TOOLING_BASICS 1-TODO_TOOLING_BASICS 1-TODO_TOOLING_BASICS.md

# UA ata
cd TODO_TODO_TOOLING_BASICS/TOOLING_BASICS_UA
# UA bu TODO'yu 5 task'a böler, her task için YZ atar
```

---

## 📝 NOTLAR

### P2'den Transfer Edilecek

**P2 (MLP-LLVM)'de bazı tooling kodları var:**
- Error handling mekanizması (~500 satır)
- JSON parser (LSP için kullanılabilir)

**Transfer Planı:**
- Task 1'de P2 error handling'i incele
- Task 3'te P2 JSON parser'ı al (LLVM bağımlılığını temizle)

### Teknik Kararlar

**LSP Implementation:**
- C ile (performance için)
- JSON-RPC 2.0 protokolü
- Stdin/stdout communication (VS Code uyumlu)

**Debug Info:**
- DWARF format (GDB standart)
- C source mapping (MLP → C → Binary)
- Inline functions için special handling

**Formatter:**
- AST-based (token-based değil)
- Idempotent (iki kez format = aynı sonuç)
- No config (opinionated, like gofmt)

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)
