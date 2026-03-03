# STO ve SAO İmplementasyon Yol Haritası

**Tarih:** 28 Ocak 2026  
**Durum:** Stage 1 Stdlib tamamlanıyor, Stage 2 için hazırlık  
**Hedef:** STO ve SAO'nun production-ready implementasyonu

---

## 🎯 Mevcut Durum (28 Ocak 2026)

### ✅ Tamamlananlar:

**1. Dokümantasyon:**
- ✅ STO_PRINCIPLES.md (201 satır) - STO felsefesi ve prensipler
- ✅ SAO_PRINCIPLES.md (597 satır) - SAO mimari ve workflow
- ✅ STO_PERFORMANCE.md (208 satır) - FFI boundary pattern
- ✅ MELP_PHILOSOPHY.md - 7 temel esas dokümantasyonu

**2. Syntax ve User API:**
- ✅ User sadece `numeric` ve `string` görüyor (STO syntax)
- ✅ FFI boundary pattern implementasyonu (double wrapper)
- ✅ Stage 0 bootstrap compiler çalışıyor

**3. Stage 1 Stdlib:**
- ✅ std.string, std.math, std.collections, std.fs, std.net, std.web
- ✅ std.test framework
- 🔄 Package manager (son 2 gün)

### ⚠️ Eksikler:

**STO (Smart Type Optimization):**
- ❌ Compile-time type inference (int64/double/BigDecimal seçimi)
- ❌ Runtime overflow handling ve auto-promotion
- ❌ String SSO (Small String Optimization)
- ❌ Value range analysis

**SAO (Semantic-Aware Optimization):**
- ❌ Frontend semantic metadata generation
- ❌ Token Borrowing ownership analysis
- ❌ Purity analysis
- ❌ LLVM attribute injection layer
- ❌ Metadata preservation pipeline

---

## 📋 İmplementasyon Planı

## Phase 1: STO Implementation (2-3 hafta)

### Week 1: Type Inference Engine

**Hedef:** Compiler'ın numeric değerleri analiz edip en uygun dahili tipi seçmesi

**Adımlar:**

1. **Type Inference Modülü (`compiler/sto/type_inference.cpp`):**
```cpp
enum class InternalType {
    INT64,          // -2^63 to 2^63-1
    DOUBLE,         // ~15 digit precision
    BIGDECIMAL      // Arbitrary precision
};

class TypeInferenceEngine {
public:
    InternalType inferNumericType(const ASTNode* expr);
    
private:
    bool fitsInInt64(const NumericLiteral* literal);
    bool fitsInDouble(const NumericLiteral* literal);
    InternalType inferFromExpression(const BinaryExpression* expr);
};
```

2. **Value Range Analysis:**
```cpp
struct ValueRange {
    std::optional<int64_t> min_value;
    std::optional<int64_t> max_value;
    bool exact_value_known;
    int64_t exact_value;
};

class RangeAnalyzer {
public:
    ValueRange analyzeRange(const ASTNode* expr);
    bool canOverflow(const BinaryExpression* expr, const ValueRange& lhs, const ValueRange& rhs);
};
```

3. **Test Cases:**
```mlp
-- Test 1: Literal analysis
numeric x = 42                     -- Should infer: INT64
numeric y = 3.14                   -- Should infer: DOUBLE
numeric z = 999999999999999999999  -- Should infer: BIGDECIMAL

-- Test 2: Expression analysis
numeric a = 10 + 20                -- INT64 + INT64 = INT64
numeric b = 10 + 3.14              -- INT64 + DOUBLE = DOUBLE
numeric c = huge_number + 1        -- BIGDECIMAL + INT64 = BIGDECIMAL
```

**Deliverables:**
- [ ] `compiler/sto/type_inference.cpp` (400+ satır)
- [ ] `compiler/sto/type_inference.h` (100+ satır)
- [ ] `compiler/sto/value_range.cpp` (300+ satır)
- [ ] 20+ unit tests

---

### Week 2: Runtime Overflow Handling

