# 📋 MELP Project TODO - Stage 1 Self-Hosting

**Created:** 15 Aralık 2025  
**Current Status:** 🚀 Stage 0 Complete - Starting Stage 1 Self-Hosting!  
**Branch:** main  
**Last Updated:** 16 Aralık 2025  
**YZ Sessions:** Stage 1 YZ_01+ (Stage 0 = 80 sessions archived in `stage_0_YZ/`)

---

## ⚠️ SYNTAX UYARISI (Çok Önemli!)

**Kullanıcı kodu (.mlp test/örnek dosyaları):** Boşluklu syntax kullan → `end if`, `else if`, `end while`  
**Compiler kodu (parser.mlp, lexer.mlp):** Alt çizgili syntax kullan → `end_if`, `else_if`, `end_while`

**Kural:** Örnekler için `end if`, modüller için `end_if` kullan. Kararsızsan → `kurallar_kitabı.md` oku!

---

## 📁 DOSYA ORGANIZASYONU KURALI

**UYARI:** Her YZ session ana dizine test dosyası bırakıyor. Bu kabul edilemez!

**Kurallar:**
1. ❌ Ana dizine test dosyası BIRAKMA (test.mlp, test_*.mlp, *.s, *.o)
2. ✅ Tüm testler `tests/manual/` veya `tests/auto/` klasörüne
3. ✅ Geçici dosyalar (assembly, binary) `temp/` klasörüne
4. ✅ Session bitiminde: Ana dizini temizle!

**Doğru Yapı:**
```
tests/
├── manual/
│   ├── test_parser_expr.mlp      ← Manuel testler
│   ├── test_parser_stmt.mlp
│   └── test_*.mlp
├── auto/
│   └── test_suite.mlp            ← Otomatik test suite
└── output/
    ├── parser_expr.s             ← Test çıktıları
    └── *.o, binary files

temp/
├── çıktı.md                      ← Çalışma notları
├── working_*.s                    ← Geçici assembly
└── debug_*                        ← Debug dosyaları
```

**Kötü Örnek (YAPMA!):**
```
MLP/                               ← Ana dizin
├── test.mlp                       ❌ YANLIŞ!
├── test_simple.mlp                ❌ YANLIŞ!
├── parser_expr.s                  ❌ YANLIŞ!
├── parser_expr.s.o                ❌ YANLIŞ!
└── a.out                          ❌ YANLIŞ!
```

---

## 🎉 Stage 0 MVC Tamamlandı!

**Durum:** ✅ COMPLETE (YZ_97 - 15 Aralık 2025)

- **Compiler Kodu:** 34,432 satır (C)
- **Runtime Kodu:** 4,584 satır (C)
- **Toplam Modül:** 33 modül
- **Test Geçme Oranı:** ~90%

**Özellikler:**
- ✅ Variables, Functions, Control Flow
- ✅ Loops (while, for with from/to/downto)
- ✅ Structs (definition, instances, methods)
- ✅ Switch/Case, Enums
- ✅ Arrays, Strings
- ✅ Operators (arithmetic, logical, comparison)

**Not:** Stage 0 artık feature-frozen. Yeni özellikler Stage 1'de MELP ile eklenecek!

---

## 🚀 STAGE 1 HEDEF: Self-Hosting (MELP ile MELP Compiler)

### 📊 Mevcut Self-Hosting Durumu

