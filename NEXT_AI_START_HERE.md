# 🚀 NEXT AI START HERE - SEN YZ_10'SUN!

**👤 SENİN ROLÜN:** YZ_10 - Structs Modülü  
**📅 Tarih:** 18 Aralık 2025  
**🌿 Dal:** `structs_module_YZ_10` (oluşturulacak)  
**📊 Durum:** ⏸️ YZ_10 BEKLİYOR - YZ_09 TAMAMLANDI!

---

## 👋 MERHABA YZ_10!

**Kullanıcıya ilk mesajın şöyle olmalı:**

```
Merhaba! Ben YZ_10'yum.

NEXT_AI_START_HERE.md ve TODO.md'yi okudum.
Görevim: Structs Modülü (Stage 1 Modüler Yapı)

TODO.md'deki görevlerimi görüyorum:
- modules/structs/ dizini oluştur
- Struct definition, member access parsing
- LLVM struct types, GEP
- Test suite

YZ_03'ün core utilities ve YZ_09'un arrays modülünü kullanacağım! 🎉

Başlamamı ister misiniz?
```

---

## 🎉 YZ_01 - YZ_09 TAMAMLANDI!

**YZ_01 - Import Tasarımı (✅ Tamamlandı - 2.5 saat)**
- ✅ Import sistem tasarımı
- ✅ Stage 0 analizi (import modülü ZATEN VAR!)
- ✅ Kural güçlendirme

**YZ_02 - Lexer + Parser (✅ Tamamlandı - 1 saat!)**
- ✅ String literal desteği eklendi (1 satır değişiklik)
- ✅ 3 test başarılı
- ✅ Import sistemi TAMAMEN HAZIR!

**YZ_03 - Core Utilities Modülü (✅ Tamamlandı - 2.5 saat)**
- ✅ compiler/stage1/modules/core/ dizini oluşturuldu
- ✅ token_types.mlp - 93 token constants (Stage 0 C → MELP)
- ✅ char_utils.mlp - 12 character functions (Stage 0 C → MELP)
- ✅ type_mapper.mlp - 14 type kinds + 7 functions (Stage 0 C → MELP)
- ✅ test_core.mlp - Comprehensive test suite
- ✅ README.md - Full documentation
- ✅ Stage 1 ilk modülü tamamlandı! (1/34)

**YZ_04 - Functions Modülü (✅ Tamamlandı - 2 saat!)**
- ✅ compiler/stage1/modules/functions/ dizini oluşturuldu
- ✅ functions_parser.mlp - Function declaration, call, return parsing (450 satır)
- ✅ functions_codegen.mlp - LLVM IR generation (380 satır)
- ✅ test_functions.mlp - 6 comprehensive tests (520 satır)
- ✅ README.md - Full documentation (400 satır)
- ✅ Stateless architecture uygulandı
- ✅ Core utilities entegrasyonu
- ✅ Stage 1 ikinci modülü tamamlandı! (2/34)

**YZ_05 - Variables Modülü (✅ Tamamlandı - 2 saat!)**
- ✅ compiler/stage1/modules/variables/ dizini oluşturuldu
- ✅ variables_parser.mlp - Variable declaration, assignment parsing (450 satır)
- ✅ variables_codegen.mlp - LLVM IR generation (alloca, store, load) (400 satır)
- ✅ test_variables.mlp - 8 comprehensive tests (480 satır)
- ✅ README.md - Full documentation (650 satır)
- ✅ Stateless architecture uygulandı
- ✅ 7 variable types support
- ✅ Stage 1 üçüncü modülü tamamlandı! (3/34)

**YZ_06 - Operators Modülü (✅ Tamamlandı - 2.5 saat!)**
- ✅ compiler/stage1/modules/operators/ dizini oluşturuldu
- ✅ operators_parser.mlp - Pratt parser, all operators (520 satır)
- ✅ operators_codegen.mlp - LLVM IR generation (380 satır)
- ✅ test_operators.mlp - 8 comprehensive tests (620 satır)
- ✅ README.md - Full documentation (750 satır)
- ✅ Stateless architecture uygulandı
- ✅ 18 operator types (arithmetic, comparison, logical, bitwise)
- ✅ Operator precedence (11 levels)
- ✅ Stage 1 dördüncü modülü tamamlandı! (4/34)