**Hedef:** Overflow detection ve automatic type promotion

**Adımlar:**

1. **Overflow Detection Runtime (`stdlib/runtime/overflow.cpp`):**
```cpp
// Runtime overflow detection
bool add_overflow_i64(int64_t a, int64_t b, int64_t* result) {
    if (__builtin_add_overflow(a, b, result)) {
        return true;  // Overflow detected
    }
    return false;
}

// Automatic promotion to BigDecimal
void* promote_to_bigdecimal(int64_t value) {
    // Convert to BigDecimal and return heap pointer
    return create_bigdecimal(value);
}
```

2. **Codegen Integration:**
```cpp
// compiler/codegen/numeric_ops.cpp
LLVMValueRef codegenNumericAdd(const BinaryExpression* expr) {
    LLVMValueRef lhs = codegen(expr->left);
    LLVMValueRef rhs = codegen(expr->right);
    
    // If both INT64, generate overflow check
    if (lhs_type == INT64 && rhs_type == INT64) {
        return generateAddWithOverflowCheck(lhs, rhs);
    }
    
    // If DOUBLE, direct add
    if (lhs_type == DOUBLE || rhs_type == DOUBLE) {
        return LLVMBuildFAdd(builder, lhs, rhs, "add");
    }
    
    // If BIGDECIMAL, call runtime
    return callBigDecimalAdd(lhs, rhs);
}

LLVMValueRef generateAddWithOverflowCheck(LLVMValueRef lhs, LLVMValueRef rhs) {
    // Generate LLVM IR:
    // 1. Try add with overflow intrinsic
    // 2. If overflow, promote to BigDecimal
    // 3. Retry operation
}
```

3. **Test Cases:**
```mlp
-- Test: Overflow handling
numeric max_int = 9223372036854775807  -- Max int64
numeric overflow = max_int + 1          -- Should auto-promote to BigDecimal
assert(overflow == 9223372036854775808)
```

**Deliverables:**
- [ ] `stdlib/runtime/overflow.cpp` (500+ satır)
- [ ] `stdlib/runtime/bigdecimal.cpp` (800+ satır)
- [ ] `compiler/codegen/numeric_ops.cpp` (600+ satır)
- [ ] 30+ unit tests (overflow scenarios)

---

### Week 3: String SSO (Small String Optimization)

**Hedef:** Kısa stringler için stack allocation optimization

**Adımlar:**

1. **SSO Data Structure:**
```cpp
// stdlib/runtime/string_sso.cpp
constexpr size_t SSO_THRESHOLD = 23;  // Fit in 24 bytes

struct MelpString {
    union {
        struct {
            char* heap_ptr;
            size_t capacity;
            size_t length;
        } heap;
        
        struct {
            char buffer[23];
            uint8_t length;  // High bit = heap flag
        } sso;
    };
    
    bool is_heap() const { return sso.length & 0x80; }
    size_t length() const { return is_heap() ? heap.length : (sso.length & 0x7F); }
};
```

2. **String Operations:**
```cpp
MelpString string_create(const char* str) {
    size_t len = strlen(str);
    
    if (len <= SSO_THRESHOLD) {
        // SSO: Copy to stack buffer
        MelpString s;
        memcpy(s.sso.buffer, str, len);
        s.sso.length = len;
        return s;
    } else {
        // Heap: Allocate and copy
        MelpString s;
        s.heap.heap_ptr = malloc(len + 1);
        memcpy(s.heap.heap_ptr, str, len + 1);
        s.heap.capacity = len + 1;
        s.heap.length = len;
        s.sso.length = 0x80;  // Set heap flag
        return s;
    }
}
```

3. **Codegen Integration:**
```cpp
// Compiler detects string literal size
LLVMValueRef codegenStringLiteral(const StringLiteral* lit) {
    if (lit->value.length() <= SSO_THRESHOLD) {
        // Generate inline stack allocation
        return generateSSOString(lit->value);
    } else {
        // Generate heap allocation
        return generateHeapString(lit->value);
    }
}
```

