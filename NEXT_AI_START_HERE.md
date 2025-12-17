# 🚀 NEXT AI START HERE - SEN YZ_04'SÜN!

**👤 SENİN ROLÜN:** YZ_04 - Functions Modülü  
**📅 Tarih:** 17 Aralık 2025  
**🌿 Dal:** `functions_module_YZ_04` (oluşturulacak)  
**📊 Durum:** ⏸️ YZ_04 BEKLİYOR - YZ_03 TAMAMLANDI!

---

## 👋 MERHABA YZ_04!

**Kullanıcıya ilk mesajın şöyle olmalı:**

```
Merhaba! Ben YZ_04'üm.

NEXT_AI_START_HERE.md ve TODO.md'yi okudum.
Görevim: Functions Modülü (Stage 1 Modüler Yapı)

TODO.md'deki görevlerimi görüyorum:
- modules/functions/ dizini oluştur
- functions_parser.mlp (mevcut koddan)
- functions_codegen.mlp (mevcut koddan)
- Test suite

Başlamamı ister misiniz?
```

---

## 🎉 YZ_01, YZ_02 & YZ_03 TAMAMLANDI!

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

**🎊 SÜPER HABER:** Stage 0 import sistemi hazır, Stage 1 modüler yapı başladı!

---

## 📍 YZ_04'ÜN GÖREVİ

### Görev: Functions Modülü (Stage 1)
**Tahmini:** 3 saat  
**Durum:** ⏸️ BEKLİYOR

#### Yapılacaklar:
- [ ] **4.1** `compiler/stage1/modules/functions/` dizini oluştur
- [ ] **4.2** Parser
  - [ ] `functions_parser.mlp` - Stage 0'dan MELP'e çevir
  - [ ] Import core utilities
  - [ ] Function parsing fonksiyonlarını al
- [ ] **4.3** CodeGen
  - [ ] `functions_codegen.mlp` - Stage 0'dan MELP'e çevir
  - [ ] Import type_mapper
  - [ ] Function declaration, call, return
- [ ] **4.4** Test
  - [ ] `test_functions.mlp` - Test suite
  - [ ] Function declaration test
  - [ ] Function call test
  - [ ] Return value test
- [ ] **4.5** README ve rapor
  - [ ] `README.md` - Dokümantasyon
  - [ ] `YZ_Stage_1/YZ_04_RAPOR.md`

**Kaynak:**
- `compiler/stage0/modules/functions/` - C implementation
- Mevcut `parser_mlp/parser_func.mlp` - Yeniden yapılandırılacak
- Mevcut `codegen_mlp/codegen_functions.mlp` - Yeniden yapılandırılacak

---

## 🎯 YZ_04 İÇİN SONRAKİ ADIMLAR

**1. İlk Mesajın (Kullanıcıya):**
```
Merhaba! Ben YZ_04'üm.

TODO.md'deki görevlerimi gördüm:
- compiler/stage1/modules/functions/ oluştur
- functions_parser.mlp (Stage 0 → MELP)
- functions_codegen.mlp (Stage 0 → MELP)
- Test suite

YZ_03'ün core utilities modülünü kullanacağım! 🎉

Başlamamı ister misiniz?
```

**2. Yapman Gerekenler:**
1. ✅ `MELP_Mimarisi.md` oku ve ONAYLA
2. ✅ `TODO.md` oku (YZ_04 bölümü)
3. ✅ `YZ_Stage_1/YZ_03_RAPOR.md` oku
4. ✅ Git dalı oluştur: `git checkout -b functions_module_YZ_04`
5. ▶️ İşe başla!

**3. YZ_03'ten Önemli Notlar:**
- ✅ Core utilities hazır (`compiler/stage1/modules/core/`)
- ✅ `token_types.mlp` - 93 token constants
- ✅ `char_utils.mlp` - 12 character functions
- ✅ `type_mapper.mlp` - Type system utilities
- ✅ Import sistemi çalışıyor
- ✅ Stage 1 ilk modül tamamlandı (1/34)
- ✅ Multiple imports çalışıyor
- ⚠️ YZ_03: Resolution ve Symbol Table kontrolü gerekli

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
    [x] Stateless (global state YASAK, const OK)
    [x] STO (runtime optimization)
    [x] Struct + Functions (OOP yok)
[x] Yasakları anladım:
    [x] Monolitik dosya YASAK
    [x] Mutable global YASAK (const immutable OK)
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
├── functions/        ← Function feature (YZ_04) ⏸️ ← ŞİMDİ BURASI
├── variables/        ← Variable feature (YZ_05)
├── operators/        ← Operators (YZ_06)
├── control_flow/     ← Control flow (YZ_07)
├── literals/         ← Literals (YZ_08)
├── arrays/           ← Arrays (YZ_09)
├── structs/          ← Structs (YZ_10)
├── enums/            ← Enums (YZ_11)
└── ...               ← 25+ modül daha (Stage 0 parity)
```

**İlerleme:** 1/34 modül ✅

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

Merhaba YZ_02! 👋

Ben YZ_01, import sistem tasarımını tamamladım.

**Sana bıraktıklarım:**
- ✅ Import tasarım dokümanı: `docs/IMPORT_SYSTEM_DESIGN.md`
- ✅ Stage 0 modifikasyon planı: `YZ_Stage_1/YZ_01_RAPOR.md`
- ✅ Import syntax: `import "path/to/module.mlp"`

**Senin görevin (YZ_02):**
1. YZ_01 raporumu oku
2. Stage 0 Lexer'a TOKEN_IMPORT ekle
3. Parser'a import parsing ekle
4. Test yaz

**Kritik notlar:**
- Stage 0'ı bozmadan çalış
- Her değişiklik sonrası test et
- Modüler yapıyı koru

Başarılar! 🚀

---

**SON GÜNCELLEME:** YZ_01 - Import tasarımı devam ediyor  
**SONRAKİ:** YZ_02 - Import Lexer/Parser

**MELP kusursuz mimariye kavuşuyor!** 🎯
