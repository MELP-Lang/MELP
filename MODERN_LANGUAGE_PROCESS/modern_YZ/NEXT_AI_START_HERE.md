# MODERN LANGUAGE DEVELOPMENT - BURADAN BAŞLA

**Son Güncelleme:** 23 Aralık 2025  
**Ana TODO:** `TODO_MODERN_FINAL.md`  
**Kurallar:** `TODO_kurallari.md`  
**Durum:** 🔵 Başlamaya hazır!

---

## 🚨 GÜNCEL DURUM (23 Aralık 2025)

**🎉 modern_YZ_01 TAMAMLANDI! File I/O Infrastructure Eklendi!**

**Phase 0, Task 0.1a TAMAMLANDI - File I/O Infrastructure:**
- ✅ mlp_read_file() ve mlp_write_file() LLVM backend'e eklendi
- ✅ String variable handling düzeltildi (_ptr suffix)
- ✅ File I/O test başarılı (exit code 42)
- ✅ Native execution çalışıyor
- ✅ Runtime stdlib rebuild edildi

**Phase 0, Task 0.1b ERTELENDİ - Real Bootstrap:**
- ⏳ String concatenation gerekli (Phase 1, Task 1.1'de)
- ⏳ Gen1 → Gen2 → Gen3 convergence (string ops sonrası)

**Tamamlanan (Self-hosting + File I/O):**
- ✅ Function definitions
- ✅ Numeric type (basic)
- ✅ Return statements
- ✅ Arithmetic (+, -, *, /)
- ✅ Comparison (>, <, ==)
- ✅ If-then-end_if
- ✅ LLVM IR backend
- ✅ Self-compilation (symbolic)
- ✅ **File I/O (read_file, write_file)** 🆕

**Eksikler (yapılacak):**
- ❌ Variables (let, assignments)
- ❌ Loops (while, for)
- ❌ **String type + concat** ← Task 0.1b için kritik!
- ❌ Arrays
- ❌ Structs
- ❌ FFI
- ❌ Module system
- ❌ Error handling

---

## 🎯 SONRAKİ YZ: modern_YZ_02

### Görev: Phase 0, Task 0.2 - Variables Support

### Görev: Phase 0, Task 0.2 - Variables Support

**Ne yapacaksın:**

Variable declaration ve assignment ekleyeceksin.

**Spesifik adımlar (TODO'dan):**

1. **Lexer: '=' tokenize et**
   - Assignment operator için token support

2. **Parser: var_decl ve assignment parse et**
   ```melp
   numeric x = 10      -- declaration + initialization
   x = x + 5           -- assignment
   ```

3. **CodeGen: alloca, store, load instructions**
   - Variable allocation
   - Value storage
   - Value loading

4. **Test: Basit variable programı derle ve çalıştır**
   ```melp
   function test() returns numeric
       numeric x = 10
       x = x + 5
       return x  -- 15 döner
   end_function
   ```

**Başarı kriterleri:**
```
[ ] numeric x = 10 çalışıyor
[ ] x = x + 5 çalışıyor
[ ] Test exit code 15
[ ] TODO dışı iş yapılmadı
```

**YASAK:**
```
❌ String variables (sadece numeric)
❌ Arrays (sonra gelecek)
❌ Multiple types (sadece numeric)
❌ TODO dışı özellik ekleme
```

**Tahmini süre:** 2 gün

**Not:** Stage 0 zaten variable support var! Minimal compiler'a eklemen gerekiyor.

**Tahmini süre:** 2 gün

---

## 📋 ZORUNLU OKUMALAR (modern_YZ_02 İçin)

1. **TODO_MODERN_FINAL.md** → Phase 0, Task 0.2'yi oku (SADECE!)
2. **TODO_kurallari.md** → YZ kurallarını oku (TAMAMI!)
3. **modern_YZ/modern_YZ_01_TAMAMLANDI.md** → Önceki YZ'nin yaptıkları
4. **/pmlp_kesin_sozdizimi.md** → MELP syntax
5. **/MELP_VISION.md** → 5 prensip

**UYARI:** TODO'nun tamamını okuma! Sadece Task 0.2!

---

## 🚫 KRİTİK UYARILAR (modern_YZ_02 İçin)

### ❌ YAPMA:
- "Variables eklerken arrays de ekleyeyim"
- "String variables da ekleyeyim" (YASAK! Sadece numeric)
- "Global variables da ekleyeyim" (YASAK! Sadece local)
- "Task 0.3'ü de yapayım" (YASAK! Sadece Task 0.2)
- "Detaylandırmamı ister misin?" deme (pasif kalma!)

### ✅ YAP:
- TODO'daki spesifik adımları takip et
- YASAK listesine %100 uy
- Sadece numeric, sadece local variables
- Başarı kriterlerini tamamla
- Test et, rapor yaz, push et

## 📊 ROADMAP ÖZET

```
Phase 0: Self-hosting improvements (1 hafta) ← ŞU AN BURADAYIZ
├── 0.1a: File I/O infrastructure (1 gün) ← ✅ TAMAMLANDI (modern_YZ_01)
├── 0.1b: Real bootstrap (ERTELENDI → Task 1.1'den sonra)
├── 0.2: Variables (2 gün) ← SONRAKİ TASK! (modern_YZ_02)
├── 0.3: While loops (1.5 gün)
└── 0.4: Finalization (0.5 gün)
├── 0.3: While loops (1.5 gün)
└── 0.4: Finalization (0.5 gün)

Phase 1: Core features (2 hafta)
├── 1.1: String type (3 gün)
├── 1.2: For loops (2 gün)
├── 1.3: Arrays (4 gün)
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
# modern_YZ_02 için:
git checkout -b modern_YZ_02
# Task 0.2'yi yap
git add -A
git commit -m "modern_YZ_02: Variables support (declaration + assignment)"
git push origin modern_YZ_02

# Merge ETME! Pull request AÇMA!
```

**Kural:** Her YZ kendi branch'inde çalışır, merge edilmez.
**Kural:** Her YZ kendi branch'inde çalışır, merge edilmez.

---

## 📝 RAPOR FORMATI

```markdown
# modern_YZ/modern_YZ_02_TAMAMLANDI.md

## TODO'daki Task:
Phase 0, Task 0.2: Variables Support
[TODO'dan AYNEN kopyala]

## Yaptığım İş:
1. Lexer: '=' token kontrolü
2. Parser: var_decl ve assignment parse
3. CodeGen: alloca, store, load instructions
4. Test: variable program derle ve çalıştır

## Başarı Kriterleri:
[✅] numeric x = 10 çalışıyor
[✅] x = x + 5 çalışıyor
[✅] Test exit code 15
[✅] TODO dışı iş yapılmadı

## TODO Dışı İş Yapıldı mı?
❌ Hayır! Sadece numeric local variables eklendi.

## Test Sonuçları:
[Output...]

## Dosyalar:
- [Değiştirilen dosyalar]

## Sonraki YZ İçin:
Task 0.3 (While loops) için hazır. Variables tamamen çalışıyor.
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

## 💡 modern_YZ_01 İÇİN SON HATIRLATMALAR

**Yapacaksın:**
## 💡 modern_YZ_02 İÇİN SON HATIRLATMALAR

**Yapacaksın:**
1. TODO_MODERN_FINAL.md'de Task 0.2'yi oku
2. TODO_kurallari.md'yi oku
3. modern_YZ_01_TAMAMLANDI.md'yi oku
4. Kendini tanıt (format uygun!)
5. Onay al
6. Spesifik adımları takip et
7. YASAK listesine uy (sadece numeric, sadece local!)
8. Test et (başarı kriterleri)
9. Rapor yaz
10. NEXT_AI güncelle
11. Push et

**Yapmayacaksın:**
❌ TODO'dan sapma
❌ String/array variables (YASAK!)
❌ Global variables (YASAK!)
❌ Ekstra özellik
❌ Pasif kalma
❌ Validation skip

**Hazır Altyapı:**
- ✅ File I/O çalışıyor (modern_YZ_01'den)
- ✅ LLVM backend hazır
- ✅ Runtime güncel

---

**Son Güncelleme:** 23 Aralık 2025 (modern_YZ_01 tamamlandı)  
**Durum:** ✅ modern_YZ_02 başlayabilir!  
**Branch:** `modern_YZ_02` (oluşturulacak)

---

🚀 **MODERN MELP JOURNEY DEVAM EDİYOR!** 🚀