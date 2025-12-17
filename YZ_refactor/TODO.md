# Parser Refactor TODO

**Project:** YZ_refactor - Stateless Parser  
**Updated:** 17 Aralık 2025, 16:30

---

## 📋 Task Breakdown

### Phase 1: Foundation ✅ COMPLETE
- [x] **Design parser state structure** (5 min)
  - State format: `[tokens, position, error_count]`
  - Return format: `[result, new_state]`
  
- [x] **Create state management functions** (20 min)
  - [x] `create_parser_state(tokens)` - Initialize state
  - [x] `state_current_token(state)` - Get current token
  - [x] `state_peek_token(state)` - Peek next token
  - [x] `state_advance(state)` - Advance position
  - [x] `state_check_token(state, type)` - Check token type
  - [x] `state_expect_token(state, type)` - Expect and advance

- [x] **Refactor error handling** (15 min)
  - [x] `state_parser_error(state, msg)` - Report error
  - [x] `state_unexpected_token_error(state, exp, got)` - Report unexpected token
  - [x] `state_get_error_count(state)` - Get error count

**Total Phase 1:** ~40 minutes ✅

---

### Phase 2: Expression Parsing ⏳ IN PROGRESS
- [x] **parse_primary** (25 min) ✅
  - [x] Refactor to `state_parse_primary(state)`
  - [x] Handle all literal types (number, string, identifier)
  - [x] Handle parenthesized expressions
  - [x] Test: literals, identifiers, parens

- [ ] **parse_expression** (15 min)
  - [ ] Refactor to `state_parse_expression(state)`
  - [ ] Entry point to precedence climbing
  - [ ] Test: simple expressions

- [ ] **parse_expression_prec** (30 min)
  - [ ] Refactor to `state_parse_expression_prec(state, min_prec)`
  - [ ] Precedence climbing algorithm
  - [ ] Binary operators with correct precedence
  - [ ] Test: `2 + 3 * 4` → correct precedence

**Total Phase 2:** ~70 minutes (25 min done, 45 min remaining)

---

### Phase 3: Statement Parsing
- [ ] **parse_var_decl** (20 min)
  - [ ] Refactor to `state_parse_var_decl(state)`
  - [ ] Handle: `numeric x = 10`
  - [ ] Test: variable declarations

- [ ] **parse_assignment** (15 min)
  - [ ] Refactor to `state_parse_assignment(state, var_name)`
  - [ ] Handle: `x = 20`
  - [ ] Test: assignments

- [ ] **parse_return** (15 min)
  - [ ] Refactor to `state_parse_return(state)`
  - [ ] Handle: `return expr`
  - [ ] Test: return statements

- [ ] **parse_print** (15 min)
  - [ ] Refactor to `state_parse_print(state, is_println)`
  - [ ] Handle: `print(x)`, `println(x)`
  - [ ] Test: print statements

- [ ] **parse_statement** (30 min)
  - [ ] Refactor to `state_parse_statement(state)`
  - [ ] Dispatch to correct statement parser
  - [ ] Test: mixed statements

**Total Phase 3:** ~95 minutes

---

### Phase 4: Control Flow Parsing
- [ ] **parse_if_statement** (40 min)
  - [ ] Refactor to `state_parse_if_statement(state)`
  - [ ] Handle: if-then-else-end
  - [ ] Nested statement parsing
  - [ ] Test: if statements, nested ifs

- [ ] **parse_while_loop** (30 min)
  - [ ] Refactor to `state_parse_while_loop(state)`
  - [ ] Handle: while-do-end
  - [ ] Loop body parsing
  - [ ] Test: while loops

- [ ] **parse_for_loop** (30 min) [if exists]
  - [ ] Refactor to `state_parse_for_loop(state)`
  - [ ] Handle: for loops
  - [ ] Test: for loops

**Total Phase 4:** ~100 minutes

---

### Phase 5: Advanced Parsing
- [ ] **parse_function** (50 min)
  - [ ] Refactor to `state_parse_function(state)`
  - [ ] Handle: function declarations
  - [ ] Parameters, return type, body
  - [ ] Test: function definitions

