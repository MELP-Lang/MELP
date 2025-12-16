# 📋 MELP Project TODO - Stage 1 Self-Hosting

**Created:** 15 Aralık 2025  
**Current Status:** 🚀 Stage 0 Complete - Starting Stage 1 Self-Hosting!  
**Branch:** main  
**Last Updated:** 16 Aralık 2025  
**YZ Sessions:** Stage 1 YZ_01+ (Stage 0 = 80 sessions archived in `stage_0_YZ/`)

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
| **Parser** | ❌ %0 | C | - | Stage 1 (TODO) |
| **CodeGen** | ❌ %0 | C | - | Stage 1 (TODO) |
| **Bootstrap** | ❌ %0 | - | - | Stage 1 (TODO) |

**İlerleme:** Stage 1 - %25 Complete (Sadece Lexer)

---

## 🎯 ŞU AN: Stage 1 Phase 1 - Parser in MELP

### Phase 1: Parser Self-Hosting (Tahmini: 4-6 hafta)

**Hedef:** Parser'ı MELP dilinde yaz, Stage 0 compiler ile derle

**Neden Kritik:**
- Parser olmadan AST üretemeyiz
- AST olmadan code generation yapamayız
- Self-hosting'in ikinci kritik adımı

#### Part 1: Parser Infrastructure (1 hafta)
- [ ] AST Node structures (struct kullanarak)
  - [ ] `ASTNode` base struct
  - [ ] `ExpressionNode`, `StatementNode`, `FunctionNode`
  - [ ] Node type enums
- [ ] Token stream management
  - [ ] `current_token()`, `peek_token()`, `advance()`
  - [ ] Lookahead implementation
- [ ] Error handling & recovery
  - [ ] Error reporting functions
  - [ ] Panic mode recovery

**Dosya:** `modules/parser_mlp/parser_core.mlp`

#### Part 2: Expression Parsing (1-2 hafta)
- [ ] Primary expressions
  - [ ] Literals (numeric, string, boolean)
  - [ ] Identifiers
  - [ ] Parenthesized expressions
- [ ] Arithmetic expressions
  - [ ] Addition, subtraction (+, -)
  - [ ] Multiplication, division (*, /, %)
  - [ ] Operator precedence (Pratt parsing veya precedence climbing)
- [ ] Boolean expressions
  - [ ] Comparison operators (==, !=, <, >, <=, >=)
  - [ ] Logical operators (and, or, not)
- [ ] Function calls
  - [ ] Call expression parsing
  - [ ] Argument list parsing
- [ ] Array/List access
  - [ ] Index expressions (arr[i])

**Dosyalar:**
- `modules/parser_mlp/parser_expr.mlp`
- `modules/parser_mlp/parser_pratt.mlp` (precedence)

#### Part 3: Statement Parsing (1-2 hafta)
- [ ] Variable declarations
  - [ ] `numeric x = 10`
  - [ ] `string name = "test"`
  - [ ] Type annotations
- [ ] Assignment statements
  - [ ] Simple assignment (x = 5)
  - [ ] Compound assignment (+=, -=, etc.)
- [ ] Control flow statements
  - [ ] If/else_if/else
  - [ ] While loops
  - [ ] For loops (from/to/downto)
- [ ] Jump statements
  - [ ] Return statements
  - [ ] Exit/Continue
- [ ] Print statements

**Dosya:** `modules/parser_mlp/parser_stmt.mlp`

#### Part 4: Advanced Parsing (1 hafta)
- [ ] Function declarations
  - [ ] Function signature
  - [ ] Parameters
  - [ ] Return type
  - [ ] Function body
- [ ] Struct definitions
  - [ ] Struct fields
  - [ ] Nested structs
  - [ ] Methods (if needed)
- [ ] Enum definitions
- [ ] Switch/Case statements
- [ ] Module imports

**Dosyalar:**
- `modules/parser_mlp/parser_func.mlp`
- `modules/parser_mlp/parser_struct.mlp`
- `modules/parser_mlp/parser_module.mlp`

#### Part 5: Integration & Testing (1 hafta)
- [ ] Lexer + Parser entegrasyonu
- [ ] AST validation
- [ ] Pretty-print AST (debugging için)
- [ ] Comprehensive tests
  - [ ] Expression parsing tests
  - [ ] Statement parsing tests
  - [ ] Error recovery tests

---

## 🔮 Stage 1 Sonraki Faz'lar

### Phase 2: CodeGen in MELP (4-6 hafta)
**Hedef:** LLVM IR code generator'ı MELP'te yaz

- [ ] Part 1: LLVM IR Emission (1 hafta)
- [ ] Part 2: Expression CodeGen (1-2 hafta)
- [ ] Part 3: Statement CodeGen (1-2 hafta)
- [ ] Part 4: Optimization & Integration (1 hafta)

### Phase 3: Bootstrap Complete (2 hafta)
**Hedef:** MELP compiler'ı MELP ile derle!

```bash
# Step 1: Stage 0 (C) ile Stage 1 (MELP) derle
stage0/melpc stage1/*.mlp -o melp_stage1

# Step 2: Stage 1 kendini derle!
melp_stage1 stage1/*.mlp -o melp_stage1_v2

# Step 3: Verification
diff melp_stage1 melp_stage1_v2  # Aynı mı?
```

- [ ] Compile Stage 1 with Stage 0
- [ ] Self-compilation test
- [ ] Binary verification
- [ ] Performance benchmarking
- [ ] 🎉 SELF-HOSTING ACHIEVED!

---

## 📅 Timeline

**Stage 1 Tahmini:**
- Parser in MELP: 4-6 hafta (~30-40 saat)
- CodeGen in MELP: 4-6 hafta (~30-40 saat)
- Bootstrap & Testing: 2 hafta (~15-20 saat)
- **TOPLAM: 10-14 hafta (~75-100 saat)**

**Hedef Tarihler:**
- 31 Aralık 2025: Parser Part 1-2 complete
- 31 Ocak 2026: Parser complete
- 28 Şubat 2026: CodeGen complete
- 31 Mart 2026: Bootstrap complete - SELF-HOSTING! 🚀

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

## ⚠️ ÖNEMLI KURALLAR

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
