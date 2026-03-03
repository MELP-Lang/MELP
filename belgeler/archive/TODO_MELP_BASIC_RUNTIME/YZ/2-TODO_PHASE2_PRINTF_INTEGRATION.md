# 📋 PHASE 2: PRINTF INTEGRATION

**Duration:** 1-2 saat  
**Priority:** 🔴 CRITICAL  
**Dependencies:** Phase 1 complete (string literals working)  
**Owner:** YZ_BASIC_01

---

## 🎯 PHASE GOAL

External function declarations (printf/wrappers) LLVM module'e eklenmeli.

**Target LLVM IR:**
```llvm
; External function declarations
declare void @__builtin_io_print_string(i8*)
declare void @__builtin_io_print_numeric(i64)

; OR (if using direct printf)
declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  ; Functions declared, ready to call
  ret i32 0
}
```

**No Implementation Yet!** Just declarations in IR.

---

## 📋 TASK BREAKDOWN

### Task 2.1: Implement declareExternalFunction() Helper (30 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Generic helper to declare external C functions

**Header Addition (function_compiler.h):**
```cpp
class FunctionCompiler {
private:
    // Existing members...
    
public:
    // NEW: External function declaration helper
    llvm::Function* declareExternalFunction(
        const std::string& name,
        llvm::FunctionType* type
    );
};
```

**Implementation (function_compiler.cpp):**
```cpp
llvm::Function* FunctionCompiler::declareExternalFunction(
    const std::string& name,
    llvm::FunctionType* type
) {
    // 1. Check if already declared (avoid duplicates)
    llvm::Function* existingFunc = module->getFunction(name);
    if (existingFunc) {
        return existingFunc;
    }
    
    // 2. Create external function declaration
    llvm::Function* func = llvm::Function::Create(
        type,
        llvm::Function::ExternalLinkage,  // External (defined elsewhere)
        name,
        module
    );
    
    // 3. Optional: Set calling convention (default is C)
    func->setCallingConv(llvm::CallingConv::C);
    
    return func;
}
```

**Key Points:**
- `ExternalLinkage` = function defined in external library
- Check for existing declaration (idempotent)
- Returns `llvm::Function*` for later use

---

### Task 2.2: Declare Print Wrappers (30 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Declare type-safe print wrappers for MELP

**Where to Add:** In `FunctionCompiler::compile()` method, at the start

**Option A: Type-Safe Wrappers (RECOMMENDED)**

```cpp
void FunctionCompiler::compile(ASTNode* root) {
    // ========================================
    // DECLARE EXTERNAL FUNCTIONS (Phase 2)
    // ========================================
    
    // void __builtin_io_print_string(i8* str)
    llvm::FunctionType* printStringType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),           // Return: void
        {llvm::Type::getInt8PtrTy(context)},      // Param: i8* (string)
        false                                      // Not varargs
    );
    declareExternalFunction("__builtin_io_print_string", printStringType);
    
    // void __builtin_io_print_numeric(i64 num)
    llvm::FunctionType* printNumericType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),           // Return: void
        {llvm::Type::getInt64Ty(context)},        // Param: i64 (number)
        false                                      // Not varargs
    );
    declareExternalFunction("__builtin_io_print_numeric", printNumericType);
    
    // ========================================
    // REST OF COMPILE LOGIC
    // ========================================
    
    // ... existing code ...
}
```

**Option B: Direct printf() (ALTERNATIVE)**

```cpp
void FunctionCompiler::compile(ASTNode* root) {
    // i32 printf(i8* format, ...)
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),          // Return: i32
        {llvm::Type::getInt8PtrTy(context)},      // First param: i8* (format)
        true                                       // Varargs (...)
    );
    declareExternalFunction("printf", printfType);
    
    // ... rest of code ...
}
```

**Recommendation:** Use Option A (wrappers) for:
- Type safety at compile time
- Consistent with MELP type system
- Easier to extend (println, formatted output)

---

### Task 2.3: Test Declarations (30 min)

**Test Case 1: Check IR Has Declarations**

```mlp
# File: tests/phase2/test_declarations.mlp
function main()
  return 0
end_function
```

**Expected IR:**
```llvm
declare void @__builtin_io_print_string(i8*)
declare void @__builtin_io_print_numeric(i64)

define i32 @main() {
entry:
  ret i32 0
}
```

**Validation:**
```bash
./melp_compiler tests/phase2/test_declarations.mlp -o /tmp/test.ll

# Check declarations exist
grep "declare void @__builtin_io_print_string" /tmp/test.ll
grep "declare void @__builtin_io_print_numeric" /tmp/test.ll

# Should see 2 matches
```

---

### Task 2.4: Verify Function Signatures (15 min)

**Goal:** Ensure function types are correct

