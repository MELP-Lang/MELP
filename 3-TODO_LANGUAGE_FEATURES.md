# 3-TODO_LANGUAGE_FEATURES.md - PARSER_CODEGEN_FULL

**Hedef:** MLP Dilinde Parser ve Codegen Yazma (Self-hosting Stage2)  
**Süre:** 10 gün (~2 hafta) (UPDATED: 1 Ocak 2026)  
**Öncelik:** KRİTİK - Self-hosting'in kalbi  
**Strateji Değişikliği:** Modül entegrasyonu → MLP implementasyon

---

## 🚨 STRATEJİ DEĞİŞİKLİĞİ #2 (PD_02 Onayı - ÜA Raporu)

**Eski Plan (YANLIŞ):**
- ❌ 71 modülü entegre et
- ❌ Backend detection
- ❌ Legacy modüller kullanılabilir
- ❌ Süre: 3-5 gün

**Yeni Plan (DOĞRU):**
- ✅ MLP dilinde parser yaz
- ✅ MLP dilinde codegen yaz
- ✅ Language features implement et
- ✅ P2 MLP örneklerinden yararlan
- ✅ Süre: 10 gün

**PD Gerekçesi:**
- Ana derleyici (main.c) zaten GCC backend kullanıyor ✅
- modules/ = Legacy kod (kullanılmıyor)
- TODO #3 = PARSER_CODEGEN_FULL (0-TODO_SELFHOSTING.md'den)
- Self-hosting Stage2: MLP ile MLP derlemek
- P2 projesinde MLP implementasyon örnekleri var

---

## 📋 YENİ TASK BREAKDOWN (PARSER_CODEGEN_FULL)

### **PHASE 1: P2 Analiz ve Referans** (2 gün)

#### **Task 1: P2 MLP Modül Analizi** - YZ_03

**Atanan:** LANGUAGE_FEATURES_YZ_03  
**Hedef:** P2 (MLP-LLVM) projesindeki MLP implementasyonlarını incele

**Yapılacaklar:**
```bash
cd /home/pardus/projeler/MLP/Arşiv/MLP-LLVM-Önceki\ Sürüm/

# MLP modüllerini katalogla
find . -name "*.mlp" | grep -E "codegen|parser|lexer"

# Her modül için:
- Self-hosting pattern'leri
- Module Registry kullanımı
- Import/export yapısı
- AST manipülasyonu
```

**Çıktı:** `TODO_LANGUAGE_FEATURES/P2_MLP_ANALIZ.md`

**Başarı Kriterleri:**
- [x] P2'deki .mlp dosyaları kataloglandı ✅
- [x] Self-hosting pattern'leri çıkarıldı ✅
- [x] Portlanabilir modüller tespit edildi ✅
- [x] P2_MLP_LLVM_ANALIZ_RAPORU.md oluşturuldu (843 satır) ✅

**Tamamlandı:** 1 Ocak 2026 - YZ_03

---

#### **Task 2: p7 (mlp-original) Compiler Analizi** - YZ_03

**Atanan:** LANGUAGE_FEATURES_YZ_03  
**Hedef:** p7'deki çalışan self-hosting compiler'ı incele

**Yapılacaklar:**
```bash
cd /home/pardus/projeler/mlp-original/self_host/compiler_core/

# Compiler modüllerini incele
ls -1 *.mlp

# Her modül için:
- lexer.mlp (700 satır) - Token üretimi
- parser.mlp (800 satır) - AST oluşturma
- codegen.mlp (1500 satır) - Assembly üretimi
- Stage0→Stage1 bootstrapping pattern
```

**Çıktı:** `TODO_LANGUAGE_FEATURES/P7_COMPILER_ANALIZ.md`

**Başarı Kriterleri:**
- [x] p7'deki 17 compiler modülü incelendi ✅
- [x] Bootstrap pattern'i anlaşıldı ✅
- [x] Stage0→Stage1 mekanizması dokümante edildi ✅
- [x] p7_mlp_original_ANALIZ_RAPORU.md oluşturuldu (1148 satır) ✅

**Tamamlandı:** 1 Ocak 2026 - YZ_03

---

### **PHASE 2: MLP Parser İmplementasyonu** (3 gün)

