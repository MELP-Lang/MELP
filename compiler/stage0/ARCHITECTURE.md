# MLP COMPILER ARCHITECTURE

**Last Updated:** 8 Aralık 2025  
**Current Status:** ✅ Modular Architecture Operational (Chained Imports Working!)  
**Architecture:** Radical Modular (Central files permanently deleted)

---

## 🎯 ARCHITECTURAL PHILOSOPHY: CHAINED IMPORTS

### **Core Principle: No Central Orchestrator, Only Module Chains**

```
❌ OLD (Deleted 7 Aralık 2025):
   main.c (801 lines) → orchestrator.c (251 lines) → modules (unused)
   Problem: Monolithic, unmaintainable, modules never used

✅ NEW (8 Aralık 2025):
   Entry point (102 lines) → Module A → Module B → Module C
   Each module imports what it needs (#include)
   Natural dependency chain via linker
```

**Example Working Chain:**
```c
functions_standalone.c (102 lines)
  → #include "functions_parser.h"
  → functions_parser.c (278 lines)
    → #include "../statement/statement_parser.h"
    → statement_parser.c (145 lines)
      → #include "../control_flow/control_flow_parser.h"
      → control_flow_parser.c (187 lines)
        → #include "../comparison/comparison_parser.h"
        → ✅ WORKS!
```

---

## 🚨 CRITICAL RULES: File Creation Lock

### ABSOLUTE PROHIBITION

**AI Agents CANNOT create:**
- ❌ `main.c` (deleted, backed up in temp/yedek_merkezi_dosyalar/)
- ❌ `orchestrator.c` (deleted permanently)
- ❌ `helpers.c` (deleted permanently)
- ❌ `router.c` or `pipeline.c` (not needed!)
- ❌ Any central coordination file

**WHY?** These files were deleted on purpose to prevent monolithic code!

### File Creation Matrix

| Location | New Files | Modification | Tests |
|----------|-----------|--------------|-------|
| `main.c` | ❌ DELETED | ❌ FORBIDDEN | N/A |
| `orchestrator.c` | ❌ DELETED | ❌ FORBIDDEN | N/A |
| `helpers.c` | ❌ DELETED | ❌ FORBIDDEN | N/A |
| `compiler/stage0/*.c` | ❌ FORBIDDEN | ⚠️ Avoid | ✅ test_*.c only |
| `modules/[name]/` | ✅ ALLOWED | ✅ ALLOWED | ✅ ALLOWED |
| `modules/[name]/*_standalone.c` | ✅ ALLOWED | ✅ ALLOWED | Entry points OK |

### ✅ CORRECT Behavior: Chained Import Pattern

```bash
User: "Add while loop body support"

AI thinks:
- "While loops are in control_flow module"
- "But need statement parsing for body"
- "Use chained imports: control_flow → statement"
- "No central orchestrator needed!"

AI does:
cd modules/control_flow/
# Add body field to WhileStatement struct
vim control_flow.h

# Use statement_parse() via import
vim control_flow_parser.c
  → #include "../statement/statement_parser.h"
  → while_data->body = statement_parse(parser);

# Generate body code via import  
vim control_flow_codegen.c
  → #include "../statement/statement_codegen.h"
  → statement_generate_code(output, stmt->body);

make test                # ✅ CORRECT - Module compiles standalone
```

### ❌ FORBIDDEN Behavior: Central Coordination

```bash
User: "Add while loop body support"

AI does:
cd compiler/stage0/
vim orchestrator.c       # ❌ DELETED FILE!
vim router.c             # ❌ NOT NEEDED!
vim pipeline.c           # ❌ WRONG APPROACH!

# VIOLATION! These patterns are FORBIDDEN!
```

**Why forbidden?**
- Leads to monolithic code
- Breaks modular architecture
- Makes modules dependent on central file
- Central files grow to 1000+ lines

**Correct approach:**
- Module imports what it needs
- Natural dependency chain
- Each module stays small (<300 lines)
- Linker resolves dependencies

### 🔒 Enforcement

### 🔒 Enforcement

