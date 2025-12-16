# 🚀 STAGE 1 SELF-HOSTING - START HERE!

**Last Session:** Stage 1 YZ_19  
**Date:** 17 December 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Branch:** codegen-integration_YZ_19  
**Status:** ✅ **YZ_19 Complete - CodeGen Phase COMPLETE! Ask user for Bootstrap (YZ_20+)**

**⚠️ YZ Directory Structure:**
- `stage_0_YZ/` - Stage 0 sessions (YZ_01 - YZ_97, 80 sessions) ✅ ARCHIVED
- `stage_1_YZ/` - Stage 1 sessions (YZ_01+, current) ⏳ ACTIVE

---

## 🤖 YZ HIZLI BAŞLANGIÇ (İLK OKUYACAĞIN!)

**📋 DETAYLI CHECKLIST İÇİN:** `YZ_CHECKLIST.md` dosyasını oku!

**Yeni YZ geldiğinde yapılacaklar (5 dakika):**

1. **Bu dosyayı oku** → Mevcut durum nedir?
2. **TODO.md oku** → Ne yapıldı, ne kaldı?
3. **Son YZ raporunu oku** → stage_1_YZ/YZ_XX.md (en son tamamlanan)
4. **KULLANICIYA SOR** → "Ne yapmamı istersin?" (TODO'dan seç veya yeni görev)

**Session bittiğinde yapılacaklar (15 dakika):**

1. **YZ raporu yaz** → `stage_1_YZ/YZ_XX.md` (görev, değişiklikler, testler, sonuçlar)
2. **TODO.md güncelle** → Tamamlananı [x] işaretle, ilerleme %'sini güncelle
3. **Bu dosyayı güncelle** → Status, tablo ("Last Session" olarak)
4. **Ana dizini temizle** → `rm -f test_*.mlp *.s *.o temp/*.s*`
5. **Git commit & push** → Değişiklikleri kendi dalına commit et ve push et
6. **KULLANICIYA SOR** → "Görev tamamlandı. Sırada ne yapmak istersin?"

**🔧 Git Workflow:**
```bash
# 1. Yeni dal oluştur (görev adı + YZ numarası)
# Format: task-description_YZ_XX
# Örnek: parser-enhancements_YZ_08, codegen-phase1_YZ_09
git checkout -b task-description_YZ_XX

# 2. Değişiklikleri ekle
git add modules/parser_mlp/*.mlp stage_1_YZ/YZ_*.md TODO.md NEXT_AI_START_HERE.md

# 3. Commit et (detaylı mesajla)
git commit -m "YZ_XX: Task Description

✅ Completed Tasks:
- Task 1
- Task 2

📊 Metrics:
- Files created: X
- Lines added: Y
- Tests: Z/Z passing

Status: Complete/Ready for review"

# 4. Push et
git push -u origin task-description_YZ_XX

# 5. KULLANICIYA SOR: Pull request oluşturalım mı?
```

**📌 Dal İsimlendirme Kuralı:**
- Format: `task-description_YZ_XX`
- Görev adı kısa ve açıklayıcı (kebab-case)
- Sonunda mutlaka `_YZ_XX` eki (XX = session numarası)
- Örnekler:
  - `parser-enhancements_YZ_08`
  - `codegen-llvm_YZ_09`
  - `array-indexing_YZ_10`
  - `ast-optimization_YZ_11`

**⚠️ ÖNEMLİ:** Session bittiğinde mutlaka TODO.md + NEXT_AI_START_HERE.md + YZ raporu + GIT COMMIT yapılmalı!

---

## 🚨 SYNTAX UYARISI - İLK OKUYACAĞIN ŞEY!

**Kullanıcı kodu (.mlp test/örnek):** Boşluklu → `end if`, `else if`, `end while`  
**Compiler kodu (parser.mlp, lexer.mlp):** Alt çizgili → `end_if`, `else_if`, `end_while`

**Hızlı Kural:** Örnekler için `end if`, modüller için `end_if`. Kararsızsan → `kurallar_kitabı.md` oku!

---

## 📁 DOSYA ORGANIZASYONU KURALI - ÖNEMLİ!

**Test Dosyaları:**
- ❌ **ANA DİZİNE TEST DOSYASI BIRAKMA!** (test.mlp, test_*.mlp, parser_expr.s vb.)
- ✅ Tüm test dosyaları `tests/` klasörüne konulmalı
- ✅ Manuel testler: `tests/manual/test_*.mlp`
- ✅ Otomatik testler: `tests/auto/`
- ✅ Test çıktıları: `tests/output/` veya `temp/`

**Geçici Dosyalar:**
- ✅ `.s`, `.o`, binary dosyalar → `temp/` klasörüne
- ✅ Çalışma notları → `temp/çıktı.md` veya `temp/notes_*.md`

**Temizlik Sonrası Ana Dizin:**
```
MLP/
├── compiler/          (Stage 0 C compiler)
├── modules/           (Stage 1 MELP modules)
├── runtime/           (C runtime libraries)
├── stage_0_YZ/        (Archived sessions)
├── stage_1_YZ/        (Active sessions)
├── tests/             (TÜM TEST DOSYALARI BURAYA!)
├── temp/              (Geçici dosyalar)
├── NEXT_AI_START_HERE.md
├── TODO.md
└── README.md
```

**Her YZ Session Sonunda:** Ana dizini temizle, test dosyalarını `tests/` altına taşı!

---

## ✅ Stage 0 Tamamlandı - Artık Tarihe Karıştı!

**Stage 0 MVC (Minimum Viable Compiler) 15 Aralık 2025'te tamamlandı!**

- ✅ 34,432 satır compiler kodu (C)
- ✅ 4,584 satır runtime kodu (C)
- ✅ 33 modül
- ✅ ~90% test geçme oranı
- ✅ Tüm MVC özellikleri çalışıyor

**Arşivlendi:** 
- `temp/TODO_old_20251215_*.md`
- `temp/NEXT_AI_START_HERE_old_20251215_*.md`
- `temp/STAGE0_SIGNOFF_20251215.md`

**Stage 0 artık feature-frozen!** Sadece bug fix ve performance fix kabul ediliyor.

---

## 🎯 Stage 1 Hedef: Self-Hosting

**MELP compiler'ı MELP dilinde yaz!**

### Strateji: 3 Faz

```
┌────────────────┐
│  PHASE 1       │  ← ŞU AN BURDAYIZİ
│  Parser (MELP) │     4-6 hafta
└────────────────┘
        ↓
┌────────────────┐
│  PHASE 2       │
│  CodeGen(MELP) │     4-6 hafta
└────────────────┘
        ↓
┌────────────────┐
│  PHASE 3       │
│  Bootstrap     │     2 hafta
└────────────────┘
        ↓
    🎉 SELF-HOSTING COMPLETE!
```

---

## 📊 Self-Hosting İlerleme

| Bileşen | Durum | Dil | Satır | YZ |
|---------|-------|-----|-------|-----|
| Lexer | ✅ 100% | MELP | ~1,803 | Stage 0 (YZ_57) |
| Parser Infra | ✅ 100% | MELP | ~400 | Stage 0 (YZ_97) |
| Parser Expr | ✅ 100% | MELP | 224 | **Stage 1 (YZ_01) ✅** |
| Parser Stmt | ✅ 100% | MELP | 103 | **Stage 1 (YZ_02) ✅** |
| Parser Control | ✅ 100% | MELP | 287 | **Stage 1 (YZ_03) ✅** |
| Parser Func | ✅ 100% | MELP | 518 | **Stage 1 (YZ_04) ✅** |
| Parser Struct/Enum | ✅ 100% | MELP | 811 | **Stage 1 (YZ_05) ✅** |
| Parser Import | ✅ 100% | MELP | 219 | **Stage 1 (YZ_06) ✅** |
| Parser Switch/Case | ✅ 100% | MELP | 332 | **Stage 1 (YZ_07) ✅** |
| Parser For Loops | ✅ 100% | MELP | 299 | **Stage 1 (YZ_08) ✅** |
| Parser Function Calls | ✅ 100% | MELP | 327 | **Stage 1 (YZ_08) ✅** |
| Parser Array Indexing | ✅ 100% | MELP | 266 | **Stage 1 (YZ_08) ✅** |
| Parser Pretty Print | ✅ 100% | MELP | 336 | **Stage 1 (YZ_08) ✅** |
| Parser Compound Assign | ✅ 100% | MELP | 227 | **Stage 1 (YZ_08) ✅** |
| Parser Integration | ✅ 100% | MELP | 583 | **Stage 1 (YZ_06/07/08) ✅** |
| CodeGen Infrastructure | ✅ 100% | MELP | 942 | **Stage 1 (YZ_09) ✅** |
| CodeGen Literals & Vars | ✅ 100% | MELP | 776 | **Stage 1 (YZ_10) ✅** |
| CodeGen Arithmetic | ✅ 100% | MELP | 472 | **Stage 1 (YZ_11) ✅** |
| CodeGen Comparison/Logic | ✅ 100% | MELP | 412 | **Stage 1 (YZ_12) ✅** |
| CodeGen Statements | ✅ 100% | MELP | 145 | **Stage 1 (YZ_13) ✅** |
| CodeGen Control Flow | ✅ 100% | MELP | 220 | **Stage 1 (YZ_14) ✅** |
| CodeGen While Loops | ✅ 100% | MELP | 333 | **Stage 1 (YZ_15) ✅** |
| CodeGen For Loops | ✅ 100% | MELP | 473 | **Stage 1 (YZ_16) ✅** |
| CodeGen Functions | ✅ 100% | MELP | 518 | **Stage 1 (YZ_17) ✅** |
| CodeGen Arrays | ✅ 100% | MELP | 465 | **Stage 1 (YZ_18) ✅** |
| CodeGen Integration | ❌ 0% | MELP | - | Stage 1 (YZ_19 - Next) |
| Bootstrap | ❌ 0% | - | - | Stage 1 (Final) |

**TOPLAM:** %68 Complete (Parser + CodeGen Arrays)

**✅ YZ_17 COMPLETE - Function CodeGen Ready!**

**YZ_17 UPDATE (17 December 2025):**

**Completed (Function CodeGen - YZ_17):**
- ✅ Function declarations (no params, 1 param, 2 params, 3 params)
- ✅ Function parameter handling (alloca + store pattern)
- ✅ Return statements (literal, variable, expression)
- ✅ Function calls (no args, 1 arg, 2 args, 3 args)
- ✅ Void function calls (procedures)
- ✅ Complete function examples with body
- ✅ Integration test with 5 scenarios (factorial, recursive pattern)
- ✅ 518 lines total, 18 functions, 15/15 tests passing

**Test Results:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  modules/codegen_mlp/codegen_functions.mlp temp/codegen_functions.s
→ 15/15 PASSED ✅

./compiler/stage0/modules/functions/functions_standalone \
  tests/manual/test_codegen_functions_integration.mlp temp/test_integration.s
→ Integration test PASSED ✅
```

**Files Created:**
- `modules/codegen_mlp/codegen_functions.mlp` (518 lines)
- `tests/manual/test_codegen_functions_integration.mlp` (129 lines)

**Status:** ✅ YZ_17 COMPLETE - Ready for YZ_18 (Array CodeGen)!

---

**✅ YZ_16 COMPLETE - For Loop CodeGen**

**YZ_16 UPDATE (17 December 2025):**

**Completed (For Loop CodeGen - YZ_16):**
- ✅ For-to loop code generation (for i from 1 to 10)
- ✅ For-downto loop code generation (for i from 10 downto 1)
- ✅ Loop counter initialization and management
- ✅ Increment (add) and decrement (sub) operations
- ✅ Nested for loops (both to and downto)
- ✅ Mixed nested loops (to + downto)
- ✅ For loops with body statements
- ✅ Variable start/end bounds support
- ✅ 473 lines total, 6 functions, 6/6 tests passing

**Test Results:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  modules/codegen_mlp/codegen_for.mlp temp/codegen_for.s
→ 6/6 PASSED ✅
```

**Files Created:**
- `modules/codegen_mlp/codegen_for.mlp` (473 lines)

**Status:** ✅ YZ_16 COMPLETE - Ready for YZ_17 (Function CodeGen)!

---

**✅ YZ_15 COMPLETE - While Loop CodeGen**

**Completed (While Loop CodeGen - YZ_15):**
- ✅ While loop code generation (while condition do body end_while)
- ✅ Loop header, body, and exit labels
- ✅ Back-edge branch (jump to header)
- ✅ Counter-based loops with increment
- ✅ Nested while loops support
- ✅ Complex conditions (integration with logical ops)
- ✅ Early exit patterns
- ✅ 333 lines total, 5 functions, 5/5 tests passing

**Test Results:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  modules/codegen_mlp/codegen_while.mlp temp/codegen_while.s
→ 5/5 PASSED ✅
```

**Files Created:**
- `modules/codegen_mlp/codegen_while.mlp` (333 lines)

**Status:** ✅ YZ_15 COMPLETE - Ready for YZ_16 (For Loop CodeGen)!

---

**✅ YZ_14 COMPLETE - Control Flow CodeGen**

**Completed (Control Flow CodeGen - YZ_14):**
- ✅ If statement code generation (if-then-end_if)
- ✅ If-else statement code generation
- ✅ If-else_if chain code generation
- ✅ Basic block label management
- ✅ Branch instruction emission (br i1)
- ✅ 220 lines total, 4 functions, 4/4 tests passing

**Test Results:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  modules/codegen_mlp/codegen_control.mlp temp/codegen_control.s
→ 4/4 PASSED ✅
```

**Files Created:**
- `modules/codegen_mlp/codegen_control.mlp` (220 lines)

**Status:** ✅ YZ_14 COMPLETE - Ready for YZ_15 (While Loop CodeGen)!

---

**✅ YZ_12 COMPLETE - Comparison & Logical Operators CodeGen**

**YZ_12 UPDATE (16 December 2025):**

**Completed (Comparison & Logical Operators - YZ_12):**
- ✅ Comparison Operators (194 lines) - ==, !=, <, >, <=, >=
- ✅ Logical Operators (218 lines) - and, or, not
- ✅ Boolean Operations - i1 ↔ i64 conversions
- ✅ 412 lines total, 18 functions, 13 tests

**Test Results:**
```bash
./functions_standalone modules/codegen_mlp/codegen_comparison.mlp temp/codegen_comparison.s
→ 12/12 PASSED ✅

./functions_standalone modules/codegen_mlp/codegen_logical.mlp temp/codegen_logical.s
→ 10/10 PASSED ✅
```

**Files Created:**
- `modules/codegen_mlp/codegen_comparison.mlp` (194 lines)
- `modules/codegen_mlp/codegen_logical.mlp` (218 lines)

**Status:** ✅ YZ_12 COMPLETE - Ready for YZ_13 (Statement CodeGen)!

---

**✅ YZ_11 COMPLETE - Arithmetic Operations CodeGen**

**YZ_11 UPDATE (16 December 2025):**

**Completed (Arithmetic Operations - YZ_11):**
- ✅ IR Builder (299 lines) - LLVM IR emission functions
- ✅ Type Mapper (265 lines) - MELP → LLVM type mapping
- ✅ Symbol Table (277 lines) - Variable tracking structure
- ✅ Integration Demo (101 lines) - Complete pipeline demo
- ✅ 942 lines total, 28 functions, 90% test coverage

**Test Results:**
```bash
./functions_standalone modules/codegen_mlp/type_mapper.mlp temp/type_mapper.s
→ 4/4 PASSED ✅

./functions_standalone modules/codegen_mlp/codegen_demo.mlp temp/codegen_demo.s
→ FULL DEMO WORKING ✅
```

**Files Created:**
- `modules/codegen_mlp/ir_builder.mlp` (299 lines)
- `modules/codegen_mlp/type_mapper.mlp` (265 lines)
- `modules/codegen_mlp/symbol_table.mlp` (277 lines)
- `modules/codegen_mlp/codegen_demo.mlp` (101 lines)

**Status:** ✅ YZ_09 COMPLETE - Ready for YZ_10 (Expression CodeGen)!

---

**✅ YZ_08 COMPLETE - All 5 Small Parser Tasks Done!**

**YZ_08 UPDATE (16 December 2025):**

**Completed (5 tasks in 1 session!):**
- ✅ For loop parsing (`for i from x to y`, `for i from x downto y`) - 299 lines
- ✅ Function call expressions (`func(arg1, arg2, ...)`) - 327 lines
- ✅ Array/List indexing (`arr[i]`, `matrix[i][j]`) - 266 lines
- ✅ AST pretty-print (debugging tool) - 336 lines
- ✅ Compound assignment structure (`+=, -=, *=, /=`) - 227 lines
- ✅ Integration test updated (9/9 tests passing)

**Test Results:**
```bash
./functions_standalone modules/parser_mlp/parser_for.mlp temp/parser_for.s
→ 4/4 PASSED

./functions_standalone modules/parser_mlp/parser_call.mlp temp/parser_call.s
→ 6/6 PASSED

./functions_standalone modules/parser_mlp/parser_index.mlp temp/parser_index.s
→ 6/6 PASSED

./functions_standalone modules/parser_mlp/parser_pretty.mlp temp/parser_pretty.s
→ 6/6 PASSED

./functions_standalone modules/parser_mlp/parser_compound.mlp temp/parser_compound.s
→ 5/5 PASSED

./functions_standalone modules/parser_mlp/parser_integration.mlp temp/parser_integration.s
→ 9/9 PASSED (was 8/8)
```

**Files Created:**
- `modules/parser_mlp/parser_for.mlp` (299 lines)
- `modules/parser_mlp/parser_call.mlp` (327 lines)
- `modules/parser_mlp/parser_index.mlp` (266 lines)
- `modules/parser_mlp/parser_pretty.mlp` (336 lines)
- `modules/parser_mlp/parser_compound.mlp` (227 lines)

**Files Updated:**
- `modules/parser_mlp/parser_integration.mlp` (583 lines, was 534)

**Total Added:** 1,455 lines of MELP parser code!

**Status:** ✅ YZ_08 COMPLETE - Parser at ~6,686 lines! All small tasks done!

---

## 🎯 KULLANICIYA SORU: Ne yapmalıyım?

**Önceki görev (YZ_17):** CodeGen Functions ✅ Tamamlandı (518 satır, 18 fonksiyon, 15/15 test geçti)

**CodeGen Phase (Phase 2):** Part 9/10 ✅ Complete!

**İlerleme:**
- ✅ YZ_09: Infrastructure (IR Builder, Type Mapper, Symbol Table) - DONE
- ✅ YZ_10: Literals & Variables - DONE
- ✅ YZ_11: Arithmetic operators - DONE
- ✅ YZ_12: Comparison & Logic - DONE
- ✅ YZ_13: Statements (var decl, assign, return, print) - DONE
- ✅ YZ_14: Control Flow - If/Else - DONE
- ✅ YZ_15: While Loops - DONE
- ✅ YZ_16: For Loops (from/to/downto) - DONE
- ✅ YZ_17: Functions (declarations, calls, parameters) - DONE
- ⏳ YZ_18: Arrays (indexing, allocation, element access) - NEXT
- ⏸️ YZ_19: Integration & final testing...

**Sıradaki görev: YZ_18 - Array CodeGen**
- Array allocation (alloca for arrays)
- Array indexing (getelementptr)
- Multi-dimensional arrays
- Array element load/store
- Array initialization

**Tahmini:** 3-4 saat, ~400-500 satır

**Soru:** 
- YZ_18'e başlamak ister misin?
- Başka bir şey mi yapalım?
- Önce YZ_17'yi commit edip push edelim mi?

## 📚 Eski Referans (Arşiv)

### Phase 1 Part 1: Parser Infrastructure (✅ TAMAMLANDI - YZ_97)

**Görev:** Parser'ın temellerini MELP ile kur

#### Step 1: Dizin ve Dosya Yapısı
```bash
mkdir -p modules/parser_mlp
touch modules/parser_mlp/parser_core.mlp
touch modules/parser_mlp/ast_nodes.mlp
touch modules/parser_mlp/token_stream.mlp
```

#### Step 2: AST Node Structures

**Dosya:** `modules/parser_mlp/ast_nodes.mlp`

```mlp
-- AST Node Types (Enum)
enum ASTNodeType
    NODE_EXPRESSION = 1
    NODE_STATEMENT = 2
    NODE_FUNCTION = 3
    NODE_LITERAL = 4
    NODE_BINARY_OP = 5
    NODE_CALL = 6
    -- ... daha fazla
end_enum

-- Base AST Node (Struct)
struct ASTNode
    numeric node_type        -- ASTNodeType enum
    numeric line
    numeric column
    -- Node-specific data için genişletilebilir
end_struct

-- Expression Node
struct ExprNode
    numeric node_type
    numeric line
    numeric column
    numeric expr_type        -- EXPR_LITERAL, EXPR_BINARY, etc.
    string value            -- For literals
end_struct

-- Statement Node
struct StmtNode
    numeric node_type
    numeric line
    numeric column
    numeric stmt_type        -- STMT_VAR, STMT_IF, etc.
end_struct
```

#### Step 3: Token Stream Management

**Dosya:** `modules/parser_mlp/token_stream.mlp`

```mlp
-- Global parser state (list of tokens)
list tokens           -- Token listesi
numeric current_pos   -- Mevcut pozisyon

function init_parser(list token_list) returns numeric
    tokens = token_list
    current_pos = 0
    return 1
end_function

function current_token() returns list
    if current_pos >= length(tokens) then
        return []  -- EOF
    end_if
    return tokens[current_pos]
end_function

function peek_token() returns list
    numeric next_pos = current_pos + 1
    if next_pos >= length(tokens) then
        return []  -- EOF
    end_if
    return tokens[next_pos]
end_function

function advance() returns numeric
    current_pos = current_pos + 1
    return current_pos
end_function

function expect_token(numeric expected_type) returns boolean
    list tok = current_token()
    if length(tok) == 0 then
        return 0  -- EOF
    end_if
    
    numeric tok_type = tok[0]
    if tok_type == expected_type then
        advance()
        return 1
    end_if
    return 0
end_function
```

#### Step 4: Error Handling

**Dosya:** `modules/parser_mlp/parser_errors.mlp`

```mlp
function parser_error(string message, numeric line, numeric col) returns numeric
    println("Parser Error: " + message)
    println("  at line " + str(line) + ", column " + str(col))
    return 0
end_function

function unexpected_token_error(numeric expected, numeric got) returns numeric
    println("Unexpected token: expected " + str(expected) + ", got " + str(got))
    return 0
end_function
```

#### Step 5: Integration Test

**Dosya:** `tests/manual/test_parser_init.mlp`

```mlp
-- Test: Initialize parser with token list

function main() returns numeric
    -- Create a simple token list
    list tok1 = [1, "function", 1, 1]  -- [type, value, line, col]
    list tok2 = [32, "main", 1, 10]
    list tok3 = [20, "(", 1, 14]
    list tok4 = [21, ")", 1, 15]
    
    list token_list = [tok1, tok2, tok3, tok4]
    
    init_parser(token_list)
    
    list first = current_token()
    println("First token type: " + str(first[0]))  -- Should be 1
    
    advance()
    list second = current_token()
    println("Second token type: " + str(second[0]))  -- Should be 32
    
    return 0
end_function
```

---

## 📋 Phase 1 Part 1 Checklist

- [ ] `modules/parser_mlp/` dizini oluştur
- [ ] `ast_nodes.mlp` - AST node structures
  - [ ] ASTNodeType enum
  - [ ] ASTNode base struct
  - [ ] ExprNode struct
  - [ ] StmtNode struct
- [ ] `token_stream.mlp` - Token stream management
  - [ ] init_parser()
  - [ ] current_token()
  - [ ] peek_token()
  - [ ] advance()
  - [ ] expect_token()
- [ ] `parser_errors.mlp` - Error handling
  - [ ] parser_error()
  - [ ] unexpected_token_error()
- [ ] `tests/manual/test_parser_init.mlp` - Integration test
- [ ] Compile ve test et
  ```bash
  ./compiler/stage0/modules/functions/functions_standalone \
      modules/parser_mlp/token_stream.mlp token_stream.s
  ```

---

## 🚦 Sonraki Adımlar (Part 2+)

**Part 1 tamamlandıktan sonra:**

1. **Part 2:** Expression Parsing
   - Primary expressions (literals, identifiers)
   - Binary operations (arithmetic, comparison)
   - Precedence handling (Pratt parsing)

2. **Part 3:** Statement Parsing
   - Variable declarations
   - Control flow (if/while/for)
   - Function calls

3. **Part 4:** Advanced Parsing
   - Function definitions
   - Struct definitions
   - Module imports

4. **Part 5:** Integration & Testing
   - Lexer + Parser pipeline
   - AST validation
   - End-to-end tests

---

## 📁 Dosya Yapısı

```
modules/
├── lexer_mlp/              ← ✅ DONE (YZ_57)
│   ├── lexer.mlp
│   ├── tokenize_*.mlp
│   └── ...
│
└── parser_mlp/             ← 🎯 ŞİMDİ BURASI!
    ├── parser_core.mlp     ← Main parser
    ├── ast_nodes.mlp       ← AST structures
    ├── token_stream.mlp    ← Token management
    ├── parser_errors.mlp   ← Error handling
    ├── parser_expr.mlp     ← Expression parsing (Part 2)
    ├── parser_stmt.mlp     ← Statement parsing (Part 3)
    ├── parser_func.mlp     ← Function parsing (Part 4)
    └── parser_struct.mlp   ← Struct parsing (Part 4)

tests/
├── manual/                 ← Manuel test dosyaları
│   ├── test_parser_*.mlp
│   └── test_lexer_*.mlp
└── e2e/                    ← End-to-end testler

temp/                       ← Geçici deneme dosyaları
    └── experiment_*.mlp
```

### ⚠️ ÖNEMLI: Dosya Organizasyonu

**Ana dizini temiz tut!**

- ✅ Test dosyaları → `tests/manual/` veya `tests/e2e/`
- ✅ Geçici dosyalar → `temp/`
- ✅ Ana dizin → Sadece proje dosyaları (README, TODO, MELP_REFERENCE, vb.)
- ❌ Ana dizinde `test_*.mlp`, `*.s`, `*.o` bırakma!

```bash
# Cleanup komutu (düzenli çalıştır)
rm -f test_*.mlp test_*.s test_*.o *.s *.o
```

---

## ⚡ Hızlı Başlangıç

```bash
# 1. Dizin oluştur
mkdir -p modules/parser_mlp

# 2. İlk dosyayı oluştur
cat > modules/parser_mlp/ast_nodes.mlp << 'EOF'
-- AST Node Types
enum ASTNodeType
    NODE_EXPRESSION = 1
    NODE_STATEMENT = 2
    NODE_FUNCTION = 3
end_enum

struct ASTNode
    numeric node_type
    numeric line
    numeric column
end_struct

function main() returns numeric
    return 0
end_function
EOF

# 3. Compile et
./compiler/stage0/modules/functions/functions_standalone \
    modules/parser_mlp/ast_nodes.mlp ast_nodes.s

# 4. Test et
./ast_nodes.s
echo $?  # Should be 0
```

---

## 🎯 Başarı Kriterleri (Part 1)

✅ **Tamamlandı sayılır eğer:**
1. AST node structures tanımlandı (enum + struct)
2. Token stream management çalışıyor
3. Error handling fonksiyonları var
4. En az 1 integration test passing
5. Stage 0 compiler ile compile ediliyor

**Tahmini Süre:** 8-10 saat (~1 hafta)

---

## 📖 Kaynaklar

- **Lexer Referansı:** `modules/lexer_mlp/lexer.mlp` (YZ_57)
- **Stage 0 Parser:** `compiler/stage0/modules/parser_core/`
- **Self-Hosting Plan:** `docs_tr/selfhosting_geçiş_planlaması.md`
- **MELP Reference:** `MELP_REFERENCE.md`

---

## 🚀 HADİ BAŞLAYALIM!

**İlk Görev:** Parser Infrastructure (Part 1)  
**Tahmini Süre:** 1 hafta  
**Sonuç:** Parser'ın temelleri MELP'te yazılmış olacak!

---

**Last Updated:** 15 Aralık 2025  
**Status:** Stage 1 Phase 1 Part 1 - Ready to Start! 🚀