| Bileşen | Durum | Dil | Satır | Oturum |
|---------|-------|-----|-------|--------|
| **Lexer** | ✅ %100 | MELP | ~1,803 | Stage 0 (YZ_57) |
| **Parser Infrastructure** | ✅ %100 | MELP | ~400 | Stage 0 (YZ_97) |
| **Parser Expressions** | ✅ %100 | MELP | 224 | **Stage 1 (YZ_01) ✅** |
| **Parser Statements** | ✅ %100 | MELP | 103 | **Stage 1 (YZ_02) ✅** |
| **Parser Control Flow** | ✅ %100 | MELP | 287 | **Stage 1 (YZ_03) ✅** |
| **Parser Functions** | ✅ %100 | MELP | 518 | **Stage 1 (YZ_04) ✅** |
| **Parser Structs/Enums** | ✅ %100 | MELP | 811 | **Stage 1 (YZ_05) ✅** |
| **Parser Import/Module** | ✅ %100 | MELP | 219 | **Stage 1 (YZ_06) ✅** |
| **Parser Switch/Case** | ✅ %100 | MELP | 332 | **Stage 1 (YZ_07) ✅** |
| **Parser For Loops** | ✅ %100 | MELP | 299 | **Stage 1 (YZ_08) ✅** |
| **Parser Function Calls** | ✅ %100 | MELP | 327 | **Stage 1 (YZ_08) ✅** |
| **Parser Array Indexing** | ✅ %100 | MELP | 266 | **Stage 1 (YZ_08) ✅** |
| **Parser Compound Assign** | ✅ %100 | MELP | 227 | **Stage 1 (YZ_08) ✅** |
| **Parser Integration** | ✅ %100 | MELP | 583 | **Stage 1 (YZ_06/07/08) ✅** |
| **CodeGen Infrastructure** | ✅ %100 | MELP | 942 | **Stage 1 (YZ_09) ✅** |
| **CodeGen Literals & Vars** | ✅ %100 | MELP | 776 | **Stage 1 (YZ_10) ✅** |
| **CodeGen Arithmetic** | ❌ %0 | MELP | - | Stage 1 (YZ_11) |
| **CodeGen Comparison/Logic** | ❌ %0 | MELP | - | Stage 1 (YZ_12) |
| **CodeGen Statements** | ❌ %0 | MELP | - | Stage 1 (YZ_13-15) |
| **CodeGen Functions** | ❌ %0 | MELP | - | Stage 1 (YZ_16) |
| **CodeGen Structs/Arrays** | ❌ %0 | MELP | - | Stage 1 (YZ_17-18) |
| **CodeGen Integration** | ❌ %0 | MELP | - | Stage 1 (YZ_19) |
| **Bootstrap** | ❌ %0 | - | - | Stage 1 (YZ_20+) |

**İlerleme:** Stage 1 Phase 2 - %20 Complete (CodeGen in progress)
**Sırada:** Stage 1 Phase 2 - CodeGen Arithmetic (YZ_11)

---

## 🎯 ŞU AN: Stage 1 Phase 1 - Parser in MELP

### Phase 1: Parser Self-Hosting (Tahmini: 4-6 hafta)

**Hedef:** Parser'ı MELP dilinde yaz, Stage 0 compiler ile derle

**Neden Kritik:**
- Parser olmadan AST üretemeyiz
- AST olmadan code generation yapamayız
- Self-hosting'in ikinci kritik adımı

**✅ TAMAMLANDI:**
- ✅ **YZ_01** - Parser Expressions (224 satır) - Arithmetic, literals, precedence
- ✅ **YZ_02** - Parser Statements (103 satır) - Variable decl, assignment, return, print
- ✅ **YZ_03** - Parser Control Flow (287 satır) - If/else/while parsing
- ✅ **YZ_04** - Parser Functions (518 satır) - Function declarations, parameters, body
- ✅ **YZ_05** - Parser Structs/Enums (528 satır) - Struct/enum definitions, fields, values

**⏸️ SONRAKI GÖREVLER (Kullanıcı seçecek):**
- ⏳ Parser Integration - All parsers working together
- ⏳ Parser Module imports - Import/module system
- ⏳ Switch/Case statements parsing
- ⏳ For loops parsing (from/to/downto)

#### Part 1: Parser Infrastructure (1 hafta) ✅ COMPLETE
- [x] AST Node structures (struct kullanarak)
  - [x] `ASTNode` base struct
  - [x] `ExpressionNode`, `StatementNode`, `FunctionNode`
  - [x] Node type enums
- [x] Token stream management
  - [x] `current_token()`, `peek_token()`, `advance()`
  - [x] Lookahead implementation
- [x] Error handling & recovery
  - [x] Error reporting functions
  - [x] Panic mode recovery

**Dosyalar:** `modules/parser_mlp/ast_nodes.mlp`, `token_stream.mlp`, `parser_errors.mlp`

#### Part 2: Expression Parsing (1-2 hafta) ✅ COMPLETE (YZ_01)
- [x] Primary expressions
  - [x] Literals (numeric, string, boolean)
  - [x] Identifiers
  - [x] Parenthesized expressions
- [x] Arithmetic expressions
  - [x] Addition, subtraction (+, -)
  - [x] Multiplication, division (*, /, %)
  - [x] Operator precedence (simple precedence climbing)
- [x] Boolean expressions
  - [x] Comparison operators (==, !=, <, >, <=, >=)
  - [x] Logical operators (and, or, not)
- [ ] Function calls (deferred to later)
  - [ ] Call expression parsing
  - [ ] Argument list parsing
