# MELP Stage 1 - Template Pattern TODO

**Başlangıç:** 19 Aralık 2025  
**Üst Akıl:** YZ_ÜA_03 (3. Üst Akıl)  
**Strateji:** Import sistemi önce, Template Pattern ile modüler yapı  
**Toplam Tahmini Süre:** ~40 saat

---

## 🎯 GENEL HEDEF

**Import sistemi olmadan modüler yapı imkansız!**

1. **Önce:** Stage 0'a çalışan import execution ekle
2. **Sonra:** Stage 1 modüllerini Template Pattern ile yaz
3. **Sonuç:** Self-hosting MELP compiler

**Önceki Denemeler (Arşivde):**
- `archive/old_stage1_monolithic/` - Monolitik yapı, BAŞARISIZ
- `archive/stage1_api_attempt/` - API pattern, bellek sızıntısı riski

---

## 🎯 MİMARİ PRENSİPLER

### ✅ DOĞRU (Rust Modeli):
```mlp
-- const OK (immutable) - Rust modeli
const numeric TOKEN_FUNCTION = 1
const numeric TOKEN_IF = 2

-- Template Pattern: Tüm state parametre olarak
function parse_statement(list tokens, numeric pos) returns list
    -- Input: tokens, pos
    -- Output: [result, new_pos]
    -- NO side effects!
end_function
```

### ❌ YANLIŞ:
```mlp
-- Global mutable state YASAK
numeric g_position = 0

-- API Pattern YASAK
function set_position(numeric pos) returns void
function get_current_token() returns list
function init_parser() returns void
```

### 🔑 Temel Felsefe:
**"Her modül ölüdür, onu çağıran diriltir ve öldürür"**
- Modül kendi state tutmaz
- Caller tüm context'i parametre olarak geçirir
- Modül sonucu return eder, state değiştirmez
- STO (Smart Type Optimization) için bu ŞART

---

## 📋 YZ ZİNCİRİ - GÖREV AKTARIMI

### **Mevcut YZ:** YZ_37 (Function Call Arguments)
### **Son Güncelleme:** 19 Aralık 2025, 02:15

**YZ_36 ✅ TAMAMLANDI** - Çoklu else_if chain desteği eklendi!

**Her YZ görev bitiminde:**
1. ✅ İşini tamamla
2. ✅ Git commit yap: `git commit -m "YZ_XX: Description"`
3. ✅ `NEXT_AI_START_HERE.md` güncelle
4. ✅ **Bu TODO.md'de görevlerini [x] olarak işaretle (ZORUNLU!)**
5. ✅ Rapor yaz: `stage_1_atölye/YZ_XX_RAPOR.md`

**🚫 YAPILMAYACAKLAR (ZORUNLU KURALLAR):**
- ❌ **Global mutable state ekleme**
- ❌ **API pattern kullanma** (`set_`, `get_`, `init_`)
- ❌ **Monolitik yapı** (tek dosyada çok sorumluluk)
- ❌ **ANA DİZİNE test dosyası oluşturma**
- ❌ **Test etmeden commit**

---

## 🚀 FAZ 1: IMPORT SİSTEMİ (YZ_01 - YZ_04)

### ⏳ YZ_01 - Import Execution Analizi ve Düzeltme
**Dal:** `import_execution_YZ_01`  
**Tahmini:** 4 saat  
**Durum:** ⏳ BAŞLANMADI

#### Mevcut Durum:
- ✅ Stage 0'da import modülü var: `compiler/stage0/modules/import/`
- ✅ Path resolution çalışıyor (YZ_28 düzeltti)
- ❌ **Import execution çalışmıyor** - Modül bulunuyor ama fonksiyonlar kullanılamıyor

#### Yapılacaklar:
- [ ] **1.1** Mevcut import kodunu analiz et
  - [ ] `compiler/stage0/modules/import/import.c` incele
  - [ ] `compiler/stage0/modules/import/import_parser.c` incele
  - [ ] `compiler/stage0/modules/import/import_cache.c` incele
- [ ] **1.2** Import execution sorununu bul
  - [ ] Modül parse ediliyor mu?
  - [ ] Fonksiyonlar symbol table'a ekleniyor mu?
  - [ ] Codegen import edilen fonksiyonları görüyor mu?
- [ ] **1.3** Sorunu düzelt (Template Pattern ile)
  - [ ] Global state varsa kaldır
  - [ ] Tüm state parametrik yap
- [ ] **1.4** Test
  - [ ] Basit import testi yaz
  - [ ] Fonksiyon çağrısı testi
  - [ ] Circular import testi
- [ ] **1.5** Rapor yaz: `stage_1_atölye/YZ_01_IMPORT_RAPOR.md`

#### Test Senaryosu:
```mlp
-- helper.mlp
function helper_add(numeric a, numeric b) returns numeric
    return a + b
end_function

-- main.mlp
import "helper.mlp"

function main() returns numeric
    numeric result = helper_add(5, 3)
    println(result)  -- Beklenen: 8
    return 0
end_function
```