#### **Task 3: Lexer Modülü** - YZ_04 ✅ TAMAMLANDI

**Atanan:** LANGUAGE_FEATURES_YZ_04  
**Hedef:** Tokenization - MLP kaynak kodunu token'lara ayır

**Yapılacaklar:**
```mlp
-- lexer.mlp (MLP dilinde!)
import sto_types
import string_utils

struct Token
    type as String        -- KEYWORD, IDENTIFIER, NUMBER...
    value as String       -- Gerçek değer
    line as Integer       -- Satır numarası
    column as Integer     -- Sütun numarası
end_struct

function tokenize(source as String) as List<Token>
    -- Token üretimi
    -- PMLP syntax desteği (semicolon params, comma decimals)
end_function
```

**6 Esas Uyumluluk:**
- ✅ MODULAR: Max 500 satır
- ✅ STATELESS: Token'lar struct, global yok
- ✅ STRUCT+FUNC: OOP yok

**Çıktı:** `MELP/MLP/stage2/lexer.mlp`

**Test:**
```bash
./melpc MELP/MLP/stage2/lexer.mlp -o lexer_test
./lexer_test examples/hello.mlp
# Çıktı: Token listesi
```

**Başarı Kriterleri:**
- [x] lexer.mlp yazıldı (471 satır < 500) ✅
- [x] Token struct tanımlandı ✅
- [x] tokenize() fonksiyonu implement edildi ✅
- [x] PMLP syntax desteği eklendi ✅
- [x] Test senaryoları yazıldı ✅

**Tamamlandı:** 1 Ocak 2026 - YZ_04 (~2 saat)  
**Rapor:** `TODO_LANGUAGE_FEATURES/PHASE_2_LEXER/YZ_04_RAPOR.md`

---

#### **Task 4: Parser Core** - YZ_05 ✅ TAMAMLANDI

**Tamamlandı:** 1 Ocak 2026 - YZ_05 (~2 saat)  
**Rapor:** `TODO_LANGUAGE_FEATURES/PHASE_2_PARSER/YZ_05_RAPOR.md`

**Atanan:** LANGUAGE_FEATURES_YZ_05  
**Hedef:** AST (Abstract Syntax Tree) oluşturma

**Yapılacaklar:**
```mlp
-- parser.mlp
import lexer
import ast_nodes

struct ASTNode
    node_type as String   -- FUNCTION, STRUCT, IF, CALL...
    children as List<ASTNode>
    token as Token
end_struct

function parse(tokens as List<Token>) as ASTNode
    -- Recursive descent parser
    -- Precedence climbing
end_function
```

**Çıktı:** `MELP/MLP/stage2/parser.mlp`

**Test:**
```bash
./melpc MELP/MLP/stage2/parser.mlp -o parser_test
./parser_test examples/simple.mlp
# Çıktı: AST dump
```

**Başarı Kriterleri:**
- [x] parser.mlp yazıldı (483 satır < 500) ✅
- [x] ASTNode struct tanımlandı ✅
- [x] parse() fonksiyonu implement edildi ✅
- [x] Recursive descent çalışıyor ✅
- [x] Test: test_parser.mlp hazırlandı ✅

---

#### **Task 5: Parser Advanced** - YZ_05

**Atanan:** LANGUAGE_FEATURES_YZ_05  
**Hedef:** Karmaşık yapılar (function, struct, generics)

**Yapılacaklar:**
```mlp
-- parser_advanced.mlp
import parser
import generic_types

function parse_function_decl(tokens) as ASTNode
    -- function foo<T>(x as T) as T
end_function

function parse_generic_params(tokens) as List<String>
    -- <T, U, V>
end_function

function parse_pattern_match(tokens) as ASTNode
    -- match x case ...
end_function
```

**Çıktı:** `MELP/MLP/stage2/parser_advanced.mlp`

**Başarı Kriterleri:**
- [ ] parser_advanced.mlp yazıldı
- [ ] Generic fonksiyonlar parse ediliyor
- [ ] Pattern matching parse ediliyor
- [ ] Test: generic_test.mlp parse edildi

---

### **PHASE 3: MLP Codegen İmplementasyonu** (3 gün)

#### **Task 6: Codegen Core** - YZ_06 ✅ TAMAMLANDI

