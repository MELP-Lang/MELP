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

## 📋 YZ ZİNCİRİ - GÖREV AKTARIMI

### **Mevcut YZ:** YZ_01 (İlk AI)
### **Sonraki YZ:** YZ_02 (bir sonraki AI oturumu)

**Her YZ görev bitiminde:**
1. ✅ İşini tamamla
2. ✅ Git commit yap
3. ✅ `NEXT_AI_START_HERE.md` güncelle
4. ✅ Bu TODO.md'de ilerlemesini işaretle
5. ✅ Rapor yaz: `YZ_Stage_1/YZ_XX_RAPOR.md`

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

### ⏳ YZ_02 - Stage 0 Import: Lexer + Parser
**Dal:** `import_lexer_parser_YZ_02`  
**Tahmini:** 4 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **2.1** YZ_01 raporunu oku
- [ ] **2.2** Stage 0 Lexer'a import token ekle
  - [ ] `TOKEN_IMPORT` tanımla
  - [ ] `import` keyword'ünü tanı
  - [ ] String literal path desteği
- [ ] **2.3** Stage 0 Parser'a import parsing ekle
  - [ ] `compiler/stage0/modules/import/` dizini oluştur
  - [ ] `import_parser.c` - Import statement parsing
  - [ ] `import_parser.h` - Header
  - [ ] AST node: `ImportStatement`
- [ ] **2.4** Test yaz
  - [ ] `tests/import/test_import_syntax.mlp`
  - [ ] Basit import parse testi

#### Başarı Kriterleri:
- ✅ Stage 0 import keyword'ünü parse edebiliyor
- ✅ Import AST node oluşturuluyor
- ✅ Testler geçiyor

#### Çıktılar:
- `compiler/stage0/modules/import/import_parser.c`
- `compiler/stage0/modules/import/import_parser.h`
- `tests/import/test_import_syntax.mlp`
- `YZ_Stage_1/YZ_02_RAPOR.md`

---

### ⏳ YZ_03 - Stage 0 Import: File Resolution + Symbol Table
**Dal:** `import_resolution_YZ_03`  
**Tahmini:** 5 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **3.1** File resolution sistemi
  - [ ] `import_resolver.c` - Dosya bulma
  - [ ] Relative path support: `import "../core/utils.mlp"`
  - [ ] Absolute path support (opsiyonel)
  - [ ] File caching (aynı dosya 1 kez import)
- [ ] **3.2** Symbol export/import
  - [ ] Public functions (default export)
  - [ ] Symbol table integration
  - [ ] Namespace prefix (optional): `utils.function_name`
- [ ] **3.3** Circular dependency detection
  - [ ] Import graph oluştur
  - [ ] Cycle detection algoritması
  - [ ] Hata mesajı: "Circular import detected"
- [ ] **3.4** Test suite
  - [ ] Single file import
  - [ ] Multiple imports
  - [ ] Circular import (hata testi)

#### Başarı Kriterleri:
- ✅ Import edilen dosyalar bulunuyor ve parse ediliyor
- ✅ Circular dependency yakalanıyor
- ✅ Symbol'ler erişilebilir

#### Çıktılar:
- `compiler/stage0/modules/import/import_resolver.c`
- `compiler/stage0/modules/import/import_resolver.h`
- `tests/import/test_multi_import.mlp`
- `tests/import/test_circular_import.mlp`
- `YZ_Stage_1/YZ_03_RAPOR.md`

---

### ⏳ YZ_04 - Stage 0 Import: CodeGen + Integration
**Dal:** `import_codegen_YZ_04`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **4.1** Import codegen (LLVM IR)
  - [ ] Import edilen dosyaların IR'ını birleştir
  - [ ] Symbol resolution
  - [ ] Linkage (external declarations)
- [ ] **4.2** Stage 0 compiler entegrasyonu
  - [ ] Main compiler pipeline'a import modülünü ekle
  - [ ] Multi-file compilation support
- [ ] **4.3** End-to-end test
  - [ ] İki dosya: `main.mlp` ve `utils.mlp`
  - [ ] `main.mlp` import eder, `utils.mlp`'den fonksiyon çağırır
  - [ ] Compile ve çalıştır
- [ ] **4.4** Döküman güncelle
  - [ ] `PMPL_SYNTAX.md` - Import syntax ekle
  - [ ] `docs/IMPORT_SYSTEM_DESIGN.md` - Implementasyon notları

#### Başarı Kriterleri:
- ✅ Import sistemi çalışıyor
- ✅ Multi-file MELP programları derlenebiliyor
- ✅ Stage 0 self-compile hala çalışıyor

