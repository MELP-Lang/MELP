# 🎯 MASTERMIND ATAMASI: TODO #0 SELFHOSTING

**Tarih:** 29 Aralık 2025  
**Mastermind:** MM_02  
**Hedef:** SELFHOSTING_UA (Üst Akıl)  
**TODO:** 0-TODO_SELFHOSTING.md (6 Haftalık Roadmap)

---

## 📋 GÖREV TANIMLAMASI

**Merhaba SELFHOSTING_UA!**

Mastermind MM_02 olarak sana **TODO #0: SELFHOSTING** görevini atıyorum.

### 🎯 Hedef

**6 hafta sonunda MLP-GCC %100 self-hosting olacak!**

```bash
# Başarı kriteri:
cd MELP/MLP/stage1
./melpc_stage1 melpc.mlp -o melpc_stage2  # MLP kendini derliyor!
./melpc_stage2 melpc.mlp -o melpc_stage3  # Stage2 kendini derliyor!
diff melpc_stage2 melpc_stage3            # Convergence: Aynı!
# 🎉 PRODUCTION-READY SELF-HOSTING!
```

---

## 📖 ZORUNLU OKUMALAR

Sırayla oku:

1. **[0-TODO_SELFHOSTING.md](../../0-TODO_SELFHOSTING.md)** (1007 satır)
   - 6 haftalık roadmap
   - TODO #1-5 detayları
   - Phase yapısı (P0-P4 her TODO için)

2. **[TODO_SELFHOSTING.txt](../../TODO_SELFHOSTING.txt)** (142 satır)
   - İzinli dosyalar listesi
   - **Sadece bu dosyalara dokunulacak!**

3. **[YZ_KURALLAR.md](../../YZ_KURALLAR.md)** (505 satır)
   - YZ workflow
   - Test zorunluluğu
   - Raporlama protokolü

4. **[TODO_KURALLARI.md](../../TODO_KURALLARI.md)** (570 satır)
   - Genel kurallar
   - 5 Temel Esas
   - Escalation protokolü

---

## 🛡️ 5 TEMEL ESAS (KORUMALısın!)

```
1. MODULAR     → Her dosya max 500 satır
2. GCC         → MLP → C → GCC → Binary (ASM yok artık!)
3. STO         → Smart Type Optimization (numeric → int64/double)
4. STATELESS   → Global state yasak
5. STRUCT+FUNC → OOP yok, sadece struct + functions
```

**Bu esasları ihlal eden YZ çıkarsa hemen MM_02'ye bildir!**

---

## 🗓️ TODO BREAKDOWN (Senin Görevin)

### TODO #1: SELFHOSTING (Hafta 1-2) 🔴 KRİTİK

**Hedef:** Codegen ASM→C dönüşümü

**Phase'ler (0-TODO_SELFHOSTING.md'de detaylı):**
- Phase 0: Hazırlık ve temel codegen (YZ_01)
- Phase 1: Control flow & functions (YZ_02)
- Phase 2: Bootstrap test (YZ_03)
- Phase 3: Stage1 derleme (YZ_04)
- Phase 4: Self-hosting temel (YZ_05)

**Senin Yapacakların:**
1. Her phase için YZ_01-YZ_05'i ata
2. Her phase tamamlandığında test et
3. Test başarısızlığı varsa düzelt veya MM_02'ye bildir
4. Phase raporları al (YZ_XX_RAPOR.md)

### TODO #2: IMPORT_SYSTEM (Hafta 3) 🔴 KRİTİK

**Hedef:** P2'den import sistemini al, LLVM temizle

**Görevler:**
- P2 import modülünü kopyala (~3000 satır)
- LLVM bağımlılıklarını C output'a dönüştür
- Circular import testi
- Namespace desteği

**YZ Sayısı:** 2-3 YZ

### TODO #3: RUNTIME_MODULARIZE (Hafta 4) 🔴 KRİTİK

**Hedef:** runtime.c'yi böl (3245 → 8 modül)