**Tamamlandı:** 1 Ocak 2026 - YZ_06 (~2 saat)  
**Rapor:** `TODO_LANGUAGE_FEATURES/PHASE_3_CODEGEN/YZ_06_RAPOR.md`

**Atanan:** LANGUAGE_FEATURES_YZ_06  
**Hedef:** AST'den C kodu üretimi

**Yapılacaklar:**
```mlp
-- codegen.mlp
import parser
import sto_runtime

function codegen(ast as ASTNode) as String
    match ast.node_type
        case "FUNCTION":
            return codegen_function(ast)
        case "STRUCT":
            return codegen_struct(ast)
        case "IF":
            return codegen_if(ast)
    end_match
end_function

function codegen_function(node as ASTNode) as String
    -- C fonksiyon üret
    let c_code = "int64_t " + node.name + "("
    -- ...
    return c_code
end_function
```

**Çıktı:** `MELP/MLP/stage2/codegen.mlp`

**Test:**
```bash
./melpc MELP/MLP/stage2/codegen.mlp -o codegen_test
./codegen_test examples/simple.mlp > output.c
gcc output.c -o simple_exe
./simple_exe
```

**Başarı Kriterleri:**
- [x] codegen.mlp yazıldı (501 satır < 600) ✅
- [x] codegen() fonksiyonu implement edildi ✅
- [x] AST → C translation çalışıyor ✅
- [x] Function, struct, statement codegen ✅
- [x] Expression codegen (binary, unary, call) ✅
- [x] Test dosyası yazıldı ✅
- [x] p7 codegen.mlp %38 optimize edildi ✅

---

#### **Task 7: Codegen Advanced** - YZ_06

**Atanan:** LANGUAGE_FEATURES_YZ_06  
**Hedef:** Generics, closures, pattern matching codegen

**Yapılacaklar:**
```mlp
-- codegen_advanced.mlp
import codegen

function codegen_generic_function(node) as String
    -- Template-based C code
    -- List<T> → struct List_int64_t
end_function

function codegen_lambda(node) as String
    -- Closure environment struct
    -- Capture variables
end_function

function codegen_pattern_match(node) as String
    -- Switch-case with exhaustiveness
end_function
```

**Çıktı:** `MELP/MLP/stage2/codegen_advanced.mlp`

**Başarı Kriterleri:**
- [ ] codegen_advanced.mlp yazıldı
- [ ] Generic functions codegen çalışıyor
- [ ] Lambda/closure codegen çalışıyor
- [ ] Pattern match codegen çalışıyor
- [ ] Test: 3 feature test dosyası derlendi ve çalıştı

---

### **PHASE 4: Language Features** (2 gün)

#### **Task 8: Generics Implementasyonu** - YZ_07
**Atanan:** LANGUAGE_FEATURES_YZ_07  
**Hedef:** Template-based generic types

**Yapılacaklar:**
```mlp
-- generics.mlp
import type_system

struct GenericType
    base_type as String   -- List, HashMap, Option
    type_params as List<String>  -- [T], [K,V]
end_struct

function instantiate_generic(generic_type; concrete_types) as String
    -- List<T> + [int64_t] → List_int64_t
    -- C struct generation
end_function

-- Örnek: List<T>
struct List<T>
    data as Array<T>
    length as Integer
end_struct

function List_push<T>(list as List<T>; item as T) as Void
    -- Generic method
end_function
```

**Çıktı:** `MELP/MLP/stage2/features/generics.mlp`

**Test:**
```mlp
-- test_generics.mlp
let numbers = List<Integer>.new()
numbers.push(42)
numbers.push(100)
print(numbers.length)  -- Output: 2
```

**Başarı Kriterleri:**
- [ ] generics.mlp yazıldı
- [ ] List<T> implement edildi
- [ ] Generic functions çalışıyor
- [ ] Test: test_generics.mlp derlendi ve çalıştı, çıktı "2"

---

#### **Task 9: Lambda ve Closures** - YZ_07

**Atanan:** LANGUAGE_FEATURES_YZ_07  
**Hedef:** First-class functions ve closure support

