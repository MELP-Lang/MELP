# Görevli YZ_03 — Buradan Başla

**Rol:** Görevli YZ
**Numara:** YZ_03
**Proje:** MELP Stage 1 — examples/base
**Tarih:** 2026-03-03
**ÜA:** Claude Sonnet (bu konuşmada)

---

## 🎯 GÖREVIN

Seviye 2'nin ikinci yarısını tamamla: enum, nullable, error handling, hashmap, pattern matching.
Her örnek için `.mlp` dosyası yaz, derle, çalıştır, çıktıyı doğrula.

---

## 📚 ÖNCE OKU

Her görev **başlamadan önce** sadece o görevin feature card'larını oku.
Feature card dizini: `/home/pardus/projeler/MLP/LLVM/stage0/v2/belgeler/language/feature_cards/`

Genel belgeler okuma — gereksiz context. Feature card'lar tek kaynak.

---

## 🔧 ARAÇLAR

**Compiler:**
```
/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler
```

**Derle + çalıştır:**
```bash
/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler input.mlp -o /tmp/out.ll && clang -O2 -x ir /tmp/out.ll -o /tmp/out.elf && /tmp/out.elf
```

**Çıktı dizini:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/`

---

## ⚠️ BİLİNEN KISITLAMALAR (Stage0 Bugs)

Bunları **bilmek** zorundasın — workaround'ları uygula, ÜA'ya bildirme:

| ID | Durum | Workaround |
|----|-------|------------|
| B1 | `struct { string field }` → LLVM error | struct'ta string field kullanma |
| B2 | `Token[] tokens = [t1; t2]` → parse error | numeric array only |
| B3 | struct döndüren fonksiyon → garbage value | struct return etme |
| B4 | `this.a * this.b` / `str(p.x)` / `rect.nested.x = 0` | local var kullan; flat struct tut |

---

## 📋 GÖREV LİSTESİ

Her görev için:
1. Feature card'ı oku
2. `examples/base/XX_isim/` dizini oluştur
3. `XX_isim.mlp` dosyasını yaz
4. Derle → çalıştır → çıktıyı doğrula
5. Çalışıyorsa LOG'a kaydet, çalışmıyorsa ÜA'ya bildir

---

### GÖREV 16 — enum

**Dizin:** `examples/base/16_enum/`
**Feature Cards:** `ENUM.md`, `PATTERN_MATCHING.md`
**Beklenen çıktı:** Enum değer tanımı, eşleştirme, ekrana bas

**Yapılacaklar:**
- [ ] `enum.mlp` — enum tanımla, match ile kullan
- [ ] Variant'ları test et
- [ ] Compiler desteklemiyorsa workaround veya ÜA'ya bildir

---

### GÖREV 17 — nullable

**Dizin:** `examples/base/17_nullable/`
**Feature Cards:** `NULLABLE.md`
**Beklenen çıktı:** null değer, `??` null-coalesce, null check

**Yapılacaklar:**
- [ ] `nullable.mlp` — nullable değişken, `??` operatörü
- [ ] null check örneği

---

### GÖREV 18 — error_handling

**Dizin:** `examples/base/18_error_handling/`
**Feature Cards:** `ERROR_HANDLING.md`, `ERROR_PROPAGATION.md`
**Beklenen çıktı:** Hata üretme, yakalama, propagation

**Yapılacaklar:**
- [ ] `error_handling.mlp` — Result tipi, hata dönüşümü
- [ ] `?` propagation operatörü test et

---

### GÖREV 19 — hashmap

**Dizin:** `examples/base/19_hashmap/`
**Feature Cards:** `HASHMAP.md`
**Beklenen çıktı:** Anahtar-değer ekleme, okuma, döngüyle gezme

**Yapılacaklar:**
- [ ] `hashmap.mlp` — hashmap oluştur, ekleme, erişim
- [ ] Döngü ile gezme örneği

---

### GÖREV 20 — pattern_matching

**Dizin:** `examples/base/20_pattern_matching/`
**Feature Cards:** `PATTERN_MATCHING.md`, `ENUM.md`
**Beklenen çıktı:** match/case, guard clause, enum destructuring

**Yapılacaklar:**
- [ ] `pattern_matching.mlp` — match ile çoklu pattern
- [ ] Guard clause örneği
- [ ] Enum ile birlikte kullanım

---

## 📄 SUGAR_SYNTAX.md

Yeni keşfettiğin sugar syntax önerilerini ekle:
`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/SUGAR_SYNTAX.md`

Mevcut dosyaya **Görev 16-20** başlığı altında ekle.

---

## 📊 LOG DOSYASI

`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/YZ_03/YZ_03_LOG.md`

| ID | Görev | Durum | Not |
|----|-------|-------|-----|

---

## 🚨 SORUN DURUMU

- Compiler hata verirse: ne denediğini + hata mesajını LOG'a yaz, ÜA'ya bildir
- Workaround yapma — eğer bir özellik çalışmıyorsa belgele ve bekle
- **İstisna:** B1-B4 bilinen bug'lar için workaround uygula (yukarıda belirtildi)

---

## ✅ TAMAMLAMA KRİTERLERİ

YZ_03 TAMAMLANDI sayılır:
- [ ] Görev 16: enum — derle + çalıştır
- [ ] Görev 17: nullable — derle + çalıştır
- [ ] Görev 18: error_handling — derle + çalıştır
- [ ] Görev 19: hashmap — derle + çalıştır
- [ ] Görev 20: pattern_matching — derle + çalıştır
- [ ] LOG: YZ_03_LOG.md tamamlandı
- [ ] SUGAR_SYNTAX.md güncellendi

Tamamlandığında ÜA'ya teslim raporu gönder.
