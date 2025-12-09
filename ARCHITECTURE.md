# MELP Architecture Rules - IMMUTABLE

## 🚨 CRITICAL: AI AGENT WORKFLOW (NEW - 9 Aralık 2025) 🚨

**EVERY AI AGENT MUST:**

1. **Read this file FIRST** before making any changes
2. **Create a numbered branch**: `git checkout -b feature-name_YZ_XX`
   - YZ_01 = First AI agent (this session)
   - YZ_02 = Second AI agent
   - YZ_03 = Third AI agent, etc.
3. **Work on your branch**, make commits with clear messages
4. **Before finishing**: `git push origin feature-name_YZ_XX`
5. **Leave a handoff note** in `/NEXT_AI_START_HERE.md`

**Example:**
```bash
git checkout -b tto-cleanup_YZ_01
# ... work ...
git add .
git commit -m "TTO: Remove legacy int/float API, pure numeric only"
git push origin tto-cleanup_YZ_01
```

**WHY:** 
- Track each AI's contribution clearly
- Prevent overwriting previous AI's work
- Easy rollback if needed
- Clear audit trail

**CURRENT AI:** YZ_03 (Next AI Agent - MVC Completion)
**PREVIOUS AI:** YZ_02 (9 Aralık 2025 - Stdlib Integration) ✅ COMPLETED

---

## ⚠️ FOR AI AGENTS: READ THIS FIRST ⚠️

Previous AI agents violated these rules and created a 736-line monolithic `main.c`.
**DO NOT REPEAT THIS MISTAKE.**

---

## Rule #1: NO CENTRAL FILES (Radical Modularity)

**WHY:** Central files (main.c, orchestrator.c, helpers.c) attract AI agents to write monolithic code.

**ENFORCED BY:** File removal + Makefile (`make check-architecture`)

**CURRENT STATUS:** 
- ✅ All central files backed up to `temp/yedek_merkezi_dosyalar/`
- ✅ AI agents FORCED to work inside `modules/` only

**VIOLATION CONSEQUENCE:** No central files exist = Cannot violate!

---

## Rule #2: Modules Communicate via JSON/Pipes ONLY

**FORBIDDEN:**
```c
// In main.c or orchestrator.c:
#include "modules/arithmetic/arithmetic_parser.h"  // ❌ NO!
result = arithmetic_parse(tokens);  // ❌ NO! (direct function call)
```

**ALLOWED:**
```bash
# Unix pipes (stdin/stdout):
echo '{"token":"NUMERIC"}' | ./modules/arithmetic/arithmetic

# JSON files:
./lexer < input.mlp > tokens.json
./parser < tokens.json > ast.json
```

**WHY:** 
- No C-level coupling
- Language-agnostic (can rewrite in MELP)
- Testable with simple text files

**ENFORCED BY:** No central files = No #include possible!

---

## Rule #3: Each Module is Standalone

**REQUIRED:** Every module must be compilable as standalone binary:

```
modules/arithmetic/arithmetic_compiler  (standalone)
modules/comparison/comparison_compiler  (standalone)
modules/tto/tto_analyzer               (standalone)
```

**WHY:** 
- Testable independently
- Replaceable without breaking system
- Self-hosting ready (can rewrite in MELP)

**COMMUNICATION:** JSON/stdin/stdout only

---

## Rule #4: Pipeline Architecture (Unix Philosophy)

**CORRECT FLOW:**
```
melp_lexer (standalone) 
    → tokens.json 
    → melp_parser (standalone)
    → ast.json
    → melp_tto (standalone)
    → ast_tto.json
    → melp_codegen (standalone)
    → output.s
```

**EACH STEP:**
- Separate binary
- Reads from stdin/file
- Writes to stdout/file
- No direct function calls between modules

---

## Rule #5: TTO is Non-Negotiable

**WITHOUT TTO:**
```c
numeric x = 3;  // Goes to HEAP → Performance disaster!
```

**WITH TTO:**
```c
numeric x = 3;  // Stays in REGISTER → Fast!
```

**WHY:** MELP's core value proposition is transparent optimization.

**STATUS:** TTO runtime needs implementation (Phase 3.5 priority)

---

## Current Architecture Status

### ✅ RESOLVED: TTO Duplicate Definition (9 Aralık 2025 - YZ_02)
- **Problem:** `tto_infer_numeric_type()` defined in both compiler and runtime
- **Action:** Renamed compiler functions with `codegen_` prefix (namespace separation)
- **Result:** Clean separation - compiler uses `codegen_tto_*`, runtime uses `tto_*`
- **Status:** ✅ NO HACKS (rejected `--allow-multiple-definition`)

