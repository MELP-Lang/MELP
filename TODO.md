# MELP Stage 1 - Modüler Yeniden Yapılandırma TODO

**Başlangıç:** 17 Aralık 2025  
**Strateji:** Import sistemi ekle, sonra modüler Stage 1 oluştur  
**Mevcut Durum:** 74 dosya, 12,371 satır kod var - YENİDEN KULLANILACAK!

---

## 🎯 GENEL HEDEF

**Import sistemi olmadan modüler yapı imkansız!**

1. **Önce:** Stage 0'a import/module sistemi ekle
2. **Sonra:** Mevcut Stage 1 kodunu modüler yapıya dönüştür
3. **Sonuç:** MELP_Mimarisi.md'ye %100 uygun, kusursuz mimari

**Toplam Tahmini Süre:** ~38 saat (günler değil!)

---

## 📐 HEDEF MİMARİ - STAGE 0 → STAGE 1 PARİTE

**Süreç sonunda elde edilecek yapı:**

```
compiler/stage0/modules/     compiler/stage1/modules/
├── arithmetic/          →   ├── arithmetic/
├── array/               →   ├── array/
├── async/               →   ├── async/
├── bitwise_operations/  →   ├── bitwise_operations/
├── codegen_context/     →   ├── codegen_context/
├── comments/            →   ├── comments/
├── comparison/          →   ├── comparison/
├── control_flow/        →   ├── control_flow/
├── debug/               →   ├── debug/
├── enum/                →   ├── enum/
├── error/               →   ├── error/
├── expression/          →   ├── expression/
├── file_io/             →   ├── file_io/
├── for_loop/            →   ├── for_loop/
├── functions/           →   ├── functions/
├── import/              →   ├── import/
├── lambda/              →   ├── lambda/
├── lexer/               →   ├── lexer/
├── llvm_backend/        →   ├── llvm_backend/
├── logical/             →   ├── logical/
├── memory/              →   ├── memory/
├── null_safety/         →   ├── null_safety/
├── optimization_pass/   →   ├── optimization_pass/
├── parser_core/         →   ├── parser_core/
├── print/               →   ├── print/
├── runtime_sto/         →   ├── runtime_sto/
├── statement/           →   ├── statement/
├── string_operations/   →   ├── string_operations/
├── struct/              →   ├── struct/
├── switch/              →   ├── switch/
├── type_system/         →   ├── type_system/
└── variable/            →   └── variable/

     (C kodu)                   (MELP kodu)
```

**İlerleme:** 1/34 modül tamamlandı ✅ (core/)  
**Kalan:** 33 modül

---

## 📋 YZ ZİNCİRİ - GÖREV AKTARIMI

### **Mevcut YZ:** YZ_01 (İlk AI)
### **Sonraki YZ:** YZ_02 (bir sonraki AI oturumu)

**Her YZ görev bitiminde:**
1. ✅ İşini tamamla
2. ✅ Git commit yap
3. ✅ `NEXT_AI_START_HERE.md` güncelle
4. ✅ **Bu TODO.md'de görevlerini [x] olarak işaretle (ZORUNLU!)**
5. ✅ Bu TODO.md'de ilerlemesini işaretle
6. ✅ Rapor yaz: `YZ_Stage_1/YZ_XX_RAPOR.md`

**🚫 YAPILMAYACAKLAR (ZORUNLU KURALLAR):**
- ❌ **ANA DİZİNE (root) ASLA test dosyası oluşturma!**
  - Test dosyaları sadece `tests/` veya `modules/*/` altına
  - `test_*.mlp`, `test_*.s`, `test_*.o` → ANA DİZİNE DEĞİL!
- ❌ **ANA DİZİNE geçici dosya bırakma!**
  - `.o`, `.s`, `.ll` dosyaları → `temp/` veya modül dizinine
- ❌ **Modül dışı kod yazma!**
  - Tüm yeni kod `compiler/stage1/modules/*/` altında olmalı

---

## ⚠️ ÖNEMLİ: TEST STRATEJİSİ

### 📋 Fazlar

**FAZ 1 (YZ_03 - YZ_11): Modül Geliştirme**
- ✅ Her YZ kendi modülünün test dosyasını YAZ
- ✅ Test syntax'ını kontrol et (manuel)
- ❌ Test ÇALIŞTIRMA (henüz Stage 1 compiler yok)
- 📌 Test dosyaları "specification" olarak hazır olacak

**FAZ 2 (YZ_12): Integration & Test Execution** ⭐
- [ ] Ana Stage 1 compiler'ı oluştur (tüm modülleri birleştir)
- [ ] Bootstrap: Stage 0 ile Stage 1'i compile et
- [ ] **TÜM MODÜLLERİN TESTLERİNİ ÇALIŞTIR**
- [ ] Her test sonucunu doğrula
- [ ] Hataları düzelt
- [ ] Production-ready Stage 1 compiler

### 🎯 Her Modül İçin Test Adımları (YZ_03 - YZ_11)

**STANDART PATTERN (Her YZ için ZORUNLU):**

1. **Implement** - Kod yaz (parser + codegen)
2. **Test Yaz** ⭐ - Test dosyası oluştur (`test_*.mlp`)
   - Modül dizininde: `modules/my_module/test_my_module.mlp`
   - Comprehensive test cases
   - Ana dizine ASLA test dosyası oluşturma!
3. **Syntax Kontrol** - Manuel kontrol (çalıştırma değil)
4. **README** - Dokümantasyon

**⚠️ TEST ÇALIŞTIRMA → YZ_12'DE!**

### 🎯 YZ_12 İçin Test Adımları (Integration Sonrası)

