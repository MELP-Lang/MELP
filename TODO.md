# 📋 MELP Project TODO - Multi-Phase LLVM Backend Development

**Created:** 13 Aralık 2025  
**Current Status:** Phase 17 ✅ COMPLETE | Next: Phase 18 Array Support  
**Branch:** phase17-string-support_YZ_61 (ready to merge)  
**Last Updated:** 14 Aralık 2025 (YZ_69)

---

## ✅ Phase 15: Standard Library Integration (COMPLETED - YZ_61)

**DURUM: ✅ TAMAMLANDI**

**AMAÇ:** LLVM backend'inin `printf` bağımlılığını kaldırıp MELP stdlib'i kullanması.

### Tamamlanan Görevler:

- [x] Git branch oluşturuldu: `phase15-stdlib-integration_YZ_61`
- [x] Print statement desteği eklendi (LLVM backend)
- [x] `mlp_println_numeric` entegrasyonu yapıldı
- [x] `statement_parser.c` güncellendi (`print <variable>` syntax)
- [x] Makefile güncellendi (stdlib ve sto runtime linking)
- [x] Test suite oluşturuldu (3/3 test başarılı)
- [x] Tüm değişiklikler commit ve push edildi

**Sonuç:** ✅ Başarıyla tamamlandı! Print statement çalışıyor.

---

## 🚀 Phase 16: Advanced LLVM Features

**DURUM: 🔵 BAŞLAMADI**  
**TAHMİNİ SÜRE:** 3-4 saat  
**ÖNCELİK:** Orta

**AMAÇ:** LLVM optimizasyonları, debug info ve gelişmiş özellikler.

### Görevler:

- [ ] Optimization flags (-O0, -O1, -O2, -O3)
- [ ] LLVM IR metadata
- [ ] Debug information generation (DWARF)
- [ ] Source location tracking
- [ ] Better error messages
- [ ] Performance benchmarks

---

## 🔤 Phase 17: String Support in LLVM

**DURUM: ✅ TAMAMLANDI (100% - YZ_69)**  
**TAHMİNİ SÜRE:** 5-6 saat (TAMAMLANDI)  
**ÖNCELİK:** Tamamlandı

**AMAÇ:** String literal ve operasyonları eklemek.

### Tamamlanan Görevler (YZ_61):

- [x] Git branch oluşturuldu: `phase17-string-support_YZ_61`
- [x] String literal support (global .rodata)
  - [x] `llvm_emit_string_global()` implementasyonu
  - [x] `StringGlobal` linked list yapısı
  - [x] Deferred emission (fonksiyonlardan sonra)
  - [x] UTF-8 karakter desteği
