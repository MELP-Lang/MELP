# 🚀 NEXT AI START HERE - YZ_17 Mission Brief

**Date:** 10 Aralık 2025, 04:00  
**Previous AI:** YZ_16 (Boolean Type Complete) ✅  
**Current Status:** Phase 0-3 100% complete! Boolean type fully working!  
**Your Mission:** Boolean Operations (and/or/not) OR If-Boolean OR Comparison→Boolean

---

## ⚠️ IMPORTANT: Git Branch Instructions

**Your branch name MUST be:** `boolean_YZ_17` (or feature name + YZ number)

**Steps:**
1. Create your branch: `git checkout -b boolean_YZ_17`
2. Do your work (commit frequently)
3. Push: `git push -u origin boolean_YZ_17`

**DO NOT MERGE!** Each YZ works in their own branch. No merge needed.

**Pattern:**
- YZ_16 → `boolean_YZ_16` branch ✅
- YZ_17 → `boolean_YZ_17` branch (you)
- YZ_18 → `arrays_YZ_18` branch (next)

---

## 📖 WHAT YZ_16 DID (1.5 hours)

### 🎯 Mission: Implement Boolean Type
**Goal:** Enable `boolean` type with `true`/`false` literals.

### ✅ What Was Done:

1. **Context Gathering** (10 min)
   - Found TOKEN_BOOLEAN, TOKEN_TRUE, TOKEN_FALSE already in lexer ✅
   - Found VAR_BOOLEAN already in variable.h ✅
   - Pattern: Follow string implementation (is_string → is_boolean)

2. **Implementation** (60 min)
   - **Modified:** 4 files (~50 lines total)
   - `arithmetic.h` - Added `is_boolean` field to ArithmeticExpr
   - `arithmetic_parser.c` - Parse true/false literals, initialize is_boolean
   - `arithmetic_codegen.c` - Generate boolean literals: true=1, false=0
   - `statement_codegen.c` - Boolean variable initialization
   - **Pattern:** Boolean = int (0/1), stored as INTERNAL_TYPE_INT64
   - **Challenge:** Initialize is_boolean=0 in ~20 locations

3. **Testing** (30 min)
   - ✅ `test_boolean.mlp` - Basic: `boolean flag = true` → Exit: 1
   - ✅ `test_boolean_full.mlp` - Multiple booleans → Exit: 1
   - ✅ `test_boolean_expr.mlp` - Boolean in expression → Exit: 1

### 🎉 Boolean Status: 100% CORE COMPLETE!
- ✅ Boolean type: `boolean flag = true` works
- ✅ Boolean literals: `true` (1), `false` (0) works
- ✅ Boolean variables in functions works
- ✅ Boolean in expressions works
- ⏳ Boolean operations: `and`, `or`, `not` (tokens exist, need implementation)
- ⏳ If with boolean: `if flag` (need parser change)
- ⏳ Comparison returns boolean: `boolean x = (y > 5)` (need codegen change)

**Files Changed:** 4 files, ~50 lines added
**Time Taken:** 1.5 hours (estimated 1-2 hours - right on target!)

---

## 📖 SADECE BURAYI OKU! (5 dakika)

### 🚀 YZ_16'nın Yaptığı:
1. ✅ Boolean type tamamen çalıştı
2. ✅ true/false literals → 1/0 assembly
3. ✅ 3 test başarılı (all exit 1)
4. ✅ Phase 3 advanced types %100 tamamlandı!

### 🎯 SENİN İÇİN HAZIR:

**SADECE BU BELGEYİ OKU (10 dakika):**  
👉 **`YZ/YZ_16.md`** ← **BURASI HER ŞEYİ ANLATIYOR!**  

**İçeriği:**
- ✅ Ne yapıldı? (Boolean type implementation)
- ✅ Nasıl yapıldı? (is_boolean field, true/false parsing)
- ✅ Test sonuçları (3 tests passing)
- ✅ Ne kaldı? (Boolean operations, if-boolean, comparison-boolean)
- ✅ Hangi dosyalar değişti? (4 dosya, ~50 satır)
- ✅ Nasıl test edilir? (./test_boolean)

**BAŞKA BİR BELGEYE BAKMA!**  
YZ_16.md'de olmayan bilgi varsa, YZ_16 eksik yapmış demektir.

### 📚 İsteğe Bağlı (Sadece Detay İstersen):

**Method Öğrenmek İstersen:**  
👉 **`YZ/AI_METHODOLOGY.md`** (YZ_16 bunu kullandı, 1.5 saat sürdü)

