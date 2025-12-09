# 🎯 MELP Compiler - TODO List
**Güncel Durum:** 9 Aralık 2025, 21:20  
**Son Tamamlanan:** YZ_06 (String Operations Runtime) ✅  
**Stage:** Stage 0 - Core Compiler Development  
**Completion:** ~70%

---

## 📊 Genel Durum

### ✅ Tamamlanan (WORKS!)
- [x] **Lexer** - Token parsing (keywords, literals, operators)
- [x] **Parser** - Statement ve expression parsing
- [x] **Functions** - Declaration, calls, parameters, return values
- [x] **Variables** - Numeric ve text declarations
- [x] **Arithmetic** - +, -, *, / operations
- [x] **Comparison** - <, <=, >, >=, ==, != operators
- [x] **Control Flow** - if/else, while loops
- [x] **Recursion** - Recursive function calls (Fibonacci works!)
- [x] **String Literals** - "Hello World" in .rodata section
- [x] **TTO Type Tracking** - is_numeric flag (1 bit per variable)
- [x] **Runtime - Numeric I/O** - println(numeric), print(numeric)
- [x] **Runtime - String I/O** - println(text), print(text)
- [x] **Runtime - String Ops** - concat(), compare() functions

### 🚧 Devam Eden (IN PROGRESS)
- [ ] **String Concatenation Codegen** - text c = a + b (YZ_07 görevi)
- [ ] **String Comparison Codegen** - if str == "test" (YZ_07 görevi)

### ⏳ Yapılacak (TODO - Öncelik Sırasıyla)

---

## 🎯 Phase 1: String Operations (2-3 saat)
**Responsible:** YZ_07  
**Priority:** ⭐⭐⭐ CRITICAL

- [ ] **String Concat Codegen** (45 min)
  - File: `modules/arithmetic/arithmetic_codegen.c`
  - Pattern: Check is_numeric, call mlp_string_concat
  - Test: `text c = "Hello" + "World"`

- [ ] **String Compare Codegen** (45 min)
  - File: `modules/comparison/comparison_codegen.c`
  - Pattern: Check is_numeric, call mlp_string_compare
  - Test: `if password == "secret"`

- [ ] **Integration Tests** (60 min)
  - Multi-operand concat: `a + b + c`
  - All 6 comparison operators
  - Mixed operations

**Deliverable:** String operations fully working end-to-end

---

## 🎯 Phase 2: For Loops (1-2 saat)
**Priority:** ⭐⭐ HIGH

- [ ] **For Loop Codegen** (90 min)
  - File: `modules/for_loop/for_loop_codegen.c`
  - Pattern: Similar to while loop
  - Test: `for i = 1 to 10`

- [ ] **Range Iteration** (30 min)
  - Support: `for i = start to end`
  - Support: `for i = start to end step 2`

**Deliverable:** For loops working

---

## 🎯 Phase 3: Arrays & Collections (4-6 saat)
**Priority:** ⭐⭐ HIGH

YZ_05 notes indicate array module already has parser/codegen/runtime started!

- [ ] **Array Literals** (2 hours)
  - Syntax: `[1, 2, 3]`
  - Codegen: Call tto_array_alloc()
  - Test: Create and access arrays

- [ ] **Array Access** (1 hour)
  - Syntax: `arr[i]`
  - Codegen: Bounds checking
  - Test: Read/write array elements

- [ ] **Lists (Heterogeneous)** (2 hours)
  - Syntax: `(1; "hello"; 3.14;)`
  - Runtime: tto_list_alloc() already exists
  - Test: Mixed-type collections

- [ ] **Tuples (Immutable)** (1 hour)
  - Syntax: `<x, y>`
  - Runtime: tto_tuple_alloc() already exists
  - Test: Pair and triple tuples

**Deliverable:** Array/List/Tuple support complete

---

## 🎯 Phase 4: Boolean Type (1-2 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Boolean Keyword** (30 min)
  - Add `boolean` to lexer
  - Add to type system

- [ ] **Boolean Literals** (30 min)
  - Add `true` and `false` keywords
  - Codegen: Store as 1/0

- [ ] **Boolean Operations** (60 min)
  - Logical: `and`, `or`, `not`
  - Already parsed, need codegen
  - Test: `if (x > 5 and y < 10)`

**Deliverable:** Boolean type fully supported

---

## 🎯 Phase 5: More Stdlib Functions (2-3 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Input Functions** (60 min)
  - `input()` - Read line from stdin
  - `input_numeric()` - Parse to number
  - Test: Interactive programs

- [ ] **Type Conversion** (60 min)
  - `toString(numeric)` - Already in mlp_io.c!
  - `toNumeric(text)` - Parse string to number
  - Test: Conversions

