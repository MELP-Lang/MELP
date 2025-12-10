# MLP Projesi - YZ Başlangıç Rehberi

> **Yeni YZ iseniz sadece bu belgeyi okuyun. Her şey burada.**

## 🎯 Hızlı Başlangıç

1. **Kullanıcıya sorun**: "Hangi görev üzerinde çalışmamı istersiniz?"
2. **TODO.md'yi okuyun**: Öncelikli görevler burada
3. **Çalışın ve belgeleyin**

## 📋 Öncelik Sırası

1. Kullanıcının verdiği görev (en yüksek öncelik)
2. `TODO.md` içindeki görevler
3. Test kapsamını artırma

## 🏗️ Proje Yapısı (Kısa)

```
compiler/stage0/    → Ana derleyici (C)
  modules/         → Modüler bileşenler
    functions/     → Fonksiyon desteği (aktif geliştirme)
docs/              → İngilizce belgeler
docs_tr/           → Türkçe belgeler
YZ/                → YZ oturum raporları (YZ_01.md ... YZ_XX.md)
```

## ⚡ Temel Komutlar

```bash
# Derleme (functions/ dizininde)
cd /home/pardus/projeler/MLP/MLP/compiler/stage0/modules/functions
make clean && make

# Test
./functions_compiler test.mlp test.s
gcc -o test test.s -L../../../../runtime/stdlib -lmlp_stdlib -L../../../../runtime/tto -ltto_runtime -lm
LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/tto ./test
```

## 📜 Kurallar

1. **Türkçe iletişim** (kod ve teknik terimler İngilizce kalabilir)
2. **Bilgi sahibi ol** ilgilendiğin konu ne ise önce onun ile ilgili bilgiye docs_tr/language/melp_syntax.md
dosyasından ulaş. söz dizimini öğren. Kullanıcıya, şu an ki görevim (örneğin list() listeler) "melp_syntax.md" belgesinin listeler bölümünü okudum, söz dizimine hakimim. ilgili bölüm şöyle diyor, benim görevim list() içinde şunları yapmaktır, diye bilgi ver.
3. **Her YZ kendi dalına commit ve push eder** (dal adı: `*_YZ_XX` formatında, yedekler GitHub'da tutulur)
4. **Her değişikliği test edin**
5. **Belgeleri güncel tutun**


## ✅ Oturum Sonu Kontrol Listesi

Oturumu bitirmeden önce:

- [ ] `YZ/YZ_XX.md` oluştur (XX = sonraki numara)
- [ ] `TODO.md` güncelle (tamamlananları işaretle, yenileri ekle)
- [ ] Bu dosyayı güncelle (önemli bir değişiklik varsa)

## 📁 Önemli Dosyalar

| Dosya | Amaç |
|-------|------|
| `TODO.md` | Görev listesi |
| `ARCHITECTURE.md` | Proje mimarisi + Tasarım Felsefesi |
| `YZ/YZ_XX.md` | Oturum raporları |
| `compiler/stage0/modules/functions/` | Aktif geliştirme alanı |

## 🔢 Son YZ Numarası

**YZ_30** - Phase 6 tamamlandı (70%) 🎉 (Compiler Error Messages & Diagnostics)

---

## 🎯 Mevcut Durum (11 Aralık 2025)

### Tamamlanan Phase'ler:
- ✅ **Phase 1-3**: Strings, For Loops, Collections, Booleans (100%)
- ✅ **Phase 4**: While loops, For-each, Exit system (100%)
- ✅ **Phase 5**: String methods - toUpperCase, toLowerCase, trim, etc. (100%)
- ✅ **Phase 6**: Error messages, "Did you mean", Division by zero (70%)

### Sonraki Görevler (YZ_31 için):

**A) Phase 6 Tamamlama (Opsiyonel):**
- [ ] Error recovery (ilk hatadan sonra parse'a devam)
- [ ] Daha fazla parser hatasına öneri ekleme
- [ ] Warning sistemi geliştirme

**B) Yeni Özellikler (Öneri):**
- [ ] Input fonksiyonları: `input()`, `input_numeric()`
- [ ] File I/O: `read_file()`, `write_file()`
- [ ] replace(), split() string metodları

**C) Phase 7 - Optimization:**
- [ ] Constant folding: `x = 2 + 3` → `x = 5`
- [ ] Dead code elimination

### Diğer görevler için `TODO.md` bak

---

## ✅ YZ_30 Tamamlananlar (Phase 6 - Hata Sistemi)

1. **Enhanced Error Module** ✅
   - `error.h` ve `error.c` tamamen yeniden yazıldı
   - Renkli çıktı desteği (terminal auto-detect)
   - `error_set_source()` ile kaynak kod gösterimi
   - `error_report_with_suggestion()` ile "Did you mean" önerileri

2. **Levenshtein Distance** ✅
   - `error_levenshtein_distance()` fonksiyonu
   - `error_find_similar()` ile benzer keyword bulma
   - `MELP_KEYWORDS[]` dizisi ile öneri havuzu

3. **Runtime Error Handling** ✅
   - `mlp_runtime_error()` fonksiyonu eklendi
   - Division by zero kontrolü (codegen seviyesinde)
   - Modulo by zero kontrolü
   - Exit code 43 for runtime errors

---

## 📊 Proje Özeti

| Phase | Durum | Tamamlanma |
|-------|-------|------------|
| Phase 1-3 | ✅ | 100% |
| Phase 4 | ✅ | 100% |
| Phase 5 | ✅ | 100% |
| Phase 6 | ✅ | 70% |
| Phase 7+ | ⏳ | 0% |

**Toplam**: ~99% core language features complete!

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*
