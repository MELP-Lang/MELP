# YZ_29: Stage 0 Const Support - Investigation Phase

**Date:** 17 Aralık 2025  
**Phase:** Investigation (2-3 hours)  
**Goal:** Understand Stage 0 architecture before implementing

---

## 🎯 Investigation Goals

### 1. Code Structure Analysis (30 min)
- [ ] Read `compiler/stage0/modules/functions/functions_parser.c`
- [ ] Read `compiler/stage0/modules/functions/functions_lexer.c`
- [ ] Read `compiler/stage0/modules/functions/functions_codegen_llvm.c`
- [ ] Understand current architecture

### 2. Const Implementation Design (45 min)
- [ ] Where to add `const` keyword token?
- [ ] How to parse `const numeric X = 42`?
- [ ] Symbol table structure?
- [ ] Inline replacement strategy?

### 3. Risk Analysis (30 min)
- [ ] What can break?
- [ ] How to test incrementally?
- [ ] Rollback strategy?
- [ ] Stage 0 regression tests?

### 4. Minimal Scope Definition (15 min)
- [ ] Phase 1: What to implement first?
- [ ] Phase 2: What to defer?
- [ ] Success criteria?
- [ ] Time estimate (realistic)?

---

## 📋 Investigation Checklist

### Code Discovery

**Files to Read:**
```bash
compiler/stage0/modules/functions/
├── functions_lexer.c          # Token definitions
├── functions_parser.c         # Parse logic
├── functions_codegen_llvm.c   # Code generation
├── functions_standalone.c     # Main entry
└── test_*.mlp                 # Test files
```

**Questions to Answer:**
1. How many lines is functions_parser.c?
2. Is there a symbol table already?
3. How are global variables handled (if any)?
4. What's the lexer token structure?
5. How does codegen handle identifiers?

### Design Questions

**Syntax:**
- `const numeric X = 42` vs `numeric const X = 42`?
- Enforce initialization? (`const numeric X` invalid?)
- Allow expressions? (`const X = 10 + 20`)

**Implementation:**
- Build-time symbol table? (hash map?)
- Inline at parse time or codegen time?
- Error messages for reassignment?

**Testing:**
- Minimal test case (1 const, 1 usage)?
- Parser constants test (50+ consts)?
- Error handling tests?

---

## 🚦 Decision Tree

After investigation, choose path:

### Path A: Simple Implementation (5-6h)

**IF:**
- ✅ Symbol table exists or easy to add
- ✅ Inline replacement straightforward
- ✅ Test infrastructure clear

**THEN:**
```
YZ_29_Phase1: Minimal const implementation
- Numeric only
- Literals only (no expressions)
- Inline at codegen
- Test: parser.mlp compiles
```

### Path B: Complex Implementation (10-15h)

**IF:**
- ❌ No symbol table (need to build)
- ❌ Complex codegen changes
- ❌ Unclear test strategy

**THEN:**
```
Break into smaller phases:
- YZ_29_Phase1: Symbol table (2-3h)
- YZ_29_Phase2: Parser (2-3h)
- YZ_29_Phase3: Codegen (3-4h)
- YZ_29_Phase4: Testing (2-3h)
```

### Path C: Too Risky (fallback)

**IF:**
- ❌ Stage 0 architecture unclear
- ❌ No regression tests
- ❌ Risk too high

**THEN:**
```
Fallback to Option 1: Magic numbers
- parser.mlp: constants → literals
- YZ_28: Continue refactor
- Revisit const later
```

---

## 📊 Investigation Template

### File: functions_parser.c

**Lines:** ??? (find with `wc -l`)

**Structure:**
```
- Token definitions: ???
- Parse functions: ???
- Symbol table: EXISTS? YES/NO
- Global handling: HOW?
```

**Const Implementation:**
```
Add parsing logic at line ???:
- Detect `const` keyword
- Parse type + name + value
- Store in symbol table
- Validate usage
```

**Estimated effort:** ???

---

### File: functions_codegen_llvm.c

**Lines:** ??? 

**Structure:**
```
- Identifier lookup: HOW?
- Inline replacement: WHERE?
- LLVM IR generation: PATTERN?
```

**Const Implementation:**
```
Replace identifier with literal at line ???:
- Check if identifier is const
- Get value from symbol table
- Generate LLVM literal
- No global variable
```

**Estimated effort:** ???

---

### File: functions_lexer.c

**Lines:** ???

**Structure:**
```
- Token enum: WHERE?
- Keyword matching: HOW?
- Add "const" keyword: LINE?
```

**Const Implementation:**
```
Add at line ???:
- TOKEN_CONST to enum
- "const" to keyword table
```

**Estimated effort:** 30 min

---

## 🧪 Minimal Test Plan

### Test 1: Single Const (10 min)

```mlp
const numeric X = 42

function main() returns numeric
    return X
end function
```

**Expected:**
- ✅ Compiles without error
- ✅ Output: `ret i64 42`
- ✅ No global variable in IR

### Test 2: Multiple Consts (10 min)

```mlp
const numeric A = 1
const numeric B = 2
const numeric C = 3

function test() returns numeric
    if A == 1 then
        return B
    end if
    return C
end function
```

**Expected:**
- ✅ All consts inlined
- ✅ Output: `icmp eq i64 1, 1` etc.

### Test 3: Parser Consts (15 min)

```mlp
-- Copy first 10 token constants from parser.mlp
const numeric T_FUNCTION = 1
const numeric T_END = 2
...

function check_token(numeric tok) returns numeric
    if tok == T_FUNCTION then
        return 1
    end if
    return 0
end function
```

**Expected:**
- ✅ Compiles
- ✅ Constants inlined

### Test 4: Error Cases (15 min)

```mlp
const numeric X          -- ❌ Error: not initialized
const numeric Y = Z      -- ❌ Error: Z undefined
const numeric A = 1
A = 2                    -- ❌ Error: cannot reassign const
```

**Expected:**
- ✅ Parser errors detected
- ✅ Clear error messages

---

## 🎯 Success Criteria

**Investigation Complete When:**
- [ ] All 3 files read and understood
- [ ] Design document created
- [ ] Risk analysis complete
- [ ] Time estimate realistic (within 20% accuracy)
- [ ] Decision made: Path A, B, or C

**Time Budget:**
- Code reading: 30 min
- Design thinking: 45 min
- Risk analysis: 30 min
- Documentation: 15 min
- **Total: 2 hours**

---

## 📝 Next Steps

**After Investigation:**

1. **Create design doc:** `YZ_refactor/YZ_29_design.md`
2. **Update TODO:** Add phases to `YZ_refactor/TODO.md`
3. **Decide path:** A, B, or C
4. **Start implementation:** YZ_29_Phase1 or fallback

---

## 🔄 Rollback Plan

**If investigation shows too complex:**

1. Git stash current work
2. Fallback to Option 1 (magic numbers)
3. Document decision in `temp/YZ_29_decision.md`
4. Continue YZ_28 refactor
5. Revisit const support in Stage 1 (not Stage 0)

---

**Investigation Start:** Now  
**Investigation End:** ~2 hours  
**Status:** NOT STARTED
