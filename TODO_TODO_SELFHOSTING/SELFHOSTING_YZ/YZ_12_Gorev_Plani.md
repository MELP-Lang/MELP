# YZ_12 GÖREV PLANI - Runtime.c Analizi

**YZ:** SELFHOSTING_YZ_12  
**Tarih:** 30 Aralık 2025, 12:45  
**TODO:** #4 - Runtime Modularization (Analiz Fazı)  
**Yönetici:** UA_04  
**Süre Hedefi:** 1 gün  
**Durum:** 🟢 AKTIF

---

## 🎯 SENİN GÖREVİN (YZ_12)

**Sorun:** `MELP/runtime/runtime.c` = 3245 satır → MODULAR esasını ihlal ediyor!  
**Görev:** Runtime'ı analiz et, 8 modüle bölme planı oluştur.

---

## 📋 TASK LİSTESİ (5 Task)

### Task 1: Runtime.c Fonksiyon Analizi ⏳ NEXT

**Hedef:** Tüm fonksiyonları kategorilere ayır.

**Adımlar:**

```bash
cd /home/pardus/projeler/MLP/MLP-GCC/MELP/runtime
wc -l runtime.c  # 3245 satır doğrula
grep -c "^[a-zA-Z_].*(" runtime.c  # Toplam fonksiyon sayısı
```

**Fonksiyon Kategorileri:**

1. **Memory Management** - GC, allocation, free
2. **String Operations** - concat, slice, compare, format
3. **Array Operations** - index, append, slice, iterate
4. **Type Conversion** - numeric ↔ string, type casting
5. **Error Handling** - panic, assert, error messages
6. **File I/O** - read, write, file operations
7. **Print/Debug** - print, yazdir, debug utilities
8. **Runtime Init** - initialization, cleanup

**Deliverable:** Fonksiyon listesi (kategorilendirilmiş)

---

### Task 2: 8 Modül Planı Oluştur ⏳

**Hedef:** Her modül < 500 satır olacak şekilde plan yap.

**8 Modül Taslağı:**

```
runtime.c (3245 satır) →
├── runtime_memory.c    (~500 satır) - GC + allocation + free
├── runtime_string.c    (~400 satır) - String ops (concat, slice, etc.)
├── runtime_array.c     (~350 satır) - Array ops (index, append, etc.)
├── runtime_types.c     (~400 satır) - Type conversion + casting
├── runtime_io.c        (~450 satır) - File I/O operations
├── runtime_print.c     (~300 satır) - Print/yazdir + debug
├── runtime_error.c     (~250 satır) - Error handling + panic
└── runtime_init.c      (~200 satır) - Initialization + cleanup

TOPLAM: ~2850 satır (gereksiz kod temizlenerek azalabilir)
```

**Her Modül İçin Belirt:**
- Hangi fonksiyonlar bu modülde?
- Tahmini satır sayısı
- Bağımlılıklar (hangi başka modüllere ihtiyaç var?)

**Deliverable:** 8 modül detaylı planı

---

### Task 3: Bağımlılık Analizi ⏳

**Hedef:** Circular dependency riski tespit et, header stratejisi belirle.

**Bağımlılık Soruları:**

1. Hangi fonksiyonlar birbirini çağırıyor?
2. Circular dependency riski var mı?
   - Örn: runtime_string.c → runtime_memory.c → runtime_string.c ❌
3. Header dosyası stratejisi:
   - `runtime_memory.h` - Public API
   - `runtime_internal.h` - Internal shared utilities
   - `runtime_types.h` - Type definitions

**Deliverable:** Bağımlılık grafiği + header stratejisi

---

### Task 4: YZ_13 İçin Tahmini Süre ⏳

**Hedef:** YZ_13'ün implementation için gereken süreyi tahmin et.

**Tahmin Kriterleri:**
- Modül sayısı: 8
- Her modül: ~0.5 gün (kod taşıma + test)
- Makefile güncelleme: 0.5 gün
- Test/Debug: 1 gün
- **TOPLAM:** ~5 gün → 3-4 güne optimize edilebilir mi?

