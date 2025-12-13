# 📋 MELP Project TODO - Phase 13.5 (LLVM Backend Migration)

**Created:** 13 Aralık 2025  
**Current Focus:** LLVM IR Backend Implementation  
**Status:** Phase 13 ✅ Complete | Phase 13.5 🚧 In Progress (75% - Parts 1-4 Done)

---

## ⚠️ BAŞLAMADAN ÖNCE - YENİ AI İÇİN KRİTİK UYARI

**📖 ZORUNLU OKUMA (ÖNCE BURAYI OKU!):**  
👉 **`NEXT_AI_START_HERE.md`** - Current session için mission brief (Buradan başla!)  
👉 **`YZ/YZ_HISTORY.md`** - Tüm YZ oturumlarının özeti

**🔴 GIT KURALLARI (ÇOK ÖNEMLİ!):**
```bash
# 1. Kendi dalını oluştur (veya mevcut feature branch'e devam et)
git checkout -b feature-name_YZ_XX
# Mevcut: phase13.5-llvm-backend (YZ_57-58)

# 2. Çalış, commit et
git add .
git commit -m "YZ_XX: Feature description"

# 3. Push et
git push origin feature-name_YZ_XX

# 4. ⚠️ ASLA MERGE YAPMA veya PULL REQUEST OLUŞTURMA!
# Human review yapıp merge edecek.
```

**📖 SONRA BUNLARI OKU:**  
Bu TODO'daki görevlere başlamadan önce **MUTLAKA** şu dosyaları oku:

👉 **`temp/kurallar_kitabı.md` - Bölüm 4: Smart Type Optimization (STO)**  
👉 **`ARCHITECTURE.md`** - Mimari kurallar (modülerlik, STO)  
👉 **`YZ/AI_METHODOLOGY.md`** - 5 adımlı hızlı geliştirme metodu  
👉 **`docs/LLVM_IR_GUIDE.md`** - MELP → LLVM IR mapping patterns (YZ_57'de oluşturuldu)

**⚠️ BİLİNEN SINIRLAMALAR:**
- **Global Variables:** MELP'te global variable YOK (sadece function-local)
- **Try-Catch:** Henüz implement edilmedi (future feature)
- **Struct Types:** Henüz implement edilmedi (future feature)
- **Relative Paths:** YZ_56'da düzeltildi (runtime'da absolute path kullanılıyor)

---

## 🎯 Current Milestone: Phase 13.5 - LLVM Backend Migration

### Strategy: Option C (Two-Stage Approach)
1. **Stage 0 Migration:** Port C compiler from Assembly → LLVM IR output
2. **Continue Self-Hosting:** Use LLVM backend for subsequent phases

### Why LLVM?
- **Portability:** Cross-platform support (x86-64, ARM, RISC-V)
- **Optimization:** Industrial-grade optimization passes
- **Maintainability:** No manual assembly code
- **Future-Proof:** Better foundation for self-hosting compiler

---

## 📦 Phase 13.5 Tasks

### Part 1: LLVM IR Basics & Study ✅ COMPLETE (YZ_57)
- [x] Create LLVM IR examples (arithmetic, functions, control flow)
- [x] Document MELP → LLVM IR mapping patterns
- [x] Test workflow: `.mlp` → `.ll` → `clang` → binary
- [x] Write LLVM IR cheat sheet for team

**Deliverables:**
- `docs/LLVM_IR_GUIDE.md` - Comprehensive mapping guide (753 lines) ✅
- `examples/llvm/` - Sample IR files (test_basic, test_function_call, test_println) ✅
- Test verification (clang compilation working) ✅

---

### Part 2: LLVM Backend Module ✅ COMPLETE (YZ_57)
- [x] Create `compiler/stage0/modules/llvm_backend/` directory
- [x] Implement `llvm_backend.h` - API definitions (177 lines)
- [x] Implement `llvm_backend.c` - IR emission functions (378 lines)
  - [x] `llvm_emit_function_start/end/entry()`
  - [x] `llvm_emit_arithmetic()` (add, sub, mul, div with constant folding)
  - [x] `llvm_emit_alloca/load/store()` (variable operations)
  - [x] `llvm_emit_return()`
  - [x] `llvm_emit_call()` (function calls)
  - [x] `llvm_emit_printf_support/println()` (printf integration)
- [x] Create `Makefile` for module
- [x] Write unit tests (`test_llvm_backend.c`)

**Deliverables:**
- LLVM backend module (clean API) ✅
- Test: Simple function compiles to valid LLVM IR (exit 30) ✅

---

### Part 3: Integration with functions_compiler ✅ COMPLETE (YZ_57)
- [x] Modify `functions_standalone.c`:
  - [x] Add `--backend=llvm` flag
  - [x] Switch between assembly/LLVM output
  - [x] Skip assembly step for LLVM IR
- [x] Create `functions_codegen_llvm.c/h` - Integration layer (245 lines)
  - [x] Statement generation (STMT_VARIABLE_DECL, STMT_RETURN)
  - [x] Expression generation (literals, variables, binary ops, function calls)
  - [x] Parameter vs local variable distinction
- [x] Update `Makefile`:
  - [x] Link LLVM backend module
  - [x] Add LLVM sources to build
- [x] Test existing `.mlp` files with LLVM backend

