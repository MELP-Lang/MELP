# 🚀 MELP Self-Hosting Geçiş Planlaması

**Oluşturulma:** 14 Aralık 2025  
**Durum:** Stage 0 ~98% Complete  
**Hedef:** Stage 1 - MELP compiler'ı MELP dilinde yazmak  
**Timeline:** 2025 Q4 → 2026 Q1

---

## ⚠️ KRİTİK PRENSIP: "Yeterince İyi" Stratejisi

**SORU:** Stage 0'da struct, try-catch, generics neden yok?

**CEVAP:** Kasıtlı olarak yok! İki kere iş yapmamak için!

```
❌ YANLIŞ Yaklaşım:
   Stage 0 (C): struct, try-catch, generics yaz
   Stage 1 (MELP): struct, try-catch, generics TEKRAR yaz
   → İKİ KERE İŞ! ❌

✅ DOĞRU Yaklaşım:
   Stage 0 (C): Core features only (var, func, string, array)
   Stage 1 (MELP): Self-hosting (core features kullan)
   Stage 1.5 (MELP): Advanced features ekle (BİR KERE!) ✅
   → TEK SEFER İŞ! ✅
```

**Stage 0:** "Minimum Viable Compiler" - Stage 1'i yazabilecek kadar güçlü
**Stage 1:** Core features ile kendini derliyor
**Stage 1.5:** Advanced features MELP ile ekleniyor (duplikasyon yok!)

---

## 📊 Mevcut Durum (14 Aralık 2025)

### ✅ Stage 0: Bootstrap Compiler (C dilinde) - %98 Tamamlandı

```
┌─────────────────────────────────────────────────────────────┐
│ STAGE 0: Bootstrap Compiler (C)                             │
├─────────────────────────────────────────────────────────────┤
│ Core Language        ████████████████████ 100%              │
│ Module System        ███████████████████░  95%              │
│ String Support       ████████████████████ 100% (YZ_69) ✅   │
│ LLVM Backend         ████████████████████ 100%              │
│ Control Flow         ████████████████████ 100%              │
│ Arrays               ░░░░░░░░░░░░░░░░░░░░   0% (Phase 18)  │
│                                                              │
│ TOPLAM: ~98% TAMAMLANDI                                     │
└─────────────────────────────────────────────────────────────┘
```

