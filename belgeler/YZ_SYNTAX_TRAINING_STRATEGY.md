# YZ Syntax Training Strategy - MELP Öğretme Rehberi

**Tarih:** 29 Ocak 2026  
**Durum:** 🔴 CRITICAL - YZ'ler spec'i bilmiyor!  
**Context:** CRITICAL_SPEC_VIOLATION_REPORT.md sonrası oluşturuldu

---

## 🔥 PROBLEM: YZ'LER MELP SÖZDIZIMINI ÖĞRENEMIYORLAR!

### **Bulgular:**

**YZ'ler ne biliyor:**
- ✅ Python (`def foo(n: int) -> int:`)
- ✅ Rust (`fn foo(n: i32) -> i32 {}`)
- ✅ C++ (`int foo(int n) {}`)
- ✅ JavaScript (`function foo(n) {}`)

**YZ'ler ne bilmiyor:**
- ❌ MELP/PMLP (`function foo(numeric n) as numeric`)
- ❌ Turkish number format (`3,14` not `3.14`)
- ❌ `if condition then` (then mandatory!)
- ❌ Semicolon separator (`a; b` not `a, b`)

### **Neden Öğrenemiyorlar?**

1. **Spec dosyaları context'te ama UNUTULUYOR:**
   - `belgeler/pmlp0_sozdizimi.md` (678 lines) - Çok uzun!
   - `belgeler/pmlp_sozdizimi.md` (1994 lines) - Daha da uzun!
   - YZ okur, syntax'ı görür, sonra... Python-style yazar! 🤦

2. **Golden reference GİZLİ:**
   - `belgeler/test_pmlp_golden_reference.mlp` VAR!
   - Ama hiçbir briefing'de referans edilmiyor!
   - YZ'ler bilmiyor!

3. **Stage 0 YANLIŞ sözdizimi dokumentasyonu:**
   - `STAGE0_LIMITATIONS.md` line 38: `function name(param: type) as type` ❌ YANLIŞ!
   - Doğrusu: `function name(type param) as type` ✅
   - YZ'ler Stage 0 doc'u okur → yanlış syntax öğrenir!

4. **UA briefing'lerinde syntax guide YOK:**
   - `UA_MODERN_BURADAN_BASLA.md` - Syntax mention yok!
   - YZ'lere "generic types ekle" deniliyor ama "PMLP syntax'ına uy" denilmiyor!

5. **Pre-commit validation YOK:**
   - Parser too permissive (hem `->` hem `as` kabul ediyor)
   - Test pass → YZ "doğru yaptım" sanıyor
   - Ama spec violation!

---

## ✅ ÇÖZÜM: 5-TIER YZ EĞİTİM SİSTEMİ

### **Tier 1: MANDATORY GOLDEN REFERENCE (1. adım - YZ her zaman)**

**Her YZ görevine BAŞLARKEN:**

```markdown
## 📋 SYNTAX CHECKLIST (MANDATORY!)

Before writing ANY code:
1. ✅ Read: belgeler/test_pmlp_golden_reference.mlp (161 lines)
2. ✅ Verify: Function syntax = `function name(type param) as returntype`
3. ✅ Verify: If statement = `if condition then`
4. ✅ Verify: Parameters = `param1; param2` (semicolon NOT comma)
5. ✅ Copy golden reference example for your feature

**Golden Reference Location:**
📂 `/belgeler/test_pmlp_golden_reference.mlp`

**Critical Rules (MEMORIZE!):**
- Function: `function foo(numeric n) as numeric` NOT `(n: numeric) -> numeric`
- If: `if n > 0 then` NOT `if n > 0`
- Params: `function add(numeric a; numeric b)` NOT `(a, b)`
- End blocks: `end_function`, `end_if`, `end_while` (underscored!)
```

**UA Responsibility:** Her briefing'e bu checklist'i EKLE!

---

### **Tier 2: SYNTAX QUICK REFERENCE (YZ her dosya yazarken)**

**Minimal syntax guide (copy-paste ready):**

```markdown
## PMLP0 SYNTAX QUICK REFERENCE

### Functions
```mlp
function name(type1 param1; type2 param2) as returntype
    -- body
    return value