**Deliverables:**
- [ ] `stdlib/runtime/string_sso.cpp` (700+ satır)
- [ ] `compiler/codegen/string_ops.cpp` (400+ satır)
- [ ] 25+ unit tests (SSO edge cases)

---

## Phase 2: SAO Implementation (4-5 hafta)

### Week 4-5: Semantic Metadata Layer

**Hedef:** Frontend'in semantic analysis sonuçlarını metadata olarak saklamak

**⚠️ STATELESS COMPLIANCE:** Metadata immutable, context function parameter olarak geçilir

**Adımlar:**

1. **SAO Metadata Yapısı (`compiler/sao/metadata.h`):**
```cpp
struct SAOMetadata {
    // Type Information (from STO)
    struct {
        bool type_proven;
        InternalType inferred_type;
        bool bounds_checked;
        bool can_eliminate_checks;
    } type_info;
    
    // Ownership Information (from Token Borrowing)
    struct {
        bool exclusive_borrow;
        bool shared_borrow;
        bool no_alias_proven;
        bool memory_safe;
        std::vector<std::string> borrowed_vars;
    } ownership;
    
    // Purity Information
    struct {
        bool pure_function;
        bool no_side_effects;
        bool referentially_transparent;
        bool can_eliminate_if_unused;
        std::vector<std::string> read_vars;
        std::vector<std::string> write_vars;
    } purity;
    
    // Constant Information
    struct {
        bool compile_time_constant;
        bool runtime_constant;
        std::optional<int64_t> constant_value;
    } constant;
};
```

2. **Metadata Generation Pipeline:**
```cpp
class SAOAnalyzer {
public:
    SAOMetadata* analyze(const ASTNode* node);
    
private:
    SAOMetadata* analyzeExpression(const Expression* expr);
    SAOMetadata* analyzeFunction(const FunctionDecl* func);
    SAOMetadata* analyzeStatement(const Statement* stmt);
    
    // STO Integration
    void inferType(SAOMetadata* sao, const Expression* expr);
    
    // Token Borrowing Integration
    void analyzeOwnership(SAOMetadata* sao, const FunctionDecl* func);
    
    // Purity Analysis
    void analyzePurity(SAOMetadata* sao, const FunctionDecl* func);
    
    // Constant Evaluation
    void evaluateConstant(SAOMetadata* sao, const Expression* expr);
};
```

3. **AST Integration (SMART POINTER for Safety):**
```cpp
// ✅ STATELESS + Memory Safety
class ASTNode {
protected:
    std::unique_ptr<SAOMetadata> sao_metadata;  // Ownership clear
    
public:
    void setSAOMetadata(std::unique_ptr<SAOMetadata> meta) { 
        sao_metadata = std::move(meta); 
    }
    const SAOMetadata* getSAOMetadata() const { return sao_metadata.get(); }
};
```

**Deliverables:**
- [ ] `compiler/sao/metadata.h` (200+ satır)
- [ ] `compiler/sao/metadata.cpp` (400+ satır)
- [ ] `compiler/sao/analyzer.cpp` (800+ satır)
- [ ] 40+ unit tests

---

### Week 6: Token Borrowing Ownership Analysis

**Hedef:** Ownership rules ile memory safety guarantee

**⚠️ STATELESS CRITICAL:** Context function parameter olarak geçilir (GLOBAL STATE YOK)

**Adımlar:**

1. **Ownership Context (STATELESS Pattern):**
```cpp
// ✅ STATELESS: State fonksiyon parametresi
struct BorrowInfo {
    BorrowType type;  // Exclusive or Shared
    std::vector<std::string> borrowers;
    bool active;
};

struct BorrowContext {
    std::map<std::string, BorrowInfo> borrow_map;
};

// ✅ Pure functions - no instance state
void trackBorrow(BorrowContext& ctx, const std::string& var, BorrowType type);
bool canBorrow(const BorrowContext& ctx, const std::string& var, BorrowType type);
bool provesNoAlias(const BorrowContext& ctx, const std::string& var1, const std::string& var2);
```

