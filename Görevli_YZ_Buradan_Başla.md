# 🤖 SEN: STDLIB_YZ_04

---

## 🎯 GÖREVİN

**TODO:** #2 - STDLIB_EXPAND  
**Task:** 5 - File I/O Complete  
**Süre:** 3 gün

---

## 📋 İŞE BAŞLAMADAN ÖNCE

1. **TODO_KURALLARI.md** dosyasını oku ← ZORUNLU!
2. **2-TODO_STDLIB_EXPAND.md** dosyasına git ← Task 5 detayları orada

**Not:** Task 4 (JSON Parsing) YZ_03 tarafından tamamlandı. Sen Task 5'ten başla!

---

## 📖 ÖNCEKİ YZ'DEN KALAN

**Önceki YZ:** YZ_03 (STDLIB_YZ_03)  
**Tamamladığı:** Task 4 - JSON Parsing  
**Tarih:** 1 Ocak 2026

### 🎯 NEREDE KALDIK?

**Tamamlanan Görevler:**
- [x] Task 1 - Networking Library (YZ_01) ✓
- [x] Task 2 - Threading & Concurrency (YZ_02) ✓
- [x] Task 3 - Advanced Collections (YZ_01) ✓
- [x] Task 4 - JSON Parsing (YZ_03) ✓

**Devam Edilecek:**
- [ ] Task 5 - File I/O Complete ← **SEN BURADASIN!**
- [ ] Task 6 - Time/Date Library
- [ ] Task 7 - Regex Support

### 📚 PROJE HAKKINDA (Yeni Gelenler İçin)

**Bu proje ne?**
MELP-GCC: Multi-Language Programming dilinin derleyicisi. Kullanıcı istediği dilde (Türkçe, İngilizce, etc.) ve istediği sözdiziminde (C-style, Python-style, etc.) kod yazar, MELP bunu x86-64 assembly'ye derler.

**Şu ana kadar ne yaptık?**
1. Task 1: TCP/UDP sockets, HTTP client ✅
2. Task 2: Thread, mutex, channel modules ✅
3. Task 3: HashMap, Set, BTree collections ✅
4. Task 4: JSON parser/serializer (module metadata!) ✅

**Ne yapmamız gerekiyor?**
**TODO #2 Task 5:** File I/O genişletme - **Modül yükleme için ŞART!**
- Path utilities (join, normalize, absolute)
- Directory listing (module discovery)
- File metadata (cache invalidation)

**Neden Kritik?**
Stage2 import sistemi modülleri dosya sisteminden yükleyecek. Path operations ve file listing olmadan modül keşfetme çalışmaz!

**Nasıl yapmalısın?**
1. `stdlib/io/file.mlp` genişlet (~550 satır)
2. `stdlib/io/path.mlp` yeni oluştur (~200 satır)
3. `MELP/runtime/io/path.c` C implementation
4. **⚠️ ÖNEMLİ:** 6. TEMEL ESAS (MODÜL=ŞABLON) prensibi:
   - ❌ Global file handles YASAK
   - ✅ Functional pattern: `file.read(path)` → born → read → died
5. 10+ test yaz ve çalıştır
6. Rapor yaz: `TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_04_RAPOR.md`

**Önemli Notlar:**
- YZ_01 raporu: [YZ_01_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_01_RAPOR.md)
- YZ_02 raporu: [YZ_02_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_02_RAPOR.md)
- YZ_03 raporu: [YZ_03_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_03_RAPOR.md)
- Test örnekleri: [tests/json/](tests/json/), [tests/thread/](tests/thread/)
- Terminoloji: "API" değil "ŞABLON" kullan!

---

**Tarih:** 1 Ocak 2026  
**Durum:** 🟢 AKTİF
