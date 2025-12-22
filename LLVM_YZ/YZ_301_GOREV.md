# YZ_301: Workspace Cleanup - Eski Yapıları Temizle

**Tarih:** 22 Aralık 2025  
**Öncelik:** 🔴 YÜKSEK (YZ_300 başlamadan önce!)  
**Süre:** ~20 dakika (Tamamlandı!)  
**Durum:** ✅ TAMAMLANDI

**Sonuç:** `YZ_301_TAMAMLANDI.md`

---

## 🎯 GÖREV TANIMI

**Hedef:** YZ_300 (Self-Hosting) başlamadan önce workspace'i temizle.

**Neden Gerekli:**
- Eski denemeler/yarım kalmış dosyalar karışıklık yaratıyor
- Deprecated dosyalar disk alanı tüketiyor
- Temiz workspace = net odak = hızlı development

**Kriter:** Sadece aktif, production-ready dosyalar kalsın.

---

## 📋 TEMİZLİK PLANI

### Phase 1: Analiz (15 dk)
**Görev:** Temizlenecek klasörleri tespit et

**Kontrol edilecekler:**
1. `temp/` - Geçici dosyalar
2. `archive/` - Eski backuplar
3. `stage_0_YZ/` - Eski YZ görevleri (50+ dosya!)
4. `stage_1_YZ/` - Stage 1 denemeleri
5. `stage1_bug_YZ/` - Bug tracking (deprecated?)
6. `melp_workshop/` - Workshop denemeleri
7. `YZ_refactor/` - Eski refactor denemeleri
8. `ust_akil_YZ/` - Üst Akıl arşivi
9. `*.backup` dosyaları
10. `*.old` dosyaları

---

### Phase 2: Kategorizasyon (15 dk)

**3 Kategori:**

#### 1️⃣ **PRESERVE (Koru)** ✅
**Kriter:** Aktif kullanımda veya referans değeri yüksek

**Örnekler:**
```
✅ compiler/stage0/         - Aktif C compiler
✅ compiler/stage1/         - MELP compiler (devam ediyor)
✅ runtime/                 - Runtime libraries
✅ tests/                   - Test suite
✅ docs/                    - Documentation
✅ examples/                - Örnek programlar
✅ LLVM_YZ/                 - Aktif YZ görevleri (200-300 serileri)
✅ README.md, ARCHITECTURE.md, TODO_MODERN_LANGUAGE.md
✅ Makefile, .gitignore
```

#### 2️⃣ **ARCHIVE (Arşivle)** 📦
**Kriter:** Geçmişe referans olarak saklanmalı ama aktif değil

**Örnekler:**
```
📦 stage_0_YZ/              - YZ_01-99 historical records
📦 stage_1_YZ/              - Stage 1 denemeleri
📦 stage1_bug_YZ/           - Bug tracking history
📦 ust_akil_YZ/             - Üst Akıl devir teslim arşivi
📦 YZ_refactor/             - Eski refactor denemeleri
📦 *.backup, *.old          - Backup dosyaları
```

**Hedef Klasör:** `archive/historical_YZ/`

#### 3️⃣ **DELETE (Sil)** 🗑️
**Kriter:** Gereksiz, duplicate, veya bozuk dosyalar

**Örnekler:**
```
🗑️ temp/ (içindekiler)     - Geçici test dosyaları
🗑️ melp_workshop/temp/     - Workshop temp files
🗑️ *.s (eski assembly)     - LLVM'e geçtik, assembly deprecated
🗑️ Duplicate dosyalar      - Aynı içerik farklı yerde
🗑️ 0 byte dosyalar         - Boş dosyalar
🗑️ Build artifacts         - .o, .out files
```

---

### Phase 3: Yedekleme (10 dk)

**Önce yedek al!**

```bash
# Full backup before cleanup
cd /home/pardus/projeler/MLP/MLP
tar -czf ../MELP_backup_$(date +%Y%m%d_%H%M%S).tar.gz .

# Verify backup
ls -lh ../MELP_backup_*.tar.gz
```

---

### Phase 4: Arşivleme (20 dk)

**Arşiv yapısı oluştur:**