- [ ] **parse_struct** (35 min)
  - [ ] Refactor to `state_parse_struct(state)`
  - [ ] Handle: struct definitions
  - [ ] Field declarations
  - [ ] Test: struct definitions

- [ ] **parse_enum** (30 min)
  - [ ] Refactor to `state_parse_enum(state)`
  - [ ] Handle: enum definitions
  - [ ] Enum values
  - [ ] Test: enum definitions

**Total Phase 5:** ~115 minutes

---

### Phase 6: Main Orchestrator
- [ ] **parse_program** (40 min)
  - [ ] Refactor to `state_parse_program(state)`
  - [ ] Top-level declarations
  - [ ] Multiple functions/structs/enums
  - [ ] Test: complete programs

- [ ] **main() function** (15 min)
  - [ ] Update to use new stateless API
  - [ ] Create initial state
  - [ ] Call state_parse_program
  - [ ] Extract results
  - [ ] Test: end-to-end parsing

**Total Phase 6:** ~55 minutes

---

### Phase 7: Testing & Validation
- [ ] **Unit tests** (60 min)
  - [ ] Test each refactored function individually
  - [ ] Edge cases (EOF, errors, malformed input)
  - [ ] State consistency checks

- [ ] **Integration tests** (45 min)
  - [ ] Run YZ_01 test suite (8 tests)
  - [ ] All tests must pass
  - [ ] Fix any regressions

- [ ] **Compilation test** (30 min)
  - [ ] Compile parser.mlp with Stage 0
  - [ ] Verify no global variable errors
  - [ ] Binary runs successfully

- [ ] **Concat test** (30 min)
  - [ ] Run concat script
  - [ ] Compile monolithic file
  - [ ] Verify no Stage 0 errors

- [ ] **Self-hosting test** (45 min)
  - [ ] Stage 1 compiles test program
  - [ ] Stage 1 compiles itself
  - [ ] Verify output correctness

**Total Phase 7:** ~210 minutes (3.5 hours)

---

## ⏱️ Time Estimates

| Phase | Tasks | Estimated Time | Status |
|-------|-------|----------------|--------|
| 1. Foundation | 13 functions | 40 min | ✅ COMPLETE |
| 2. Expression | 3 functions | 70 min | ⏳ 25 min done |
| 3. Statement | 5 functions | 95 min | ⏳ Pending |
| 4. Control Flow | 3 functions | 100 min | ⏳ Pending |
| 5. Advanced | 3 functions | 115 min | ⏳ Pending |
| 6. Orchestrator | 2 functions | 55 min | ⏳ Pending |
| 7. Testing | 5 test suites | 210 min | ⏳ Pending |
| **TOTAL** | **31 functions** | **~685 min** | **11.4 hours** |

**Progress:** 14/31 functions (45%) | ~65 min / 685 min (9.5%)

---

## 🎯 Next Steps

**Current:** Phase 2 - Expression Parsing  
**Next Task:** `state_parse_expression()` - 15 min  
**After That:** `state_parse_expression_prec()` - 30 min

**Session Plan:**
1. Finish Phase 2 (45 min remaining)
2. Phase 3: Statement parsing (95 min)
3. Checkpoint: Compile & test
4. Continue with Phase 4-6
5. Final testing & validation

---

## 🚀 Quick Start (Resume Work)

```bash
cd /home/pardus/projeler/MLP/MLP
code modules/parser_mlp/parser.mlp

# Find next function to refactor:
grep -n "^function parse_" modules/parser_mlp/parser.mlp

# Test after each function:
./compiler/stage0/modules/functions/functions_standalone \
    modules/parser_mlp/parser.mlp temp/parser_test
```

---

## 📌 Notes

- **Preserve YZ_01 tests:** 8/8 must pass after refactor
- **Backup before changes:** Already done (git commit)
- **Incremental approach:** Test after each 2-3 functions
- **No shortcuts:** Every function must be stateless
- **Documentation:** Update parser_api.mlp after completion

---

**Last Updated:** 17 Aralık 2025, 16:30  
**Next Session:** YZ_28_session2 (Expression parsing completion)
