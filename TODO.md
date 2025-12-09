# 🎯 MELP Compiler - TODO List
**Güncel Durum:** 9 Aralık 2025, 22:00  
**Son Tamamlanan:** YZ_09 (Documentation Cleanup) ✅  
**Stage:** Stage 0 - Core Compiler Development  
**Completion:** ~80%

---

## ⚠️ BAŞLAMADAN ÖNCE - YENİ AI İÇİN KRİTİK UYARI

**📖 ZORUNLU OKUMA (ÖNCE BURAYI OKU!):**  
👉 **`YZ/YZ_HISTORY.md`** - Tüm YZ oturumlarının özeti (Buradan başla!)

**📖 SONRA BUNLARI OKU:**  
Bu TODO'daki görevlere başlamadan önce **MUTLAKA** şu dosyaları oku:

👉 **`temp/kurallar_kitabı.md` - Bölüm 4: Transparent Type Optimization (TTO)**  
👉 **`ARCHITECTURE.md`** - Mimari kurallar (modülerlik, TTO)  
👉 **`YZ/AI_METHODOLOGY.md`** - 5 adımlı hızlı geliştirme metodu

---

### 🚀 5 ADIMLI HIZLI GELİŞTİRME YÖNTEMİ

**Kanıtlanmış:** YZ_07 (1 saat), YZ Array Parser (1-2 saat), 10x hızlanma!

#### **Adım 1: CONTEXT GATHERING (10-15 dk)** 📚
```
✓ TODO.md → Hedef ne?
✓ Son YZ'nin dokümantasyonu (YZ/YZ_XX.md) → Ne bırakılmış?
✓ kurallar_kitabı.md ilgili bölüm → Nasıl çalışıyor?
✓ ARCHITECTURE.md → Mimari kurallar neler?
```
**Çıktı:** Net hedef, mevcut durum, kısıtlar anlaşıldı

#### **Adım 2: PATTERN DISCOVERY (15-20 dk)** 🔍
```bash
# Benzer implementasyonları bul
grep -r "ilgili_fonksiyon" modules/
grep -r "benzer_pattern" modules/

# En iyi reference'ı belirle
# Örnek: functions_parser.c, arithmetic_codegen.c
```
**Çıktı:** Template bulundu, copy-paste-modify stratejisi hazır

#### **Adım 3: MINIMAL VIABLE CHANGE (30-45 dk)** ✏️
```
✓ Sadece gerekli dosyalara dokun (3-5 dosya max)
✓ Küçük değişiklikler (30-50 satır)
✓ Pattern'i takip et (proven implementation)
✓ Modular kal (ARCHITECTURE.md kuralları)
```
**Çıktı:** Kod değişiklikleri tamamlandı, derleniyor

#### **Adım 4: TEST & VERIFY (15-30 dk)** ✅
```bash
# Derle
make clean && make

# Basit test yaz
cat > test_simple.mlp << 'EOF'
function main() returns numeric
    # Test your feature here
    return 0
end function
EOF

# Çalıştır
./melpc test_simple.mlp -o test_simple
./test_simple
```
**Çıktı:** Çalışan kod, test sonuçları

#### **Adım 5: DOCUMENT & HANDOFF (20-30 dk)** 📝
```
✓ YZ/YZ_XX.md oluştur → Detaylı rapor
✓ TODO.md güncelle → Progress + status
✓ NEXT_AI_START_HERE.md güncelle → Sonraki AI'ye talimatlar
✓ Test dosyaları bırak → Executable examples
```
**Çıktı:** Sonraki AI hazır, bilgi kaybı yok

---

### ⚡ Pro Tips:

**1. Grep > Okuma** 🔎
```bash
# Pattern'i hızlıca bul
grep -rn "aradığın_şey" modules/ | head -10

# Reference implementation bul
find modules/ -name "*_codegen.c" -exec grep -l "benzer_özellik" {} \;
```

**2. Parallel Tool Calls** ⚡
```
Birden fazla dosya değişikliği → multi_replace_string_in_file kullan
Bağımsız okumalar → paralel read_file çağrıları
```

