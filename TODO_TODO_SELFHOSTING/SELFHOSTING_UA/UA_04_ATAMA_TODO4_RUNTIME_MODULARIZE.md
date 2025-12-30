# UA_04_ATAMA_TODO4_RUNTIME_MODULARIZE.md

**Tarih:** 30 Aralık 2025, 12:30  
**Üst Akıl:** UA_04  
**TODO:** #4 - Runtime Modularization  
**Öncelik:** 🔴 KRİTİK (MODULAR esası ihlali!)  
**Süre Hedefi:** 3-4 gün (pragmatik)  
**Mastermind Onayı:** MM_05 (30 Aralık 2025, 07:00)

---

## 📋 GÖREV ÖZETİ

**Sorun:** `MELP/C/stage0/runtime.c` = 3245 satır (MODULAR esasını ihlal ediyor! Max 500 satır olmalı)

**Hedef:** Runtime'ı 8 modüle böl, her biri < 500 satır

**6 Temel Esas Kontrolü:**
1. **MODULAR** → ❌ İHLAL (3245 satır!) ← DÜZELTİLECEK!
2. **GCC** → ✅ Korunacak
3. **STO** → ✅ Korunacak
4. **STATELESS** → ✅ Kontrol edilecek
5. **STRUCT+FUNC** → ✅ Korunacak
6. **MODÜL=ŞABLON** → ✅ Uygulanacak

---

## 🎯 YZ_12 GÖREVİ

### Phase 1: Runtime Analizi (1 gün)

**YZ_12, şunları yap:**

1. **runtime.c Analizi:**
   ```bash
   cd MELP/C/stage0
   wc -l runtime.c  # 3245 satır doğrula
   grep -c "^[a-zA-Z_].*{$" runtime.c  # Fonksiyon sayısı
   ```

2. **Fonksiyon Kategorileri Belirle:**
   - Memory management (GC, allocation)
   - String operations (concat, slice, compare)
   - Type conversion (numeric → string, etc.)
   - Array operations (index, append, etc.)
   - Error handling (panic, assert)
   - File I/O (read, write)
   - Print/Debug utilities
   - Runtime initialization

3. **8 Modül Planı Oluştur:**
   ```
   runtime.c (3245 satır) →
   ├── runtime_memory.c    (~500 satır) - GC + allocation
   ├── runtime_string.c    (~400 satır) - String ops
   ├── runtime_array.c     (~350 satır) - Array ops
   ├── runtime_types.c     (~400 satır) - Type conversion
   ├── runtime_io.c        (~450 satır) - File I/O
   ├── runtime_print.c     (~300 satır) - Print/debug
   ├── runtime_error.c     (~250 satır) - Error handling
   └── runtime_init.c      (~200 satır) - Initialization
   
   TOPLAM: ~2850 satır (optimizasyon ile azalacak)
   ```

4. **Bağımlılık Analizi:**
   - Hangi fonksiyonlar birbirine bağımlı?
   - Circular dependency riski var mı?
   - Header dosyası stratejisi ne olmalı?

5. **YZ_12_ANALIZ_RAPOR.md Yaz:**
   - 8 modül detayları
   - Fonksiyon listesi (her modülde hangi fonksiyonlar)
   - Bağımlılık grafiği
   - Tahmini süre (YZ_13 için)

---

## 🎯 YZ_13 GÖREVİ (YZ_12'den sonra)

### Phase 2: Implementation (2-3 gün)

**YZ_13, YZ_12'nin planına göre:**

1. **8 Modül Oluştur:**
   - Her modül < 500 satır
   - Header dosyaları (.h) oluştur
   - Makefile güncelle

2. **Test:**
   ```bash
   cd MELP/C/stage0
   make clean && make
   ./run_tests.sh
   ```
   **Tüm testler pass etmeli!**

3. **Commit:**
   ```bash
   git commit -m "YZ_13: Runtime modularization complete (3245 → 8 modüller)"
   ```

4. **YZ_13_IMPLEMENTATION_RAPOR.md Yaz:**
   - 8 modül final satır sayıları
   - Test sonuçları
   - Sorunlar (varsa) ve çözümler

---

## ✅ BAŞARI KRİTERLERİ

TODO #4 başarılı sayılır eğer:

1. ✅ `runtime.c` 8 modüle bölünmüş
2. ✅ Her modül < 500 satır
3. ✅ Tüm testler pass
4. ✅ MODULAR esası ihlali düzeltilmiş
5. ✅ 6 Temel Esas korunmuş
6. ✅ Makefile güncellenmiş
7. ✅ Git commit temiz

---

## 📚 REFERANSLAR

**Dosyalar:**
- `MELP/C/stage0/runtime.c` (3245 satır) - Analiz edilecek
- `MELP/C/stage0/Makefile` - Güncellenecek
- `docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md` - Mimari referans

**TODO Belgesi:**
- [0-TODO_SELFHOSTING.md](../../0-TODO_SELFHOSTING.md) - TODO #4 detayları

**6 Temel Esas:**
1. MODULAR - Her modül max 500 satır
2. GCC - MLP → C → GCC
3. STO - Smart Type Optimization
4. STATELESS - Global state yasak
5. STRUCT+FUNC - OOP yok
6. MODÜL=ŞABLON - Context pattern

---

## 🚨 ÖNEMLİ NOTLAR

**Dikkat edilecekler:**

1. **Backward compatibility:** Stage0'ın mevcut test suite'i çalışmalı
2. **No optimization:** Fonksiyonları aynen kopyala (refactor değil!)
3. **Header guards:** Tüm .h dosyalarında #ifndef/#define
4. **Circular dependency:** Önlemek için dikkatli planlama

**3 Deneme Kuralı:**
- Test fail ederse 3 deneme
- 3 denemede başarısız → UA_04'e rapor et

---

## 📊 ZAMAN TAHMİNİ

| YZ | Görev | Süre | Başarı Olasılığı |
|----|-------|------|------------------|
| YZ_12 | Analiz + Plan | 1 gün | %95 |
| YZ_13 | Implementation | 2-3 gün | %85 |
| **TOPLAM** | | **3-4 gün** | **%90** |

**Pragmatik buffer:** +1 gün (toplam 5 gün max)

---

**UA_04 Notu:** Bu TODO mimari temizlik. %100 başarı zorunlu, çünkü MODULAR esası ihlali kabul edilemez!

---

**Hazırlayan:** UA_04  
**Onaylayan:** MM_05  
**Başlatma:** 30 Aralık 2025, 12:30
