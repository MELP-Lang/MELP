# 🎯 NEXT STEPS - Phase 2: Enhancement & Testing

**Date:** 8 Aralık 2025  
**Status:** ✅ **Phase 1 COMPLETE** - If/Else + While + Architecture Refactoring Done  
**Next Phase:** Cleanup, Testing, Logical Operations

---

## ✅ COMPLETED (Phase 1)

### Core Control Flow
- ✅ If/Else statements (with and without else)
- ✅ While loops
- ✅ Nested structures (tested working)
- ✅ Variable declarations with complex expressions
- ✅ Arithmetic expressions in all contexts

### Architectural Refactoring
- ✅ Stateless template pattern for parsers
- ✅ Token borrowing (clear ownership rules)
- ✅ Context passing for variable resolution
- ✅ Memory leak prevention (no malloc/free per parse)

**All test cases passing:**
- test_if.mlp (if with else)
- test_if_no_else.mlp (if without else)
- test_while.mlp (while loop)
- test_complex.mlp (complex expressions)
- test_return.mlp (return expressions)

---

## 🎯 PHASE 2: PRIORITIES

### Priority 1: Code Cleanup (30 min)
**Task:** Remove debug statements (82 found)
- Keep: Error messages (fprintf stderr "Error:")
- Remove: Debug flow prints (fprintf stderr "DEBUG")
- Status: Partial cleanup done, ~40 statements remaining

**Files to clean:**
- comparison_parser.c (remaining debug prints)
- Other parser modules

### Priority 2: Nested Structure Testing (1 hour)
**Test cases needed:**
```mlp
# test_nested_if.mlp
if a > 5 then
    if b > 10 then
        x = 1
    else
        x = 2
    end if
else
    x = 3
end if

# test_nested_while.mlp
while i < 10
    while j < 5
        sum = sum + 1
        j = j + 1
    end while
    i = i + 1
end while

# test_mixed.mlp
while x < 10
    if x > 5 then
        y = 1
    end if
    x = x + 1
end while
```

### Priority 3: Documentation Update (30 min)
- ✅ ARCHITECTURE.md (DONE - added parser patterns)
- ✅ IF_ELSE_IMPLEMENTATION.md (DONE - completion report)
- ⏳ NEXT_STEPS.md (this file - being updated now)

---

## 🚀 PHASE 3: NEW FEATURES (Future)

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
---

## 🚀 PHASE 3: NEW FEATURES (Future)

### Logical Operations (High Priority)
**Syntax:** `and`, `or`, `not`
```mlp
if (x > 5) and (y < 10) then
    # ...
end if
```

**Implementation:**
- Extend comparison_parser for logical operators
- Add short-circuit evaluation in codegen
- Test: compound conditions

### For Loops (Medium Priority)
**Syntax:**
```mlp
for i = 0 to 10
    # ...
end for
```

**Implementation:**
- Similar to while loop structure
- Initialize counter, test condition, increment
- Reuse while loop codegen patterns

### Function Parameters (Medium Priority)
**Current:** Functions parse but don't use parameters
**Needed:**
- Parameter to stack offset mapping
- Caller/callee register convention (rdi, rsi, rdx, rcx, r8, r9)
- Parameter passing in function calls

### Array Support (Low Priority)
**Syntax:**
```mlp
numeric[] arr = [1, 2, 3]
arr[0] = 5
```

---

## 🏗️ ARCHITECTURE REFERENCE (DON'T CHANGE!)

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

## ⚠️ YAPILMAMASI GEREKENLER

**ASLA YAPMA:**
- ❌ Merkezi orchestrator.c oluşturma
- ❌ helper.c, utils.c gibi genel dosyalar
- ❌ Modülleri birleştirme
- ❌ Zincirleme import'u bozma

**YAP:**
- ✅ Sadece lexer.c'yi düzelt
- ✅ Mevcut mimariyi koru
- ✅ Test sonuçlarını göster
- ✅ Her değişiklik sonrası validate_architecture.sh çalıştır