**YZ_07 - Control Flow Modülü (✅ Tamamlandı - 2.5 saat!)**
- ✅ compiler/stage1/modules/control_flow/ dizini oluşturuldu
- ✅ control_flow_parser.mlp - if/else, while, for parsing (540 satır)
- ✅ control_flow_codegen.mlp - LLVM IR generation (380 satır)
- ✅ test_control_flow.mlp - 12 comprehensive tests (580 satır)
- ✅ README.md - Full documentation (850 satır)
- ✅ Stateless architecture uygulandı
- ✅ If/else statements, while loops, for loops
- ✅ LLVM branch instructions, label generation
- ✅ Break/continue support
- ✅ Stage 1 beşinci modülü tamamlandı! (5/34)

**YZ_08 - Literals Modülü (✅ Tamamlandı - 1.5 saat!)**
- ✅ compiler/stage1/modules/literals/ dizini oluşturuldu
- ✅ literals_parser.mlp - Integer, string, boolean literal parsing (438 satır)
- ✅ literals_codegen.mlp - LLVM constant generation (450 satır)
- ✅ test_literals.mlp - 10 comprehensive tests (608 satır)
- ✅ README.md - Full documentation (620 satır)
- ✅ Stateless architecture uygulandı
- ✅ Integer literals (i64), string literals (i8*), boolean literals (i1)
- ✅ LLVM constant generation (global strings, immediate values)
- ✅ Type inference and LLVM type mapping
- ✅ Stage 1 altıncı modülü tamamlandı! (6/34)

**YZ_09 - Arrays Modülü (✅ Tamamlandı - 2.5 saat!)**
- ✅ compiler/stage1/modules/arrays/ dizini oluşturuldu
- ✅ arrays_parser.mlp - Array literal, indexing, length parsing (467 satır)
- ✅ arrays_codegen.mlp - LLVM array operations (alloca, GEP) (477 satır)
- ✅ test_arrays.mlp - 10 comprehensive tests (559 satır)
- ✅ README.md - Full documentation (620 satır)
- ✅ Stateless architecture uygulandı
- ✅ Array literals [1, 2, 3], indexing arr[i], length len(arr)
- ✅ LLVM array allocation, getelementptr, store, load
- ✅ Homogeneous arrays (integer, string, boolean)
- ✅ Stage 1 yedinci modülü tamamlandı! (7/34)
- ✅ test_literals.mlp - 10 comprehensive tests (608 satır)
- ✅ README.md - Full documentation (620 satır)
- ✅ Stateless architecture uygulandı
- ✅ Integer literals (i64), string literals (i8*), boolean literals (i1)
- ✅ LLVM constant generation (global strings, immediate values)
- ✅ Type inference and LLVM type mapping
- ✅ Stage 1 altıncı modülü tamamlandı! (6/34)

---

## 📍 YZ_10'UN GÖREVİ

### Görev: Structs Modülü (Stage 1)
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **10.1** `compiler/stage1/modules/structs/` dizini oluştur
- [ ] **10.2** Parser
  - [ ] `structs_parser.mlp` - Struct definition, member access parsing
  - [ ] Import core utilities
  - [ ] Struct definition: struct Person { name: string, age: numeric }
  - [ ] Member access: person.name
- [ ] **10.3** CodeGen
  - [ ] `structs_codegen.mlp` - LLVM struct types, GEP
  - [ ] Struct type definition
  - [ ] Member access (getelementptr)
- [ ] **10.4** Test
  - [ ] `test_structs.mlp` - Test suite
  - [ ] Struct definition test
  - [ ] Member access test
  - [ ] Nested structs test
- [ ] **10.5** README ve rapor
  - [ ] `README.md` - Dokümantasyon
  - [ ] `YZ_Stage_1/YZ_10_RAPOR.md`

**Kaynak:**
- `compiler/stage0/modules/struct/` - Struct implementation
- Arrays modülü (YZ_09) - Struct içinde array kullanımı
---

## 🎯 YZ_10 İÇİN SONRAKİ ADIMLAR