#### Çıktılar:
- `compiler/stage0/modules/import/import_codegen.c`
- `tests/import/test_end_to_end.mlp`
- `tests/import/utils.mlp`
- Güncellenmiş dökümanlar
- `YZ_Stage_1/YZ_04_RAPOR.md`

---

## 🏗️ FAZ 2: MODÜLER STAGE 1 (YZ_05 - YZ_14)

**Ön Koşul:** Import sistemi çalışıyor olmalı!

### ⏳ YZ_05 - Core Utilities Modülü
**Dal:** `core_utils_YZ_05`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **5.1** `modules/core/` dizini oluştur
- [ ] **5.2** Paylaşılan utilities
  - [ ] `char_utils.mlp` ← mevcut `lexer_mlp/char_utils.mlp` (taşı)
  - [ ] `string_utils.mlp` - String operations
  - [ ] `list_utils.mlp` - List operations (ihtiyaç varsa)
- [ ] **5.3** Token type definitions
  - [ ] `token_types.mlp` - Tüm token sabitleri (enum)
  - [ ] Mevcut koddan çıkar, merkezi hale getir
- [ ] **5.4** Type mapping
  - [ ] `type_mapper.mlp` ← mevcut `codegen_mlp/type_mapper.mlp`
  - [ ] MELP type → LLVM type
- [ ] **5.5** Test
  - [ ] `modules/core/test_core_utils.mlp`
  - [ ] Import ve kullan

#### Başarı Kriterleri:
- ✅ Core utilities Stage 0 ile derleniyor
- ✅ Diğer modüller import edebiliyor
- ✅ Testler geçiyor

#### Çıktılar:
- `modules/core/char_utils.mlp`
- `modules/core/token_types.mlp`
- `modules/core/type_mapper.mlp`
- `modules/core/README.md`
- `YZ_Stage_1/YZ_05_RAPOR.md`

---

### ⏳ YZ_06 - Functions Modülü
**Dal:** `functions_module_YZ_06`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **6.1** `modules/functions/` dizini oluştur
- [ ] **6.2** Parser
  - [ ] `functions_parser.mlp` ← mevcut `parser_mlp/parser_func.mlp`
  - [ ] Import core utilities
  - [ ] Sadece function parsing fonksiyonlarını al
  - [ ] Gereksiz kodu temizle
- [ ] **6.3** CodeGen
  - [ ] `functions_codegen.mlp` ← mevcut `codegen_mlp/codegen_functions.mlp`
  - [ ] Import type_mapper
  - [ ] Function declaration, call, return
- [ ] **6.4** Test
  - [ ] `functions_test.mlp`
  - [ ] Test: `function add(numeric a, numeric b) returns numeric`
  - [ ] Test: `function greet(string name) returns void`
- [ ] **6.5** README
  - [ ] Module documentation
  - [ ] Usage examples

#### Başarı Kriterleri:
- ✅ Functions modülü bağımsız çalışıyor
- ✅ Stage 0 ile derleniyor
- ✅ Testler geçiyor

#### Çıktılar:
- `modules/functions/functions_parser.mlp`
- `modules/functions/functions_codegen.mlp`
- `modules/functions/functions_test.mlp`
- `modules/functions/README.md`
- `YZ_Stage_1/YZ_06_RAPOR.md`

---

### ⏳ YZ_07 - Variables Modülü
**Dal:** `variables_module_YZ_07`  
**Tahmini:** 2.5 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **7.1** `modules/variables/` dizini oluştur
- [ ] **7.2** Parser
  - [ ] `variables_parser.mlp` ← `parser_mlp/parser_stmt.mlp`'den extract
  - [ ] Variable declaration parsing
  - [ ] Assignment parsing
- [ ] **7.3** CodeGen
  - [ ] `variables_codegen.mlp` ← `codegen_mlp/codegen_variable.mlp`
  - [ ] LLVM alloca, store, load
- [ ] **7.4** Test
  - [ ] Local variables
  - [ ] Type checking
- [ ] **7.5** README

#### Başarı Kriterleri:
- ✅ Variable modülü izole
- ✅ Testler geçiyor

#### Çıktılar:
- `modules/variables/variables_parser.mlp`
- `modules/variables/variables_codegen.mlp`
- `modules/variables/variables_test.mlp`
- `modules/variables/README.md`
- `YZ_Stage_1/YZ_07_RAPOR.md`

---

### ⏳ YZ_08 - Operators Modülü
**Dal:** `operators_module_YZ_08`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **8.1** `modules/operators/` dizini oluştur
- [ ] **8.2** Parser
  - [ ] `operators_parser.mlp` ← `parser_mlp/parser_expr.mlp`'den extract
  - [ ] Arithmetic: +, -, *, /, %
  - [ ] Comparison: ==, !=, <, >, <=, >=
  - [ ] Logical: and, or, not
