# 📋 MELP Project TODO - Stage 1 Self-Hosting (Clean)

**Created:** 17 Aralık 2025  
**Current Status:** 🚧 Stage 1 Phase 3 - Module Integration  
**Last Session:** YZ_26 (API Wrappers Complete)  
**Next Goal:** Full Self-Hosting Compiler

---

## 🎯 CURRENT FOCUS: YZ_27 Self-Hosting

### ✅ Tamamlanan (YZ_01-26)

**Infrastructure:**
- ✅ Lexer (modules/lexer_mlp/) - 1,803 satır - YZ_57
- ✅ Parser (modules/parser_mlp/) - 3,000+ satır - YZ_01-08
- ✅ CodeGen (modules/codegen_mlp/) - 3,500+ satır - YZ_09-19
- ✅ Compiler Entry Point (compiler_full.mlp) - 320 satır - YZ_24
- ✅ File I/O Support (runtime wrappers) - YZ_25
- ✅ Clean API Wrappers (lexer_api, parser_api, codegen_api) - YZ_26

**Status:** Clean architecture documented, single-file compiler working!

---

## 🚀 YZ_27: Self-Hosting Final Push (3-5 gün)

### Hedef: MELP compiler compiles itself successfully

---

## 🚀 YZ_27: Self-Hosting Final Push (3-5 gün)

### Hedef: MELP compiler compiles itself successfully

### Strategy A: Real Module Integration (Recommended)

**Approach:** Replace stubs with real lexer/parser/codegen calls

**Tasks:**
1. Integrate real lexer (use tokenize_next loop)
2. Integrate real parser (use parse_program)
3. Integrate real codegen (use codegen_complete_program)
4. Test with simple .mlp file
5. Self-compile test

### Strategy B: Wait for Stage 1 Imports

**Approach:** Keep current single-file approach, add import system later

**Tasks:**
1. Implement import statement support
2. Add module loader
3. Link multiple .mlp files
4. Self-compile with imports

**Current Recommendation:** Strategy A - gradual integration

---

## 📊 Progress Summary

**Stage 1 Overall:** ~60% Complete

| Component | Status | Lines | Session |
|-----------|--------|-------|---------|
| Lexer | ✅ 100% | 1,803 | YZ_57 |
| Parser | ✅ 100% | 3,000+ | YZ_01-08 |
| CodeGen | ✅ 100% | 3,500+ | YZ_09-19 |
| Compiler Entry | ✅ 100% | 320 | YZ_24 |
| File I/O | ✅ 100% | 20 | YZ_25 |
| API Wrappers | ✅ 100% | 225 | YZ_26 |
| Integration | ⏳ 20% | - | YZ_27 Next |

---

## 🎯 YZ_27 Plan

### Part 1: Simple Test Program (1 gün)

**Goal:** Make compiler handle simplest possible .mlp file

**Test Input (`test_simple.mlp`):**
```mlp
function test() returns numeric
    return 42
end_function
```

**Expected Output (`output.ll`):**
```llvm
define i64 @test() {
entry:
  ret i64 42
}
```

**Tasks:**
- [ ] Write test_simple.mlp
- [ ] Run compiler on it
- [ ] Verify output.ll is correct
- [ ] Compile output.ll with clang
- [ ] Execute binary

### Part 2: Self-Compile (2-3 gün)

**Goal:** compiler_full.mlp compiles itself

**Test:**
```bash
cp modules/compiler_full.mlp input.mlp
./temp/compiler_full
# Should produce output.ll with compiler code
```

**Tasks:**
- [ ] Run self-compile
- [ ] Check output.ll validity
- [ ] Compare with Stage 0 output
- [ ] Generate v2 binary
- [ ] Test v2 binary

### Part 3: Bootstrap Chain (1 gün)

**Goal:** v2 compiles v3, v3 == v2 (convergence)

**Tasks:**
- [ ] v1 (Stage 0) → compiler_full → v2
- [ ] v2 → compiler_full → v3  
- [ ] Compare v2 and v3 binaries
- [ ] If v2 == v3: SUCCESS! 🎉

---

## 📝 Old YZ_26 Plan (Archived - Completed with API Wrappers)
```mlp
function parse_tokens() returns numeric
    println("[PARSER] Starting AST construction...")
    println("  ✓ AST construction complete (stub)")
    return 0
end_function
```