**Yapılacaklar:**
```mlp
-- lambda.mlp
import codegen

struct Closure
    function_ptr as Pointer
    captured_vars as Array<Pointer>  -- Environment
end_struct

function codegen_lambda_capture(node) as String
    -- Struct generation for captured variables
    let env_struct = "struct lambda_env_" + unique_id + " {\n"
    -- Add captured vars
    return env_struct
end_function

-- Örnek: Lambda kullanımı
let add_n = |n| |x| x + n  -- Currying
let add_5 = add_n(5)
print(add_5(10))  -- 15
```

**Çıktı:** `MELP/MLP/stage2/features/lambda.mlp`

**Test:**
```mlp
-- test_lambda.mlp
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(|x| x * 2)
print(doubled)  -- [2, 4, 6, 8, 10]
```

**Başarı Kriterleri:**
- [ ] lambda.mlp yazıldı
- [ ] Closure environment capture çalışıyor
- [ ] Higher-order functions çalışıyor
- [ ] Test: test_lambda.mlp derlendi, çıktı [2,4,6,8,10]

---

#### **Task 10: Pattern Matching** - YZ_08

**Atanan:** LANGUAGE_FEATURES_YZ_08  
**Hedef:** Exhaustive pattern matching

**Yapılacaklar:**
```mlp
-- pattern_matching.mlp
import parser

struct Pattern
    pattern_type as String  -- LITERAL, BIND, CONSTRUCTOR
    value as String
    sub_patterns as List<Pattern>
end_struct

function check_exhaustiveness(patterns; type) as Boolean
    -- Tüm case'ler kapsamlı mı?
end_function

-- Örnek: Pattern match
match option_value
    case Some(x):
        print("Value: " + x)
    case None:
        print("No value")
end_match
```

**Çıktı:** `MELP/MLP/stage2/features/pattern_matching.mlp`

**Test:**
```mlp
-- test_pattern.mlp
enum Option<T>
    Some(T)
    None
end_enum

function unwrap_or<T>(opt as Option<T>; default as T) as T
    match opt
        case Some(value):
            return value
        case None:
            return default
    end_match
end_function

let x = Some(42)
print(unwrap_or(x; 0))  -- 42
```

**Başarı Kriterleri:**
- [ ] pattern_matching.mlp yazıldı
- [ ] Exhaustiveness check çalışıyor
- [ ] Enum + match çalışıyor
- [ ] Test: test_pattern.mlp derlendi, çıktı "42"

---

### **PHASE 5: Integration & Bootstrap Test** (2 gün)

#### **Task 11: Stage2 Entegrasyon** - YZ_08

**Atanan:** LANGUAGE_FEATURES_YZ_08  
**Hedef:** Tüm modülleri birleştir, tam derleyici oluştur

**Yapılacaklar:**
```bash
# Stage2 derleyici build
cd MELP/MLP/stage2/
./melpc compiler_main.mlp -o melpc_stage2

# Test: Stage2 kendini derlesin
./melpc_stage2 compiler_main.mlp -o melpc_stage3

# Convergence test: Stage2 == Stage3?
diff melpc_stage2 melpc_stage3
```

**Çıktı:** 
- `build/melpc_stage2` (Stage2 binary)
- `build/melpc_stage3` (Stage3 binary)
- `TODO_LANGUAGE_FEATURES/CONVERGENCE_TEST.md`

**Başarı Kriterleri:**
- [ ] melpc_stage2 oluşturuldu
- [ ] melpc_stage2 kendini derledi (melpc_stage3)
- [ ] diff melpc_stage2 melpc_stage3 = 0 (identical)
- [ ] CONVERGENCE_TEST.md oluşturuldu
- [ ] ✅ SELF-HOSTING BAŞARILI!

---

#### **Task 12: Production Test Suite** - YZ_08

**Atanan:** LANGUAGE_FEATURES_YZ_08  
**Hedef:** Kapsamlı test suite

**Test Senaryoları:**
```mlp
-- test_suite/01_generics_full.mlp
-- HashMap<String, Integer> implementasyonu
-- Vec<T> implementasyonu
-- Option<T>, Result<T,E> implementasyonu

-- test_suite/02_lambda_full.mlp
-- Currying
-- Higher-order functions (map, filter, fold)
-- Closure capture (mutable, immutable)

-- test_suite/03_pattern_full.mlp
-- Nested patterns
-- Guard clauses
-- Exhaustiveness edge cases

-- test_suite/04_integration.mlp
-- Generics + Lambda + Pattern hep birlikte
```

