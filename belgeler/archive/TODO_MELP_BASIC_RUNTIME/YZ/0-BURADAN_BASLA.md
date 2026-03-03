# 🚀 MELP BASIC RUNTIME IMPLEMENTATION'A HOŞGELDİN!

**Proje:** MELP Basic Runtime (Print + String Literals + Stdlib)  
**Hedef:** MELP compiler'ı temel düzeyde çalışabilir hale getirmek  
**Süre:** 2-2.5 gün (11-18 saat aktif iş + %30 buffer)  
**Durum:** 🟡 Planning → Implementation Ready  
**Öncelik:** 🔴 CRITICAL - Minimum Viable Compiler

---

## 🎯 PROJENIN AMACI

**Ne Yapıyoruz?**

```
Current State                    Target State
├─ Variables ✅           →     ├─ Variables ✅
├─ Control flow ✅        →     ├─ Control flow ✅
├─ Math operators ✅      →     ├─ Math operators ✅
├─ print() ❌            →     ├─ print() ✅
├─ String literals ❌    →     ├─ String literals ✅
└─ Stdlib ❌             →     └─ Stdlib integration ✅
```

**Test Programı (ŞU AN ÇALIŞMIYOR):**
```mlp
function main()
  print("merhaba dünya")
  numeric x = 42
  print(x)
  return 0
end_function
```

**Hedef (ÇALIŞMALI):**
```bash
$ ./melp_compiler test.mlp -o test.ll
$ llc test.ll -o test.o
$ clang test.o -lmelp_runtime -o test
$ ./test
merhaba dünya
42
```

---

## 📋 PHASE SIRASI (Implementation Order)

### 🔴 PHASE 1: String Literals (2-3 saat)
String literals compile edilip LLVM global constants'a dönüşmeli

📄 **[1-TODO_PHASE1_STRING_LITERALS.md](1-TODO_PHASE1_STRING_LITERALS.md)**
- Task 1: Parser support validation (STRING_LITERAL node)
- Task 2: AST node definition (if missing)
- Task 3: createStringLiteral() implementation
- Task 4: LLVM global constant generation

**Hedef:** `"hello"` → `@.str.0 = private unnamed_addr constant [6 x i8] c"hello\00"`

**Success Criteria:**
- [ ] String literals parse to AST
- [ ] LLVM IR has global string constants
- [ ] `llc` validates IR
- [ ] No warnings

---

### 🟠 PHASE 2: Printf Integration (1-2 saat)
External function declarations (printf/wrappers) eklenmeli

📄 **[2-TODO_PHASE2_PRINTF_INTEGRATION.md](2-TODO_PHASE2_PRINTF_INTEGRATION.md)**
- Task 1: declareExternalFunction() method
- Task 2: Declare print wrappers (__builtin_io_print_string, __builtin_io_print_numeric)
- Task 3: Alternative printf() declaration (if wrappers not available)

**Hedef:** External functions declared in module

**Success Criteria:**
- [ ] External functions in IR
- [ ] Correct function signatures
- [ ] No linker errors

---

### 🟡 PHASE 3: Print Wrappers (2-3 saat)
`print()` function calls compile edilip çağrılmalı

📄 **[3-TODO_PHASE3_PRINT_WRAPPERS.md](3-TODO_PHASE3_PRINT_WRAPPERS.md)**
- Task 1: CALL_EXPR AST node (if missing)
- Task 2: compileCallExpr() method implementation
- Task 3: Type-based print() dispatcher
- Task 4: Handle CALL_EXPR in compile() method

**Hedef:** `print("hello")` ve `print(42)` compile olmalı

**Success Criteria:**
- [ ] `print("hello")` compiles
- [ ] `print(42)` compiles
- [ ] Type dispatch working
- [ ] Correct call instructions in IR

---

### 🟢 PHASE 4: Stdlib Integration (2-3 saat)
Print wrappers gerçekten çalışmalı (linking, runtime)

📄 **[4-TODO_PHASE4_STDLIB_INTEGRATION.md](4-TODO_PHASE4_STDLIB_INTEGRATION.md)**
- Task 1: Stdlib linking strategy (Stage 0 vs new implementation)
- Task 2: Create runtime wrappers (builtin_io.cpp)
- Task 3: Update build system (CMake/build.sh)
- Task 4: Compilation pipeline update