- [x] Array/List access ✅ YZ_08
  - [x] Index expressions (arr[i])
  - [x] Multi-dimensional arrays (matrix[i][j])

**Dosya:** `modules/parser_mlp/parser_expr.mlp` (224 satır)

#### Part 3: Statement Parsing (1-2 hafta) ✅ COMPLETE
- [x] Variable declarations (YZ_02) ✅
  - [x] `numeric x = 10`
  - [x] `string name = "test"`
  - [x] Type annotations
- [x] Assignment statements (YZ_02) ✅
  - [x] Simple assignment (x = 5)
  - [ ] Compound assignment (+=, -=, etc.) - deferred
- [x] Control flow statements (YZ_03) ✅
  - [x] If/else_if/else
  - [x] While loops
  - [ ] For loops (from/to/downto) - deferred
- [ ] Jump statements
  - [x] Return statements (YZ_02) ✅
  - [ ] Exit/Continue - deferred
- [x] Print statements (YZ_02) ✅

**Dosyalar:** `modules/parser_mlp/parser_stmt.mlp` (103 satır), `parser_control.mlp` (287 satır)

#### Part 4: Advanced Parsing (1 hafta) ✅ Tamamlandı (Temel kısmı)
- [x] Function declarations (YZ_04) ✅
  - [x] Function signature
  - [x] Parameters
  - [x] Return type
  - [x] Function body
- [x] Struct definitions (YZ_05) ✅
  - [x] Struct fields
  - [x] Multiple field types
  - [ ] Nested structs (deferred)
  - [ ] Methods (deferred)
- [x] Enum definitions (YZ_05) ✅
  - [x] Enum values (name = number)
- [x] Module imports (YZ_06) ✅
  - [x] Import statement parsing
  - [x] Include statement parsing
  - [x] From...import syntax
  - [x] Multiple imports
- [x] Switch/Case statements (YZ_07) ✅
  - [x] Switch expression parsing
  - [x] Case clauses with colon
  - [x] Default case (optional)
  - [x] Multiple cases
  - [x] end switch terminator

**Dosyalar:**
- `modules/parser_mlp/parser_func.mlp` (518 satır, YZ_04) ✅
- `modules/parser_mlp/parser_struct.mlp` (246 satır, YZ_05) ✅
- `modules/parser_mlp/parser_enum.mlp` (283 satır, YZ_05) ✅
- `modules/parser_mlp/parser_import.mlp` (219 satır, YZ_06) ✅
- `modules/parser_mlp/parser_switch.mlp` (332 satır, YZ_07) ✅

#### Part 5: Integration & Testing (1 hafta) ✅ COMPLETE (YZ_06)
- [x] Lexer + Parser entegrasyonu
- [x] Integration test suite
- [x] Comprehensive tests
  - [x] Expression parsing tests
  - [x] Statement parsing tests
  - [x] Control flow parsing tests
  - [x] Function parsing tests
  - [x] Struct/Enum parsing tests
  - [x] Import parsing tests

**Dosya:**
- `modules/parser_mlp/parser_integration.mlp` (428 satır, YZ_06/07) ✅ (6 tests)

---

## 🔮 Stage 1 Sonraki Faz'lar

### Phase 2: CodeGen in MELP (8-10 hafta, 8-10 YZ sessions)
**Hedef:** LLVM IR code generator'ı MELP'te yaz

**Strateji:** Küçük, test edilebilir adımlarla ilerle. Her Part bir YZ session.

#### Part 1: CodeGen Infrastructure (1 hafta - YZ_09) ✅ COMPLETE
**Hedef:** LLVM IR emission altyapısı kur

- [x] IR Builder temel fonksiyonlar
  - [x] `emit_header()` - LLVM module başlığı
  - [x] `emit_function_decl()` - Function declaration
  - [x] `emit_basic_block()` - Basic block creation
  - [x] `emit_instruction()` - Generic instruction emission
- [x] Type mapping (MELP → LLVM types)
  - [x] `numeric` → `i64`
  - [x] `string` → `i8*`
  - [x] `boolean` → `i1`
  - [x] `list` → `i8*` (pointer)
- [x] Symbol table management
  - [x] Local variables tracking
  - [x] Function signatures
  - [x] Scope management
- [x] Basic validation & testing
  - [x] Test IR header emission
  - [x] Test type mapping
  - [x] Test symbol table operations