**Deliverable:** Tahmini süre + riskler

---

### Task 5: YZ_12_ANALIZ_RAPOR.md Yaz ⏳

**Hedef:** Tüm analizleri raporla, UA_04'e sun.

**Rapor İçeriği:**

```markdown
# YZ_12 ANALIZ RAPOR - Runtime Modularization

## 📊 Özet
- Runtime.c: 3245 satır → 8 modül (~2850 satır)
- Fonksiyon sayısı: [X] fonksiyon
- Modül planı: ✅ Hazır
- Circular dependency: ❌/✅

## 📋 8 Modül Planı
[Her modül detayları]

## 🔗 Bağımlılık Analizi
[Bağımlılık grafiği]

## ⏱️ YZ_13 Tahmini Süre
- [3-4 gün]

## ⚠️ Riskler
- [Circular dependency?]
- [Test karmaşıklığı?]

## ✅ Kabul Kriterleri
- [ ] Her modül < 500 satır
- [ ] Circular dependency yok
- [ ] Makefile planı hazır
- [ ] Test stratejisi belirlendi
```

**Deliverable:** YZ_12_ANALIZ_RAPOR.md

---

## 📚 REFERANSLAR

**Dosyalar:**
- Runtime.c: `/home/pardus/projeler/MLP/MLP-GCC/MELP/runtime/runtime.c`
- Architecture doc: `/home/pardus/projeler/MLP/MLP-GCC/docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md`
- TODO_SELFHOSTING: `/home/pardus/projeler/MLP/MLP-GCC/0-TODO_SELFHOSTING.md`

**Referans Komutlar:**

```bash
# Fonksiyon sayısı
grep -c "^[a-zA-Z_].*(" runtime.c

# Fonksiyon isimleri listele
grep "^[a-zA-Z_].*(" runtime.c | head -20

# Memory ile ilgili fonksiyonlar
grep -E "(malloc|free|gc_|alloc)" runtime.c

# String ile ilgili fonksiyonlar
grep -E "(string|str_|concat|slice)" runtime.c
```

**6 Temel Esas (Koru!):**
1. **MODULAR** → ✅ DÜZELTİLİYOR!
2. **GCC** → Korunacak
3. **STO** → Korunacak
4. **STATELESS** → Kontrol et
5. **STRUCT+FUNC** → Korunacak
6. **MODÜL=ŞABLON** → Uygulanacak

---

## 🚫 YAPAMAZSIN

❌ Kod yazma - Sadece analiz yap!  
❌ Implementation - Bu YZ_13'ün görevi!  
❌ Gereksiz context yükleme - Sadece ihtiyacın olanı oku!

---

## ✅ BAŞARI KRİTERLERİ

TODO #4 (Analiz) bittiğinde:

- [ ] YZ_12_ANALIZ_RAPOR.md tamamlandı
- [ ] 8 modül planı oluşturuldu (her biri < 500 satır)
- [ ] Bağımlılık analizi yapıldı (circular dependency yok)
- [ ] YZ_13 için tahmini süre belirlendi
- [ ] UA_04'e rapor edildi

---

## 📌 İLK ADIM

**ŞİMDİ YAP:**

1. Terminal'de runtime.c'yi analiz et:
   ```bash
   cd /home/pardus/projeler/MLP/MLP-GCC/MELP/runtime
   wc -l runtime.c
   grep -c "^[a-zA-Z_].*(" runtime.c
   ```

2. İlk 50 fonksiyonu listele:
   ```bash
   grep "^[a-zA-Z_].*(" runtime.c | head -50
   ```

3. Task 1'e başla: Fonksiyon kategorileri belirle!

---

**İYİ ÇALIŞMALAR! UA_04 seni takip ediyor.** 🚀

---

*Oluşturuldu: 30 Aralık 2025, 12:45*
*Yönetici: UA_04*