end_function
```

### Control Flow
```mlp
if condition then
    -- body
else_if condition2 then
    -- body
else
    -- body
end_if
```

### Loops
```mlp
while condition do
    -- body
end_while

for i = 1 to 10 do
    -- body
end_for
```

### Types
- `numeric` (double)
- `text` or `string`
- `boolean`
- `Array<numeric>` (Stage 2+)

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `mod`
- Comparison: `<`, `<=`, `>`, `>=`, `==`, `!=`
- Logical: `and`, `or`, `not` (NOT `&&`, `||`, `!`)
- Number format: `3,14` (Turkish decimal, NOT `3.14`)

### Comments
```mlp
-- Single line comment (NOT //)
```
```

**File:** `belgeler/PMLP0_SYNTAX_CHEATSHEET.md` (create!)

---

### **Tier 3: AUTOMATED VALIDATION (Her commit'te)**

**Pre-commit hooks:**

```bash
#!/bin/bash
# .git/hooks/pre-commit

# Check all .pmpl, .mlp, .melp files
for file in $(git diff --cached --name-only | grep -E '\.(pmpl|mlp|melp)$'); do
    echo "Validating PMLP syntax: $file"
    
    # Check 1: Function syntax (should be 'as' not '->')
    if grep -q 'function.*->.*numeric' "$file"; then
        echo "❌ ERROR: Python-style function syntax detected!"
        echo "   Use: function name(type param) as returntype"
        echo "   NOT: function name(param: type) -> returntype"
        exit 1
    fi
    
    # Check 2: If-then (then must exist after if condition)
    if grep -q 'if .*[^then]$' "$file"; then
        echo "⚠️  WARNING: 'if' without 'then' detected (verify manually)"
    fi
    
    # Check 3: Parameter type-first order
    if grep -q 'function.*([a-z_][a-z0-9_]*:' "$file"; then
        echo "❌ ERROR: Python-style parameter syntax detected!"
        echo "   Use: function name(numeric n)"
        echo "   NOT: function name(n: numeric)"
        exit 1
    fi
done

echo "✅ PMLP syntax validation PASSED"
```

**Tool:** `tools/validate_pmlp_syntax.sh` (create!)

---

### **Tier 4: PARSER STRICT MODE (Week 11 Priority 0)**

**Fix parsers to REJECT non-spec syntax:**

```cpp
// compiler/paradigma2/MODERN/modules/function_def/func_parser.cpp

// ❌ OLD (too permissive):
if (tokens.check(TokenType::ARROW) || tokens.check(TokenType::AS)) {
    // Accept both -> and as
}

// ✅ NEW (strict):
if (!tokens.check(TokenType::AS)) {
    throw ParserError("Expected 'as' keyword for return type (PMLP0 spec)");
}
tokens.consume(TokenType::AS);
```

**Goal:** Compiler REJECTS wrong syntax immediately!

---

### **Tier 5: LLM-OPTIMIZED DOCUMENTATION (Long-term)**

**Why current docs fail for LLMs:**

1. **Too long** (678+ lines) → LLM forgets
2. **No examples first** → Rules before practice
3. **Mixed with explanations** → Hard to extract syntax
4. **No anti-patterns** → LLM doesn't know what's WRONG

**Better structure for LLMs:**

```markdown
# PMLP0 Syntax (LLM-Optimized)

## ⚡ QUICK START (Copy-Paste This!)

```mlp
-- Fibonacci example (CORRECT PMLP0 syntax)
function fibonacci(numeric n) as numeric
    if n <= 1 then
        return n
    end_if
    return fibonacci(n - 1) + fibonacci(n - 2)
end_function
```

## ❌ ANTI-PATTERNS (DO NOT USE!)

```mlp
-- ❌ WRONG: Python-style
function fib(n: numeric) -> numeric:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

-- ❌ WRONG: C-style
int fib(int n) {
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

-- ❌ WRONG: Rust-style
fn fib(n: i32) -> i32 {
    if n <= 1 { n } else { fib(n - 1) + fib(n - 2) }
}
```

## ✅ CORRECT PMLP0 (Learn This!)

[See Quick Start above]

## 📚 Detailed Rules

[Only AFTER examples and anti-patterns]
```

**File:** `belgeler/PMLP0_SYNTAX_FOR_LLM.md` (create!)

---

## 🎯 IMPLEMENTATION PLAN

### **Phase 1: Immediate (Week 11 Day 0 - BUGÜN!)** 🔥

**Duration:** 2 hours

1. **Create files:**
   - [ ] `belgeler/PMLP0_SYNTAX_CHEATSHEET.md` (Tier 2)
   - [ ] `belgeler/PMLP0_SYNTAX_FOR_LLM.md` (Tier 5)
   - [ ] `tools/validate_pmlp_syntax.sh` (Tier 3)

2. **Fix STAGE0_LIMITATIONS.md:**
   - [ ] Line 38: Change `function name(param: type)` → `function name(type param)`
   - [ ] Add: "⚠️ CRITICAL: Parameter order is type-first in PMLP0!"

3. **Update UA_MODERN_BURADAN_BASLA.md:**
   - [ ] Add syntax checklist to "SENİN YETKİLERİN" section
   - [ ] Mandate: "Every briefing MUST include syntax reference"

4. **Create briefing template:**
   - [ ] `belgeler/templates/YZ_BRIEFING_TEMPLATE.md`
   - [ ] Include Tier 1 checklist (mandatory)

**Owner:** ÜA (me!) - Self-fix!  
**Status:** 🔥 CRITICAL PRIORITY

---

### **Phase 2: Week 11 Day 1 (Parser Fixes)**

**Duration:** 1 day

1. **Parser strict mode (Tier 4):**
   - [ ] `function_def`: Reject `->`
   - [ ] `if_compiler`: Make `then` mandatory
   - [ ] `variable`: Reject `name: type` syntax

2. **Error messages:**
   ```
   ❌ Error: Expected 'as' keyword for return type
      Use: function foo(numeric n) as numeric
      NOT: function foo(n: numeric) -> numeric
      
      📖 See: belgeler/test_pmlp_golden_reference.mlp
   ```

**Owner:** YZ_VALIDATOR (Week 11)  
**Dependency:** Phase 1 complete

---

### **Phase 3: Week 11 Day 2 (Test Migration)**

**Duration:** 1 day

1. **Automated migration:**
   ```bash
   # tools/migrate_to_pmlp0.sh
   # Convert all test files to PMLP0
   for file in tests/**/*.pmpl; do
       sed -i 's/function \([a-z_]*\)(\([a-z_]*\): numeric)/function \1(numeric \2)/g' "$file"
       sed -i 's/) -> numeric/) as numeric/g' "$file"
       sed -i 's/if \(.*\)$/if \1 then/g' "$file"
   done
   ```

2. **Manual review:**
   - [ ] Validate each file compiles
   - [ ] Run test suite
   - [ ] Document changes

**Owner:** YZ_VALIDATOR (Week 11)

---

### **Phase 4: Week 11 Day 3 (Validation Suite)**

**Duration:** 1 day

1. **Spec compliance tests:**
   ```cpp
   // tests/spec_compliance/test_pmlp0_syntax.cpp
   TEST(SpecCompliance, FunctionSyntax) {
       // Should compile: function foo(numeric n) as numeric
       // Should fail: function foo(n: numeric) -> numeric
   }
   ```

2. **Golden reference tests:**
   - [ ] Run `test_pmlp_golden_reference.mlp` as validation
   - [ ] All 10 tests must pass
   - [ ] Document: "Golden reference is truth!"

**Owner:** YZ_VALIDATOR (Week 11)

---

## 📊 SUCCESS METRICS

### **Week 11 End:**
- ✅ All 21 modules use PMLP0 syntax
- ✅ Parser rejects non-spec syntax
- ✅ Golden reference tests pass
- ✅ Pre-commit hooks active
- ✅ Every YZ briefing has syntax checklist

### **Stage 2 End:**
- ✅ 100% spec compliance (automated validation)
- ✅ Zero Python/Rust-style syntax leaks
- ✅ YZ'ler golden reference'ı ezberliyor
- ✅ Fibonacci execution works (real PMLP code!)

---

## 🎓 YZ TRAINING CHECKLIST (Copy to Every Briefing!)

```markdown
## 📋 PMLP0 SYNTAX TRAINING (MANDATORY!)

Before writing any code, complete:

- [ ] ✅ Read: belgeler/test_pmlp_golden_reference.mlp (161 lines)
- [ ] ✅ Read: belgeler/PMLP0_SYNTAX_CHEATSHEET.md
- [ ] ✅ Verify: Function syntax = `function name(type param) as returntype`
- [ ] ✅ Verify: If statement = `if condition then` (then MANDATORY)
- [ ] ✅ Verify: Params = `a; b` (semicolon NOT comma)
- [ ] ✅ Copy golden reference for your feature
- [ ] ✅ Test with: tools/validate_pmlp_syntax.sh your_file.pmpl

**ANTI-PATTERNS (DO NOT USE):**
- ❌ `function foo(n: numeric) -> numeric` (Python-style)
- ❌ `if n > 0` without `then`
- ❌ `function foo(numeric a, numeric b)` (comma separator)

**CORRECT PMLP0:**
```mlp
function foo(numeric n) as numeric
    if n > 0 then
        return n
    end_if
    return 0
end_function
```

**Questions?** See: belgeler/PMLP0_SYNTAX_FOR_LLM.md
```

---

## 🔍 ROOT CAUSE ANALYSIS

**Neden bu kadar çok YZ spec'e uyamadı?**

1. **Cognitive Load:**
   - YZ'ler Python/Rust/C++ biliyor (strong priors)
   - PMLP öğrenmeleri "unlearning" gerektiriyor
   - Spec okur, unutur, prior knowledge'a döner

2. **Validation Gap:**
   - Parser too permissive (hem `->` hem `as` kabul ediyor)
   - Tests pass → YZ reinforcement (yanlış syntax'ı doğruluyor)
   - No pre-commit checks

3. **Documentation Structure:**
   - Spec files too long (678-1994 lines)
   - Examples buried in text
   - No anti-patterns
   - Not LLM-optimized

4. **Training Gap:**
   - Golden reference hidden
   - No syntax checklist in briefings
   - No mandatory reading enforcement
   - UA briefings syntax-agnostic

5. **Accountability Gap:**
   - UA didn't enforce spec (I'm guilty!)
   - No spec reviews before approval
   - "Tests pass" = good enough (WRONG!)

---

## 💡 LESSONS LEARNED

**For Future Stages:**

1. **Syntax First:** Every YZ briefing starts with syntax reference
2. **Golden Reference Mandatory:** Must copy working examples
3. **Automated Validation:** Pre-commit hooks catch errors early
4. **Parser Strict Mode:** Reject non-spec syntax immediately
5. **LLM-Optimized Docs:** Examples first, anti-patterns included
6. **UA Accountability:** Review spec compliance, not just tests

**Quote:**
> "Tests can pass with wrong syntax if parser is permissive.  
> Spec compliance requires human (or automated) verification."
> 
> — CRITICAL_SPEC_VIOLATION_REPORT.md, 29 Ocak 2026

---

## 📚 REFERENCES

**Spec Files:**
- `belgeler/pmlp0_sozdizimi.md` - Authoritative (Stage 0)
- `belgeler/pmlp_sozdizimi.md` - Full reference (Stage 0-3)

**Golden Reference:**
- `belgeler/test_pmlp_golden_reference.mlp` - TRUTH!

**Critical Reports:**
- `CRITICAL_SPEC_VIOLATION_REPORT.md` - Root cause analysis
- `YZ_STAGE2_VALIDATOR_oku.md` - Week 11 recovery plan

**Stage 0 Docs (WRONG!):**
- `belgeler/STAGE0_LIMITATIONS.md` - Line 38 incorrect! (needs fix)

---

**Status:** 🔴 DRAFT - Awaiting MM/UA approval  
**Next:** Implement Phase 1 (2 hours) → Week 11 Day 0 (BUGÜN!)