**Dosyalar:**
- `modules/codegen_mlp/ir_builder.mlp` (~300 satır)
- `modules/codegen_mlp/type_mapper.mlp` (~200 satır)
- `modules/codegen_mlp/symbol_table.mlp` (~250 satır)

**Test:** Simple function declaration → LLVM IR

#### Part 2: Expression CodeGen - Literals & Variables (1 hafta - YZ_10) ✅ COMPLETE
**Hedef:** Basit expression'ları LLVM IR'e çevir

- [x] Literal expressions
  - [x] Numeric literals (`42` → `i64 42`)
  - [x] String literals (`"hello"` → global string)
  - [x] Boolean literals (`true/false` → `i1 1/0`)
- [x] Variable references
  - [x] Load local variable (`%x` → `load i64, i64* %x`)
  - [x] Store to variable (`x = 5` → `store i64 5, i64* %x`)
- [x] Assignment statements (simple and compound)
- [x] Simple tests
  - [x] `numeric x = 42` → IR
  - [x] `string s = "test"` → IR
  - [x] Variable load/store test
  - [x] 16 tests total (10 unit + 6 integration)

**Dosyalar:**
- `modules/codegen_mlp/codegen_literal.mlp` (380 satır) ✅
- `modules/codegen_mlp/codegen_variable.mlp` (396 satır) ✅
- `tests/manual/test_codegen_literals_vars.mlp` (303 satır) ✅
- `tests/manual/test_yz_10_unit.mlp` (68 satır) ✅
- `tests/manual/test_yz_10_e2e.mlp` (113 satır) ✅

**Test:** Variable declaration & assignment → LLVM IR ✅ (16 tests passing)

#### Part 3: Expression CodeGen - Arithmetic (1 hafta - YZ_11)
**Hedef:** Arithmetic operations

- [ ] Binary arithmetic operators
  - [ ] Addition (`+` → `add i64`)
  - [ ] Subtraction (`-` → `sub i64`)
  - [ ] Multiplication (`*` → `mul i64`)
  - [ ] Division (`/` → `sdiv i64`)
  - [ ] Modulo (`%` → `srem i64`)
- [ ] Operator precedence handling
  - [ ] Expression tree traversal
  - [ ] Temporary register allocation (`%t1, %t2, ...`)
- [ ] Tests
  - [ ] `2 + 3` → IR
  - [ ] `(2 + 3) * 4` → IR (precedence)
  - [ ] Complex expression test

**Dosyalar:**
- `modules/codegen_mlp/codegen_arithmetic.mlp` (~350 satır)

**Test:** Arithmetic expression → IR + execution + result verification

#### Part 4: Expression CodeGen - Comparison & Logic (1 hafta - YZ_12)
**Hedef:** Boolean operations

- [ ] Comparison operators
  - [ ] `==, !=` → `icmp eq/ne`
  - [ ] `<, >, <=, >=` → `icmp slt/sgt/sle/sge`
- [ ] Logical operators
  - [ ] `and` → `and i1`
  - [ ] `or` → `or i1`
  - [ ] `not` → `xor i1 %x, 1`
- [ ] Tests
  - [ ] `x == 5` → IR
  - [ ] `x > 0 and y < 10` → IR
  - [ ] Boolean expression test

**Dosyalar:**
- `modules/codegen_mlp/codegen_comparison.mlp` (~300 satır)
- `modules/codegen_mlp/codegen_logical.mlp` (~250 satır)

**Test:** Boolean expressions → IR + execution

#### Part 5: Statement CodeGen - Basic Statements (1 hafta - YZ_13)
**Hedef:** Temel statement'lar

- [ ] Variable declarations
  - [ ] Local variable allocation (`alloca`)
  - [ ] Initialization
- [ ] Assignment statements
  - [ ] Simple assignment (`x = expr`)
  - [ ] Compound assignment (`x += 5`)
- [ ] Print statements
  - [ ] `println()` → `printf` call
  - [ ] String formatting
- [ ] Return statements
  - [ ] `return expr` → `ret i64 %value`
- [ ] Tests
  - [ ] Variable decl → IR
  - [ ] Print test → IR + output
  - [ ] Return test → IR

**Dosyalar:**
- `modules/codegen_mlp/codegen_stmt.mlp` (~400 satır)

**Test:** Simple program with variables, print, return

#### Part 6: Control Flow - If/Else (1 hafta - YZ_14)
**Hedef:** Conditional branching

- [ ] If statement
  - [ ] Condition evaluation
  - [ ] Branch creation (`br i1 %cond, label %then, label %else`)
  - [ ] Basic block management