**1. İlk Mesajın (Kullanıcıya):**
```
Merhaba! Ben YZ_10'yum.

TODO.md'deki görevlerimi gördüm:
- compiler/stage1/modules/structs/ oluştur
- structs_parser.mlp (struct definition, member access)
- structs_codegen.mlp (LLVM struct types, GEP)
- Test suite

YZ_03'ün core utilities ve YZ_09'un arrays modülünü kullanacağım! 🎉

Başlamamı ister misiniz?
```

**2. Yapman Gerekenler:**
1. ✅ `MELP_Mimarisi.md` oku ve ONAYLA
2. ✅ `TODO.md` oku (YZ_10 bölümü)
3. ✅ `YZ_Stage_1/YZ_09_RAPOR.md` oku
4. ✅ Git dalı oluştur: `git checkout -b structs_module_YZ_10`
5. ▶️ İşe başla!

**3. YZ_09'dan Önemli Notlar:**
- ✅ Arrays modülü hazır (`compiler/stage1/modules/arrays/`)
- ✅ Array literal, indexing, length working
- ✅ LLVM array operations (alloca, getelementptr, store, load)
- ✅ Homogeneous arrays (integer, string, boolean)
- ✅ Stateless pattern örneği
- ✅ Test pattern örneği
- ✅ Import sistemi çalışıyor
- ✅ Stage 1 yedinci modül tamamlandı (7/34)
- ⚠️ Structs, array field'larını destekleyecek

**4. ⚠️ ÖNEMLİ: Test Stratejisi!**
- ⚠️ Test dosyası YAZ ama ÇALIŞTIRAMAZSIN (Stage 1 compiler henüz yok)
- ✅ Her modül için `test_*.mlp` oluştur
- ❌ Stage 0 ile compile edemezsin (Stage 1 kodu MELP dilinde)
- 📌 **Çözüm:** Test dosyaları şimdilik "specification" olarak kalacak
- 🎯 **Gelecek:** Stage 1 compiler tamamlanınca tüm testler çalıştırılacak

**5. Standart Test Pattern (Her Modül İçin):**
```
- [ ] **X.4** Test suite YAZ
  - [ ] test_module.mlp oluştur
  - [ ] Test cases yaz
  
- [ ] **X.5** TEST ÇALIŞTIR ⭐
  - [ ] Stage 0 ile compile et (Stage 1 hazır olunca)
  - [ ] Testleri çalıştır
  - [ ] Sonuçları doğrula
  - [ ] ⚠️ ŞU ANDA YAPILMIYOR (compiler yok)
  
- [ ] **X.6** README
```

---

## 📋 YZ_01 CHECKLİST

### ✅ Başlangıç ZORUNLU!

**Her YZ session başında MUTLAKA ONAYLA:**

```
ONAYLA - YZ_01:

[x] NEXT_AI_START_HERE.md okudum
[x] MELP_Mimarisi.md okudum ve ONAYLADIM ✅
[x] TODO.md okudum
[x] 5 Temel Prensibi anladım:
    [x] %100 Modüler (her feature = ayrı modül)
    [x] LLVM Backend (değişmez)
    [x] Stateless:
        [x] Mutable global state YASAK
        [x] Const (compile-time, immutable) OK - Rust modeli
        [x] Parametre geçişi kullan
    [x] STO (runtime optimization)
    [x] Struct + Functions (OOP yok)
[x] Yasakları anladım:
    [x] Monolitik dosya YASAK
    [x] Mutable global YASAK (const immutable OK - Rust modeli)
    [x] Önceki kuralları ezme YASAK
    [x] Import olmadan modül YASAK (FAZ 2+)
    [x] Stage 0'ı bozma YASAK
    [x] **ANA DİZİNE TEST DOSYASI YASAK** (test_*.mlp/s/o → tests/ veya modules/*/)
[x] Git status kontrol ettim ✅
[x] Git dalı oluşturdum: import_design_YZ_01 ✅

✅ ONAYLIYORUM - YZ_01 (17 Aralık 2025, 22:22)
```

### Geliştirme
- [ ] Import syntax tasarla
- [ ] Import mekanizması tasarla
- [ ] Döküman yaz: `docs/IMPORT_SYSTEM_DESIGN.md`
- [ ] Stage 0 modifikasyon planı

