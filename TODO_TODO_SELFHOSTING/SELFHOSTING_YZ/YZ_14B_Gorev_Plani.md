# YZ_14B GÖREV PLANI - TODO_SELFHOSTING.md Cleanup

**YZ:** SELFHOSTING_YZ_14B  
**Tarih:** 30 Aralık 2025, 16:00  
**TODO:** Gündem Dışı - Documentation Cleanup  
**Yönetici:** UA_04  
**Süre Hedefi:** 30 dakika  
**Durum:** 🟢 AKTIF

---

## 🎯 SENİN GÖREVİN (YZ_14B)

**Sorun:** 0-TODO_SELFHOSTING.md karışık durumda
- TODO numaralandırması tutarsız (TODO #3 vs #4)
- Roadmap vs içerik uyumsuz
- YZ'ler karışıyor (YZ_14 bile zorlandı)

**Görev:** TODO dosyasını temizle, yeniden yapılandır

---

## 📋 TASK LİSTESİ (5 Task)

### Task 1: Mevcut TODO'yu Yedekle ⏳ NEXT

**Adımlar:**
```bash
cd /home/pardus/projeler/MLP/MLP-GCC
cp 0-TODO_SELFHOSTING.md 0-TODO_SELFHOSTING.md.backup_$(date +%Y%m%d_%H%M%S)
git add 0-TODO_SELFHOSTING.md.backup*
git commit -m "YZ_14B: TODO yedeklendi - cleanup öncesi"
```

**Deliverable:** Backup dosyası oluşturuldu

---

### Task 2: TODO Yapısını Analiz Et ⏳

**Yapılacaklar:**

1. **Mevcut TODO'ları listele:**
   ```bash
   grep -n "^## TODO #" 0-TODO_SELFHOSTING.md
   grep -n "^### TODO #" 0-TODO_SELFHOSTING.md
   ```

2. **Roadmap vs İçerik Karşılaştır:**
   - Roadmap: Satır 20-50 civarı
   - TODO #0-6 içerik: Satır 1000+ civarı
   - Tutarsızlıkları tespit et

3. **Tamamlanan TODO'ları Belirle:**
   - ✅ TODO #0: Stage0+Stage1 Type Infrastructure
   - ✅ TODO #1: (TODO #0 ile aynı - tekrar?)
   - ✅ TODO #2: Import System
   - ✅ TODO #3: Stage1-simple compiler (Bootstrap)
   - ✅ TODO #4: Runtime modularization (YZ_12/13/14)
   - ⏳ TODO #5: Stdlib Complete (Sonraki)
   - ⏳ TODO #6: Self-hosting convergence

**Deliverable:** Tutarsızlık listesi

---

### Task 3: TODO Numaralandırmasını Düzelt ⏳

**Hedef:** Tutarlı, net numaralandırma

**Yeni Yapı:**
```markdown
# 📋 TODO: MLP-GCC %100 SELF-HOSTING (v2.1 - CLEANED)

## 🎯 6 HAFTALIK ROADMAP (Net Numaralandırma)

TODO #0: [DEPRECATED - TODO #1 ile birleştirildi]
TODO #1: PHASE 0-3 COMPLETE ✅ (Stage0+Stage1 Type Infrastructure)
TODO #2: IMPORT_SYSTEM ✅ (ast_nodes.mlp + import_handler.mlp)
TODO #3: STAGE1_SIMPLE ✅ (Bootstrap pattern)
TODO #4: RUNTIME_MODULARIZE ✅ (8 modül, STATELESS Dict)
TODO #5: STDLIB_COMPLETE ⏳ (Next - YZ_15)
TODO #6: SELF_HOSTING ⏳ (Convergence test)

## 📊 MEVCUT DURUM (30 Aralık 2025 - GÜNCEL)

[Tablo - TODO #1-4 ✅, #5-6 ⏳]

## 📋 TODO DETAYLARI

### TODO #1: PHASE 0-3 COMPLETE ✅
[Detaylar]

### TODO #2: IMPORT_SYSTEM ✅
[Detaylar]

### TODO #3: STAGE1_SIMPLE ✅
[Detaylar]

### TODO #4: RUNTIME_MODULARIZE ✅
[Detaylar - YZ_14 az önce güncelledi]

### TODO #5: STDLIB_COMPLETE ⏳
[Detaylar - YZ_15 için hazır]

### TODO #6: SELF_HOSTING ⏳
[Detaylar]
```

**Önemli:**
- TODO #0 kaldır veya deprecated işaretle
- Her TODO'nun tek bir bölümü olsun
- Tamamlananlar ✅, sonrakiler ⏳

**Deliverable:** Temiz TODO yapısı

---

### Task 4: Referansları Güncelle ⏳

**Kontrol edilecekler:**

1. **NEXT_AI_START_HERE.md:**
   - TODO #4 referansları doğru mu?
   - YZ_15 için TODO #5 hazır mı?

2. **YZ Raporları:**
   - YZ_12/13/14 raporlarında TODO #4 referansları
   - Tutarsızlık varsa düzelt

3. **Mastermind Dosyaları:**
   - `TODO_TODO_SELFHOSTING/SELFHOSTING_MM/` altında
   - TODO numaraları güncel mi?

**Deliverable:** Referanslar güncellendi

---

### Task 5: Validation + Commit ⏳

**Test:**
```bash
# TODO yapısı doğru mu?
grep -c "^### TODO #[1-6]:" 0-TODO_SELFHOSTING.md  # Should be 6

# Tamamlananlar işaretli mi?
grep "TODO #[1-4].*✅" 0-TODO_SELFHOSTING.md  # Should show 4 completed

# Roadmap uyumlu mu?
grep -A 10 "ROADMAP" 0-TODO_SELFHOSTING.md
```

**Commit:**
```bash
git add 0-TODO_SELFHOSTING.md NEXT_AI_START_HERE.md
git commit -m "YZ_14B: TODO cleanup - numaralandırma düzeltildi, yapı temizlendi"
```

**Deliverable:** Temiz, tutarlı TODO dosyası

---

## 🚫 YAPAMAZSIN

**❌ İçerik değişikliği yapma:**
- Sadece numaralandırma ve yapı düzelt
- TODO detaylarını değiştirme
- Tamamlanmış işleri silme

**❌ Yeni TODO ekleme:**
- Mevcut TODO #1-6 yeterli
- İçerik değiştirme, sadece organize et

**❌ Backup'ı silme:**
- Backup dosyası kalmalı
- Git'te commit edilmeli

---

## ✅ BAŞARI KRİTERLERİ

**Görev bittiğinde:**

1. ✅ **Backup oluşturuldu:**
   - [ ] 0-TODO_SELFHOSTING.md.backup_YYYYMMDD_HHMMSS
   - [ ] Git'te commit edildi

2. ✅ **TODO yapısı temiz:**
   - [ ] TODO #1-6 net numaralandırma
   - [ ] Her TODO tek bir bölüm
   - [ ] Roadmap vs içerik uyumlu

3. ✅ **Tamamlananlar işaretli:**
   - [ ] TODO #1-4 ✅ işaretlendi
   - [ ] TODO #5-6 ⏳ işaretlendi

4. ✅ **Referanslar güncel:**
   - [ ] NEXT_AI_START_HERE.md güncel
   - [ ] YZ raporları tutarlı

5. ✅ **Git commit:**
   - [ ] Temiz commit mesajı
   - [ ] Backup + cleaned dosya

---

## 📚 REFERANSLAR

**Dosyalar:**
- `/home/pardus/projeler/MLP/MLP-GCC/0-TODO_SELFHOSTING.md` (ana dosya)
- `NEXT_AI_START_HERE.md` (YZ_15 için hazır)
- `TODO_TODO_SELFHOSTING/SELFHOSTING_YZ/YZ_14_RAPOR.md` (en son rapor)

**Kontrol:**
```bash
# Mevcut yapı
wc -l 0-TODO_SELFHOSTING.md  # 1842 satır
grep -c "TODO #" 0-TODO_SELFHOSTING.md  # Kaç TODO var?
```

---

## 📌 İLK ADIM

**ŞİMDİ YAP:**

1. Backup oluştur:
   ```bash
   cd /home/pardus/projeler/MLP/MLP-GCC
   cp 0-TODO_SELFHOSTING.md 0-TODO_SELFHOSTING.md.backup_$(date +%Y%m%d_%H%M%S)
   ```

2. TODO yapısını analiz et:
   ```bash
   grep -n "^## TODO #\|^### TODO #" 0-TODO_SELFHOSTING.md
   ```

3. Task 1'e başla!

---

**İYİ ÇALIŞMALAR YZ_14B! UA_04 seni takip ediyor.** 🚀

*Oluşturuldu: 30 Aralık 2025, 16:00*  
*Yönetici: UA_04*  
*Süre: 30 dakika hedef*