- [ ] Else/Else-if
  - [ ] Multiple branches
  - [ ] Phi nodes (if needed)
- [ ] Tests
  - [ ] `if x > 0 then ... end if` → IR
  - [ ] `if-else` → IR + execution
  - [ ] Nested if test

**Dosyalar:**
- `modules/codegen_mlp/codegen_if.mlp` (~350 satır)

**Test:** If/else program → IR + correct branch execution

#### Part 7: Control Flow - Loops (1 hafta - YZ_15)
**Hedef:** Loop structures

- [ ] While loop
  - [ ] Loop condition block
  - [ ] Loop body block
  - [ ] Back edge (`br label %loop_cond`)
  - [ ] Exit block
- [ ] For loop
  - [ ] Loop counter initialization
  - [ ] Condition check
  - [ ] Increment/decrement
  - [ ] from/to/downto variants
- [ ] Tests
  - [ ] While loop → IR
  - [ ] For loop → IR
  - [ ] Nested loop test

**Dosyalar:**
- `modules/codegen_mlp/codegen_while.mlp` (~300 satır)
- `modules/codegen_mlp/codegen_for.mlp` (~350 satır)

**Test:** Loop programs → IR + correct iteration count

#### Part 8: Functions (1 hafta - YZ_16)
**Hedef:** Function definitions & calls

- [ ] Function definition
  - [ ] Parameter handling
  - [ ] Function prologue/epilogue
  - [ ] Return value
- [ ] Function calls
  - [ ] Argument passing
  - [ ] Call instruction (`call i64 @func(i64 %arg)`)
  - [ ] Return value capture
- [ ] Tests
  - [ ] Simple function → IR
  - [ ] Function with params → IR
  - [ ] Recursive function test

**Dosyalar:**
- `modules/codegen_mlp/codegen_function.mlp` (~450 satır)

**Test:** Function definition + call → IR + execution

#### Part 9: Structs & Arrays (1-2 hafta - YZ_17/18)
**Hedef:** Composite types

- [ ] Struct handling
  - [ ] Struct type definition (`%struct_t = type { i64, i8* }`)
  - [ ] Field access (`getelementptr`)
  - [ ] Struct allocation & initialization
- [ ] Array/List handling
  - [ ] Array allocation
  - [ ] Element access (indexing)
  - [ ] Bounds checking (optional)
- [ ] Tests
  - [ ] Struct definition + access → IR
  - [ ] Array indexing → IR
  - [ ] Complex data structure test

**Dosyalar:**
- `modules/codegen_mlp/codegen_struct.mlp` (~400 satır)
- `modules/codegen_mlp/codegen_array.mlp` (~350 satır)

**Test:** Struct & array programs → IR + execution

#### Part 10: Integration & Optimization (1 hafta - YZ_19)
**Hedef:** Tüm parçaları birleştir

- [ ] Full pipeline
  - [ ] Lexer → Parser → CodeGen
  - [ ] End-to-end compilation
- [ ] Basic optimizations
  - [ ] Dead code elimination
  - [ ] Constant folding
  - [ ] Register allocation hints
- [ ] Comprehensive testing
  - [ ] Full program suite
  - [ ] Performance benchmarks
  - [ ] Edge cases
- [ ] Documentation
  - [ ] CodeGen architecture doc
  - [ ] API reference
  - [ ] Examples

**Dosyalar:**
- `modules/codegen_mlp/codegen_main.mlp` (~500 satır)
- `modules/codegen_mlp/optimizer.mlp` (~300 satır)

**Test:** Complex programs → IR → executable → correct output

**TOPLAM CodeGen Satır Tahmini:** ~4,500-5,000 satır MELP kodu
**TOPLAM YZ Session:** 10-11 sessions (YZ_09 - YZ_19)
**TOPLAM Süre:** 8-10 hafta

---

### Phase 3: Bootstrap Complete (2-3 hafta, 2-3 YZ sessions)
**Hedef:** MELP compiler'ı MELP ile derle!

#### Part 1: Self-Compilation Setup (1 hafta - YZ_20)
**Hedef:** Stage 1 compiler'ı Stage 0 ile derle

- [ ] Full compiler integration
  - [ ] Lexer + Parser + CodeGen pipeline
  - [ ] Module linking
  - [ ] Runtime library integration
- [ ] Build system
  - [ ] Compilation scripts
  - [ ] Dependency management
  - [ ] Output validation