**Git Pre-commit Hook (scripts/validate_architecture.sh):**
```bash
# Checks for forbidden patterns:
# - New files in compiler/stage0/ (except tests)
# - Files >500 lines (encourages modularity)
# - Central orchestrator patterns
# Rejects commit if rules violated
```

**Validation passes for:**
- ✅ New files in modules/
- ✅ *_standalone.c <200 lines (thin wrappers)
- ✅ Module files <300 lines (focused modules)
- ✅ Chained import patterns (#include "../other/")

### 📋 Current Architecture Status (8 Aralık 2025)

**Deleted Files (Backed up in temp/yedek_merkezi_dosyalar/):**
- ❌ main.c (801 bytes)
- ❌ orchestrator.c (6102 bytes)  
- ❌ helpers.c (2494 bytes)
- ❌ lexer.c (10876 bytes - moved to modules/lexer/)

**Working Entry Points:**
- ✅ modules/functions/functions_standalone.c (102 lines)
- ✅ modules/statement/statement_standalone.c (working)
- ✅ modules/control_flow/control_flow_standalone.c (working)

**Active Modules: 27**
- ✅ Fully working: 12 modules
- 🚧 Partial: 13 modules (chained imports being added)
- ⏳ Stub: 2 modules

### 🎯 Why This Rule?

**Problem without rule (Old architecture):**
```
compiler/stage0/
├── main.c (801 lines)        ← AI kept adding code ❌
├── orchestrator.c (251 lines) ← Growing monolithic ❌
├── helpers.c (72 lines)       ← Utility dumping ground ❌
└── [MONOLITHIC CHAOS] ❌

Result: Modules existed but were NEVER USED!
```

**With this rule (New architecture):**
```
compiler/stage0/
├── [NO CENTRAL FILES] ✅
└── modules/
    ├── functions/
    │   ├── functions_standalone.c (102 lines) ← Entry point
    │   ├── functions_parser.c (278 lines)
    │   └── functions_codegen.c (149 lines)
    ├── statement/
    │   ├── statement_parser.c (145 lines)
    │   └── statement_codegen.c (60 lines)
    └── [ORGANIZED, WORKING] ✅

Result: Chained imports, each module <300 lines, ACTUALLY WORKING!
```

### 🤖 AI Agent Instructions

**Before creating ANY file, ask:**

1. ❓ "Does this create a central orchestrator?"
   - YES → ❌ STOP! Use chained imports instead
   - NO → Continue to question 2

2. ❓ "Is this file going in `modules/[module_name]/`?"
   - YES → ✅ Proceed
   - NO → ❌ STOP!

3. ❓ "Can I use chained imports instead?"
   - YES → ✅ Use #include "../other_module/"
   - NO → Maybe you need a new module in modules/

**Pattern to follow:**
```c
// ✅ CORRECT: Module A needs Module B functionality
// File: modules/module_a/module_a.c
#include "../module_b/module_b.h"  // Import what you need

void module_a_function() {
    module_b_function();  // Direct call, no orchestrator!
}
```

**Pattern to avoid:**
```c
// ❌ WRONG: Creating central coordinator
// File: compiler/stage0/coordinator.c
#include "modules/module_a/module_a.h"
#include "modules/module_b/module_b.h"

void coordinate() {  // ❌ This pattern is FORBIDDEN!
    module_a_function();
    module_b_function();
}
```

**If you violate this rule:**
- Pre-commit hook will REJECT
- Build will FAIL
- Changes will be DELETED

### 🚨 Violation Examples

❌ **FORBIDDEN:**
```bash
# Creating ANY central coordination
touch compiler/stage0/orchestrator.c  # ❌ DELETED!
touch compiler/stage0/router.c        # ❌ NOT NEEDED!
touch compiler/stage0/pipeline.c      # ❌ WRONG PATTERN!
touch compiler/stage0/dispatcher.c    # ❌ ANTI-PATTERN!
touch compiler/stage0/coordinator.c   # ❌ NO!

# Creating utilities outside modules
touch compiler/stage0/utils.c
touch compiler/stage0/helpers.c
touch shared/common.c
```

✅ **CORRECT:**
```bash
# Using chained imports
cd modules/functions/
vim functions_parser.c
  → #include "../statement/statement_parser.h"  # ✅ Direct import

# Creating module-specific code
touch modules/string/string_utils.c
touch modules/control_flow/while_helpers.c

# Creating test
touch compiler/stage0/test_while_loop.c
```

### 💡 What If I Need Shared Code?

**Question:** "functions and control_flow both need statement parsing"

**Wrong answer:** "Create orchestrator to coordinate them" ❌

**Correct answer:** "Use chained imports!" ✅
```c
// modules/functions/functions_parser.c
#include "../statement/statement_parser.h"  // ✅ Import directly

// modules/control_flow/control_flow_parser.c  
#include "../statement/statement_parser.h"  // ✅ Same import

// No orchestrator needed! Linker handles it.
```

**Question:** "Multiple modules need same utility function"

**Options (in order of preference):**
1. ✅ Create `modules/parser_core/` or `modules/common/` 
2. ✅ Duplicate in both modules (OK for <50 lines!)
3. ✅ Use header-only utilities
4. ❌ Never create central utility file

```bash
# Option 1: Common module
modules/parser_core/
├── parser_core.h
└── parser_core.c

# Both modules import it:
modules/functions/functions_parser.c:
  → #include "../parser_core/parser_core.h"

modules/control_flow/control_flow_parser.c:
  → #include "../parser_core/parser_core.h"
```

### 🔐 Summary

> **Golden Rule: Use chained imports. No central orchestrators. Ever.**

**Current Working Architecture (8 Aralık 2025):**
```
Entry Point: functions_standalone.c (102 lines)
  → #include "modules/functions/functions_parser.h"
    → #include "../statement/statement_parser.h"
      → #include "../control_flow/control_flow_parser.h"
        → #include "../comparison/comparison_parser.h"

All linked naturally. No coordinator needed.
```

**Allowed File Creation:**
- ✅ `modules/[any_module]/*.c`
- ✅ `modules/[any_module]/*.h`
- ✅ `compiler/stage0/test_*.c` (test entry points only)
- ✅ `compiler/stage0/test_*.mlp` (test input files)
- ✅ `compiler/stage0/*.s` (generated assembly)

**Forbidden File Creation:**
- ❌ `compiler/stage0/orchestrator.c` (DELETED 7 Aralık!)
- ❌ `compiler/stage0/main.c` (DELETED 7 Aralık!)
- ❌ `compiler/stage0/helpers.c` (DELETED 7 Aralık!)
- ❌ `compiler/stage0/router.c` (NEVER CREATE!)
- ❌ `compiler/stage0/utils.c` (Use modules/parser_core/ instead)
- ❌ Any central coordination file

**Validation:**
```bash
./scripts/validate_architecture.sh  # Must pass!
```

**Remember:**
- Direct import = ✅ Good
- Central coordinator = ❌ Forbidden
- Real working proof: Current codebase compiles and generates correct assembly via chained imports

**Penalty:** Immediate rejection, no exceptions!

---

## 📏 File Size Limits

### Existing Files (FROZEN)

| File | Current | Max | Status |
|------|---------|-----|--------|
| `main.c` | 32 lines | 50 lines | ✅ FROZEN |
| `orchestrator.c` | 190 lines | 200 lines | ⚠️ FROZEN |
| `helpers.c` | 72 lines | 100 lines | ✅ FROZEN |

**FROZEN means:**
- Can be modified (bug fixes)
- CANNOT grow beyond limit
- If limit reached → Extract to module

### Module Files (FLEXIBLE)

- No hard limit
- Recommended: ≤500 lines per file
- Split into multiple files if needed

---

## 🔒 Enforcement Mechanisms

1. **Pre-commit Hook** - Blocks forbidden file creation
2. **Makefile Check** - `make check-forbidden-files`
3. **Architecture Validation** - `make check-architecture`
4. **Manual Review** - PR reviewers enforce rules

---

**Last Updated:** 7 Aralık 2025  
**Enforcement:** ACTIVE (pre-commit hook + Makefile)  
**Zero Tolerance:** No exceptions!
