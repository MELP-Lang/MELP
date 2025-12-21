# YZ_300: Self-Hosting Completion 🚀
**Başlangıç:** 22 Aralık 2025  
**Öncelik:** 🔴 **KRİTİK** (Tüm gelecek development buna bağlı)  
**Tahmini Süre:** 1-2 hafta  
**Durum:** 🚀 **BAŞLATILDI!** - No more C hamallık!

---

## 🎯 Görev Özeti

**HEDEF:** MELP compiler'ı MELP ile yaz, C'yi artık sadece bootstrap için kullan.

**NEDEN YAPIYORUZ:**
- ❌ **Şu An:** Her feature C'de yaz → Stage 1'e port et (2x effort)
- ✅ **Sonra:** Her feature MELP'te yaz → 1x effort, self-compile!
- 🗑️ **Stage 0 (C):** Geçici scaffold, deprecated olacak
- 🎯 **Stage 1 (MELP):** Asıl ürün, self-hosting

**MEVCUT DURUM:**
- Stage 1: %50 complete (YZ_01-20'de başlanmış, yarım kalmış)
- Stage 0: %95 complete (production-ready ama geçici!)
- Şimdi: Stage 0'a feature ekliyoruz (hamallık!)

---

## 📋 Phase 1: Critical Modules Port (3-4 gün)

### 1.1. Lexer (1 gün)
**Kaynak:** `compiler/stage0/modules/lexer/lexer.c`  
**Hedef:** `compiler/stage1/lexer.mlp`

**İşler:**
```
Stage 0 lexer.c → MELP translation:

1. Token types enum → MELP enum
2. Lexer struct → MELP struct
3. Tokenize functions:
   - read_number()
   - read_string()
   - read_identifier()
   - keyword matching
4. Comment handling
5. Whitespace handling
```

**Test:**
```melp
-- Test lexer independently
function test_lexer() returns numeric
    string source = "function main() returns numeric end_function"
    Lexer lex = lexer_create(source)
    
    Token tok1 = lexer_next_token(lex)  -- TOKEN_FUNCTION
    Token tok2 = lexer_next_token(lex)  -- TOKEN_IDENTIFIER ("main")
    
    return 0
end_function
```

---

### 1.2. Parser (1 gün)
**Kaynak:** `compiler/stage0/modules/functions/functions_parser.c`  
**Hedef:** `compiler/stage1/parser.mlp`

**İşler:**
```
Stage 0 parser → MELP translation:

1. AST node types → MELP structs
2. Parser functions:
   - parse_function_declaration()
   - parse_expression()
   - parse_statement()
   - parse_type()
3. Error handling
4. AST tree construction
```

**Test:**
```melp
-- Test parser independently
function test_parser() returns numeric
    string source = "function add(x; y) returns numeric return x + y end_function"
    Lexer lex = lexer_create(source)
    Parser parser = parser_create(lex)
    
    ASTNode func = parse_function_declaration(parser)
    -- Verify AST structure
    
    return 0
end_function
```

---

### 1.3. LLVM Codegen (1-2 gün)
**Kaynak:** `compiler/stage0/modules/functions/functions_codegen_llvm.c`  
**Hedef:** `compiler/stage1/codegen_llvm.mlp`

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