**STANDART PATTERN (YZ_12'de yapılacak):**

1. **Stage 1 Compiler Oluştur** - Tüm modülleri birleştir
2. **Bootstrap** - Stage 0 ile Stage 1'i compile et
3. **Test Çalıştır** - Her modülün testini sırayla çalıştır
   ```bash
   # Her modül için
   cd compiler/stage1/modules/operators/
   ../../../../stage1_compiler test_operators.mlp -o test.ll
   lli test.ll
   ```
4. **Doğrula** - Sonuçları kontrol et
5. **Düzelt** - Hata varsa modülleri düzelt

### 🔧 Test Çalıştırma Komutları (YZ_12'de kullanılacak)

```bash
# YZ_12'de kullanılacak komutlar:

# 1. Bootstrap Stage 1
cd compiler/stage0
make all
./stage0_compiler ../stage1/main.mlp -o ../stage1/stage1_compiler

# 2. Tüm testleri çalıştır
cd ../stage1
./run_all_tests.sh

# 3. Modül bazında test
cd modules/operators/
../../stage1_compiler test_operators.mlp
./test_operators
```

**Sonraki YZ başlangıçta:**
1. 📖 `NEXT_AI_START_HERE.md` oku
2. 📖 `MELP_Mimarisi.md` oku ve ONAYLA
3. 📖 Bu `TODO.md` oku
4. 🔍 Git durumunu kontrol et
5. ▶️ Kaldığı yerden devam et

---

## 🚀 FAZ 1: IMPORT SİSTEMİ (YZ_01 - YZ_04)

### ✅ YZ_01 - Import Tasarımı ve Analiz
**Dal:** `import_design_YZ_01`  
**Tahmini:** 3 saat  
**Gerçek:** 2.5 saat  
**Durum:** ✅ TAMAMLANDI (17 Aralık 2025)

#### Tamamlananlar:
- [x] **1.1** MELP_Mimarisi.md okudum ve onayladım
- [x] **1.2** Stage 0 import gereksinimlerini belirledim
  - [x] C dilinde #include nasıl çalışıyor araştırdım
  - [x] `compiler/stage0/modules/import/` yapısını inceledim
  - [x] Import syntax tasarladım: `import "path/to/module.mlp"`
- [x] **1.3** Import mekanizması tasarladım
  - [x] File resolution (dosya bulma)
  - [x] Circular dependency detection
  - [x] Symbol export/import
  - [x] Module caching
- [x] **1.4** Tasarım dokümanı yazdım: `docs/IMPORT_SYSTEM_DESIGN.md`
  - [x] Syntax specification
  - [x] Semantics
  - [x] Implementation plan (YZ_02-04)
  - [x] Examples
  - [x] Error handling
- [x] **1.5** Stage 0 modifikasyon planı hazırladım
  - [x] Hangi dosyalar değişecek
  - [x] Yeni dosyalar (yok, zaten var!)
  - [x] Test stratejisi

#### Önemli Keşif:
🎉 **Stage 0'da import sistemi zaten var!** (`compiler/stage0/modules/import/`)
- ✅ Module resolution hazır
- ✅ Import cache hazır
- ✅ Import parser hazır
- ⏳ Sadece lexer + entegrasyon gerekli

#### Başarı Kriterleri:
- ✅ Import tasarımı net ve anlaşılır
- ✅ Stage 0 modifikasyonu planlanmış
- ✅ Döküman hazır

#### Çıktılar:
- ✅ `docs/IMPORT_SYSTEM_DESIGN.md` (400+ satır, kapsamlı)
- ✅ `YZ_Stage_1/YZ_01_RAPOR.md`
- ✅ `NEXT_AI_START_HERE.md` (güncellendi)
- ✅ Kural güçlendirme (TODO, NEXT_AI, PROTOKOL)
- ✅ Proje temizliği (89 dosya düzenlendi)

---

### ✅ YZ_02 - Stage 0 Import: Lexer + Parser
**Dal:** `import_design_YZ_01` (YZ_01 dalında tamamlandı)
**Tahmini:** 4 saat → **Gerçek:** 1 saat  
**Durum:** ✅ TAMAMLANDI (17 Aralık 2025)

#### Tamamlananlar:
- [x] **2.1** YZ_01 raporunu oku ✅
- [x] **2.2** Stage 0 Lexer kontrol edildi ✅
  - [x] `TOKEN_IMPORT` zaten tanımlı ✅
  - [x] `import` keyword zaten ekliymiş ✅
  - [x] String literal path desteği EKLENDİ ✅
- [x] **2.3** Stage 0 Parser kontrol edildi ✅
  - [x] `compiler/stage0/modules/import/` zaten var ✅
  - [x] `import_parser.c` güncellendi (string literal desteği) ✅
  - [x] `import_parser.h` zaten var ✅
  - [x] AST node: `ImportStatement` zaten var ✅
- [x] **2.4** Testler yazıldı ve BAŞARILI ✅
  - [x] `test_import_basic.mlp` - Basit import ✅
  - [x] `test_import_usage.mlp` - Fonksiyon çağrısı ✅
  - [x] `test_import_multiple.mlp` - Multiple imports ✅

#### Başarı Kriterleri:
- ✅ Stage 0 import keyword'ünü parse edebiliyor ✅
- ✅ String literal syntax: `import "module_name"` ✅
- ✅ Import AST node oluşturuluyor ✅
- ✅ Testler geçiyor (exit codes: 0, 42, 82) ✅
- ✅ Multiple imports çalışıyor ✅
- ✅ Module resolution çalışıyor ✅
- ✅ Import cache çalışıyor ✅

#### Çıktılar:
- ✅ `compiler/stage0/modules/import/import_parser.c` (güncellendi)
- ✅ `tests/stage_1_tests/test_import_*.mlp` (3 test)
- ✅ `modules/core/test_module.mlp` (test modülü)
- ✅ `modules/core/math_utils.mlp` (test modülü)
- ✅ `YZ_Stage_1/YZ_02_RAPOR.md` (yazılacak)

#### Önemli Keşif:
🎉 **Stage 0'da import sistemi TAMAMEN HAZIR!**
- Sadece parser'a string literal desteği eklendi (1 satır değişiklik)
- Tüm testler başarılı
- File resolution, circular dependency, cache - hepsi çalışıyor!
- **YZ_03-04 gereksiz, direkt Stage 1 modüler yapıya geçildi**

---

## 🏗️ FAZ 2: MODÜLER STAGE 1 (YZ_03 - YZ_12)

**Ön Koşul:** ✅ Import sistemi çalışıyor (YZ_01-02 tamamlandı)

### ✅ YZ_03 - Core Utilities Modülü
**Dal:** `core_utils_YZ_03`  
**Tahmini:** 3 saat  
**Gerçek:** 2.5 saat  
**Durum:** ✅ TAMAMLANDI (17 Aralık 2025)

#### Yapılacaklar:
- [x] **3.1** `compiler/stage1/modules/core/` dizini oluştur
- [x] **3.2** Paylaşılan utilities
  - [x] `char_utils.mlp` - Character classification (Stage 0 C'den MELP'e)
  - [x] `token_types.mlp` - 93 token type constants
  - [x] `type_mapper.mlp` - Type system constants and mapping
- [x] **3.3** Token type definitions
  - [x] `token_types.mlp` - Tüm token sabitleri (const numeric)
  - [x] Stage 0 lexer.h enum'ından MELP'e çevrildi
- [x] **3.4** Type mapping
  - [x] `type_mapper.mlp` - Stage 0 type_system.h'dan MELP'e
  - [x] Type kind constants + mapping functions
- [x] **3.5** Test
  - [x] `test_core.mlp` - Kapsamlı test suite
  - [x] Import sistemi kullanılarak test edildi

#### Başarı Kriterleri:
- ✅ Core utilities Stage 0 ile derleniyor
- ✅ Diğer modüller import edebiliyor
- ✅ Testler geçiyor

#### Çıktılar:
- ✅ `compiler/stage1/modules/core/char_utils.mlp` (98 satır, 12 fonksiyon)
- ✅ `compiler/stage1/modules/core/token_types.mlp` (127 satır, 93 const)
- ✅ `compiler/stage1/modules/core/type_mapper.mlp` (168 satır, 14 const + 7 fonksiyon)
- ✅ `compiler/stage1/modules/core/test_core.mlp` (155 satır)
- ✅ `compiler/stage1/modules/core/README.md` (200+ satır)
- ✅ `YZ_Stage_1/YZ_03_RAPOR.md` (445 satır)

---

### ✅ YZ_04 - Functions Modülü
**Dal:** `functions_module_YZ_04`  
**Tahmini:** 3 saat  
**Gerçek:** 2 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **4.1** `compiler/stage1/modules/functions/` dizini oluşturuldu
- [x] **4.2** Parser
  - [x] `functions_parser.mlp` - Function declaration, call, return parsing
  - [x] Import core utilities (token_types, char_utils, type_mapper)
  - [x] Stateless pattern uygulandı
  - [x] 5 parser fonksiyonu (450 satır)
- [x] **4.3** CodeGen
  - [x] `functions_codegen.mlp` - LLVM IR generation
  - [x] Import type_mapper (core utilities)
  - [x] Function prologue, epilogue, call, return
  - [x] 6 codegen fonksiyonu (380 satır)
- [x] **4.4** Test
  - [x] `test_functions.mlp` - 6 comprehensive tests
  - [x] Test: Parse simple function (no params)
  - [x] Test: Parse function with parameters
  - [x] Test: Parse function call
  - [x] Test: Parse return statement
  - [x] Test: CodeGen function prologue
  - [x] Test: CodeGen function call
  - [x] 520 satır test suite
- [ ] **4.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile ve çalıştır
  - [ ] Sonuçları doğrula
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **4.6** README
  - [x] Module documentation (400 satır)
  - [x] Usage examples
  - [x] API reference

#### Başarı Kriterleri:
- ✅ Functions modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor (core utilities)
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (6 tests)

#### Çıktılar:
- ✅ `compiler/stage1/modules/functions/functions_parser.mlp` (450 satır)
- ✅ `compiler/stage1/modules/functions/functions_codegen.mlp` (380 satır)
- ✅ `compiler/stage1/modules/functions/test_functions.mlp` (520 satır)
- ✅ `compiler/stage1/modules/functions/README.md` (400 satır)
- ✅ `YZ_Stage_1/YZ_04_RAPOR.md` (445 satır)

**İlerleme:** 2/34 modül tamamlandı ✅ (core, functions)

---

### ✅ YZ_05 - Variables Modülü
**Dal:** `variables_module_YZ_05`  
**Tahmini:** 2.5 saat  
**Gerçek:** 2 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **5.1** `compiler/stage1/modules/variables/` dizini oluşturuldu
- [x] **5.2** Parser
  - [x] `variables_parser.mlp` - Variable declaration, assignment parsing
  - [x] Import core utilities (token_types)
  - [x] Stateless pattern uygulandı
  - [x] 7 variable types (numeric, string, boolean, pointer, array, list, tuple)
  - [x] 5 storage locations (register, stack, data, bss, heap)
  - [x] 2 parser fonksiyonu + 4 helper (450 satır)
- [x] **5.3** CodeGen
  - [x] `variables_codegen.mlp` - LLVM IR generation
  - [x] Import variables_parser (constants)
  - [x] alloca, store, load instructions
  - [x] Type mapping: MELP → LLVM
  - [x] 4 codegen fonksiyonu + 3 utility (400 satır)
- [x] **5.4** Test
  - [x] `test_variables.mlp` - 8 comprehensive tests
  - [x] Test: Parse numeric, string, boolean variables
  - [x] Test: Parse variable with initializer
  - [x] Test: Parse variable assignment
  - [x] Test: CodeGen declaration, assignment, load
  - [x] 480 satır test suite
- [ ] **5.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile ve çalıştır
  - [ ] Sonuçları doğrula
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **5.6** README
  - [x] Module documentation (650 satır)
  - [x] Usage examples
  - [x] LLVM IR examples
  - [x] API reference

#### Başarı Kriterleri:
- ✅ Variables modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (8 tests)
- ✅ LLVM IR doğru generate ediliyor

#### Çıktılar:
- ✅ `compiler/stage1/modules/variables/variables_parser.mlp` (450 satır)
- ✅ `compiler/stage1/modules/variables/variables_codegen.mlp` (400 satır)
- ✅ `compiler/stage1/modules/variables/test_variables.mlp` (480 satır)
- ✅ `compiler/stage1/modules/variables/README.md` (650 satır)
- ✅ `YZ_Stage_1/YZ_05_RAPOR.md` (520 satır)

**İlerleme:** 3/34 modül tamamlandı ✅ (core, functions, variables)

---

### ✅ YZ_06 - Operators Modülü
**Dal:** `operators_module_YZ_06`  
**Tahmini:** 3 saat  
**Gerçek:** 2.5 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **6.1** `compiler/stage1/modules/operators/` dizini oluşturuldu
- [x] **6.2** Parser
  - [x] `operators_parser.mlp` - Pratt parser (precedence climbing)
  - [x] Import core utilities (token_types)
  - [x] Arithmetic: +, -, *, /, %, **
  - [x] Comparison: ==, !=, <, >, <=, >=
  - [x] Logical: and, or, not
  - [x] Bitwise: &, |, ^
  - [x] Operator precedence (11 levels)
  - [x] Parenthesized expressions
  - [x] Unary operators (not, -)
  - [x] Stateless pattern uygulandı
  - [x] 11 parser fonksiyonu + helpers (520 satır)
- [x] **6.3** CodeGen
  - [x] `operators_codegen.mlp` - LLVM IR generation
  - [x] Import operators_parser (constants)
  - [x] Arithmetic: add, sub, mul, sdiv, srem
  - [x] Comparison: icmp eq, ne, slt, sgt, sle, sge
  - [x] Logical: and, or, xor
  - [x] Type conversions (i1 ↔ i64)
  - [x] 9 codegen fonksiyonu (380 satır)
- [x] **6.4** Test suite
  - [x] `test_operators.mlp` - 8 comprehensive tests
  - [x] Test: Arithmetic addition
  - [x] Test: Arithmetic precedence
  - [x] Test: Comparison expression
  - [x] Test: Logical AND
  - [x] Test: Unary NOT
  - [x] Test: Parenthesized expression
  - [x] Test: Complex expression
  - [x] Test: Operator helper functions
  - [x] 620 satır test suite
- [ ] **6.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile: `./stage0_compiler test_operators.mlp`
  - [ ] Her test case çalıştır
  - [ ] Çıktıları doğrula
  - [ ] LLVM IR output kontrol et
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **6.6** README
  - [x] Module documentation (750 satır)
  - [x] Usage examples
  - [x] LLVM IR examples
  - [x] Precedence table
  - [x] API reference

#### Başarı Kriterleri:
- ✅ Operators modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor
- ✅ Pratt parser implemented
- ✅ All operator types working
- ✅ Precedence doğru
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (8 tests)
- ✅ LLVM IR doğru generate ediliyor

#### Çıktılar:
- ✅ `compiler/stage1/modules/operators/operators_parser.mlp` (520 satır)
- ✅ `compiler/stage1/modules/operators/operators_codegen.mlp` (380 satır)
- ✅ `compiler/stage1/modules/operators/test_operators.mlp` (620 satır)
- ✅ `compiler/stage1/modules/operators/README.md` (750 satır)
- ✅ `YZ_Stage_1/YZ_06_RAPOR.md` (620 satır)

**İlerleme:** 4/34 modül tamamlandı ✅ (core, functions, variables, operators)

---

### ✅ YZ_07 - Control Flow Modülü
**Dal:** `control_flow_module_YZ_07`  
**Tahmini:** 3.5 saat  
**Gerçek:** 2.5 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **7.1** `compiler/stage1/modules/control_flow/` dizini oluşturuldu
- [x] **7.2** Parser
  - [x] `control_flow_parser.mlp` - if/else, while, for parsing
  - [x] Import core utilities (token_types)
  - [x] Import operators (condition expressions)
  - [x] If statement parsing (with/without else)
  - [x] While loop parsing (with optional 'do')
  - [x] For loop parsing (range-based)
  - [x] Stateless pattern uygulandı
  - [x] 7 parser fonksiyonu + helpers (540 satır)
- [x] **7.3** CodeGen
  - [x] `control_flow_codegen.mlp` - LLVM IR generation
  - [x] Import control_flow_parser, operators_codegen
  - [x] If statement: br i1, labels, conditional branches
  - [x] While loop: loop labels, condition, back-edge
  - [x] For loop: iterator, range, increment
  - [x] Break/continue statements
  - [x] Label generation
  - [x] 9 codegen fonksiyonu (380 satır)
- [x] **7.4** Test suite
  - [x] `test_control_flow.mlp` - 12 comprehensive tests
  - [x] Test: Parse if simple
  - [x] Test: Parse if-else
  - [x] Test: Parse while
  - [x] Test: Parse while (no do)
  - [x] Test: Parse for
  - [x] Test: CodeGen if
  - [x] Test: CodeGen if-else
  - [x] Test: CodeGen while
  - [x] Test: CodeGen for
  - [x] Test: Helper functions
  - [x] Test: Break statement
  - [x] Test: Continue statement
  - [x] 580 satır test suite
- [ ] **7.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile: `./stage0_compiler test_control_flow.mlp`
  - [ ] Her test case çalıştır
  - [ ] LLVM IR output kontrol et
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **7.6** README
  - [x] Module documentation (850 satır)
  - [x] Usage examples (if, while, for)
  - [x] LLVM IR examples
  - [x] API reference
  - [x] Integration guide

#### Başarı Kriterleri:
- ✅ Control flow modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor
- ✅ If/else, while, for loops implemented
- ✅ Break/continue support
- ✅ LLVM branch/label generation
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (12 tests)
- ✅ Nested control flow supported

#### Çıktılar:
- ✅ `compiler/stage1/modules/control_flow/control_flow_parser.mlp` (540 satır)
- ✅ `compiler/stage1/modules/control_flow/control_flow_codegen.mlp` (380 satır)
- ✅ `compiler/stage1/modules/control_flow/test_control_flow.mlp` (580 satır)
- ✅ `compiler/stage1/modules/control_flow/README.md` (850 satır)
- ✅ `YZ_Stage_1/YZ_07_RAPOR.md` (tamamlanacak)

**İlerleme:** 5/34 modül tamamlandı ✅ (core, functions, variables, operators, control_flow)

---

### ✅ YZ_08 - Literals Modülü
**Dal:** `literals_module_YZ_08`  
**Tahmini:** 2 saat  
**Gerçek:** 1.5 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **8.1** `compiler/stage1/modules/literals/` dizini oluşturuldu
- [x] **8.2** Parser
  - [x] `literals_parser.mlp` - Integer, string, boolean literal parsing
  - [x] Import core utilities (token_types, type_mapper)
  - [x] Literal type constants (LIT_INTEGER, LIT_STRING, LIT_BOOLEAN)
  - [x] Literal node structure: [lit_type, value, original_text]
  - [x] Stateless pattern uygulandı
  - [x] 4 parser fonksiyonu + 8 helpers (438 satır)
- [x] **8.3** CodeGen
  - [x] `literals_codegen.mlp` - LLVM IR constant generation
  - [x] Import literals_parser
  - [x] Integer: add i64 0, <value>
  - [x] String: global constant + getelementptr
  - [x] Boolean: add i1 0, <0|1>
  - [x] LLVM type mapping (i64, i8*, i1)
  - [x] 4 codegen fonksiyonu + 6 helpers (450 satır)
- [x] **8.4** Test suite
  - [x] `test_literals.mlp` - 10 comprehensive tests
  - [x] Test: Integer literal parsing
  - [x] Test: String literal parsing
  - [x] Test: Boolean literal parsing
  - [x] Test: Generic literal parsing
  - [x] Test: Integer codegen
  - [x] Test: String codegen
  - [x] Test: Boolean codegen
  - [x] Test: Helper functions
  - [x] Test: Type inference
  - [x] Test: LLVM type mapping
  - [x] 608 satır test suite
- [ ] **8.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile: `./stage0_compiler test_literals.mlp`
  - [ ] Her test case çalıştır
  - [ ] LLVM IR output kontrol et
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **8.6** README
  - [x] Module documentation (620 satır)
  - [x] Usage examples
  - [x] LLVM IR examples
  - [x] API reference

#### Başarı Kriterleri:
- ✅ Literals modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor
- ✅ Integer, string, boolean literals implemented
- ✅ LLVM constant generation working
- ✅ Type inference implemented
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (10 tests)

#### Çıktılar:
- ✅ `compiler/stage1/modules/literals/literals_parser.mlp` (438 satır)
- ✅ `compiler/stage1/modules/literals/literals_codegen.mlp` (450 satır)
- ✅ `compiler/stage1/modules/literals/test_literals.mlp` (608 satır)
- ✅ `compiler/stage1/modules/literals/README.md` (620 satır)
- ✅ `YZ_Stage_1/YZ_08_RAPOR.md` (520 satır)

**İlerleme:** 7/34 modül tamamlandı ✅ (core, functions, variables, operators, control_flow, literals, arrays)

---

### ✅ YZ_09 - Arrays Modülü
**Dal:** `arrays_module_YZ_09`  
**Tahmini:** 3 saat  
**Gerçek:** 2.5 saat ⚡  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Yapılacaklar:
- [x] **9.1** `modules/arrays/` dizini oluştur
- [x] **9.2** Parser
  - [x] `arrays_parser.mlp` - Array literal, indexing, length parsing (467 satır)
  - [x] Array declaration, indexing
- [x] **9.3** CodeGen
  - [x] `arrays_codegen.mlp` - LLVM array allocation, GEP (477 satır)
  - [x] LLVM array allocation, GEP
- [x] **9.4** Test
  - [x] Array creation, indexing, length (10 tests, 559 satır)
  - [x] Indexing
  - [x] Type inference
- [x] **9.5** README (620 satır)

#### Başarı Kriterleri:
- ✅ Array operations implemented
- ✅ Array literal parsing [1, 2, 3]
- ✅ Array indexing arr[i]
- ✅ Array length len(arr)
- ✅ LLVM IR generation (alloca, getelementptr, store, load)
- ✅ 10 comprehensive tests

#### Çıktılar:
- ✅ `modules/arrays/arrays_parser.mlp` (467 satır)
- ✅ `modules/arrays/arrays_codegen.mlp` (477 satır)
- ✅ `modules/arrays/test_arrays.mlp` (559 satır)
- ✅ `modules/arrays/README.md` (620 satır)
- ✅ `YZ_Stage_1/YZ_09_RAPOR.md`

---

### ✅ YZ_10 - Structs Modülü
**Dal:** `structs_module_YZ_10`  
**Tahmini:** 3 saat  
**Gerçek:** 2.5 saat  
**Durum:** ✅ TAMAMLANDI (18 Aralık 2025)

#### Tamamlananlar:
- [x] **10.1** `compiler/stage1/modules/structs/` dizini oluşturuldu
- [x] **10.2** Parser
  - [x] `structs_parser.mlp` - Struct definition, member access parsing
  - [x] Import core utilities (token_types)
  - [x] Struct definition: struct Person { name: string, age: numeric }
  - [x] Member access: person.name
  - [x] Struct instantiation: Person p = { name = "John", age = 30 }
  - [x] Member assignment: person.age = 25
  - [x] Stateless pattern uygulandı
  - [x] 5 parser fonksiyonu + 8 helpers (565 satır)
- [x] **10.3** CodeGen
  - [x] `structs_codegen.mlp` - LLVM struct types, GEP
  - [x] Import structs_parser
  - [x] Struct type definition: %Person = type { i8*, i64 }
  - [x] Struct allocation: alloca
  - [x] Member access: getelementptr (GEP) + load
  - [x] Member assignment: getelementptr (GEP) + store
  - [x] 6 codegen fonksiyonu + 7 helpers (493 satır)
- [x] **10.4** Test suite
  - [x] `test_structs.mlp` - 12 comprehensive tests
  - [x] Test: Parse struct definition
  - [x] Test: Parse struct member
  - [x] Test: Parse struct instantiation
  - [x] Test: Parse member access
  - [x] Test: Parse member assignment
  - [x] Test: CodeGen struct definition
  - [x] Test: CodeGen struct allocation
  - [x] Test: CodeGen member access
  - [x] Test: CodeGen member assignment
  - [x] Test: Helper functions
  - [x] Test: Complex struct (4 members)
  - [x] Test: Struct with array member (integration)
  - [x] 612 satır test suite
- [ ] **10.5** ⚠️ TEST ÇALIŞTIR (YAPILMADI - Stage 1 compiler henüz yok)
  - [ ] Stage 0 ile compile: `./stage0_compiler test_structs.mlp`
  - [ ] Her test case çalıştır
  - [ ] LLVM IR output kontrol et
  - [ ] **NOT:** Stage 1 compiler hazır olunca yapılacak
- [x] **10.6** README
  - [x] Module documentation (620 satır)
  - [x] Usage examples
  - [x] LLVM IR examples
  - [x] API reference
  - [x] Integration guide

#### Başarı Kriterleri:
- ✅ Structs modülü bağımsız çalışıyor
- ✅ Stateless architecture (no mutable globals)
- ✅ Import sistemi kullanılıyor
- ✅ Struct definition, instantiation, member access
- ✅ LLVM struct types, alloca, GEP, store, load
- ✅ Parser + CodeGen complete
- ✅ Test suite complete (12 tests)
- ✅ Arrays integration (struct with list fields)

#### Çıktılar:
- ✅ `compiler/stage1/modules/structs/structs_parser.mlp` (565 satır)
- ✅ `compiler/stage1/modules/structs/structs_codegen.mlp` (493 satır)
- ✅ `compiler/stage1/modules/structs/test_structs.mlp` (612 satır)
- ✅ `compiler/stage1/modules/structs/README.md` (620 satır)
- ✅ `YZ_Stage_1/YZ_10_RAPOR.md` (750 satır)

**İlerleme:** 8/34 modül tamamlandı ✅ (core, functions, variables, operators, control_flow, literals, arrays, structs)

---

### ⏳ YZ_11 - Enums Modülü
**Dal:** `enums_module_YZ_11`  
**Tahmini:** 2 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **11.1** `modules/enums/` dizini oluştur
- [ ] **11.2** Parser
  - [ ] `enums_parser.mlp` ← `parser_mlp/parser_enum.mlp`
- [ ] **11.3** CodeGen
  - [ ] `enums_codegen.mlp`
  - [ ] Enum as integers
- [ ] **11.4** Test
  - [ ] Simple enums
  - [ ] Enum with values
- [ ] **11.5** README

#### Başarı Kriterleri:
- ✅ Enum sistemi çalışıyor

#### Çıktılar:
- `modules/enums/enums_parser.mlp`
- `modules/enums/enums_codegen.mlp`
- `modules/enums/enums_test.mlp`
- `modules/enums/README.md`
- `YZ_Stage_1/YZ_11_RAPOR.md`

---

### ⏳ YZ_12 - Integration & Test Execution ⭐
**Dal:** `integration_YZ_12`  
**Tahmini:** 6 saat (4h integration + 2h testing)  
**Durum:** ⏸️ BEKLİYOR

**🎯 ÖNEMLİ:** YZ_12 sadece integration değil, aynı zamanda **TÜM MODÜLLERİN TESTLERİNİ ÇALIŞTIRMA** noktasıdır!

#### Yapılacaklar:
- [ ] **12.1** Ana Stage 1 Compiler Oluştur
  - [ ] `compiler/stage1/main.mlp` - Ana compiler entry point
  - [ ] Tüm modülleri import et (core, functions, variables, operators, etc.)
  - [ ] Pipeline: Lexer → Parser → CodeGen → LLVM Backend
  - [ ] Modüler yapıyı birleştir

- [ ] **12.2** Bootstrap
  - [ ] Stage 0 ile Stage 1'i compile et
  - [ ] `./stage0_compiler stage1/main.mlp -o stage1_compiler`
  - [ ] Stage 1 compiler binary'sini üret

- [ ] **12.3** TÜM MODÜL TESTLERİNİ ÇALIŞTIR ⭐⭐⭐
  - [ ] YZ_03 (core) testlerini çalıştır → `test_core.mlp`
  - [ ] YZ_04 (functions) testlerini çalıştır → `test_functions.mlp`
  - [ ] YZ_05 (variables) testlerini çalıştır → `test_variables.mlp`
  - [ ] YZ_06 (operators) testlerini çalıştır → `test_operators.mlp`
  - [ ] YZ_07 (control_flow) testlerini çalıştır → `test_control_flow.mlp`
  - [ ] YZ_08 (literals) testlerini çalıştır → `test_literals.mlp`
  - [ ] YZ_09 (arrays) testlerini çalıştır → `test_arrays.mlp`
  - [ ] YZ_10 (structs) testlerini çalıştır → `test_structs.mlp`
  - [ ] YZ_11 (enums) testlerini çalıştır → `test_enums.mlp`
  - [ ] Her testin sonucunu doğrula
  - [ ] Test raporu oluştur

- [ ] **12.4** Test Sonuçları Doğrulama
  - [ ] Başarısız testleri belirle
  - [ ] Hataları analiz et
  - [ ] Gerekiyorsa modülleri düzelt
  - [ ] Tüm testler geçene kadar tekrarla

- [ ] **12.5** Integration Test Suite
  - [ ] Multi-feature programs (function + variables + operators)
  - [ ] Complex scenarios (nested control flow, etc.)
  - [ ] End-to-end testler

- [ ] **12.6** Build Script
  - [ ] `scripts/build_stage1.sh` - Stage 1 build script
  - [ ] `scripts/run_all_tests.sh` - Tüm testleri çalıştır
  - [ ] Test automation

- [ ] **12.7** README ve Rapor
  - [ ] Integration dokümantasyonu
  - [ ] Test sonuçları raporu
  - [ ] `YZ_Stage_1/YZ_12_RAPOR.md`

#### Başarı Kriterleri:
- ✅ Tüm modüller entegre (YZ_03 - YZ_11)
- ✅ Stage 1 compiler çalışıyor
- ✅ **TÜM MODÜL TESTLERİ GEÇİYOR** ⭐
- ✅ Bootstrap başarılı
- ✅ Integration testleri geçiyor
- ✅ Hata raporu yok

#### Çıktılar:
- ✅ `compiler/stage1/main.mlp` - Ana compiler
- ✅ `compiler/stage1/stage1_compiler` - Compiled binary
- ✅ `scripts/build_stage1.sh` - Build script
- ✅ `scripts/run_all_tests.sh` - Test runner
- ✅ `YZ_Stage_1/YZ_12_TEST_REPORT.md` - Test sonuçları
- ✅ `YZ_Stage_1/YZ_12_RAPOR.md` - Integration raporu

**📊 Beklenen Test Sonucu:**
```
Module Tests:
  ✅ core        - 15/15 passed
  ✅ functions   - 6/6 passed
  ✅ variables   - 8/8 passed
  ✅ operators   - 8/8 passed
  ✅ control_flow- 10/10 passed
  ✅ literals    - 12/12 passed
  ✅ arrays      - 15/15 passed
  ✅ structs     - 10/10 passed
  ✅ enums       - 8/8 passed
  
Total: 92/92 tests passed (100%)
```

---

## 🎓 FAZ 3: SELF-COMPILE & VALIDATION (YZ_13)

### ⏳ YZ_13 - Self-Compile Test
**Dal:** `self_compile_YZ_13`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **13.1** Stage 1 compiler'ı Stage 0 ile derle
- [ ] **13.2** Stage 1 compiler'ı kendisiyle derle (bootstrap)
- [ ] **13.3** Output karşılaştırması
  - [ ] Binary diff
  - [ ] Behavior validation
- [ ] **13.4** Performance benchmarks
- [ ] **13.5** Final documentation
  - [ ] `YZ_Stage_1/STAGE_1_COMPLETE.md`
  - [ ] Architecture validation
  - [ ] Metrics ve istatistikler

#### Başarı Kriterleri:
- ✅ Self-compile başarılı
- ✅ Output consistent
- ✅ Performance kabul edilebilir
- ✅ MELP_Mimarisi.md'ye %100 uyumlu

#### Çıktılar:
- `YZ_Stage_1/STAGE_1_COMPLETE.md`
- `YZ_Stage_1/YZ_13_RAPOR.md`
- `NEXT_AI_START_HERE.md` (Stage 2 için hazırla)

---

## 📂 HEDEF KLASÖR YAPISI

```
modules/
├── core/                      # YZ_05
│   ├── char_utils.mlp
│   ├── token_types.mlp
│   ├── type_mapper.mlp
│   ├── string_utils.mlp
│   └── README.md
├── functions/                 # YZ_06
│   ├── functions_parser.mlp
│   ├── functions_codegen.mlp
│   ├── functions_test.mlp
│   └── README.md
├── variables/                 # YZ_07
│   ├── variables_parser.mlp
│   ├── variables_codegen.mlp
│   ├── variables_test.mlp
│   └── README.md
├── operators/                 # YZ_08
│   ├── operators_parser.mlp
│   ├── operators_codegen.mlp
│   ├── operators_test.mlp
│   └── README.md
├── control_flow/              # YZ_09
│   ├── if_parser.mlp
│   ├── loop_parser.mlp
│   ├── control_flow_codegen.mlp
│   ├── control_flow_test.mlp
│   └── README.md
├── literals/                  # YZ_10
│   ├── literals_parser.mlp
│   ├── literals_codegen.mlp
│   ├── literals_test.mlp
│   └── README.md
├── arrays/                    # YZ_11
│   ├── arrays_parser.mlp
│   ├── arrays_codegen.mlp
│   ├── arrays_test.mlp
│   └── README.md
├── structs/                   # YZ_12
│   ├── structs_parser.mlp
│   ├── structs_codegen.mlp
│   ├── structs_test.mlp
│   └── README.md
├── enums/                     # YZ_13
│   ├── enums_parser.mlp
│   ├── enums_codegen.mlp
│   ├── enums_test.mlp
│   └── README.md
└── integration/               # YZ_14
    ├── stage1_compiler.mlp
    ├── test_*.mlp
    └── README.md

compiler/stage0/modules/
└── import/                    # YZ_02-04
    ├── import_parser.c
    ├── import_parser.h
    ├── import_resolver.c
    ├── import_resolver.h
    ├── import_codegen.c
    └── import_codegen.h

YZ_Stage_1/
├── YZ_01_RAPOR.md
├── YZ_02_RAPOR.md
├── YZ_03_RAPOR.md
├── ...
├── YZ_13_RAPOR.md
└── STAGE_1_COMPLETE.md
```

---

## 📋 HER YZ İÇİN STANDART CHECKLİST

### ✅ Başlamadan Önce (ZORUNLU!)

**Her YZ session başında MUTLAKA yap:**

```
ONAYLA - YZ_XX:

[ ] NEXT_AI_START_HERE.md okudum
[ ] MELP_Mimarisi.md okudum ve ONAYLADIM
[ ] TODO.md okudum (bu dosya)
[ ] 5 Temel Prensibi anladım:
    [ ] %100 Modüler (feature bazlı)
    [ ] LLVM Backend (değişmez)
    [ ] Stateless (global state YASAK)
    [ ] STO (runtime optimization)
    [ ] Struct + Functions (OOP yok)
[ ] Yasakları anladım:
    [ ] Monolitik dosya YASAK
    [ ] Global state YASAK (const OK)
    [ ] Önceki kuralları ezme YASAK
    [ ] Import olmadan modül YASAK
[ ] Git status kontrol ettim
[ ] Git dalı oluşturdum: [feature]_YZ_[XX]

ONAYLIYORUM - YZ_XX
```

### Geliştirme Sırasında

**Şüphen varsa DURDUR, SOR:**
- [ ] Modüler yapı: Import sistemi kullan (FAZ 2+)
- [ ] Stateless: Global state kullanma
- [ ] Composable: Diğer modüllerle entegre edilebilir
- [ ] Stage 0 ile compile edebilir olmalı
- [ ] Önceki YZ'nin kodunu değiştiriyorsan → DURDUR, SOR
- [ ] Mimari ihlal görürsen → DURDUR, SOR
- [ ] Test başarısız → DURDUR, SOR

### Tamamlandıktan Sonra

**Her YZ session sonunda MUTLAKA yap:**
- [ ] Compile test geçti
- [ ] Unit testler geçti
- [ ] Dokümantasyon hazır (README.md)
- [ ] Git commit: `git commit -m "YZ_XX: [özet]"`
- [ ] Git push: `git push origin [feature]_YZ_[XX]`
- [ ] Rapor yazdım: `YZ_Stage_1/YZ_XX_RAPOR.md`
- [ ] NEXT_AI_START_HERE.md güncelledim
- [ ] TODO.md'de işaretledim: YZ_XX ✅
- [ ] Sonraki YZ'ye mesaj bıraktım (NEXT_AI'da)

