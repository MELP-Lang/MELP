# 🚀 START HERE - New AI Agent Onboarding (YZ Series)

**Welcome YZ_04!** You're taking over from YZ_03.  
**Last Session:** 9 Aralık 2025 - YZ_03: MVC 100% Complete! 🎉  
**Status:** Minimum Viable Compiler DONE, Control Flow Codegen Next

---

## 📖 Quick Start (5 minutes)

### 1️⃣ Read This First
**File:** `/NEXT_AI_START_HERE.md` (Your mission from YZ_03)
- Current status: MVC 100%
- Your goal: Control flow codegen
- Time estimate: 3-5 hours

**File:** `/ARCHITECTURE.md` (Critical rules)
- YZ workflow (branch, commit, handoff)
- Modular architecture (no central files)
- AI agent progress log

**File:** `/YZ/YZ_03.md` (Previous session details)
- What was fixed (text keyword, token type)
- Test results (30, 30, 60 ✅)
- Lessons learned

### 2️⃣ Build & Test MVC
```bash
cd compiler/stage0/modules/functions
make clean && make

# Test MVC (should work!)
./test_mvc
# Output: 30, 30, 60 ✅

# Or compile manually
./functions_compiler test_mvc.mlp test_mvc.s
gcc -no-pie test_mvc.s \
    -L../../../../runtime/stdlib -lmlp_stdlib \
    -L../../../../runtime/tto -ltto_runtime -lm \
    -o test_mvc
./test_mvc
```

### 3️⃣ Explore Control Flow
**Control Flow Parsers (Already Done!):**
- `modules/control_flow/control_flow_parser.c` - if/else, while
- `modules/for_loop/for_loop_parser.c` - for loops
- `modules/comparison/comparison_parser.c` - >, <, ==, !=

**What You Need to Implement:**
- `modules/control_flow/control_flow_codegen.c` - if/else assembly
- `modules/comparison/comparison_codegen.c` - comparison assembly
- `modules/for_loop/for_loop_codegen.c` - for loop assembly

---

## 🗺️ Project Structure

```
compiler/stage0/
├── ARCHITECTURE.md          ← 📖 READ THIS FIRST!
├── START_HERE.md            ← You are here
├── modules/
│   ├── functions/           ← ✅ Stateless (Phase 4.3)
│   │   ├── functions_parser.c
│   │   └── functions_standalone.c  (102 lines entry point)
│   ├── variable/            ← ✅ Stateless (Phase 4.4.1)
│   │   └── variable_parser.c
│   ├── logical/             ← ✅ Stateless (Phase 4.4.2)
│   │   └── logical_parser.c
│   ├── array/               ← ✅ Parser Stateless (9 Aralık)
│   │   ├── array_parser.c   (✅ stateless - 470 lines)
│   │   ├── STATELESS_TODO.md (marked complete)
│   │   ├── ARRAY_STATELESS_CONVERSION.md (full report)
│   │   └── array_standalone.c (⏳ needs stateless update)
│   ├── arithmetic/          ← ✅ Has _stateless version
│   ├── comparison/          ← ✅ Has _stateless version
│   ├── control_flow/        ← ✅ Uses stateless
│   ├── for_loop/            ← ✅ Uses stateless
│   ├── statement/           ← ✅ Updated to use stateless APIs
│   └── lexer/               ← Core (lexer_next_token, lexer_unget_token)
└── docs/
    └── PHASE_4_4_GUIDE.md   ← Detailed guide (now marked COMPLETED)
```

---

## 📋 Current Status Summary (YZ_03 Completion)

### ✅ MVC (Minimum Viable Compiler) - 100% Complete!
- **Functions:** Declaration + calls + return values ✅
- **Variables:** Numeric type (local in functions) ✅
- **Arithmetic:** All operations (+, -, *, /) ✅
- **Stdlib:** println() integrated (libmlp_stdlib.a) ✅
- **Test:** Comprehensive multi-function test passing ✅

**Example Working Program:**
```mlp
function add(numeric a, numeric b) returns numeric
    numeric result = a + b
    return result
end function

function main() returns numeric
    numeric sum = add(10, 20)
    numeric result = println(sum)  # Outputs: 30
    return 0
end function
```

### ⏳ Next Priority (YZ_04)
- **Control Flow Codegen:** if/else, while, for
  - Parsers exist ✅ (`control_flow_parser.c`, `for_loop_parser.c`)
  - Codegen needed ❌ (assembly generation)
  - Time estimate: 3-5 hours
  
- **Comparison Codegen:** >, <, ==, !=
  - Parser exists ✅ (`comparison_parser.c`)
  - Codegen partial ❌
  - Time estimate: 1 hour

**Goal:** Fibonacci and factorial programs working!