**Deliverables:**
- `functions_compiler --backend=llvm test.mlp test.ll` ✅
- Tests passing: test_basic.mlp (exit 30), test_llvm_functions.mlp (exit 42) ✅

---

### Part 4: Basic Testing ✅ COMPLETE (YZ_57)
- [x] Unit tests (llvm_backend module)
- [x] Integration tests (test_basic, test_llvm_functions)
- [x] Verify output correctness
- [x] Fix compilation errors (struct fields, includes)
- [x] Fix runtime errors (parameter vs local variable handling)

**Deliverables:**
- All basic tests passing ✅
- Git branch created: phase13.5-llvm-backend ✅
- Committed and pushed to GitHub ✅

---

### Part 5: Advanced Features 🚧 NEXT (YZ_58)
- [ ] Control Flow IR generation
  - [ ] `llvm_emit_br()` - Unconditional branch
  - [ ] `llvm_emit_br_cond()` - Conditional branch
  - [ ] `llvm_emit_label()` - Label emission
  - [ ] Update `generate_statement_llvm()` for STMT_IF, STMT_WHILE, STMT_FOR
- [ ] Standard Library Integration
  - [ ] Replace printf with `mlp_println_numeric()`
  - [ ] Link with `libmlp_stdlib.a` and `libsto_runtime.a`
  - [ ] Handle STO type tags correctly
- [ ] Comprehensive Testing
  - [ ] Run all Stage 0 tests with `--backend=llvm`
  - [ ] Compare outputs: Assembly vs LLVM
  - [ ] Performance benchmarking (compile time, binary size, runtime)
- [ ] Documentation & Finalization
  - [ ] Update `ARCHITECTURE.md` with LLVM backend section
  - [ ] Create `YZ/YZ_58.md` session report
  - [ ] Update this TODO marking Phase 13.5 complete
  - [ ] Create backup branch: `melp_stage0_phase13.5_complete_20251213`
  - [ ] Merge to main after human review

**Deliverables:**
- Full test suite passing
- Complete LLVM backend
- Ready for Phase 14 (self-hosting with LLVM)

---

### Part 5: Documentation & Cleanup (1-2 hours)
- [ ] Update `ARCHITECTURE.md` - LLVM backend section
- [ ] Update `README.md` - Build instructions
- [ ] Create `LLVM_BACKEND.md` - Technical deep dive
- [ ] Update `NEXT_AI_START_HERE.md`
- [ ] Git workflow:
  - Branch: `phase13.5-llvm-backend`
  - Commit messages clear
  - Merge to main

**Deliverables:**
- Complete documentation
- Clean git history
- YZ_58.md session report

---

## 🔄 Future Phases (Post-LLVM Migration)

### Phase 14: Self-Hosting Parser (5 parts)
- Parser written in MELP (generates LLVM IR)
- Estimated: 20-25 hours

### Phase 15: Self-Hosting Code Generator
- Code generator written in MELP (emits LLVM IR)
- Estimated: 15-20 hours

### Phase 16: Bootstrap Complete
- MELP compiler fully self-hosting
- Stage 1 replaces Stage 0

---

## ✅ Recently Completed

### Phase 13: Self-Hosting Lexer (Complete - 13 Aralık 2025)
- ✅ Part 6.3: Literal tokenization
- ✅ Part 6.4: Identifier tokenization (24 keywords)
- ✅ Part 6.5: Operator tokenization (26 token types)
- ✅ Part 6.6: Lexer integration
- **Total:** 4 modules, 38 functions, 56 token types
- **Git:** Merged to main (commit 071d39b)
- **Backup:** Branch `melp_stage1_phase13_complete_20251213`

---

## 📊 Project Statistics

### Compiler Implementation
- **Stage 0 (C):** ~8,000 lines (modular architecture)
- **Stage 1 (MELP):** ~1,500 lines (lexer modules)
- **Tests:** 60+ test files
- **Modules:** 15+ independent modules

### Build System
- **Incremental Compilation:** 15x faster (modules cached)
- **Smart Linking:** Only recompile changed dependencies
- **Module Cache:** `.mlp.cache/` directory

---

## 🎯 Success Metrics

### Phase 13.5 Definition of Done
- [ ] All existing tests pass with LLVM backend
- [ ] Documentation complete
- [ ] Zero performance regression
- [ ] Clean git history
- [ ] YZ session report finalized

---

## 📝 Notes & Decisions

### LLVM Version
- **Installed:** Clang 14.0.6, LLVM 14
- **Platform:** Debian 12, x86-64
- **Test:** Simple hello world ✅ working

### Design Decisions
- Keep assembly backend for reference
- Use `--backend` flag for output selection
- LLVM IR unoptimized initially (focus: correctness)
- Optimization passes later (Phase 13.6)

---

## 🔗 Related Documents

- **Previous TODO:** `temp/TODO_old_phase13_20251213.md`
- **Architecture:** `ARCHITECTURE.md`
- **Phase 13 Report:** `YZ/YZ_57.md`
- **Next Session:** `NEXT_AI_START_HERE.md`
- **LLVM Guide:** `docs/LLVM_IR_GUIDE.md` (to be created)

---

**Last Updated:** 13 Aralık 2025  
**Responsible:** YZ_57 → YZ_58 (transition)  
**Target Completion:** Phase 13.5 - 18 Aralık 2025 (estimated)
