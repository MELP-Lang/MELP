# SELF-HOSTING YZ - BURADAN BAŞLA

**Son Güncelleme:** 22 Aralık 2025 (Üst Akıl - Yeni Keşif)  
**Üst Akıl:** Opus  
**Ana TODO:** `/TODO_SELFHOSTING_FINAL.md`  
**Kurallar:** `/TODO_kurallari.md`

---

## 🚨 KRİTİK KEŞİF (22 Aralık 2025)

**1,034 `if` statement'da `then` anahtar kelimesi eksik!**

Stage 1 modüllerinde iki farklı `if` syntax'ı kullanılmış:
- DOĞRU: `if <condition> then` (854 adet)
- YANLIŞ: `if <condition>` (1,034 adet - **then yok!**)

**En çok etkilenen dosyalar:**
| # | Dosya | Eksik `then` |
|---|-------|--------------|
| 1 | `lexer_mlp/tokenize_identifiers.mlp` | 84 |
| 2 | `parser_mlp/parser.mlp` | 78 |
| 3 | `lexer_mlp/lexer.mlp` | 76 |
| 4 | `operators/operators_parser.mlp` | 70 |
| 5 | `variables/variables_parser.mlp` | 39 |

**Diğer keşifler:**
- `token_types.mlp` LLVM IR ile üzerine yazılmıştı → GERİ YÜKLENDİ ✅
- Stage 0 hem `end if` hem `end_if` kabul ediyor (normalizer var)

---

## 🎯 PROJE HEDEFİ

Stage 1 compiler'ın kendini derleyebilmesi (self-hosting %100).

```
Stage 0 (C) ──compile──> Stage 1 (MELP) ──compile──> Stage 1' (MELP)
                                │                         │
                                └─────── AYNI ────────────┘
```

---

## 📋 GÖREV DAĞILIMI

| YZ | Phase | Görev | Durum | Branch |
|----|-------|-------|-------|--------|
| YZ_00 | Phase 0 | Sistem Tutarlılığı | ✅ TAMAMLANDI | `selfhosting_YZ_00` |
| YZ_01 | Phase 1.1-1.2 | Core + Parser Syntax Fix | ✅ TAMAMLANDI | `selfhosting_YZ_01` |
| YZ_02 | Phase 1.3-1.5 | Kalan Modüller + While Syntax + Doğrulama | ✅ TAMAMLANDI | `selfhosting_YZ_02` |
| YZ_03 | Phase 2 | Integration + **THEN FIX** | 🔵 AKTİF | `selfhosting_YZ_03` |
| YZ_04 | Phase 3 | Bootstrap | ⏳ BEKLEMEDE | `selfhosting_YZ_04` |
| YZ_05 | Phase 4 | Convergence | ⏳ BEKLEMEDE | `selfhosting_YZ_05` |
| YZ_06 | Phase 5 | Finalization | ⏳ BEKLEMEDE | `selfhosting_YZ_06` |

---

## 🔵 ŞU AN AKTİF GÖREV

### YZ_03: Phase 2 - Integration + THEN FIX

**Durum:** 🔵 AKTİF  
**Bağımlılık:** YZ_02 ✅ (tamamlandı)  
**Tahmini Süre:** 4-6 saat

**⚠️ YENİ ÖNCELİK:**

**1. `then` Ekleme (2-3 saat)**
   - 1,034 `if` statement'a `then` ekle
   - Her `if <condition>` satırını `if <condition> then` yap
   - Zaten `then` içerenleri bozma!

**Script Önerisi:**
```bash
# Her "if ..." satırını (then ile bitmiyorsa) "if ... then" yap
for f in $(find compiler/stage1/modules -name "*.mlp"); do
    sed -i '/^[[:space:]]*if .*[^n]$/s/$/ then/' "$f"
done
```

**2. Import Sorunlarını Çöz (1-2 saat)**
   - `control_flow_codegen.mlp` ve `enums_codegen.mlp` import hatalarını araştır
   - token_types.mlp geri yüklendi, test et

**3. Integration Test (1-2 saat)**
   - Tüm modülleri birlikte test et
   - Bootstrap sürecini dene

**Tamamlandığında:**
- `selfhosting_YZ/YZ_03_TAMAMLANDI.md` oluştur
- `NEXT_AI_START_HERE.md`'yi güncelle

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_02)

**YZ_02 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ Task 1.3: 12 modül syntax fix (compiler ana modüller, arrays, control_flow, enums)
- ✅ Task 1.4: 52 while syntax fix (while X → while X do), 20 dosya
- ✅ Task 1.5: Test ve doğrulama (7/9 başarılı)
- ✅ **Toplam 77+ modül düzeltildi** (YZ_01: 65+ | YZ_02: 12)

**Syntax Düzeltmeleri:**
- Virgül → Semicolon: ~400+ değişiklik
- While do ekleme: 52 değişiklik
- Blok sonları: ~100+ değişiklik
- Boolean → numeric: (devam)
- exit/break düzeltmeleri

**Test Sonuçları:**
- ✅ compiler.mlp, compiler_integration.mlp, compiler_full.mlp: Derlenmiş (78KB toplam)
- ✅ arrays_codegen.mlp, arrays_parser.mlp: Derlenmiş (19KB)
- ✅ control_flow_parser.mlp, enums_parser.mlp: Derlenmiş (26KB)
- ⚠️ control_flow_codegen.mlp, enums_codegen.mlp: Import errors

