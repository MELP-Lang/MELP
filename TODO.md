# 📋 MELP Project TODO - Phase 14 (Parser Enhancement for Control Flow)

**Created:** 13 Aralık 2025  
**Current Focus:** ✅ Phase 14 Complete - Control Flow Support  
**Status:** Phase 14 ✅ Complete (100% - 13/13 tests passing)

---

## ⚠️ BAŞLAMADAN ÖNCE - YENİ AI İÇİN KRİTİK UYARI

**📖 ZORUNLU OKUMA (ÖNCE BURAYI OKU!):**  
👉 **`NEXT_AI_START_HERE.md`** - Current session için mission brief (Buradan başla!)  
👉 **`YZ/YZ_HISTORY.md`** - Tüm YZ oturumlarının özeti

**🔴 GIT KURALLARI (ÇOK ÖNEMLİ!):**
```bash
# ⚠️ HER YZ AYRI DAL OLUŞTURMALIDIR! ⚠️

# 1. YENİ DAL OLUŞTUR (her zaman main/master'dan)
git checkout main
git pull origin main
git checkout -b feature-description_YZ_XX

# ÖRNEK:
# YZ_60: git checkout -b phase14-parser-enhancement_YZ_60
# YZ_61: git checkout -b phase14-testing_YZ_61

# 2. Çalış, commit et
git add .
git commit -m "YZ_XX: Feature description"

# 3. KENDI DALINA push et
git push origin feature-description_YZ_XX

# 4. ⚠️ ASLA MERGE YAPMA veya PULL REQUEST OLUŞTURMA!
# Human review yapıp merge edecek.
```

**📖 SONRA BUNLARI OKU:**  
👉 **`ARCHITECTURE.md`** - Mimari kurallar (modülerlik, STO, LLVM Backend)  
👉 **`YZ/AI_METHODOLOGY.md`** - 5 adımlı hızlı geliştirme metodu  
👉 **`docs/LLVM_IR_GUIDE.md`** - LLVM IR mapping patterns  
👉 **`temp/TODO_phase13.5_complete_20251213.md`** - Completed Phase 13.5 reference

---

## 🎯 Current Milestone: Phase 14 - Parser Enhancement for Control Flow

### Goal
Add while and for loop parsing support to `functions_parser.c` to enable full control flow in LLVM backend.

### Context
- LLVM backend codegen for while/for loops is **already implemented** (YZ_58)
- Parser currently doesn't recognize while/for statements
- Need to add AST generation for these constructs

### Why This Matters
- Completes the LLVM backend feature set
- Enables writing real-world programs with loops
- Unlocks full potential of MELP's control flow system

---

## 📦 Phase 14 Tasks

### Part 1: While Loop Parser Implementation (90 min)

#### Task 1.1: Study Existing Parser Structure (15 min)
- [x] Read `compiler/stage0/modules/functions/functions_parser.c`
- [x] Understand current statement parsing flow
- [x] Review control_flow module integration
- [x] Check how if/else statements are parsed

**Files to Review:**
- `compiler/stage0/modules/control_flow/control_flow_parser.h`
- `compiler/stage0/modules/functions/functions_parser.c`
- `compiler/stage0/modules/statement/statement.h` (AST definitions)

#### Task 1.2: Implement While Statement Parsing (30 min)
- [ ] Add while keyword recognition
- [ ] Parse while condition expression
- [ ] Parse while body (statement list)
- [ ] Create WhileStatement AST node
- [ ] Add to statement parsing switch

**Implementation Steps:**
```c
// 1. In parse_statement():
case TOKEN_WHILE:
    return parse_while_statement(lexer, current);

// 2. Implement parse_while_statement():
Statement* parse_while_statement(Lexer* lexer, Token** current) {
    // Parse: while <condition> ... end while
    // 1. Consume 'while' keyword
    // 2. Parse condition expression
    // 3. Parse body statements until 'end'
    // 4. Consume 'end while'
    // 5. Return STMT_WHILE with condition and body
}
```

**Files to Modify:**
- `compiler/stage0/modules/functions/functions_parser.c`

#### Task 1.3: Test While Loop Parsing (30 min)
- [ ] Create test file: `test_while_loop.mlp`
- [ ] Compile with `--backend=llvm`
- [ ] Verify LLVM IR generation
- [ ] Test execution and output
- [ ] Fix any parsing errors

