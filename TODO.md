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
| **Parser Pretty Print** | ✅ %100 | MELP | 336 | **Stage 1 (YZ_08) ✅** |
| **Parser Compound Assign** | ✅ %100 | MELP | 227 | **Stage 1 (YZ_08) ✅** |
| **Parser Integration** | ✅ %100 | MELP | 583 | **Stage 1 (YZ_06/07/08) ✅** |
| **CodeGen** | ❌ %0 | MELP | - | Stage 1 (Future) |
| **Bootstrap** | ❌ %0 | - | - | Stage 1 (Future) |

**İlerleme:** Stage 1 Phase 1 - %90 Complete (Parser Phase Complete!)

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

### 3️⃣ Session Bitişi (Son 10 dakika) - ÇOK ÖNEMLİ!

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
mv temp/*.s temp/archive/ 2>/dev/null || true

# Adım 5: KULLANICIYA SOR
# "Görev YZ_XX tamamlandı! Sırada ne yapmak istersin?"
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