- [x] `print("Hello World")` syntax
  - [x] `print_parser` stateless pattern'e güncellendi
  - [x] `mlp_println_string` entegrasyonu
  - [x] Escape sequence handling (\n, \t, \\, ")
- [x] İlk test suite (test_string_literal.mlp, test_multiline.mlp)
- [x] Tüm değişiklikler commit ve push edildi

### Tamamlanan Görevler (YZ_62):

- [x] String variable declaration (`string x = "test"`)
  - [x] LLVM IR: i8* alloca on stack
  - [x] Store pointer to global string constant
  - [x] Naming convention: %varname_ptr
- [x] String variable printing (`print(x)`)
  - [x] Type detection via function body scan
  - [x] Load i8* and call mlp_println_string
- [x] Comprehensive test suite
  - [x] test_string_var.mlp - Single variable
  - [x] test_string_var_multi.mlp - Multiple variables
  - [x] test_string_full.mlp - Mix of literals + variables
  - [x] YZ_61 regression tests passing
- [x] UTF-8 variable support ("MELP Dünya" works)
- [x] Tüm değişiklikler commit ve push edildi

### Tamamlanan Görevler (YZ_63):

- [x] String function parameters (`function greet(string msg)`)
  - [x] LLVM backend param_types array support
  - [x] `llvm_emit_function_start()` signature updated
  - [x] i8* parameters in LLVM IR
  - [x] String params can be printed inside function
- [x] Mixed type support verified (numeric + string + boolean)
  - [x] test_mixed_types.mlp - All types coexist ✅
- [x] Critical bug fixes:
  - [x] Multi-function parsing (end function check removed)
  - [x] Statement parser token ungetting fixed
  - [x] TOKEN_STRING_TYPE vs TOKEN_STRING recognition
- [x] Test suite expanded
  - [x] test_string_param_literal.mlp (partial)
  - [x] test_string_param_var.mlp
  - [x] test_string_param_multiple.mlp
  - [x] test_two_funcs.mlp (regression)
- [x] Tüm değişiklikler commit ve push edildi

### Tamamlanan Görevler (YZ_64):

- [x] **Function calls with string LITERAL arguments** ✅
  - [x] Extended LLVMValue with type field (LLVM_TYPE_I64, LLVM_TYPE_I8_PTR, LLVM_TYPE_I1)
  - [x] Added `llvm_emit_string_ptr()` for getelementptr emission
  - [x] Updated `llvm_emit_call()` to emit correct argument types
  - [x] Modified `generate_expression_llvm()` to handle string literals
  - [x] String literals → i8* pointers in function calls
  - [x] `greet("Hello MELP")` now works! 🎉
  - [x] Test: test_string_param_literal.mlp ✅ PASSING
- [x] Tüm değişiklikler commit ve push edildi

### Tamamlanan Görevler (YZ_65):

- [x] **Function calls with string VARIABLE arguments** ✅ ÇALIŞIYOR!
  - [x] Statement parser'a function call desteği eklendi
  - [x] TOKEN_LPAREN kontrolü ile function call detection
  - [x] LLVM backend için LocalVariable registry population
  - [x] scan_statement_for_variables() fonksiyonu eklendi
  - [x] Variable type tracking (is_numeric flag) çalışıyor
  - [x] String variables: i8* load doğru yapılıyor
  - [x] Test: test_string_param_var.mlp ✅ PASSING
  - [x] Test: test_string_param_multiple.mlp ✅ PASSING
  - [x] Mixed arguments: `greet("Hello", name)` ✅ PASSING
  - [x] Regression test: test_string_param_literal.mlp ✅ PASSING
- [x] Tüm değişiklikler commit ve push edildi

### Tamamlanan Görevler (YZ_66):

- [x] **Print statement x86-64 assembly support** ✅ FIXED!
  - [x] statement_codegen.c'ye print.h include eklendi
  - [x] STMT_PRINT case'i implement edildi
  - [x] String literal → .rodata section + puts@PLT
  - [x] String variable → load from stack + puts@PLT
  - [x] Function parameter → load from -8(%rbp) + puts@PLT
  - [x] Test: test_string_param_var.mlp ✅ PASSING (x86-64)
  - [x] Test: test_string_param_literal.mlp ✅ PASSING (x86-64)
  - [x] Test: test_string_param_multiple.mlp ✅ CREATED & PASSING
  - [x] Test: test_string_param_mixed.mlp ✅ CREATED & PASSING
- [x] **String concatenation - BOTH BACKENDS** ✅ WORKING!
  - [x] Variable type propagation fix (arithmetic_expr_mark_as_string)
  - [x] x86-64 backend: mlp_string_concat çağrısı (YZ_07'den var)
  - [x] LLVM backend: llvm_emit_string_concat() eklendi
  - [x] functions_codegen_llvm.c: ARITH_ADD'de string check
  - [x] Test: test_string_concat_literal.mlp ✅ PASSING (compile-time optimization!)
  - [x] Test: test_string_concat_var.mlp ✅ PASSING (both backends)
  - [x] Test: test_string_concat_multi.mlp ✅ PASSING (a + b + c works!)
- [x] Tüm testler çalışıyor ve doğrulandı!

### Kalan Görevler (Phase 17 - String Operations):

- [x] String concatenation (`x + " world"`) ✅ TAMAMLANDI (YZ_66)
- [x] **String comparison (==, !=, <, >, <=, >=)** ✅ TAMAMLANDI (YZ_69)
  - Runtime fonksiyonlar ✅ hazır (mlp_string_compare, mlp_string_equals - YZ_06/07)
  - x86-64 codegen ✅ çalışıyor (32-bit register fix)
  - LLVM codegen ✅ çalışıyor
  - Bug fix: mlp_string_compare int döndürüyor, %eax kullanmalıyız
- [ ] String methods (length, substring, indexOf) (3-4 saat)
- [ ] STO string type integration (2-3 saat)
- [ ] Memory management (heap allocation for strings) (2-3 saat)

**NOT:** 
- String literals ✅, variables ✅, function parameters ✅, literal arguments ✅, ve variable arguments ✅ tamamlandı!
- String concatenation ✅ - Her iki backend de (x86-64 ve LLVM) destekliyor! (YZ_66)
- String comparison ✅ - Tüm operatörler çalışıyor (==, !=, <, >, <=, >=) (YZ_69)
- x86-64 assembly backend de ✅ string support ile tamamlandı! (YZ_66)
- Her iki backend (LLVM IR ve x86-64) de string printing + concat + comparison destekliyor!
- Compile-time optimization: String literal concatenation optimize ediliyor!
- **Phase 17 TAMAMLANDI! 🎉**

---

## �� Phase 18: Array Support in LLVM

**DURUM: 🔵 BAŞLAMADI**  
**TAHMİNİ SÜRE:** 6-8 saat  
**ÖNCELİK:** Orta-Yüksek

**AMAÇ:** Array ve list desteği eklemek.

### Görevler:

- [ ] Array literals ([1, 2, 3])
- [ ] Array indexing (arr[i])
- [ ] Bounds checking
- [ ] Array operations (length, push, pop)
---

**Son Güncelleme:** 14 Aralık 2025  
**YZ Session:** YZ_69 ✅ TAMAMLANDI  
**Durum:** Phase 15 ✅ Complete | Phase 17 ✅ Complete (100% - String comparison bug fixed! 🎉)  
**Next:** Phase 16 (Advanced LLVM Features) veya Phase 18 (Array Support)
---

## 📝 Phase 19: Documentation & Polish

**DURUM: 🔄 KISMİ TAMAMLANDI**  
**TAHMİNİ SÜRE:** 8-10 saat (küçük YZ oturumlarına bölünmüş)  
**ÖNCELİK:** Yüksek (Stage 0 finalize için gerekli)

**AMAÇ:** Proje dokümantasyonunu tamamlamak, README'yi güncellemek, mimari dokümantasyon eklemek.

### Phase 19.1: README.md Güncelleme (YZ_70)

**TAHMİNİ SÜRE:** 1.5-2 saat  
**ÖNCELİK:** Çok Yüksek

- [ ] **Project Overview Section**
  - [ ] MELP nedir açıklaması (Türkçe + English)
  - [ ] Temel özellikler listesi
  - [ ] Neden MELP? (motivasyon)
  - [ ] Hedef kitle tanımı
  
- [ ] **Features Section**
  - [ ] ✅ Tamamlanan özellikler (Phase 1-17)
  - [ ] Variables (Int, String, Boolean)
  - [ ] Functions (parameters, return values)
  - [ ] Control flow (if/else, while, for)
  - [ ] String operations (concat, comparison)
  - [ ] LLVM backend
  - [ ] x86-64 native backend
  
- [ ] **Quick Start Guide**
  - [ ] Installation talimatları
  - [ ] İlk program yazma (Hello World)
  - [ ] Compile ve run örnekleri
  - [ ] Her iki backend için örnekler (LLVM vs x86-64)
  
- [ ] **Build Instructions**
  - [ ] Dependencies (LLVM 19, GCC, Make)
  - [ ] Build komutu (`make all`)
  - [ ] Test komutu (`make test`)
  - [ ] Clean komutu (`make clean`)

### Phase 19.2: ARCHITECTURE.md Detaylandırma (YZ_71)

**TAHMİNİ SÜRE:** 2-2.5 saat  
**ÖNCELİK:** Yüksek  
**DURUM:** ✅ TAMAMLANDI (14 Aralık 2025)

- [x] **Compiler Architecture Overview**
  - [x] Stage 0 mimarisi (bootstrap compiler)
  - [x] Pipeline diagram (Lexer → Parser → CodeGen)
  - [x] Module structure (`compiler/stage0/modules/`)
  - [x] Dual backend architecture (LLVM + x86-64)
  
- [x] **Component Documentation**
  - [x] Lexer modülü (`lexer_mlp/`)
  - [x] Parser modülü (`parser/`, `statement_parser.c`)
  - [x] CodeGen modülleri (LLVM vs x86-64)
  - [x] Runtime library (`runtime/stdlib/`, `runtime/sto/`)
  
- [x] **Code Flow Examples**
  - [x] Basit program compile akışı
  - [x] LLVM IR generation adımları
  - [x] x86-64 assembly generation adımları
  - [x] Runtime function call mekanizması
  
- [x] **Design Decisions**
  - [x] Neden dual backend?
  - [x] Neden C dilinde Stage 0?
  - [x] STO vs TTO string migration
  - [x] LLVM 19 seçimi
  - [x] Stateless parser design
  - [x] No central files (radical modularity)
  - [x] "Good Enough" philosophy

**Sonuç:** ✅ +850 satır kapsamlı dokümantasyon eklendi! ARCHITECTURE.md artık tam bir referans dokümanı.

**Git Commit:** 7c68d2d - "YZ_71: Add comprehensive architecture documentation"  
**Branch:** `architecture-docs_YZ_71` (pushed to GitHub)

### Phase 19.3: Tutorial Oluşturma (YZ_72)

**TAHMİNİ SÜRE:** 2-2.5 saat  
**ÖNCELİK:** Orta-Yüksek

- [ ] **docs/tutorials/01_hello_world.md**
  - [ ] İlk MELP programı
  - [ ] Adım adım açıklama
  - [ ] Her iki backend ile compile
  - [ ] Çıktı analizi
  
- [ ] **docs/tutorials/02_variables.md**
  - [ ] Variable declaration örnekleri
  - [ ] Type system açıklaması (Int, String, Boolean)
  - [ ] Variable scope kavramı
  - [ ] Örnekler ve alıştırmalar
  
- [ ] **docs/tutorials/03_functions.md**
  - [ ] Function tanımlama
  - [ ] Parameters ve return values
  - [ ] Function call mekanizması
  - [ ] Recursive fonksiyonlar
  
- [ ] **docs/tutorials/04_strings.md**
  - [ ] String literals ve variables
  - [ ] String concatenation
  - [ ] String comparison
  - [ ] STO string type açıklaması

### Phase 19.4: API Documentation (YZ_73)

**TAHMİNİ SÜRE:** 1.5-2 saat  
**ÖNCELİK:** Orta

- [ ] **docs/api/runtime_functions.md**
  - [ ] `mlp_println_numeric()` dokümantasyonu
  - [ ] `mlp_println_string()` dokümantasyonu
  - [ ] `mlp_string_concat()` dokümantasyonu
  - [ ] `mlp_string_compare()` dokümantasyonu
  - [ ] `mlp_string_equals()` dokümantasyonu
  - [ ] Her fonksiyon için: signature, parameters, return, örnek kullanım
  
- [ ] **docs/api/compiler_cli.md**
  - [ ] Command line interface dokümantasyonu
  - [ ] Flags ve options
  - [ ] Output file format
  - [ ] Error messages açıklaması

### Phase 19.5: Code Cleanup & Refactoring (YZ_74)

**TAHMİNİ SÜRE:** 1.5-2 saat  
**ÖNCELİK:** Orta

- [ ] **Remove Dead Code**
  - [ ] Kullanılmayan fonksiyonları temizle
  - [ ] Eski TTO kodlarını temizle (migration tamamlandıysa)
  - [ ] Debug print statements temizle
  - [ ] Commented-out code temizle
  
- [ ] **Code Formatting**
  - [ ] Tutarlı indentation (4 spaces)
  - [ ] Tutarlı brace style
  - [ ] Tutarlı naming conventions
  
- [ ] **Comment Quality**
  - [ ] Tüm public fonksiyonlara header comments
  - [ ] Karmaşık algoritmalara inline comments
  - [ ] TODO/FIXME/HACK yorumlarını gözden geçir

### Phase 19.6: Testing & Validation (YZ_75)

**TAHMİNİ SÜRE:** 1.5-2 saat  
**ÖNCELİK:** Çok Yüksek

- [ ] **Test Coverage Analysis**
  - [ ] Mevcut test suite gözden geçir
  - [ ] Eksik test senaryoları belirle
  - [ ] Edge case testleri ekle
  
- [ ] **Integration Tests**
  - [ ] End-to-end compile tests
  - [ ] Both backend validation (LLVM vs x86-64)
  - [ ] Runtime library integration tests
  
- [ ] **Performance Benchmarks**
  - [ ] Basit benchmark suite oluştur
  - [ ] LLVM vs x86-64 performans karşılaştırması
  - [ ] Compile time metrics
  - [ ] Runtime performance metrics

---

## 🚀 Phase 16: Advanced LLVM Features

**DURUM: 🔵 BAŞLAMADI**  
**TAHMİNİ SÜRE:** 6-8 saat (küçük YZ oturumlarına bölünmüş)  
**ÖNCELİK:** Orta

**AMAÇ:** LLVM optimizasyonları, debug info ve gelişmiş özellikler.

### Phase 16.1: Optimization Flags (YZ_76)

**TAHMİNİ SÜRE:** 1.5-2 saat

- [ ] `-O0` flag (no optimization)
- [ ] `-O1` flag (basic optimization)
- [ ] `-O2` flag (moderate optimization)
- [ ] `-O3` flag (aggressive optimization)
- [ ] LLVM PassManager integration
- [ ] Performance benchmarks

### Phase 16.2: Debug Information (YZ_77)

**TAHMİNİ SÜRE:** 2-2.5 saat

- [ ] DWARF debug info generation
- [ ] Source location tracking
- [ ] Line number mapping
- [ ] Variable name preservation
- [ ] GDB/LLDB compatibility

### Phase 16.3: Better Error Messages (YZ_78)

**TAHMİNİ SÜRE:** 1.5-2 saat

- [ ] Enhanced parser error messages
- [ ] Source location in errors
- [ ] Color-coded error output
- [ ] Suggestion system (did you mean?)
- [ ] Multi-error reporting

### Phase 16.4: LLVM IR Metadata (YZ_79)

**TAHMİNİ SÜRE:** 1-1.5 saat

- [ ] Function metadata
- [ ] Type metadata
- [ ] Optimization hints
- [ ] Custom attributes

---

## 📊 Phase 18: Array Support

**DURUM: 🔵 BAŞLAMADI**  
**TAHMİNİ SÜRE:** 10-12 saat (küçük YZ oturumlarına bölünmüş)  
**ÖNCELİK:** Orta-Yüksek

**AMAÇ:** Array ve list desteği eklemek.

### Phase 18.1: Array Literals (YZ_80)

**TAHMİNİ SÜRE:** 2-2.5 saat

- [ ] Lexer: `[`, `]`, `,` token support
- [ ] Parser: array literal parsing
- [ ] LLVM backend: array allocation
- [ ] x86-64 backend: array allocation
- [ ] Test: `nums: Array<Int> = [1, 2, 3]`

### Phase 18.2: Array Indexing (YZ_81)

**TAHMİNİ SÜRE:** 2-2.5 saat

- [ ] Parser: `arr[index]` syntax
- [ ] LLVM backend: GEP (GetElementPtr) instruction
- [ ] x86-64 backend: offset calculation
- [ ] Test: `x = arr[0]`, `arr[i] = 42`

### Phase 18.3: Bounds Checking (YZ_82)

**TAHMİNİ SÜRE:** 1.5-2 saat

- [ ] Runtime bounds check function
- [ ] Error handling (out of bounds)
- [ ] Optional bounds checking flag
- [ ] Performance overhead analysis

### Phase 18.4: Array Operations (YZ_83)

**TAHMİNİ SÜRE:** 2-2.5 saat

- [ ] `array.length` property
- [ ] `array.push(value)` method
- [ ] `array.pop()` method
- [ ] Dynamic resizing logic

### Phase 18.5: Multi-dimensional Arrays (YZ_84)

**TAHMİNİ SÜRE:** 2-2.5 saat

- [ ] 2D array syntax: `arr: Array<Array<Int>>`
- [ ] Nested indexing: `arr[i][j]`
- [ ] Memory layout optimization
- [ ] Test suite

---

## 🎯 Önerilen YZ Oturumları Sırası (Phase 19 Öncelikli)

1. ✅ **YZ_69** - Phase 17 String Comparison Bug Fix + Self-hosting Plan + Glossary (TAMAMLANDI)
2. ✅ **YZ_70** - Phase 19.1: README.md Güncelleme (Atlandı - Sonraya bırakıldı)
3. ✅ **YZ_71** - Phase 19.2: ARCHITECTURE.md Detaylandırma (TAMAMLANDI - 14 Aralık 2025)
4. 📝 **YZ_72** - Phase 19.3: Tutorial Oluşturma (2-2.5 saat) ← SİRADAKİ
5. 📝 **YZ_73** - Phase 19.4: API Documentation (1.5-2 saat)
6. 📝 **YZ_74** - Phase 19.5: Code Cleanup & Refactoring (1.5-2 saat)
7. 📝 **YZ_75** - Phase 19.6: Testing & Validation (1.5-2 saat)
8. 📝 **YZ_76** - Phase 19.1: README.md Güncelleme (geri dönülecek)
9. 🚀 **YZ_77** - Phase 16.1: Optimization Flags (1.5-2 saat)
10. 🚀 **YZ_78** - Phase 16.2: Debug Information (2-2.5 saat)
11. 🚀 **YZ_79** - Phase 16.3: Better Error Messages (1.5-2 saat)
12. 🚀 **YZ_80** - Phase 16.4: LLVM IR Metadata (1-1.5 saat)
13. 📊 **YZ_81** - Phase 18.1: Array Literals (2-2.5 saat)
14. 📊 **YZ_82** - Phase 18.2: Array Indexing (2-2.5 saat)
15. 📊 **YZ_83** - Phase 18.3: Bounds Checking (1.5-2 saat)
16. 📊 **YZ_84** - Phase 18.4: Array Operations (2-2.5 saat)
17. 📊 **YZ_85** - Phase 18.5: Multi-dimensional Arrays (2-2.5 saat)

**Toplam Tahmini Süre:** ~30-35 saat (17 YZ oturumu)
**Hedef:** 31 Aralık 2025 - Stage 0 Complete

---

**Son Güncelleme:** 14 Aralık 2025  
**YZ Session:** YZ_71 ✅ TAMAMLANDI  
**Durum:** Phase 15 ✅ Complete | Phase 17 ✅ Complete (100%) | Phase 19.2 ✅ Complete  
**Next:** YZ_72 - Phase 19.3 (Tutorial Creation)
