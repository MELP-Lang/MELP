# UA TODO #3 Handoff - 31 Aralık 2025

## 📊 Durum Özeti

**Tamamlanan:**
- ✅ TODO #3 (PARSER_CODEGEN_FULL) → Self-hosting MVP başarılı!
- ✅ Stage2 binary (16KB, convergence test pass)
- ✅ GitHub backup: mlp-gcc-selfhosting branch
- ✅ Experimental: stage2_LLVM_TO_GCC branch (current)
- ✅ Tag: v1.0-SELF-HOSTING

**Sıradaki Görev:** 3-TODO (Advanced Features)

---

## 🎯 3-TODO Stratejisi (1 Hafta)

### Kaynak: MLP-LLVM Modülleri
```
/home/pardus/projeler/MLP/MLP-LLVM/compiler/stage0/modules/
├── functions/      (function definitions)
├── struct/         (struct parsing)
├── array/          (array/list operations)
├── for_loop/       (complete loop implementation)
├── lambda/         (lambda expressions)
└── 34 modül toplam
```

### Backend Farkı
```
MLP-LLVM: MLP → LLVM IR (LLVMBuildX functions)
MLP-GCC:  MLP → C code (fprintf statements)

Adapte: LLVM codegen → C codegen çevirme
```

---

## 📋 TODO #3 Roadmap (1 Hafta)

### **Phase 1: Functions Module (Day 1-2)**
```bash
# Source
/MLP-LLVM/compiler/stage0/modules/functions/

# Files
├── functions_parser.c       (parser - backend agnostic) ✅
├── functions_codegen.c      (LLVM codegen) ⚠️ Adapte gerekli
├── functions_generic.c      (generic support)
└── functions.h

# Tasks
1. Copy parser to MLP-GCC
2. Adapt codegen: LLVM → C fprintf
3. Integration test
```

### **Phase 2: Struct Module (Day 2-3)**
```bash
# Similar pattern
/MLP-LLVM/compiler/stage0/modules/struct/
→ Copy parser
→ Adapt codegen (LLVM → C)
→ Test
```

### **Phase 3: Array Module (Day 3-4)**
```bash
/MLP-LLVM/compiler/stage0/modules/array/
→ Same workflow
```

### **Phase 4: Integration (Day 5-6)**
```bash
# Compile Stage1 source with new features
./melpc_stage2 MELP/MLP/stage1/modules/*.mlp

# Test features:
- Function definitions ✅
- Struct parsing ✅
- Array operations ✅
```

### **Phase 5: Stage3 Convergence (Day 7)**
```bash
./melpc_stage2 melpc.mlp -o melpc_stage3
diff melpc_stage2 melpc_stage3
# Exit code 0 = Full self-hosting! 🎉
```

---

## 🔧 Technical Details

### LLVM → C Codegen Pattern
```c
// LLVM Pattern (functions_codegen.c)
LLVMValueRef func = LLVMBuildFunction(ctx, name, params);
LLVMBuildRet(builder, ret_val);

// C Pattern (MLP-GCC adaptation)
fprintf(out, "void %s(", name);
for (params...) fprintf(out, "type param, ");
fprintf(out, ") {\n");
fprintf(out, "    return %s;\n", ret_val);
fprintf(out, "}\n");
```

### Modül Structure
```
modules/functions/
├── functions.h           (API definitions)
├── functions_parser.c    (Token → AST)
├── functions_codegen.c   (AST → C code)
├── functions_test.c      (Unit tests)
└── Makefile
```

---

## ⚠️ Riskler & Mitigations

**Risk 1: LLVM Dependencies**
```c
#include <llvm-c/Core.h>  // ❌ Remove!
LLVMBuildX(...)           // ❌ Replace with fprintf!
```
**Mitigation:** Grep search + manual replacement

**Risk 2: Token Sync Issues**
- Stage0 main.c = 646 lines (monolithic)
- Context loss risk (YZ_21 struggled)
- **Mitigation:** Break into smaller tasks, frequent tests

**Risk 3: Convergence Failure**
- Stage2 != Stage3 → More work needed
- **Mitigation:** Incremental testing, MVP scope

---

## 📊 Success Metrics

**Week 1 Complete:**
- ✅ Function definitions working
- ✅ Struct parsing working
- ✅ Array operations working
- ✅ Stage3 binary compiles
- ✅ Convergence test: Stage2 == Stage3

**Timeline:**
- Estimated: 7 days
- Optimistic: 5 days (if adaptation smooth)
- Pessimistic: 10 days (if many bugs)

---

## 🎯 UA Assignments

**UA_05 (veya yeni UA):**
1. YZ_22 assign → functions module adapte
2. YZ_23 assign → struct module adapte
3. YZ_24 assign → array module adapte
4. Integration + convergence test

**PD_02 (Danışman):**
- Context korunur (4 aylık hafıza)
- Kritik kararlarda devreye gir
- Architecture reviews
- 6 Core Principles compliance check

---

## 📂 Çalışma Branch

**Current:**
```bash
git branch
# * stage2_LLVM_TO_GCC

git status
# clean working tree
```

**Backup:**
```
mlp-gcc-selfhosting (GitHub)
v1.0-SELF-HOSTING (tag)
```

---

## 🚀 İlk Komutlar

```bash
# MLP-LLVM modül analizi
ls -la /home/pardus/projeler/MLP/MLP-LLVM/compiler/stage0/modules/functions/

# Parser inceleme
head -100 /home/pardus/projeler/MLP/MLP-LLVM/compiler/stage0/modules/functions/functions_parser.c

# Codegen LLVM dependency check
grep -n "LLVM" /home/pardus/projeler/MLP/MLP-LLVM/compiler/stage0/modules/functions/functions_codegen.c | head -20
```

---

## 📖 Context Documents

1. **0-TODO_SELFHOSTING.md** - Master roadmap
2. **YZ_21_Gorev_Plani.md** - TODO #3 MVP plan (completed)
3. **NEXT_AI_START_HERE.md** - Agent coordination
4. **6 Core Principles** - Architecture rules

---

**Handoff Date:** 31 Aralık 2025, 23:45  
**PD_02 Status:** Advisory mode, 4-month context preserved  
**Next Agent:** UA_05 (TODO #3 Advanced Features)

---

## 💬 PD_02 Final Notes

**4 aylık ders:**
- Pragmatic > Perfect ✅
- Momentum > Completeness ✅
- Modular = Faster ✅
- Context preservation kritik ✅

**3-TODO için:**
- MLP-LLVM modülleri hazır (34 modül!)
- Parser kodları backend-agnostic (copy-paste!)
- Codegen adapte 1 hafta realistik
- Stage0 son extend → sonra C'ye FREEZE 🧊

**Sen (UA) başarırsın!** 🚀

---

*PD_02 signing off. Context preserved. Ready for advisory calls.*