### Tamamlama
- [x] Döküman hazır
- [x] Git commit: `git commit -m "YZ_03: Core Utilities Module"`
- [x] Git push: `git push origin core_utils_YZ_03`
- [x] **TODO.md güncelle:** YZ_03'ü tamamlandı işaretle [x] ✅
- [x] **NEXT_AI_START_HERE.md güncelle:** YZ_04 için hazırla
  - [x] Başlık değiştir: `# NEXT AI START HERE - YZ_03` → `YZ_04`
  - [x] YZ_03 completion notları ekle
  - [x] YZ_04 görevini öne çıkar
  - [x] Kritik bilgileri aktar
- [x] Rapor yaz: `YZ_Stage_1/YZ_03_RAPOR.md`
- [x] Bu dosyayı güncelle (NEXT_AI_START_HERE.md)
- [x] TODO.md'de işaretle: YZ_03 ✅

**⚠️ ZORUNLU KURAL:** Her YZ görev bitiminde TODO.md'deki tüm alt görevlerini [x] olarak işaretle!

---

## 📂 PROJE YAPISI

### Mevcut (Monolitik):
```
modules/
├── lexer_mlp/        ← 74 dosya
├── parser_mlp/       ← 12,371 satır
└── codegen_mlp/      ← (katmanlı yapı)
```

### Hedef (Modüler - Stage 1):
```
compiler/stage1/modules/
├── core/             ← Shared utilities (YZ_03) ✅ TAMAMLANDI
├── functions/        ← Function feature (YZ_04) ✅ TAMAMLANDI
├── variables/        ← Variable feature (YZ_05) ✅ TAMAMLANDI
├── operators/        ← Operators (YZ_06) ✅ TAMAMLANDI
├── control_flow/     ← Control flow (YZ_07) ✅ TAMAMLANDI
├── literals/         ← Literals (YZ_08) ⏸️ ← ŞİMDİ BURASI
├── arrays/           ← Arrays (YZ_09)
├── structs/          ← Structs (YZ_10)
├── enums/            ← Enums (YZ_11)
└── ...               ← 25+ modül daha (Stage 0 parity)
```

**İlerleme:** 7/34 modül ✅

---

## 🎓 REFERANSLAR

**Önce Oku:**
1. `MELP_Mimarisi.md` - Temel mimari kuralları
2. `TODO.md` - 15 YZ görev planı
3. `compiler/stage0/modules/` - Stage 0 modüler yapı örneği

**Dökümanlar:**
- `PMPL_SYNTAX.md` - PMPL sözdizimi
- `compiler/stage0/ARCHITECTURE.md` - Stage 0 mimarisi
- `docs/LLVM_IR_GUIDE.md` - LLVM IR

**Eski TODO'lar (referans):**
- `TODO_OLD_YZ26.md` - Önceki Stage 1 TODO
- `NEXT_AI_START_HERE_OLD_YZ29.md` - Önceki NEXT_AI

---

## ⚠️ HATIRLATMALAR - MELP MİMARİSİ KURALLARI

**⚠️ ÖNEMLİ:** Bu kuralları ihlal eden önceki YZ'ler yüzünden Stage 1 baştan yapılıyor!

### 🎯 5 TEMEL PRENSİP (ASLA İHLAL ETME!)

1. **%100 Modüler** - Her feature ayrı modül (kendi parser + codegen)
2. **LLVM Backend** - Platform-independent, değiştirilmez  
3. **Stateless** - Global state YASAK, parametre geçişi
4. **STO** - Runtime'da otomatik optimizasyon
5. **Struct + Functions** - OOP YOK (Class/Inheritance yasak)

### ❌ YASAKLAR (BUNLARI YAPMA!)

1. **Monolitik Dosya Oluşturma**
   - ❌ `parser.mlp` (tüm parsing burada)
   - ✅ `modules/functions/functions_parser.mlp` (feature bazlı)

2. **Global State Kullanma**
   ```mlp
   ❌ list g_tokens  -- Mutable global YASAK!
   ✅ const numeric TOKEN_ID = 1  -- Immutable const OK
   ```

3. **Önceki Kuralları Ezme**
   - Önceki YZ'nin kodunu değiştirmeden önce SOR!
   - Mimari kararları değiştirme

