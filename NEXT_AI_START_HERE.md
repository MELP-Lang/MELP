# 🚀 NEXT AI START HERE - YZ_01

**Son Güncelleme:** 17 Aralık 2025  
**Güncelleyen:** YZ_01 (İlk AI - Modüler yeniden yapılandırma)  
**Dal:** `import_design_YZ_01` (oluşturulacak)  
**Durum:** ⏳ DEVAM EDİYOR

---

## ⚠️ ÖNEMLİ: STAGE 1 RESET!

**Önceki YZ'ler Stage 1'i tamamladı ama...**
- ❌ Monolitik yapı kullanılmış (MELP_Mimarisi.md'ye aykırı)
- ❌ Import sistemi yok (modüler yapı imkansız)
- ✅ 12,371 satır kod var (YENİDEN KULLANILACAK!)

**Yeni Strateji:**
1. Stage 0'a import sistemi ekle
2. Mevcut kodu modüler yapıya dönüştür
3. Kusursuz mimari elde et

**Detaylar:** `TODO.md` dosyasına bak

---

# 🚀 NEXT AI START HERE - YZ_02

**Son Güncelleme:** 17 Aralık 2025, 22:30  
**Güncelleyen:** YZ_01 (Import tasarımı tamamlandı ✅)  
**Dal:** `import_lexer_parser_YZ_02` (oluşturulacak)  
**Durum:** ⏸️ YZ_02'YE HAZ

IR

---

## 🎉 YZ_01 TAMAMLANDI!

**Harika Haber:** Stage 0'da import sistemi zaten var!

YZ_01 tarafından tamamlandı:
- ✅ Import sistem tasarımı (`docs/IMPORT_SYSTEM_DESIGN.md`)
- ✅ Stage 0 analizi (import modülü mevcut)
- ✅ Kural güçlendirme (TODO, NEXT_AI, PROTOKOL)
- ✅ Proje temizliği (89 dosya düzenlendi)

---

## 📍 MEVCUT DURUM

### Görev: YZ_02 - Stage 0 Import: Lexer + Parser
**Tahmini:** 4 saat  
**Durum:** ⏸️ BEKLİYOR

#### Tamamlanması Gerekenler:
- [ ] **2.1** YZ_01 raporunu oku (`YZ_Stage_1/YZ_01_RAPOR.md`)
- [ ] **2.2** Stage 0 Lexer'a import token ekle
  - [ ] `TOKEN_IMPORT` tanımla
  - [ ] `import` keyword'ünü tanı
- [ ] **2.3** Stage 0 Parser'a import parsing ekle
  - [ ] Import statement parsing (zaten var, aktive et)
  - [ ] Test: `import "test"` parse olmalı
- [ ] **2.4** Test yaz
  - [ ] `tests/stage_1_tests/test_import_syntax.mlp`
  - [ ] Basit import parse testi

#### Kritik Bilgi:
🎉 **Import altyapısı Stage 0'da hazır!**
- ✅ `compiler/stage0/modules/import/` (8 dosya)
- ✅ Module resolution çalışıyor
- ✅ Import cache çalışıyor
- ⏳ Sadece TOKEN_IMPORT ekle, aktive et

---

## 🎯 SONRAKİ GÖREV: YZ_02

**Görev:** Stage 0 Import - Lexer + Parser  
**Tahmini:** 4 saat

### Yapacakları:
1. YZ_01 raporunu oku
2. Stage 0 Lexer'a `TOKEN_IMPORT` ekle
3. Import parsing (AST node)
4. Test yaz

**Detaylar:** `TODO.md` > YZ_02

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
    [x] Ana dizine test dosyası YASAK
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
- [ ] Döküman hazır
- [ ] Git commit: `git commit -m "YZ_01: Import system design"`
- [ ] Git push: `git push origin import_design_YZ_01`
- [ ] Rapor yaz: `YZ_Stage_1/YZ_01_RAPOR.md`
- [ ] Bu dosyayı güncelle (NEXT_AI_START_HERE.md)
- [ ] TODO.md'de işaretle: YZ_01 ✅

---

## 📂 PROJE YAPISI

### Mevcut (Monolitik):
```
modules/
├── lexer_mlp/        ← 74 dosya
├── parser_mlp/       ← 12,371 satır
└── codegen_mlp/      ← (katmanlı yapı)
```

### Hedef (Modüler):
```
modules/
├── core/             ← Shared utilities (YZ_05)
├── functions/        ← Function feature (YZ_06)
├── variables/        ← Variable feature (YZ_07)
├── operators/        ← Operators (YZ_08)
└── ...               ← 10 modül daha
```

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

5. **Her YZ Sonunda**
   - Rapor yaz: `YZ_Stage_1/YZ_XX_RAPOR.md`
   - Bu dosyayı güncelle (NEXT_AI_START_HERE.md)
   - TODO.md'de işaretle
   - Git commit + push
   - **Test dosyalarını temizle:** Ana dizinde test dosyası bırakma!

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