**Mimari Kurallar:**  
👉 **`ARCHITECTURE.md`** + **`compiler/stage0/RADICAL_CHANGE.md`**

**TTO Sistemi:**  
👉 **`temp/kurallar_kitabı.md` Bölüm 4**

---

## 🎯 Your Mission: Choose One

### Priority 1: Boolean Operations (1 hour) ⭐⭐⭐ RECOMMENDED

**Goal:** Add `and`, `or`, `not` logical operators for boolean expressions.

**Expected Syntax:**
```mlp
boolean a = true
boolean b = false
boolean result = a and b  # result = false
boolean inverted = not a  # inverted = false
```

**Implementation Checklist:**
- [x] Tokens already exist: TOKEN_AND, TOKEN_OR, TOKEN_NOT ✅
- [ ] Add BooleanOp enum to `modules/logical/logical.h`
- [ ] Parse boolean operations in `logical_parser.c`
- [ ] Generate assembly: `andq`, `orq`, `xorq $1` in `logical_codegen.c`
- [ ] Test: boolean expressions with and/or/not
- [ ] Document in YZ_17.md

**Pattern:** Follow YZ_11 comparison implementation
- File: `modules/logical/logical_parser.c` (exists but minimal)
- Reference: `modules/comparison/comparison_parser.c`

**Estimated Time:** 1 hour (Easy - tokens + structure ready!)

---

### Priority 2: If-Boolean (30 min) ⭐⭐ EASY WIN
- [ ] Statement parser: Detect `arr[i] = expr` pattern (similar to variable assignment)
- [ ] Add lvalue support to expressions (distinguish read vs write)
- [ ] Codegen: Generate store instead of load (`movq %rax, offset(%rbx)`)
- [ ] Test: `arr[0] = 10` and `arr[i] = value`

**Key Insight:**
- Array indexing already works for READ
- Just need to generate STORE instruction instead of LOAD
- Pattern: `assignment_target = expression` where target is `arr[index]`

**Assembly Pattern:**
```asm
# arr[0] = 10
movq -8(%rbp), %rbx     # Load array pointer
movq $10, %rax          # Load value
movq %rax, 0(%rbx)      # Store at offset 0
```

**Estimated Time:** 2-3 hours

---

### Priority 2: Boolean Type (1-2 hours) ⭐⭐

**Goal:** Add boolean type with true/false literals.

**Expected Syntax:**
```mlp
boolean flag = true
boolean result = (x > 5 and y < 10)
if flag
    println("true")
end if
```

**Implementation Checklist:**
- [ ] Add TOKEN_TRUE, TOKEN_FALSE to lexer (already has TOKEN_BOOLEAN!)
- [ ] Update arithmetic parser for boolean literals
- [ ] Boolean codegen (store as 1/0)
- [ ] Test: boolean variables, logical operations
- [ ] Document in YZ_15.md

**Estimated Time:** 1-2 hours (Simpler than array assignment!)

---

## 📊 Current Compiler Status

### ✅ Working Features:
- Variables (numeric, text)
- Arithmetic (+, -, *, /)
- Functions (parameters, return values)
- Println (numeric + text)
- String concatenation ("Hello" + "World")
- String literals ("text")
- **Arrays (initialization)** ← YZ_13
- **Array indexing (read)** ← YZ_14 NEW!
- For loops (to/downto)
- While loops
- If-else statements

### ⏳ TODO Features:
- **Array indexing** ← YOUR MISSION (arr[i] read/write)
- **Boolean type** ← Alternative mission
- String comparison in expressions (text == text)
- Array methods (.length, etc.)

### 🏗️ Architecture:
- ✅ Modular-only (no main.c, no pipeline.c)
- ✅ Makefile enforces architecture (check-monolithic)
- ✅ TTO system working (1-bit type tracking)
- ✅ Runtime libraries (libmlp_stdlib.a, libtto_runtime.a)
- ✅ Array allocation (tto_array_alloc)

---

## 🚀 Quick Start Commands

```bash
# 1. Navigate to compiler
cd /home/pardus/projeler/MLP/MLP/compiler/stage0

# 2. Build compiler (modular-only)
make clean && make

# 3. Test arrays (verify YZ_13's work)
cd modules/functions
./functions_compiler test_array_basic.mlp test.s
gcc test.s -L../../../../runtime/stdlib -lmlp_stdlib -L../../../../runtime/tto -ltto_runtime -lm -o test
./test && echo "Exit: $?"  # Should work!

# 4. Try array indexing (will FAIL - that's your job!)
cat > test_index.mlp << 'EOF'
function main() returns numeric
    numeric[] arr = [10, 20, 30]
    numeric x = arr[0]
    return x
end function
EOF
./functions_compiler test_index.mlp test_index.s  # Parser error expected

# 5. Start your work!
# Read: modules/array/array_parser.h
# Check: modules/arithmetic/arithmetic_parser.c
# Pattern: Similar to function calls (postfix expression)
```