2. **Aliasing Analysis (with Context):**
```cpp
// ✅ STATELESS: Context passed as parameter
bool proveNoAlias(const BorrowContext& ctx, const Expression* lhs, const Expression* rhs) {
    // If different exclusive borrows → no alias guaranteed
    auto lhs_borrow = getBorrowInfo(ctx, lhs);
    auto rhs_borrow = getBorrowInfo(ctx, rhs);
    
    if (lhs_borrow && rhs_borrow && 
        lhs_borrow->type == BorrowType::Exclusive && 
        rhs_borrow->type == BorrowType::Exclusive) {
        return true;
    }
    
    // If different struct fields → no alias
    if (lhs->isStructField() && rhs->isStructField()) {
        return lhs->getFieldName() != rhs->getFieldName();
    }
    
    return false;  // Conservative: assume alias
}
```

3. **SAO Integration:**
```cpp
void analyzeOwnership(SAOMetadata* sao, const FunctionDecl* func) {
    for (auto param : func->parameters) {
        if (param->hasBorrowAnnotation()) {
            sao->ownership.exclusive_borrow = param->isExclusiveBorrow();
            sao->ownership.shared_borrow = param->isSharedBorrow();
            sao->ownership.no_alias_proven = true;
        }
    }
}
```

**Deliverables:**
- [ ] `compiler/sao/ownership.cpp` (600+ satır)
- [ ] `compiler/sao/aliasing.cpp` (400+ satır)
- [ ] 35+ unit tests

---

### Week 7: Purity Analysis

**Hedef:** Pure function detection ve side effect tracking

**Adımlar:**

1. **Purity Analyzer:**
```cpp
class PurityAnalyzer {
public:
    bool isPureFunction(const FunctionDecl* func);
    bool hasSideEffects(const Statement* stmt);
    
private:
    bool readsGlobalState(const Expression* expr);
    bool writesGlobalState(const Statement* stmt);
    bool performsIO(const Statement* stmt);
    bool callsImpureFunction(const CallExpression* call);
};
```

2. **Side Effect Tracking:**
```cpp
struct SideEffectInfo {
    bool reads_global;
    bool writes_global;
    bool performs_io;
    bool allocates_memory;
    bool throws_exception;
    std::vector<std::string> modified_vars;
};

SideEffectInfo analyzeSideEffects(const FunctionDecl* func) {
    SideEffectInfo info;
    
    // Traverse function body
    for (auto stmt : func->body->statements) {
        if (isGlobalAccess(stmt)) {
            info.reads_global = true;
        }
        if (isGlobalModification(stmt)) {
            info.writes_global = true;
        }
        if (isIOOperation(stmt)) {
            info.performs_io = true;
        }
    }
    
    return info;
}
```

3. **SAO Integration:**
```cpp
void analyzePurity(SAOMetadata* sao, const FunctionDecl* func) {
    SideEffectInfo effects = analyzeSideEffects(func);
    
    sao->purity.pure_function = !effects.writes_global && 
                                 !effects.performs_io;
    sao->purity.no_side_effects = effects.modified_vars.empty();
    sao->purity.referentially_transparent = sao->purity.pure_function;
    sao->purity.can_eliminate_if_unused = sao->purity.pure_function;
}
```

**Deliverables:**
- [ ] `compiler/sao/purity.cpp` (500+ satır)
- [ ] 30+ unit tests

---

### Week 8: LLVM Metadata & Attribute Injection

**Hedef:** SAO metadata'yı LLVM attributes/metadata'ya dönüştürmek