**Check Script:**
```bash
#!/bin/bash
# tests/phase2/verify_signatures.sh

IR_FILE=/tmp/test.ll

# Check print_string signature
if grep -q "declare void @__builtin_io_print_string(i8\*)" "$IR_FILE"; then
    echo "✅ print_string signature correct"
else
    echo "❌ print_string signature wrong"
    exit 1
fi

# Check print_numeric signature
if grep -q "declare void @__builtin_io_print_numeric(i64)" "$IR_FILE"; then
    echo "✅ print_numeric signature correct"
else
    echo "❌ print_numeric signature wrong"
    exit 1
fi

echo "✅ All signatures correct"
```

**Run:**
```bash
./melp_compiler tests/phase2/test_declarations.mlp -o /tmp/test.ll
bash tests/phase2/verify_signatures.sh
```

---

### Task 2.5: LLVM Validation (15 min)

**Goal:** Ensure IR is valid

**Commands:**
```bash
# Compile test case
./melp_compiler tests/phase2/test_declarations.mlp -o /tmp/test.ll

# Assemble to bitcode
llvm-as /tmp/test.ll -o /tmp/test.bc
echo "Exit code: $?"  # Should be 0

# Verify IR
opt -verify /tmp/test.bc -o /dev/null
echo "Exit code: $?"  # Should be 0

# Compile to object (should link later)
llc /tmp/test.ll -o /tmp/test.o
echo "Exit code: $?"  # Should be 0

echo "✅ Phase 2 IR validates"
```

---

## ✅ SUCCESS CRITERIA

**Phase 2 Complete When:**
- [ ] `declareExternalFunction()` method works
- [ ] `__builtin_io_print_string(i8*)` declared in IR
- [ ] `__builtin_io_print_numeric(i64)` declared in IR
- [ ] Function signatures are correct
- [ ] LLVM IR validates (`opt -verify` passes)
- [ ] Object file compiles (`llc` succeeds)
- [ ] No duplicate declarations
- [ ] No compilation warnings

---

## 🚧 TROUBLESHOOTING

### Problem 1: Duplicate Declarations

**Symptom:**
```llvm
declare void @__builtin_io_print_string(i8*)
declare void @__builtin_io_print_string(i8*)  ; ERROR: Duplicate!
```

**Solution:**
Check for existing function before declaring:
```cpp
llvm::Function* existingFunc = module->getFunction(name);
if (existingFunc) {
    return existingFunc;  // Already declared
}
```

---

### Problem 2: Wrong Function Type

**Symptom:**
```
error: function type mismatch
```

**Solution:**
Ensure parameter types match exactly:
```cpp
// WRONG:
{llvm::Type::getInt32Ty(context)}  // i32 instead of i64

// CORRECT:
{llvm::Type::getInt64Ty(context)}  // i64 for numeric
```

---

### Problem 3: Varargs Not Working

**Symptom:**
```
error: printf expects varargs
```

**Solution:**
Set third parameter to `true` for varargs:
```cpp
llvm::FunctionType* printfType = llvm::FunctionType::get(
    llvm::Type::getInt32Ty(context),
    {llvm::Type::getInt8PtrTy(context)},
    true  // ← MUST be true for printf(...)
);
```

---

## 📚 REFERENCE CODE (Stage 0)

**Stage 0 External Declarations:**
```bash
grep -A 10 "declareRuntimeFunction" compiler/stage0/backend/llvm_backend.cpp
```

**LLVM API Examples:**
```cpp
// Example from Stage 0
llvm::Function* printFunc = module->getOrInsertFunction(
    "printf",
    llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt8PtrTy(*context)},
        true
    )
).getCallee();
```

---

## 🎯 NEXT PHASE

**When Phase 2 Complete:**
📄 **Move to [3-TODO_PHASE3_PRINT_WRAPPERS.md](3-TODO_PHASE3_PRINT_WRAPPERS.md)**

**You'll add:**
- Function call mechanism (CALL_EXPR)
- compileCallExpr() implementation
- Type-based print() dispatcher
- Actual function calls in IR

---

## 📋 PHASE 2 CHECKLIST

Before moving to Phase 3:

**Code:**
- [ ] declareExternalFunction() implemented
- [ ] Print wrappers declared in compile()
- [ ] Function signatures correct
- [ ] No duplicate declarations

**Testing:**
- [ ] IR has declare statements
- [ ] Signatures match expected
- [ ] LLVM validates IR
- [ ] Object file compiles

**Quality:**
- [ ] No compiler warnings
- [ ] Code documented
- [ ] No TODOs in code

**Documentation:**
- [ ] Declared functions listed
- [ ] Signature choices documented
- [ ] Known issues noted (if any)

---

**Ready? Start Task 2.1! 🚀**