- [ ] Test
  - [ ] Compile Stage 1 with Stage 0
  - [ ] Test executable
  - [ ] Benchmark performance

**Komut:**
```bash
# Stage 0 (C) ile Stage 1 (MELP) derle
stage0/melpc stage1/*.mlp -o melp_stage1

# Test: Simple program compile
./melp_stage1 examples/hello.mlp -o hello
./hello  # "Hello World" çıktısı
```

**Dosyalar:**
- `modules/compiler_main.mlp` (~400 satır)
- `scripts/build_stage1.sh` (build script)

**Tahmini:** 1 hafta, ~400 satır

#### Part 2: Self-Hosting Verification (1 hafta - YZ_21)
**Hedef:** Stage 1 kendini derlesin!

- [ ] Self-compilation test
  - [ ] melp_stage1 compile melp_stage1 source
  - [ ] Output: melp_stage1_v2
- [ ] Binary verification
  - [ ] Compare melp_stage1 vs melp_stage1_v2
  - [ ] Functionality tests
  - [ ] Edge case testing
- [ ] Bootstrap loop
  - [ ] v2 compile v3, v3 compile v4, ...
  - [ ] Convergence check (v2 == v3 == v4)

**Komut:**
```bash
# Step 1: Stage 0 → Stage 1
stage0/melpc stage1/*.mlp -o melp_v1

# Step 2: Stage 1 → Stage 1 (self-compile!)
./melp_v1 stage1/*.mlp -o melp_v2

# Step 3: Verify
diff melp_v1 melp_v2  # Ideally identical or functionally equivalent

# Step 4: Bootstrap loop
./melp_v2 stage1/*.mlp -o melp_v3
diff melp_v2 melp_v3  # Should be identical!
```

**Success Criteria:**
- ✅ melp_v2 can compile itself
- ✅ melp_v2 == melp_v3 (convergence)
- ✅ All tests pass with melp_v2
- ✅ Performance acceptable

**Tahmini:** 1 hafta

#### Part 3: Performance & Cleanup (1 hafta - YZ_22)
**Hedef:** Optimize & finalize

- [ ] Performance benchmarks
  - [ ] Compilation speed tests
  - [ ] Memory usage profiling
  - [ ] Compare vs Stage 0
- [ ] Optimization pass
  - [ ] Bottleneck identification
  - [ ] Critical path optimization
  - [ ] Memory optimization
- [ ] Documentation
  - [ ] Self-hosting guide
  - [ ] Architecture documentation
  - [ ] Migration guide (Stage 0 → Stage 1)
- [ ] Cleanup
  - [ ] Remove debug code
  - [ ] Code review
  - [ ] Final testing

**Deliverables:**
- 📊 Performance report
- 📚 Complete documentation
- ✅ 95%+ test coverage
- 🎉 **SELF-HOSTING ACHIEVED!**

**Tahmini:** 1 hafta

**TOPLAM Bootstrap:** 2-3 hafta, 3 YZ sessions (YZ_20-22)

---

## 📅 Güncellenmiş Timeline

**Stage 1 Toplam Tahmini:**

| Phase | YZ Sessions | Süre | Satır |
|-------|-------------|------|-------|
| Parser (Phase 1) | YZ_01 - YZ_08 | ✅ 4 hafta | ~6,686 |
| CodeGen (Phase 2) | YZ_09 - YZ_19 | ⏳ 8-10 hafta | ~4,500-5,000 |
| Bootstrap (Phase 3) | YZ_20 - YZ_22 | ⏳ 2-3 hafta | ~1,000 |
| **TOPLAM** | **22 sessions** | **14-17 hafta** | **~12,000 satır** |

**Hedef Tarihler:**
- ✅ 16 Aralık 2025: Parser complete (YZ_08)
- 🎯 15 Şubat 2026: CodeGen complete (YZ_19)
- 🎯 8 Mart 2026: Bootstrap complete (YZ_22)
- 🎉 **15 Mart 2026: SELF-HOSTING ACHIEVED!**

**Haftalık İlerleme Hedefi:** 1 YZ session/hafta (ortalama)

---

## 🔧 Stage 1.5 - Advanced Features (Sonrası)

Self-hosting tamamlandıktan sonra, MELP ile yeni özellikler eklenecek:

- [ ] Try-Catch Exception Handling
- [ ] Type Inference (var keyword)
- [ ] Generics/Templates
- [ ] Lambda/Closures
- [ ] Advanced OOP features
- [ ] Pattern Matching
- [ ] Module system improvements