**Test Case:**
```mlp
function main() returns numeric
    numeric x = 0
    while x < 5
        x = x + 1
    end while
    return x  -- Should return 5
end function
```

#### Task 1.4: Documentation (15 min)
- [ ] Update parser comments
- [ ] Document while loop syntax
- [ ] Add to test suite

**Deliverables:**
- While loops working in functions_compiler ✓
- Test case passing ✓
- Parser documentation updated ✓

---

### Part 2: For Loop Parser Implementation (90 min)

#### Task 2.1: Study For Loop Requirements (15 min)
- [ ] Review existing for loop codegen (YZ_58)
- [ ] Check AST structure needed
- [ ] Understand `for i = start to end` syntax
- [ ] Review increment/decrement logic

**Reference:**
- `compiler/stage0/modules/control_flow/control_flow_codegen.c`
- Look for `control_flow_generate_for()`

#### Task 2.2: Implement For Statement Parsing (30 min)
- [ ] Add for keyword recognition
- [ ] Parse iterator variable
- [ ] Parse start expression
- [ ] Parse end expression
- [ ] Parse optional step (future enhancement)
- [ ] Parse for body
- [ ] Create ForStatement AST node

**Implementation Steps:**
```c
// Parse: for i = start to end ... end for
Statement* parse_for_statement(Lexer* lexer, Token** current) {
    // 1. Consume 'for' keyword
    // 2. Parse iterator variable name
    // 3. Consume '=' token
    // 4. Parse start expression
    // 5. Consume 'to' keyword
    // 6. Parse end expression
    // 7. Parse body statements
    // 8. Consume 'end for'
    // 9. Return STMT_FOR
}
```

**Files to Modify:**
- `compiler/stage0/modules/functions/functions_parser.c`

#### Task 2.3: Test For Loop Parsing (30 min)
- [ ] Create test file: `test_for_loop.mlp`
- [ ] Compile with `--backend=llvm`
- [ ] Verify LLVM IR generation
- [ ] Test execution and output
- [ ] Fix any parsing errors

**Test Cases:**
```mlp
function main() returns numeric
    numeric sum = 0
    for i = 1 to 5
        sum = sum + i
    end for
    return sum  -- Should return 15 (1+2+3+4+5)
end function
```

#### Task 2.4: Documentation (15 min)
- [ ] Update parser comments
- [ ] Document for loop syntax
- [ ] Add to test suite

**Deliverables:**
- For loops working in functions_compiler ✓
- Test cases passing ✓
- Parser documentation updated ✓

---

### Part 3: AST Integration & Error Handling (60 min)

#### Task 3.1: AST Validation (20 min)
- [ ] Verify WhileStatement structure
- [ ] Verify ForStatement structure
- [ ] Check field initialization
- [ ] Test memory management (no leaks)

**AST Structures to Verify:**
```c
typedef struct WhileStatement {
    Expression* condition;
    Statement** body;
    int body_count;
} WhileStatement;

typedef struct ForStatement {
    char* iterator;
    Expression* start;
    Expression* end;
    Statement** body;
    int body_count;
} ForStatement;
```

#### Task 3.2: Error Handling (20 min)
- [ ] Add syntax error messages
- [ ] Handle missing 'end while'
- [ ] Handle missing 'end for'
- [ ] Handle invalid conditions
- [ ] Handle missing iterator in for loop

**Error Cases to Handle:**
```mlp
-- Missing end while
while x < 10
    x = x + 1
-- ERROR: Expected 'end while'

-- Missing iterator
for = 1 to 10
    -- ...
-- ERROR: Expected iterator variable
```

#### Task 3.3: Edge Cases (20 min)
- [ ] Empty loop bodies
- [ ] Nested loops (while in while, for in for)
- [ ] Mixed nesting (for in while, while in for)
- [ ] Loops with if/else inside

**Test Cases:**
```mlp
-- Nested for loops
for i = 1 to 3
    for j = 1 to 3
        -- sum = sum + (i * j)
    end for
end for
```

**Deliverables:**
- Robust error handling ✓
- Edge cases covered ✓
- No memory leaks ✓

---

### Part 4: Comprehensive Testing (60 min)

#### Task 4.1: Create Test Suite (20 min)
- [ ] `test_while_simple.mlp` - Basic while loop
- [ ] `test_while_nested.mlp` - Nested while
- [ ] `test_for_simple.mlp` - Basic for loop
- [ ] `test_for_nested.mlp` - Nested for
- [ ] `test_loop_mixed.mlp` - While + for + if/else
- [ ] `test_loop_empty.mlp` - Empty loop bodies