```bash
# Create archive structure
mkdir -p archive/historical_YZ/{stage0,stage1,bugs,upper_mind,refactors,backups}

# Move historical YZ records
mv stage_0_YZ/ archive/historical_YZ/stage0/
mv stage_1_YZ/ archive/historical_YZ/stage1/
mv stage1_bug_YZ/ archive/historical_YZ/bugs/
mv ust_akil_YZ/ archive/historical_YZ/upper_mind/
mv YZ_refactor/ archive/historical_YZ/refactors/

# Move backup files
find . -name "*.backup" -o -name "*.old" | while read f; do
    mkdir -p archive/historical_YZ/backups/$(dirname "$f")
    mv "$f" archive/historical_YZ/backups/$(dirname "$f")/
done

# Create README in archive
cat > archive/historical_YZ/README.md << 'EOF'
# Historical YZ Tasks Archive

**Archived:** 22 Aralık 2025  
**Reason:** YZ_301 - Workspace cleanup before YZ_300 (Self-Hosting)

## Contents

- `stage0/` - YZ_01-99 (Stage 0 development history)
- `stage1/` - Stage 1 self-hosting attempts
- `bugs/` - Bug tracking from Stage 1 development
- `upper_mind/` - Üst Akıl handoff documents
- `refactors/` - Old refactoring attempts
- `backups/` - Backup files (*.backup, *.old)

**Note:** These are historical records. For active work, see:
- `compiler/stage0/` - Active C compiler
- `compiler/stage1/` - Active MELP compiler
- `LLVM_YZ/` - Active YZ tasks (200-300 series)
EOF
```

---

### Phase 5: Temizlik (15 dk)

**Gereksiz dosyaları sil:**

```bash
# Clean temp directories
rm -rf temp/* 
rm -rf melp_workshop/temp/*

# Remove old assembly files (LLVM'e geçtik)
find tests/ -name "*.s" -type f -delete

# Remove build artifacts
find . -name "*.o" -type f -delete
find . -name "*.out" -type f -delete
find . -name "a.out" -type f -delete

# Remove 0-byte files
find . -type f -size 0 -delete

# Remove duplicate test files (if any)
# (Manuel kontrol gerekebilir)
```

---

### Phase 6: Reorganizasyon (10 dk)

**Aktif dosyaları düzenle:**

```bash
# LLVM_YZ klasörünü temizle
cd LLVM_YZ/

# Aktif görevleri grupla
mkdir -p active/ completed/ planning/

# Active tasks (300+ serisi)
mv YZ_300_*.md active/

# Completed (200 serisi)
mv YZ_200_TAMAMLANDI.md YZ_201_TAMAMLANDI.md completed/
mv YZ_202_TAMAMLANDI.md completed/
# ... (diğerleri)

# Planning docs
mv TODO.md planning/
mv README.md planning/

# Create clean index
cat > README.md << 'EOF'
# LLVM Backend & Modern Features - Active Tasks

**Last Updated:** 22 Aralık 2025

## Active Tasks
- `active/` - Current work (YZ_300+)
- `completed/` - Finished tasks (YZ_200-213)
- `planning/` - Roadmaps and TODOs

## Quick Links
- [YZ_300: Self-Hosting](active/YZ_300_GOREV.md) - 🔄 IN PROGRESS
- [Completed Features](completed/) - ✅ Lists, Maps, Generics, etc.

See `../TODO_MODERN_LANGUAGE.md` for full roadmap.
EOF
```

---

### Phase 7: Doğrulama (10 dk)

**Temizlik sonrası kontroller:**

```bash
# 1. Compile testi
cd /home/pardus/projeler/MLP/MLP/compiler/stage0
make clean
make
# Derlemeli! ✅

# 2. Test suite çalışıyor mu?
cd tests/llvm/
./run_quick_test.sh
# En az 5/5 passing olmalı ✅

# 3. Git durumu temiz mi?
git status
# Untracked files minimal olmalı ✅

# 4. Disk kullanımı azaldı mı?
du -sh /home/pardus/projeler/MLP/MLP/
# Önceki boyut: ~XXX MB
# Sonraki boyut: <XXX MB (daha küçük) ✅
```

---

## ✅ TAMAMLANMA KRİTERİ

### Must Have:
- [x] Backup alındı
- [x] Eski YZ klasörleri arşivlendi
- [x] Temp dosyalar temizlendi
- [x] Build artifacts silindi
- [x] Compiler hala çalışıyor
- [x] Test suite passing

### Should Have:
- [x] LLVM_YZ reorganize edildi
- [x] Duplicate dosyalar temizlendi
- [x] Archive README oluşturuldu
- [x] Disk kullanımı %20+ azaldı

---

## 📊 BEKLENTİLER