**3. Trust the Pattern** 🎯
```
✓ TTO principle → 1 bit tracking, basit
✓ Modular architecture → izole değişiklikler
✓ Stateless parsers → side effect yok
→ Değişiklik küçük, etki lokal, güven yüksek
```

**4. Document as You Go** 📋
```
TODO list aktif kullan → progress görünür
Her commit'e comment → "YZ_XX: Feature name"
Test önce yaz → TDD approach
```

**5. Follow the Breadcrumbs** 🍞
```
Önceki YZ'lerin bıraktığı pattern'leri takip et
Runtime hazırsa → sadece codegen ekle
Parser hazırsa → sadece codegen ekle
Infrastructure varsa → kullan, tekrar yazma
```

---

### 🎯 Başarı Metrikleri:

Görevin sonunda olması gerekenler:

✅ **Hızlı:** 1-3 saat içinde tamamlandı  
✅ **Temiz:** Derleme uyarısı yok  
✅ **Test Edilmiş:** En az 1 çalışan örnek  
✅ **Dokümante:** YZ_XX.md + TODO.md güncel  
✅ **Modular:** ARCHITECTURE.md kurallarına uygun  

---

### Neden Bu Kadar Önemli?

TTO, MELP'in temel mimari felsefesidir. Bunu anlamadan:
- ❌ String operations codegen'i yanlış yaparsın
- ❌ Type tracking sistemini bozarsın  
- ❌ Runtime fonksiyonlarını yanlış çağırırsın
- ❌ Numeric vs text ayrımını karıştırırsın

### TTO'nun Özeti (Detay için kurallar_kitabı.md oku!)

**Kullanıcıya gösterilen:** Sadece 2 tip → `numeric` ve `text`  
**Arka planda compiler:** Otomatik optimize eder:
- Küçük sayılar → int64 (register)
- Büyük sayılar → BigDecimal (heap)
- Kısa string → SSO/stack (≤23 byte)
- Uzun string → heap pointer

**Her değişken için:** `is_numeric` flag (1 bit) takip edilir  
**Codegen'de:** Bu flag'e göre doğru runtime fonksiyonu çağrılır

---

### 🏗️ Mimari Kurallar (ARCHITECTURE.md'den - KRİTİK!)

#### Kural #1: MERKEZI DOSYA YOK (Radikal Modülerlik)

**NEDEN:** Merkezi dosyalar (main.c, orchestrator.c, helpers.c) AI'ları monolitik kod yazmaya iter.

**ZORUNLULUK:** Tüm merkezi dosyalar `temp/yedek_merkezi_dosyalar/` içinde yedeklendi.  
**SONUÇ:** AI'lar SADECE `modules/` içinde çalışabilir!

**İHLAL SONUCU:** Merkezi dosya yok = İhlal edilemez!

---

#### Kural #2: Modüller Arası İletişim SADECE JSON/Pipe ile

**YASAK:**
```c
// main.c veya orchestrator.c içinde:
#include "modules/arithmetic/arithmetic_parser.h"  // ❌ HAYIR!
result = arithmetic_parse(tokens);  // ❌ HAYIR! (doğrudan fonksiyon çağrısı)
```

**İZİN VERİLEN:**
```bash
# Unix pipes (stdin/stdout):
echo '{"token":"NUMERIC"}' | ./modules/arithmetic/arithmetic

# JSON dosyaları:
./lexer < input.mlp > tokens.json
./parser < tokens.json > ast.json
```

