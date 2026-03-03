# 🧠 MASTERMIND TALİMATI: PARADIGM2 POC BAŞLATMA

**Tarih:** 24 Ocak 2026  
**Hedef:** Paradigm2 POC (3 gün)  
**Durum:** ⏳ BAŞLATILIYOR

---

## 🎯 ÜST AKIL'A TALİMAT

**Görev:** Paradigm2 POC'yi yönet ve tamamlat

**POC Kapsamı:**
- Common infrastructure (interfaces)
- 5 module: variable, if_statement, while_loop, function_call, binary_ops
- Coordinator (module loader + dispatcher)
- Test: hello_world.pmpl, fibonacci.pmpl

**Timeline:** 3 gün (27 saat)

**Detaylı Plan:** [POC_tests/POC_Paradigm2/TODO.md](../POC_tests/POC_Paradigm2/TODO.md)

---

## 📊 STRATEJİK PRENSLER (İhlal Etme!)

### 1. WORKAROUND YASAK
```
❌ "Şimdilik çalışsın" kodu
✅ %100 gerçek implementasyon
```

### 2. MODÜLER ESAS
```
❌ Module'ler birbirine bağımlı
✅ Her module standalone
```

### 3. Objectivity
```
❌ Bias'lı değerlendirme
✅ Data-driven karşılaştırma
```

---

## 📋 ÜA'NIN SORUMLULUKLARI

### Phase 1: YZ Ataması
- [ ] YZ_POC_01 assign et
- [ ] Görev dokümantı ver: [POC_tests/POC_Paradigm2/TODO.md](../POC_tests/POC_Paradigm2/TODO.md)
- [ ] Timeline izle: 3 gün deadline

### Phase 2: Progress Tracking
- [ ] Günlük checkpoint (her gün sonu)
- [ ] Blocker'ları MM'e escalate et
- [ ] Quality check (WORKAROUND kontrolü)

### Phase 3: Validation
- [ ] Gün 3 sonu: hello_world.pmpl çalışıyor mu?
- [ ] fibonacci.pmpl → 55 çıktısı veriyor mu?
- [ ] Module independence test: if_statement/ kopyalanabilir mi?

---

## 🚨 ESCALATION KURALLARI

**ÜA, şu durumlarda MM'e sor:**

1. **Timeline kayıyor**
   - Gün 2 sonu %50'den azsa
   - YZ overwhelmed hissediyorsa

2. **Technical blocker**
   - Module coordination çalışmıyorsa
   - Parse dispatcher ambiguity varsa
   - Codegen orchestration problemi varsa

3. **Quality concern**
   - WORKAROUND görürsen
   - Module dependency tespit edersen
   - "Şimdilik çalışıyor" kodu görürsen

---

## 📊 SUCCESS METRICS

### Gün 1 Sonu (9 saat)
- [ ] Common infrastructure compile ediyor
- [ ] Variable module: "numeric x = 42" parse ediliyor
- [ ] Binary ops module: "x + y" parse ediliyor
- [ ] 2 library: libcommon.a, libvar_module.a

### Gün 2 Sonu (18 saat)
- [ ] If module: "if x > 0 ... end_if" parse ediliyor
- [ ] While module: "while x > 0 ... end_while" parse ediliyor
- [ ] Function call module: "print(x)" parse ediliyor
- [ ] 5 library toplam

### Gün 3 Sonu (27 saat)
- [ ] Module loader çalışıyor
- [ ] Parse dispatcher route ediyor
- [ ] hello_world.pmpl → "Hello, World!" çıktısı
- [ ] fibonacci.pmpl → 55 çıktısı
- [ ] POC COMPLETE ✅

---

## 📅 CHECKPOINT SCHEDULE

**Daily Standup (her gün 18:00):**
- ✅ Bugün ne tamamlandı?
- ⏳ Şu anda nerede takılıyız?
- 📋 Yarın ne yapılacak?
- 🚨 MM'e escalate edilecek blocker var mı?

**Günlük Rapor Format:**
```
Gün X: [Tamamlanan / Hedeflenen]
✅ Tamamlananlar: (liste)
⏳ Devam edenler: (liste)
🚨 Blocker'lar: (liste veya YOK)
📊 Timeline: On track / Kayıyor (+X saat)
```

---

## 🎯 ÜA, ŞUNU YAP:

1. **YZ_POC_01'i ata**
   - POC_Paradigm2/TODO.md'yi ver
   - 3 gün deadline'ı açıkla
   - Quality standartları aktar

2. **İlk görev başlat**
   - common/token_base.hpp (30 dakika)
   - Hemen test: compile ediyor mu?

3. **MM'e ilk checkpoint ver**
   - 2 saat sonra: İlk dosya tamamlandı mı?
   - Blocker var mı?

---

## 📞 İLETİŞİM

**MM → ÜA:** Bu talimat
**ÜA → MM:** Günlük checkpoint (18:00)
**ÜA → YZ:** Operasyonel talimatlar
**YZ → ÜA:** Progress update

---

**BAŞLA: ÜA, YZ_POC_01'i ata ve Paradigm2 POC'yi başlat!** 🚀