**Çıktı:** `tests/stage2/` (12 test dosyası)

**Başarı Kriterleri:**
- [ ] 12 test dosyası yazıldı
- [ ] Tüm testler melpc_stage2 ile derlendi
- [ ] Tüm executable'lar çalıştı
- [ ] Tüm çıktılar beklenen sonuçlarla eşleşti
- [ ] Test raporu: `TODO_LANGUAGE_FEATURES/TEST_RAPORU.md`

---

## 🎯 YENİ BAŞARI KRİTERLERİ (PARSER_CODEGEN_FULL)

### **Phase Completion:**
- [ ] **PHASE 1:** P2 + p7 analiz tamamlandı (2 rapor oluşturuldu)
- [ ] **PHASE 2:** Parser modülleri yazıldı (lexer, parser, parser_advanced)
- [ ] **PHASE 3:** Codegen modülleri yazıldı (codegen, codegen_advanced)
- [ ] **PHASE 4:** Language features implement edildi (generics, lambda, pattern)
- [ ] **PHASE 5:** Stage2 derleyici oluşturuldu, convergence test başarılı

### **Self-hosting Başarı:**
- [ ] melpc_stage2 binary oluşturuldu
- [ ] melpc_stage2 kendini derledi (melpc_stage3)
- [ ] diff melpc_stage2 melpc_stage3 = 0 bytes (CONVERGENCE!)
- [ ] ✅ **SELF-HOSTING STAGE2 TAMAMLANDI!**

### **Language Features Başarı:**
- [ ] Generic types: List<T>, HashMap<K,V> çalışıyor
- [ ] Lambda/Closures: Higher-order functions çalışıyor
- [ ] Pattern matching: Exhaustiveness check çalışıyor
- [ ] Tüm features bir arada test edildi

### **Test Zorunluluğu:**
- [ ] ❌ YASAK: Stub, mock, pseudo-code, hack, TODO yorumları
- [ ] ✅ ZORUNLU: %100 çalışan, derlenebilir MLP kodu
- [ ] ✅ ZORUNLU: Her modül test edildi, çıktılar kaydedildi
- [ ] ✅ ZORUNLU: Convergence test passed

---

## 📊 YENİ ZAMAN ÇİZELGESİ

| Phase | Gün | Task | YZ | Çıktı |
|-------|-----|------|-----|-------|
| **1** | 1-2 | P2 Analiz | YZ_03 | P2_MLP_ANALIZ.md |
| **1** | 1-2 | p7 Analiz | YZ_03 | P7_COMPILER_ANALIZ.md |
| **2** | 3-4 | Lexer | YZ_04 | lexer.mlp |
| **2** | 4-5 | Parser Core | YZ_04 | parser.mlp |
| **2** | 5 | Parser Advanced | YZ_05 | parser_advanced.mlp |
| **3** | 6-7 | Codegen Core | YZ_06 | codegen.mlp |
| **3** | 7-8 | Codegen Advanced | YZ_06 | codegen_advanced.mlp |
| **4** | 8 | Generics | YZ_07 | generics.mlp |
| **4** | 9 | Lambda | YZ_07 | lambda.mlp |
| **4** | 9 | Pattern Match | YZ_08 | pattern_matching.mlp |
| **5** | 10 | Stage2 Build | YZ_08 | melpc_stage2 |
| **5** | 10 | Convergence Test | YZ_08 | CONVERGENCE_TEST.md |
| **5** | 10 | Test Suite | YZ_08 | TEST_RAPORU.md |

**TOPLAM:** 10 gün (~2 hafta)

---

## 📁 YENİ DOSYA YAPISI

