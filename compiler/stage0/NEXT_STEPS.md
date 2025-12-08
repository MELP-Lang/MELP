# 🎯 NEXT AI START HERE - Lexer Bug Fix Required!

**Date:** 8 Aralık 2025  
**Status:** 🐛 **CRITICAL BUG BLOCKING PROGRESS**  
**Priority:** Fix lexer duplicate token generation IMMEDIATELY

---

## 🚨 IMMEDIATE ISSUE: Lexer Duplicate Token Bug

**What's broken:**
```bash
./test_lexer_debug test_var_simple.mlp
# Output shows:
7: type=0 value='numeric'
8: type=3 value='x'
9: type=3 value='x'  ← DUPLICATE! Should not exist
10: type=4 value='='
11: type=5 value='5'
```

**Impact:**
- Variable assignments fail (only first statement parsed)
- Multi-statement function bodies broken
- Blocks all further progress

**Test Case:**
```mlp
# test_var_simple.mlp
function test() returns numeric
    numeric x
    x = 5      ← Parser never reaches this line!
    return x
end function
```

**Debug Files:**
- `test_lexer_debug.c` - Standalone lexer test (shows bug clearly)
- `test_var_simple.mlp` - Minimal failing test case

---

## ✅ What's Working (DON'T BREAK THIS!)

**Architecture:** Chained imports - NO central files!
- ✅ functions → statement → control_flow → comparison
- ✅ Entry: `functions_standalone.c` (102 lines, down from 1244)
- ✅ NO main.c, NO orchestrator.c, NO helpers.c (deleted 7 Aralık)

**Parser:** Current_token mechanism prevents token loss
- ✅ Return statements: `return 10 + 5 * 2` works with TTO
- ✅ Function parameters: `function test(numeric x)` parses correctly
- ✅ Return type: `returns numeric` keyword working

**Codegen:** Return statements generate correct assembly
```asm
# return 10 + 5 * 2;
mov r8, 10
mov r9, 5
mov r10, 2
imul r9, r10
add r8, r9
movq %r8, %rax  # Return value in rax
```

---

## 🔧 How to Fix the Lexer Bug

**Step 1:** Investigate `modules/lexer/lexer.c`
- Look for identifier/keyword tokenization logic
- Check if buffer position advances correctly after identifier
- Search for any double-read patterns

**Step 2:** Test with `test_lexer_debug.c`
```bash
cd compiler/stage0
gcc test_lexer_debug.c modules/lexer/lexer.c modules/comments/comments.c -I. -o test_lexer_debug
./test_lexer_debug test_var_simple.mlp
```

**Expected output after fix:**
```
7: type=0 value='numeric'
8: type=3 value='x'
10: type=4 value='='      ← No duplicate 'x' here!
11: type=5 value='5'
12: type=0 value='return'
13: type=3 value='x'
```

**Step 3:** Verify with full parser
```bash
cd modules/functions
make clean && make
./test_function_body ../test_var_simple.mlp
```

Should output: "Parsed 3 statements in function body" (not just 1!)

---

## 📋 After Lexer Fix - Next Steps

1. **Complete variable assignment codegen** (stub ready in `statement_codegen.c:59-71`)
   - Evaluate expression to r8
   - Store to variable's stack location: `movq %r8, -8(%rbp)`

2. **Implement stack offset resolution** (TODO comments in place)
   - Track local variables in function context
   - Assign offsets: -8(%rbp), -16(%rbp), -24(%rbp), ...

3. **Remove debug output** (after verifying fix works)
   - `functions_parser.c` lines ~195-215
   - `statement_parser.c` lines ~20-27, ~175-180
   - `variable_parser.c` lines ~108-113

---

## 🏗️ Architecture Reference (DON'T CHANGE!)

**Chained Imports Pattern:**
```c
// functions_standalone.c (102 lines - test entry point)
#include "modules/functions/functions_parser.h"

// modules/functions/functions_parser.c (280 lines)
#include "../statement/statement_parser.h"  // ✅ Chained!

// modules/statement/statement_parser.c (216 lines)
#include "../arithmetic/arithmetic_parser.h"
#include "../variable/variable_parser.h"
#include "../control_flow/control_flow_parser.h"

// modules/control_flow/control_flow_parser.c
#include "../comparison/comparison_parser.h"
```

**Validation:** `scripts/validate_architecture.sh` enforces rules
- Rejects creation of main.c, orchestrator.c, helpers.c, router.c
- Rejects new files in `compiler/stage0/` (tests only!)

---

## 🎯 Success Criteria

- [ ] `./test_lexer_debug` shows NO duplicate tokens
- [ ] `test_var_simple.mlp` parses all 3 statements
- [ ] Variable assignments compile to correct assembly
- [ ] Architecture remains modular (no central files)