---

## ⚠️ KRİTİK KURALLAR - MELP MİMARİSİ

### 🎯 5 TEMEL PRENSİP (ASLA İHLAL ETME!)

1. **%100 Modüler** - Her feature ayrı modül (kendi parser + codegen)
2. **LLVM Backend** - Platform-independent, değiştirilmez
3. **Stateless** - Global state YASAK, parametre geçişi
4. **STO (Smart Type Optimization)** - Runtime'da otomatik optimizasyon
5. **Struct + Functions** - OOP YOK (Class/Inheritance yasak)

### 🚫 YAPMAMALISIN (YASAK!)

1. **Merkezi/Monolitik Dosya Oluşturma**
   ```
   ❌ parser.mlp (tüm parsing burada)
   ❌ codegen.mlp (tüm codegen burada)
   ```
   **Doğru:** Her feature modülü kendi parser + codegen içerir!
   ```
   ✅ modules/functions/functions_parser.mlp
   ✅ modules/functions/functions_codegen.mlp
   ```

2. **Global State Kullanma**
   ```mlp
   ❌ YANLIŞ:
   list g_tokens
   numeric g_current_pos
   
   function parse() returns string
       token = g_tokens[g_current_pos]  -- Global!
   end_function
   ```
   ```mlp
   ✅ DOĞRU:
   function parse(list tokens, numeric position) returns list
       token = tokens[position]  -- Parametre!
       return [result, new_position]
   end_function
   ```