**Hedef (Real):**
```mlp
function parse_tokens(list tokens) returns list
    println("[PARSER] Starting AST construction...")
    
    -- Call real parser (from parser_mlp/parser.mlp)
    list ast = parser_parse(tokens)
    
    println("  ✓ AST construction complete")
    return ast
end_function
```

**Parser Entry Point:** `parser_mlp/parser_integration.mlp` veya `parser.mlp`

---

### Part 4: Stub Replacement - CodeGen (2-3 saat)

**Mevcut (Stub):**
```mlp
function codegen_ast() returns string
    println("[CODEGEN] Starting LLVM IR generation...")
    println("  ✓ LLVM IR generation complete (stub)")
    return "define i64 @main() { entry: ret i64 0 }"
end_function
```

**Hedef (Real):**
```mlp
function codegen_ast(list ast) returns string
    println("[CODEGEN] Starting LLVM IR generation...")
    
    -- Call real codegen (from codegen_mlp/codegen_integration.mlp)
    string llvm_ir = codegen_generate(ast)
    
    println("  ✓ LLVM IR generation complete")
    return llvm_ir
end_function
```

**CodeGen Entry Point:** `codegen_mlp/codegen_integration.mlp`

---

### Part 5: Full Multi-Module Compilation (2-3 saat)

**Görev:** Tüm modülleri birlikte derle

```bash
./functions_standalone --backend=llvm \
  modules/compiler_full.mlp \
  modules/lexer_mlp/lexer.mlp \
  modules/lexer_mlp/char_utils.mlp \
  modules/lexer_mlp/token.mlp \
  modules/parser_mlp/parser_integration.mlp \
  modules/parser_mlp/parser_core.mlp \
  modules/codegen_mlp/codegen_integration.mlp \
  modules/codegen_mlp/ir_builder.mlp \
  temp/compiler_v2_full
```

**Test:**
```bash
LD_LIBRARY_PATH=runtime/stdlib:runtime/sto ./temp/compiler_v2_full
→ Real lexer/parser/codegen çalışmalı!
```

**Output:** Çalışan compiler binary

---

### Part 6: Simple Program Test (1-2 saat)

**Test Input:**
```mlp
function test() returns numeric
    return 42
end_function
```

**Expected Output (`output.ll`):**
```llvm
define i64 @test() {
entry:
  ret i64 42
}
```

**Not Stub!** Gerçek LLVM IR olmalı.

**Başarı Kriterleri:**
- ✅ `output.ll` oluştu
- ✅ LLVM IR geçerli (llc-14 ile kontrol et)
- ✅ Function definition var (stub değil)
- ✅ Return value doğru (42)

---

## 🔮 YZ_27: Self-Hosting Convergence Test (1 hafta)

### Hedef: Compiler kendini derlesin, v2 → v3 → aynı output

### Part 1: Self-Compile (v1 → v2)

```bash
cp modules/compiler_full.mlp input.mlp
LD_LIBRARY_PATH=runtime/stdlib:runtime/sto ./temp/compiler_v2_full
```

**Beklenen:**
- ✅ Dosya okundu (320 satır)
- ✅ Lexer çalıştı (gerçek tokenization)
- ✅ Parser çalıştı (gerçek AST)
- ✅ CodeGen çalıştı (gerçek LLVM IR)
- ✅ `output.ll` yazıldı (büyük dosya, 5-10 KB)

---

### Part 2: Binary Compilation (v2 LLVM IR → Executable)

```bash
# IR → Assembly
llc-14 output.ll -o compiler_v2.s

# Assembly → Object
gcc -c compiler_v2.s -o compiler_v2.o

# Linking
gcc compiler_v2.o \
  -Lruntime/stdlib -lmlp_stdlib \
  -Lruntime/sto -lsto_runtime \
  -lm -o compiler_v2_binary
```

**Test:**
```bash
./compiler_v2_binary
→ Compiler v2 çalışmalı!
```

---

### Part 3: Generation 3 (v2 → v3)

```bash
cp modules/compiler_full.mlp input.mlp
LD_LIBRARY_PATH=runtime/stdlib:runtime/sto ./compiler_v2_binary
mv output.ll compiler_v3.ll
```

---

### Part 4: Convergence Test

```bash
diff output.ll compiler_v3.ll
```

**Beklenen:** Identical! (veya minimal farklar)

