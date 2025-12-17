# YZ_refactor - Parser Stateless Refactoring Project

**Start Date:** 17 Aralık 2025  
**Goal:** Remove global state from parser, make it Stage 0 compatible  
**Estimated Time:** 8-12 hours  
**Status:** 🚧 IN PROGRESS

---

## 🎯 Mission

Convert parser from **stateful** (global variables) to **stateless** (parameter passing) architecture.

**Why:** Stage 0 compiler doesn't support global variables → concat approach fails  
**Impact:** Enables self-hosting bootstrap (Stage 1 compiles itself)

---

## 📊 Scope

**Files to Modify:**
- `modules/parser_mlp/parser.mlp` (820 lines, 26 functions)

**NOT touching:**
- ✅ Lexer (already clean)
- ✅ CodeGen (already clean)
- ✅ Other parser modules (will use new API)

---

## 🏗️ Architecture

### Before (Stateful):
```mlp
list g_tokens           -- ❌ Global
numeric g_current_pos   -- ❌ Global
numeric g_error_count   -- ❌ Global

function parse_primary() returns string
    list tok = current_token()  -- uses g_tokens
    advance()                   -- mutates g_current_pos
    return result
end_function
```

### After (Stateless):
```mlp
-- Parser state: [tokens, position, error_count]

function state_parse_primary(list parser_state) returns list
    list tok = state_current_token(parser_state)
    list new_state = state_advance(parser_state)
    
    list result = []
    result = result + [value]      -- result[0]
    result = result + [new_state]  -- result[1]
    return result
end_function
```

---

## 📝 Progress Tracking

See: [TODO.md](./TODO.md) for detailed task list

**Summary:**
- ✅ Phase 1: Foundation (10 functions) - COMPLETE
- ⏳ Phase 2: Expression parsing (2 functions remaining)
- ⏳ Phase 3: Statement parsing (5 functions)
- ⏳ Phase 4: Control flow (3 functions)
- ⏳ Phase 5: Advanced (3 functions)
- ⏳ Phase 6: Orchestrator (2 functions)
- ⏳ Phase 7: Testing & validation

**Total:** 31 functions | 10 done (32%) | 21 remaining

---

## 📂 Session Logs

- [YZ_28_session1.md](./YZ_28_session1.md) - Foundation ✅ (30 min)
- YZ_28_session2.md - Expression parsing (TBD)
- YZ_28_session3.md - Statement parsing (TBD)

---

## 🚦 Current Status

**Last Updated:** 17 Aralık 2025, 16:35  
**Progress:** 10/31 functions (32%)  
**Next:** Phase 2 - Expression parsing

See [TODO.md](./TODO.md) for next steps.