**Araçlar:**
- `temp/fix_syntax_advanced.py` - Kapsamlı syntax fixer
- `temp/fix_while.py` - While do fixer
- `temp/test_stage1_modules.sh` - Test scripti
- `temp/compilation_results_yz02.txt` - Detaylı rapor

**Bilinen Sorunlar:**
- 2 modül import edilen dosyalardaki syntax sorunları nedeniyle derlenemiyor
- ~30 modül henüz kontrol edilmedi (test dosyaları, yardımcı modüller)

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_01)

**YZ_01 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ lexer_mlp: lexer.mlp düzeltildi (6 fonksiyon derlenmiş)
- ✅ parser_mlp: 28 modül syntax fix (toplu düzeltme)
- ✅ codegen_mlp: 17 modül syntax fix (toplu düzeltme)
- ✅ **Toplam 65+ modül düzeltildi**

**Syntax Düzeltmeleri:**
- Virgül → Semicolon: ~300+ değişiklik
- Blok sonları: ~200+ değişiklik (end_if, end_while, vb.)
- Boolean → numeric: ~50 değişiklik (STO prensibi)
- exit while → exit: ~20 değişiklik

**Önemli Bulgular:**
- ✅ Stage 0 semicolon'u TAM destekliyor
- ✅ Array literal'lerde semicolon zorunlu: `[a; b; c]`
- ✅ Fonksiyon parametreleri/çağrıları: semicolon
- ⚠️ Bazı modüller kısmen derlenmiş (hata var ama output üretiyor)

**Araçlar:**
- `temp/fix_syntax_complete.py` oluşturuldu
- Python script ile toplu düzeltme çok hızlı

**Bilinen Sorunlar:**
- Bazı modüller "println not found" hatası veriyor (runtime dependency)
- While syntax (32 adet `while X` do eksik) henüz düzeltilmedi

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_00)

**YZ_00 Tamamlandı:** ✅ (22 Aralık 2025)

**Bulguları:**
- ✅ Stage 0 build ve test başarılı
- ✅ Import sistemi çalışıyor
- ⚠️ 89/107 Stage 1 modülü syntax fix gerekiyor (%83)
- 1,104 virgül → semicolon değişikliği
- 32 `while` → `while do` değişikliği
- 10 `break` → `exit` değişikliği

**En Çok Sorun Olan Modüller:**
1. codegen_mlp/codegen_arrays.mlp - 52 virgül
2. codegen_mlp/codegen_functions.mlp - 45 virgül
3. codegen_mlp/codegen_structs.mlp - 38 virgül
4. parser_mlp/parser_expressions.mlp - 34 virgül

**Önemli:** `temp/syntax_inventory.txt` detaylı rapor (982 satır)

**Bilinen durumlar:**
- Stage 1 modüllerinde eski syntax var (virgül kullanımı)
- 98/107 modül derleniyor (%92)
- `lexer.mlp`'de substring() çağrıları düzeltilmeli

---

## ⚠️ KRİTİK KURALLAR

### Git Workflow

```bash
# 1. Branch oluştur
git checkout -b selfhosting_YZ_XX

# 2. Çalış ve commit et
git add .
git commit -m "YZ_XX: [açıklama]"

# 3. Push et
git push origin selfhosting_YZ_XX

# ⚠️ MERGE YAPMA! Pull request AÇMA!
```

### Zorunlu Okumalar

Göreve başlamadan önce oku:
1. `TODO_kurallari.md` - Tüm kurallar
2. `TODO_SELFHOSTING_FINAL.md` - Detaylı görev listesi
3. `pmlp_kesin_sozdizimi.md` - Syntax referans
4. `MELP_VISION.md` - Vizyon
5. `MELP_REFERENCE.md` - Referans
6. `ARCHITECTURE.md` - Mimari

## 📊 GENEL İLERLEME

```
Phase 0: [✅] [✅] [✅] [✅]         4/4  (YZ_00 ✅)
Phase 1: [✅] [✅] [✅] [✅] [✅]    5/5  (YZ_01 ✅ | YZ_02 ✅)
Phase 2: [ ] [ ] [ ] [ ] [ ]       0/5 
Phase 3: [ ] [ ] [ ]               0/3 
Phase 4: [ ] [ ] [ ]               0/3 
Phase 5: [ ] [ ] [ ] [ ]           0/4 

TOPLAM: 9/24 task (38%)
```
**Prensip ihlali tespit edersen: DURDUR ve Üst Akıl'a danış!**

---

## 📊 GENEL İLERLEME

```
Phase 0: [ ] [ ] [ ] [ ]           0/4 
Phase 1: [ ] [ ] [ ] [ ] [ ]       0/5 
Phase 2: [ ] [ ] [ ] [ ] [ ]       0/5 
Phase 3: [ ] [ ] [ ]               0/3 
Phase 4: [ ] [ ] [ ]               0/3 
Phase 5: [ ] [ ] [ ] [ ]           0/4 

TOPLAM: 0/24 task (0%)
```

---

## 🚀 BAŞLA!

1. `TODO_kurallari.md` oku ✓
2. Bu dosyayı oku ✓
3. `TODO_SELFHOSTING_FINAL.md` oku
4. Zorunlu belgeleri oku
5. Kullanıcıya kendini tanıt ve onay al
6. Branch oluştur
7. Çalış
8. Rapor yaz
9. Push et
10. Bu dosyayı güncelle

**Başarılar!**
