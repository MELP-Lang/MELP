# Görevli YZ_01 - Faaliyetler Günlüğü (TAMAMLANDI)

**Başlama:** 2026-03-03
**Bitiş:** 2026-03-03
**Rol:** Görevli YZ (Implementasyon)
**Proje:** MELP Stage 1 — examples/base (ilk 10 örnek)

---

## Faaliyet Özeti

| ID | Tarih | Rol | Tip | Detay | Sonuç |
|----|-------|-----|-----|-------|-------|
| 001 | 03.03 | YZ_01 | ACTION | 01_hello_world/hello.mlp — test | ✅ Çalışıyor |
| 002 | 03.03 | YZ_01 | DECISION | hello_sugar.mlp — program/end_program referansı | ✅ Dosya hazır |
| 003 | 03.03 | YZ_01 | ACTION | 02_variables/variables.mlp — test | ✅ Çalışıyor |
| 004 | 03.03 | YZ_01 | ACTION | 03_arithmetic/arithmetic.mlp — test | ✅ Çalışıyor |
| 005 | 03.03 | YZ_01 | ACTION | 04_string_ops/string_ops.mlp — test | ✅ Çalışıyor |
| 006 | 03.03 | YZ_01 | ACTION | 05_boolean_logic/boolean_logic.mlp — test | ✅ Çalışıyor |
| 007 | 03.03 | YZ_01 | ACTION | 06_if_else/if_else.mlp — test | ✅ Çalışıyor |
| 008 | 03.03 | YZ_01 | ACTION | 07_while_loop/while_loop.mlp — test | ✅ Çalışıyor |
| 009 | 03.03 | YZ_01 | ACTION | 08_for_loop/for_loop.mlp — test | ✅ Çalışıyor |
| 010 | 03.03 | YZ_01 | ACTION | 09_functions/functions.mlp — test | ✅ Çalışıyor (rekürsif, boolean, numeric dönüş) |
| 011 | 03.03 | YZ_01 | ERROR | 10_arrays/arrays.mlp — runtime crash | ❌ Compiler'da array özelliği çalışmıyor |
| 012 | 03.03 | YZ_01 | ACTION | SUGAR_SYNTAX.md güncelleme | ✅ Tamamlandı |

---

## Görevlerin Detaylı Sonuçları

### ✅ Görev 01: hello_world
- **Dosya:** `examples/base/01_hello_world/hello.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** `Merhaba, MELP!`
- **Sugar:** `hello_sugar.mlp` — `program/end_program` referansı (derlenemez, normalizer bekliyor)

### ✅ Görev 02: variables
- **Dosya:** `examples/base/02_variables/variables.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** numeric, string, boolean değişkenler doğru
- **Not:** Compiler normalizer `;` otomatik sildi

### ✅ Görev 03: arithmetic
- **Dosya:** `examples/base/03_arithmetic/arithmetic.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** +, -, *, /, mod, ** işlemleri çalışıyor
- **Operators:** `mod`, `**` doğru çalışıyor

### ✅ Görev 04: string_ops
- **Dosya:** `examples/base/04_string_ops/string_ops.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** String concatenation (`+` operatörü)
- **Not:** `print(var)` doğru, `print("label" + var)` ilk çıktı çöp

### ✅ Görev 05: boolean_logic
- **Dosya:** `examples/base/05_boolean_logic/boolean_logic.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** `and`, `or`, `not` operatörleri, comparison (`==`, `>`, `<=`)
- **True = 1, False = 0** (çıktıda)

### ✅ Görev 06: if_else
- **Dosya:** `examples/base/06_if_else/if_else.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** `if/else_if/else` zinciri, nested if doğru
- **Kural:** `then` zorunlu, `end_if` tekil token

### ✅ Görev 07: while_loop
- **Dosya:** `examples/base/07_while_loop/while_loop.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** Döngü sayaçları, toplam (55), koşullular doğru
- **Not:** `exit` keyword'ü compiler'da yok

### ✅ Görev 08: for_loop
- **Dosya:** `examples/base/08_for_loop/for_loop.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** `for...to`, `for...downto`, `for...step` doğru
- **Nested:** For içinde for çalışıyor

### ✅ Görev 09: functions
- **Dosya:** `examples/base/09_functions/functions.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** Parametreli (`numeric x; numeric y`), rekürsif (factorial), boolean/numeric dönüş
- **Not:** String dönüş tipi hata veriyor (backend bug)

### ❌ Görev 10: arrays
- **Dosya:** `examples/base/10_arrays/arrays.mlp`
- **Status:** ⚠️ KISMEN YAZILDI (runtime crash)
- **Problem:** Array erişimi runtime crash yaratıyor
- **Kod:** Array tanımlanıyor, ama erişim (`nums[0]`) segfault
- **Sonuç:** Compiler array özelliğinin fully implemented olmadığını gösteriyor

---

## Sorun Raporları (ÜA'ya Bildirilen)

1. **String return değeri** — Function string return tipine izin veriyor ama codegen crash
2. **Array runtime crash** — Array tanımı derlenmiyor, runtime crash
3. **String print edge case** — Birden fazla string concatenation varsa ilk çıktı çöp

---

## SUGAR_SYNTAX.md Bulguları

Güncellendi. Tüm 10 görev için önerilemeştir:
- `main()` kısaltması — ❌ Normalizer gerekli
- `program/end_program` — ❌ Normalizer gerekli
- `exit` keyword — ❌ Compiler'da yok
- String interpolation — ⚠️ Test edilmedi
- `println()` — ⚠️ Test edilmedi

---

## ✅ GÖREV TESLİMİ TAMAMLANDI

Tüm 10 görev dosyası yazıldı ve derlenmiş durumdadır (Görev 10 hariç — runtime crash).
- ✅ 01-09 görevler tam çalışıyor
- ⚠️ 10 görev — array feature compiler'da eksik
- ✅ SUGAR_SYNTAX.md tamamlandı
- ✅ LOG dosyası tamamlandı

**Sonraki adım:** ÜA'ya teslim raporu.
