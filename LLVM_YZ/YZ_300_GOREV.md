# YZ_300: Self-Hosting Completion 🚀
**Başlangıç:** 22 Aralık 2025  
**Öncelik:** 🔴 **KRİTİK** (Tüm gelecek development buna bağlı)  
**Tahmini Süre:** 3-5 gün  
**Durum:** 🚀 **PHASE 1 - Critical Fixes**

---

## 📚 ÖNCE OKU - Zorunlu Belgeler

**Implementation AI, işe başlamadan önce MUTLAKA oku:**

1. **PMPL Sözdizimi:** `temp/archived_docs/pmlp_kesin_sozdizimi.md`
   - MELP'in tek ve kesin sözdizimi
   - Önemli: `end_function`, `end_if`, `end_while` (alt çizgili!)
   - Parametre ayırıcı: `;` (noktalı virgül)

2. **Stage 1 Test Sonuçları:** `LLVM_YZ/YZ_300_MODULE_TEST_RESULTS.md`
   - 107 modül test edildi
   - %90 başarı (78/87 modül çalışıyor)
   - Sadece 9 modül fix gerekiyor

3. **Stage 1 Modüller:** `compiler/stage1/modules/` (YZ_11'den restore edildi)
   - Template import kullanır (compile-time code copy)
   - C'nin `#include` ile aynı mantık
   - MELP felsefesine uygun ✅

---

## 🎯 Görev Özeti

**GÜNCEL DURUM (22 Aralık 2025):**
- ✅ Stage 1: %90 hazır (78/87 modül çalışıyor)
- ✅ YZ_11 restore tamamlandı (107 modül)
- ✅ Template import çalışıyor
- ⚠️ Sadece 9 modülde küçük fix gerekli

**HEDEF:** 9 modülü fix et → Bootstrap test → Self-compile!

---

## 📋 Phase 1: Critical Fixes (1 gün - 9 modül)

**Test Sonuçları Analizi:**
- ✅ Çalışan: 78/87 (%90)
- ❌ Hatalı: 9/87 (%10)

### Hatalı Modüller:

| Modül | Sorun | Fix Süresi |
|-------|-------|------------|
| parser_mlp/ast_nodes.mlp | Syntax error line 102 | 5 dk |
| enums/enums_codegen.mlp | Stateless violation (g_enum_registry) | 10 dk |
| control_flow/control_flow_codegen.mlp | LLVM IR syntax | 15 dk |
| 6 test dosyası | Parser sorunları | İhtiyaç yok |

**Toplam Fix Süresi:** ~30 dakika (test dosyaları hariç)

### 1.1. Fix: ast_nodes.mlp

**Dosya:** `compiler/stage1/modules/parser_mlp/ast_nodes.mlp`  
**Sorun:** Line 102 - Parser error (muhtemelen end_function vs end function)

**Yapılacak:**
```bash
# 1. Hatayı göster
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/parser_mlp/ast_nodes.mlp \
  temp/ast_test.s 2>&1 | grep error

# 2. Fix uygula (PMPL syntax'a göre)
# 3. Test et
```

### 1.2. Fix: enums_codegen.mlp

**Dosya:** `compiler/stage1/modules/enums/enums_codegen.mlp`  
**Sorun:** Line 47 - Global mutable state (stateless violation)

**Mevcut Kod:**
```mlp
list g_enum_registry = []  -- ❌ GLOBAL STATE

function register_enum(list enum_node) returns boolean
    g_enum_registry = append(g_enum_registry, entry)
    return true
end_function
```

**Fix (commit 4d744fc2'den):**
```mlp
-- ✅ NO GLOBAL - Registry parametre olarak geçilir

function register_enum(list enum_node; list enum_registry) returns list
    list updated_registry = append(enum_registry; entry)
    return [true; updated_registry]
end_function
```

**Not:** Parametre ayırıcı `;` (noktalı virgül) - PMPL syntax!

### 1.3. Fix: control_flow_codegen.mlp

**Dosya:** `compiler/stage1/modules/control_flow/control_flow_codegen.mlp`  
**Sorun:** LLVM IR syntax hataları

**Yapılacak:**
```bash
# Test et, hataları gör
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/control_flow/control_flow_codegen.mlp \
  temp/control_test.s 2>&1

# Fix uygula (LLVM IR syntax'a göre)
```

### 1.4. Test Dosyaları (OPTIONAL)

**6 test dosyası hatalı ama ÖNEMLİ DEĞİL:**
- Bunlar sadece test amaçlı
- Production compiler'da kullanılmıyor
- İsteğe bağlı fix (öncelik düşük)

---

## 📋 Phase 2: Bootstrap Test (2-3 saat)

**Hedef:** Stage 0 ile Stage 1 compiler'ını compile et

### 2.1. Basit Test Case

```mlp
-- test_simple.mlp
function add(numeric a; numeric b) returns numeric
    return a + b
end_function

function main() returns numeric
    numeric result = add(5; 10)
    return result
end_function
```

**Compile:**
```bash
# Stage 0 ile compile et
./compiler/stage0/modules/functions/functions_standalone \
  test_simple.mlp test_simple.ll

# LLVM ile çalıştır
lli test_simple.ll
# Expected: 15
```

### 2.2. Stage 1 Compiler Test

**Working Modül Sayısı:** 78 (Core + Lexer + Parser + Codegen)

**Test:**
```bash
# Stage 0 ile Stage 1 modüllerini compile et
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/compiler.mlp \
  build/stage1_compiler.ll

# Test et
lli build/stage1_compiler.ll test_simple.mlp
```

---

## 📋 Phase 3: Self-Compile (1-2 gün)

**Hedef:** Stage 1 kendini compile etsin!

### 3.1. Bootstrap Chain

**İşler:**
```
Stage 0 codegen → MELP translation:

1. LLVM IR generation:
   - Function definitions
   - Variable declarations
   - Expressions (arithmetic, logic)
   - Control flow (if, while, for)
   - Function calls
2. Type mapping (MELP → LLVM)
3. Temp variable management
4. String concatenation for LLVM IR
```

**Test:**
```melp
-- Test codegen independently
function test_codegen() returns numeric
    ASTNode func = create_test_ast()
    string llvm_ir = codegen_function(func)
    
    -- Verify LLVM IR format
    -- Should contain: "define i64 @func_name() {"
    
    return 0
end_function
```

---

## 📋 Phase 2: Compiler Driver (2-3 gün)

### 2.1. Main Compiler Loop
**Dosya:** `compiler/stage1/compiler_main.mlp`

**İşler:**
```melp
-- Full compilation pipeline
function compile_source(source_file; output_file) returns numeric
    -- 1. Read source file
    string source = read_file(source_file)
    
    -- 2. Lexer
    Lexer lex = lexer_create(source)
    
    -- 3. Parser
    Parser parser = parser_create(lex)
    list functions = parse_all_functions(parser)
    
    -- 4. Codegen
    string llvm_ir = codegen_module(functions)
    
    -- 5. Write output
    write_file(output_file; llvm_ir)
    
    return 0
end_function
```

### 2.2. CLI Interface
```melp
function main() returns numeric
    -- Parse command line args
    -- Usage: melp_compiler input.mlp output.ll
    
    if argc() < 3 then
        println("Usage: melp_compiler <input.mlp> <output.ll>")
        return 1
    end_if
    
    string input = argv(1)
    string output = argv(2)
    
    numeric result = compile_source(input; output)
    
    if result == 0 then
        println("✅ Compilation successful!")
    else
        println("❌ Compilation failed!")
    end_if
    
    return result
end_function
```

### 2.3. Error Handling
```melp
-- Error reporting system
function report_error(line; message) returns numeric
    println("Error at line " + to_string(line) + ": " + message)
    return 1
end_function
```

---

## 📋 Phase 3: Bootstrap Test (2 gün)

### 3.1. Stage 0 Compiles Stage 1
```bash
# Test 1: Stage 0 compiler compiles Stage 1 compiler
cd /home/pardus/projeler/MLP/MLP

./compiler/stage0/modules/functions/functions_compiler \
    compiler/stage1/compiler_main.mlp \
    build/melp_stage1.ll

clang -O2 build/melp_stage1.ll runtime/stdlib/mlp_*.c \
    -L runtime/sto -lsto_runtime -lm \
    -o build/melp_stage1

./build/melp_stage1 --version
# Expected: "MELP Stage 1 Compiler v1.0"
```

### 3.2. Stage 1 Compiles Itself! (Self-Hosting)
```bash
# Test 2: Stage 1 compiler compiles itself
./build/melp_stage1 \
    compiler/stage1/compiler_main.mlp \
    build/melp_stage1_v2.ll

clang -O2 build/melp_stage1_v2.ll runtime/stdlib/mlp_*.c \
    -L runtime/sto -lsto_runtime -lm \
    -o build/melp_stage1_v2

./build/melp_stage1_v2 --version
# Expected: "MELP Stage 1 Compiler v1.0" (same!)
```

### 3.3. Binary Equivalence Test
```bash
# Test 3: Both compilers produce identical output
./build/melp_stage1 tests/llvm/01_basic/test_hello.mlp test1.ll
./build/melp_stage1_v2 tests/llvm/01_basic/test_hello.mlp test2.ll

diff test1.ll test2.ll
# Expected: No difference (deterministic compilation)

# If different, check:
# - Temp variable naming
# - Label numbering
# - Comment timestamps
```

---

## 📋 Phase 4: Feature Parity (2-3 gün)

### 4.1. Port Stage 0 Features to Stage 1
**Critical Features:**
```
✅ Basic: Functions, Variables, Arithmetic
✅ Control: if/else, while, for
✅ Types: numeric, string, boolean
🔄 Advanced (port from Stage 0):
   - List operations (YZ_200)
   - Map/Dictionary (YZ_201)
   - Optional Type (YZ_202)
   - Generics (YZ_203)
   - Imports/Exports (YZ_204)
   - Result Type (YZ_206)
   - Try/Catch (YZ_207)
   - Lambda (YZ_208)
   - Collections (YZ_212-213) ← Just finished!
```

### 4.2. Test Each Feature
```bash
# Test list operations
./build/melp_stage1 tests/llvm/08_list/test_list_basic.mlp test.ll
clang test.ll runtime/stdlib/mlp_*.c -o test && ./test

# Test collections
./build/melp_stage1 tests/llvm/13_collections/test_set.mlp test.ll
clang test.ll runtime/stdlib/mlp_*.c -o test && ./test

# ... repeat for all features
```

---

## ✅ Başarı Kriterleri

### Must Have (Critical):
1. ✅ Stage 1 compiler MELP ile yazılmış
2. ✅ Stage 0 ile compile edilebiliyor
3. ✅ Kendini compile edebiliyor (bootstrap)
4. ✅ Basic features çalışıyor (function, var, if, while)
5. ✅ Test suite passing (en az 10 test)

### Should Have (Important):
6. ✅ Aynı/benzer LLVM IR üretiyor
7. ✅ Error messages anlamlı
8. ✅ Performance acceptable (compile time < 5x Stage 0)
9. ✅ Advanced features: List, Map, Collections

### Nice to Have (Optional):
10. 🟡 Binary equivalence (deterministic)
11. 🟡 Full feature parity (tüm YZ_200+ features)
12. 🟡 Optimization passes

---

## 🗂️ Dosya Yapısı

```
compiler/stage1/
├── lexer.mlp                  (NEW - Phase 1.1)
├── parser.mlp                 (NEW - Phase 1.2)
├── codegen_llvm.mlp          (NEW - Phase 1.3)
├── ast.mlp                   (NEW - AST types)
├── types.mlp                 (NEW - Type system)
├── error.mlp                 (NEW - Error handling)
├── compiler_main.mlp         (UPDATE - Main driver)
└── utils.mlp                 (NEW - Utilities)

build/
├── melp_stage1               (Compiled by Stage 0)
├── melp_stage1_v2            (Compiled by Stage 1)
└── *.ll                      (Intermediate LLVM IR)

tests/stage1/
├── test_lexer.mlp            (Unit test: lexer)
├── test_parser.mlp           (Unit test: parser)
├── test_codegen.mlp          (Unit test: codegen)
└── test_bootstrap.sh         (Integration: self-compile)
```

---

## 📊 İlerleme Takibi

### Week 1 (Day 1-7):
- [x] YZ_213 commit & push (DONE!)
- [x] YZ_300 branch create (DONE!)
- [ ] Phase 1.1: Lexer port (Day 1-2)
- [ ] Phase 1.2: Parser port (Day 3-4)
- [ ] Phase 1.3: Codegen port (Day 5-6)
- [ ] Phase 2.1: Compiler driver (Day 7)

### Week 2 (Day 8-14):
- [ ] Phase 2.2: CLI interface (Day 8)
- [ ] Phase 2.3: Error handling (Day 9)
- [ ] Phase 3.1: Bootstrap test 1 (Day 10)
- [ ] Phase 3.2: Bootstrap test 2 (Day 11)
- [ ] Phase 3.3: Binary equivalence (Day 12)
- [ ] Phase 4.1: Feature parity (Day 13-14)
- [ ] Phase 4.2: Full testing (Day 14)

---

## ⚠️ Risks & Mitigation

### Risk 1: MELP'te eksik features
**Problem:** Stage 1 yazmak için gereken feature Stage 0'da yok
**Solution:** 
- File I/O primitives ekle (read_file, write_file)
- String manipulation (concat, substring)
- List operations (zaten var - YZ_200)

### Risk 2: Performance concern
**Problem:** MELP compiler çok yavaş olabilir
**Solution:**
- İlk başta performance önemli değil (doğruluk önemli)
- Sonra optimize edilir
- LLVM zaten optimize eder (O2 flag)

### Risk 3: Debugging zor
**Problem:** MELP'te yazılmış compiler debug etmek zor
**Solution:**
- Verbose logging ekle
- Step-by-step test (lexer → parser → codegen)
- LLVM IR inspect et

### Risk 4: "10. self-hosting macerası"
**Problem:** Yine yarım bırakılabilir
**Solution:**
- ✅ **Commitment:** 1-2 hafta focus, başka iş yok
- ✅ **Incremental:** Her gün bir milestone
- ✅ **Testing:** Her phase sonrası test
- ✅ **Motivation:** "No more C hamallık!" reminder

---

## 🎊 Başarı Sonrası (Post-YZ_300)

### Stage 0 → Archive Mode
```
compiler/stage0/
├── README.md: "⚠️ DEPRECATED - Use Stage 1"
├── (Keep for bootstrap only)
└── (No more feature additions!)
```

### Stage 1 → Production Mode
```
All new features in MELP:
✅ YZ_214: String Library → stage1/stdlib/string.mlp
✅ YZ_215: Math Library → stage1/stdlib/math.mlp
✅ YZ_216: File I/O → stage1/stdlib/io.mlp
✅ Collections → Already ported from YZ_212-213
```

### Development Workflow (Future)
```bash
# Edit MELP compiler
vim compiler/stage1/parser.mlp

# Compile with itself!
./build/melp_stage1 compiler/stage1/parser.mlp -o parser.ll

# Recompile full compiler
make stage1

# Self-hosting achieved! 🎉
```

---

## 📚 Referanslar

**Zorunlu Okuma:**
1. `STAGE1_STATUS_REPORT.md` - Previous attempt analysis
2. `compiler/stage0/modules/lexer/lexer.c` - Reference implementation
3. `compiler/stage0/modules/functions/functions_parser.c` - Parser logic
4. `compiler/stage0/modules/functions/functions_codegen_llvm.c` - Codegen

**Inspirasyon:**
- Rust: Self-hosted since 2011 (rustc written in Rust)
- Go: Self-hosted since 2015 (gc written in Go)
- Swift: Self-hosted since 2015 (swiftc written in Swift)

**Quote:**
> "A programming language isn't fully real until it can compile itself."
> — Old compiler hacker wisdom

---

## 🚀 Let's Go!

**Current Status:** ✅ Branch created, ready to start!  
**Next Action:** Phase 1.1 - Port lexer.c to lexer.mlp

**Motto:** 
```
╔═══════════════════════════════════════╗
║  NO MORE C HAMALLIQ!                  ║
║  MELP COMPILES MELP!                  ║
║  SELF-HOSTING OR BUST! 🚀             ║
╚═══════════════════════════════════════╝
```

---

**Başlangıç:** 22 Aralık 2025  
**Hedef Bitiş:** 5 Ocak 2026  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Önceki Görev:** YZ_213 (Collections Parser - Stage 0)  
**Bu Görev:** YZ_300 (Self-Hosting Completion - Stage 1)  
**Sonraki Görev:** YZ_301+ (MELP'te yazılacak! 🎉)
