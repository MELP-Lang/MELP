# MELP Stage 1 — Seviye 2 Görevleri (11-15)
## ÜA (Üst Yönetim) Kontrol Listesi

---

## 📋 GENEL KONTROL

- [ ] Tüm görev dosyaları dizin yapısı doğru
- [ ] Her görev için `.mlp` dosyası var
- [ ] Dökümentasyon dosyaları tamamlandı
- [ ] RUN_TASKS_11_15.sh script'i test edildi
- [ ] SUGAR_SYNTAX.md güncel

---

## 🎯 GÖREV 11: String Interpolation — VERIFICATION

**Dosya:** `11_string_interpolation/string_interpolation.mlp`

### File Check
- [ ] Dosya var ve erişilebilir
- [ ] Sözdizimi hatası yok
- [ ] Formatting düzgün

### Feature Check
- [ ] `str(42)` — numeric literal
- [ ] `str(age)` — numeric variable
- [ ] `str(true)` — boolean true
- [ ] `str(false)` — boolean false
- [ ] Multiple `str()` concatenation
- [ ] Expression result: `str(5 + 5)`

### Compilation Test
```bash
MELP=/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler
$MELP 11_string_interpolation/string_interpolation.mlp -o /tmp/out11.ll
# [ ] Compile başarılı
# [ ] Hiçbir error yok
# [ ] LL file üretildi
```

### Link & Run Test
```bash
clang -O2 -x ir /tmp/out11.ll -o /tmp/out11.elf
# [ ] Link başarılı
# [ ] Executable üretildi

/tmp/out11.elf
# [ ] Program çalıştı
# [ ] Output doğru
```

### Expected Output
```
Değer: 42
Yaş: 25
Durum: true
Bayrak: false
x=10 y=20
Sonuç: 10
```

**Verification:** [ ] ✅ PASS / [ ] ❌ FAIL / [ ] ⚠️ PARTIAL

---

## 🎯 GÖREV 12: Nested Functions — VERIFICATION

**Dosya:** `12_nested_functions/nested_functions.mlp`

### File Check
- [ ] Dosya var ve erişilebilir
- [ ] Sözdizimi hatası yok
- [ ] 4 helper function tanımlandı

### Function Check
- [ ] `add(numeric x; numeric y)` ✅
- [ ] `multiply(numeric x; numeric y)` ✅
- [ ] `calculate(numeric a; numeric b)` ✅
- [ ] `print_with_label(string label; numeric value)` ✅
- [ ] `main()` fonksiyonu ✅

### Nested Call Check
- [ ] `calculate` fonksiyonu `add` çağırıyor
- [ ] `calculate` fonksiyonu `multiply` çağırıyor
- [ ] Parametreler doğru ayraçla (`;`)

### Compilation Test
```bash
$MELP 12_nested_functions/nested_functions.mlp -o /tmp/out12.ll
# [ ] Compile başarılı
```

### Link & Run Test
```bash
clang -O2 -x ir /tmp/out12.ll -o /tmp/out12.elf
/tmp/out12.elf
# [ ] Çalıştı
# [ ] Output doğru
```

### Expected Output
```
add(10;5): 15
multiply(3;7): 21
calculate(4;5): 45
chained: 52
```

**Verification:** [ ] ✅ PASS / [ ] ❌ FAIL / [ ] ⚠️ PARTIAL

---

## 🎯 GÖREV 13: Higher Order Functions — VERIFICATION

**Dosya:** `13_higher_order/higher_order.mlp`

### File Check
- [ ] Dosya var ve erişilebilir
- [ ] Sözdizimi hatası yok
- [ ] Fallback pattern uygulandı

### Feature Check (Expected to Fail)
- [ ] FUNCREF token'ı yok (beklenen hata)
- [ ] Fallback pattern var
- [ ] Direct function calls yazıldı

### Compilation Test
```bash
$MELP 13_higher_order/higher_order.mlp -o /tmp/out13.ll
# [ ] Compile başarılı (fallback nedeniyle)
# VEYA
# [ ] Compile error (FUNCREF eksikse beklenen)
```

### Status Note
🔴 **BİLİNEN SORUN:** FUNCREF (Phase 3) implement edilmemiş
- [ ] Sorun kaydedildi
- [ ] Workaround uygulandı
- [ ] Belgelenmiştir

**Verification:** [ ] ✅ PASS / [ ] ❌ FAIL / [ ] ⚠️ PARTIAL (expected)

---

## 🎯 GÖREV 14: Lambda — VERIFICATION

**Dosya:** `14_lambda/lambda.mlp`

### File Check
- [ ] Dosya var ve erişilebilir
- [ ] Sözdizimi hatası yok
- [ ] 6 lambda test case var

### Lambda Check
- [ ] Inline lambda: `lambda(x) -> x * 2` ✅
- [ ] Multi-param lambda: `lambda(x; y) -> x + y` ✅
- [ ] Closure (1 capture) ✅
- [ ] Closure (multiple capture) ✅
- [ ] Nested closure ✅

### Compilation Test
```bash
$MELP 14_lambda/lambda.mlp -o /tmp/out14.ll
# [ ] Compile başarılı
```

### Link & Run Test
```bash
clang -O2 -x ir /tmp/out14.ll -o /tmp/out14.elf
/tmp/out14.elf
# [ ] Çalıştı
# [ ] Closure'lar doğru çalıştı
```

### Expected Output
```
double(5) = 10
add(10;20) = 30
multiply(6;7) = 42
add_base(23) = 123
complex(4) = 17
nested(15) = 25
```

**Verification:** [ ] ✅ PASS / [ ] ❌ FAIL / [ ] ⚠️ PARTIAL

---

## 🎯 GÖREV 15: Struct — VERIFICATION

