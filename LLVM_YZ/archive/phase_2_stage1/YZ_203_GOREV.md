# YZ_203: Generic Types (Type Parameters) - GÖREV BAŞLADI

**Başlangıç Tarihi:** 21 Aralık 2025  
**Agent:** YZ_203 (GitHub Copilot - Claude Sonnet 4.5)  
**Branch:** `generic-types_YZ_203`  
**Durum:** 🟡 **DEVAM EDİYOR**  
**Süre Tahmini:** 2 hafta

---

## 🎯 Görev Tanımı

Modern programlama dillerinin temel özelliklerinden **generic types** (type parameters) implementasyonu.

**Kaynak:** `TODO_MODERN_LANGUAGE.md` Line 216-242

**Hedef:**
- Generic function syntax: `function max<T>(T a, T b) returns T`
- Generic struct syntax: `struct Box<T>` (future)
- Type parameter constraints: `<T: Comparable>` (future)
- Monomorphization (compile-time specialization)
- LLVM IR codegen per instance
- Type inference: `max(10, 20)` → infer T = numeric

---

## 📋 Implementation Plan

### **Phase 1: Lexer Analysis** ✅ TAMAMLANDI
**Durum:** TOKEN_LESS ve TOKEN_GREATER zaten var!
- `<` → TOKEN_LESS (comparison & generic açılış)
- `>` → TOKEN_GREATER (comparison & generic kapanış)
- Context-aware parsing gerekli (comparison vs generic)

### **Phase 2: Parser - Generic Function Syntax** (3 gün)
**Hedef:** `function max<T>(T a, T b) returns T` parse edilebilsin

**Yapılacaklar:**
- [ ] FunctionDeclaration extend: `char** type_params; int type_param_count`
- [ ] `functions_parser.c`: Generic type parameter parsing after function name
- [ ] Type parameter validation (T, U, V... uppercase convention)
- [ ] Parameter type resolution: T → TYPE_GENERIC
- [ ] Return type resolution: T → TYPE_GENERIC

**Test Case:**
```pmpl
function max<T>(T a, T b) returns T
    if a > b then
        return a
    end_if
    return b
end_function
```

### **Phase 3: Monomorphization Engine** (5 gün - CORE!)
**Hedef:** `max<numeric>(10, 20)` → generate `max_numeric()` specialized function

**Yapılacaklar:**
- [ ] Generic call detection: `max<numeric>(...)`
- [ ] Template instantiation: Clone FunctionDeclaration
- [ ] Type substitution: T → numeric in all params/return/body
- [ ] Name mangling: `max_numeric`, `max_string`, `max_boolean`
- [ ] Instance caching: Same type → reuse instance
- [ ] Dependency tracking: Generic calls generic → recursive instantiation