### 📊 Stage 0 Progress
```
MVC:              ████████████████████ 100% ✅
Parsers:          ████████████████████ 100% ✅
Codegen (Core):   ████████████████░░░░  80% 🟢
Codegen (Flow):   ████░░░░░░░░░░░░░░░░  20% 🟡
Overall:          ████████████████░░░░  80% 🟢
```

---

## 🎯 Recommended Next Steps

### Option A: New Features (Recommended)
1. **String Operations** - Add string concatenation, length
2. **Array Support** - Now that parser is clean, add runtime
3. **Struct Support** - Basic struct parsing and codegen
4. **Standard Library** - File I/O, math functions

### Option B: Polish & Testing
1. **Fix TTO Link Issues** - Complete BigDec integration
2. **Test Suite** - More comprehensive .mlp test files
3. **Error Recovery** - Better parser error handling
4. **Documentation** - Language spec, examples

### Option C: Self-Hosting Prep
1. **Bootstrap Compiler** - Write parser in MLP
2. **Type System** - Formalize type checking
3. **Optimizer** - Basic peephole optimization

**Suggestion:** Go with **Option A** - add features that make the language useful!

---

## 🔑 Key Concepts You Need to Know

### Stateless Parser Pattern
```c
// ❌ OLD (stateful):
Parser* p = parser_create(lexer);
AST* node = parse_something(p);
parser_free(p);

// ✅ NEW (stateless):
Token* tok = lexer_next_token(lexer);
AST* node = parse_something(lexer, tok);  // tok is BORROWED
token_free(tok);  // Caller owns token
```

### Token Ownership
- **BORROWED:** Function parameter tokens (don't free!)
- **OWNED:** Tokens from `lexer_next_token()` (must free!)
- **PUSHBACK:** `lexer_unget_token(lexer, tok)` for lookahead

### Module Chain (No Orchestrator)
```c
// Entry point includes what it needs:
#include "functions_parser.h"

// functions_parser includes what IT needs:
#include "../statement/statement_parser.h"

// statement_parser includes what IT needs:
#include "../control_flow/control_flow_parser.h"

// Natural dependency chain → linker handles it
```

---

## 📞 Getting Help

### When Stuck
1. **Read ARCHITECTURE.md** - Most questions answered there
2. **Look at functions_parser.c** - Reference implementation
3. **Check git history:** `git log --oneline --graph`
4. **Read commit messages:** They explain WHY decisions were made

### Understanding Decisions
- **Why stateless?** → ARCHITECTURE.md lines 47-75
- **Why no central orchestrator?** → ARCHITECTURE.md lines 13-45
- **Why defer array_parser?** → modules/array/STATELESS_TODO.md
- **Token borrowing pattern?** → ARCHITECTURE.md lines 76-120

---

## 🧪 Testing Checklist

Before making changes:
```bash
# 1. Build
cd modules/functions && make clean && make

# 2. Test basic function
./functions_compiler test_minimal.mlp test.s
gcc -no-pie test.s -o test && ./test
echo $?  # Verify exit code

# 3. Test variables
cat > test_var.mlp << 'EOF'
function main() returns numeric
    numeric x = 10
    numeric y = 20
    return x
end function
EOF
./functions_compiler test_var.mlp test.s
gcc -no-pie test.s -o test && ./test
echo $?  # Should be 10
```

After making changes:
- ✅ Code compiles with no errors
- ✅ Existing tests still pass
- ✅ New test for your feature passes
- ✅ No memory leaks (valgrind if available)
- ✅ Commit message explains WHY, not just WHAT

---

## 💡 Pro Tips

1. **Don't Refactor Unused Code** - YAGNI principle (see array_parser decision)
2. **Document Deferrals** - Create TODO.md files for future work
3. **Pragmatic > Perfect** - Working features > theoretical completeness
4. **Test Incrementally** - Don't build for hours without testing
5. **Read Commits** - Previous AI explained decisions thoroughly

---

## 🎓 Philosophy Reminders

From ARCHITECTURE.md:
1. **Stateless > Stateful** - Parsers are functions, not objects
2. **Borrowed > Owned** - Clear token ownership prevents bugs
3. **Chain > Orchestrator** - Natural dependencies via #include
4. **Pragmatic > Dogmatic** - YAGNI, ROI, incremental progress

---

## 🚦 You're Ready!

**Checklist before starting:**
- [ ] Read ARCHITECTURE.md (at least first 150 lines)
- [ ] Built and tested current compiler
- [ ] Understand stateless parser pattern
- [ ] Know token ownership rules
- [ ] Reviewed functions_parser.c example

**Now go build something awesome!** 🚀

**Questions?** Check ARCHITECTURE.md first. Most answers are there.

**Good luck!** The previous AI left you a solid foundation. 💪