**Hedef:** Compiled programs execute successfully

**Success Criteria:**
- [ ] Programs link without errors
- [ ] Runtime functions resolve
- [ ] Programs produce correct output
- [ ] Valgrind clean (no leaks)

---

### 🔵 PHASE 5: Testing & Validation (2-3 saat)
Full test suite validation

📄 **[5-TODO_TESTING_VALIDATION.md](5-TODO_TESTING_VALIDATION.md)**
- Task 1: Unit tests (4 test programs)
- Task 2: Integration tests (full pipeline)
- Task 3: Memory safety (valgrind)
- Task 4: LLVM validation (opt --verify)

**Hedef:** All tests pass, production ready

**Success Criteria:**
- [ ] 4/4 test programs pass
- [ ] No memory leaks
- [ ] LLVM IR validates
- [ ] No warnings/errors

---

## ✅ SUCCESS CRITERIA (Overall Project)

**Minimum Viable Product (MVP):**

Aşağıdaki 4 test programı çalışmalı:

**Test 1: Hello World**
```mlp
function main()
  print("merhaba dünya")
  return 0
end_function
```
Expected: `merhaba dünya`

**Test 2: Print Numeric**
```mlp
function main()
  numeric x = 42
  print(x)
  return 0
end_function
```
Expected: `42`

**Test 3: Mixed I/O**
```mlp
function main()
  print("Sayı: ")
  numeric x = 42
  print(x)
  print("\n")
  return 0
end_function
```
Expected: `Sayı: 42\n`

**Test 4: Math + Print**
```mlp
function main()
  numeric x = 10
  numeric y = 32
  numeric sum = x + y
  print("10 + 32 = ")
  print(sum)
  return 0
end_function
```
Expected: `10 + 32 = 42`

---

## 🎯 IMPLEMENTATION STRATEGY

### Core Principles

**1. AUTONOMOUS Pattern**
- Stage 1 modülleri bağımsız kalmalı
- OK Layer pattern bozulmamalı
- Zero runtime overhead

**2. No Workarounds**
- ❌ Stub functions YASAK
- ❌ Mock implementations YASAK
- ❌ "Geçici çözümler" YASAK
- ✅ %100 gerçek implementation

**3. Quality > Speed**
- Hızlı değil, doğru implementation
- Her phase %100 çalışmalı
- Test coverage zorunlu

### Technical Decisions

**String Literals:**
- Global constants pattern (@.str.0)
- Stage 0 proven approach
- Zero runtime overhead

**Printf Integration:**
- Type-safe wrappers (__builtin_io_print_*)
- No direct printf varargs
- Compile-time type checking

**Stdlib Strategy:**
- Reuse Stage 0 code (Option A)
- OR create minimal wrappers (Option B)
- Link against runtime library

---

## 📊 TIMELINE & RESOURCES

### Phase Breakdown

| Phase | Duration | Tasks | Deliverable |
|-------|----------|-------|-------------|
| Phase 1 | 2-3h | String literals | LLVM global constants |
| Phase 2 | 1-2h | Printf integration | External function decls |
| Phase 3 | 2-3h | Print wrappers | Function calls work |
| Phase 4 | 2-3h | Stdlib integration | Programs execute |
| Phase 5 | 2-3h | Testing | All tests pass |
| **TOTAL** | **9-14h** | | **MVP Complete** |

**Buffer:** +30% (11-18 saat → 2-2.5 gün)

### Resource Allocation

**YZ_BASIC_01: Phase 1-2 (String Literals + Printf)**
- Duration: 3-5 hours
- Dependencies: None
- Deliverable: String literals compile

**YZ_BASIC_02: Phase 3 (Print Wrappers)**
- Duration: 2-3 hours
- Dependencies: Phase 1-2 complete
- Deliverable: print() calls compile

**YZ_BASIC_03: Phase 4-5 (Stdlib + Testing)**
- Duration: 4-6 hours
- Dependencies: Phase 3 complete
- Deliverable: Programs execute, tests pass

---

## 🚧 RISK MITIGATION

### Critical Risks

**Risk 1: Parser Doesn't Support String Literals**
- Mitigation: Check parser first day
- Contingency: Port Stage 0 logic (2-3 hours)

