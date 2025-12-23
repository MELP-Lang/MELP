# MODERN LANGUAGE DEVELOPMENT - BURADAN BAŞLA

**Son Güncelleme:** 23 Aralık 2025  
**Ana TODO:** `TODO_MODERN_FINAL.md`  
**Kurallar:** `TODO_kurallari.md`  
**Durum:** 🔵 Başlamaya hazır!

---

## 🚨 GÜNCEL DURUM (23 Aralık 2025)

**🎉 modern_YZ_02 TAMAMLANDI! Variables Support Eklendi!**

**Phase 0, Task 0.2 TAMAMLANDI - Variables Support:**
- ✅ Variable declaration çalışıyor (numeric x = 10)
- ✅ Variable assignment çalışıyor (x = x + 5)
- ✅ LLVM IR: alloca, store, load instructions
- ✅ Test programı başarılı (exit code 15)
- ✅ Stage 0 variable module validation

**Phase 0, Task 0.1a TAMAMLANDI - File I/O Infrastructure:**
- ✅ mlp_read_file() ve mlp_write_file() LLVM backend'e eklendi
- ✅ String variable handling düzeltildi (_ptr suffix)
- ✅ File I/O test başarılı (exit code 42)
- ✅ Native execution çalışıyor
- ✅ Runtime stdlib rebuild edildi