**Kritik:** 500 satır kuralı ihlali giderilecek!

**Modüller:**
1. runtime_core.c (500 satır)
2. runtime_print.c (300 satır)
3. runtime_math.c (300 satır)
4. runtime_string.c (500 satır)
5. runtime_list.c (500 satır)
6. runtime_dict.c (300 satır)
7. runtime_file_io.c (500 satır)
8. runtime_utils.c (345 satır)

**YZ Sayısı:** 2 YZ

### TODO #4: STDLIB_COMPLETE (Hafta 5) 🟡 YÜKSEK

**Hedef:** P2'den eksik stdlib modülleri

**Alınacaklar:**
- mlp_list.c/h (328 satır)
- mlp_optional.c/h (191 satır)
- mlp_state.c/h (426 satır)
- mlp_panic.c/h (103 satır)
- mlp_io.c/h (268 satır)
- file_io.c/h (384 satır)
- mlp_math.c/h (243 satır)

**YZ Sayısı:** 1-2 YZ

### TODO #5: POLISH & FINAL TEST (Hafta 6) 🟢 ORTA

**Hedef:** Final doğrulama ve convergence

**Görevler:**
- Stage0 → Stage1 → Stage2 → Stage3 zinciri
- Convergence testi (Stage2 == Stage3)
- Tüm testler %100 geçmeli
- Performance benchmark
- Dokümantasyon

**YZ Sayısı:** 1-2 YZ

---

## 📊 PROGRESS TRACKING (Senin Sorumluluğun)

Her TODO için şu tabloyu doldur:

```markdown
### TODO #1: SELFHOSTING

| Phase | YZ | Süre Tahmini | Gerçek Süre | Durum | Test |
|-------|-----|--------------|-------------|-------|------|
| P0 | YZ_01 | 3-5h | ? | ⏳ | ❌ |
| P1 | YZ_02 | 3-5h | ? | ⏳ | ❌ |
| P2 | YZ_03 | 2-3h | ? | ⏳ | ❌ |
| P3 | YZ_04 | 3-4h | ? | ⏳ | ❌ |
| P4 | YZ_05 | 2-3h | ? | ⏳ | ❌ |

**Toplam Tahmini:** 13-20 saat  
**Gerçek Toplam:** ?
```

---

## 🚨 ESCALATION PROTOKOLÜ

### MM_02'ye Hemen Bildir:

1. **5 Temel Esas İhlali**
   - Dosya 500 satırı geçti
   - LLVM bağımlılığı eklendi
   - Global state kullanıldı
   - OOP pattern görüldü

2. **TODO Süresi 2x Aştı**
   - TODO #1 hedef: 2 hafta, aşıldı: 4 hafta
   - Stratejik revizyon gerekli

3. **YZ Sürekli Başarısız**
   - Aynı YZ 3 kez test başarısız
   - UA değişimi gerekebilir

### Kendin Çöz:

- Test başarısızlığı (YZ düzeltsin)
- Küçük bug (YZ halleder)
- Syntax hatası (YZ düzeltir)

---

## 🧪 TEST ZORUNLULUĞU

**Her phase sonunda MUTLAKA test çalıştır!**

```bash
cd MELP/C/stage0
make clean && make
./run_tests.sh
```

**Test geçmeden sonraki phase'e geçme!**