**Tamamlanan Phase'ler:**
- ✅ Phase 13: Self-Hosting Lexer (MELP'te - ilk adım!)
- ✅ Phase 13.5: LLVM Backend
- ✅ Phase 14: Control Flow (if/else, while, for)
- ✅ Phase 15: Standard Library Integration
- ✅ Phase 17: String Support (100% - YZ_69)

**Kalan Phase'ler (Stage 0):**
- ⏳ Phase 16: Advanced LLVM Features (3-4 saat)
- ⏳ Phase 18: Array Support (6-8 saat)
- ⏳ Phase 19: Documentation (1-2 saat)

---

## 🎯 Self-Hosting Pipeline: 3 Aşamalı Plan

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│  STAGE 0    │ ───> │  STAGE 1    │ ───> │  STAGE 2    │
│             │      │             │      │             │
│ Bootstrap   │      │ Self-Host   │      │ Multi-Lang  │
│ (C dilinde) │      │ (MELP'te)   │      │ (Çok dilli) │
└─────────────┘      └─────────────┘      └─────────────┘
      ↓                     ↓                     ↓
   %98 DONE            2026 Q1              2026 Q2-Q3
```

---

## 📋 STAGE 0 → STAGE 1 Geçiş Planı

### Faz 1: Stage 0'ı Tamamla (1-2 hafta)

**Hedef:** Bootstrap compiler'ı "yeterince iyi" hale getir

**⚠️ ÖNEMLİ PRENSIP: "Yeterince İyi" Stratejisi**

Stage 0'ı %100 tamamlamaya GEREK YOK! Sadece Stage 1'i yazabilecek kadar güçlü olmalı.

**NEDEN Advanced Features Stage 0'da YOK?**
- Try-catch, Generics, Struct gibi özellikleri Stage 0'da yapsak...
- ...Stage 1'de aynı şeyi TEKRAR yazmamız gerekir (MELP ile)
- **İki kere iş yapmış oluruz!** ❌

**Strateji:**
1. Stage 0: Core features (var, func, string, array, module) ✅
2. Stage 1: MELP'le yazılmış compiler (core features ile)
3. **Stage 1'de MELP ile yeni özellikler ekle** (try-catch, generics, struct)
4. Stage 2: Multi-language support

**Sonuç:** Advanced features sadece **bir kere** yazılacak (Stage 1'de MELP ile)!

```
Week 1-2: Stage 0 Son Rötuşlar
├── Phase 16: Advanced LLVM Features (3-4 saat)
│   ├── Optimization flags (-O0, -O1, -O2, -O3)
│   ├── Debug info (DWARF)
│   └── Performance benchmarks
│
├── Phase 18: Array Support (6-8 saat)
│   ├── Array literals [1, 2, 3]
│   ├── Array indexing arr[i]
│   ├── Bounds checking
│   └── Array operations (length, push, pop)
│
├── Phase 19: Documentation (1-2 saat)
│   ├── README.md update
│   ├── ARCHITECTURE.md - LLVM docs
│   └── LLVM_IR_GUIDE.md genişlet
│
└── Final Testing & Stabilization (2-3 saat)
    ├── Regression tests
    ├── Bug fixes
    └── Performance profiling

TOPLAM SÜRE: ~15-20 saat (~2 hafta)
```

**Teslim Kriterleri:**
- ✅ Tüm testler geçiyor
- ✅ LLVM backend stable
- ✅ String + Array desteği tam
- ✅ Dokümantasyon güncel
- ✅ **Stage 0 feature-frozen** (yeni özellik yok, sadece bug fix)
- ✅ **Minimum Viable Compiler** - Stage 1 yazabilecek kadar güçlü

**Stage 0'da OLMAYACAK (kasıtlı!):**
- ❌ Struct/Record Types → Stage 1'de eklenecek (MELP ile)
- ❌ Try-Catch → Stage 1'de eklenecek (MELP ile)
- ❌ Type Inference (var) → Stage 1'de eklenecek (MELP ile)
- ❌ Lambda/Closure → Stage 1'de eklenecek (MELP ile)
- ❌ Enum Types → Stage 1'de eklenecek (MELP ile)
- ❌ Generics → Stage 1'de eklenecek (MELP ile)

**Mantık:** Bu özellikleri iki kere yazmamak için! (C'de + MELP'te)

---

### Faz 2: Stage 1 - Parser in MELP (3-4 hafta)

**Hedef:** Parser'ı MELP dilinde yaz, Stage 0 ile derle

```
Week 3-6: Parser Self-Hosting
├── Part 1: Token Processing (1 hafta)
│   ├── Token stream management
│   ├── Lookahead implementation
│   └── Error recovery
│
├── Part 2: Expression Parsing (1 hafta)
│   ├── Arithmetic expressions
│   ├── Boolean expressions
│   ├── String operations
│   └── Function calls
│
├── Part 3: Statement Parsing (1 hafta)
│   ├── Variable declarations
│   ├── If/else, while, for
│   ├── Function declarations
│   └── Return statements
│
└── Part 4: Advanced Parsing (1 hafta)
    ├── Array/list parsing
    ├── Module imports
    ├── Type annotations
    └── Integration with Lexer (Phase 13'ten)

TOPLAM SÜRE: ~20-25 saat (~4 hafta)
```

**Dosya Yapısı:**
```
modules/
└── parser_mlp/           # NEW: MELP'te yazılmış parser
    ├── parser_core.mlp
    ├── parser_expr.mlp
    ├── parser_stmt.mlp
    ├── parser_func.mlp
    └── parser_module.mlp
```

**Test Stratejisi:**
```mlp
# Stage 0 ile derle:
./stage0/melpc modules/parser_mlp/parser_core.mlp -o parser_mlp.o

# Test et:
./parser_mlp test_program.mlp  # AST çıktısı üret
```

---

### Faz 3: Stage 1 - Code Generator in MELP (3-4 hafta)

**Hedef:** LLVM IR code generator'ı MELP'te yaz

```
Week 7-10: CodeGen Self-Hosting
├── Part 1: LLVM IR Emission (1 hafta)
│   ├── Function declarations
│   ├── Basic blocks
│   ├── SSA form management
│   └── Type conversions
│
├── Part 2: Expression CodeGen (1 hafta)
│   ├── Arithmetic operations
│   ├── Function calls
│   ├── String operations
│   └── Array access
│
├── Part 3: Statement CodeGen (1 hafta)
│   ├── Variable declarations
│   ├── Control flow (if/while/for)
│   ├── Return statements
│   └── Memory management
│
└── Part 4: Optimization & Integration (1 hafta)
    ├── Constant folding
    ├── Dead code elimination
    ├── Register allocation hints
    └── Full pipeline integration

TOPLAM SÜRE: ~15-20 saat (~4 hafta)
```

**Dosya Yapısı:**
```
modules/
└── codegen_mlp/          # NEW: MELP'te yazılmış codegen
    ├── codegen_llvm.mlp
    ├── codegen_expr.mlp
    ├── codegen_stmt.mlp
    ├── codegen_func.mlp
    └── codegen_optimize.mlp
```

---

### Faz 4: Stage 1 - Bootstrap Complete (1-2 hafta)

**Hedef:** Stage 1 compiler'ı Stage 0 ile derle, sonra kendini derle!

```
Week 11-12: Bootstrap & Self-Compilation
├── Step 1: Compile Stage 1 with Stage 0
│   $ cd stage0
│   $ ./melpc ../stage1/*.mlp -o ../melp_stage1
│   └── Output: melp_stage1 (MELP compiler compiled by C compiler)
│
├── Step 2: Stage 1 compiles itself!
│   $ cd stage1
│   $ ./melp_stage1 *.mlp -o melp_stage1_v2
│   └── Output: melp_stage1_v2 (MELP compiler compiled by MELP!)
│
├── Step 3: Verification
│   $ diff melp_stage1 melp_stage1_v2
│   └── Should be identical (or functionally equivalent)
│
└── Step 4: Stage 0 Retirement
    ├── Archive Stage 0 (keep for reference)
    ├── melp_stage1 becomes official compiler
    └── 🎉 SELF-HOSTING ACHIEVED! 🎉

TOPLAM SÜRE: ~10-15 saat (~2 hafta)
```

---

## 📅 Detaylı Timeline

### 2025 Q4 (Aralık) - Stage 0 Finalization

| Hafta | Phase | Görev | Tahmini Süre | Durum |
|-------|-------|-------|--------------|-------|
| **Hafta 50** (9-15 Aralık) | Phase 17 | String Support | 8 saat | ✅ DONE (YZ_69) |
| **Hafta 51** (16-22 Aralık) | Phase 19 | Documentation | 2 saat | ⏳ TODO |
| **Hafta 51** | Phase 18 | Array Support (Part 1) | 4 saat | ⏳ TODO |
| **Hafta 52** (23-29 Aralık) | Phase 18 | Array Support (Part 2) | 4 saat | ⏳ TODO |
| **Hafta 52** | Phase 16 | LLVM Features | 3 saat | ⏳ TODO |
| **Hafta 52** | Testing | Final validation | 2 saat | ⏳ TODO |

**Milestone:** Stage 0 Feature Complete (31 Aralık 2025) ✅

---

### 2026 Q1 (Ocak-Mart) - Stage 1 Development

| Hafta | Faz | Görev | Tahmini Süre | Durum |
|-------|-----|-------|--------------|-------|
| **Hafta 1-2** (1-12 Ocak) | Parser | Token Processing + Expressions | 10 saat | ⏳ TODO |
| **Hafta 3-4** (13-26 Ocak) | Parser | Statements + Advanced | 10 saat | ⏳ TODO |
| **Hafta 5-6** (27 Oca - 9 Şub) | CodeGen | LLVM IR + Expressions | 8 saat | ⏳ TODO |
| **Hafta 7-8** (10-23 Şubat) | CodeGen | Statements + Optimization | 8 saat | ⏳ TODO |
| **Hafta 9-10** (24 Şub - 9 Mar) | Bootstrap | Integration + Testing | 10 saat | ⏳ TODO |
| **Hafta 11-12** (10-23 Mart) | Bootstrap | Self-compilation + Verification | 8 saat | ⏳ TODO |

**Milestone:** Stage 1 Self-Hosting Complete (31 Mart 2026) 🎯

---

## 🔄 Self-Hosting Verification Pipeline

```
┌────────────────────────────────────────────────────────────────┐
│ SELF-HOSTING VERIFICATION PROCESS                              │
├────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. Stage 0 (C) compiles Stage 1 (MELP source)                 │
│     Input:  stage1/*.mlp                                       │
│     Tool:   stage0/melpc (C compiler)                          │
│     Output: melp_stage1_gen0                                   │
│                                                                 │
│  2. Stage 1 (Gen 0) compiles Stage 1 (MELP source)             │
│     Input:  stage1/*.mlp                                       │
│     Tool:   melp_stage1_gen0                                   │
│     Output: melp_stage1_gen1                                   │
│                                                                 │
│  3. Stage 1 (Gen 1) compiles Stage 1 (MELP source)             │
│     Input:  stage1/*.mlp                                       │
│     Tool:   melp_stage1_gen1                                   │
│     Output: melp_stage1_gen2                                   │
│                                                                 │
│  4. Binary Comparison                                          │
│     $ diff melp_stage1_gen1 melp_stage1_gen2                   │
│     Expected: Identical or functionally equivalent             │
│                                                                 │
│  5. Test Suite Validation                                      │
│     All three compilers run same test suite                    │
│     Results must be identical                                  │
│                                                                 │
│  ✅ SUCCESS = Self-hosting achieved!                           │
│                                                                 │
└────────────────────────────────────────────────────────────────┘
```

---

## 🏗️ Architecture Evolution

### Stage 0 Architecture (Current)

```
melpc (C executable)
├── lexer.c           (C code)
├── parser.c          (C code)
├── codegen.c         (C code)
└── llvm_backend.c    (C code)

Input: program.mlp
Output: program.ll → (clang) → executable
```

### Stage 1 Architecture (Target)

```
melp (MELP executable, compiled by Stage 0)
├── lexer.mlp         (MELP code - from Phase 13!)
├── parser.mlp        (MELP code - NEW)
├── codegen.mlp       (MELP code - NEW)
└── llvm_backend.mlp  (MELP code - NEW)

Input: program.mlp
Output: program.ll → (clang) → executable
```

### Stage 2 Architecture (Future - 2026 Q2+)

**Stage 2:** Advanced features + Multi-language support

**Stage 1.5: Feature Expansion (Önce bu!)**
```
melp (MELP compiler with advanced features)
├── frontend/
│   ├── lexer.mlp         (existing)
│   ├── parser.mlp        (existing)
│   └── parser_advanced.mlp  (NEW)
│       ├── struct_parser.mlp     ← Stage 1'de eklenecek
│       ├── try_catch_parser.mlp  ← Stage 1'de eklenecek
│       ├── generics_parser.mlp   ← Stage 1'de eklenecek
│       ├── enum_parser.mlp       ← Stage 1'de eklenecek
│       └── lambda_parser.mlp     ← Stage 1'de eklenecek
├── midend/
│   └── type_checker.mlp  (NEW - Stage 1'de)
└── backend/
    └── llvm_backend.mlp  (existing, genişletilecek)

Input: program.mlp (MELP syntax with advanced features)
Output: program.ll → executable
```

**Stage 2: Multi-Language Support**
```
melp (Multi-language support)
├── frontend/
│   ├── syntax_loader.mlp     (Load diller.json)
│   ├── lexer_multilang.mlp   (Multi-syntax lexer)
│   └── parser_multilang.mlp  (Multi-syntax parser)
├── midend/
│   └── pmpl_ir.mlp           (Pragmatic MLP IR)
└── backend/
    ├── llvm_backend.mlp
    ├── wasm_backend.mlp      (NEW)
    └── arm_backend.mlp       (NEW)

Input: program.mlp (any language syntax - Türkçe, English, etc.)
Output: program.ll/wasm/arm → executable
```

**Yol Haritası:**
1. **Stage 0:** Core features only (şu an) ✅
2. **Stage 1:** Self-hosting (core features) → Mart 2026
3. **Stage 1.5:** Advanced features (struct, try-catch, generics) → Mayıs 2026
4. **Stage 2:** Multi-language support → Ağustos 2026

---

## 📊 Effort Estimation

### Toplam Süre Tahmini

```
┌─────────────────────────────────────────────────────┐
│ PHASE              │ SÜRE    │ KİŞİ    │ TAKVIM    │
├─────────────────────────────────────────────────────┤
│ Stage 0 Finish     │ 15h     │ 1 dev   │ 2 hafta   │
│ Parser in MELP     │ 25h     │ 1 dev   │ 4 hafta   │
│ CodeGen in MELP    │ 20h     │ 1 dev   │ 4 hafta   │
│ Bootstrap          │ 15h     │ 1 dev   │ 2 hafta   │
│ Testing & Polish   │ 10h     │ 1 dev   │ 1 hafta   │
├─────────────────────────────────────────────────────┤
│ TOPLAM             │ 85h     │ 1 dev   │ ~13 hafta │
└─────────────────────────────────────────────────────┘

Başlangıç: 14 Aralık 2025
Bitiş:     31 Mart 2026 (hedef)
```

### Haftada 8 saat çalışma ile:
- **Stage 0 Finish:** 2 hafta (16-29 Aralık)
- **Parser MELP:** 4 hafta (30 Aralık - 26 Ocak)
- **CodeGen MELP:** 4 hafta (27 Ocak - 23 Şubat)
- **Bootstrap:** 3 hafta (24 Şubat - 16 Mart)
- **Testing:** 2 hafta (17-31 Mart)

**Toplam:** ~15 hafta = ~3.5 ay

---

## 🎯 Success Criteria (Stage 1)

### Minimum Viable Self-Hosting (MVP)

```mlp
# Bu program Stage 1 ile derlenebilmeli:

# 1. Temel özellikler
function fibonacci(numeric n) returns numeric
    if n <= 1 then
        return n
    end if
    return fibonacci(n - 1) + fibonacci(n - 2)
end function

# 2. String operations
string message = "Hello, MELP!"
print(message)

# 3. Arrays
numeric[] numbers = [1, 2, 3, 4, 5]
for i in 0 to numbers.length() - 1
    print(numbers[i])
end for

# 4. Module imports
import "stdlib"
import "string_utils"

# 5. Control flow
while condition
    if check then
        continue
    else
        exit
    end if
end while
```

### Verification Checklist

- [ ] Stage 1 compiles itself successfully
- [ ] Binary output is stable (gen1 == gen2)
- [ ] All Stage 0 tests pass with Stage 1
- [ ] Performance: Within 2x of Stage 0 compile time
- [ ] Memory: No leaks, reasonable usage
- [ ] Error messages: Clear and helpful
- [ ] Documentation: Complete and accurate
- [ ] Examples: All examples compile and run

---

## 🚧 Known Challenges & Mitigations

### Challenge 1: Memory Management in MELP

**Problem:** MELP doesn't have manual memory management (no malloc/free)

**Solution:**
- Use STO runtime for automatic memory
- Implement simple garbage collection
- Rely on stack allocation where possible
- LLVM backend handles most optimization

### Challenge 2: Performance

**Problem:** MELP compiler in MELP might be slower than C version

**Mitigation:**
- Use LLVM optimizations (-O2, -O3)
- Profile and optimize hot paths
- Implement incremental compilation
- Cache intermediate results
- Target: Within 2x of Stage 0 speed

### Challenge 3: Debugging

**Problem:** Debugging MELP code harder than C

**Mitigation:**
- Implement good error messages early
- Add debug logging in MELP code
- Use LLVM debug info (DWARF)
- Keep Stage 0 for comparison
- Comprehensive test suite

### Challenge 4: Bootstrap Complexity

**Problem:** Circular dependency (compiler needs itself)

**Mitigation:**
- Three-stage bootstrap (proven method)
- Automated verification scripts
- Binary diff tools
- Regression test suite
- Git branches for each generation

---

## 📈 Metrics & Tracking

### Development Metrics

| Metric | Stage 0 | Stage 1 Target |
|--------|---------|----------------|
| Lines of Code | ~20,000 (C) | ~8,000 (MELP) |
| Compile Time (self) | N/A | < 5 seconds |
| Binary Size | ~500 KB | < 1 MB |
| Test Coverage | 85% | > 90% |
| Bootstrap Time | N/A | < 30 seconds |

### Weekly Progress Tracking

```bash
# Her hafta:
- Completed tasks
- Code commits
- Tests passing
- Blockers/issues
- Next week plan
```

---

## 🎓 Learning Resources

### For Team Members

**Compiler Construction:**
- "Crafting Interpreters" by Bob Nystrom
- "Engineering a Compiler" by Cooper & Torczon
- LLVM Tutorial: https://llvm.org/docs/tutorial/

**Self-Hosting:**
- "Bootstrapping a Simple Compiler" (Niklaus Wirth)
- Oberon compiler study
- TinyCC bootstrap process

**MELP-Specific:**
- `/docs/LLVM_IR_GUIDE.md` - MELP to LLVM patterns
- `/ARCHITECTURE.md` - System design
- `/YZ/YZ_*.md` - Development history

---

## 🔄 Continuous Integration

### CI/CD Pipeline for Stage 1

```yaml
# .github/workflows/stage1.yml

name: Stage 1 Bootstrap

on: [push, pull_request]

jobs:
  bootstrap:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      # Build Stage 0
      - name: Build Stage 0 (C compiler)
        run: |
          cd stage0
          make clean && make
      
      # Compile Stage 1 with Stage 0
      - name: Bootstrap Gen 0
        run: |
          ./stage0/melpc stage1/*.mlp -o melp_gen0
      
      # Self-compile (Gen 1)
      - name: Bootstrap Gen 1
        run: |
          ./melp_gen0 stage1/*.mlp -o melp_gen1
      
      # Self-compile (Gen 2)
      - name: Bootstrap Gen 2
        run: |
          ./melp_gen1 stage1/*.mlp -o melp_gen2
      
      # Verify stability
      - name: Verify Binary Stability
        run: |
          diff melp_gen1 melp_gen2
      
      # Run test suite with all compilers
      - name: Test Suite
        run: |
          ./run_tests.sh melp_gen0
          ./run_tests.sh melp_gen1
          ./run_tests.sh melp_gen2
```

---

## 🎯 Milestones Summary

```
┌──────────────────────────────────────────────────────┐
│ MILESTONE 1: Stage 0 Complete                        │
│ Date: 31 Aralık 2025                                │
│ Deliverable: Stable bootstrap compiler               │
│ Status: 98% (Array support kaldı)                   │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ MILESTONE 2: Parser in MELP                          │
│ Date: 31 Ocak 2026                                  │
│ Deliverable: MELP parser compiled by Stage 0         │
│ Status: Not started                                  │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ MILESTONE 3: CodeGen in MELP                         │
│ Date: 28 Şubat 2026                                 │
│ Deliverable: Full MELP compiler in MELP              │
│ Status: Not started                                  │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ MILESTONE 4: Self-Hosting Achievement! 🎉            │
│ Date: 31 Mart 2026                                  │
│ Deliverable: MELP compiling itself successfully      │
│ Status: Not started                                  │
│ Note: Core features only (no struct/try-catch)      │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ MILESTONE 5: Stage 1.5 - Advanced Features          │
│ Date: 31 Mayıs 2026                                 │
│ Deliverable: Struct, Try-Catch, Generics, Enums     │
│ Status: Not started (Stage 1'den sonra!)            │
│ Note: Bu özellikleri MELP ile yazacağız!            │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ MILESTONE 6: Stage 2 - Multi-Language Support       │
│ Date: 31 Ağustos 2026                               │
│ Deliverable: Türkçe/English syntax support          │
│ Status: Not started (Stage 1.5'ten sonra!)          │
└──────────────────────────────────────────────────────┘
```

---

## 📝 Next Actions (Immediate)

### Bu Hafta (15-21 Aralık 2025)

1. **YZ_70: Phase 19 - Documentation** (1-2 saat)
   - Update README.md with Phase 17 success
   - Update ARCHITECTURE.md with LLVM backend docs
   - Expand LLVM_IR_GUIDE.md

2. **YZ_71: Phase 18 Part 1 - Array Literals** (3-4 saat)
   - Array literal parsing `[1, 2, 3]`
   - LLVM IR array generation
   - Basic tests

3. **YZ_72: Phase 18 Part 2 - Array Operations** (3-4 saat)
   - Array indexing `arr[i]`
   - Bounds checking
   - Array methods (length, push, pop)

### Gelecek Hafta (22-28 Aralık 2025)

4. **YZ_73: Phase 16 - LLVM Optimizations** (3-4 saat)
   - Optimization flags
   - Debug info
   - Benchmarks

5. **YZ_74: Stage 0 Final Testing** (2-3 saat)
   - Regression tests
   - Bug fixes
   - Performance profiling

6. **Stage 0 Feature Freeze** (31 Aralık 2025)
   - No new features
   - Bug fixes only
   - Documentation complete

---

## 🎊 Vision: What Success Looks Like

### 31 Mart 2026 - Self-Hosting Day

```bash
# The moment of truth:
$ cd /home/pardus/projeler/MLP/MLP/stage1

# Stage 1 compiling itself:
$ ./melp *.mlp -o melp_v2

# Verification:
$ ./melp_v2 *.mlp -o melp_v3
$ diff melp_v2 melp_v3
Binary files melp_v2 and melp_v3 are identical

# The magic moment:
$ echo "🎉 SELF-HOSTING ACHIEVED! 🎉"
$ echo "MELP compiler is now written in MELP!"
$ echo "Stage 0 (C) retired. Stage 1 (MELP) is official."

# Ship it:
$ git tag v1.0.0-selfhosting
$ git push origin v1.0.0-selfhosting
```

### Community Announcement

```markdown
# 🎉 MELP 1.0: Self-Hosting Milestone Achieved!

Today, March 31, 2026, MELP compiler achieved self-hosting!

**What this means:**
- MELP compiler is now written in MELP itself
- No dependency on C compiler for development
- True dog-fooding: We use our own language daily
- Foundation for Stage 2 (multi-language support)

**Journey:**
- Started: December 2024 (Stage 0 in C)
- Lexer in MELP: December 2025 (Phase 13)
- Full self-hosting: March 2026 (3 months!)

**Stats:**
- Lines of MELP code: ~8,000
- Bootstrap time: ~30 seconds
- Test coverage: 92%
- Community contributors: X developers

**Next Steps:**
- Stage 2: Multi-language syntax support
- WebAssembly backend
- Package manager
- IDE plugins

Thank you to everyone who made this possible! 🙏

Download: https://github.com/MELP-Lang/MELP/releases/tag/v1.0.0
```

---

## 📚 Appendix

### A. File Organization After Self-Hosting

```
MELP/
├── stage0/               # ARCHIVED - C bootstrap compiler
│   ├── README.md        # "See stage1/ for active development"
│   └── src/             # Kept for reference only
│
├── stage1/              # ACTIVE - MELP self-hosting compiler
│   ├── lexer/
│   │   ├── lexer_token.mlp
│   │   ├── lexer_char.mlp
│   │   └── lexer_numeric.mlp
│   ├── parser/
│   │   ├── parser_core.mlp
│   │   ├── parser_expr.mlp
│   │   └── parser_stmt.mlp
│   ├── codegen/
│   │   ├── codegen_llvm.mlp
│   │   └── codegen_optimize.mlp
│   └── main.mlp
│
├── runtime/             # Runtime libraries (still in C)
├── stdlib/              # Standard library (MELP)
├── docs/                # Documentation
└── tests/               # Test suite
```

### B. References

- **MELP_VISION.md** - Project philosophy
- **ARCHITECTURE.md** - System design
- **TODO.md** - Current development roadmap
- **YZ/YZ_HISTORY.md** - Complete development history
- **temp/TODO_phase13.5_complete_20251213.md** - LLVM backend journey

### C. Contact & Collaboration

- GitHub: https://github.com/MELP-Lang/MELP
- Issues: Report bugs, request features
- Discussions: Architecture, design decisions
- Contributing: CONTRIBUTING.md (to be written)

---

**Oluşturan:** YZ_69  
**Tarih:** 14 Aralık 2025  
**Versiyon:** 1.0  
**Durum:** Living document - güncellenir

**Son Güncelleme:** 14 Aralık 2025

---

## 🎯 TL;DR - Executive Summary

**Şu an:** Stage 0 (C) %98 tamamlandı, String support ✅  
**Hedef:** Stage 1 (MELP) - Self-hosting compiler (**core features only!**)  
**Süre:** ~3.5 ay (Ocak-Mart 2026)  
**Çaba:** ~85 saat geliştirme  
**Sonuç:** MELP compiler MELP dilinde yazılmış olacak! 🎉

**⚠️ ÖNEMLİ PRENSIP: "Yeterince İyi" Stratejisi**

Stage 0'da advanced features (struct, try-catch, generics) **kasıtlı olarak YOK!**

**NEDEN?**
- Bu özellikleri Stage 0'da C ile yazsak...
- ...Stage 1'de aynı şeyi MELP ile tekrar yazmalıyız
- **İki kere iş yapmış oluruz!** ❌

**STRATEJİ:**
1. **Stage 0:** Minimum Viable Compiler (core features)
2. **Stage 1:** Self-hosting (core features ile kendini derliyor)
3. **Stage 1.5:** Advanced features ekle (**MELP ile, bir kere!**) ✅
4. **Stage 2:** Multi-language support

**Kritik Yol:**
1. Array support (1 hafta)
2. Parser in MELP (4 hafta)
3. CodeGen in MELP (4 hafta)
4. Bootstrap (2 hafta)
5. Advanced features (Stage 1.5 - 2 ay)

**Başarı:** 
- 31 Mart 2026 - MELP kendini derliyor! 🚀
- 31 Mayıs 2026 - Advanced features eklendi! (MELP ile) 🎉

---

## 📚 Terminoloji Sözlüğü

### Derleme Süreci Terimleri

**Compiler (Derleyici):**
Yüksek seviyeli bir programlama dilini (örn. MELP) makine koduna veya ara koda çeviren program. Lexer, Parser, Semantic Analyzer, Optimizer ve Code Generator bileşenlerinden oluşur.

**Lexer (Lexical Analyzer / Sözcüksel Çözümleyici):**
Kaynak kodunu tokenlara (sözcük birimlerine) ayıran compiler'ın ilk aşaması. Örneğin `x = 42 + y` kodunu `[ID("x"), ASSIGN, NUMBER(42), PLUS, ID("y")]` token dizisine dönüştürür.

**Parser (Sözdizimsel Çözümleyici):**
Token dizisini alıp dilin gramer kurallarına göre kontrol eden ve AST (Abstract Syntax Tree) oluşturan compiler aşaması. Örneğin `if x > 5 then y = 10` ifadesini IF-THEN düğümü içeren bir ağaç yapısına dönüştürür.

**AST (Abstract Syntax Tree / Soyut Sözdizim Ağacı):**
Programın yapısını hiyerarşik ağaç formatında temsil eden veri yapısı. Parantez, noktalama gibi sözdizimi detayları dışlanır, sadece anlamsal yapı tutulur. Örnek:
```
    =
   / \
  x   +
     / \
    42  y
```

**Semantic Analysis (Anlamsal Analiz):**
AST'yi kontrol ederek tip uyumluluğu, değişken tanımları, kapsam kuralları gibi anlamsal hataları bulan compiler aşaması. Örneğin `x: Int = "merhaba"` kodunda tip uyumsuzluğunu tespit eder.

**IR (Intermediate Representation / Ara Gösterim):**
Kaynak kodu ile makine kodu arasında bir ara formattır. Optimizasyon ve hedef platform bağımsızlığı sağlar. LLVM IR, Java bytecode, .NET CIL örnekleridir.

**CodeGen (Code Generator / Kod Üretici):**
AST veya IR'den hedef platform için assembly veya makine kodu üreten compiler'ın son aşaması. MELP Stage 0'da C dilinden x86-64 assembly kodu üretir.

**Optimizer (Optimize Edici):**
Programın işlevselliğini değiştirmeden daha hızlı ve/veya küçük kod üreten compiler bileşeni. Dead code elimination, constant folding, inlining gibi teknikler kullanır.

### Backend & Target Terimleri

**LLVM (Low Level Virtual Machine):**
Modüler compiler altyapısı ve araç zinciri. Platform bağımsız IR (LLVM IR) kullanır, 30+ hedef platformu destekler. Rust, Swift, Julia, Kotlin/Native gibi diller LLVM kullanır. MELP Stage 0'da LLVM backend kullanılıyor.

**GCC (GNU Compiler Collection):**
C, C++, Fortran gibi dilleri destekleyen açık kaynak compiler paketi. x86, ARM, RISC-V gibi birçok hedef platformu destekler. MELP'te LLVM alternatifi olarak kullanılabilir.

**Backend (Arka Uç):**
Compiler'ın platforma özgü kod üreten kısmı. MELP'te LLVM backend sayesinde aynı kod Windows/Linux/macOS için derlenebiliyor.

**x86-64 / AMD64:**
64-bit Intel ve AMD işlemciler için instruction set architecture (ISA). Modern PC ve sunucularda standart. MELP Stage 0 direkt x86-64 assembly üretiyor.

**Assembly (Makine Dili):**
İşlemcinin anlayabileceği komutların insan-okunabilir metinsel hali. Örnek: `movq $42, %rax` - 42 sayısını RAX registerına yükle.

**Register (Yazmaç):**
İşlemci içinde ultra hızlı veri depolama alanı. x86-64'te %rax, %rbx, %rcx gibi 64-bit registerlar var. 32-bit halleri %eax, %ebx şeklindedir.

**ABI (Application Binary Interface):**
Fonksiyon çağrılarında parametrelerin nasıl geçileceği, registerların nasıl kullanılacağı gibi binary-level kurallar. Örnek: x86-64 System V ABI'de ilk parametre %rdi'ye gelir.

**Cross-Platform (Platformlar Arası):**
Aynı kodun farklı işletim sistemleri ve donanımlar üzerinde çalışabilmesi. LLVM sayesinde MELP cross-platform.

### Runtime Terimleri

**Runtime (Çalışma Zamanı):**
Programın çalışması sırasında sağlanan servisler ve altyapı. Garbage collection, tip kontrolleri, standart kütüphane fonksiyonları runtime'a dahildir.

**Garbage Collection (Çöp Toplama):**
Kullanılmayan belleği otomatik olarak temizleyen runtime mekanizması. MELP'te şu anda manuel memory management var, ileride GC eklenecek.

**Memory Management (Bellek Yönetimi):**
Programın heap ve stack belleği nasıl kullandığının kontrolü. Stack: otomatik yerel değişkenler, Heap: dinamik allocasyonlar.

**Standard Library (Standart Kütüphane):**
Dille birlikte gelen temel fonksiyon ve modül seti. String işleme, dosya I/O, matematiksel fonksiyonlar gibi. MELP'te `runtime/stdlib/` altında.

**STO (String Type Object):**
MELP'in string veri yapısı. Uzunluk (length), kapasite (capacity) ve karakter dizisi (data) içerir. Dinamik boyutlandırma destekler.

**TTO (Text Type Object):**
MELP'in eski string sistemi (deprecated). STO'ya geçildi çünkü TTO'da performans ve bellek sorunları vardı.

### Self-Hosting Terimleri

**Self-Hosting (Kendi Kendini Derleme):**
Bir compiler'ın kendi dilinde yazılıp kendi kendini derlemesi. Örnek: C compiler C'de yazılır, MELP compiler MELP'te yazılacak.

**Bootstrap (Önyükleme):**
Bir dili başka bir dilde yazıp (Stage 0), sonra kendi dilinde yeniden yazma (Stage 1) ve kendi kendini derleme süreci. Tavuk-yumurta probleminin çözümü.

**Stage 0 (Aşama 0):**
Bootstrap için gerekli ilk compiler. Genellikle başka bir dilde yazılır (MELP Stage 0 C dilinde). "Good enough" - sadece temel özellikler içerir.

**Stage 1 (Aşama 1):**
Hedef dilde yazılmış compiler. Stage 0 tarafından derlenir. MELP Stage 1, MELP dilinde yazılacak ve Stage 0 tarafından derlenecek.

**Stage 2 (Aşama 2):**
Stage 1'in kendisini derlemesiyle oluşan compiler. Artık tamamen bağımsız ve optimize edilmiş. Çok dilli destek gibi advanced özellikler buraya eklenir.

**Dogfooding (Kendi Ürününü Kullanma):**
Yazılım geliştiricilerin kendi ürününü kullanması. Self-hosting compiler'lar için en iyi test yöntemi.

### Dil Özellikleri Terimleri

**Type System (Tip Sistemi):**
Değişkenlerin, fonksiyonların ve ifadelerin veri tiplerinin tanımlanması ve kontrol edilmesi sistemi. MELP statik tip sistemine sahip (compile-time kontrol).

**Static Typing (Statik Tipleme):**
Tiplerin compile-time'da kontrol edilmesi. Hataları erken yakalar, performans avantajı sağlar. Örnek: `x: Int = 42`

**Dynamic Typing (Dinamik Tipleme):**
Tiplerin runtime'da kontrol edilmesi. Daha esnek ama daha yavaş. Python, JavaScript örnektir. MELP statik tip sistemli.

**Struct (Yapı):**
İlişkili verileri gruplandıran kullanıcı tanımlı veri tipi. Örnek:
```melp
struct Person {
    name: String
    age: Int
}
```

**Generic (Genel Tip):**
Tip parametreleri alan, farklı tiplerle çalışabilen kod yapıları. Örnek: `Array<T>` hem `Array<Int>` hem `Array<String>` olabilir.

**Exception Handling (İstisna Yönetimi):**
Hata durumlarını yakalama ve işleme mekanizması. `try-catch-finally` blokları kullanılır.

**Array (Dizi):**
Aynı tipte birden fazla elemanı ardışık bellekte saklayan veri yapısı. Örnek: `nums: Array<Int> = [1, 2, 3]`

**Operator Overloading (Operatör Aşırı Yükleme):**
Kullanıcı tanımlı tipler için +, -, * gibi operatörlerin özel tanımlanması. Örnek: `Point` tipi için + operatörü iki noktayı toplar.

### Optimizasyon Terimleri

**Dead Code Elimination (Ölü Kod Eleme):**
Hiç çalışmayan veya kullanılmayan kodun temizlenmesi. Örnek: `if (false) { ... }` bloğu tamamen silinir.

**Constant Folding (Sabit Katlama):**
Compile-time'da hesaplanabilen ifadelerin önceden hesaplanması. Örnek: `x = 5 + 3` → `x = 8`

**Inlining (İçe Yerleştirme):**
Küçük fonksiyonların çağrıldığı yere direkt kopyalanması. Fonksiyon çağrı maliyetini ortadan kaldırır.

**Loop Unrolling (Döngü Açma):**
Döngü iterasyonlarının tekrarlı kod olarak yazılması. Branch prediction ve paralellik avantajı sağlar.

**Register Allocation (Register Tahsisi):**
Değişkenlerin hangi CPU registerlarında tutulacağına karar verme. İyi register allocation büyük performans kazancı sağlar.

### Test & Debug Terimleri

**Unit Test (Birim Test):**
Kodun küçük birimlerinin (fonksiyon, sınıf) izole şekilde test edilmesi. `tests/` klasöründeki testler.

**Integration Test (Entegrasyon Testi):**
Farklı bileşenlerin birlikte çalışmasının test edilmesi. Örnek: Lexer + Parser + CodeGen'in birlikte test edilmesi.

**E2E Test (End-to-End Test):**
Tüm sistemin baştan sona gerçek kullanım senaryolarıyla test edilmesi. `.mlp` dosyasından çalıştırılabilir program üretme testi.

**Debugging (Hata Ayıklama):**
Programdaki hataları bulma ve düzeltme süreci. GDB, LLDB gibi debugger'lar kullanılır.

**GDB (GNU Debugger):**
C/C++ ve assembly kodunu adım adım çalıştırıp değişkenleri inceleyebilen debugger. MELP runtime kodunu debug etmek için kullanılıyor.

**Assertion (İddia):**
Kodun belirli bir noktada bir koşulun doğru olması gerektiğini kontrol eden ifade. Yanlışsa program durur. Örnek: `assert(x > 0)`

### Mimari Terimler

**Module System (Modül Sistemi):**
Kodun mantıksal birimlere ayrılması ve yeniden kullanılabilirliği sağlayan sistem. `import`, `export` gibi mekanizmalar.

**Namespace (İsim Alanı):**
İsimlerin çakışmasını önlemek için mantıksal gruplama. Örnek: `Math.sqrt()` vs `String.sqrt()` farklı namespace'ler.

**API (Application Programming Interface):**
Bir yazılımın diğer yazılımlar tarafından kullanılması için sunduğu fonksiyon ve yapılar. MELP runtime API.

**CLI (Command Line Interface):**
Programın terminal/komut satırından kullanılması. MELP compiler CLI: `melp build file.mlp`

**REPL (Read-Eval-Print Loop):**
Etkileşimli programlama ortamı. Kod yaz → çalıştır → sonuç gör → tekrarla. Python, Node.js gibi.

**Toolchain (Araç Zinciri):**
Geliştirme sürecinde kullanılan araçlar bütünü. Compiler, linker, debugger, package manager vs.

### Performans Terimleri

**Benchmark (Kıyaslama):**
Performans ölçümü için standart testler. Örnek: Fibonacci(40) hesaplama süresi, string concat hızı.

**Latency (Gecikme):**
Bir işlemin başlangıç ile bitişi arasındaki süre. Örnek: Fonksiyon çağrısı latency'si 10ns.

**Throughput (İşlem Hacmi):**
Birim zamanda yapılan işlem sayısı. Örnek: Saniyede 1000 HTTP isteği işleme.

**JIT (Just-In-Time Compilation):**
Programın çalışma sırasında derlenmesi. V8 (JavaScript), JVM (Java) JIT kullanır. MELP'te şimdilik yok, ileride eklenebilir.

**AOT (Ahead-of-Time Compilation):**
Programın çalıştırılmadan önce tamamen derlenmesi. MELP AOT compiler. C, C++, Rust, Go gibi.

### Git & Geliştirme Terimleri

**Branch (Dal):**
Git'te bağımsız geliştirme hattı. Örnek: `phase17-string-support` branch'inde string özellikleri geliştiriliyor.

**Commit (İşleme):**
Kod değişikliklerinin Git'e kaydedilmesi. Her commit bir snapshot'tır. Örnek: `git commit -m "Fix string comparison bug"`

**Merge (Birleştirme):**
Bir branch'teki değişikliklerin başka bir branch'e dahil edilmesi. Örnek: `phase17` → `main` merge.

**PR (Pull Request):**
GitHub/GitLab'da kod değişikliklerinin incelenmesi ve ana branch'e dahil edilmesi talebi.

**CI/CD (Continuous Integration/Deployment):**
Kodun otomatik test edilmesi ve deploy edilmesi süreci. GitHub Actions, GitLab CI gibi.

**Refactoring (Yeniden Yapılandırma):**
Kodun işlevselliğini değiştirmeden iç yapısını iyileştirme. Okunabilirlik ve bakım kolaylığı için.

### Platform & Ekosistem

**Cross-Compilation (Çapraz Derleme):**
Bir platformda (örn. Linux) başka platform (örn. Windows) için kod üretme. LLVM cross-compilation destekler.

**Package Manager (Paket Yöneticisi):**
Kütüphanelerin indirilmesi, kurulması ve güncellenmesini yöneten araç. npm (JavaScript), cargo (Rust), pip (Python) gibi.

**Dependency (Bağımlılık):**
Bir projenin çalışması için gerekli harici kütüphane veya modül. MELP'in LLVM'e dependency'si var.

**Linker (Bağlayıcı):**
Object dosyalarını birleştirip executable üreten program. GCC, LLVM linker kullanır.

**Object File (.o):**
Derlenmiş ama henüz link edilmemiş makine kodu dosyası. Birden fazla .o dosyası linker ile birleştirilerek executable oluşur.

**Executable (Çalıştırılabilir Dosya):**
İşletim sistemi tarafından direkt çalıştırılabilen program. Linux'ta genellikle uzantısız, Windows'ta .exe.

### Özel MELP Terimleri

**MELP:**
**M**odern **E**asy **L**earning **P**rogramming language. Türkçe dokümantasyon, kolay sözdizimi, performans odaklı dil.

**Phase (Aşama):**
MELP geliştirme sürecindeki her ana özellik grubu. Phase 1: Variables, Phase 17: String Support gibi.

**YZ (Yapay Zeka):**
AI assistant oturumlarının numaralandırılması. YZ_01, YZ_69 gibi. Her YZ bir geliştirme oturumunu temsil eder.

**Good Enough Strategy (Yeterince İyi Stratejisi):**
Stage 0'a sadece temel özellikleri koymak, advanced özellikleri Stage 1.5'e bırakmak. Duplicate work'ten kaçınma stratejisi.

**Hybrid Architecture (Hibrit Mimari):**
MELP compiler logic'i MELP'te, performance-critical runtime C'de, LLVM her ikisini optimize ediyor. En iyi performans stratejisi.

---

**Sözlük Kullanım Önerileri:**

1. **Yeni Katkıcılar:** Sözlüğü baştan sona okuyun, temel kavramları öğrenin
2. **Geliştirme Sırasında:** Anlamadığınız terim olduğunda buraya dönün
3. **Dokümantasyon Yazarken:** Teknik terimleri tutarlı kullanın
4. **Code Review'da:** Herkesin aynı terminolojiyi kullandığından emin olun

**Not:** Bu sözlük MELP projesi ilerledikçe güncellenecektir. Yeni terimler eklendiğinde bu bölüm genişletilecektir.