**Algoritma:**
1. Parse generic function definition (template)
2. Store in template registry (don't emit LLVM yet)
3. On call `max<numeric>(...)`:
   - Check instance cache: `max_numeric` exists?
   - If no: Clone template, substitute T→numeric, emit LLVM
   - If yes: Use existing instance
4. Name mangling: `{func_name}_{type1}_{type2}...`

**Data Structure:**
```c
typedef struct GenericInstance {
    char* mangled_name;        // "max_numeric"
    FunctionDeclaration* specialized_func;
    char** concrete_types;     // ["numeric"]
    int emitted;               // 1 if LLVM IR emitted
    struct GenericInstance* next;
} GenericInstance;

typedef struct GenericTemplate {
    char* name;                // "max"
    FunctionDeclaration* template_func;
    GenericInstance* instances;
    struct GenericTemplate* next;
} GenericTemplate;
```

### **Phase 4: LLVM Codegen** (3 gün)
**Hedef:** Specialized functions emit correct LLVM IR

**Yapılacaklar:**
- [ ] Generic function → store template, skip immediate emission
- [ ] Generic call → trigger monomorphization
- [ ] Emit specialized function with mangled name
- [ ] Call site: Use mangled name (`call @max_numeric`)

### **Phase 5: Type Inference** (2 gün)
**Hedef:** `max(10, 20)` → automatically infer T = numeric

**Yapılacaklar:**
- [ ] Call without explicit types: `max(10, 20)`
- [ ] Analyze argument types: [numeric, numeric]
- [ ] Unify constraints: T = numeric
- [ ] Trigger monomorphization with inferred type

**Algoritma:**
1. Parse call: `max(arg1, arg2)`
2. Get argument types: [typeof(arg1), typeof(arg2)]
3. Match with template params: [T a, T b]
4. Unify: T must equal typeof(arg1) and typeof(arg2)
5. If mismatch: Type error
6. If match: Instantiate with inferred type

### **Phase 6: Testing & Validation** (1 gün)
**Test Cases:**
```pmpl
-- Test 1: Explicit type
function test_explicit() returns numeric
    return max<numeric>(10, 20)  -- Should return 20
end_function

-- Test 2: Type inference
function test_inference() returns numeric
    return max(10, 20)  -- T inferred as numeric
end_function

-- Test 3: Multiple types
function test_multiple() returns numeric
    numeric n = max<numeric>(10, 20)
    string s = max<string>("apple", "banana")
    return length(s)  -- Should work
end_function

-- Test 4: Generic identity
function identity<T>(T value) returns T
    return value
end_function

function test_identity() returns numeric
    return identity<numeric>(42)
end_function
```

---

## 🛠️ Technical Details

### Context-Aware Parsing: `<` as Comparison vs Generic
**Problem:** `<` is both comparison operator and generic delimiter

**Solution:**
1. **After function name:** `function max<` → Generic
2. **In expression:** `if a < b` → Comparison
3. **After identifier in call:** `max<numeric>` → Generic
4. **Lookahead:** Check next token:
   - Uppercase identifier (T, U, V) → Generic
   - Type keyword (numeric, string) → Generic
   - Else → Comparison

### Name Mangling Convention
```
max<numeric>       → max_numeric
max<string>        → max_string
identity<boolean>  → identity_boolean
max<numeric, string> → max_numeric_string
```

### Type Substitution Algorithm
```
For each type parameter T in template:
    1. Replace T in parameter types
    2. Replace T in return type
    3. Replace T in function body variable declarations
    4. Replace T in type casts/checks
```

---

## 📊 Progress Tracking

**Week 1:**
- [x] Day 1: Görev analizi, lexer kontrol ✅
- [ ] Day 2-3: Parser generic syntax
- [ ] Day 4-6: Monomorphization core
- [ ] Day 7: Testing week 1 progress

**Week 2:**
- [ ] Day 8-10: LLVM codegen
- [ ] Day 11-12: Type inference
- [ ] Day 13: Integration tests
- [ ] Day 14: Documentation & report

---

## 📚 References

**Pattern:** YZ_200 (List), YZ_201 (Map), YZ_202 (Optional)
- Incremental development
- Test-driven approach
- Runtime library first → Parser → Codegen

**Similar Features:**
- C++ templates (compile-time)
- Rust generics (monomorphization)
- Go generics (type parameters)

**Key Difference:**
- MELP: Monomorphization (C++ style) vs Go's interface approach
- Reason: Better performance, explicit types in LLVM IR

---

## ⚠️ Known Limitations (Future Work)

**Ertelenen:**
1. Generic structs: `struct Box<T>` (Phase 7)
2. Type constraints: `<T: Comparable>` (Phase 8)
3. Generic methods: `method Box<T>.get() returns T` (Phase 9)
4. Variadic generics: `function tuple<T...>(T... args)` (Phase 10)

**Reason:** Generic functions are foundation, structs build on top

---

## 🎯 Success Criteria

✅ **Minimal Success:**
- Generic function definition parsed
- Explicit type instantiation works: `max<numeric>(10, 20)`
- Single type parameter T
- Basic LLVM IR emission

✅ **Full Success:**
- Type inference works: `max(10, 20)`
- Multiple instantiations: `max<numeric>`, `max<string>`
- Correct name mangling
- Instance caching (no duplicate functions)
- Integration tests pass

✅ **Bonus:**
- Multiple type parameters: `function pair<T, U>(T a, U b)`
- Generic function calls generic: Recursive instantiation

---

## 📝 Notes

**Timestamp:** 21 Aralık 2025 23:30  
**Status:** Phase 1 complete, moving to Phase 2

**Next Steps:**
1. Read `functions_parser.c` carefully
2. Design generic parsing flow
3. Extend FunctionDeclaration struct
4. Implement type parameter parsing
5. Write first test case
