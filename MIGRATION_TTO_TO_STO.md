# Migration Guide: TTO → STO

**Date:** 12 Aralık 2025  
**Phase:** Phase 12 (YZ_49 through YZ_53)  
**Status:** ✅ Complete

## 📋 Overview

This guide documents the complete migration from **TTO** (Transparent Type Optimization) to **STO** (Smart Type Optimization) across the MELP codebase.

**Reason for Change:**
- **TTO** was the original name but implied "transparent" behavior
- **STO** better reflects the "smart" decision-making in type optimization
- **Consistency:** Aligns with project terminology and documentation
- **Clarity:** "Smart" is more descriptive of the optimization strategy

## 🎯 Scope of Changes

### Statistics
- **Total Files Modified:** 65+
- **Total Renames:** 453 TTO→STO references
- **Duration:** ~6 hours (across 5 YZ sessions)
- **Regression Rate:** 0% (9/9 tests passing)

### Areas Affected
1. ✅ Documentation (README, docs, YZ history)
2. ✅ Runtime library (directory, files, functions)
3. ✅ Compiler modules (all 26 modules updated)
4. ✅ Build system (Makefiles, linker flags)
5. ✅ Test suite (validation scripts)

## 📦 Directory Structure Changes

### Before (TTO)
```
runtime/
├── tto/                     # Old name
│   ├── tto_runtime.h
│   ├── tto_runtime.c
│   ├── test_tto_runtime.c
│   └── Makefile
└── stdlib/
    └── tto_bigdec_to_string.c
```

### After (STO)
```
runtime/
├── sto/                     # New name
│   ├── sto_runtime.h
│   ├── sto_runtime.c
│   ├── test_sto_runtime.c
│   └── Makefile
└── stdlib/
    └── sto_bigdec_to_string.c
```

## 🔧 Function Name Changes

### Runtime Functions (40+ functions renamed)

| Old Name (TTO) | New Name (STO) | Purpose |
|----------------|----------------|---------|
| `TTOValue` | `STOValue` | Core value type |
| `TTOTypeInfo` | `STOTypeInfo` | Type information struct |
| `tto_create_int` | `sto_create_int` | Create integer value |
| `tto_create_double` | `sto_create_double` | Create double value |
| `tto_create_bigdec` | `sto_create_bigdec` | Create BigDecimal |
| `tto_add` | `sto_add` | Addition operation |
| `tto_sub` | `sto_sub` | Subtraction operation |
| `tto_mul` | `sto_mul` | Multiplication operation |
| `tto_div` | `sto_div` | Division operation |
| `tto_would_overflow_add` | `sto_would_overflow_add` | Overflow check |
| `tto_safe_add_i64` | `sto_safe_add_i64` | Safe addition |
| `tto_to_int64` | `sto_to_int64` | Convert to int64 |
| `tto_to_double` | `sto_to_double` | Convert to double |
| `tto_free` | `sto_free` | Free STO value |
| `tto_bigdec_to_string` | `sto_bigdec_to_string` | BigDecimal to string |

### Compiler Functions (15+ functions)

| Old Name (TTO) | New Name (STO) | Location |
|----------------|----------------|----------|
| `tto_should_use_double` | `sto_should_use_double` | codegen_context.c |
| `tto_detect_type` | `sto_detect_type` | arithmetic_optimize.c |
| `tto_analyze_expression` | `sto_analyze_expression` | Various modules |

## 📝 Build System Changes

### Makefile Updates

**Before:**
```makefile
RUNTIME_LIB = -L../../../../runtime/tto -ltto_runtime
```

**After:**
```makefile
RUNTIME_LIB = -L../../../../runtime/sto -lsto_runtime
```

### Include Path Changes

**Before:**
```c
#include "../../../../runtime/tto/tto_runtime.h"
```

**After:**
```c
#include "../../../../runtime/sto/sto_runtime.h"
```

## 🧪 Testing Changes

### Test Script Updates

**Before:**
```bash
LD_LIBRARY_PATH=../../runtime/tto:../../runtime/stdlib ./test
```

**After:**
```bash
LD_LIBRARY_PATH=../../runtime/sto:../../runtime/stdlib ./test
```

### Library Linking

**Before:**
```bash
gcc -o program program.o -L../../runtime/tto -ltto_runtime -lm
```

**After:**
```bash
gcc -o program program.o -L../../runtime/sto -lsto_runtime -lm
```

## 📚 Documentation Updates

### Files Updated
- ✅ `README.md` - Main project README
- ✅ `runtime/README.md` - Runtime library docs
- ✅ `runtime/sto/README.md` - STO-specific docs
- ✅ `docs_tr/README.md` - Turkish documentation
- ✅ `ARCHITECTURE.md` - Architecture guide
- ✅ `TODO.md` - Task tracking
- ✅ `NEXT_AI_START_HERE.md` - AI handoff guide