- [ ] **8.3** CodeGen
  - [ ] `operators_codegen.mlp` ← Mevcut codegen dosyalarından merge:
    - `codegen_arithmetic.mlp`
    - `codegen_comparison.mlp`
    - `codegen_logical.mlp`
- [ ] **8.4** Test suite
  - [ ] Arithmetic operations
  - [ ] Comparison operations
  - [ ] Logical operations
  - [ ] Operator precedence
- [ ] **8.5** README

#### Başarı Kriterleri:
- ✅ Tüm operatörler çalışıyor
- ✅ Precedence doğru

#### Çıktılar:
- `modules/operators/operators_parser.mlp`
- `modules/operators/operators_codegen.mlp`
- `modules/operators/operators_test.mlp`
- `modules/operators/README.md`
- `YZ_Stage_1/YZ_08_RAPOR.md`

---

### ⏳ YZ_09 - Control Flow Modülü
**Dal:** `control_flow_module_YZ_09`  
**Tahmini:** 3.5 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **9.1** `modules/control_flow/` dizini oluştur
- [ ] **9.2** Parser
  - [ ] `if_parser.mlp` ← `parser_mlp/parser_control.mlp`
  - [ ] `loop_parser.mlp` ← `parser_mlp/parser_for.mlp`, while parsing
- [ ] **9.3** CodeGen
  - [ ] `control_flow_codegen.mlp` ← Merge:
    - `codegen_control.mlp`
    - `codegen_while.mlp`
    - `codegen_for.mlp`
  - [ ] LLVM branch, label generation
- [ ] **9.4** Test
  - [ ] If/else nesting
  - [ ] While loops
  - [ ] For loops
- [ ] **9.5** README

#### Başarı Kriterleri:
- ✅ Control flow yapıları çalışıyor
- ✅ Nested yapılar destekleniyor

#### Çıktılar:
- `modules/control_flow/if_parser.mlp`
- `modules/control_flow/loop_parser.mlp`
- `modules/control_flow/control_flow_codegen.mlp`
- `modules/control_flow/control_flow_test.mlp`
- `modules/control_flow/README.md`
- `YZ_Stage_1/YZ_09_RAPOR.md`

---

### ⏳ YZ_10 - Literals Modülü
**Dal:** `literals_module_YZ_10`  
**Tahmini:** 2 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **10.1** `modules/literals/` dizini oluştur
- [ ] **10.2** Parser
  - [ ] `literals_parser.mlp` ← `lexer_mlp/tokenize_literals.mlp`
  - [ ] Numeric, string, boolean literals
- [ ] **10.3** CodeGen
  - [ ] `literals_codegen.mlp` ← `codegen_mlp/codegen_literal.mlp`
  - [ ] LLVM constant generation
- [ ] **10.4** Test
  - [ ] Integer, float, string, boolean
- [ ] **10.5** README

#### Başarı Kriterleri:
- ✅ Tüm literal türleri destekleniyor

#### Çıktılar:
- `modules/literals/literals_parser.mlp`
- `modules/literals/literals_codegen.mlp`
- `modules/literals/literals_test.mlp`
- `modules/literals/README.md`
- `YZ_Stage_1/YZ_10_RAPOR.md`

---

### ⏳ YZ_11 - Arrays Modülü
**Dal:** `arrays_module_YZ_11`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **11.1** `modules/arrays/` dizini oluştur
- [ ] **11.2** Parser
  - [ ] `arrays_parser.mlp` ← Mevcut parser'dan extract
  - [ ] Array declaration, indexing
- [ ] **11.3** CodeGen
  - [ ] `arrays_codegen.mlp` ← `codegen_mlp/codegen_arrays.mlp`
  - [ ] LLVM array allocation, GEP
- [ ] **11.4** Test
  - [ ] Array creation
  - [ ] Indexing
  - [ ] Multi-dimensional
- [ ] **11.5** README

#### Başarı Kriterleri:
- ✅ Array operations çalışıyor

#### Çıktılar:
- `modules/arrays/arrays_parser.mlp`
- `modules/arrays/arrays_codegen.mlp`
- `modules/arrays/arrays_test.mlp`
- `modules/arrays/README.md`
- `YZ_Stage_1/YZ_11_RAPOR.md`

---

### ⏳ YZ_12 - Structs Modülü
**Dal:** `structs_module_YZ_12`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **12.1** `modules/structs/` dizini oluştur
- [ ] **12.2** Parser
  - [ ] `structs_parser.mlp` ← `parser_mlp/parser_struct.mlp`
  - [ ] Struct definition, member access
