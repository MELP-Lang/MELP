# YZ_21 Görev Planı: TODO #3 - PARSER_CODEGEN_FULL (MVP Self-Hosting)

**Tarih:** 30 Aralık 2025, 18:30 (REVİZE)  
**Görevli:** YZ_21 (Yeni UA tarafından atanacak)  
**Hazırlayan:** UA_04 (Scope revize edildi - MVP-first!)  
**Durum:** 📝 PLAN HAZIR - MVP SCOPE ✅

---

## 🎯 GÖREV ÖZETİ

**Hedef:** Stage1 compiler source'unu derleyebilecek MINIMUM features  
**Süre:** **5-7 gün (1 hafta)** ← Revize! (önceki: 2-3 hafta)  
**Kritiklik:** 🔴 KRİTİK (Self-hosting için gerekli minimum!)

**Neden Ertelenmişti:** Stage1 = Type Infrastructure only (MM_05 kararı, 30 Ara 2025)  
**Şimdi Neden Yapılıyor:** Bootstrap-Ready checkpoint oluşturuldu (commit: 4b1b549)

---

## ⚠️ SCOPE REVİZYONU (30 Aralık 18:30)

**❌ ESKİ PLAN (Scope Creep!):**
- Advanced generics, pattern matching, closures, operator overloading
- 13 tasks, 5 phases, 2-3 hafta
- 3-TODO_LANGUAGE_FEATURES.md içeriği karışmış!

**✅ YENİ PLAN (MVP-First!):**
- Sadece Stage1 source'u derleyebilecek minimum features
- 8 tasks, 3 phases, 5-7 gün
- Advanced features → 3-TODO_LANGUAGE_FEATURES.md'de!

---

## 📊 STAGE1 SOURCE ANALİZİ (Neyi Derleyeceğiz?)

### Stage1 Kullandığı Features:

```bash
cd MELP/MLP/stage1/modules
wc -l *.mlp  # ~4000 lines total
grep -E "^function" *.mlp | wc -l  # 262 functions
```

**Stage1 Source Kullanıyor:**
- ✅ Function declarations (262 function)
- ✅ Struct definitions (LexerState, ParserState, etc.)
- ✅ Variable assignments (x := 5)
- ✅ Simple expressions (a + b, x * 2)
- ✅ Function calls (lexer_next_token(state))
- ✅ If/else statements
- ✅ For loops (for i = 0 to 10)
- ✅ String operations
- ✅ List operations (mlp_list_*)
- ✅ Imports (zaten çalışıyor ✅)
- ⚠️ **Basit lambda** (lambda var ama complex closure YOK)
- ⚠️ **Basit match** (match var ama advanced pattern YOK)

**Stage1 Source KULLANMIYOR:**
- ❌ Advanced generics (<T extends Comparable>)
- ❌ Operator overloading (custom + operator)
- ❌ Compile-time macros
- ❌ Type inference (bidirectional)
- ❌ Advanced closures (capture by ref/value)

---

## 📋 GÖREV LİSTESİ (MVP SCOPE - 8 Tasks, 3 Phases)

### 🔹 PHASE 1: ANALIZ VE GAP BELIRLEME (1-2 gün)

#### Task 3.1: Stage1 Source Analysis

**Hedef:** Stage1 hangi language features kullanıyor? (Checklist oluştur)

```bash
cd MELP/MLP/stage1/modules

# Function patterns
grep -E "^function" *.mlp > /tmp/functions.txt

# Control flow patterns  
grep -E "if |for |while |match " *.mlp > /tmp/control.txt

# Expression patterns
grep -E ":= |return |lambda" *.mlp > /tmp/expressions.txt
```

**Çıktı Beklentisi:**
- Stage1 feature checklist (hangi dil özellikleri kullanılıyor?)
- Frequency analysis (en çok kullanılan patterns)
- Priority matrix (önce neleri implement edeceğiz?)

