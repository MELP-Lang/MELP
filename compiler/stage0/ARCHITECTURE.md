# MLP COMPILER ARCHITECTURE

## 🚨 CRITICAL RULE: File Creation Lock

### ABSOLUTE PROHIBITION

**AI Agents and Developers CANNOT create new files in:**
- ❌ `compiler/stage0/` (root directory, except tests)
- ❌ Any directory outside `modules/`

**ONLY ALLOWED:**
- ✅ Create files in `modules/[module_name]/`
- ✅ Create test files: `test_*.c`, `test_*.mlp`, `test_*.s`

### File Creation Matrix

| Location | New Files | Modification | Tests |
|----------|-----------|--------------|-------|
| `main.c` | ❌ FORBIDDEN | ⚠️ Avoid | N/A |
| `orchestrator.c` | ❌ FORBIDDEN | ⚠️ Avoid | N/A |
| `helpers.c` | ❌ FORBIDDEN | ⚠️ Avoid | N/A |
| `compiler/stage0/*.c` | ❌ FORBIDDEN | ⚠️ Avoid | ✅ test_*.c only |
| `modules/print/` | ✅ ALLOWED | ✅ ALLOWED | ✅ ALLOWED |
| `modules/control_flow/` | ✅ ALLOWED | ✅ ALLOWED | ✅ ALLOWED |
| `/` (root) | ❌ FORBIDDEN | ⚠️ README only | ❌ FORBIDDEN |

### ✅ CORRECT Behavior

```bash
User: "Add numeric support to print module"

AI thinks:
- "New feature for print module"
- "Must go in modules/print/"
- "Cannot create files in compiler/stage0/"

AI does:
cd modules/print/
vim print_numeric.c      # ✅ CORRECT
vim print_numeric.h      # ✅ CORRECT
make test                # ✅ CORRECT
```

### ❌ FORBIDDEN Behavior

```bash
User: "Add numeric support to print module"

AI does:
cd compiler/stage0/
vim numeric_utils.c      # ❌ FORBIDDEN!
vim helper.c             # ❌ FORBIDDEN!
vim utils.c              # ❌ FORBIDDEN!

# VIOLATION! Commit will be REJECTED!
```

### 🔒 Enforcement

**Pre-commit Hook:**
```bash
# Checks for new files in forbidden directories
# Rejects commit if rules violated
```

**Makefile Check:**
```makefile
check-forbidden-files:
	# Scans for new files outside modules/
	# Fails build if found
```

### 📋 Allowed Exceptions (Rare!)

**ONLY these files allowed in compiler/stage0/:**
1. `test_*.c` - Test files
2. `test_*.mlp` - Test programs  
3. `test_*.s` - Test assembly
4. `README.md` - Documentation
5. Existing files: `main.c`, `orchestrator.c`, `helpers.c`, `lexer.c`

**Everything else MUST go in `modules/`!**

### 🎯 Why This Rule?

**Problem without rule:**
```
compiler/stage0/
├── main.c
├── utils.c          ← AI creates this ❌
├── helpers2.c       ← AI creates this ❌
├── string_ops.c     ← AI creates this ❌
├── parser_utils.c   ← AI creates this ❌
└── [CHAOS] ❌
```

**With this rule:**
```
compiler/stage0/
├── main.c (32 lines, frozen)
├── orchestrator.c (190 lines, frozen)
└── [CLEAN] ✅

modules/
├── print/
│   ├── print_parser.c   ← All print logic HERE ✅
│   └── print_codegen.c
├── control_flow/
│   ├── while_parser.c   ← All while logic HERE ✅
│   └── if_parser.c
└── [ORGANIZED] ✅
```

### 🤖 AI Agent Instructions

**Before creating ANY file, ask:**

1. ❓ "Is this file going in `modules/[module_name]/`?"
   - YES → ✅ Proceed
   - NO → ❌ STOP!

2. ❓ "Is this a test file?"
   - YES, starts with `test_` → ✅ OK in compiler/stage0/
   - NO → ❌ FORBIDDEN in compiler/stage0/!

3. ❓ "Am I creating in root or compiler/stage0/?"
   - YES → ❌ STOP IMMEDIATELY!
   - NO (in modules/) → ✅ Proceed

**If you violate this rule:**
- Pre-commit hook will REJECT
- Build will FAIL
- Changes will be DELETED

### 🚨 Violation Examples

❌ **FORBIDDEN:**
```bash
# Creating utility in compiler/stage0/
touch compiler/stage0/string_utils.c

# Creating helper in root
touch helper.c

# Creating shared code outside modules
touch shared/common.c

# Creating router/orchestrator variant
touch compiler/stage0/router.c
touch compiler/stage0/dispatcher.c
```

✅ **CORRECT:**
```bash
# Creating utility in module
touch modules/string/string_utils.c

# Creating test
touch compiler/stage0/test_string_module.c

# Creating in appropriate module
touch modules/control_flow/while_helpers.c
```

### 💡 What If I Need Shared Code?

**Question:** "print and control_flow both need same utility"

**Wrong answer:** "Create compiler/stage0/utils.c" ❌

**Correct answers:** 
1. Create `modules/common/` for truly shared code
2. Or duplicate in both modules (OK for small code!)
3. Or use header-only utilities

```bash
# Option 1: Common module
modules/common/
└── common_utils.c

# Option 2: Duplicate (OK!)
modules/print/utils.c
modules/control_flow/utils.c

# Option 3: Header-only
modules/common/utils.h (inline functions)
```

### 🔐 Summary

> **Golden Rule: If it's not a test, and it's not in `modules/`, you CANNOT create it!**

**Allowed:**
- ✅ `modules/[any_module]/*`
- ✅ `compiler/stage0/test_*.c`
- ✅ `compiler/stage0/test_*.mlp`
- ✅ `compiler/stage0/test_*.s`

**Forbidden:**
- ❌ `compiler/stage0/[anything_else].c`
- ❌ Root directory `[anything]`
- ❌ Any other directory

**Penalty:** Immediate rejection, no exceptions!

---

## 📏 File Size Limits

### Existing Files (FROZEN)

| File | Current | Max | Status |
|------|---------|-----|--------|
| `main.c` | 32 lines | 50 lines | ✅ FROZEN |
| `orchestrator.c` | 190 lines | 200 lines | ⚠️ FROZEN |
| `helpers.c` | 72 lines | 100 lines | ✅ FROZEN |

**FROZEN means:**
- Can be modified (bug fixes)
- CANNOT grow beyond limit
- If limit reached → Extract to module

### Module Files (FLEXIBLE)

- No hard limit
- Recommended: ≤500 lines per file
- Split into multiple files if needed

---

## 🔒 Enforcement Mechanisms

1. **Pre-commit Hook** - Blocks forbidden file creation
2. **Makefile Check** - `make check-forbidden-files`
3. **Architecture Validation** - `make check-architecture`
4. **Manual Review** - PR reviewers enforce rules

---

**Last Updated:** 7 Aralık 2025  
**Enforcement:** ACTIVE (pre-commit hook + Makefile)  
**Zero Tolerance:** No exceptions!
