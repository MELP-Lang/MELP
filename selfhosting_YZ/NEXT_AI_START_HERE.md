# SELF-HOSTING YZ - BURADAN BAŞLA

**Son Güncelleme:** 22 Aralık 2025  
**Üst Akıl:** Opus  
**Ana TODO:** `/TODO_SELFHOSTING_FINAL.md`  
**Kurallar:** `/TODO_kurallari.md`

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
| YZ_01 | Phase 1.1-1.2 | Core + Parser Syntax Fix | 🔵 AKTİF | `selfhosting_YZ_01` |
| YZ_02 | Phase 1.3-1.5 | CodeGen + Diğer Syntax Fix | ⏳ BEKLEMEDE | `selfhosting_YZ_02` |
| YZ_03 | Phase 2 | Integration | ⏳ BEKLEMEDE | `selfhosting_YZ_03` |
| YZ_04 | Phase 3 | Bootstrap | ⏳ BEKLEMEDE | `selfhosting_YZ_04` |
| YZ_05 | Phase 4 | Convergence | ⏳ BEKLEMEDE | `selfhosting_YZ_05` |
| YZ_06 | Phase 5 | Finalization | ⏳ BEKLEMEDE | `selfhosting_YZ_06` |

---

## 🔵 ŞU AN AKTİF GÖREV

### YZ_01: Phase 1.1-1.2 - Core + Parser Syntax Fix

**Durum:** 🔵 AKTİF  
**Bağımlılık:** YZ_00 ✅ (tamamlandı)  
**Tahmini Süre:** 4-6 saat

**Görevler:**

1. **Task 1.1: Core Modüller Syntax Fix (2 saat)**
   - `compiler/stage1/modules/lexer_mlp/lexer.mlp` (345 satır)
   - `compiler/stage1/modules/codegen_mlp/codegen_api.mlp`
   - Virgül → Semicolon
   - `while X` → `while X do`
   - `break` → `exit`
   - Her dosya sonrası test et

2. **Task 1.2: Parser Modülleri Syntax Fix (2-3 saat)**
   - `compiler/stage1/modules/parser_mlp/*.mlp` (~16 modül)
   - Aynı düzeltmeler
   - Her modül ayrı test edilecek

**Düzeltme Pattern:**
```bash
# Dikkat: String içindeki virgülleri değiştirme!
# Ondalık sayılardaki virgüllere dokunma (3,14)
# Her düzeltmeden sonra test et

timeout 10 compiler/stage0/modules/functions/functions_compiler <file.mlp> temp/test.ll
```

**Hedef:** Core ve Parser modülleri %100 derlenebilir hale getir

**Tamamlandığında:**
- `selfhosting_YZ/YZ_01_TAMAMLANDI.md` oluştur
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
Phase 1: [ ] [ ] [ ] [ ] [ ]       0/5 
Phase 2: [ ] [ ] [ ] [ ] [ ]       0/5 
Phase 3: [ ] [ ] [ ]               0/3 
Phase 4: [ ] [ ] [ ]               0/3 
Phase 5: [ ] [ ] [ ] [ ]           0/4 

TOPLAM: 4/24 task (17%)
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