**Başarı Kriterleri:**
- [ ] Stage1 feature checklist oluşturuldu
- [ ] Hiç unexpected feature yok (hepsi biliniyor)
- [ ] Priority order belirlendi (MVP-first)

**Tahmini Süre:** 3-4 saat

---

#### Task 3.2: Stage0 Parser/Codegen Gap Analysis

**Hedef:** Stage0 şu an neyi destekliyor, neyi desteklemiyor?

```bash
cd MELP/C/stage0

# Mevcut parser modules
ls modules/*/\*_parser.c

# Mevcut codegen modules  
ls modules/*/\*_codegen.c

# Test et
./melpc ../../MLP/stage1/modules/lexer.mlp /tmp/test.c 2>&1 | head -20
# Hata mesajları → eksik features
```

**Çıktı Beklentisi:**
- Desteklenen features listesi
- Eksik olan features listesi  
- Gap matrix (Stage1 needs vs Stage0 has)

**Başarı Kriterleri:**
- [ ] Mevcut parser capabilities dokümente edildi
- [ ] Mevcut codegen capabilities dokümente edildi
- [ ] Gap list oluşturuldu (neyi implement edeceğiz?)

**Tahmini Süre:** 3-4 saat

---

#### Task 3.3: Implementation Plan (YZ_21_MVP_PLAN.md)

**Hedef:** Hangi modülleri extend edeceğiz? Hangi yeni modüller lazım?

**Plan İçeriği:**
```markdown
## MVP Features (Must Have):
1. ✅ Function definitions (var, extend edilecek)
2. ⏳ Nested expressions (a + b * c)
3. ⏳ Nested control flow (if içinde for)
4. ⏳ List operations codegen
5. ⏳ String operations codegen
6. ⏳ Basic lambda (simple anonymous functions)
7. ⏳ Basic match (simple pattern matching)

## Module Breakdown:
- expression_parser.c (<500 lines) → extend
- control_flow_parser.c (<500 lines) → extend
- functions_codegen.c (<500 lines) → extend
- lambda_parser.c (<400 lines) → NEW
- match_parser.c (<350 lines) → NEW
```

**Başarı Kriterleri:**
- [ ] MVP feature list finalized
- [ ] Module breakdown (<500 lines each)
- [ ] Implementation order (dependency aware)

**Tahmini Süre:** 2-3 saat

---

### 🔹 PHASE 2: PARSER/CODEGEN EXTENSION (3-4 gün)

#### Task 3.4: Expression Parser Extension

**Hedef:** Nested expressions, operator precedence

**Örnek MLP Code:**
```mlp
-- Stage1 source'da bu pattern var:
x := (a + b) * c - d / e
result := function_call(nested_call(x), y + 1)
```

**Implementation:**
```c
// modules/expression/expression_parser.c
// Extend parse_expression() for nested operations
```

**Başarı Kriterleri:**
- [ ] Operator precedence doğru (*, / > +, -)
- [ ] Nested expressions parse ediliyor
- [ ] Function arguments parse (nested calls)
- [ ] <500 lines kuralı korundu

**Test:**
```bash
./melpc test_expr.mlp -o test.c
gcc test.c -lruntime -o test && ./test
```

**Tahmini Süre:** 8-10 saat

---

#### Task 3.5: Control Flow Parser Extension

**Hedef:** Nested if/for, basic match statement

**Örnek MLP Code:**
```mlp
-- Stage1 source pattern:
if condition then
    for i = 0 to 10 do
        if i % 2 == 0 then
            yazdir(i)
        end_if
    end_for
end_if

-- Basic match (Stage1 kullanıyor)
match token_type
    case TOKEN_IF: parse_if()
    case TOKEN_FOR: parse_for()
    default: error("Unknown token")
end_match
```

**Implementation:**
```c
// modules/control_flow/control_flow_parser.c - extend
// modules/match/match_parser.c - NEW (~350 lines)
```

