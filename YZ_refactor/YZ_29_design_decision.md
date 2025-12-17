# YZ_29 Design Decision: Ekosistem Perspektifi

**Date:** 17 Aralık 2025  
**Decision:** Const implementation strategy  
**Context:** 150-200 YZ, entire ecosystem dependency

---

## 🌍 Ekosistem Analizi

### Current Const Usage

**Workspace-wide scan:**
```bash
grep -r "^numeric [A-Z_].*=" modules/ compiler/ --include="*.mlp"
```

**Result:** 
- **parser.mlp ONLY:** 50+ token constants (T_FUNCTION=1, T_IF=3, etc.)
- **All other files:** Local variables only (lowercase names)

**Conclusion:** 
✅ **Const kullanımı SINIRLI** - Sadece parser token definitions
❌ **Ekosistem geneli yok** - Test files bile kullanmıyor

---

## 🎯 Token Constants'ın Rolü

### Stage 0 (C): TokenType Enum

```c
// compiler/stage0/modules/lexer/lexer.h
typedef enum {
    TOKEN_FUNCTION,   // = 0 (implicit)
    TOKEN_IF,         // = 1
    TOKEN_THEN,       // = 2
    ...
} TokenType;
```

**C compiler'da:** Enum otomatik sayılar atar (0, 1, 2, ...)

### Stage 1 (MELP): Manual Constants

```mlp
-- modules/parser_mlp/parser.mlp
numeric T_FUNCTION = 1
numeric T_IF = 3
numeric T_THEN = 4
...
```

**MELP'de:** Enum yok → Manual constant definition gerekli

---

## 💡 Insight: Enum Simulation!

**Parser constants aslında ENUM!**

```c
// C equivalent:
typedef enum {
    T_FUNCTION = 1,
    T_END = 2,
    T_IF = 3,
    ...
} ParserTokenType;
```

**MELP needs:**
```mlp
-- Ideally:
enum TokenType
    T_FUNCTION = 1
    T_END = 2
    T_IF = 3
end enum

-- Currently (workaround):
numeric T_FUNCTION = 1  -- ❌ Stage 0 rejects
```

---

## 🔍 Philosophy Alignment

### MELP Core Principles:
1. **Stateless** ✅
2. **LLVM** ✅
3. **STO** ✅
4. **Modular** ✅
5. **Struct + Functions** ✅

### Additional Design Goals:
6. **Type Safety** - Constants should be typed
7. **Compile-Time** - No runtime overhead
8. **Ecosystem-Wide** - Not just parser, future modules too
9. **Readable** - Self-documenting code

---

## 🚦 3 Solutions Re-evaluated

### ❌ Rust-Style `const` (Path A rejected)

**Syntax:**
```mlp
const numeric T_FUNCTION = 1
```

**Problems:**
- ⚠️ Not really "const" - more like "literal alias"
- ⚠️ Doesn't match MELP's type-first philosophy
- ⚠️ Keyword clutter (`const numeric` vs `numeric`)
- ⚠️ No type safety (just numeric)

**Philosophy:** ❌ Adds keyword without adding structure

---

### ✅ Main Loop + Symbol Table (Path A revised)

**Approach:** Compile-time constant folding

**Syntax:** 
```mlp
-- No new keyword!
numeric T_FUNCTION = 1  -- Top-level = compile-time constant
```

**Implementation:**
1. Main loop: Detect top-level `numeric X = <literal>`
2. Symbol table: Store name → value mapping
3. CodeGen: Inline replacement (T_FUNCTION → 1)
4. Validation: Cannot reassign top-level constants

**Philosophy Alignment:**
- ✅ **Stateless:** Symbol table is compile-time only
- ✅ **Modular:** Clean separation (const_table module)
- ✅ **LLVM:** Inline optimization, zero runtime cost
- ✅ **Simple:** No new keywords, natural syntax
- ✅ **Future-proof:** Works for any module

**Pros:**
- Natural MELP syntax (type-first)
- Top-level vs local scope distinction
- Works for entire ecosystem
- Minimal language change

**Cons:**
- Implicit behavior (top-level = const)
- Could confuse with mutable globals (if added later)

---

### 🎯 BEST: Enum Module (Path D - NEW)

**Approach:** First-class enum support