**Mantık:** Bu özellikleri sadece BİR KERE yazmak (MELP ile), iki kere değil!

---

## 🤖 YZ WORKFLOW SİSTEMİ

**Her YZ session için standart süreç:**

### 1️⃣ Session Başlangıcı (İlk 5 dakika)

```bash
# Adım 1: TODO.md oku (tamamlananlar ve bekleyenler)
cat TODO.md | grep "✅\|⏸️" | head -20

# Adım 2: NEXT_AI_START_HERE.md oku
cat NEXT_AI_START_HERE.md | head -50

# Adım 3: Son YZ raporunu oku
ls -t stage_1_YZ/YZ_*.md | head -1 | xargs cat | head -30
```

**Görev Belirleme:**
- ❌ Otonom karar VERME!
- ✅ Kullanıcıya sor: "Ne yapmamı istersin?"
- ✅ TODO.md'deki seçenekleri sun
- ✅ Kullanıcı seçtikten sonra başla

### 2️⃣ Session Çalışması (Ana süre)

**Çalışma Prensibi:**
- Görevi tamamla (kod yaz, test et, debug yap)
- Küçük adımlarla ilerle, her adımı test et
- temp/çıktı.md'ye notlar al
- Test dosyalarını tests/manual/ veya temp/'e koy

### 3️⃣ Session Bitişi (Son 15 dakika) - ÇOK ÖNEMLİ!

**Görev tamamlandıysa:**

```bash
# Adım 1: YZ raporu yaz
# Dosya: stage_1_YZ/YZ_XX.md
# İçerik: Tarih, görev, değişiklikler, testler, sonuçlar
# ÖNEMLİ: "Next Session" bölümü YOK! Sadece özet.

# Adım 2: TODO.md güncelle
# - Tamamlanan task'ı [x] işaretle
# - İlerleme yüzdesini güncelle (%50 → %60)
# - Diğer görevler ⏸️ (Bekliyor) olarak kalsın

# Adım 3: NEXT_AI_START_HERE.md güncelle
# - "Last Session: YZ_XX" olarak güncelle
# - Status: "YZ_XX Complete - Ask user for next task"
# - Tablo güncelle (tamamlanan ✅)
# - "KULLANICIYA SORU" bölümünde TODO seçeneklerini listele

# Adım 4: Ana dizini temizle
rm -f test_*.mlp *.s *.o a.out
rm -f temp/*.s*
mv temp/*.s temp/archive/ 2>/dev/null || true

# Adım 5: Git commit & push
# Yeni dal oluştur (Format: task-description_YZ_XX)
# Örnek: parser-enhancements_YZ_08, codegen-phase1_YZ_09
git checkout -b task-description_YZ_XX

# Değişiklikleri ekle
git add modules/parser_mlp/*.mlp stage_1_YZ/YZ_*.md TODO.md NEXT_AI_START_HERE.md

# Commit et (detaylı mesajla)
git commit -m "YZ_XX: Task Description

✅ Completed Tasks:
- Task 1 description
- Task 2 description

📊 Metrics:
- Files created: X
- Lines added: Y  
- Tests: Z/Z passing

Status: Complete/Ready for review"

# Push et
git push -u origin task-description_YZ_XX

# ⚠️ ÖNEMLI: Dal adı formatı: task-description_YZ_XX
# Görev adı + YZ numarası (örn: parser-for-loops_YZ_08)

# Adım 6: KULLANICIYA SOR
# "Görev YZ_XX tamamlandı ve commit edildi! Sırada ne yapmak istersin?"
# "Pull request oluşturalım mı?"
# TODO.md'deki seçenekleri sun
```

**Görev tamamlanmadıysa (yarım kaldıysa):**

```bash
# Adım 1: YZ raporuna PARTIAL ekle
# Dosya: stage_1_YZ/YZ_XX.md
# Status: "🏗️ IN PROGRESS - XX% Complete"
# Not: Nerede kaldığını açıkla

# Adım 2: TODO.md'ye not ekle
# Görevin yanına: "⏸️ Kaldığımız yer: ..."

# Adım 3: NEXT_AI_START_HERE.md güncelle
# Status: "YZ_XX in progress - XX% done"

# Adım 4: temp/çıktı.md'yi koru (silinmesin!)

# Adım 5: KULLANICIYA SOR
# "Görev yarım kaldı. Devam etmek ister misin yoksa başka bir şey mi yapalım?"
```