#### Başarı Kriterleri:
- [ ] Import edilen modüldeki fonksiyon çağrılabilir
- [ ] Circular import koruması var
- [ ] Hata mesajları anlaşılır
- [ ] Template Pattern uygulandı (global state yok)

---

### ⏳ YZ_02 - Import Symbol Table Integration
**Dal:** `import_symbols_YZ_02`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE (YZ_01'e bağlı)

#### Yapılacaklar:
- [ ] **2.1** Symbol table yapısını incele
- [ ] **2.2** Import edilen sembollerin eklenmesini sağla
- [ ] **2.3** Namespace/scope yönetimi
- [ ] **2.4** Test ve doğrulama
- [ ] **2.5** Rapor yaz: `stage_1_atölye/YZ_02_SYMBOLS_RAPOR.md`

#### Başarı Kriterleri:
- [ ] Import edilen fonksiyonlar symbol table'da
- [ ] Name collision handling
- [ ] Scope isolation

---

### ⏳ YZ_03 - Import CodeGen Integration
**Dal:** `import_codegen_YZ_03`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE (YZ_02'ye bağlı)

#### Yapılacaklar:
- [ ] **3.1** CodeGen'in import edilen sembolleri görmesini sağla
- [ ] **3.2** LLVM IR'da extern declaration
- [ ] **3.3** Linking stratejisi
- [ ] **3.4** Test ve doğrulama
- [ ] **3.5** Rapor yaz: `stage_1_atölye/YZ_03_CODEGEN_RAPOR.md`

#### Başarı Kriterleri:
- [ ] CodeGen import edilen fonksiyonları çağırabiliyor
- [ ] LLVM IR doğru üretiliyor
- [ ] Linking çalışıyor

---

### ⏳ YZ_04 - Import System Validation
**Dal:** `import_validation_YZ_04`  
**Tahmini:** 2 saat  
**Durum:** ⏳ BEKLEMEDE (YZ_03'e bağlı)

#### Yapılacaklar:
- [ ] **4.1** Kapsamlı test suite
- [ ] **4.2** Edge case'ler (circular, missing, nested)
- [ ] **4.3** Performance test
- [ ] **4.4** Dokümantasyon
- [ ] **4.5** Rapor yaz: `stage_1_atölye/YZ_04_VALIDATION_RAPOR.md`

#### Başarı Kriterleri:
- [ ] Tüm import senaryoları çalışıyor
- [ ] Error handling complete
- [ ] Dokümantasyon hazır

---

## 🏗️ FAZ 2: MODÜLER STAGE 1 (YZ_05 - YZ_14)

**Ön Koşul:** ✅ Import sistemi çalışıyor (YZ_01-04 tamamlandı)

### ⏳ YZ_05 - Core Utilities Modülü
**Dal:** `core_utils_YZ_05`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

#### Yapılacaklar:
- [ ] **5.1** `compiler/stage1/modules/core/` dizini oluştur
- [ ] **5.2** `token_types.mlp` - Tüm token sabitleri (const numeric)
- [ ] **5.3** `char_utils.mlp` - Character classification
- [ ] **5.4** `type_mapper.mlp` - MELP→LLVM type mapping
- [ ] **5.5** Test dosyası: `test_core.mlp`
- [ ] **5.6** README.md

#### Başarı Kriterleri:
- [ ] Core utilities Stage 0 ile derleniyor
- [ ] Diğer modüller import edebiliyor
- [ ] Template Pattern uygulandı
- [ ] Testler geçiyor

---

### ⏳ YZ_06 - Functions Modülü
**Dal:** `functions_module_YZ_06`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

#### Yapılacaklar:
- [ ] **6.1** `compiler/stage1/modules/functions/` dizini oluştur
- [ ] **6.2** `functions_parser.mlp` - Function declaration, call, return parsing
- [ ] **6.3** `functions_codegen.mlp` - LLVM IR generation
- [ ] **6.4** Test dosyası: `test_functions.mlp`
- [ ] **6.5** README.md

#### Başarı Kriterleri:
- [ ] Function declaration parsing
- [ ] Function call parsing
- [ ] Return statement parsing
- [ ] LLVM IR generation
- [ ] Template Pattern (no global state)

---

### ⏳ YZ_07 - Variables Modülü
**Dal:** `variables_module_YZ_07`  
**Tahmini:** 2.5 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_08 - Operators Modülü
**Dal:** `operators_module_YZ_08`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_09 - Control Flow Modülü
**Dal:** `control_flow_module_YZ_09`  
**Tahmini:** 3.5 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_10 - Literals Modülü
**Dal:** `literals_module_YZ_10`  
**Tahmini:** 2 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_11 - Arrays Modülü
**Dal:** `arrays_module_YZ_11`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_12 - Structs Modülü
**Dal:** `structs_module_YZ_12`  
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_13 - Enums Modülü
**Dal:** `enums_module_YZ_13`  
**Tahmini:** 2 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_14 - Mini Bootstrap
**Dal:** `mini_bootstrap_YZ_14`  
**Tahmini:** 4 saat  
**Durum:** ⏳ BEKLEMEDE

#### Yapılacaklar:
- [ ] **14.1** `main.mlp` oluştur (imports all modules)
- [ ] **14.2** Stage 0 ile compile et
- [ ] **14.3** Basit MELP dosyasını derle
- [ ] **14.4** Self-hosting test

---

## 🎯 FAZ 3: SELF-HOSTING (YZ_15 - YZ_16)

### ⏳ YZ_15 - Stage 0 ile Compile Test
**Tahmini:** 3 saat  
**Durum:** ⏳ BEKLEMEDE

---

### ⏳ YZ_16 - Self-Hosting Proof
**Tahmini:** 4 saat  
**Durum:** ⏳ BEKLEMEDE

#### Başarı Kriterleri:
- [ ] Stage 1 compiler kendini derleyebiliyor
- [ ] Derlenen compiler çalışıyor
- [ ] Self-hosting döngüsü tamamlandı

---

## 📁 HEDEF YAPI

```
compiler/stage1/
├── modules/
│   ├── core/
│   │   ├── token_types.mlp      -- const definitions (Rust model)
│   │   ├── char_utils.mlp       -- character utilities
│   │   └── type_mapper.mlp      -- MELP→LLVM type mapping
│   ├── functions/
│   │   ├── functions_parser.mlp
│   │   └── functions_codegen.mlp
│   ├── variables/
│   │   ├── variables_parser.mlp
│   │   └── variables_codegen.mlp
│   ├── operators/
│   │   ├── operators_parser.mlp
│   │   └── operators_codegen.mlp
│   ├── control_flow/
│   │   ├── control_flow_parser.mlp
│   │   └── control_flow_codegen.mlp
│   ├── literals/
│   │   ├── literals_parser.mlp
│   │   └── literals_codegen.mlp
│   ├── arrays/
│   │   ├── arrays_parser.mlp
│   │   └── arrays_codegen.mlp
│   ├── structs/
│   │   ├── structs_parser.mlp
│   │   └── structs_codegen.mlp
│   └── enums/
│       ├── enums_parser.mlp
│       └── enums_codegen.mlp
└── main.mlp                     -- imports all modules
```

---

## ⚠️ KRİTİK KURALLAR

### 1. Template Pattern (ZORUNLU)
```mlp
-- ✅ DOĞRU: Tüm state parametre olarak
function parse_expression(list tokens, numeric pos) returns list
    -- Process
    return [result, new_pos]
end_function

-- ❌ YANLIŞ: Global state
numeric g_pos = 0
function parse_expression(list tokens) returns list
    -- g_pos kullanıyor - YASAK!
end_function
```

### 2. Const OK, Mutable Global YASAK
```mlp
-- ✅ OK (Rust modeli - immutable)
const numeric TOKEN_FUNCTION = 1
const string VERSION = "1.0.0"

-- ❌ YASAK
numeric g_error_count = 0
list g_tokens = []
```

### 3. Modül Boyutu Limitleri
- < 200 satır: ✅ Normal
- 200-400 satır: ⚠️ Dikkatli ol
- > 400 satır: ❌ Böl!

### 4. Dosya Organizasyonu
- Her modül kendi dizininde: `modules/feature_name/`
- Parser + CodeGen ayrı dosyalarda
- Test dosyası modül dizininde: `test_feature.mlp`
- README.md her modülde

---

## 📚 REFERANS BELGELER

1. `ARCHITECTURE.md` - Genel mimari
2. `docs_tr/kurallar_kitabı_v1.md` - MELP kuralları
3. `runtime/sto/STO.md` - Smart Type Optimization
4. `docs/IMPORT_SYSTEM_DESIGN.md` - Import tasarımı (ilk Üst Akıl)

---

## 📊 İLERLEME TAKİBİ

| Faz | Görev | Durum | Süre |
|-----|-------|-------|------|
| 1 | YZ_01: Import Execution | ⏳ | 4h |
| 1 | YZ_02: Symbol Table | ⏳ | 3h |
| 1 | YZ_03: CodeGen Integration | ⏳ | 3h |
| 1 | YZ_04: Validation | ⏳ | 2h |
| 2 | YZ_05: Core | ⏳ | 3h |
| 2 | YZ_06: Functions | ⏳ | 3h |
| 2 | YZ_07: Variables | ⏳ | 2.5h |
| 2 | YZ_08: Operators | ⏳ | 3h |
| 2 | YZ_09: Control Flow | ⏳ | 3.5h |
| 2 | YZ_10: Literals | ⏳ | 2h |
| 2 | YZ_11: Arrays | ⏳ | 3h |
| 2 | YZ_12: Structs | ⏳ | 3h |
| 2 | YZ_13: Enums | ⏳ | 2h |
| 2 | YZ_14: Bootstrap | ⏳ | 4h |
| 3 | YZ_15: Compile Test | ⏳ | 3h |
| 3 | YZ_16: Self-Hosting | ⏳ | 4h |
| **TOPLAM** | | | **~48h** |

---

*Bu dosya her YZ tarafından güncel tutulmalı!*
