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
6. **⚠️ NEVER merge or create pull requests** - Human will review and merge

**Example:**
```bash
git checkout -b sto-cleanup_YZ_01
# ... work ...
git add .
git commit -m "STO: Remove legacy int/float API, pure numeric only"
git push origin sto-cleanup_YZ_01
```

**WHY:** 
- Track each AI's contribution clearly
- Prevent overwriting previous AI's work
- Easy rollback if needed
- Clear audit trail
- **Human reviews and merges** - AI never merges to main

**CURRENT AI:** YZ_49 (Phase 12: STO Refactoring - Documentation)
**PREVIOUS AI:** YZ_48 (12 Aralık 2025 - println() Complete) ✅ COMPLETED

---

## ⚠️ FOR AI AGENTS: READ THIS FIRST ⚠️

Previous AI agents violated these rules and created a 736-line monolithic `main.c`.
**DO NOT REPEAT THIS MISTAKE.**

---

## Rule #0: STO (Smart Type Optimization) - CORE PRINCIPLE

### 🎯 The Philosophy

**User sees only 2 types:**
- `numeric` - All numbers (integers, decimals, big numbers)
- `text` - All strings (short, long, constant)

**Compiler optimizes behind the scenes.** User never knows, never cares.

### 📊 Why This Approach?

**Key Insights:**
1. In other languages, BigDecimal is actually string-based
2. MLP has only `numeric` and `string` → essentially one concept: "data"
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
- ✅ Keep user API simple (only `numeric` and `string`)

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
    int is_numeric;    // 1=numeric, 0=string (2 types, 1 bit!)
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

## Rule #0.5: Stateless Architecture

### 🎯 MELP is Stateless by Default

**Core Philosophy:** MELP is a **stateless language by default**. All variables are function-scoped (local) and do not persist between function calls.

#### 1. Language Level: Stateless by Default
- **No global variables** - All variables are local to functions
- **No implicit state** - Functions are pure by default
- **Explicit state when needed** - Use `import state` module for persistence
- **Predictable behavior** - Same inputs always produce same outputs

```mlp
-- ✅ Default: Stateless (no state persists)
function counter() returns numeric
    numeric x = 0    -- Always starts at 0
    x = x + 1
    return x         -- Always returns 1
end function

-- ✅ Explicit state: Import state module when needed
import state
state.set("counter", 0)

function counter_with_state() returns numeric
    numeric x = state.get("counter")
    x = x + 1
    state.set("counter", x)
    return x         -- Returns 1, 2, 3, 4...
end function
```

#### 2. Compiler Level: Stateless Implementation
- Parser functions are stateless (`*_stateless()` pattern)
- No global state between modules
- Token ownership/borrowing pattern for memory management
- Pipeline: Lexer → Parser → Codegen are separate, independent stages

**Why Stateless by Default?**
- Easier to test and debug
- Predictable, deterministic behavior
- Better for parallel execution
- State is opt-in, not opt-out

---

## Rule #0.6: Design Philosophy

### 🎯 MELP = VB.NET Readability + Python Simplicity

MELP combines the best of both worlds:

| Decision | MELP Choice | Reason |
|----------|-------------|--------|
| `=` vs `==` | `=` assignment, `==` comparison | Explicit, prevents bugs (unlike VB.NET's context-dependent `=`) |
| `function` keyword | Single keyword, no `Sub` | Python-like simplicity, no void/return distinction needed |
| Variable declaration | `numeric x = 5` (no `Dim`) | Type-first is clear enough, `Dim` is verbose |
| String type | `string` keyword | Universal term, understood in all languages |
| Collection iteration | `for each item in collection` | VB.NET compatibility, readable |
| Block endings | `end if`, `end while`, `end function` | VB.NET style, self-documenting |

### ⚠️ Design Principles

1. **Readability over brevity** - `end if` not `}`
2. **Explicit over implicit** - `==` for comparison, not context-dependent `=`
3. **One way to do things** - Single `function` keyword, no `Sub`/`Function` split
4. **Type-first declarations** - `numeric x`, not `Dim x As Integer`

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

## Rule #6: LLVM Backend (Phase 13.5) 🆕

### 🎯 Compiler Backend Architecture

**Design Decision:** Two-backend system for flexibility and portability

**Backend Options:**
1. **Assembly Backend** (x86-64) - Direct assembly generation
2. **LLVM IR Backend** (Portable) - LLVM IR text format (`.ll`)

**Selection:** Via `--backend` flag (default: assembly)

```bash
# Assembly backend (original, x86-64 only)
./functions_compiler -c input.mlp output.s

# LLVM backend (portable, multi-platform)
./functions_compiler -c --backend=llvm input.mlp output.ll
clang output.ll -o program
```

### 📁 LLVM Backend Module Structure

```
compiler/stage0/modules/
├── llvm_backend/           # Core LLVM IR emission
│   ├── llvm_backend.h      # API definitions (174 lines)
│   ├── llvm_backend.c      # IR generation (368 lines)
│   ├── Makefile           # Independent build system
│   └── test_llvm_backend.c # Unit tests
│
└── functions/              # Integration layer
    ├── functions_codegen_llvm.h   # LLVM codegen wrapper
    ├── functions_codegen_llvm.c   # Statement/expression IR gen (535 lines)
    └── functions_standalone.c     # CLI with --backend flag
```

### 🔧 LLVM Backend API Design

**Principle:** Clean abstraction over LLVM IR text format

```c
// Context management
LLVMContext* ctx = llvm_context_create(output_file);

// Module-level
llvm_emit_module_header(ctx);
llvm_emit_function_start(ctx, "main", params, count);
llvm_emit_function_entry(ctx);

// Variables
LLVMValue* ptr = llvm_emit_alloca(ctx, "x");
LLVMValue* val = llvm_const_i64(42);
llvm_emit_store(ctx, val, ptr);
LLVMValue* loaded = llvm_emit_load(ctx, ptr);

// Arithmetic
LLVMValue* sum = llvm_emit_add(ctx, left, right);
LLVMValue* diff = llvm_emit_sub(ctx, left, right);

// Logical (Boolean)
LLVMValue* and_result = llvm_emit_and(ctx, a, b);
LLVMValue* or_result = llvm_emit_or(ctx, a, b);

// Comparison
LLVMValue* cmp = llvm_emit_icmp(ctx, "sgt", a, b);  // signed greater than

// Control flow
llvm_emit_br_cond(ctx, condition, "then_label", "else_label");
llvm_emit_label(ctx, "then_label");
llvm_emit_br(ctx, "after_if");

// Function end
llvm_emit_return(ctx, result);
llvm_emit_function_end(ctx);
llvm_emit_module_footer(ctx);
```

### ✅ Implemented Features (Phase 13.5)

**Part 1-4: Core Infrastructure** (YZ_57)
- ✅ LLVM backend module (llvm_backend.c/h)
- ✅ Integration with functions_compiler
- ✅ `--backend=llvm` CLI flag
- ✅ Basic arithmetic operations (+, -, *, /)
- ✅ Function calls and parameters
- ✅ Variable declarations and assignments

**Part 5.1: Control Flow** (YZ_58)
- ✅ If/else statements with conditional branches
- ✅ Comparison operators (>, <, ==, !=, >=, <=)
- ✅ Label generation and branching
- ✅ Assignment statements

**Part 5.2: Boolean Operations** (YZ_59)
- ✅ Boolean literals (true=1, false=0)
- ✅ Logical AND operation (`and i64`)
- ✅ Logical OR operation (`or i64`)
- ✅ Boolean expression evaluation

### 📊 Test Results

**All tests passing (8/8):**
```bash
✅ test_basic.mlp          # 10 + 20 = Exit 30
✅ test_sanity.mlp         # return 100 = Exit 100
✅ test_llvm_functions.mlp # add(15, 27) = Exit 42
✅ test_llvm_if.mlp        # if 15 > 10 then 1 else 0 = Exit 1
✅ test_llvm_assign.mlp    # x=30, y=25 = Exit 25
✅ test_boolean_and.mlp    # true and false = Exit 0
✅ test_boolean_and_true.mlp # true and true = Exit 1
✅ test_boolean_or.mlp     # true or false = Exit 1
```

### ⚠️ Known Limitations

1. **While/For Loops:** Codegen ready, parser doesn't support them yet
2. **Standard Library:** Using printf temporarily (TODO: mlp_println_numeric)
3. **Optimization:** No LLVM opt passes yet (focus on correctness)
4. **String Operations:** Not implemented in LLVM backend

### 🔜 Next Steps (Phase 14)

- Parser enhancement for while/for loops
- Standard library integration (mlp_println_numeric)
- Self-hosting lexer/parser with LLVM backend
- Performance benchmarking vs Assembly backend

### 📚 Documentation

- **LLVM IR Guide:** `docs/LLVM_IR_GUIDE.md` (753 lines)
- **Examples:** `examples/llvm/*.ll` (working IR samples)
- **Session Reports:** `YZ/YZ_57.md`, `YZ/YZ_58.md`, `YZ/YZ_59.md`

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

### YZ_59 (13 Aralık 2025) - LLVM Boolean Operations ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Duration:** ~2 hours
- **Token Usage:** 56K / 1M (5.6%)
- **Achievements:**
  - ✅ Boolean literal support (true=1, false=0)
  - ✅ Logical AND operation (`and i64`)
  - ✅ Logical OR operation (`or i64`)
  - ✅ All 8 LLVM tests passing
- **Documentation:** Part of Phase 13.5 (YZ_57-YZ_59)
- **Status:** Phase 13.5 ~95% complete

### YZ_58 (13 Aralık 2025) - LLVM Control Flow ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Achievements:**
  - ✅ If/else statements with conditional branches
  - ✅ Comparison operators (>, <, ==, !=, >=, <=)
  - ✅ Assignment statements
  - ✅ Label generation and branching
- **Documentation:** `/YZ/YZ_58.md`
- **Status:** Control flow complete

### YZ_57 (12-13 Aralık 2025) - LLVM Backend Core ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Duration:** ~6 hours
- **Achievements:**
  - ✅ LLVM backend module (llvm_backend.c/h)
  - ✅ Integration with functions_compiler
  - ✅ `--backend=llvm` flag working
  - ✅ Basic arithmetic and function calls
- **Documentation:** `/YZ/YZ_57.md`, `docs/LLVM_IR_GUIDE.md`
- **Status:** Foundation complete

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
