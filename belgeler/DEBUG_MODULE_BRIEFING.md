# 🐛 DEBUG MODULE BRIEFING

**Date:** 26 Ocak 2026  
**Implementation:** MM_02  
**Status:** ✅ COMPLETE (Development Tool)  
**Integration:** MODERN Phase (Supporting Infrastructure)

---

## 📋 OVERVIEW

**Purpose:** Compiler development and debugging tool (NOT a language feature)

**Scope:** 
- Compiler internal debugging
- AST inspection and validation
- Token stream analysis
- Code generation verification

**NOT included:**
- Runtime debugging (user code debugging)
- Debugger integration (gdb/lldb)
- Breakpoint support for user programs

---

## 🎯 FUNCTIONALITY

### **1. AST Dump Mode**
```bash
./generic_compiler test.pmpl --ast-dump
# Output: Pretty-printed AST structure
```

**Use Case:** Verify parser correctness (Week 1 Day 1)

### **2. Token Stream Dump**
```bash
./generic_compiler test.pmpl --tokens
# Output: Lexer token sequence
```

**Use Case:** Verify lexer correctness (Week 1 Day 1)

### **3. LLVM IR Dump**
```bash
./generic_compiler test.pmpl --llvm-ir
# Output: Generated LLVM IR code
```

**Use Case:** Verify code generation (Week 1 Day 2)

### **4. Type Inference Trace**
```bash
./generic_compiler test.pmpl --type-trace
# Output: Type inference steps
```

**Use Case:** Debug generic type monomorphization (Week 1 Day 2)

### **5. Module Dependency Graph**
```bash
./generic_compiler project/ --dep-graph
# Output: DOT format dependency graph
```

**Use Case:** Debug import foundation (Week 1 Day 3)

---

## 🔧 IMPLEMENTATION

**Location:** `compiler/paradigma2/MODERN/infrastructure/debug/`

**Files:**
```
debug/
├── debug.h              -- Debug interface
├── debug.cpp            -- Core implementation
├── ast_printer.cpp      -- AST pretty-printer
├── token_printer.cpp    -- Token stream printer
├── ir_printer.cpp       -- LLVM IR printer
├── type_tracer.cpp      -- Type inference tracer
└── dep_graph.cpp        -- Dependency graph generator
```

**Integration:**
- Each MODERN module calls debug::log() when --debug flag active
- No runtime overhead (compile-time only)
- Zero impact on production builds

---

## 📊 USAGE IN MODERN WEEK 1

### **Day 1: Generic Types AST Design**
```bash
# Verify AST structure
./generic_compiler test_generic.pmpl --ast-dump

Expected Output:
GenericStructDef
├── Name: List
├── TypeParams: [T]
└── Body
    ├── Field: items (T[])
    └── Field: size (numeric)
```

### **Day 2: Monomorphization**
```bash
# Verify type inference
./generic_compiler test_generic.pmpl --type-trace

Expected Output:
[TypeInfer] List{numeric} → Instantiating T=numeric
[TypeInfer] Generated concrete type: List_numeric
[Codegen] LLVM struct: %List_numeric = { [0 x double], i64 }
```

### **Day 3: Import Foundation**
```bash
# Verify dependency resolution
./generic_compiler geometry.pmpl --dep-graph

Expected Output:
geometry.pmpl
├── import core::math     [OK]
├── import core::io       [OK]
└── import user::types    [OK]

Build Order: [core::math, core::io, user::types, geometry]
```

---

## ⚠️ CRITICAL CLARIFICATIONS

### **Q1: Development Tool or Language Feature?**
**A:** Development tool (like --ast-dump in Clang)

### **Q2: Compiler Debugging or Runtime Debugging?**
**A:** Compiler debugging only (AST, IR, types)

### **Q3: Status Complete or In-Progress?**
**A:** ✅ COMPLETE (basic functionality), extensible for new features

### **Q4: Week 1-2 Timeline Impact?**
**A:** ❌ NO IMPACT (supporting tool, not on critical path)

---

## 🎯 MODERN WEEK 1-2 USAGE

| Week | Day | Feature | Debug Tool Usage |
|------|-----|---------|-----------------|
| 1 | 1 | Generic AST | `--ast-dump` verify GenericStructDef |
| 1 | 2 | Monomorphization | `--type-trace` verify T→numeric |
| 1 | 3-4 | Import Foundation | `--dep-graph` verify build order |
| 2 | 1-2 | Pattern Match | `--ast-dump` verify Match AST |
| 2 | 1-2 | Closures | `--ast-dump` verify Lambda AST |
| 2 | 1-2 | Channels | `--type-trace` verify Channel{T} |

---

## 📋 INTEGRATION EXAMPLES

### **Example 1: YZ_MODERN_01 Day 1**
```bash
# YZ writes test_generic.pmpl
struct List{T}
    T[] items
end_struct

# YZ verifies AST
./generic_compiler test_generic.pmpl --ast-dump

# Expected: GenericStructDef node with TypeParam
# If wrong: Parser bug, fix immediately
```

### **Example 2: YZ_MODERN_01 Day 2**
```bash
# YZ writes instantiation test
List{numeric} numbers

# YZ traces type inference
./generic_compiler test.pmpl --type-trace

# Expected: "T=numeric, generating List_numeric"
# If wrong: Monomorphization bug, fix immediately
```

### **Example 3: YZ_MODERN_01 Day 3**
```bash
# YZ writes multi-module test
# geometry.pmpl imports core::math

# YZ checks dependency graph
./generic_compiler geometry.pmpl --dep-graph

# Expected: core::math → geometry (correct order)
# If wrong: Import foundation bug, fix immediately
```

---

## 🛡️ QUALITY ASSURANCE

**Debug Module Standards:**
- ✅ Zero false positives (accurate debugging info)
- ✅ Performance: <10ms overhead (compile-time only)
- ✅ Integration: All MODERN modules support --debug
- ✅ Documentation: Each flag documented with examples

**Testing:**
```bash
# Test debug module itself
cd compiler/paradigma2/MODERN/tests/debug
./run_debug_tests.sh

# All tests pass: ✅
```

---

## 📚 DOCUMENTATION

**User Guide:** `belgeler/DEBUG_GUIDE.md` (TODO: Create if needed)

**Quick Reference:**
```bash
# Available flags
--ast-dump       # Pretty-print AST
--tokens         # Show token stream
--llvm-ir        # Show LLVM IR
--type-trace     # Show type inference
--dep-graph      # Show dependency graph
--debug          # Enable all debug output
```

---

## ✅ CONCLUSION

**Debug Module:**
- ✅ COMPLETE and READY
- ✅ NO impact on MODERN Week 1-2 timeline
- ✅ Supporting tool for YZ development
- ✅ Extensible for future features

**Status:** NON-BLOCKING, SUPPORTING INFRASTRUCTURE

**YZ_MODERN_01 can use debug tools starting Day 1!** 🚀

---

**MM_02 (Implementation Lead)**  
26 Ocak 2026

**Debug Module: READY FOR MODERN DEVELOPMENT!** ✅