**Başarı Kriterleri:**
- [ ] Nested if/for çalışıyor (3 level)
- [ ] Basic match parse ediliyor (enum matching)
- [ ] <500 lines per module

**Tahmini Süre:** 8-10 saat

---

#### Task 3.6: Function Codegen Extension

**Hedef:** Function bodies → C code generation

**Örnek MLP Code:**
```mlp
function calculate(a; b)
    Numeric result = a + b * 2
    if result > 10 then
        return result
    end_if
    return 0
end_function
```

**C Output:**
```c
int calculate(int a, int b) {
    int result = a + b * 2;
    if (result > 10) {
        return result;
    }
    return 0;
}
```

**Başarı Kriterleri:**
- [ ] Function bodies → valid C
- [ ] Local variables handled
- [ ] Return statements working
- [ ] 0 GCC warnings

**Tahmini Süre:** 10-12 saat

---

#### Task 3.7: Lambda & Match Codegen (Basic)

**Hedef:** Basic lambda ve match → C code

**Lambda (Simple):**
```mlp
-- Stage1 pattern (simple, no closure):
callback := lambda(x) return x * 2 end_lambda
result := callback(5)  -- 10
```

**C Output:**
```c
int lambda_1(int x) { return x * 2; }
int result = lambda_1(5);
```

**Match (Simple):**
```mlp
match value
    case 1: yazdir("bir")
    case 2: yazdir("iki")
    default: yazdir("diğer")
end_match
```

**C Output:**
```c
switch (value) {
    case 1: runtime_print("bir"); break;
    case 2: runtime_print("iki"); break;
    default: runtime_print("diğer"); break;
}
```

**Başarı Kriterleri:**
- [ ] Simple lambda → function pointer
- [ ] Simple match → C switch
- [ ] NO advanced features (closure, pattern destructuring)
- [ ] <500 lines per module

**Tahmini Süre:** 8-10 saat

---

### 🔹 PHASE 3: STAGE2 BINARY + SELF-HOSTING (2-3 gün)

#### Task 3.8: Build Stage2 Binary

**Hedef:** Stage1 source'u derle → melpc_stage2 binary

```bash
cd MELP/C/stage0

# Stage1 source'u Stage0 ile derle
./melpc ../../MLP/stage1/modules/*.mlp -o ../../MLP/stage2/

cd ../../MLP/stage2
# Stage2 binary oluştur
gcc *.c -I../../runtime -L../../runtime -lruntime -o melpc_stage2

# Test
./melpc_stage2 --version
```

**Başarı Kriterleri:**
- [ ] Stage1 source compile oldu (0 errors)
- [ ] melpc_stage2 binary oluşturuldu
- [ ] Binary çalışıyor (basic test)
- [ ] Size < 2MB

**Tahmini Süre:** 6-8 saat

---

#### Task 3.9: Self-hosting Convergence Test

**Hedef:** Stage2 == Stage3 (self-hosting kanıtı!)

```bash
cd MELP/MLP/stage2

# Stage2 ile Stage1 source'u derle
./melpc_stage2 ../stage1/modules/*.mlp -o stage3/

cd stage3
gcc *.c -L../../runtime -lruntime -o melpc_stage3

# Convergence test
diff -r ../stage2_output/ ./  # C code aynı mı?
md5sum melpc_stage2 melpc_stage3  # Binary aynı mı?

# Eğer aynı ise:
echo "✅ SELF-HOSTING CONVERGENCE ACHIEVED!"
```

**Başarı Kriterleri:**
- [ ] Stage2 compiles Stage1 source ✅
- [ ] Stage3 binary created ✅
- [ ] Stage2 == Stage3 (C code identical) ✅
- [ ] 🎉 **FULL SELF-HOSTING PROOF!** ✅

**Tahmini Süre:** 8-10 saat

---

## ⏱️ REVİZE TİMELİNE