4. **Import Olmadan Modül**
   - FAZ 2'den itibaren import ZORUNLU

5. **Stage 0'ı Bozma**
   - Her değişiklik sonrası test et

6. **Merge/PR Yapma**
   - Kendi dalında kal

7. **Ana Dizine Test/Geçici Dosya Oluşturma**
   - ❌ `/test_*.mlp` (ana dizinde)
   - ✅ `tests/stage_1_tests/test_*.mlp`
   - Ana dizin temiz kalmalı!

### ✅ YAPMALISIN (ZORUNLU!)

1. **MELP_Mimarisi.md Oku ve Onayla**
   ```
   [ ] MELP_Mimarisi.md okudum
   [ ] 5 temel prensibi anladım
   [ ] Modüler yapı gereksinimini anladım
   [ ] Stateless pattern'i anladım
   [ ] Const = Rust modeli (immutable) anladım
   
   ONAYLIYORUM - YZ_XX
   ```

2. **Modüler Yapı Koru**
   - Her feature = ayrı modül (parser + codegen)
   - Bağımsız, izole, test edilebilir

3. **Stateless Tasarım**
   - Parametre geçişi kullan
   - State fonksiyondan döndür
   - Mutable global YASAK

4. **Şüphen Varsa DURDUR**
   - Mimari ihlal görürsen → DURDUR, kullanıcıya sor
   - Test başarısız → DURDUR, kullanıcıya sor
   - Otomatik düzeltme yapma → Kullanıcı onayı al

5. **Her YZ Sonunda** ⚠️ **ZORUNLU ADIMLAR!**
   
   **Tamamlama Protokolü:**
   ```bash
   # 1. TODO.md'yi güncelle
   - Kendi görevini ✅ işaretle
   - Tamamlanma zamanı ve notları ekle
   
   # 2. NEXT_AI_START_HERE.md'yi güncelle
   - Başlık değiştir: YZ_02 → YZ_03
   - "YZ_02 TAMAMLANDI ✅" bölümü ekle
   - YZ_03 görevini öne çıkar
   - Kritik keşifleri/notları sonraki YZ'ye aktar
   
   # 3. Rapor yaz (opsiyonel ama tavsiye)
   - YZ_Stage_1/YZ_XX_RAPOR.md
   
   # 4. Git commit + push
   git add -A
   git commit -m "YZ_XX: [Görev özeti] - Complete"
   git push origin [dal_adi]
   
   # 5. Test dosyalarını temizle
   - Ana dizinde test dosyası bırakma!
   - tests/stage_1_tests/ kullan
   ```
   
   **NEXT_AI güncellemezsen → Sonraki YZ kaybolur!**

### 📁 DİZİN KURALLARI (ÇOK ÖNEMLİ!)

**Ana dizin kirletme yasağı:**

```bash
# ❌ YANLIŞ - Ana dizine test dosyası
/home/pardus/projeler/MLP/MLP/test_my_feature.mlp
/home/pardus/projeler/MLP/MLP/output.ll
/home/pardus/projeler/MLP/MLP/debug.txt

# ✅ DOĞRU - Doğru dizinler
tests/stage_1_tests/test_my_feature.mlp
temp/output.ll
temp/debug.txt
```

**Kural:** Ana dizin sadece:
- README, TODO, NEXT_AI gibi ana dökümanlar
- Klasörler (modules/, tests/, temp/, vb.)
- ⚠️ Test/geçici dosya YASAK!

### 📖 REFERANSLAR

**MUTLAKA OKU:**
- ✅ `MELP_Mimarisi.md` - Temel kurallar (Session başında oku!)
- ✅ `TODO.md` - Görev detayları
- ✅ `compiler/stage0/modules/` - Modüler yapı örneği

**İhlal Örnekleri (YAPMA!):**
```mlp
-- ❌ ÖRNEK 1: Global state (YASAK!)
list g_tokens = []
numeric g_pos = 0

-- ✅ DOĞRU: Parametre geçişi
function parse(list tokens, numeric pos) returns list
    return [result, new_pos]
end_function

-- ❌ ÖRNEK 2: Monolitik dosya (YASAK!)
-- parser.mlp içinde tüm features

-- ✅ DOĞRU: Feature bazlı modül
-- modules/functions/functions_parser.mlp
-- modules/variables/variables_parser.mlp
```

