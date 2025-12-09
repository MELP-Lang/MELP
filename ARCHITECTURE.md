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

**CURRENT AI:** YZ_05 (String Support + TTO Type Tracking) ✅ COMPLETED
**PREVIOUS AI:** YZ_04 (9 Aralık 2025 - Control Flow Codegen) ✅ COMPLETED

---

## ⚠️ FOR AI AGENTS: READ THIS FIRST ⚠️

Previous AI agents violated these rules and created a 736-line monolithic `main.c`.
**DO NOT REPEAT THIS MISTAKE.**

---

## Rule #0: TTO (Transparent Type Optimization) - CORE PRINCIPLE

### 🎯 The Philosophy

**User sees only 2 types:**
- `numeric` - All numbers (integers, decimals, big numbers)
- `text` - All strings (short, long, constant)

**Compiler optimizes behind the scenes.** User never knows, never cares.

### 📊 Why This Approach?

**Key Insights:**
1. In other languages, BigDecimal is actually string-based
2. MLP has only `numeric` and `text` → essentially one concept: "data"
3. "Ali" and a 10,000-page book shouldn't use same memory strategy
4. User doesn't see pragmatic layer → backend optimization possible

### 🔄 How TTO Works

**User writes:**
```mlp
numeric small = 42
numeric decimal = 3.14159
numeric huge = 10 ^ 1000

text short = "Ali"
text long = read_file("book.txt")
```

**Compiler decides internally:**

| User Type | Value | Internal Type | Location | Speed |
|-----------|-------|---------------|----------|-------|
| `numeric` | 42 | int64 | register/stack | ⚡ Fastest |
| `numeric` | 3.14 | double | xmm register | ⚡ Fast |
| `numeric` | 10^100 | BigDecimal | heap | 🐢 Safe |
| `text` | "Ali" | SSO (inline) | stack | ⚡ Fastest |
| `text` | long... | heap pointer | heap | 🔄 Normal |
| `text` | constant | .rodata | readonly | ⚡ Shared |

### ⚠️ CRITICAL FOR AI AGENTS

**DO:**
- ✅ Track minimal type info (1 bit: `is_numeric` flag)
- ✅ Read `temp/kurallar_kitabı.md` TTO section for full details
- ✅ Keep user API simple (only `numeric` and `text`)

**DON'T:**
- ❌ Create complex type enums (VarType with 10 variants)
- ❌ Expose internal types to user
- ❌ Add type tracking beyond what's needed for codegen

**Example (YZ_05):**
```c
// ✅ CORRECT: Simple flag
typedef struct LocalVariable {
    char* name;
    int stack_offset;
    int is_numeric;    // 1=numeric, 0=text (2 types, 1 bit!)
    struct LocalVariable* next;
} LocalVariable;

// ❌ WRONG: Complex enum
typedef enum { VAR_INT, VAR_FLOAT, VAR_BIGDEC, VAR_STRING, ... } VarType;
```

**Key Principle:** "2 types, 1 bit, simple!" - Keep it minimal.

### 📚 Full TTO Documentation

For complete implementation details, algorithms, and memory strategies:
→ See **`temp/kurallar_kitabı.md`** Section 4: "Transparent Type Optimization (TTO)"

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

## Rule #5: MELP Value Proposition

**Core Principle:** Transparent optimization without user complexity

**User Experience:**
```mlp
numeric x = 3        # Simple syntax
text msg = "Hello"   # No type annotations needed
```

**Behind The Scenes:**
- Compiler automatically chooses optimal representation
- Small values → registers (fast)
- Large values → heap with safety (correct)
- User never sees complexity

**See Rule #0 (TTO) for full architecture details.**

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
