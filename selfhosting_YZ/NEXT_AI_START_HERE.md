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
| YZ_01 | Phase 1.1-1.2 | Core + Parser Syntax Fix | ✅ TAMAMLANDI | `selfhosting_YZ_01` |
| YZ_02 | Phase 1.3-1.5 | Kalan Modüller + While Syntax + Doğrulama | 🔵 AKTİF | `selfhosting_YZ_02` |
| YZ_03 | Phase 2 | Integration | ⏳ BEKLEMEDE | `selfhosting_YZ_03` |
| YZ_04 | Phase 3 | Bootstrap | ⏳ BEKLEMEDE | `selfhosting_YZ_04` |
| YZ_05 | Phase 4 | Convergence | ⏳ BEKLEMEDE | `selfhosting_YZ_05` |
| YZ_06 | Phase 5 | Finalization | ⏳ BEKLEMEDE | `selfhosting_YZ_06` |

---

## 🔵 ŞU AN AKTİF GÖREV

### YZ_02: Phase 1.3-1.5 - Kalan Modüller + While Syntax + Doğrulama

**Durum:** 🔵 AKTİF  
**Bağımlılık:** YZ_01 ✅ (tamamlandı)  
**Tahmini Süre:** 3-4 saat

**Görevler:**

1. **Task 1.3: Kalan Core Modüller (1 saat)**
   - `compiler.mlp`, `compiler_integration.mlp`, `compiler_full.mlp`
   - `arrays/`, `control_flow/`, `enums/` klasörleri
   - ~40 modül
   - Python script kullan: `temp/fix_syntax_complete.py`

2. **Task 1.4: While Syntax Fix (1 saat)**
   - `while X` → `while X do` değişiklikleri
   - YZ_00 raporunda 32 adet tespit edilmişti
   - grep ile bul: `grep -rn "while .* " --include="*.mlp"`

3. **Task 1.5: Test ve Doğrulama (1-2 saat)**
   - Her modülü Stage 0 ile derlemeyi dene
   - Tam derlenenleri listele
   - Kısmi derlenenlerin sorunlarını belirle
   - `temp/compilation_results.txt` raporu oluştur

**Kullanılacak Araçlar:**
```bash
# Toplu syntax fix
python3 temp/fix_syntax_complete.py <file.mlp>

# While syntax fix
sed -i 's/while \([^d][^ ]*\) /while \1 do /g' <file.mlp>

# Test
timeout 15 compiler/stage0/modules/functions/functions_compiler <file.mlp> temp/test.ll
```

**Hedef:** Tüm Stage 1 modülleri syntax açısından %100 temiz

**Tamamlandığında:**
- `selfhosting_YZ/YZ_02_TAMAMLANDI.md` oluştur
- `NEXT_AI_START_HERE.md`'yi güncelle

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
