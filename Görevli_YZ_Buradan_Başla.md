# 🤖 SEN: STDLIB_YZ_03

---

## 🎯 GÖREVİN

**TODO:** #2 - STDLIB_EXPAND  
**Task:** 4 - JSON Parsing  
**Süre:** 5 gün

---

## 📋 İŞE BAŞLAMADAN ÖNCE

1. **TODO_KURALLARI.md** dosyasını oku ← ZORUNLU!
2. **2-TODO_STDLIB_EXPAND.md** dosyasına git ← Task 4 detayları orada

**Not:** Task 3 (Collections) YZ_01 tarafından tamamlandı. Sen Task 4'ten başla!

---

## 📖 ÖNCEKİ YZ'DEN KALAN

**Önceki YZ:** YZ_02 (STDLIB_YZ_02)  
**Tamamladığı:** Task 2 - Threading & Concurrency  
**Tarih:** 1 Ocak 2026

### 🎯 NEREDE KALDIK?

**Tamamlanan Görevler:**
- [x] Task 1 - Networking Library (YZ_01) ✓
- [x] Task 2 - Threading & Concurrency (YZ_02) ✓
- [x] Task 3 - Advanced Collections (YZ_01) ✓

**Devam Edilecek:**
- [ ] Task 4 - JSON Parsing ← **SEN BURADASIN!**
- [ ] Task 5 - File I/O Complete
- [ ] Task 6 - Time/Date Library
- [ ] Task 7 - Regex Support

### 📚 PROJE HAKKINDA (Yeni Gelenler İçin)

**Bu proje ne?**
MELP-GCC: Multi-Language Programming dilinin derleyicisi. Kullanıcı istediği dilde (Türkçe, İngilizce, etc.) ve istediği sözdiziminde (C-style, Python-style, etc.) kod yazar, MELP bunu x86-64 assembly'ye derler.

**Şu ana kadar ne yaptık?**
1. Task 1: TCP/UDP sockets, HTTP client ✅
2. Task 2: Thread, mutex, channel modules ✅
3. Task 3: HashMap, Set, BTree collections ✅

**Ne yapmamız gerekiyor?**
**TODO #2 Task 4:** JSON parser/serializer implementasyonu
- Module metadata için kritik (import system ihtiyacı!)
- Parse: JSON string → MLP objects
- Stringify: MLP objects → JSON string

**Neden Kritik?**
Stage2 import sistemi module metadata'yı JSON formatında saklayacak. JSON parser olmadan modül sistemi çalışmaz!

**Nasıl yapmalısın?**
1. `stdlib/json/json.mlp` modül şablonu oluştur (~400 satır)
2. `MELP/runtime/json/parser.c` C implementation (recursive descent parser)
3. **⚠️ ÖNEMLİ:** 6. TEMEL ESAS (MODÜL=ŞABLON) prensibi:
   - ❌ Persistent state YASAK
   - ✅ Functional pattern: `json.parse(text)` → born → parsed → died
4. 12+ test yaz ve çalıştır
5. Rapor yaz: `TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_03_RAPOR.md`

**Önemli Notlar:**
- YZ_01 raporu: [YZ_01_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_01_RAPOR.md)
- YZ_02 raporu: [YZ_02_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_02_RAPOR.md)
- Test örnekleri: [tests/thread/](tests/thread/), [tests/net/](tests/net/)
- Terminoloji: "API" değil "ŞABLON" kullan!

---

**Tarih:** 1 Ocak 2026  
**Durum:** 🟢 AKTİF