### ✅ RESOLVED: Stdlib Integration (9 Aralık 2025 - YZ_02)
- **Achievement:** `println()` working in compiled programs! 🎉
- **Action:** Proper linker order - stdlib before tto_runtime
- **Result:** Function calls + stdlib = MVC ~90% complete
- **Status:** ✅ MODULAR (no central files created)

### ✅ RESOLVED: Central Files Removed! (7 Aralık 2025)
- **Action:** All central files moved to `temp/yedek_merkezi_dosyalar/`
- **Backed up:**
  - main.c, orchestrator.c, helpers.c (orchestration files)
  - lexer.c, lexer.h (will move to modules/lexer/)
  - cli/, pipeline/, error/ (will convert to modules)
- **Result:** AI agents CANNOT create monolithic code (no place to put it!)
- **Status:** ✅ RADICAL SOLUTION ACTIVE

### 📋 TODO: Convert Backed-up Files to Modules
1. **modules/lexer/** - Move lexer.c + add main.c (standalone)
2. **modules/parser/** - Create new parser module
3. **modules/codegen/** - Create new codegen module
4. **modules/cli/** - Convert cli/ directory
5. **Delete:** pipeline/ (replaced by Unix pipes)

### ✅ Problem: TTO runtime
- **Status:** ✅ RESOLVED (libtto_runtime.a linked)
- **Date:** 7 Aralık 2025

### ✅ Problem: Stdlib Integration
- **Status:** ✅ RESOLVED (println working, proper namespace separation)
- **Date:** 9 Aralık 2025 (YZ_02)

---

## How to Avoid Architecture Decay

### For Current AI Agent:
1. ✅ Implement TTO runtime FIRST
2. ✅ Keep main.c < 300 lines
3. ✅ Use existing modules, don't merge them
4. ✅ Run `make check-architecture` before committing

### For Future AI Agents:
1. 📖 Read this file BEFORE coding
2. 🛡️ Run checks: `make check-architecture`
3. 🧪 Test modules independently
4. ❌ NEVER merge modules into main.c "for simplicity"

---

## Enforcement

**Automatic Checks:**
```bash
make check-architecture   # Runs all architecture checks
make check-main-size      # Checks main.c line count
make check-coupling       # Checks forbidden imports
```

**Manual Review:**
- Every PR reviewed for architecture compliance
- Violations = immediate rejection

---

## Success Metrics

**Stage 0 (Current - 9 Aralık 2025):**
- [x] TTO runtime implemented ✅ (YZ_01)
- [x] Stdlib integrated ✅ (YZ_02)
- [x] Functions module complete ✅ (YZ_02)
- [x] Variables module complete ✅ (YZ_02)
- [x] println() working ✅ (YZ_02)
- [ ] Array/string integration tests (YZ_03 - next)
- [ ] MVC 100% complete

**Stage 1 (Self-hosting):**
- [ ] Each module has MELP rewrite
- [ ] Pipeline fully JSON-based
- [ ] Zero coupling between modules

---

## Remember

> "Architecture is about the important stuff. Whatever that is."
> — Ralph Johnson

For MELP, the important stuff is:
1. **Modularity** (AI can manage small pieces)
2. **TTO** (Performance without complexity)
3. **Self-hosting** (MELP written in MELP)

If you violate these, you're breaking MELP's core vision.

---

## 📊 AI Agent Progress Log

### YZ_02 (9 Aralık 2025) - Stdlib Integration ✅
- **Branch:** `stdlib-integration_YZ_02` (pushed)
- **Duration:** ~2 hours
- **Token Usage:** 75K / 1M (7.5%)
- **Achievements:**
  - ✅ Fixed TTO duplicate definition (namespace separation)
  - ✅ Integrated stdlib (println working!)
  - ✅ Maintained modular architecture (no central files)
  - ✅ Tests passing: println(42), function+println
- **Documentation:** `/YZ/YZ_02.md`
- **Status:** MVC ~90% complete

### YZ_01 (7 Aralık 2025) - TTO Cleanup ✅
- **Branch:** `tto-cleanup_YZ_01` (assumed pushed)
- **Achievements:**
  - ✅ TTO runtime linked
  - ✅ Central files removed
  - ✅ Radical modularity enforced
- **Status:** Foundation set for stdlib integration

---

**Last Updated:** 9 Aralık 2025  
**Status:** Architecture rules active, enforcement in progress