| Phase | Duration | Tasks |
|-------|----------|-------|
| **PHASE 1: Analiz** | 1-2 gün | 3.1, 3.2, 3.3 |
| **PHASE 2: Parser/Codegen** | 3-4 gün | 3.4, 3.5, 3.6, 3.7 |
| **PHASE 3: Stage2 + Self-hosting** | 2-3 gün | 3.8, 3.9 |
| **TOTAL** | **6-9 gün** | **9 tasks** |

**Realistic Estimate:** 5-7 gün (1 hafta) ← **REVİZE!**  
**Worst Case:** 10 gün (2 hafta)

---

## 📁 DELIVERABLES

### Code Deliverables
1. `MELP/C/stage0/` - Extended parser + codegen modules
2. `MELP/MLP/stage2/melpc_stage2` - Stage2 compiler binary
3. `MELP/MLP/stage3/melpc_stage3` - Stage3 convergence proof
4. `tests/integration/test_stage2_*.sh` - Full test suite

### Documentation Deliverables
1. `YZ_21_RAPOR.md` - Final completion report
2. `YZ_21_GAP_ANALYSIS.md` - Parser/Codegen analysis
3. `MELP/MLP/stage2/README.md` - Stage2 usage guide
4. `SELF_HOSTING_PROOF.md` - Convergence documentation

### Test Deliverables
1. Unit tests for each module
2. Integration test suite
3. Self-hosting test script
4. Performance benchmarks

---

## ⚠️ POTENTIAL RISKS & MITIGATION

### Risk 1: Parser Complexity Explosion
**Problem:** Nested expressions might require complex parsing logic  
**Mitigation:** 
- Use well-tested parsing techniques (Pratt parser)
- Keep modular (<500 lines per module)
- Break into smaller modules if needed

### Risk 2: Codegen Edge Cases
**Problem:** Unusual MLP constructs might produce invalid C  
**Mitigation:**
- Comprehensive unit tests for each codegen module
- Validate C output with `gcc -fsyntax-only`
- Incremental development (test each feature before moving on)

### Risk 3: Self-Hosting Divergence
**Problem:** Stage2 != Stage3 (convergence fails)  
**Mitigation:**
- Deterministic codegen (no timestamps, random IDs)
- Normalize C output (consistent formatting)
- Debug with diff tool (side-by-side comparison)

### Risk 4: Memory Leaks in Complex Programs
**Problem:** Complex expressions might leak memory  
**Mitigation:**
- Arena allocator in runtime (already exists)
- Valgrind tests for every integration test
- Memory profiling (`/usr/bin/time -v`)

### Risk 5: Timeline Slip (> 3 weeks)
**Problem:** Tasks take longer than estimated  
**Mitigation:**
- Daily progress tracking (commit after each task)
- UA checkpoints (end of each phase)
- Focus on MVP first (advanced features can defer)

---

## 🎯 NEXT STEPS (YENİ UA İÇİN)