**Temizlik Öncesi:**
```
stage_0_YZ/          ~2,000 files
stage_1_YZ/          ~500 files
stage1_bug_YZ/       ~100 files
ust_akil_YZ/         ~50 files
YZ_refactor/         ~200 files
temp/                ~1,000 files
Duplicates           ~500 files
-------------------------
TOTAL:               ~4,350 files
```

**Temizlik Sonrası:**
```
archive/historical_YZ/    ~2,850 files (moved)
compiler/                 ~200 files (active)
LLVM_YZ/active/          ~20 files (active)
LLVM_YZ/completed/       ~30 files (reference)
tests/                   ~150 files (clean)
docs/                    ~50 files
-------------------------
TOTAL:                   ~500 active files
ARCHIVED:                ~2,850 files
DELETED:                 ~1,000 files
```

**Kazanç:**
- %88 daha az aktif dosya
- Daha hızlı `grep` / `find`
- Daha net workspace
- YZ_300 için temiz başlangıç

---

## ⚠️ DİKKAT EDİLECEKLER

### SAKLA (Critical Files):
```
✅ Makefile
✅ .gitignore
✅ compiler/stage0/ (TÜM DOSYALAR!)
✅ compiler/stage1/ (TÜM DOSYALAR!)
✅ runtime/ (TÜM DOSYALAR!)
✅ tests/ (test suite)
✅ LLVM_YZ/YZ_300_*.md (aktif görev)
✅ LLVM_YZ/YZ_2XX_TAMAMLANDI.md (referans)
✅ README.md, ARCHITECTURE.md, TODO_MODERN_LANGUAGE.md
```

### SİLME (Never Delete):
```
❌ compiler/ klasörü içinden HİÇBİR ŞEY
❌ runtime/ klasörü içinden HİÇBİR ŞEY
❌ LLVM_YZ/YZ_300_*.md (aktif görev)
❌ Makefile, .gitignore
❌ Main documentation files
```

---

## 🚀 EXECUTION

**Sıra:**
1. Phase 1: Analiz (liste yap)
2. Phase 3: Backup (önce yedek!)
3. Phase 4: Arşivle
4. Phase 5: Temizle
5. Phase 7: Doğrula
6. Phase 6: Reorganize (son)

**Commit Mesajı:**
```
YZ_301: Workspace Cleanup - Pre-YZ_300 🧹

- Archived: stage_0_YZ, stage_1_YZ, ust_akil_YZ (2,850 files)
- Cleaned: temp/, build artifacts, duplicates (1,000 files)
- Reorganized: LLVM_YZ/ structure (active/completed/planning)
- Verified: Compiler working, tests passing ✅

Before: ~4,350 files
After: ~500 active files + 2,850 archived

Ready for YZ_300 (Self-Hosting) with clean workspace! 🚀
```

---

## 📝 RAPOR ŞABLONU

YZ_301 bitince `LLVM_YZ/YZ_301_TAMAMLANDI.md` oluştur:

```markdown
# YZ_301: Workspace Cleanup - TAMAMLANDI ✅

**Tarih:** 22 Aralık 2025
**Süre:** X saat

## Özet
- Archived: X files → archive/historical_YZ/
- Deleted: X files (temp, duplicates, build artifacts)
- Reorganized: LLVM_YZ/ structure
- Disk saved: XX MB

## Before/After
- Before: 4,350 files, XXX MB
- After: 500 active files, XX MB
- Reduction: 88% fewer files

## Verification
✅ Compiler works
✅ Tests pass (X/X)
✅ Git clean
✅ No critical files lost

**Ready for YZ_300!** 🚀
```

---

## ✅ SUCCESS CRITERIA

**YZ_301 başarılı sayılır eğer:**
1. Backup alındı ve doğrulandı ✅
2. Eski YZ klasörleri `archive/` taşındı ✅
3. Temp/duplicate dosyalar silindi ✅
4. Compiler hala derlenip çalışıyor ✅
5. Test suite passing ✅
6. LLVM_YZ reorganize edildi ✅
7. Git repo temiz (manageable untracked files) ✅
8. YZ_301_TAMAMLANDI.md raporu yazıldı ✅

**→ YZ_300 başlayabilir!** 🎯

---

**Estimated Time:** 1-2 hours  
**Risk:** Düşük (backup var)  
**Impact:** Yüksek (temiz workspace)

🧹 **LET'S CLEAN UP!** 🧹