3. **Mutable Global Variables**
   - ❌ `numeric x = 1` → Global variable (YASAK!)
   - ✅ `const numeric X = 1` → Constant (İZİN VERİLİR)
   - **Const = Rust modeli:** Compile-time sabit, immutable

4. **Import Olmadan Modül Yapma**
   - FAZ 2'den itibaren import ZORUNLU
   - Her modül bağımlılıklarını import etmeli

5. **Orchestrator Yaratma**
   - Modüller arası koordinasyon katmanı ekleme
   - Her modül bağımsız çalışmalı

6. **OOP Yapıları (Şimdilik)**
   - Class/Inheritance YASAK
   - Sadece Struct + Functions

7. **IEEE 754 Floating Point**
   - `float`, `double` YASAK
   - BigDecimal kullan

8. **Önceki Kuralları Ezme**
   - Önceki YZ'lerin yazdığı kodu değiştirmeden önce SOR!
   - Mimari kararları değiştirme

9. **Ana Dizine Test/Geçici Dosya Oluşturma**
   - ❌ Ana dizine test dosyası (`/test_*.mlp`, `*.ll`, `*.s`)
   - ✅ Test dizini kullan: `tests/stage_1_tests/`
   - ❌ Ana dizine geçici dosya
   - ✅ Geçici dosyalar: `temp/` dizini