- [ ] **12.3** CodeGen
  - [ ] `structs_codegen.mlp` ← Mevcut koddan
  - [ ] LLVM struct types, GEP
- [ ] **12.4** Test
  - [ ] Struct definition
  - [ ] Member access
  - [ ] Nested structs
- [ ] **12.5** README

#### Başarı Kriterleri:
- ✅ Struct yapıları çalışıyor

#### Çıktılar:
- `modules/structs/structs_parser.mlp`
- `modules/structs/structs_codegen.mlp`
- `modules/structs/structs_test.mlp`
- `modules/structs/README.md`
- `YZ_Stage_1/YZ_12_RAPOR.md`

---

### ⏳ YZ_13 - Enums Modülü
**Dal:** `enums_module_YZ_13`  
**Tahmini:** 2 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **13.1** `modules/enums/` dizini oluştur
- [ ] **13.2** Parser
  - [ ] `enums_parser.mlp` ← `parser_mlp/parser_enum.mlp`
- [ ] **13.3** CodeGen
  - [ ] `enums_codegen.mlp`
  - [ ] Enum as integers
- [ ] **13.4** Test
  - [ ] Simple enums
  - [ ] Enum with values
- [ ] **13.5** README

#### Başarı Kriterleri:
- ✅ Enum sistemi çalışıyor

#### Çıktılar:
- `modules/enums/enums_parser.mlp`
- `modules/enums/enums_codegen.mlp`
- `modules/enums/enums_test.mlp`
- `modules/enums/README.md`
- `YZ_Stage_1/YZ_13_RAPOR.md`

---

### ⏳ YZ_14 - Integration & Orchestration
**Dal:** `integration_YZ_14`  
**Tahmini:** 4 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **14.1** `modules/integration/` dizini oluştur
- [ ] **14.2** Compiler orchestrator
  - [ ] `stage1_compiler.mlp` - Ana compiler
  - [ ] Tüm modülleri import et
  - [ ] Pipeline: Lexer → Parser → CodeGen
- [ ] **14.3** End-to-end test suite
  - [ ] Multi-feature programs
  - [ ] Complex scenarios
- [ ] **14.4** Build script
  - [ ] `scripts/build_stage1_modular.sh`
  - [ ] Modüler yapıyı compile et
- [ ] **14.5** README

#### Başarı Kriterleri:
- ✅ Tüm modüller entegre
- ✅ Stage 1 compiler çalışıyor
- ✅ End-to-end testler geçiyor

#### Çıktılar:
- `modules/integration/stage1_compiler.mlp`
- `scripts/build_stage1_modular.sh`
- `tests/integration/test_*.mlp`
- `YZ_Stage_1/YZ_14_RAPOR.md`

---

## 🎓 FAZ 3: SELF-COMPILE & VALIDATION (YZ_15)

### ⏳ YZ_15 - Self-Compile Test
**Dal:** `self_compile_YZ_15`  
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **15.1** Stage 1 compiler'ı Stage 0 ile derle
- [ ] **15.2** Stage 1 compiler'ı kendisiyle derle (bootstrap)
- [ ] **15.3** Output karşılaştırması
  - [ ] Binary diff
  - [ ] Behavior validation
- [ ] **15.4** Performance benchmarks
- [ ] **15.5** Final documentation
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
- `YZ_Stage_1/YZ_15_RAPOR.md`
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
├── ...
├── YZ_15_RAPOR.md
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

### Faz 1: Import Sistemi (13 saat)
- [ ] YZ_01 - Import Tasarımı (3h)
- [ ] YZ_02 - Lexer + Parser (4h)
- [ ] YZ_03 - Resolution + Symbol Table (5h)
- [ ] YZ_04 - CodeGen + Integration (3h)

### Faz 2: Modüler Stage 1 (26 saat)
- [ ] YZ_05 - Core Utilities (3h)
- [ ] YZ_06 - Functions (3h)
- [ ] YZ_07 - Variables (2.5h)
- [ ] YZ_08 - Operators (3h)
- [ ] YZ_09 - Control Flow (3.5h)
- [ ] YZ_10 - Literals (2h)
- [ ] YZ_11 - Arrays (3h)
- [ ] YZ_12 - Structs (3h)
- [ ] YZ_13 - Enums (2h)
- [ ] YZ_14 - Integration (4h)

### Faz 3: Validation (3 saat)
- [ ] YZ_15 - Self-Compile (3h)

**TOPLAM:** ~38 saat (gerçekçi tahmin)

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