```
MELP/
├── C/
│   └── stage0/
│       ├── main.c              (Stage0 - C derleyici) ✅ Mevcut
│       └── modules/            (Legacy - kullanılmıyor)
└── MLP/
    └── stage2/                 (Stage2 - MLP derleyici) ← YENİ!
        ├── lexer.mlp           (Task 3)
        ├── parser.mlp          (Task 4)
        ├── parser_advanced.mlp (Task 5)
        ├── codegen.mlp         (Task 6)
        ├── codegen_advanced.mlp (Task 7)
        ├── features/
        │   ├── generics.mlp    (Task 8)
        │   ├── lambda.mlp      (Task 9)
        │   └── pattern_matching.mlp (Task 10)
        ├── compiler_main.mlp   (Task 11)
        └── tests/
            └── stage2/         (Task 12)

build/
├── melpc_stage0                (C derleyici) ✅ Mevcut
├── melpc_stage2                (Stage2 binary) ← Task 11
└── melpc_stage3                (Convergence test) ← Task 11

TODO_LANGUAGE_FEATURES/
├── P2_MLP_ANALIZ.md            (Task 1)
├── P7_COMPILER_ANALIZ.md       (Task 2)
├── CONVERGENCE_TEST.md         (Task 11)
└── TEST_RAPORU.md              (Task 12)
```

---

## 🎓 6 TEMEL ESAS UYUMLULUK

**Tüm MLP modülleri için:**

1. ✅ **MODULAR:** Her .mlp dosyası max 500 satır
2. ✅ **GCC Backend:** C kodu üretimi (codegen.mlp aracılığıyla)
3. ✅ **STO:** Smart Type Optimization (sto_types import)
4. ✅ **STATELESS:** Struct-based design, global yok
5. ✅ **STRUCT+FUNC:** OOP yok, sadece struct ve fonksiyon
6. ✅ **MODÜL=ŞABLON:** Her modül state-free, import ile kullanılır

---

## 🚦 ÜST AKIL VE YZ KOORDİNASYONU

### YZ Rolleri (Güncellenmiş):

| YZ | Önceki Görev (❌ İptal) | Yeni Görev (✅ Aktif) |
|----|-------------------------|----------------------|
| **YZ_01** | Modül envanteri | ✅ Tamamlandı (MODÜL_ENVANTER.md) |
| **YZ_02** | Backend detection | ❌ DURDURULDU (rapor yazılacak) |
| **YZ_03** | Kritik modül test | ➡️ **P2+p7 Analiz** (Task 1-2) |
| **YZ_04** | Modül düzeltme | ➡️ **Lexer+Parser** (Task 3-4) |
| **YZ_05** | Entegrasyon test | ➡️ **Parser Advanced** (Task 5) |
| **YZ_06** | - | ➡️ **Codegen** (Task 6-7) |
| **YZ_07** | - | ➡️ **Language Features** (Task 8-9) |
| **YZ_08** | - | ➡️ **Integration+Test** (Task 10-12) |

### YZ_02 Durdurma Talimatı:

```
YZ_02: Backend detection işini durdur.

Neden: Legacy modules/ dizini kullanılmıyor, ana derleyici (main.c) 
zaten GCC backend kullanıyor. Backend detection gereksiz.

Görev: 
1. Yaptığın çalışmayı rapor et (YZ_02_KAPANIŞ_RAPORU.md)
2. Context'i kaydet (ne öğrendin, ne buldun)
3. Görevi sonlandır

Teşekkürler! Değerli analiz yaptın, ama strateji değişti.
```

---

## 📊 İLERLEME TAKİBİ

### Tamamlanan İşler:
- [x] Task 1: Modül Envanteri (YZ_01) - 31 Aralık 2025
- [x] Strateji revizesi #1 (PD_01) - 31 Aralık 2025
- [x] ÜA durum raporu - 1 Ocak 2026
- [x] Strateji revizesi #2 (PD_02) - 1 Ocak 2026
- [x] TODO #3 revize edildi - 1 Ocak 2026

### Devam Eden İşler:
- [ ] YZ_02 durdurma süreci
- [ ] YZ_03 P2/p7 analiz başlatma

### Bekleyen İşler:
- [ ] Task 1-2: P2+p7 Analiz (2 gün)
- [ ] Task 3-5: Parser (3 gün)
- [ ] Task 6-7: Codegen (3 gün)
- [ ] Task 8-10: Language Features (2 gün)
- [ ] Task 11-12: Integration & Test (2 gün)

---

**Güncelleme:** 1 Ocak 2026 (Strateji Revize #2)  
**Güncelleyen:** MM_01 (Mastermind)  
**PD Onayı:** PD_02 ✅ (ÜA raporu sonrası)  
**Sonraki Adım:** YZ_02 durdurma, YZ_03 başlatma