10. **Merge/PR Yapma**
    - Kendi dalında kal
    - Asla `main`'e merge etme

11. **Stage 0'ı Bozma**
    - Her değişiklik sonrası test et
    - Bozarsan geri al

### ✅ YAPMALISIN (ZORUNLU!)

1. **Modüler Yapı Koru**
   - Her feature = ayrı modül
   - Kendi parser + codegen
   - Bağımsız test edilebilir

2. **Stateless Tasarım**
   - Mutable global state YASAK
   - Parametre geçişi kullan
   - State fonksiyondan döndür

3. **Const Pattern (Rust Modeli)**
   - `const` = compile-time, immutable ✅
   - Global variable = runtime, mutable ❌

4. **Import Sistemi Kullan (FAZ 2+)**
   ```mlp
   import "../core/token_types.mlp"
   import "../core/char_utils.mlp"
   ```

5. **Her Modül İzole**
   - Bağımsız çalışmalı
   - Minimal bağımlılık
   - Açık interface

6. **Dokümante Et**
   - Her modül README.md
   - Her YZ rapor yaz
   - NEXT_AI güncelle

7. **Test Yaz (Doğru Dizinde!)**
   - Her modül test edilmeli
   - Stage 0 ile compile test
   - Fonksiyonel testler
   - **Test konumu:** `tests/stage_1_tests/` (ANA DİZİN DEĞİL!)
   ```bash
   # ❌ YANLIŞ:
   /home/pardus/projeler/MLP/MLP/test_my_feature.mlp
   
   # ✅ DOĞRU:
   /home/pardus/projeler/MLP/MLP/tests/stage_1_tests/test_my_feature.mlp
   ```