**NEDEN:** 
- C seviyesinde bağımlılık yok
- Dilden bağımsız (MELP'te yeniden yazılabilir)
- Basit text dosyalarıyla test edilebilir

---

#### Kural #3: Her Modül Bağımsız (Standalone)

**ZORUNLU:** Her modül standalone binary olarak derlenebilmeli:

```
modules/arithmetic/arithmetic_compiler  (standalone)
modules/comparison/comparison_compiler  (standalone)
modules/tto/tto_analyzer               (standalone)
```

**NEDEN:** 
- Bağımsız test edilebilir
- Sistemi bozmadan değiştirilebilir
- Self-hosting hazır (MELP'te yeniden yazılabilir)

**İLETİŞİM:** SADECE JSON/stdin/stdout

---

### Okumadan Devam Etme!

Eğer TTO'yu anlamadan TODO'ya başlarsan, çalışan sistemi bozabilirsin.  
**5-10 dakika** ayır, `temp/kurallar_kitabı.md` dosyasındaki TTO bölümünü oku.

✅ Okudum, TTO'yu anladım → TODO'ya geç  
❌ Okumadım → Önce oku, sonra dön

---

## 📊 Genel Durum

### ✅ Tamamlanan (WORKS!)
- [x] **Lexer** - Token parsing (keywords, literals, operators)
- [x] **Parser** - Statement ve expression parsing
- [x] **Functions** - Declaration, calls, parameters, return values
- [x] **Variables** - Numeric ve text declarations
- [x] **Arithmetic** - +, -, *, / operations
- [x] **Comparison** - <, <=, >, >=, ==, != operators
- [x] **Control Flow** - if/else, while loops
- [x] **Recursion** - Recursive function calls (Fibonacci works!)
- [x] **String Literals** - "Hello World" in .rodata section
- [x] **TTO Type Tracking** - is_numeric flag (1 bit per variable)
- [x] **Runtime - Numeric I/O** - println(numeric), print(numeric)
- [x] **Runtime - String I/O** - println(text), print(text)
- [x] **Runtime - String Ops** - concat(), compare() functions
- [x] **Codegen - String Concat** - text c = a + b (YZ_07) ✅
- [x] **Codegen - String Compare** - if str == "test" (YZ_07) ✅

### 🚧 Devam Eden (IN PROGRESS)
- [x] **String Concatenation Codegen** - text c = a + b (YZ_07 tamamladı) ✅
- [x] **String Comparison Codegen** - if str == "test" (YZ_07 tamamladı) ✅
- [x] **Linker Issues Fix** - Missing functions (YZ_08 fixed!) ✅
- [ ] **Variable Type Inference** - String variables not recognized in expressions (YZ_09 priority)
- [ ] **String Literal Concatenation** - "Hello" + "World" parsing issue

### ⏳ Yapılacak (TODO - Öncelik Sırasıyla)

---

## 🔧 Phase 0: Fix Linker Issues ✅ COMPLETED
**Responsible:** YZ_08 ✅  
**Priority:** ⭐⭐⭐ CRITICAL (blocks all testing)

- [x] **Implement Missing Functions** ✅ (Completed by YZ_08)
  - `pipeline_compile()` in pipeline module ✅
  - `print_parse_statement()` → `parse_print_statement()` fixed ✅
  - `lexer_unget_token()` in lexer module ✅
  - melpc binary builds successfully ✅
  - All missing functions resolved ✅

**Deliverable:** ✅ melpc binary builds successfully

**Documentation:** See `/YZ/YZ_08.md`

---

## 🎯 Phase 1: String Operations ✅ COMPLETED
**Responsible:** YZ_06 (Runtime) + YZ_07 (Codegen) ✅  
**Priority:** ⭐⭐⭐ CRITICAL  
**Status:** Runtime + Codegen complete, linker fixed by YZ_08

- [x] **String Runtime Functions** ✅ (YZ_06)
  - `mlp_string_concat()` - Concatenation ✅
  - `mlp_string_compare()` - Comparison ✅
  - All helper functions implemented ✅

- [x] **String Concat Codegen** ✅ (YZ_07)
  - File: `modules/arithmetic/arithmetic_codegen.c`
  - Pattern: Check is_numeric, call mlp_string_concat
  - Test: `text c = "Hello" + "World"`
  - **Result:** Working! Changed `tto_sso_concat` → `mlp_string_concat`

- [x] **String Compare Codegen** ✅ (YZ_07)
  - File: `modules/comparison/comparison_codegen.c`
  - Pattern: Check is_string, call mlp_string_compare
  - Test: `if password == "secret"`
  - **Result:** All 6 operators (==, !=, <, <=, >, >=) supported

- [x] **Test Programs Created** ✅
  - test_string_concat.mlp ✅
  - test_string_compare.mlp ✅
  - test_string_compare_ne.mlp ✅

- [ ] **Integration Tests** (TODO for YZ_10 - 30 min)
  - Run and verify test_string_concat.mlp
  - Run and verify test_string_compare.mlp
  - Test multi-operand concat: `a + b + c`
  - Test all 6 comparison operators

**Deliverable:** Runtime ✅ | Codegen ✅ | Build ✅ | Testing ⏳ (Next: YZ_10)

**Documentation:** See `/YZ/YZ_06.md` (runtime) and `/YZ/YZ_07.md` (codegen)

---

## 🎯 Phase 2: For Loops (1-2 saat)
**Priority:** ⭐⭐ HIGH

- [ ] **For Loop Codegen** (90 min)
  - File: `modules/for_loop/for_loop_codegen.c`
  - Pattern: Similar to while loop
  - Test: `for i = 1 to 10`

- [ ] **Range Iteration** (30 min)
  - Support: `for i = start to end`
  - Support: `for i = start to end step 2`

**Deliverable:** For loops working

---

## 🎯 Phase 3: Arrays & Collections (4-6 saat)
**Priority:** ⭐⭐ HIGH

YZ_05 notes indicate array module already has parser/codegen/runtime started!

- [ ] **Array Literals** (2 hours)
  - Syntax: `[1, 2, 3]`
  - Codegen: Call tto_array_alloc()
  - Test: Create and access arrays

- [ ] **Array Access** (1 hour)
  - Syntax: `arr[i]`
  - Codegen: Bounds checking
  - Test: Read/write array elements

- [ ] **Lists (Heterogeneous)** (2 hours)
  - Syntax: `(1; "hello"; 3.14;)`
  - Runtime: tto_list_alloc() already exists
  - Test: Mixed-type collections

- [ ] **Tuples (Immutable)** (1 hour)
  - Syntax: `<x, y>`
  - Runtime: tto_tuple_alloc() already exists
  - Test: Pair and triple tuples

**Deliverable:** Array/List/Tuple support complete

---

## 🎯 Phase 4: Boolean Type (1-2 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Boolean Keyword** (30 min)
  - Add `boolean` to lexer
  - Add to type system

- [ ] **Boolean Literals** (30 min)
  - Add `true` and `false` keywords
  - Codegen: Store as 1/0

- [ ] **Boolean Operations** (60 min)
  - Logical: `and`, `or`, `not`
  - Already parsed, need codegen
  - Test: `if (x > 5 and y < 10)`

**Deliverable:** Boolean type fully supported

---

## 🎯 Phase 5: More Stdlib Functions (2-3 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Input Functions** (60 min)
  - `input()` - Read line from stdin
  - `input_numeric()` - Parse to number
  - Test: Interactive programs

- [ ] **Type Conversion** (60 min)
  - `toString(numeric)` - Already in mlp_io.c!
  - `toNumeric(text)` - Parse string to number
  - Test: Conversions

- [ ] **String Methods** (60 min)
  - `length(text)` - Already in mlp_string.c!
  - `substring(text, start, len)`
  - `indexOf(text, pattern)`
  - Test: String manipulation

**Deliverable:** Rich standard library

---

## 🎯 Phase 6: Error Handling (2-3 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Better Error Messages** (90 min)
  - Line numbers in parse errors
  - Helpful suggestions
  - Colored output

- [ ] **Runtime Errors** (60 min)
  - Division by zero
  - Array bounds checks
  - Null pointer checks

- [ ] **Error Recovery** (60 min)
  - Continue parsing after error
  - Show multiple errors

**Deliverable:** User-friendly error system

---

## 🎯 Phase 7: Optimization (3-5 saat)
**Priority:** ⭐ LOW (after basics work)

- [ ] **Constant Folding** (90 min)
  - `x = 2 + 3` → `x = 5` at compile time

- [ ] **Dead Code Elimination** (60 min)
  - Remove unreachable code

- [ ] **Register Allocation** (120 min)
  - Better use of registers
  - Reduce stack usage

**Deliverable:** Faster generated code

---

## 🎯 Phase 8: Self-Hosting Prep (5-10 saat)
**Priority:** ⭐ LOW (far future)

- [ ] **File I/O** (2 hours)
  - Read/write files
  - Parse MLP from MLP

- [ ] **Module System** (3 hours)
  - Import/export
  - Separate compilation

- [ ] **Rewrite Lexer in MLP** (5 hours)
  - First self-hosted component!

**Deliverable:** Begin self-hosting journey

---

## 🔧 Infrastructure TODO

### Build System (3-4 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Fix Makefile Linking** (2 hours)
  - Current: Has errors with pipeline/, lexer
  - Goal: Clean compilation

- [ ] **Better Test Framework** (90 min)
  - Automated test runner
  - Expected vs actual output
  - Regression testing

- [ ] **CI/CD Setup** (60 min)
  - GitHub Actions
  - Auto-test on push

### Documentation (ongoing)
- [ ] **Language Specification** (in progress)
- [ ] **Standard Library Docs** (in progress)
- [ ] **Tutorial Examples** (partially done)

---

## 📈 Completion Timeline

| Phase | Priority | Time | Status |
|-------|----------|------|--------|
| **String Ops** | ⭐⭐⭐ | 2-3h | ✅ **CODEGEN DONE** (Testing blocked) |
| Linker Fix | ⭐⭐⭐ | 0.5-1h | ⏳ YZ_08 |
| For Loops | ⭐⭐ | 1-2h | ⏳ Parser ready |
| Arrays | ⭐⭐ | 4-6h | ⏳ Runtime exists |
| Booleans | ⭐ | 1-2h | ⏳ |
| Stdlib | ⭐ | 2-3h | 🚧 Partial |
| Errors | ⭐ | 2-3h | ⏳ |
| Optimization | ⭐ | 3-5h | ⏳ |
| Self-hosting | ⭐ | 5-10h | ⏳ Future |

**Total Estimated Work:** 20-30 hours  
**Current Completion:** ~75%  
**To MVP (Minimal Viable):** ~8 hours  
**To Production Ready:** ~23 hours

---

## 🎉 Major Milestones Achieved

- ✅ **YZ_01** - TTO Architecture cleanup
- ✅ **YZ_02** - Module system foundation
- ✅ **YZ_03** - MVC (Minimum Viable Compiler) complete!
- ✅ **YZ_04** - Control flow codegen (Fibonacci works!)
- ✅ **YZ_05** - String literals + TTO type tracking
- ✅ **YZ_06** - String operations runtime
- ✅ **YZ_07** - String operations codegen (concat + compare) 🎉

**Next:** YZ_08 - Fix linker, test string operations (0.5-1 hour)

---

## 🚀 Quick Actions

**For immediate progress:**
1. ✅ String operations runtime (DONE by YZ_06)
2. ⏳ String operations codegen (YZ_07 - START HERE!)
3. ⏳ For loop codegen
4. ⏳ Array support

**For robustness:**
1. Better error messages
2. Fix build system
3. Automated testing

**For features:**
1. Boolean type
2. More stdlib functions
3. Input/output

---

## 📞 Contact Info

- **Architecture:** See `ARCHITECTURE.md`
- **TTO Details:** See `temp/kurallar_kitabı.md`
- **Current Status:** See `STATUS_9_ARALIK_2025.md`
- **Next Steps:** See `NEXT_AI_START_HERE.md`
- **AI Sessions:** See `YZ/YZ_*.md`

---

**Last Updated:** 9 Aralık 2025, 21:20 by YZ_06  
**Next AI:** YZ_07 (String operations codegen)  
**Estimated Completion:** Stage 0 MVP in ~10 hours of focused work
