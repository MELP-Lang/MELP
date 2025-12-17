# YZ_29 Investigation - Session 1 Findings

**Date:** 17 Aralık 2025  
**Duration:** 30 minutes  
**Status:** 🎯 ROOT CAUSE FOUND

---

## 🔍 Discovery: Stage 0 Architecture

### File Sizes
```
functions_parser.c:        379 lines
functions_codegen_llvm.c:  828 lines
functions_standalone.c:    951 lines (main loop)
```

### MELP Philosophy: ✅ VERIFIED

```
✅ Stateless    - variable_parser.c: "STATELESS PATTERN: No parser struct"
✅ LLVM         - llvm_backend/ module exists
✅ STO          - runtime_sto/ module exists
✅ Modular      - 29 separate modules in compiler/stage0/modules/
✅ Struct+Func  - No classes, only structs and functions
```

**Mimari KORUNMUŞ!** ✅

---

## 🎯 ROOT CAUSE: Top-Level Parser Logic

### functions_standalone.c (Line 219):

```c
// YZ_40: FIRST PASS - Register all function names
while (1) {
    Token* tok = lexer_next_token(lexer);
    
    if (tok->type == TOKEN_FUNCTION) {
        // Register function
    }
    
    // ❌ NO OTHER TOP-LEVEL TOKENS HANDLED
}
```

### functions_parser.c (Line 42):

```c
FunctionDeclaration* parse_function_declaration(Lexer* lexer) {
    Token* tok = lexer_next_token(lexer);
    
    if (tok->type != TOKEN_FUNCTION) {
        error_parser(tok->line, "Expected 'function' keyword");
        return NULL;  // ❌ REJECT
    }
}
```

**Top-level sadece `TOKEN_FUNCTION` kabul ediyor!**

---

## 💡 Discovery: Existing Features

### 1. Constant Folding ✅ (YZ_32)

**Module:** `compiler/stage0/modules/arithmetic/arithmetic_optimize.c`

```c
// Constant folding optimization
ArithmeticExpr* arithmetic_optimize_constant_fold(ArithmeticExpr* expr) {
    // ✅ Compile-time evaluation
    // 2 + 3 → 5
    // 10 * 5 → 50
}
```

**Usage:** Expressions içinde (not top-level)

### 2. Variable Module Has `is_constant` ✅

**File:** `compiler/stage0/modules/variable/variable_standalone.c:98`

```c
if (decl->sto_info->is_constant) {
    printf("    [STO] Constant: YES\n");
}
```

**Context:** STO (String-Table Optimization) için kullanılıyor

### 3. Modular Architecture ✅

**29 Modules:**
- arithmetic/ - Expression evaluation
- variable/ - Variable declarations
- functions/ - Function parsing
- optimization_pass/ - Optimizations
- lexer/ - Tokenization
- **NO global/ or const/ module!** ❌

---

## 🚧 What's Missing?

### Top-Level Const Support ❌

**Needed:**
1. **Lexer:** `TOKEN_CONST` keyword
2. **Parser:** Parse `const numeric X = 42` at top-level
3. **Storage:** Symbol table for const values
4. **CodeGen:** Inline replacement

**Currently missing in main loop!**

---

## 💡 Solution Paths

### Path A: Add to Main Loop (MINIMAL - 3-4h)

**Modify:** `functions_standalone.c` main loop

```c
// YZ_40: FIRST PASS - Register functions AND constants
while (1) {
    Token* tok = lexer_next_token(lexer);
    
    if (tok->type == TOKEN_FUNCTION) {
        // Register function
    }
    
    // ✨ NEW: Register constants
    if (tok->type == TOKEN_CONST) {
        // Parse: const numeric X = 42
        // Store in symbol table
    }
}
```

**Changes:**
1. Lexer: Add `TOKEN_CONST` (10 min)
2. Main loop: Parse const at top-level (1h)
3. Symbol table: Hash map for consts (1h)
4. CodeGen: Inline replacement (1h)
5. Testing: parser.mlp (30 min)

**Total: 3-4 hours** ✅ REALISTIC

---

### Path B: Create const/ Module (MODULAR - 5-6h)

**Follow MELP pattern:**

```
compiler/stage0/modules/const/
├── const_parser.c/.h      - Parse const declarations
├── const_codegen.c/.h     - Inline replacement
├── const.c/.h             - Data structures
└── const_standalone.c     - Integration
```

**Pattern:**
- Stateless functions ✅
- Separate concerns ✅
- Like variable/ module

**Effort:** 5-6 hours (more modular, cleaner)

---

### Path C: Reuse variable/ Module (HYBRID - 2-3h)

**Insight:** Variable module already has:
- `is_constant` field ✅
- STO optimization ✅
- Parse logic ✅

**Modify:**
```c
// variable_parser.c
VariableDeclaration* variable_parse_declaration(Lexer* lexer, Token* type_token) {
    // ✨ NEW: Check for 'const' keyword first
    Token* const_tok = lexer_peek_token(lexer);
    int is_const = (const_tok->type == TOKEN_CONST);
    
    if (is_const) {
        lexer_next_token(lexer);  // Consume 'const'
    }
    
    // Existing parse logic...
    decl->is_constant = is_const;
}
```

**Pros:**
- ✅ Reuse existing code
- ✅ Minimal changes
- ✅ Already has constant support (STO)

**Cons:**
- ⚠️ Variable vs Const semantics different
- ⚠️ May confuse architecture

**Effort:** 2-3 hours (quickest, but less clean)

---

## 🎯 Recommendation

### CHOOSE PATH A: Minimal Main Loop (3-4h)

**Why:**
1. **Stateless maintained** ✅
2. **Modular** - New symbol table module
3. **Clean separation** - Consts ≠ Variables
4. **Test-friendly** - Incremental
5. **MELP philosophy** - Follow existing patterns

**Next Steps:**
1. Create `const_table.c/.h` (symbol table)
2. Modify main loop (parse const)
3. Modify codegen (inline replacement)
4. Test with parser.mlp

**Time:** 3-4 hours realistic estimate

---

## 📊 Risk Analysis

### Low Risk ✅
- Main loop modification: Small, localized change
- Symbol table: Independent module
- Codegen inline: Non-intrusive
- Rollback: Easy (git revert)

### Medium Risk ⚠️
- Testing: Need comprehensive test suite
- Edge cases: Error handling

### No High Risk ✅
- Architecture stays stateless
- Modular design maintained
- No global state added

---

## 🧪 Test Strategy

### Phase 1: Lexer (10 min)
```mlp
const numeric X = 42
-- Expected: TOKEN_CONST recognized
```

### Phase 2: Parser (30 min)
```mlp
const numeric X = 42
function main() returns numeric
    return 0
end function
-- Expected: Parses without error
```

### Phase 3: CodeGen (30 min)
```mlp
const numeric X = 42
function test() returns numeric
    return X
end function
-- Expected LLVM: ret i64 42 (inlined)
```

### Phase 4: Full Parser (1h)
```mlp
-- Copy all 50+ constants from parser.mlp
const numeric T_FUNCTION = 1
const numeric T_IF = 3
...
-- Expected: Compiles ✅
```

---

## ✅ Conclusion

**FEASIBLE! 3-4 hours, low risk, MELP-compliant**

**Next:** Create design document and start implementation

---

**Investigation Complete:** 17:30  
**Decision:** Path A (Minimal Main Loop)  
**Confidence:** HIGH ✅