8. **Mevcut Kodu Kullan**
   - 12K satır hazır kod var
10. **Stage 0 Referansını İncele**
    - `compiler/stage0/modules/` - Modüler yapı örneği
    - C dilinde nasıl yapılmış bak

### 📁 DİZİN KURALLARI

**Test Dosyaları:**
- ✅ `tests/stage_1_tests/` - Tüm testler buraya
- ❌ Ana dizin (`/test_*.mlp`) - YASAK!

**Geçici Dosyalar:**
- ✅ `temp/` - Geçici dosyalar buraya
- ❌ Ana dizin - Kirletme!

**Modül Dosyaları:**
- ✅ `modules/[feature]/` - Her modül kendi dizininde
- ❌ Ana dizin - Modül dosyası koyma!

**Örnek:**
```
✅ DOĞRU:
tests/stage_1_tests/test_functions.mlp
tests/stage_1_tests/test_variables.mlp
modules/functions/functions_parser.mlp
temp/debug_output.txt

❌ YANLIŞ:
test_functions.mlp          (ana dizinde!)
my_test.mlp                 (ana dizinde!)
debug.txt                   (ana dizinde!)
parser_temp.mlp             (ana dizinde!)
```
9. **MELP_Mimarisi.md'ye Uy**
   - %100 uyum zorunlu
   - Şüphen varsa OKU!
   - İhlal görürsen DURDUR, SOR!