### 🚨 DURDURMA SENARYOLARI

Aşağıdaki durumlarda İŞİ DURDUR ve kullanıcıya SOR:

1. Mevcut kod global state kullanıyor
2. Monolitik yapı görüyorsun
3. Import sistemi yok ama modül yapmak istiyorsun
4. Test başarısız oluyor
5. Stage 0 bozuldu
6. Mimari ihlal görüyorsun
7. Önceki YZ'nin kararını değiştirmen gerekiyor

**Otomatik düzeltme yapma, önce SOR!**

---

## 🚀 BAŞLANGIÇ KOMUTU

```bash
# Mevcut durumu kontrol et
git status

# Yeni dal oluştur
git checkout -b import_design_YZ_01

# YZ_Stage_1 dizini oluştur
mkdir -p YZ_Stage_1

# İşe başla!
# 1. MELP_Mimarisi.md oku
# 2. C dilinde #include nasıl çalışıyor araştır
# 3. Import syntax tasarla
# 4. Döküman yaz: docs/IMPORT_SYSTEM_DESIGN.md
```

---

## 📊 İLERLEME

**Faz 1: Import Sistemi (13h)**
- [ ] YZ_01 - Import Tasarımı (3h) ⏳ DEVAM EDİYOR
- [ ] YZ_02 - Lexer + Parser (4h)
- [ ] YZ_03 - Resolution + Symbol Table (5h)
- [ ] YZ_04 - CodeGen + Integration (3h)

**Faz 2: Modüler Stage 1 (26h)**
- 10 modül (YZ_05 - YZ_14)

**Faz 3: Validation (3h)**
- [ ] YZ_15 - Self-Compile

**TOPLAM:** 38 saat (tahmini)

---

## 🤝 YZ AKTARIMI

**Sonraki YZ'ye mesajım:**

Merhaba YZ_10! 👋

Ben YZ_09, arrays modülünü tamamladım.

**Sana bıraktıklarım:**
- ✅ Arrays modülü: `compiler/stage1/modules/arrays/`
- ✅ Parser + CodeGen complete (944 satır)
- ✅ Test suite (10 tests, 559 satır)
- ✅ Array literals: [1, 2, 3]
- ✅ Array indexing: arr[i]
- ✅ Array length: len(arr)
- ✅ LLVM operations (alloca, getelementptr, store, load)
- ✅ Homogeneous arrays (integer, string, boolean)
- ✅ Stateless pattern örneği
- ✅ Import kullanımı örneği
- ✅ README dokümantasyonu

**Senin görevin (YZ_10):**
1. YZ_09 raporumu oku (`YZ_Stage_1/YZ_09_RAPOR.md`)
2. Structs modülü oluştur
3. Struct definition parsing
4. Member access (person.name)
5. LLVM struct types
6. Test suite yaz

**Kritik notlar:**
- Arrays modülünü pattern olarak kullan
- Structs, array field'ları destekleyecek (struct içinde list)
- LLVM struct types: `%Person = type { i8*, i64 }`
- Member access: getelementptr for struct fields
- Stage 0'da struct implementasyonu var

**Pattern Örneği:**
```mlp
-- structs_parser.mlp'den örnek
function parse_struct_definition(tokens: list, pos: numeric) returns list
    -- Takes: tokens + position
    -- Returns: [struct_node, new_position]
    return [struct_def, current_pos]
end_function
```

**Structs Integration:**
```mlp
-- Struct definition:
struct Person
    string name
    numeric age
    list hobbies    -- Array field!
end_struct

-- Struct instance:
Person john = {
    name = "John",
    age = 30,
    hobbies = ["coding", "music"]
}

-- Member access:
string name = john.name         -- "John"
numeric age = john.age          -- 30
string hobby = john.hobbies[0]  -- "coding"
```

Başarılar! 🚀

---

**SON GÜNCELLEME:** YZ_09 - Arrays modülü tamamlandı (18 Aralık 2025, 20:00)  
**SONRAKİ:** YZ_10 - Structs modülü

**MELP kusursuz mimariye kavuşuyor!** 🎯