### Day 1: Onboarding
1. Read this plan carefully (YZ_21_Gorev_Plani.md)
2. Read 0-TODO_SELFHOSTING.md (TODO #3 section)
3. Read YZ_KURALLAR.md (YZ working principles)
4. Check git log (understand recent commits)
5. Test current Stage0/Stage1 (make sure bootstrap works)

### Day 2-4: PHASE 1 (Analysis)
1. Start Task 3.1 (Parser analysis)
2. Complete Task 3.2 (Codegen analysis)
3. Write YZ_21_GAP_ANALYSIS.md report
4. Get UA approval before proceeding

### Week 1-2: PHASE 2-3 (Parser + Codegen)
1. Implement parser extensions (Tasks 3.4-3.6)
2. Implement codegen extensions (Tasks 3.7-3.9)
3. Continuous testing (unit tests after each module)
4. Commit frequently (small, atomic commits)

### Week 2-3: PHASE 4-5 (Stage2 + Self-hosting)
1. Build Stage2 binary (Task 3.10)
2. Integration tests (Task 3.11)
3. Self-hosting test (Tasks 3.12-3.13)
4. Write YZ_21_RAPOR.md final report

### Post-Completion:
1. Update 0-TODO_SELFHOSTING.md (mark TODO #3 complete)
2. Commit with detailed message
3. Celebrate! 🎉 Full self-hosting achieved!
4. Proceed to directory cleanup + GitHub push

---

## 📖 REFERENCE DOCUMENTS

### Essential Reading:
1. `0-TODO_SELFHOSTING.md` - Master TODO tracker (TODO #3 details)
2. `6-CORE-PRINCIPLES.md` - Project principles (MODULAR, GCC, STO, etc.)
3. `YZ_KURALLAR.md` - YZ working rules
4. `MELP/C/stage0/README.md` - Stage0 architecture
5. `MELP/MLP/stage1/README.md` - Stage1 architecture
6. `MELP/runtime/README.md` - Runtime library API

### Previous YZ Reports (Learn from history):
1. `YZ_12_RAPOR.md` - Runtime modularization (3245 lines → 8 modules)
2. `YZ_15_RAPOR.md` - Stdlib integration (7 P2 modules)
3. `YZ_20_RAPOR.md` - Bootstrap validation (7/7 tests passing)

### Technical References:
1. `docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md` - Memory management
2. `docs/LANGUAGE_EVOLUTION_GUIDE.md` - Language features
3. `examples/*.mlp` - Example MLP programs
4. `tests/test_bootstrap_validation.sh` - Current test suite

---

## 💬 CONTACT & ESCALATION

**Üst Akıl (UA):** Yeni UA atanacak (bu planı hazırlayan UA token limiti doldu)  
**Mastermind (MM):** Kritik kararlar için (6 Core Principles violations, major architecture changes)  
**Danışman (PD):** Structural design questions (module organization, dependency management)

**Escalation Triggers:**
- 6 Core Principles violation detected
- Self-hosting convergence fails (Stage2 != Stage3)
- Timeline slips > 1 week (> 4 weeks total)
- Memory leaks > 1MB in any test
- GCC warnings can't be resolved

---

## 🎉 FINAL NOTES

**Mevcut UA'dan Yeni UA'ya:**

Bu plan 4 aylık deneyimin ürünü. Bootstrap'tan buraya geldik:
- TODO #1: Phase 0-3 ✅ (Stage0+Stage1)
- TODO #2: Import System ✅ (700% faster)
- TODO #4: Runtime Modularize ✅ (85% faster)
- TODO #5: Stdlib Complete ✅ (96% faster)
- TODO #6: Bootstrap Validation ✅ (7/7 tests)

TODO #3 son büyük adım. 2-3 hafta sonra **tam self-hosting compiler**'a sahip olacaksın!

**Core Principles'a sadık kal:**
- MODULAR: Her dosya <500 satır (en önemli!)
- GCC: LLVM yok, sadece C+GCC
- STO: Smart Type Optimization (şimdilik stubs)
- STATELESS: Global state yok (mlp_state exception)
- STRUCT+FUNC: OOP yok
- MODÜL=ŞABLON: Stateless modules

**Test-driven development yap:**
- Her module implement et → test et → commit et
- Integration test'ler yazmayı unutma
- Valgrind her zaman (memory leaks zero tolerance)

**Stuck olursan:**
- Previous YZ reports'a bak (benzer problemler vardır)
- Runtime/Stage0 source code oku (clean, modular code)
- UA'ya sor (escalate etmekten çekinme)

**İyi şanslar! 🚀**

---

**PLAN HAZIRLAYan:** Mevcut UA (30 Aralık 2025, 17:45)  
**STATUS:** ✅ PLAN READY - YENİ UA BEKLENİYOR  
**TOKEN KALAN:** ~950K (yeterli, ama büyük task için taze UA tercih)