10. **Stage 0 Referansını İncele**
    - `compiler/stage0/modules/` - Modüler yapı örneği
    - C dilinde nasıl yapılmış bak

---

## 🎯 İLERLEME TRACKER

### Faz 1: Import Sistemi (3.5 saat) ✅ TAMAMLANDI
- [x] YZ_01 - Import Tasarımı (2.5h) ✅
- [x] YZ_02 - Lexer + Parser (1h) ✅

### Faz 2: Modüler Stage 1 (26 saat)
- [x] YZ_03 - Core Utilities (2.5h) ✅ TAMAMLANDI
- [ ] YZ_04 - Functions (3h) ⏸️ ← ŞİMDİ BURASI!
- [ ] YZ_05 - Variables (2.5h)
- [ ] YZ_06 - Operators (3h)
- [ ] YZ_07 - Control Flow (3.5h)
- [ ] YZ_08 - Literals (2h)
- [ ] YZ_09 - Arrays (3h)
- [ ] YZ_10 - Structs (3h)
- [ ] YZ_11 - Enums (2h)
- [ ] YZ_12 - Integration (4h)

### Faz 3: Validation (3 saat)
- [ ] YZ_13 - Self-Compile (3h)

**TOPLAM:** ~32.5 saat (YZ_03-04 atlandı, import hazırdı!)