### 4️⃣ YZ Geçiş Kontrolü (Sonraki YZ için)

**Yeni YZ başlarken:**

```bash
# Kontrol 1: TODO.md güncel mi?
cat TODO.md | grep "✅" | tail -5  # Son tamamlananlar

# Kontrol 2: NEXT_AI_START_HERE.md oku
head -30 NEXT_AI_START_HERE.md

# Kontrol 3: Son YZ raporu var mı?
ls -t stage_1_YZ/YZ_*.md | head -1

# Kontrol 4: Ana dizin temiz mi?
ls test_*.mlp *.s *.o 2>/dev/null && echo "⚠️ TEMIZLIK GEREKLI!" || echo "✅ Temiz"
```

**YZ'nin İLK YAPACAĞI:**
1. ✅ NEXT_AI_START_HERE.md + TODO.md oku
2. ✅ Kullanıcıya sor: "Merhaba! Ne yapmamı istersin?"
3. ✅ TODO.md'deki seçenekleri sun
4. ❌ ASLA otonom görev atama!

---

## ⚠️ ÖNEMLI KURALLAR

### Kural 0: YZ Otonom Görev Atamaz! 🚫
**ZORUNLU:** Her YZ kullanıcıya sormalı!

#### YZ'nin YAPABİLECEKLERİ:
- ✅ Görev tamamlandığında: "Görev bitti! Sırada ne yapmak istersin?"
- ✅ TODO.md'deki seçenekleri sunmak
- ✅ Öneri yapmak: "Şunlardan birini yapabilirim: A, B, C"
- ✅ Kullanıcının kararını beklemek

#### YZ'nin YAPAMAYACAKLARİ:
- ❌ "Şimdi YZ_05'e başlıyorum" demek
- ❌ NEXT_AI_START_HERE.md'de "NEXT TASK: YZ_05" yazmak
- ❌ stage_1_YZ/YZ_05.md dosyası otomatik oluşturmak
- ❌ TODO.md'de bir görevi "🏃 CURRENT" işaretlemek

**Neden?** YZ'ler çok hızlı ilerliyor, kullanıcı kontrol kaybediyor!

### Kural 1: Test ve Geçici Dosyalar
**ZORUNLU:** Ana dizin temiz tutulmalı!

#### Test Dosyaları:
- ✅ Doğru: `tests/manual/test_parser.mlp`
- ✅ Doğru: `tests/e2e/test_integration.mlp`
- ❌ Yanlış: Ana dizine test dosyası (`test_*.mlp`)

#### Geçici Dosyalar:
- ✅ Doğru: `temp/experiment_*.mlp`
- ✅ Doğru: `.gitignore`'a ekle: `test_*.mlp`, `*.s`, `*.o`
- ❌ Yanlış: Ana dizinde `test_list.mlp`, `test_minimal.mlp` vs.

#### Cleanup:
```bash
# Ana dizindeki test dosyalarını düzenli temizle
rm -f test_*.mlp test_*.s test_*.o
mv test_*.mlp tests/manual/  # Veya temp/ dizinine
```

### Kural 2: YZ Rapor Dosyaları
**ZORUNLU:** Her YZ oturumu sonunda `YZ/` dizinine rapor!

- ✅ Format: `YZ/YZ_XX.md`
- ✅ İçerik: Tarih, değişiklikler, testler, sonuçlar

### Kural 3: Stage 0 Feature-Frozen
**ÖNEMLI:** Stage 0'a artık yeni özellik eklenmeyecek!

- ✅ Bug fix: İzin veriliyor
- ✅ Performance fix: İzin veriliyor
- ❌ Yeni özellik: Stage 1'de MELP ile eklenecek

---

## 📊 İlerleme Takibi

**Stage 0:** ████████████████████ 100% ✅  
**Stage 1:** █████░░░░░░░░░░░░░░░  25% (Lexer done)  
**Self-Hosting:** ░░░░░░░░░░░░░░░░░░░░   0%

---

## 🎯 Bir Sonraki Adım

**ŞİMDİ BAŞLA:** Stage 1 Phase 1 Part 1 - Parser Infrastructure

1. `modules/parser_mlp/` dizini oluştur
2. AST Node structures tasarla (struct kullanarak)
3. Token stream management implement et
4. Temel error handling ekle

**Tahmini Süre:** ~8-10 saat (1 hafta)

---

**Son Güncelleme:** 15 Aralık 2025  
**Durum:** Stage 0 Complete ✅ → Stage 1 Starting 🚀