**Dosya:** `15_struct/struct.mlp`

### File Check
- [ ] Dosya var ve erişilebilir
- [ ] Sözdizimi hatası yok
- [ ] 4 struct tanımlandı

### Struct Check
- [ ] `struct Point` ✅
- [ ] `struct Color` (numeric only) ✅
- [ ] `struct Rectangle` (nested) ✅
- [ ] `struct Pixel` (deeply nested) ✅

### Method Check
- [ ] `Rectangle.area()` dışarıda tanımlandı ✅
- [ ] `Rectangle.perimeter()` dışarıda tanımlandı ✅
- [ ] `this` kullanıldı ✅

### Compilation Test
```bash
$MELP 15_struct/struct.mlp -o /tmp/out15.ll
# [ ] Compile başarılı
# [ ] String field error yok (numeric only kullanıldı)
```

### Link & Run Test
```bash
clang -O2 -x ir /tmp/out15.ll -o /tmp/out15.elf
/tmp/out15.elf
# [ ] Çalıştı
# [ ] Nested struct erişimi doğru
# [ ] Method calls doğru
```

### Expected Output
```
Point: x=10 y=20
Point: x=5 y=15
Color: r=255 g=128 b=64
Rectangle area: 5000
Rectangle perimeter: 300
Pixel pos: x=100 y=200
Pixel color: r=255 g=0 b=0
```

**Verification:** [ ] ✅ PASS / [ ] ❌ FAIL / [ ] ⚠️ PARTIAL

---

## 📊 GENEL KONTROL TABLOSU

| Görev | Başlık | File | Compile | Run | Output | Status |
|-------|--------|------|---------|-----|--------|--------|
| 11 | String Interp. | [ ] | [ ] | [ ] | [ ] | [ ] |
| 12 | Nested Func. | [ ] | [ ] | [ ] | [ ] | [ ] |
| 13 | Higher Order | [ ] | [ ] | [ ] | [ ] | [ ] |
| 14 | Lambda | [ ] | [ ] | [ ] | [ ] | [ ] |
| 15 | Struct | [ ] | [ ] | [ ] | [ ] | [ ] |

---

## 🔧 OTOMATIK TEST SCRIPT

```bash
# Tüm görevleri test et
cd /home/pardus/projeler/MLP/LLVM/stage1/examples/base
bash RUN_TASKS_11_15.sh

# Beklenen output: Tüm görevler için compile + run sonuçları
```

**Script Test Sonucu:** [ ] ✅ PASS / [ ] ❌ FAIL

---

## 📝 DÖKÜMENTASYON KONTROL

- [ ] YZ_02_SUMMARY.md — Tüm görevlerin özeti (okundu)
- [ ] TASKS_11_15_REPORT.md — Detaylı rapor (okundu)
- [ ] INDEX_YZ_02.md — Dosya dizini (okundu)
- [ ] TASK_VERIFICATION.md — Checklist (kontrol edildi)
- [ ] README_YZ_02.md — Başlangıç rehberi (okundu)
- [ ] SUGAR_SYNTAX.md — Güncellendi (kontrol edildi)

---

## 🐛 BUG RAPORLARI

### Bug 1: FUNCREF Missing
- **Görev:** 13
- **Status:** 🔴 TODO (Phase 3)
- **Belge:** TASKS_11_15_REPORT.md, line 126-145
- [ ] Doğrulandı
- [ ] Workaround test edildi

### Bug 2: end_lambda Missing
- **Görev:** 14
- **Status:** ❌ TODO (Normalizer)
- **Belge:** TASKS_11_15_REPORT.md, line 168-180
- [ ] Doğrulandı
- [ ] Workaround test edildi

### Bug 3: String Fields Risk
- **Görev:** 15
- **Status:** ⚠️ Known (B1 bug)
- **Belge:** TASKS_11_15_REPORT.md, line 217-230
- [ ] Doğrulandı
- [ ] Workaround uygulandı (numeric only)

---

## ✅ FİNAL APPROVAL

### Gerekli Kontroller

- [ ] Tüm 5 görev dosyası kontrol edildi
- [ ] Tüm compile testleri yapıldı
- [ ] Tüm run testleri yapıldı
- [ ] Tüm output'lar doğrulandı
- [ ] Dökümentasyon okunundu
- [ ] Bug raporları incelendi
- [ ] Syntax kuralları kontrol edildi

### Approval Decision

**Genel Durum:**
- [ ] ✅ APPROVED — Tüm görevler geçti
- [ ] ⚠️ APPROVED WITH NOTES — Görev 13 expected to fail
- [ ] ❌ REJECTED — Kritik hata var

**Notlar:**
```
[YÖNETİCİ NOTU - BURAYA YAZILACAK]
```

**Onayan Kişi:** ________________
**Tarih:** ________________

---

## 📋 SONRAKI ADIMLAR

### Eğer ✅ APPROVED:
- [ ] Seviye 3 görevlerine geçiş
- [ ] Feature implementation planlanacak
- [ ] FUNCREF implementation başlanacak
- [ ] end_lambda block syntax implementation

### Eğer ⚠️ APPROVED WITH NOTES:
- [ ] Görev 13 tekrar gözden geçirilecek
- [ ] FUNCREF workaround iyileştirilecek
- [ ] Diğer görevler devam edecek

### Eğer ❌ REJECTED:
- [ ] Hata kaynağı belirlenmeli
- [ ] Düzeltme yapılmalı
- [ ] Yeniden test yapılmalı

---

## 📞 İLETİŞİM

**Görevli:** YZ_02 AI Assistant
**Rapor Tarihi:** 2026-03-03
**Durum:** Bekleme (ÜA onayı)

---

**Bu checklist ÜA tarafından doldurulmalıdır.**

