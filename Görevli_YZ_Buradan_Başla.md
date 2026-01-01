# 🤖 SEN: STDLIB_YZ_06

---

## 🎯 GÖREVİN

**TODO:** #2 - STDLIB_EXPAND  
**Task:** 7 - Regex Support (OPSIYONEL) VEYA TODO #3'e geç  
**Süre:** 2-3 gün (eğer Regex yapılacaksa)

---

## 📋 İŞE BAŞLAMADAN ÖNCE

1. **TODO_KURALLARI.md** dosyasını oku ← ZORUNLU!
2. **2-TODO_STDLIB_EXPAND.md** dosyasına git ← Task 7 detayları orada
3. **YZ_05_RAPOR.md**'yi oku ← Time/Date library nasıl yapıldı?

**Not:** Task 6 (Time/Date Library) YZ_05 tarafından tamamlandı. TODO #2 neredeyse bitti!

---

## 📖 ÖNCEKİ YZ'DEN KALAN

**Önceki YZ:** YZ_05 (STDLIB_YZ_05)  
**Tamamladığı:** Task 6 - Time/Date Library  
**Tarih:** 1 Ocak 2026

### 🎯 NEREDE KALDIK?

**Tamamlanan Görevler:**
- [x] Task 1 - Networking Library (YZ_01) ✓
- [x] Task 2 - Threading & Concurrency (YZ_02) ✓
- [x] Task 3 - Advanced Collections (YZ_01) ✓
- [x] Task 4 - JSON Parsing (YZ_03) ✓
- [x] Task 5 - File I/O Complete (YZ_04) ✓
- [x] Task 6 - Time/Date Library (YZ_05) ✓

**Devam Edilecek:**
- [ ] Task 7 - Regex Support ← **OPSIYONEL! (self-hosting için gerekli değil)**

### 📚 PROJE HAKKINDA (Yeni Gelenler İçin)

**Bu proje ne?**
MELP-GCC: Multi-Language Programming dilinin derleyicisi. Kullanıcı istediği dilde (Türkçe, İngilizce, etc.) ve istediği sözdiziminde (C-style, Python-style, etc.) kod yazar, MELP bunu x86-64 assembly'ye derler.

**Şu ana kadar ne yaptık?**
1. Task 1: TCP/UDP sockets, HTTP client ✅
2. Task 2: Thread, mutex, channel modules ✅
3. Task 3: HashMap, Set, BTree collections ✅
4. Task 4: JSON parser/serializer (module metadata!) ✅
5. Task 5: File I/O complete + path utilities (module loading!) ✅
6. Task 6: Time/Date library (clock, format, parse, timezone) ✅

**Ne yapmamız gerekiyor?**

**SEÇ BİR TANESINI:**

**Seçenek A: Task 7 - Regex (OPSIYONEL)**
- Pattern matching with POSIX regex
- Match, find_all, replace operations
- ~350 satır stdlib/regex/regex.mlp
- ~500 satır MELP/runtime/regex/
- 8+ tests

**Seçenek B: TODO #3'e geç (ÖNERİLEN!)**
- TODO #2 kritik kısmı tamamlandı
- Regex self-hosting için şart değil
- TODO #3 (LANGUAGE_FEATURES) daha öncelikli
- Generics, pattern matching, advanced features

**Nasıl yapmalısın? (Eğer Regex seçersen)**
1. `stdlib/regex/regex.mlp` oluştur (~350 satır)
2. `MELP/runtime/regex/` C implementation (POSIX regex.h wrapper)
3. **⚠️ ÖNEMLİ:** 6. TEMEL ESAS (MODÜL=ŞABLON) prensibi:
   - ❌ Global regex state YASAK
   - ✅ Functional pattern: `regex.match(pattern, text)` → return result
4. 8+ test yaz ve çalıştır
5. Rapor yaz: `TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_06_RAPOR.md`

**Önemli Notlar:**
- YZ_01 raporu: [YZ_01_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_01_RAPOR.md)
- YZ_02 raporu: [YZ_02_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_02_RAPOR.md)
- YZ_03 raporu: [YZ_03_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_03_RAPOR.md)
- YZ_04 raporu: [YZ_04_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_04_RAPOR.md)
- YZ_05 raporu: [YZ_05_RAPOR.md](TODO_STDLIB_EXPAND/STDLIB_YZ/YZ_05_RAPOR.md) ← **YENİ!**
- Test örnekleri: [tests/time/](tests/time/)
- Terminoloji: "API" değil "ŞABLON" kullan!

---

**Tarih:** 1 Ocak 2026  
**Durum:** 🟢 AKTİF