**⚠️ LLVM STRATEGY:** Custom attributes yerine metadata kullanımı (LLVM built-in'lere bağımlılık)

**Adımlar:**

1. **Attribute Injection Layer:**
```cpp
class SAOLLVMBridge {
public:
    void applySAOMetadata(LLVMValueRef value, const SAOMetadata* sao);
    
private:
    void applyTypeAttributes(LLVMValueRef value, const SAOMetadata* sao);
    void applyOwnershipAttributes(LLVMValueRef value, const SAOMetadata* sao);
    void applyPurityAttributes(LLVMValueRef value, const SAOMetadata* sao);
    void applyConstantAttributes(LLVMValueRef value, const SAOMetadata* sao);
};
```

2. **LLVM Metadata Generation (Built-in + Custom):**
```cpp
void applySAOMetadata(LLVMValueRef value, const SAOMetadata* sao) {
    // Type proven → custom metadata + bounds check elimination
    if (sao->type_info.type_proven) {
        // ✅ Custom metadata (LLVM supports this)
        LLVMSetMetadata(value, LLVMGetMDKindID("melp.sao.type.proven", 20), 
                        LLVMValueAsMetadata(LLVMConstInt(LLVMInt1Type(), 1, 0)));
        
        if (sao->type_info.can_eliminate_checks) {
            // Custom metadata for our optimization passes
            LLVMSetMetadata(value, LLVMGetMDKindID("melp.sao.no_check", 17),
                           LLVMValueAsMetadata(LLVMConstInt(LLVMInt1Type(), 1, 0)));
        }
    }
    
    // No alias proven → LLVM built-in attribute ✅
    if (sao->ownership.no_alias_proven) {
        LLVMAddAttribute(value, LLVMCreateEnumAttribute(LLVMGetGlobalContext(), 
                                                         LLVMGetEnumAttributeKindForName("noalias", 7), 0));
    }
    
    // Pure function → LLVM built-in attributes ✅
    if (sao->purity.pure_function) {
        LLVMAddAttribute(value, LLVMCreateEnumAttribute(LLVMGetGlobalContext(),
                                                         LLVMGetEnumAttributeKindForName("readonly", 8), 0));
        LLVMAddAttribute(value, LLVMCreateEnumAttribute(LLVMGetGlobalContext(),
                                                         LLVMGetEnumAttributeKindForName("willreturn", 10), 0));
    }
    
    // Constant → LLVM constant value
    if (sao->constant.compile_time_constant) {
        // Mark as constant via metadata
        LLVMSetMetadata(value, LLVMGetMDKindID("melp.sao.constant", 19),
                       LLVMValueAsMetadata(LLVMConstInt(LLVMInt64Type(), 
                                                        sao->constant.constant_value.value(), 0)));
    }
}
```

3. **Optimization Pass Integration:**
```cpp
// LLVM optimization passes SAO metadata'yı kullanır
void runSAOOptimizationPasses(LLVMModuleRef module) {
    LLVMPassManagerRef pm = LLVMCreatePassManager();
    
    // SAO-aware passes
    LLVMAddSAOTypeOptimizationPass(pm);
    LLVMAddSAOAliasAnalysisPass(pm);
    LLVMAddSAOPurityOptimizationPass(pm);
    LLVMAddSAOConstantPropagationPass(pm);
    
    // Standard LLVM passes (SAO metadata'dan faydalanır)
    LLVMAddInstructionCombiningPass(pm);
    LLVMAddCFGSimplificationPass(pm);
    LLVMAddDeadCodeEliminationPass(pm);
    LLVMAddLoopVectorizePass(pm);  // Uses noalias hints
    
    LLVMRunPassManager(pm, module);
}
```

**Deliverables:**
- [ ] `compiler/sao/llvm_bridge.cpp` (700+ satır)
- [ ] `compiler/sao/optimization_passes.cpp` (500+ satır)
- [ ] 40+ integration tests
- [ ] LLVM metadata documentation (SAO custom metadata reference)

---

## Phase 3: Integration & Testing (3 hafta)

**⚠️ EXTENDED:** Integration testing 2→3 hafta (Stage 0/1 regression tests)

### Week 9-10: End-to-End Integration

**Hedef:** STO + SAO pipeline'ı tüm compiler'a entegre etmek

**⚠️ CRITICAL:** Stage 0 + Stage 1 regression testing (500+ tests)

**Adımlar:**

1. **Compiler Pipeline Update:**
```
User Code (.mlp)
     ↓
[Lexer → Parser → AST]
     ↓
[STO Type Inference]  ← NEW
     ↓
[SAO Semantic Analysis]  ← NEW
     ↓
[SAO Metadata Generation]  ← NEW
     ↓
[LLVM Codegen + SAO Attributes]  ← UPDATED
     ↓
[LLVM Optimization Passes (SAO-aware)]  ← NEW
     ↓
[Assembly → Binary]
```

2. **Integration Tests:**
```mlp
-- Test 1: STO + SAO together
function fibonacci(n: numeric) -> numeric
    if n <= 1 then
        return n  -- STO: INT64, SAO: constant known
    end_if
    return fibonacci(n-1) + fibonacci(n-2)  -- STO: overflow handling, SAO: pure function
end_function

-- Test 2: Token Borrowing + SAO
function sum_array(borrow arr: Array<numeric>) -> numeric
    let sum: numeric = 0
    for item in arr do
        sum = sum + item  -- SAO: noalias proven (exclusive borrow)
    end_for
    return sum
end_function
```

**Deliverables:**
- [ ] 50+ end-to-end tests (STO+SAO combined)
- [ ] 500+ regression tests (Stage 0 + Stage 1 test suites)
- [ ] Performance benchmarks
- [ ] Integration documentation
- [ ] AddressSanitizer validation (memory safety)

---

### Week 11: Benchmark Suite

**Hedef:** STO/SAO performance impact ölçümü

**Benchmarks:**
1. **Computation:** fibonacci, primes, matrix multiplication
2. **Memory:** Array operations, string operations
3. **Real-world:** JSON parsing, file I/O, web server

**Expected Results:**
- **STO alone:** 1.5-2x speedup (type optimization)
- **SAO alone:** 2-3x speedup (ownership + purity)
- **STO + SAO:** 3-5x speedup (combined) 🎯

**Deliverables:**
- [ ] Benchmark suite (extend existing benchmarks/)
- [ ] Performance comparison report
- [ ] Optimization analysis
- [ ] Correctness verification (STO/SAO doesn't change program behavior)

---

### Week 12: Buffer & Polish

**Hedef:** Timeline buffer, documentation polish, final QA

**Adımlar:**
- Catch-up for any delays (Weeks 1-11)
- Documentation final review
- Code quality polish
- Final integration testing
- Release preparation

**Deliverables:**
- [ ] All tasks from Weeks 1-11 completed
- [ ] Production-ready STO+SAO implementation
- [ ] Comprehensive documentation
- [ ] Release notes

---

## 🎯 Milestone Schedule

**⚠️ REVISED TIMELINE:** 10 → 12 hafta (+2 hafta buffer for safety)

| Week | Phase | Deliverable | Status |
|------|-------|-------------|---------|
| W1 | STO Type Inference | Type inference engine | ⏳ Not started |
| W2 | STO Overflow | Runtime overflow handling | ⏳ Not started |
| W3 | STO String | SSO implementation | ⏳ Not started |
| W4-5 | SAO Metadata | Metadata layer | ⏳ Not started |
| W6 | SAO Ownership | Token Borrowing analysis (STATELESS) | ⏳ Not started |
| W7 | SAO Purity | Purity analyzer | ⏳ Not started |
| W8 | SAO LLVM | LLVM metadata injection | ⏳ Not started |
| W9-10 | Integration | End-to-end + regression testing | ⏳ Not started |
| W11 | Benchmarks | Performance testing | ⏳ Not started |
| W12 | Buffer | Catch-up + polish | ⏳ Not started |

**Total Duration:** 12 hafta (~3 ay) + 2 hafta buffer = **14 hafta maksimum**
**Start Date:** Stage 1 tamamlandıktan sonra (Mart başı 2026)  
**Target Completion:** Haziran sonu 2026 (12 hafta)  
**Hard Deadline:** Temmuz ortası 2026 (14 hafta with buffer)

---

## 🔧 Teknik Gereksinimler

### Compiler Dependencies:
- LLVM 14+ (optimization passes için)
- C++17 (compiler code için)
- **GMP library (BigDecimal için - OPTIONAL):**
  - **Rationale:** Production-quality arbitrary precision math
  - **Fallback:** int64/double only (covers 99% use cases)
  - **Stage 3:** Custom BigDecimal implementation (remove GMP dependency)
  - **Decision:** PD approval required for external dependency

### Development Tools:
- LLVM IR viewer (opt -print-module)
- Performance profiler (perf, valgrind)
- Memory debugger (AddressSanitizer, Valgrind)

### Testing Framework:
- Unit tests: Google Test
- Integration tests: Custom MLP test framework
- Benchmarks: Existing benchmark suite

---

## ⚠️ RISK MITIGATION & CONTINGENCY PLANS

### Risk 1: BigDecimal Complexity (HIGH)
**Problem:** BigDecimal runtime 800+ LOC, GMP external dependency  
**Impact:** Timeline delay, dependency management  
**Probability:** MEDIUM (60%)  
**Mitigation:**
- **Phase 1:** int64 + double only (sufficient for 99% cases) - NO BIGDECIMAL
- **Phase 2 (optional):** BigDecimal if stdlib requires it
- **Fallback:** Use GMP wrapper (reduce custom LOC to 200)
- **Stage 3:** Custom BigDecimal (remove GMP dependency)

**Contingency:** If BigDecimal blocks timeline → defer to Stage 3

---

### Risk 2: Token Borrowing Dependency (MEDIUM)
**Problem:** SAO ownership analysis requires Token Borrowing, TB not implemented yet  
**Impact:** SAO Week 6 blocked  
**Probability:** MEDIUM (50%)  
**Mitigation:**
- **SAO Phase 1:** Purity + constant analysis (TB-independent) - PROCEED
- **SAO Phase 2:** Conservative ownership analysis (local vars only)
- **TB Integration:** When TB available → SAO automatically improves
- **Parallel Path:** TB development separate from SAO

**Contingency:** SAO delivers 60-70% value without TB (purity + constant optimization)

---

### Risk 3: LLVM Metadata Complexity (MEDIUM)
**Problem:** Custom LLVM metadata API complex, limited documentation  
**Impact:** Week 8 implementation difficulty  
**Probability:** LOW (30%)  
**Mitigation:**
- **Week 8 Spike (2 days):** Feasibility check before full implementation
- **Fallback:** Use only built-in attributes (noalias, readonly, willreturn)
- **Tradeoff:** 80% performance gain acceptable (vs 100% with custom metadata)
- **LLVM Fork:** NOT RECOMMENDED (maintenance burden)

**Contingency:** If custom metadata fails → built-in attributes sufficient for 80% gains

---

### Risk 4: Performance Regression (CRITICAL)
**Problem:** Optimization sometimes slows code (Amdahl's Law, overhead)  
**Impact:** User-visible performance degradation  
**Probability:** LOW (20%) but HIGH IMPACT  
**Mitigation:**
- **Every commit:** Run benchmark suite (automated CI)
- **Regression detection:** >5% slowdown = BLOCK merge
- **Optimization flags:** Allow users to disable STO/SAO (escape hatch)
- **Profiling:** Identify hot paths before optimization

**Contingency:** Disable specific optimization if causes regression (granular control)

**Gate:** Stage 0 baseline performance MUST be maintained (0% regression tolerance)

---

### Risk 5: STATELESS Violation (HIGH)
**Problem:** Ownership tracking needs state, conflicts with STATELESS principle  
**Impact:** Architecture violation, technical debt  
**Probability:** MEDIUM (50%)  
**Mitigation:**
- **BorrowContext pattern:** State as function parameter (not instance variable)
- **Immutable metadata:** SAOMetadata read-only after construction
- **Pure functions:** All analysis functions stateless
- **Code review:** MM reviews for STATELESS compliance

**Contingency:** Architectural refactor if violation detected (Week 6 catch this early)

---

### Risk 6: Timeline Overrun (MEDIUM)
**Problem:** 10 weeks aggressive, SAO complex  
**Impact:** Delayed delivery, resource pressure  
**Probability:** HIGH (70%)  
**Mitigation:**
- **Buffer:** 12 weeks target + 2 weeks buffer = 14 weeks max
- **Incremental delivery:** STO first (Weeks 1-3), SAO later (Weeks 4-8)
- **Scope flexibility:** BigDecimal optional, TB-independent SAO
- **Weekly checkpoints:** Early detection of delays

**Contingency:** Reduce SAO scope if timeline critical (purity + constant only)

---

## 📊 Success Criteria

### STO Success:
- ✅ User sadece `numeric` görüyor
- ✅ Compiler otomatik int64/double/BigDecimal seçiyor
- ✅ Overflow otomatik handle ediliyor
- ✅ SSO kısa stringleri optimize ediyor
- ✅ Performance: 1.5-2x speedup

### SAO Success:
- ✅ Semantic metadata preserved
- ✅ LLVM attributes doğru inject ediliyor
- ✅ Ownership analysis çalışıyor
- ✅ Purity detection doğru
- ✅ Performance: 3-5x total speedup (STO+SAO)

### Integration Success:
- ✅ Tüm stdlib modülleri STO/SAO ile optimize ediliyor
- ✅ **No performance regression** (Stage 0 baseline maintained)
- ✅ All tests passing (500+ regression tests)
- ✅ Benchmark results documented
- ✅ **STATELESS compliance** (MM verification)
- ✅ **Memory safety** (AddressSanitizer clean)

---

## 🔍 MM REVIEW COMPLIANCE

**Review Date:** 28 Ocak 2026  
**Reviewer:** MM_03 (Mastermind)  
**Status:** ✅ **APPROVED WITH REVISIONS**

**Revisions Applied:**
1. ✅ **STATELESS Compliance:** OwnershipTracker → BorrowContext (function parameter)
2. ✅ **Memory Safety:** SAOMetadata raw pointer → unique_ptr
3. ✅ **LLVM Strategy:** Custom attributes → Built-in + metadata
4. ✅ **Timeline Buffer:** 10 → 12 hafta (+2 buffer = 14 max)
5. ✅ **GMP Dependency:** Documented rationale + fallback strategy
6. ✅ **Risk Mitigation:** 6 major risks identified + contingency plans

**Outstanding Issues:**
- 🔄 **PD Consultation:** GMP dependency acceptance (PENDING)
- 🔄 **TB Integration:** Token Borrowing availability timeline (UNKNOWN)

**Final Approval:** ✅ Ready for implementation after PD consultation

---

## 🚀 Next Steps

**ŞU AN (28 Ocak 2026):**
1. ✅ Stage 1 tamamlandı (stdlib + applications)
2. 🔄 **PD CONSULTATION:** GMP dependency approval (PENDING)
3. 📋 MM review revisions applied (DONE)

**Stage 2 Hazırlık (Şubat 2026):**
1. 🎯 PD approval for GMP dependency
2. 🎯 Token Borrowing status assessment
3. 🎯 Team assignments (STO YZ, SAO YZ)
4. 🎯 Development environment setup

**Stage 2 Kickoff (Mart 2026):**
1. 📋 Week 1 starts: STO Type Inference Engine
2. 🔄 Haftalık progress reports
3. 🎯 Weekly milestone reviews

---

**Timeline:** 12 hafta target (Mart-Haziran 2026) + 2 hafta buffer = **14 hafta max**  
**Hedef:** Production-ready STO + SAO implementation  
**Expected Outcome:** 3-5x performance improvement 🚀  
**Quality Gate:** 98/100 (Stage 0/1 standard maintained)

---

**Document Version:** 2.0 (MM Review Applied)  
**Last Updated:** 28 Ocak 2026  
**Status:** ✅ Ready for PD approval + implementation
