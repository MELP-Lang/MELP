# Görevli YZ_04 — Buradan Başla

**Rol:** Görevli YZ
**Numara:** YZ_04
**Proje:** MELP Stage 1 — examples/base
**Tarih:** 2026-03-03
**ÜA:** Claude Sonnet (bu konuşmada)

---

## 🎯 GÖREVIN

Seviye 3 örneklerini tamamla: interfaces, generics, pipe, closures, defer, file_io, modules, operator_overload, async_await, channels.
Her görev için `.mlp` dosyası yaz, derle, çalıştır, çıktıyı doğrula.

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

### GÖREV 21 — interfaces

**Dizin:** `examples/base/21_interfaces/`
**Feature Cards:** `INTERFACES.md`
**Beklenen çıktı:** Interface tanımı, implementasyon, polimorfizm

---

### GÖREV 22 — generics

**Dizin:** `examples/base/22_generics/`
**Feature Cards:** `GENERICS.md`
**Beklenen çıktı:** Generic fonksiyon/struct, tip parametresi

---

### GÖREV 23 — pipe_operator

**Dizin:** `examples/base/23_pipe_operator/`
**Feature Cards:** `PIPE_OPERATOR.md`
**Beklenen çıktı:** `|>` zinciri ile veri dönüşüm pipeline

---

### GÖREV 24 — closures

**Dizin:** `examples/base/24_closures/`
**Feature Cards:** `CLOSURE.md`
**Beklenen çıktı:** Capture, memoize, counter factory

---

### GÖREV 25 — defer

**Dizin:** `examples/base/25_defer/`
**Feature Cards:** `DEFER.md`
**Beklenen çıktı:** Kaynak temizleme, hata durumunda garanti

---

### GÖREV 26 — file_io

**Dizin:** `examples/base/26_file_io/`
**Feature Cards:** `FILE_IO.md`
**Beklenen çıktı:** Dosya okuma, yazma, satır satır işleme

---

### GÖREV 27 — modules

**Dizin:** `examples/base/27_modules/`
**Feature Cards:** `MODULES.md`
**Beklenen çıktı:** import, export, namespace kullanımı

---

### GÖREV 28 — operator_overload

**Dizin:** `examples/base/28_operator_overload/`
**Feature Cards:** `OPERATOR_OVERLOAD.md`
**Beklenen çıktı:** Struct için `+`, `-`, `*` tanımı

---

### GÖREV 29 — async_await

**Dizin:** `examples/base/29_async_await/`
**Feature Cards:** `ASYNC_AWAIT.md`
**Beklenen çıktı:** Asenkron fonksiyon, await kullanımı

---

### GÖREV 30 — channels

**Dizin:** `examples/base/30_channels/`
**Feature Cards:** `CHANNELS.md`
**Beklenen çıktı:** Producer-consumer, mesaj geçişi

---

## 📄 SUGAR_SYNTAX.md

Yeni keşfettiğin sugar syntax önerilerini ekle:
`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/SUGAR_SYNTAX.md`

Mevcut dosyaya **Görev 21-30** başlığı altında ekle.

---

## 📊 LOG DOSYASI

`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/YZ_04/YZ_04_LOG.md`

| ID | Görev | Durum | Not |
|----|-------|-------|-----|

---

## 🚨 SORUN DURUMU

- Compiler hata verirse: ne denediğini + hata mesajını LOG'a yaz, ÜA'ya bildir
- Workaround yapma — eğer bir özellik çalışmıyorsa belgele ve bekle
- **İstisna:** B1-B4 bilinen bug'lar için workaround uygula (yukarıda belirtildi)

---

## ✅ TAMAMLAMA KRİTERLERİ

YZ_04 TAMAMLANDI sayılır:
- [ ] Görev 21-30: her biri derle + çalıştır
- [ ] LOG: YZ_04_LOG.md tamamlandı
- [ ] SUGAR_SYNTAX.md güncellendi

Tamamlandığında ÜA'ya teslim raporu gönder.