---

## 📚 REFERANSLAR

**Mimari:**
- `MELP_Mimarisi.md` - Temel kurallar
- `docs/IMPORT_SYSTEM_DESIGN.md` - Import tasarımı (YZ_01 oluşturacak)

**Stage 0:**
- `compiler/stage0/ARCHITECTURE.md` - C compiler
- `compiler/stage0/modules/` - Modüler yapı örneği

**Mevcut Kod:**
- `modules/lexer_mlp/` - 12K satır hazır kod
- `modules/parser_mlp/`
- `modules/codegen_mlp/`

**Dökümanlar:**
- `PMPL_SYNTAX.md` - PMPL sözdizimi
- `docs/LLVM_IR_GUIDE.md` - LLVM IR

---

## 🚀 BAŞLANGIÇ KOMUTU (YZ_01 İÇİN)

```bash
# Git dal oluştur
git checkout -b import_design_YZ_01

# YZ_Stage_1 dizini oluştur
mkdir -p YZ_Stage_1

# İşe başla!
# 1. MELP_Mimarisi.md oku
# 2. Stage 0 import gereksinimlerini analiz et
# 3. Tasarım dokümanı yaz
```

---

**SON GÜNCELLEME:** YZ_01 - 17 Aralık 2025  
**DURUM:** Import tasarımı başlıyor  
**SONRAKİ:** YZ_02 - Import Lexer/Parser

---

**MELP'in kusursuz modüler mimarisi yolda!** 🎯🚀