**Phase 0, Task 0.1b ERTELENDİ - Real Bootstrap:**
- ⏳ String concatenation gerekli (Phase 1, Task 1.1'de)
- ⏳ Gen1 → Gen2 → Gen3 convergence (string ops sonrası)

**Tamamlanan (Self-hosting + File I/O + Variables):**
- ✅ Function definitions
- ✅ Numeric type (basic)
- ✅ Return statements
- ✅ Arithmetic (+, -, *, /)
- ✅ Comparison (>, <, ==)
- ✅ If-then-end_if
- ✅ LLVM IR backend
- ✅ Self-compilation (symbolic)
- ✅ **File I/O (read_file, write_file)**
- ✅ **Variables (declaration + assignment)** 🆕

**Eksikler (yapılacak):**
- ❌ Loops (while, for)
- ❌ **String type + concat** ← Task 0.1b için kritik!
- ❌ Arrays
- ❌ Structs
- ❌ FFI
- ❌ Module system
- ❌ Error handling

---

## 🎯 SONRAKİ YZ: modern_YZ_03

### Görev: Phase 0, Task 0.3 - While Loops

**Ne yapacaksın:**

While döngüsü ekleyeceksin.

**Spesifik adımlar (TODO'dan):**

1. **Parser: while-do-end_while parse et**
   ```melp
   while i < 10 do
       i = i + 1
   end_while
   ```

2. **CodeGen: br, phi, basic blocks**
   - Loop header block
   - Loop body block
   - Loop exit block
   - Branch instructions

3. **Test: 1'den 10'a kadar toplama**
   ```melp
   function count() returns numeric
       numeric i = 0
       while i < 10 do
           i = i + 1
       end_while
       return i  -- 10 döner
   end_function
   ```

**Başarı kriterleri:**
```
[ ] while-do-end_while çalışıyor
[ ] Loop condition değerlendiriliyor
[ ] Loop body tekrarlanıyor
[ ] Test exit code 55 (1+2+...+10)
```

**YASAK:**
```
❌ For loop (sonra gelecek)
❌ Break/continue (sonra gelecek)
❌ TODO dışı özellik ekleme
```

**Tahmini süre:** 1.5 gün

**Not:** Stage 0'da control_flow modülü var! While loop support kontrol et.

---

## 📋 ZORUNLU OKUMALAR (modern_YZ_03 İçin)

1. **TODO_MODERN_FINAL.md** → Phase 0, Task 0.3'ü oku (SADECE!)
2. **TODO_kurallari.md** → YZ kurallarını oku (TAMAMI!)
3. **modern_YZ/modern_YZ_02_TAMAMLANDI.md** → Önceki YZ'nin yaptıkları
4. **/pmlp_kesin_sozdizimi.md** → MELP syntax (while loops)
5. **/MELP_VISION.md** → 5 prensip

**UYARI:** TODO'nun tamamını okuma! Sadece Task 0.3!

---

## 🚫 KRİTİK UYARILAR (modern_YZ_03 İçin)

### ❌ YAPMA:
- "While eklerken for loop da ekleyeyim"
- "Break/continue da ekleyeyim" (YASAK!)
- "Task 0.4'ü de yapayım" (YASAK! Sadece Task 0.3)
- "Detaylandırmamı ister misin?" deme (pasif kalma!)

### ✅ YAP:
- TODO'daki spesifik adımları takip et
- YASAK listesine %100 uy
- Sadece while loops (do-end_while syntax)
- Başarı kriterlerini tamamla
- Test et, rapor yaz, push et

## 📊 ROADMAP ÖZET

```
Phase 0: Self-hosting improvements (1 hafta) ← ŞU AN BURADAYIZ
├── 0.1a: File I/O infrastructure (1 gün) ← ✅ TAMAMLANDI (modern_YZ_01)
├── 0.1b: Real bootstrap (ERTELENDI → Task 1.1'den sonra)
├── 0.2: Variables (2 gün) ← ✅ TAMAMLANDI (modern_YZ_02)
├── 0.3: While loops (1.5 gün) ← SONRAKİ TASK! (modern_YZ_03)
└── 0.4: Finalization (0.5 gün)

Phase 1: Core features (2 hafta)
├── 1.1: String type (3 gün)
├── 1.2: For loops (2 gün)
├── 1.3: Arrays (4 gün)
└── 1.4: Structs (3 gün)

Phase 2-7: FFI, Modules, Error handling, Advanced, Tooling, Finalization
```

**Toplam:** 8-10 hafta, 20+ task
└── 1.4: Structs (3 gün)

Phase 2-7: FFI, Modules, Error handling, Advanced, Tooling, Finalization
```

**Toplam:** 8-10 hafta, 20+ task

---

## 🎯 HEDEF

**Modern MELP özellikleri:**
- ✅ Variables, loops, strings, arrays
- ✅ Structs, FFI, modules
- ✅ Error handling, generics, closures
- ✅ Build system, package manager, LSP
- ✅ v1.0.0 release!

**Süre:** 8-10 hafta (verimli, net, kurallara uygun!)

---
## 🔧 GIT WORKFLOW

```bash
# modern_YZ_03 için:
git checkout -b modern_YZ_03
# Task 0.3'ü yap
git add -A
git commit -m "modern_YZ_03: While loops support (while-do-end_while)"
git push origin modern_YZ_03

# Merge ETME! Pull request AÇMA!
```

**Kural:** Her YZ kendi branch'inde çalışır, merge edilmez.

---

## 📝 RAPOR FORMATI

```markdown
# modern_YZ/modern_YZ_03_TAMAMLANDI.md

## TODO'daki Task:
Phase 0, Task 0.3: While Loops
[TODO'dan AYNEN kopyala]

## Yaptığım İş:
1. Parser: while-do-end_while syntax
2. CodeGen: br, phi, basic blocks
3. Test: 1'den 10'a kadar toplama

## Başarı Kriterleri:
[✅] while-do-end_while çalışıyor
[✅] Loop condition değerlendiriliyor
[✅] Loop body tekrarlanıyor
[✅] Test exit code 55
[✅] TODO dışı iş yapılmadı

## TODO Dışı İş Yapıldı mı?
❌ Hayır! Sadece while loops eklendi.

## Test Sonuçları:
[Output...]

## Dosyalar:
- [Değiştirilen dosyalar]

## Sonraki YZ İçin:
Task 0.4 (Finalization) için hazır. While loops tamamen çalışıyor.
```

---

## 🎓 DERS: Self-hosting'den Öğrenilenler

**500 session'da neler yanlış gitti:**
- YZ'ler TODO'dan saptı ("Phase 3.4" icat ettiler)
- Ekstra özellik eklediler (TODO'da yoktu)
- Pasif kaldılar ("Detaylandırmamı ister misin?")
- Başarı kriterlerini skip ettiler

**Bu sefer nasıl olacak:**
- ✅ Sıkı TODO takip
- ✅ YASAK listesine uyma
- ✅ Başarı kriterlerini zorunlu kılma
- ✅ Aktif, net, hızlı çalışma

**Hedef:** 8-10 hafta, verimli! (500 session değil!) 🚀

---

## 💡 modern_YZ_03 İÇİN SON HATIRLATMALAR

**Yapacaksın:**
1. TODO_MODERN_FINAL.md'de Task 0.3'ü oku
2. TODO_kurallari.md'yi oku
3. modern_YZ_02_TAMAMLANDI.md'yi oku
4. Kendini tanıt (format uygun!)
5. Onay al
6. Spesifik adımları takip et
7. YASAK listesine uy (sadece while, for/break/continue yok!)
8. Test et (başarı kriterleri - exit code 55)
9. Rapor yaz
10. NEXT_AI güncelle
11. Push et

**Yapmayacaksın:**
❌ TODO'dan sapma
❌ For loop ekleme (YASAK!)
❌ Break/continue ekleme (YASAK!)
❌ Ekstra özellik
❌ Pasif kalma
❌ Validation skip

**Hazır Altyapı:**
- ✅ File I/O çalışıyor (modern_YZ_01'den)
- ✅ Variables çalışıyor (modern_YZ_02'den)
- ✅ LLVM backend hazır
- ✅ Runtime güncel

---

**Son Güncelleme:** 23 Aralık 2025 (modern_YZ_02 tamamlandı)  
**Durum:** ✅ modern_YZ_03 başlayabilir!  
**Branch:** `modern_YZ_03` (oluşturulacak)

---

🚀 **MODERN MELP JOURNEY DEVAM EDİYOR!** 🚀