**Syntax:**
```mlp
-- NEW: enum type
enum TokenType
    T_FUNCTION = 1
    T_END = 2
    T_IF = 3
    T_THEN = 4
end enum

-- Usage:
function check_token(numeric tok_type) returns numeric
    if tok_type == TokenType.T_FUNCTION then
        return 1
    end if
    return 0
end function
```

**Implementation:**
1. **Lexer:** `TOKEN_ENUM`, `TOKEN_END_ENUM` (already exist! ✅)
2. **Parser:** Parse enum declarations (like struct)
3. **Symbol table:** Enum name → members mapping
4. **CodeGen:** Inline enum values
5. **Type safety:** Optional type checking (future)

**Philosophy Alignment:**
- ✅ **Struct + Functions:** Enums complete the trio!
- ✅ **Stateless:** Compile-time only
- ✅ **Modular:** `enum/` module (like `struct/`)
- ✅ **Type Safety:** Proper enum type
- ✅ **Readable:** Self-documenting (`TokenType.T_FUNCTION`)
- ✅ **Ecosystem:** Works everywhere (parser, codegen, etc.)

**Existing Infrastructure:**
```c
// lexer.h:95
TOKEN_ENUM,            // ✅ Already exists!
TOKEN_END_ENUM,        // ✅ Already exists!
```

**Already in Stage 0!** 🎉

---

## 📊 Decision Matrix

| Criteria | Const | Main Loop | Enum |
|----------|-------|-----------|------|
| **New Keywords** | 1 (const) | 0 | 0 (exists) |
| **Type Safety** | ⚠️ Weak | ❌ None | ✅ Strong |
| **Readability** | ⚠️ OK | ⚠️ OK | ✅ Excellent |
| **Ecosystem** | ✅ Yes | ✅ Yes | ✅ Yes |
| **Philosophy** | ⚠️ Partial | ✅ Good | ✅ Perfect |
| **Effort** | 3-4h | 3-4h | 5-6h |
| **Future-proof** | ⚠️ Limited | ✅ Good | ✅ Excellent |
| **MELP Style** | ⚠️ Rust-like | ✅ Natural | ✅ Native |

---

## 🎯 RECOMMENDATION: Enum First-Class

### Why Enum?

1. **Already planned:** `TOKEN_ENUM` exists in lexer!
2. **Philosophy:** Completes MELP's type system
   - Primitives: numeric, string, boolean
   - Collections: list, array, tuple
   - Structured: struct ✅
   - **Enumerated: enum** ← Missing piece!
3. **Type safety:** `TokenType.T_FUNCTION` > `1`
4. **Readable:** Self-documenting code
5. **Ecosystem:** Works for ALL modules
6. **Future:** Pattern matching, exhaustiveness checking

### Implementation Path

**Phase 1: Basic Enum (5-6h)**
```mlp
enum TokenType
    T_FUNCTION = 1
    T_IF = 3
end enum
```

**Phase 2: Usage (parser.mlp)**
```mlp
if tok_type == TokenType.T_FUNCTION then
    -- or just: if tok_type == T_FUNCTION then
end if
```

**Phase 3: Type Safety (future)**
```mlp
function check_token(TokenType tok) returns numeric
    -- Type-safe parameter
end function
```

---

## 🚀 Alternative: Hybrid Approach

**SHORT-TERM (1h):** Magic numbers workaround
```mlp
-- parser.mlp
-- Token constants (inline for Stage 0 compatibility)
-- T_FUNCTION = 1, T_IF = 3, T_THEN = 4

if tok_type == 1 then  -- T_FUNCTION
    ...
end if
```

**LONG-TERM (5-6h):** Enum implementation
```mlp
enum TokenType
    T_FUNCTION = 1
    T_IF = 3
end enum
```

**Reason:** Enum is strategic investment for entire ecosystem!

---

## ✅ Final Decision

### RECOMMEND: Enum Module (Path D)

**Effort:** 5-6 hours  
**Impact:** Ecosystem-wide type system improvement  
**Philosophy:** ✅ Perfect alignment  

**Alternative if time-critical:** Main Loop (3-4h), migrate to Enum later

---

## 📝 Next Steps

**If Enum chosen:**
1. Read existing `TOKEN_ENUM` implementation
2. Check if enum parsing already exists (YZ search)
3. Design enum symbol table
4. Implement enum codegen
5. Test with parser.mlp

**Investigation continues...**

---

**Decision Time:** 17:45  
**Status:** RECOMMENDATION READY