**Başarı:** Diff boş → **SELF-HOSTING COMPLETE!** 🎉

---

### Part 5: Test Suite

Basit programları derle:
```mlp
# test_arithmetic.mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end_function

# test_if.mlp
function max(numeric a, numeric b) returns numeric
    if a > b then
        return a
    else
        return b
    end_if
end_function
```

**Başarı:** Her test için valid LLVM IR

---

## 📋 Öncelikli Görevler (Sırayla)

### 🔥 Kritik (YZ_26)
- [ ] **Part 1:** Multi-module compilation test
- [ ] **Part 2:** Lexer stub replacement
- [ ] **Part 3:** Parser stub replacement
- [ ] **Part 4:** CodeGen stub replacement
- [ ] **Part 5:** Full compilation
- [ ] **Part 6:** Simple program test

### 🎯 Hedef (YZ_27)
- [ ] **Part 1:** Self-compile (v1 → v2)
- [ ] **Part 2:** Binary compilation (LLVM IR → executable)
- [ ] **Part 3:** Generation 3 (v2 → v3)
- [ ] **Part 4:** Convergence test (diff output)
- [ ] **Part 5:** Test suite validation

---

## ⚠️ Known Issues & Blockers

### Stage 0 Limitations
1. **No import system** - Manual multi-file compilation (use YZ_44 support)
2. **String display bug** - Addresses gösteriliyor (functional, just ugly)
3. **No argc/argv** - Command-line args hardcoded (use input.mlp/output.ll)
4. **Limited list support** - STO wrappers needed for complex structures

### Workarounds Applied
- ✅ File I/O: Runtime wrappers (YZ_25)
- ✅ Multi-module: functions_standalone support (YZ_44)
- ⏳ Module linking: In progress (YZ_26)

---

## 📊 Metrics

### Code Size (Stage 1 - MELP)
- Lexer: ~1,803 satır (YZ_57)
- Parser: ~3,000 satır (YZ_01-08)
- CodeGen: ~3,500 satır (YZ_09-19)
- Compiler Entry: ~320 satır (YZ_24)
- **Total:** ~8,600+ satır MELP kodu

### Self-Hosting Progress
```
Stage 1 Self-Hosting: ██████████░░░░░░░░░░ 50%

✅ Complete:
   - Infrastructure (100%)
   - File I/O (100%)
   - Individual modules (100%)

🚧 In Progress:
   - Module linking (YZ_26)
   - Integration (YZ_26)

❌ TODO:
   - Self-compile test (YZ_27)
   - Convergence test (YZ_27)
```

---

## 🎓 Best Practices

### YZ_26 Guidelines
- ✅ One module = One responsibility
- ✅ Test incrementally (2 modules → 3 modules → full)
- ✅ Document errors (function name collisions, missing deps)
- ✅ Keep stub implementations until verified working

### Testing Strategy
1. **Unit level:** Single module compilation
2. **Integration level:** 2-3 modules linked
3. **System level:** Full compiler with all modules
4. **Acceptance level:** Self-compile test

---

## 📚 Reference Documents

### Internal Docs
- `ARCHITECTURE.md` - System overview
- `MELP_REFERENCE.md` - Language guide
- `stage_1_YZ/YZ_*.md` - Session reports

### Created This Session
- `TODO.md` (cleaned, 366 lines)
- `stage_1_YZ/YZ_25.md` - Session report
- `temp/TODO_OLD_YZ24.md` - Backup (945 lines)

---

## 🎯 Success Criteria

### YZ_26 Success
- [ ] All modules compile together (no errors)
- [ ] Stubs replaced with real function calls
- [ ] Simple test program compiles (function test() returns numeric return 42 end_function)
- [ ] Output is valid LLVM IR (not stub)
- [ ] Binary executable runs

### YZ_27 Success (Self-Hosting)
- [ ] Compiler compiles itself
- [ ] Generated LLVM IR is valid
- [ ] v2 binary compiles v2 → v3
- [ ] v2 and v3 outputs are identical (convergence)
- [ ] Test suite passes with self-compiled compiler

---

**Next Session:** YZ_26 Part 1 - Multi-Module Compilation Test  
**Estimated Time:** 10-15 hours total (2 YZ sessions)  
**Target Date:** Self-hosting by end of December 2025

---

*Keep this TODO focused! Archive old details to temp/ when needed.*