### Language Documentation
- `docs/language/TTO.md` → `docs/language/STO.md`
- `docs_tr/language/TTO.md` → `docs_tr/language/STO.md`

## ✅ Verification Steps

### 1. Build Verification
```bash
# Clean build
cd compiler/stage0
make clean && make all

# Should compile without errors
```

### 2. Runtime Tests
```bash
cd runtime/sto
make test
./test_sto_runtime

# All tests should pass
```

### 3. Integration Tests
```bash
cd compiler/stage0
./test_sto.sh

# Expected: 9/9 tests passing
```

### 4. Grep Verification
```bash
# Check for remaining TTO references (should only find in YZ history)
grep -r "tto_" --include="*.c" --include="*.h" compiler/stage0/ runtime/

# Should return no results (or only comments)
```

## 🚀 Migration Timeline

| Session | Date | Task | Duration | Status |
|---------|------|------|----------|--------|
| YZ_49 | 12 Dec 2025 | Documentation update | 1h | ✅ Complete |
| YZ_50 | 12 Dec 2025 | Runtime library rename | 2-3h | ✅ Complete |
| YZ_51 | 12 Dec 2025 | Compiler code update | 1-2h | ✅ Complete |
| YZ_53 | 12 Dec 2025 | Comprehensive testing | 1.5h | ✅ Complete |
| YZ_54 | 12 Dec 2025 | Final cleanup & docs | 30min | ✅ Complete |

**Total Time:** ~6 hours  
**Result:** Zero regressions, all tests passing

## 💡 Best Practices (For Future Renames)

### 1. Use Multi-Stage Approach
- Stage 1: Documentation (safe, reversible)
- Stage 2: Runtime (isolated, testable)
- Stage 3: Compiler (thorough, validated)
- Stage 4: Testing (comprehensive)
- Stage 5: Cleanup (final polish)

### 2. Create Backups
```bash
# Before major changes
git checkout -b backup-phase12-before-sto
git push origin backup-phase12-before-sto
```

### 3. Automated Testing
- Create test suite BEFORE refactoring
- Run tests AFTER each stage
- Document expected vs actual results

### 4. Use Search & Replace Tools
```bash
# Find all occurrences
grep -rn "old_name" .

# Replace in specific file types
find . -name "*.c" -o -name "*.h" | xargs sed -i 's/old_name/new_name/g'
```

### 5. Update Documentation Incrementally
- Update README files as you go
- Keep YZ session notes detailed
- Document breaking changes clearly

## 🎉 Success Metrics

- ✅ **Build Success:** 100% (no compilation errors)
- ✅ **Test Pass Rate:** 100% (9/9 tests)
- ✅ **Regression Rate:** 0% (zero regressions)
- ✅ **Documentation:** Complete (all files updated)
- ✅ **Code Quality:** Maintained (no technical debt)

## 📖 References

- **YZ_49.md** - Documentation update details
- **YZ_50.md** - Runtime library rename details
- **YZ_51.md** - Compiler code update details
- **YZ_53.md** - Comprehensive testing report
- **TODO.md** - Phase 12 complete plan
- **ARCHITECTURE.md** - STO system architecture

## 🆘 Troubleshooting

### Issue: Linker errors after migration

**Symptom:**
```
/usr/bin/ld: cannot find -ltto_runtime
```

**Solution:**
```bash
# Update Makefile to use -lsto_runtime
sed -i 's/-ltto_runtime/-lsto_runtime/g' Makefile

# Update library path
sed -i 's/runtime\/tto/runtime\/sto/g' Makefile
```

### Issue: Header file not found

**Symptom:**
```
fatal error: tto_runtime.h: No such file or directory
```

**Solution:**
```bash
# Update include paths
sed -i 's/tto_runtime\.h/sto_runtime.h/g' *.c *.h
sed -i 's/runtime\/tto/runtime\/sto/g' *.c *.h
```

### Issue: Test failures after migration

**Symptom:**
```
Expected: 30, Got: 0
```

**Solution:**
```bash
# Verify LD_LIBRARY_PATH
export LD_LIBRARY_PATH=../../runtime/sto:../../runtime/stdlib

# Rebuild runtime
cd runtime/sto && make clean && make
```

## 🎓 Lessons Learned

1. **Plan Thoroughly:** Multi-stage approach prevented chaos
2. **Test Early:** Having test suite before refactoring saved time
3. **Document Everything:** YZ notes made tracking easy
4. **Automate When Possible:** sed/grep saved hours of manual work
5. **Keep Backups:** Git branches provided safety net

---

**Migration Complete!** 🎉

For questions or issues, refer to YZ session notes or raise an issue in the repository.