---

## ❓ Need Help?

**If you're stuck:**
1. Read **YZ_13.md** - Shows array initialization implementation
2. Read **AI_METHODOLOGY.md** - 5-step pattern-based approach
3. Check existing code: `grep -rn "IndexAccess" modules/`
4. Look at array_parser.h - `array_parse_index_access()` function exists!

**Remember:**
- YZ_13.md has everything about arrays
- Don't create monolithic files (ARCHITECTURE.md)
- Use patterns, don't reinvent (AI_METHODOLOGY.md)
- Test incrementally, commit often

---

**Good luck, YZ_14!** 🚀  
**Expected Duration:** 2-3 hours (array indexing) OR 1-2 hours (boolean)  
**Expected Output:** Array access working OR Boolean type complete

---

## 📝 Legacy Content (Ignore - For Historical Context Only)

<details>
<summary>Old YZ_10 Mission (Completed by YZ_10)</summary>

### Priority 1: Test String Operations (COMPLETED ✅)
- YZ_10 completed this and more
- String concatenation working
- Read YZ_10.md for details

### Priority 2: Variable Type Inference (TODO)
- Might be needed for variable concatenation
- Check YZ_10.md to see if this was done

</details>

---

### Priority 3: For Loops (1-2 hours) ⭐⭐

**If string ops are solid:**
- Parser already exists: `modules/for_loop/`
- Need: Codegen similar to while loop
- Pattern: Copy from `control_flow_codegen.c`

---

## 📚 Essential Reading Order:

1. **YZ/YZ_HISTORY.md** (10 min) ← START HERE!
2. **TODO.md** (5 min) - Your task list
3. **YZ/AI_METHODOLOGY.md** (optional, 10 min) - How to work fast

---

## 🎓 Quick Tips from YZ_09:

### Pattern-Based Development:
```bash
# Find similar code
grep -rn "similar_feature" modules/

# Copy-paste-modify approach
# Don't reinvent the wheel!
```

### Testing First:
```bash
# Always test immediately
make clean && make
./melpc test.mlp -o test
./test
```

### Document as You Go:
- Create YZ_10.md with your changes
- Update TODO.md
- Update this file for YZ_11

**⚠️ DOKÜMANTASYON KURALI:**
```bash
✅ SADECE BUNLARI YAZ:
YZ/YZ_10.md           # Senin oturum raporu
TODO.md               # Güncelle (completed işaretle)
NEXT_AI_START_HERE.md # Güncelle (YZ_11 için)
test_*.mlp            # Test programları

❌ BUNLARI YAZMA:
YZ_10_SUMMARY.md      # ❌ Gereksiz
YZ_10_NOTES.md        # ❌ Gereksiz
IMPLEMENTATION.md     # ❌ Gereksiz
```

**Özet zaten var:** `YZ/YZ_HISTORY.md` tüm bilgiyi içeriyor!

---

## 🔗 Quick Links:

**History:**
- `YZ/YZ_HISTORY.md` - Tüm YZ özeti (YZ_09'un eseri)
- `YZ/YZ_01.md` to `YZ_08.md` - Detaylı oturum raporları

**Guides:**
- `YZ/AI_METHODOLOGY.md` - 5 adımlı hızlı metod
- `ARCHITECTURE.md` - Mimari kurallar
- `temp/kurallar_kitabı.md` - TTO rehberi

**Code:**
- `compiler/stage0/modules/` - Tüm modüller
- `runtime/stdlib/` - Runtime functions
- `*.mlp` files - Test programs

---

**Good luck, YZ_10!** 🚀

**Remember:**
1. Read YZ_HISTORY.md first! (10 min investment, 2 hours saved)
2. Test early, test often
3. Document your work (YZ_10.md)
4. Update TODO.md
5. Update this file for YZ_11

**Previous Success:** YZ_07 completed string ops codegen in 1 hour (estimated 2-3)!  
**Your Goal:** Test string ops (30 min) + Start for loops (1-2 hours) = Total ~2 hours

---

**Last Updated:** 9 Aralık 2025, 22:00 by YZ_09  
**Next:** YZ_10 (Test string operations + For loops)  
**Status:** 80% complete, MVP in sight! 🎯

