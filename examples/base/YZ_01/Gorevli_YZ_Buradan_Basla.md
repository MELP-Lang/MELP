# Görevli YZ_01 — Buradan Başla

**Rol:** Görevli YZ
**Numara:** YZ_01
**Proje:** MELP Stage 1 — examples/base
**Tarih:** 2026-03-02
**ÜA:** Claude Sonnet (bu konuşmada)

---

## 🎯 GÖREVIN

MELP dilinin dünyaya açılacağı ilk örnekleri yaz.
Her örnek için **sugar syntax** öner, uygula, derle, test et, doğrula.

---

## 📚 ÖNCE OKU

Her görev başlamadan önce **sadece o görevin feature card'larını** oku.
Feature card dizini: `/home/pardus/projeler/MLP/LLVM/stage0/v2/belgeler/language/feature_cards/`

Genel kitap veya uzun belgeler okuma — gereksiz context. Feature card'lar tek kaynak.

---

## 🔧 ARAÇLAR

**Compiler:**
```bash
MELP=/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler

# Derle:
$MELP input.mlp -o output.ll

# Derle + çalıştır:
$MELP input.mlp -o /tmp/out.ll && clang -O2 -x ir /tmp/out.ll -o /tmp/out.elf && /tmp/out.elf
```

**Çıktı dizini:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/`

---

## 📋 GÖREV LİSTESİ (Seviye 1 — Temel)

Her görev için:
1. Feature card'ı oku
2. pmlp canonical syntax'ı öğren
3. Sugar syntax öner (SUGAR_SYNTAX.md'ye kaydet)
4. `.mlp` dosyasını yaz
5. Derle → çalıştır → çıktıyı doğrula
6. LOG'a kaydet

---

### GÖREV 01 — hello_world

**Dizin:** `examples/base/01_hello_world/`
**Feature Cards:** `FUNCTION_DEF.md`, `PRINT.md`, `BASIC_TYPES.md`
**Beklenen çıktı:** `Merhaba, MELP!`

**Yapılacaklar:**
- [ ] `hello.mlp` — canonical pmlp syntax ile yaz, derle, çalıştır
- [ ] `hello_sugar.mlp` — sugar syntax öner ve uygula (eğer compiler destekliyorsa)
- [ ] İkisi de aynı çıktıyı vermeli

**Sugar syntax ipucu:** `main()` kısaltması, `->` yerine `as`, `print` ile string interpolation araştır.

---

### GÖREV 02 — variables

**Dizin:** `examples/base/02_variables/`
**Feature Cards:** `VARIABLE.md`, `BASIC_TYPES.md`, `ASSIGNMENT.md`
**Beklenen çıktı:** Tüm tip tanımlamalarının doğru çalıştığı gösterilmeli

**Yapılacaklar:**
- [ ] `variables.mlp` — numeric, string, boolean tanımla, ekrana bas
- [ ] Türkçe ondalık formatını test et (3,14)
- [ ] Atama kurallarını doğrula (ilk tanımda `;`, sonrakinde yok)

---

### GÖREV 03 — arithmetic

**Dizin:** `examples/base/03_arithmetic/`
**Feature Cards:** `OPERATORS.md`, `BASIC_TYPES.md`
**Beklenen çıktı:** Dört işlem, mod, üs sonuçları

**Yapılacaklar:**
- [ ] `arithmetic.mlp` — +, -, *, /, mod, üs işlemleri
- [ ] Her işlemin sonucunu print et
- [ ] Ondalık sayılarla da test et

---

### GÖREV 04 — string_ops

**Dizin:** `examples/base/04_string_ops/`
**Feature Cards:** `STRING_CONCAT.md`, `STR_BUILTIN.md`, `STRING_INTERPOLATION.md`
**Beklenen çıktı:** Birleştirme, uzunluk, substring sonuçları

**Yapılacaklar:**
- [ ] `string_ops.mlp` — string işlemlerini göster
- [ ] `str()` builtin'i test et (numeric → string dönüşümü)
- [ ] String interpolation destekleniyorsa örnekle

---

### GÖREV 05 — boolean_logic

**Dizin:** `examples/base/05_boolean_logic/`
**Feature Cards:** `BOOLEAN_OPS.md`, `COMPARISON.md`
**Beklenen çıktı:** and/or/not/karşılaştırma sonuçları

**Yapılacaklar:**
- [ ] `boolean_logic.mlp` — tüm boolean operatörleri örnekle

---

### GÖREV 06 — if_else

**Dizin:** `examples/base/06_if_else/`
**Feature Cards:** `IF_STATEMENT.md`, `ELSE_IF.md`
**Beklenen çıktı:** Koşullara göre farklı mesajlar

**Yapılacaklar:**
- [ ] `if_else.mlp` — if / else_if / else zinciri
- [ ] Nested if örneği
- [ ] `then` zorunluluğunu doğrula

---

### GÖREV 07 — while_loop

**Dizin:** `examples/base/07_while_loop/`
**Feature Cards:** `WHILE_LOOP.md`
**Beklenen çıktı:** 1'den 10'a kadar sayılar

**Yapılacaklar:**
- [ ] `while_loop.mlp` — sayaç, toplam
- [ ] `exit` ile erken çıkış örneği (`break` YOKTUR!)
- [ ] `do` kullanılmaz, doğrula

---

### GÖREV 08 — for_loop

**Dizin:** `examples/base/08_for_loop/`
**Feature Cards:** `FOR_LOOP.md`, `FOR_EACH.md`, `RANGE.md`
**Beklenen çıktı:** Farklı adımlarla döngü çıktıları

**Yapılacaklar:**
- [ ] `for_loop.mlp` — to, downto, step örnekleri

---

### GÖREV 09 — functions

**Dizin:** `examples/base/09_functions/`
**Feature Cards:** `FUNCTION_DEF.md`, `RETURN.md`
**Beklenen çıktı:** Fonksiyon çağrıları doğru değerleri döndürmeli

**Yapılacaklar:**
- [ ] `functions.mlp` — parametreli, dönüş değerli, rekürsif fonksiyonlar
- [ ] Parametre ayracının `;` olduğunu doğrula

---

### GÖREV 10 — arrays

**Dizin:** `examples/base/10_arrays/`
**Feature Cards:** `ARRAY.md`
**Beklenen çıktı:** Dizi elemanlarına erişim ve döngüyle gezme

**Yapılacaklar:**
- [ ] `arrays.mlp` — tanımlama, erişim, döngüyle gezme

---

## 📄 SUGAR_SYNTAX.md

Her görevde keşfettiğin sugar syntax önerilerini şu dosyaya kaydet:

`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/SUGAR_SYNTAX.md`

Format:
```
## [özellik adı]
**pmlp canonical:**
[mevcut syntax]

**Önerilen sugar:**
[yeni syntax]

**Durum:** ✅ Compiler destekliyor / ❌ Normalizer gerektirir / ⚠️ Test edilmedi
```

---

## 📊 LOG DOSYASI

`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/YZ_01/YZ_01_LOG.md`

| ID | Tarih | Rol | Tip | Detay | Sonuç |
|----|-------|-----|-----|-------|-------|

---

## 🚨 SORUN DURUMU

Takıldın mı? → **ÜA'ya bildir, bekle. Kendin çözme.**

Workaround yapma. Derleyemediğin bir özellik varsa:
1. Ne denediğini yaz
2. Hata mesajını kaydet
3. ÜA'ya ilet

---

## ✅ GÖREV TAMAMLAMA KRİTERLERİ

Her görev için:
- [ ] `.mlp` dosyası yazıldı
- [ ] `melp_compiler` ile hatasız derlendi
- [ ] Binary çalıştırıldı, çıktı beklenenle eşleşti
- [ ] LOG'a kaydedildi
- [ ] Sugar syntax önerisi SUGAR_SYNTAX.md'ye eklendi

Tüm 10 görev tamamlandığında → ÜA'ya rapor ver.