- [ ] **String Methods** (60 min)
  - `length(text)` - Already in mlp_string.c!
  - `substring(text, start, len)`
  - `indexOf(text, pattern)`
  - Test: String manipulation

**Deliverable:** Rich standard library

---

## 🎯 Phase 6: Error Handling (2-3 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Better Error Messages** (90 min)
  - Line numbers in parse errors
  - Helpful suggestions
  - Colored output

- [ ] **Runtime Errors** (60 min)
  - Division by zero
  - Array bounds checks
  - Null pointer checks

- [ ] **Error Recovery** (60 min)
  - Continue parsing after error
  - Show multiple errors

**Deliverable:** User-friendly error system

---

## 🎯 Phase 7: Optimization (3-5 saat)
**Priority:** ⭐ LOW (after basics work)

- [ ] **Constant Folding** (90 min)
  - `x = 2 + 3` → `x = 5` at compile time

- [ ] **Dead Code Elimination** (60 min)
  - Remove unreachable code

- [ ] **Register Allocation** (120 min)
  - Better use of registers
  - Reduce stack usage

**Deliverable:** Faster generated code

---

## 🎯 Phase 8: Self-Hosting Prep (5-10 saat)
**Priority:** ⭐ LOW (far future)

- [ ] **File I/O** (2 hours)
  - Read/write files
  - Parse MLP from MLP

- [ ] **Module System** (3 hours)
  - Import/export
  - Separate compilation

- [ ] **Rewrite Lexer in MLP** (5 hours)
  - First self-hosted component!

**Deliverable:** Begin self-hosting journey

---

## 🔧 Infrastructure TODO

### Build System (3-4 saat)
**Priority:** ⭐ MEDIUM

- [ ] **Fix Makefile Linking** (2 hours)
  - Current: Has errors with pipeline/, lexer
  - Goal: Clean compilation

- [ ] **Better Test Framework** (90 min)
  - Automated test runner
  - Expected vs actual output
  - Regression testing

- [ ] **CI/CD Setup** (60 min)
  - GitHub Actions
  - Auto-test on push

### Documentation (ongoing)
- [ ] **Language Specification** (in progress)
- [ ] **Standard Library Docs** (in progress)
- [ ] **Tutorial Examples** (partially done)

---

## 📈 Completion Timeline

| Phase | Priority | Time | Status |
|-------|----------|------|--------|
| String Ops | ⭐⭐⭐ | 2-3h | 🚧 Runtime ✅, Codegen ⏳ |
| For Loops | ⭐⭐ | 1-2h | ⏳ Parser ready |
| Arrays | ⭐⭐ | 4-6h | ⏳ Runtime exists |
| Booleans | ⭐ | 1-2h | ⏳ |
| Stdlib | ⭐ | 2-3h | 🚧 Partial |
| Errors | ⭐ | 2-3h | ⏳ |
| Optimization | ⭐ | 3-5h | ⏳ |
| Self-hosting | ⭐ | 5-10h | ⏳ Future |

**Total Estimated Work:** 20-30 hours  
**Current Completion:** ~70%  
**To MVP (Minimal Viable):** ~10 hours  
**To Production Ready:** ~25 hours

---

## 🎉 Major Milestones Achieved

- ✅ **YZ_01** - TTO Architecture cleanup
- ✅ **YZ_02** - Module system foundation
- ✅ **YZ_03** - MVC (Minimum Viable Compiler) complete!
- ✅ **YZ_04** - Control flow codegen (Fibonacci works!)
- ✅ **YZ_05** - String literals + TTO type tracking
- ✅ **YZ_06** - String operations runtime

**Next:** YZ_07 - String operations codegen (2-3 hours)

---

## 🚀 Quick Actions

**For immediate progress:**
1. ✅ String operations runtime (DONE by YZ_06)
2. ⏳ String operations codegen (YZ_07 - START HERE!)
3. ⏳ For loop codegen
4. ⏳ Array support

**For robustness:**
1. Better error messages
2. Fix build system
3. Automated testing

**For features:**
1. Boolean type
2. More stdlib functions
3. Input/output

---

## 📞 Contact Info

- **Architecture:** See `ARCHITECTURE.md`
- **TTO Details:** See `temp/kurallar_kitabı.md`
- **Current Status:** See `STATUS_9_ARALIK_2025.md`
- **Next Steps:** See `NEXT_AI_START_HERE.md`
- **AI Sessions:** See `YZ/YZ_*.md`

---

**Last Updated:** 9 Aralık 2025, 21:20 by YZ_06  
**Next AI:** YZ_07 (String operations codegen)  
**Estimated Completion:** Stage 0 MVP in ~10 hours of focused work
