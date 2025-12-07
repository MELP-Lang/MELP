# MELP Architecture Rules - IMMUTABLE

## ⚠️ FOR AI AGENTS: READ THIS FIRST ⚠️

Previous AI agents violated these rules and created a 736-line monolithic `main.c`.
**DO NOT REPEAT THIS MISTAKE.**

---

## Rule #1: main.c Maximum 300 Lines

**WHY:** Beyond 300 lines, files become unmaintainable by AI agents.

**ENFORCED BY:** Makefile (`make check-architecture`)

**CURRENT STATUS:** 
- main.c: 736 lines ❌ (NEEDS REFACTORING)
- Target: < 300 lines ✅

**VIOLATION CONSEQUENCE:** Build fails, commit rejected

---

## Rule #2: No Module Internal Imports in main.c

**FORBIDDEN:**
```c
#include "modules/arithmetic/arithmetic_parser.h"  // ❌ NO!
#include "modules/comparison/comparison.h"          // ❌ NO!
```

**ALLOWED:**
```c
#include "lexer.h"                                  // ✅ OK
#include "pipeline.h"                               // ✅ OK (orchestration only)
```

**WHY:** Tight coupling prevents modularity and self-hosting.

**ENFORCED BY:** Makefile + pre-commit hook

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

## Current Architecture Violations

### ❌ Problem 1: Monolithic main.c (744 lines) - CRITICAL!
- **Current:** 744 lines (248% over limit!)
- **Target:** 80 lines (orchestration only)
- **Fix:** Split into cli/, pipeline/, codegen/
- **Deadline:** IMMEDIATE (blocking all commits)
- **Status:** 🚨 ACTIVE VIOLATION

### ❌ Problem 2: Versioned binary name (melpc_26)
- **Current:** melpc_26
- **Target:** melpc
- **Fix:** Rename binary in Makefile
- **Deadline:** IMMEDIATE
- **Status:** 🚨 ACTIVE VIOLATION

### ✅ Problem 3: TTO runtime
- **Status:** ✅ RESOLVED (libtto_runtime.a linked)
- **Date:** 7 Aralık 2025

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

**Stage 0 (Current):**
- [ ] TTO runtime implemented
- [ ] main.c < 300 lines
- [ ] All modules testable standalone

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

**Last Updated:** 7 Aralık 2025  
**Status:** Architecture rules active, enforcement in progress