#### Task 4.2: Run Test Suite (20 min)
- [ ] Compile all tests with LLVM backend
- [ ] Verify LLVM IR output
- [ ] Execute all binaries
- [ ] Check exit codes
- [ ] Document results

**Test Automation:**
```bash
#!/bin/bash
# test_parser_loops.sh

for test in test_while_*.mlp test_for_*.mlp; do
    echo "Testing: $test"
    ./functions_compiler -c --backend=llvm "$test" "${test%.mlp}.ll"
    clang "${test%.mlp}.ll" -o "${test%.mlp}"
    ./"${test%.mlp}"
    echo "Exit code: $?"
done
```

#### Task 4.3: Regression Testing (20 min)
- [ ] Re-run all Phase 13.5 tests (8 tests)
- [ ] Verify no regressions
- [ ] Check assembly backend still works
- [ ] Document any breaking changes

**Expected Results:**
- All Phase 13.5 tests still pass ✓
- New loop tests pass ✓
- Zero regressions ✓

**Deliverables:**
- 6+ new test cases ✓
- Automated test script ✓
- Regression test results ✓

---

### Part 5: Documentation & Finalization (30 min)

#### Task 5.1: Update Documentation (15 min)
- [ ] Update `ARCHITECTURE.md` if needed
- [ ] Update `README.md` examples
- [ ] Update `NEXT_AI_START_HERE.md`
- [ ] Create `YZ_60.md` session report

#### Task 5.2: Git Workflow (15 min)
- [ ] Commit all changes with clear message
- [ ] Create backup branch: `phase14_parser_complete_YZ60_20251213`
- [ ] Push to GitHub
- [ ] Update TODO.md marking Phase 14 complete

**Deliverables:**
- Complete documentation ✓
- Clean git history ✓
- YZ_60 session report ✓

---

## 🔄 Success Criteria

### Phase 14 Definition of Done
- [ ] While loops parse correctly
- [ ] For loops parse correctly
- [ ] LLVM IR generation works for loops
- [ ] All new tests pass (6+)
- [ ] Zero regressions (Phase 13.5 tests still pass)
- [ ] Documentation complete
- [ ] Git backup created and pushed

---

## 📊 Time Estimates

| Part | Task | Estimated Time |
|------|------|----------------|
| Part 1 | While Loop Parser | 90 min |
| Part 2 | For Loop Parser | 90 min |
| Part 3 | AST & Error Handling | 60 min |
| Part 4 | Comprehensive Testing | 60 min |
| Part 5 | Documentation | 30 min |
| **Total** | | **5.5 hours** |

**Buffer:** 30 min for unexpected issues  
**Total Estimate:** 6 hours

---

## 🎯 Next Phase Preview

### Phase 15: Standard Library Integration (Optional)
- Replace printf with `mlp_println_numeric()`
- Link with libmlp_stdlib.a
- Handle STO type tags
- **Estimated:** 1-2 hours

### Phase 16: Advanced LLVM Features (Optional)
- Optimization passes
- Debug information
- Error messages in IR
- **Estimated:** 3-4 hours

### Phase 17: Self-Hosting Parser (Major)
- Parser written in MELP
- Generates LLVM IR
- **Estimated:** 15-20 hours

---

## 📝 Notes & Decisions

### Parser Design Decisions
- Follow existing if/else parsing pattern
- Use control_flow module for AST structures
- Keep parser stateless (no global state)
- Maintain backward compatibility

### Testing Strategy
- Unit tests for parser functions
- Integration tests with LLVM backend
- Regression tests for existing features
- Edge case coverage

---

## 🔗 Related Documents

- **Previous TODO:** `temp/TODO_phase13.5_complete_20251213.md`
- **Architecture:** `ARCHITECTURE.md`
- **YZ Reports:** `YZ/YZ_57.md`, `YZ/YZ_58.md`, `YZ/YZ_59.md`
- **Next Session:** `NEXT_AI_START_HERE.md`
- **LLVM Guide:** `docs/LLVM_IR_GUIDE.md`

---

**Last Updated:** 13 Aralık 2025  
**Created By:** YZ_59 (Phase 13.5 completion)  
**Next Session:** YZ_60 (Phase 14 start)  
**Target Completion:** 13 Aralık 2025 (same day, ~6 hours)
