# 📋 UA_03 ATAMA: TODO #3 (Stage1-Simple Modules)

**UA:** UA_03 (Üst Akıl #03)  
**Tarih:** 30 Aralık 2025, 04:30  
**Atayan:** MM_05 (Mastermind)  
**TODO:** TODO #3 - Stage1-Simple Modules + Stage2 Binary  
**Süre:** 2 saat hedef  
**Durum:** 🟢 AKTIF

---

## 🎯 GÖREVİN

**Primary Objective:**
YZ_11'i yönet, Stage1-simple modules oluşturulsun ve Stage2 binary build edilsin.

**Deliverables:**
1. ✅ 4 MLP modülü (lexer, parser, codegen, main)
2. ✅ Stage0 ile derlenmiş C dosyaları
3. ✅ melpc_stage2 binary
4. ✅ Self-compile test başarılı
5. 📊 UA_03 raporu

---

## 📊 YZ_11 TASK LIST (SEN TAKİP EDECEKS İN)

| # | Task | Durum | Not |
|---|------|-------|-----|
| 1 | lexer_simple.mlp | ✅ DONE | 133 satır |
| 2 | parser_simple.mlp | ⏳ ACTIVE | ~150 satır |
| 3 | codegen_simple.mlp | ⏸️ TODO | ~150 satır |
| 4 | main.mlp | ⏸️ TODO | ~50 satır |
| 5 | Stage0 compile | ⏸️ TODO | 4 C dosyası |
| 6 | gcc build | ⏸️ TODO | melpc_stage2 |
| 7 | Self-compile test | ⏸️ TODO | test.mlp → test.c |
| 8 | YZ_11 rapor | ⏸️ TODO | YZ_11_RAPOR.md |

**Progress:** 1/8 (12.5%)

---

## 🎯 BAŞARI KRİTERLERİ

### ✅ Functional Requirements:

1. **4 MLP Modülü:**
   - lexer_simple.mlp (~100 satır)
   - parser_simple.mlp (~150 satır)
   - codegen_simple.mlp (~150 satır)
   - main.mlp (~50 satır)

2. **Stage0 Compilation:**
   - Hatasız derleme (no syntax errors)
   - 4 C dosyası üretildi
   - Function bodies DOLU (not empty)

3. **Stage2 Binary:**
   - gcc link başarılı
   - melpc_stage2 çalıştırılabiliyor
   - `./melpc_stage2 --version` çalışıyor

4. **Self-Compile Test:**
   - Basit MLP dosyası derliyor
   - C output doğru
   - gcc ile executable oluşturuluyor

### ✅ Quality Requirements:

1. **Stage0-Friendly Syntax:**
   - ❌ No struct member access
   - ❌ No string concatenation
   - ❌ No method calls on structs
   - ✅ Basic expressions only

2. **Modular Design:**
   - Her modül < 200 satır
   - Clear function separation
   - No monolithic code

3. **Bootstrap Pattern:**
   - Stage0 → Stage1-simple → Stage2
   - Self-hosting path validated

---

## 🚨 SORUN ÇÖZME REHBERİ

### Senaryo 1: YZ_11 Context Doldu

**Belirti:**
- YZ_11 yavaşladı
- Token warning'leri
- Response quality düştü

**Çözüm:**
```bash
# Yeni YZ ata (YZ_12)
# NEXT_AI_START_HERE.md güncelle
# Kalan task'ları YZ_12'ye devret
```

### Senaryo 2: Stage0 Compile Error

**Belirti:**
- Stage0 syntax error veriyor
- C dosyaları boş
- Function bodies eksik

**Çözüm:**
1. Stage0-friendly constraints'i hatırlat
2. Problematic syntax'ı göster
3. Basit alternatif öner

**Örnek:**
```mlp
❌ YANLIŞ: state.position = state.position + 1
✅ DOĞRU: pos = pos + 1  (separate variable)
```

### Senaryo 3: Binary Çalışmıyor

**Belirti:**
- gcc link error
- Segmentation fault
- Missing symbols

**Çözüm:**
1. C dosyalarını kontrol et
2. Function signatures doğru mu?
3. Main function var mı?
4. Gerekirse MM_05'e escalate et

### Senaryo 4: Self-Compile Test Fail

**Belirti:**
- melpc_stage2 MLP dosyası parse edemiyor
- C output yanlış
- Test executable crash ediyor

**Çözüm:**
1. Basit test case kullan (hello world)
2. Parser logic'i kontrol et
3. Codegen output'u manuel kontrol et

---

## 📊 ZAMANLAMAve MILESTONE'LAR

**Hedef Süre:** 2 saat

| Zaman | Milestone | Check |
|-------|-----------|-------|
| +30 dk | 4 MLP modülü complete | ls stage1-simple/*.mlp |
| +45 dk | Stage0 compile complete | ls stage2/*.c |
| +60 dk | gcc build complete | ls stage2/melpc_stage2 |
| +75 dk | Self-compile test PASS | ./melpc_stage2 test.mlp |
| +90 dk | YZ_11 rapor complete | cat YZ_11_RAPOR.md |
| +120 dk | UA_03 rapor + MM_05 submit | DONE ✅ |

**Takip:**
- Her 30 dk: YZ_11 progress check
- +60 dk: Milestone review (50% olmalı)
- +120 dk: Final review ve rapor

---

## 📚 REFERANSLAR

**Teknik Dökümanlar:**
- [0-TODO_SELFHOSTING.md](../../0-TODO_SELFHOSTING.md) - Master TODO
- [NEXT_AI_START_HERE.md](../../NEXT_AI_START_HERE.md) - YZ_11 assignment
- [UA_02_RAPOR_TODO2_IMPORT_SYSTEM.md](UA_02_RAPOR_TODO2_IMPORT_SYSTEM.md) - TODO #2 raporu

**Kod Lokasyonları:**
- Stage0: `MELP/C/stage0/melpc`
- Stage1-simple: `MELP/MLP/stage1-simple/` (YZ_11 buraya yazar)
- Stage2: `MELP/MLP/stage2/` (compile output buraya)
- Examples: `examples/*.mlp` (test için)

**Stage0 Kısıtlamalar:**
```mlp
-- ✅ ÇALIŞIR
function add(a; b)
    Numeric result
    result = a + b
    return result
end_function

-- ❌ ÇALIŞMAZ (struct member access)
function update_state(state)
    state.position = state.position + 1  -- ERROR!
end_function

-- ✅ ÇALIŞIR (workaround)
function update_position(pos)
    Numeric new_pos
    new_pos = pos + 1
    return new_pos
end_function
```

---

## ✅ RAPOR ŞABLONU

**Görev bitince `UA_03_RAPOR_TODO3_STAGE1_SIMPLE.md` oluştur:**

```markdown
# UA_03 RAPOR: TODO #3 (Stage1-Simple)

**Tarih:** [bitiriş tarihi]
**Süre:** [gerçek süre]
**YZ:** YZ_11 (+ YZ_12 if needed)
**Durum:** [✅ COMPLETE / ⚠️ PARTIAL / ❌ FAILED]

## Executive Summary
[2-3 paragraf özet]

## Deliverables
1. ✅/❌ 4 MLP modülü
2. ✅/❌ Stage0 compile
3. ✅/❌ melpc_stage2 binary
4. ✅/❌ Self-compile test

## Metrics
- Süre: X saat (Hedef: 2 saat)
- Lines of code: XXX satır
- Test sonuçları: X/4 PASS

## Sorunlar
[varsa listele]

## TODO #4 Önerileri
[sonraki adım için öneriler]
```

---

## 🚀 BAŞLANGIÇ KOMUTLARIçok (YENİ ÜA İÇİN)

```bash
# 1. Durumu kontrol et
cd /home/pardus/projeler/MLP/MLP-GCC
cat NEXT_AI_START_HERE.md | grep -A 20 "YZ_11"

# 2. YZ_11 çalışmalarını gör
ls -lh MELP/MLP/stage1-simple/

# 3. Task progress
# (YZ_11'in todo list'ine bak)

# 4. YZ_11'e talimat ver
# "YZ_11, Task 2'yi (parser_simple.mlp) tamamla."

# 5. Her 30 dk kontrol et
watch -n 1800 'ls -lh MELP/MLP/stage1-simple/'
```

---

**HAZIR! SEN ŞİMDİ YZ_11'İ YÖNETİYORSUN!** 🚀

**İlk görevin:** YZ_11'e "Task 2 (parser_simple.mlp)'yi tamamla" talimatını ver.

**MM_05 monitoring modunda, sorun olursa escalate et!**