Test başarısız → YZ düzeltsin (3 denemede başarısız → MM_02'ye bildir)

---

## 📝 RAPORLAMA PROTOKOLÜ

### Her Phase Tamamlandığında:

1. **YZ Raporu Al** (`YZ_XX_RAPOR.md`)
2. **Test Sonuçlarını Doğrula**
3. **NEXT_AI_START_HERE.md Güncelle**
4. **MM_02'ye Özet Rapor**

**Özet Rapor Formatı:**
```markdown
## UA Raporu - Phase X

**YZ:** YZ_XX  
**Phase:** X (Görev adı)  
**Durum:** ✅ Tamamlandı / ❌ Başarısız  
**Süre:** X saat

**Sonuç:**
- Yapılanlar listesi
- Test sonuçları
- Sorunlar (varsa)

**Sonraki Phase:** YZ_YY'ye geçilsin mi?
```

---

## 🎯 BAŞARI KRİTERLERİ (TODO #0 Final)

6 hafta sonunda şunlar MUTLAKA olmalı:

### Teknik:
- ✅ MLP → C codegen çalışıyor (ASM yok artık!)
- ✅ Import sistemi tam (P2'den alınmış)
- ✅ Runtime modüler (8 modül, <500 satır/dosya)
- ✅ stdlib tam (list, optional, state, panic, io, math)
- ✅ Self-hosting: Stage2 == Stage3 (convergence)

### Mimari:
- ✅ 500 satır kuralı: Hiçbir dosya 500 satırı geçmiyor
- ✅ LLVM bağımlılığı: Tamamen temizlenmiş
- ✅ GCC backend: C → GCC → Binary
- ✅ STATELESS: Global state yok
- ✅ STRUCT+FUNC: OOP pattern yok

### Test:
- ✅ Tüm testler %100 geçiyor
- ✅ Bootstrap stable (Stage2 == Stage3)
- ✅ Convergence başarılı

**Bu kriterler karşılanmazsa TODO başarısız sayılır!**

---

## 🚀 BAŞLANGIÇ KOMUTU

```bash
# UA workspace'inde çalış
cd /home/pardus/projeler/MLP/MLP-GCC/TODO_TODO_SELFHOSTING/SELFHOSTING_UA

# İlk YZ'yi ata (YZ_01, Phase 0)
# YZ_01'e talimat ver, NEXT_AI_START_HERE.md güncelle
```

---

## 💬 İLK RAPORUN

Tüm belgeleri okuduktan sonra bana **şu formatta** rapor ver:

```markdown
# SELFHOSTING_UA İlk Rapor

**Tarih:** 29 Aralık 2025  
**UA:** SELFHOSTING_UA

✅ HAZIRLIK TAMAMLANDI

📋 Okuduklarım:
- 0-TODO_SELFHOSTING.md ✓
- TODO_SELFHOSTING.txt ✓
- YZ_KURALLAR.md ✓
- TODO_KURALLARI.md ✓

🎯 TODO BREAKDOWN:
TODO #1-5 için phase breakdown hazır:
- TODO #1: 5 phase (YZ_01-YZ_05)
- TODO #2: 3 phase (YZ_06-YZ_08)
- TODO #3: 2 phase (YZ_09-YZ_10)
- TODO #4: 2 phase (YZ_11-YZ_12)
- TODO #5: 2 phase (YZ_13-YZ_14)

🛡️ 5 TEMEL ESAS:
- MODULAR ✓
- GCC ✓
- STO ✓
- STATELESS ✓
- STRUCT+FUNC ✓

📊 İZLEME PLANI:
- Her phase için progress tracking
- Test sonuçları dokümantasyonu
- Escalation protokolü hazır

🚀 DURUM:
Tüm ön hazırlıkları tamamladım.
YZ_01'i Phase 0'a atamaya hazırım.

Başlamak için onay bekliyorum.
```

---

## 📞 İLETİŞİM

**Mastermind MM_02'ye Ulaşma:**
- Kritik durumlar için hemen bildir
- Günlük özet raporlar gönder
- TODO tamamlandığında final rapor sun

**Kanal:** TODO_TODO_SELFHOSTING/SELFHOSTING_UA/ dizini

---

## 🎉 SON SÖZ

**6 hafta sonunda MLP-GCC PRODUCTION-READY olacak!** 🚀

**Başarılar!**

---

**Mastermind MM_02**  
29 Aralık 2025

**Not:** Bu görev Danışman PD_01 tarafından onaylanmıştır. Mimari esaslara uygun şekilde ilerle!
