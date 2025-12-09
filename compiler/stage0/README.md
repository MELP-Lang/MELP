# 🚀 START HERE - New AI Agent Onboarding

**Welcome!** You're taking over the MLP compiler development.  
**Last Session:** 9 Aralık 2025 - Phase 4.4 completed  
**Status:** All active parsers now stateless ✅

---

## 📖 Quick Start (5 minutes)

### 1️⃣ Read This First
**File:** `ARCHITECTURE.md` (Lines 1-150)
- Understand the stateless parser pattern
- Token ownership rules (BORROWED vs OWNED)
- Module chain architecture (no central orchestrator)

**Key Sections:**
- Lines 1-50: Current status & philosophy
- Lines 50-150: Stateless template pattern & examples
- Lines 575-700: Phase 4.4 completion details

### 2️⃣ Build & Test
```bash
cd compiler/stage0/modules/functions
make clean && make

# Test compilation
cat > test.mlp << 'EOF'
function main() returns numeric
    return 42
end function
EOF

./functions_compiler test.mlp test.s
gcc -no-pie test.s -o test
./test
echo $?  # Should output: 42
```

### 3️⃣ Explore the Code
**Reference Implementation:**
- `modules/functions/functions_parser.c` - Perfect stateless example
- `modules/variable/variable_parser.c` - With lexer_unget_token()
- `modules/logical/logical_parser.c` - Token** recursive pattern

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

## 📋 Current Status Summary

### ✅ What Works
- **Stateless Parsers:** functions, variable, logical, **array** ✨
- **Features:** Function declarations, parameters, return types
- **Control Flow:** if/else, while, for loops
- **Expressions:** Arithmetic, comparison (with overflow detection)
- **Variables:** Declarations, assignments, initialization
- **Error Handling:** Clean messages with line numbers
- **TTO (Trapped Type Overflow):** BigDec support for safe arithmetic

### ⏳ What's In Progress
- **array module:** Parser ✅ converted, entry point needs update
  - See: `modules/array/ARRAY_STATELESS_CONVERSION.md`
  - Parser: 470 lines, fully stateless with token borrowing
  - Entry point: Needs rewrite (30-45 min work)
  - Pattern: Same as functions_standalone.c (102 lines)

### 🐛 Known Issues
- Some TTO link errors in complex expressions (BigDec functions)
- Some tests have syntax that doesn't match current parser

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