**Risk 2: Linking Issues with Stage 0 Stdlib**
- Mitigation: Test linking early
- Contingency: Create standalone wrappers (3 hours)

**Risk 3: AST Node Types Missing**
- Mitigation: Review ast.h carefully
- Contingency: Add nodes if needed (1-2 hours)

---

## 📚 REFERENCE MATERIALS

### Stage 0 Code (Proven Patterns)
```
compiler/stage0/codegen/string_ops.cpp        # String literal handling
compiler/stage0/builtin/builtin_registry.cpp  # Print wrappers
compiler/stage0/backend/llvm_backend.cpp      # External function declarations
```

### Stage 1 Files to Modify
```
compiler/stage1/modules/function_compiler/function_compiler.cpp  # Main implementation
compiler/stage1/common/ast.h                                     # AST nodes
compiler/stage0/parser/parser.cpp                                # String literal parsing
```

### LLVM Documentation
- [LLVM String Constants](https://llvm.org/docs/LangRef.html#constant-expressions)
- [LLVM Function Calls](https://llvm.org/docs/LangRef.html#call-instruction)
- [LLVM External Linkage](https://llvm.org/docs/LangRef.html#linkage-types)

---

## 🎯 QUALITY GATES

### Phase Completion Criteria
- [ ] Code compiles (no warnings)
- [ ] LLVM IR validates
- [ ] Phase-specific test passes
- [ ] No TODO/stub code
- [ ] Documentation updated

### Final Approval Criteria
- [ ] All 4 MVP tests pass
- [ ] Valgrind clean (zero leaks)
- [ ] No compilation warnings
- [ ] Build system updated
- [ ] Documentation complete

---

## 🚀 NASIL BAŞLARIM?

### Adım 1: Phase 1'i Oku
📄 **[1-TODO_PHASE1_STRING_LITERALS.md](1-TODO_PHASE1_STRING_LITERALS.md)**
- Parser support kontrolü
- AST node tanımı
- createStringLiteral() implementation
- Test case'ler

### Adım 2: Implementation'a Başla
```bash
cd /home/pardus/projeler/MLP/LLVM/stage1/C++
# Phase 1 task'lerini takip et
# Her task tamamlandıkça test et
# Phase 1 bitmeden Phase 2'ye geçme!
```

### Adım 3: Her Phase Sonrası Test
```bash
# LLVM IR validation
./melp_compiler test.mlp -o test.ll
llvm-as test.ll -o test.bc
opt -verify test.bc

# Compilation test
llc test.ll -o test.o
clang test.o -o test
./test
```

### Adım 4: Raporlama (Opsiyonel)
Her phase sonrası:
- Ne yaptın?
- Ne çalıştı?
- Ne çalışmadı?
- Sonraki adım ne?

---

## 📋 PROGRESS TRACKING

### Day 1 (14 Şubat 2026)
- [x] TODO document created
- [x] YZ folder structure created
- [ ] Phase 1 started

### Day 2 (15 Şubat 2026)
- [ ] Phase 1-2 complete
- [ ] Phase 3 in progress

### Day 3 (16 Şubat 2026)
- [ ] Phase 4-5 complete
- [ ] Final validation
- [ ] Project COMPLETE 🎉

---

## 🎉 FINAL GOAL

**Bu TODO tamamlandığında:**
```bash
$ cat test.mlp
function main()
  print("MELP Basic Runtime: ")
  print("READY! ✅")
  return 0
end_function

$ ./melp_compiler test.mlp -o test.ll
$ llc test.ll -o test.o
$ clang test.o -lmelp_runtime -o test
$ ./test
MELP Basic Runtime: READY! ✅
```

**Stage 1 Artık Minimum Viable Compiler! 🚀**

---

**Hazırsan, Phase 1'e başla! 💪**  
📄 **[1-TODO_PHASE1_STRING_LITERALS.md](1-TODO_PHASE1_STRING_LITERALS.md)**

**Sorular varsa: MM, ÜA, veya PD'ye danış.**

---

**Created By:** Mastermind (MM)  
**Date:** 14 Şubat 2026  
**Status:** 🟢 READY FOR IMPLEMENTATION
