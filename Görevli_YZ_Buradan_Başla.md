# 🤖 SEN: STDLIB_YZ_05

---

## 🎯 GÖREVİN

**TODO:** #2 - STDLIB_EXPAND  
**Task:** 6 - Time/Date Library  
**Süre:** 3 gün

---

## 📋 İŞE BAŞLAMADAN ÖNCE

1. **TODO_KURALLARI.md** dosyasını oku ← ZORUNLU!
2. **2-TODO_STDLIB_EXPAND.md** dosyasına git ← Task 6 detayları orada

**Not:** Task 5 (File I/O Complete) YZ_04 tarafından tamamlandı. Sen Task 6'dan başla!

---

## 📖 ÖNCEKİ YZ'DEN KALAN

**Önceki YZ:** YZ_04 (STDLIB_YZ_04)  
**Tamamladığı:** Task 5 - File I/O Complete  
**Tarih:** 1 Ocak 2026

### 🎯 NEREDE KALDIK?

**Tamamlanan Görevler:**
- [x] Task 1 - Networking Library (YZ_01) ✓
- [x] Task 2 - Threading & Concurrency (YZ_02) ✓
- [x] Task 3 - Advanced Collections (YZ_01) ✓
- [x] Task 4 - JSON Parsing (YZ_03) ✓
- [x] Task 5 - File I/O Complete (YZ_04) ✓

**Devam Edilecek:**
- [ ] Task 6 - Time/Date Library ← **SEN BURADASIN!**
- [ ] Task 7 - Regex Support

### 📚 PROJE HAKKINDA (Yeni Gelenler İçin)

**Bu proje ne?**
MELP-GCC: Multi-Language Programming dilinin derleyicisi. Kullanıcı istediği dilde (Türkçe, İngilizce, etc.) ve istediği sözdiziminde (C-style, Python-style, etc.) kod yazar, MELP bunu x86-64 assembly'ye derler.

**Şu ana kadar ne yaptık?**
1. Task 1: TCP/UDP sockets, HTTP client ✅
2. Task 2: Thread, mutex, channel modules ✅
3. Task 3: HashMap, Set, BTree collections ✅
4. Task 4: JSON parser/serializer (module metadata!) ✅
5. Task 5: File I/O complete + path utilities (module loading!) ✅

**Ne yapmamız gerekiyor?**
**TODO #2 Task 6:** Time/Date library (future use)
- Clock operations (now, timestamp)
- Date formatting and parsing
- Timezone handling

**Nasıl yapmalısın?**
1. `stdlib/time/time.mlp` oluştur (~300 satır)
2. `MELP/runtime/time/` C implementation
3. **⚠️ ÖNEMLİ:** 6. TEMEL ESAS (MODÜL=ŞABLON) prensibi:
   - ❌ Global time state YASAK
   - ✅ Functional pattern: `time.now()` → query → return value
4. 10+ test yaz ve çalıştır
5. Rapor yaz: `TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_05_RAPOR.md`

**Önemli Notlar:**
- YZ_01 raporu: [YZ_01_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_01_RAPOR.md)
- YZ_02 raporu: [YZ_02_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_02_RAPOR.md)
- YZ_03 raporu: [YZ_03_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_03_RAPOR.md)
- YZ_04 raporu: [YZ_04_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_04_RAPOR.md)
- Test örnekleri: [tests/json/](tests/json/), [tests/thread/](tests/thread/), [tests/io/](tests/io/)
- Terminoloji: "API" değil "ŞABLON" kullan!

---

**Tarih:** 1 Ocak 2026  
**Durum:** 🟢 AKTİF
